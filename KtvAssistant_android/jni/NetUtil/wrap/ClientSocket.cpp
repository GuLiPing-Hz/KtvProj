#include "clientsocket.h"
#include "datadecoder.h"
#include "../ResultData.h"

#include "config.h"
#ifndef NETUTIL_ANDROID
#define LOGI printf
#define LOGE printf
#endif


namespace NetworkUtil
{
	void ClientSocketBase::onFDRead()
	{
		char buf[16384] = {0};/* 16*1024 */
		int len = ::recv(m_fd, buf, sizeof(buf), 0);//接收网络数据

		if (len == 0)
		{
			closeSocket();
			onSocketClose();
			return;
		}

		if (len == SOCKET_ERROR)
		{
#ifdef WIN32
			DWORD derrno = GetLastError();
			if (derrno != WSAEWOULDBLOCK)
			{
				closeSocket();
				onSocketRecvError(derrno);
			}
#else//Linux
            int errorcode = errno;
			if(errorcode!=EAGAIN)
			{
				closeSocket();
				onSocketRecvError(errorcode);
			}
#endif
			return;
		}

		if (!m_pDecoder)
			return;

		if (m_recvdata.append(buf,len) < 0)
		{
			closeSocket();
			onNetLevelError(Net_RecvBuf_Error);
			return;
		}
		//解析网络数据，不足一个包的部分会有相应处理
		if (m_pDecoder->process(this) < 0)
		{
			closeSocket();
			onNetLevelError(Net_Stream_Error);
			return;
		}
	}
	void ClientSocketBase::onFDWrite()
	{
        CriticalSectionScoped lock(m_pCSSendData);
		unsigned int buflen = m_senddata.getPos();
		int len = ::send(m_fd,m_senddata.getBuf(),(int)buflen,0);
		if (len == SOCKET_ERROR)
		{
#ifdef WIN32
			DWORD derrno = GetLastError();
			if( derrno != WSAEWOULDBLOCK )
			{
				closeSocket();
				onSocketSendError(derrno);
			}
#else//Linux
			if(errno != EAGAIN)
			{
				closeSocket();
				onSocketSendError(errno);
			}
#endif
			return;
		}
		if((unsigned int)len == buflen)
		{
			m_senddata.initPos();
			unRegisterWrite();
			return;
		}
		else if((unsigned int)len < buflen)
		{
			m_senddata.copy(0,m_senddata.getBuf() + len , buflen - len);
		}
	}
	int ClientSocketBase::addBuf(const char* buf,unsigned int buflen)
	{
        CriticalSectionScoped lock(m_pCSSendData);
		if(m_senddata.append(buf,buflen) != 0)
		{
            LOGE("%s : SendData Append Failed\n",__FUNCTION__);
			return -1;
		}
		if(registerWrite() != 0)
		{
            LOGE("%s : RegisterWrite Failed\n",__FUNCTION__);
			return -1;
		}
		return 0;
	}
	char* ClientSocketBase::getPeerIp()
	{
		sockaddr_in addr;
#ifdef NETUTIL_MAC
		unsigned int len = sizeof(sockaddr_in);
#else
		int len = sizeof(sockaddr_in);
#endif
		getpeername(m_fd,(struct sockaddr*)&addr,&len);
		static char ip[32];
		strncpy(ip,inet_ntoa(addr.sin_addr),sizeof(ip));
		return ip;
	}
	const char* ClientSocketBase::GetIpv4FromHostname(const char* name)
	{
		static char sIp[300] = {0};
		memset(sIp,0,sizeof(sIp));
		if(!name)
			return sIp;
		// 返回地址信息
		hostent* host = gethostbyname(name);
		if (!host)
		{
			LOGE("%s : gethostbyname return null[%s], check the internet permission or internet connect.\n",__FUNCTION__,name);
			return sIp;
		}

		// 解析地址信息
		for (char **p = host->h_addr_list; *p ; p++)
		{
			char *temp;
			temp = inet_ntoa(*(struct in_addr*)*p);
			strcpy(sIp,temp);
			break;
		}
		return sIp;
	}
	void ClientSocketBase::closeSocket()
	{
		m_bIsClosed = true;
		m_recvdata.initPos();
		m_senddata.initPos();
		FDEventHandler::closeSocket();
	}
/////////////////////////ClientSocket 实现/////////////////////////////////////////////////
	void ClientSocket::onTimeOut()
	{
		m_isWaitingConnectComplete = false;
		ClientSocketBase::closeSocket();
		onSocketConnectTimeout();
	}
	void ClientSocket::closeSocket()
	{
		if (m_isConnected || m_isWaitingConnectComplete)
		{
			m_isConnected = false;
			m_isWaitingConnectComplete = false;
			ClientSocketBase::closeSocket();
			TMEventHandler::closeSocket();
		}
	}
	
