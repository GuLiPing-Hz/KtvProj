#ifndef TUNNEL__H__
#define TUNNEL__H__

#include "XingguangDef.h"
#include "wrap/ClientSocket.h"

class Tunnel : public NetworkUtil::ClientSocket
{
public:
	virtual ~Tunnel(){}
	Tunnel(NetworkUtil::Reactor *pReactor) : NetworkUtil::ClientSocket(pReactor)
	{
		m_bIsTokenLogin = false;
		memset(m_Sessionkey, 0, sizeof(m_Sessionkey));
	}

	bool SendRightBuf(NetworkUtil::BinaryWriteStream &stream);
	bool SendTEABuf(NetworkUtil::BinaryWriteStream &stream);
	bool SendCompressBuf(NetworkUtil::BinaryWriteStream &stream);
	virtual void closeSocket();
	inline bool IsTokenLogin(){
		return m_bIsTokenLogin;
	}
	inline void SetTokenLogin(bool bIsTokenLogin){
		m_bIsTokenLogin = bIsTokenLogin;
	}

public:
	virtual void onSocketConnect();
	virtual void onSocketConnectTimeout();
	virtual void onSocketConnectError(int errCode);
	virtual void onSocketClose();
	virtual void onSocketRecvError(int errCode);
	virtual void onSocketSendError(int errCode);
	virtual void onNetLevelError(int errCode);

public:
	bool m_bIsTokenLogin;
	char m_Sessionkey[16];
};

class RoomTunnel: public Tunnel
{
public:
	RoomTunnel(NetworkUtil::Reactor * pReactor):Tunnel(pReactor), m_connect_type(0){}

public:
	int m_connect_type;
};

typedef Tunnel LobbyTunnel;
typedef Tunnel AuthTunnel;
typedef Tunnel ChannellistTunnel;

#endif
