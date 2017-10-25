#include "Tunnel.h"
#include "wrap/crypt.h"
#include "wrap/RWStream.h"
#include "NetApp.h"

#include "wrap/config.h"

#ifndef NETUTIL_ANDROID
#define LOGI printf
#endif

void Tunnel::closeSocket()
{
	////TRACE_INFO((Info, "close socket, fd=%d, this=%x", GetFD(), this);
	m_bIsTokenLogin = false;
	memset(m_Sessionkey,0,sizeof(m_Sessionkey));
	ClientSocket::closeSocket();
}

bool Tunnel::SendRightBuf(NetworkUtil::BinaryWriteStream &stream)
{
	if(stream.getSize() < MAXBUFSIZETEA)
		return SendTEABuf(stream);
	return SendCompressBuf(stream);
}

bool Tunnel::SendTEABuf(NetworkUtil::BinaryWriteStream &stream)
{
	char outbuf[65535];                     
	int outbuflen = sizeof(outbuf);                 
	if(NetworkUtil::StreamEncrypt(stream.getData(),(int)stream.getSize(),outbuf,outbuflen,m_Sessionkey,1))
	{
		if(!IsConnect())
		{
			return false;
		}

		return SendBuf(outbuf,outbuflen);                                    
	}
	return false;
}

bool Tunnel::SendCompressBuf(NetworkUtil::BinaryWriteStream &stream)
{
	char outbuf[65535];                     
	int outbuflen = sizeof(outbuf);                 
	if(NetworkUtil::StreamCompress(stream.getData(),(int)stream.getSize(),outbuf,outbuflen))
	{
		if(!IsConnect())
			return false;

		return SendBuf(outbuf,outbuflen);                                     
	}
	return false;
}

void Tunnel::onSocketConnect()
{
	if (g_NetApp.GetAuthTunnel() == this)
		g_NetApp.GetResponseHandler()->OnAuthTunnelConnectSuccess();
	else if (g_NetApp.GetLobbyTunnel() == this)
		g_NetApp.GetResponseHandler()->OnLobbyTunnelConnectSuccess();
	else if (g_NetApp.GetRoomTunnel() == this)
		g_NetApp.GetResponseHandler()->OnRoomTunnelConnectSuccess();
	else if (g_NetApp.GetChannellistTunnel() == this)
		g_NetApp.GetResponseHandler()->OnChannellistTunnelConnectSuccess();
}

void Tunnel::onSocketConnectTimeout()
{
	if (g_NetApp.GetAuthTunnel() == this)
	{
		////TRACE_INFO((Error, "connect auth timeout, fd=%d, this=%x", GetFD(), this);
		g_NetApp.GetResponseHandler()->OnAuthTunnelConnectTimeout();
	}
	else if (g_NetApp.GetLobbyTunnel() == this)
	{
		////TRACE_INFO((Error, "connect lobby timeout, fd=%d, this=%x", GetFD(), this);
		g_NetApp.GetResponseHandler()->OnLobbyTunnelConnectTimeout();
	}
	else if (g_NetApp.GetRoomTunnel() == this)
	{
		////TRACE_INFO((Error, "connect room timeout, fd=%d, this=%x", GetFD(), this);
		g_NetApp.GetResponseHandler()->OnRoomTunnelConnectTimeout();
	}
	else if (g_NetApp.GetChannellistTunnel() == this)
	{
		////TRACE_INFO((Error, "connect channel timeout, fd=%d, this=%x", GetFD(), this);
		g_NetApp.GetResponseHandler()->OnChannellistTunnelConnectTimeout();
	}
}

void Tunnel::onSocketConnectError(int errCode)
{
	if (g_NetApp.GetAuthTunnel() == this)
	{
		////TRACE_INFO((Error, "connect auth error, fd=%d, this=%x, error=%d", GetFD(), this, errCode);
		g_NetApp.GetResponseHandler()->OnAuthTunnelConnectError(errCode);
	}
	else if (g_NetApp.GetLobbyTunnel() == this)
	{
		////TRACE_INFO((Error, "connect lobby error, fd=%d, this=%x, error=%d", GetFD(), this, errCode);
		g_NetApp.GetResponseHandler()->OnLobbyTunnelConnectError(errCode);
	}
	else if (g_NetApp.GetRoomTunnel() == this)
	{
		////TRACE_INFO((Error, "connect room error, fd=%d, this=%x, error=%d", GetFD(), this, errCode);
		g_NetApp.GetResponseHandler()->OnRoomTunnelConnectError(errCode);
	}
	else if (g_NetApp.GetChannellistTunnel() == this)
	{
		////TRACE_INFO((Error, "connect channel error, fd=%d, this=%x, error=%d", GetFD(), this, errCode);
		g_NetApp.GetResponseHandler()->OnChannellistTunnelConnectError(errCode);
	}
}

void Tunnel::onSocketClose()
{
	if (g_NetApp.GetAuthTunnel() == this)
	{
		////TRACE_INFO((Info, "auth close, fd=%d, this=%x", GetFD(), this);
		g_NetApp.GetResponseHandler()->OnAuthTunnelClose();
	}
	else if (g_NetApp.GetLobbyTunnel() == this)
	{
		////TRACE_INFO((Info, "lobby close, fd=%d, this=%x", GetFD(), this);
		g_NetApp.GetResponseHandler()->OnLobbyTunnelClose();
	}
	else if (g_NetApp.GetRoomTunnel() == this)
	{
		////TRACE_INFO((Info, "room close, fd=%d, this=%x", GetFD(), this);
		g_NetApp.GetResponseHandler()->OnRoomTunnelClose();
	}
	else if (g_NetApp.GetChannellistTunnel() == this)
	{
		////TRACE_INFO((Info, "channel close, fd=%d, this=%x", GetFD(), this);
		g_NetApp.GetResponseHandler()->OnChannellistTunnelClose();
	}
}

void Tunnel::onSocketRecvError(int errCode)
{
	LOGI("recv error, fd=%d, this=%x, error=%d\n", getFD(), (unsigned int)this, errCode);
	onNetLevelError(errCode);
}

void Tunnel::onSocketSendError(int errCode)
{
	LOGI("send error, fd=%d, this=%x, error=%d\n", getFD(), (unsigned int)this, errCode);
	onNetLevelError(errCode);
}

void Tunnel::onNetLevelError(int errCode)
{
	if (g_NetApp.GetAuthTunnel() == this)
	{
		////TRACE_INFO((Error, "auth tunnel error, fd=%d, this=%x, error=%d", GetFD(), this, errCode);
		g_NetApp.GetResponseHandler()->OnAuthTunnelError(errCode);
	}
	else if (g_NetApp.GetLobbyTunnel() == this)
	{
		////TRACE_INFO((Error, "lobby tunnel error, fd=%d, this=%x, error=%d", GetFD(), this, errCode);
		g_NetApp.GetResponseHandler()->OnLobbyTunnelError(errCode);
	}
	else if (g_NetApp.GetRoomTunnel() == this)
	{
		////TRACE_INFO((Error, "room tunnel error, fd=%d, this=%x, error=%d", GetFD(), this, errCode);
		g_NetApp.GetResponseHandler()->OnRoomTunnelError(errCode);
	}
	else if (g_NetApp.GetChannellistTunnel() == this)
	{
		////TRACE_INFO((Error, "channel tunnel error, fd=%d, this=%x, error=%d", GetFD(), this, errCode);
		g_NetApp.GetResponseHandler()->OnChannellistTunnelError(errCode);
	}
}