	void ClientSocket::onFDWrite()
	{
		if(m_isConnected)//如果已经连接上了
		{
			ClientSocketBase::onFDWrite();
			return;
		}
		//如果是第一次回调，则说明网络描述符可用了
		m_isConnected = true;
		m_isWaitingConnectComplete = false;
		open();
		unRegisterTimer();//注销timer，从连接超时列表中去除
		unRegisterWrite();//注销写fd_set

		onSocketConnect();
		registerRead();
	}

	int ClientSocket::Connect(const char* host,short port,int to)
	{
		//已经连接，或者正在连接
		if (m_isConnected || m_isWaitingConnectComplete)
			return 0;
		//创建网络描述符 TCP
		m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_fd == INVALID_SOCKET)
			return -1;

		strncpy(m_host, host, sizeof(m_host));
		m_port = port;
		//定义连接地址IP PORT
		sockaddr_in clientService;
		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr(host);
		clientService.sin_port = htons(port);
		//设置地址复用
//#ifdef WIN32
//		BOOL bReuseaddr = TRUE;
//		if(setsockopt( m_fd, SOL_SOCKET, SO_REUSEADDR, ( const char* )&bReuseaddr, sizeof( BOOL ) ) == SOCKET_ERROR )
//#else
//		int on=1;
//		if(setsockopt(m_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))==SOCKET_ERROR)
//#endif
//		{
//			return -1;
//		}
		//设置非阻塞
		if (setNonBlocking() != 0)
			return -1;

		int errorCode;
#ifdef WIN32
		if (connect(m_fd, (struct sockaddr*)&clientService, sizeof(clientService)) == SOCKET_ERROR 
			&& !(((errorCode = WSAGetLastError()) == WSAEWOULDBLOCK) || (errorCode==WSAEINPROGRESS)))
		{
			closesocket(m_fd);
#else
		if(connect(m_fd, (struct sockaddr*)&clientService, sizeof(clientService)) == -1 
			&& !(((errorCode=errno) == EAGAIN) || (errorCode == EINPROGRESS)))
		{
			close(m_fd);
#endif
			onSocketConnectError(errorCode);
			return -1;
		}

		//LOGI("%s : RegisterWrite wait for connected %d\n",__FUNCTION__,m_fd);
		//如果为connect 返回-1 并且errorno为 EAGAIN
		registerWrite();//注册到写fd_set中 等待 OnFDWrite回调，第一次的话就说明这个fd可用了。
		registerTimer(to);//注册超时处理
		m_isWaitingConnectComplete = true;

		return 0;
	}

	bool ClientSocket::SendBuf(BinaryWriteStream &stream)
	{
		if(addBuf(stream.getData(),stream.getSize()) != 0)
		{
			closeSocket();
			return false;
		}
		return true;
	}

	bool ClientSocket::SendBuf(const char* buf,unsigned int buflen)
	{
		if(addBuf(buf,buflen) != 0)
		{
			closeSocket();
			return false;
		}
		return true;
	}
};

