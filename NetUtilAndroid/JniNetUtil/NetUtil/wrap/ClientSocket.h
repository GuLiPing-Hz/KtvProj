#ifndef CLIENTSOCKET__H__
#define CLIENTSOCKET__H__
/*
	注释添加以及修改于 2014-4-3

	UdpSocket接口类封装一个监听本地UDP的接口类，OnFDRead（FD可读时）需要自己实现

	ClientSocketBase接口类封装对FD的读写事件，处理网络业务，分发给各个事件
	//连接成功
	OnSocketConnect()
	// 连接超时
	OnSocketConnectTimeout()
	// 正常关闭(被动关闭),recv == 0的情况
	OnSocketClose()
	// errcode为错误码(socket提供)
	OnSocketConnectError(int errCode) //连接错误
	OnSocketRecvError(int errCode) //接受错误
	OnSocketSendError(int errCode) //发送错误
	// 网络层错误(errCode网络层定义)
	OnNetLevelError(int errCode)

	ClientSocket类在ClientSocketBase的基础上又加入网络连接Connect的一些属性
	Connect的时候生成一个网络描述符，并设置为非阻塞，注册到写fd_set中，并注册超时时间，
	来对超时进行处理，第一次调用OnFDWrite的时候即为跟服务器成功连接，注销在Connect中注册的东西，并注册到写fd_set中.
*/

#include "eventhandler.h"
#include "datablock.h"
#include "RWStream.h"
#include "NUtility.h"

namespace NetworkUtil
{
	enum _Errorcode{
		//网络接受Buf异常
		EC_RECV_BUFFER,
		//数据流异常
		EC_STREAM,
	};

	class DataBlock;
	class DataDecoderBase;

	class UdpSocket : public FDEventHandler
	{
	public:
		virtual ~UdpSocket(){}
		UdpSocket():m_port(){}
		UdpSocket(Reactor *pReactor,const char* host,short port) : FDEventHandler(pReactor),m_port(port)
		{
			strncpy(m_host,host,sizeof(m_host));
		}
		virtual void onFDWrite(){}
		virtual int Init()
		{
			m_fd = socket(AF_INET,SOCK_DGRAM,0);
			if(m_fd == INVALID_SOCKET)
				return -1;

			int i = 100;
			while(i-- > 0)
			{
				struct sockaddr_in local={0};
				local.sin_family = AF_INET;
				local.sin_port = htons(m_port); ///监听端口
				local.sin_addr.s_addr = inet_addr("127.0.0.1"); ///本机
				if(bind(m_fd,(struct sockaddr*)&local,sizeof(local)) == SOCKET_ERROR)
				{
					m_port++;
					continue;
				}
				else
					break;
			}
			if(i <= 0)
				return -1;

			registerRead();
			return 0;
		}
		short Getport(){
			return m_port;
		}
	private:
		short m_port;
		char m_host[20];
	};

	//////////////////////////////////////////////////////////////////////////

	class ClientSocketBase : public FDEventHandler
	{
	public:
		virtual ~ClientSocketBase(){if(m_pCSSendData)delete m_pCSSendData;}
        
		ClientSocketBase() : m_pDecoder(NULL),m_pCSSendData(NULL),m_bIsClosed(true){
            m_pCSSendData = CriticalSectionWrapper::CreateCriticalSection();
        }
		ClientSocketBase(Reactor *pReactor) : FDEventHandler(pReactor),m_pDecoder(NULL),m_bIsClosed(true){
            m_pCSSendData = CriticalSectionWrapper::CreateCriticalSection();
        }
		void setDecoder(DataDecoderBase* pDecoder){m_pDecoder = pDecoder;}
		//网络可读的时候，recv数据
		virtual void onFDRead();
		//网络可写的时候，send数据
		virtual void onFDWrite();
		//关闭处理
		virtual void closeSocket();

		inline bool isClosed(){return m_bIsClosed;}
		DataBlock* getRB(){return &m_recvdata;}
		DataBlock* getWB(){return &m_senddata;}
		//add buffer的时候注册写fd_set，这样可以被执行到OnFDWrite();
		virtual int addBuf(const char* buf,unsigned int buflen);
		char* getPeerIp();

		//从域名中解析出Ip地址,只返回第一个解析出来的,字符串保存在静态空间中，返回值不需要释放！
		static const char* GetIpv4FromHostname(const char* name);
	protected:
		void open(){m_bIsClosed = false;}
	public:
		// 连接成功
		virtual bool onSocketConnect() = 0; 
		// 连接超时
		virtual void onSocketConnectTimeout() = 0;
		// 正常关闭(被动关闭),recv == 0的情况
		virtual void onSocketClose() = 0;
		// errcode为错误码(socket提供)
		virtual void onSocketConnectError(int errCode) = 0;
		virtual void onSocketRecvError(int errCode) = 0;
		virtual void onSocketSendError(int errCode) = 0;
		// 网络层错误(errCode网络层定义)
		virtual void onNetLevelError(int errCode) = 0;

	private:
		DataBlock m_recvdata;
		DataBlock m_senddata;
		DataDecoderBase *m_pDecoder;
		bool m_bIsClosed;
    public:
        CriticalSectionWrapper* m_pCSSendData;
	};
	class ClientSocket : public ClientSocketBase ,public TMEventHandler
	{
	public:
		virtual ~ClientSocket(){}
		ClientSocket(Reactor *pReactor) : ClientSocketBase(pReactor),m_isConnected(false),
			m_isWaitingConnectComplete(false),m_port(0){ }

		//增加对非阻塞描述符的情况判断。
		virtual void onFDWrite();
		//连接超时
		virtual void onTimeOut();
		virtual void closeSocket();

		int Connect(const char* host,short port,int to = 10);
		bool IsConnect(){return m_isConnected;}
		//添加的sendbuf中，并注册到写fd_set中
		bool SendBuf(BinaryWriteStream &stream);
		bool SendBuf(const char* buf,unsigned int buflen);

		inline const char* Gethost(){return m_host;}
		inline short Getport(){return m_port;}
	public:
		virtual bool onSocketConnect() {return true;} 
		virtual void onSocketConnectTimeout() {}
		virtual void onSocketConnectError(int errCode) {}
		virtual void onSocketClose() {}
		virtual void onSocketRecvError(int errCode) {}
		virtual void onSocketSendError(int errCode) {}
		virtual void onNetLevelError(int errCode) {}

	private:
		bool m_isConnected;
		bool m_isWaitingConnectComplete;
		char m_host[20];
		short m_port;
	};
};
#endif//CLIENTSOCKET__H__

