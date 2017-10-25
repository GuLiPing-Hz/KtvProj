#ifndef NETAPP__H__
#define NETAPP__H__

#include "wrap/Reactor.h"
#include "wrap/Counter.h"
#include "ReserveData.h"
#include "wrap/SeqMap.h"
#include "wrap/NUtility.h"
#include "ThreadInformer.h"
#include "TMSeqMap.h"
#include "XingguangDef.h"
#include "ResponseBase.h"
#include <assert.h>
#include "Tunnel.h"
#include "charset/CharsetCodec.h"
#include "wrap/HttpDownloadMgr.h"
#include "HttpUniformCallBack.h"

extern float g_Version;

int SendToSvr(NetworkUtil::ClientSocket* pSvr,NetworkUtil::BinaryWriteStream* stream);

class NetApp
{
public:
	typedef struct _MSGINFO{
		int type;
		void* v;
		int wseq;

		_MSGINFO& operator=(const _MSGINFO& other){
			if(this != &other)
			{
				this->type = other.type;
				this->v = other.v;
				this->wseq = other.wseq;
			}
			return *this;
		}
	}MSGINFO;
public:
	NetApp();
	virtual ~NetApp();

	bool				Start();
	void				Stop();

	LobbyTunnel*        GetLobbyTunnel(){return &m_LobbyTunnel;}
	RoomTunnel*			GetRoomTunnel(){return &m_RoomTunnel;}
	AuthTunnel*			GetAuthTunnel(){return &m_AuthTunnel;}
	ChannellistTunnel*  GetChannellistTunnel(){return &m_ChannellistTunnel;}
	Counter*			GetCounter(){return &m_Counter;}

	void				SetToken(const char* token,unsigned int tokenlen);
	const char*			GetToken(){return m_Token;}
	unsigned int		GetTokenlen(){return m_Tokenlen;}

	void				SetMyIDx( int idx ){ m_iMyIDx = idx; }
	const int			GetMyIDx() { return m_iMyIDx; }

	TMSeqMap*			GetRDMap(){return &m_RDMap;}
	ThreadInformer*		GetInformer(){return &m_Informer;};

	void				SetResponseHandler(ResponseBase* pRb){m_pResponse=pRb;}
	ResponseBase*		GetResponseHandler(){return m_pResponse;}

	//处理消息的发送与接收
	int PostMessage(int type, void *v, int wseq=0);
	int GetMessage(MSGINFO& msg);

	inline bool IsAuth(){return m_bIsAuth;}
private:
	CriticalSectionWrapper*		m_pObjectCS;
	NetworkUtil::NetReactor		m_Reactor;
	LobbyTunnel							m_LobbyTunnel;
	RoomTunnel							m_RoomTunnel;
	AuthTunnel							    m_AuthTunnel;
	ChannellistTunnel					m_ChannellistTunnel;
	Counter								    m_Counter;
	char								            m_Token[256];
	int									        m_iMyIDx;
	unsigned int						    m_Tokenlen;
	TMSeqMap							    m_RDMap;
	ThreadInformer						m_Informer;
	std::list<MSGINFO>              m_requestlist;
	std::list< int >					    m_wseq;
	bool								        m_bIsAuth;
	ResponseBase*						m_pResponse;	//回调响应基类
public:
	unsigned int						                m_nThreadId;
	CCharsetCodec*						            m_pCharsetCodecGBK;
    CCharsetCodec*                                 m_pCharsetCodecUTF8;
    NetworkUtil::CHttpDownloadMgr    m_gMgr;
    HttpUniformCallBack							m_gHttpUCB;
};

extern NetApp g_NetApp;

class RDGuard
{
public:
	RDGuard(ReserveData* pRD,int seq):m_pRD(pRD),m_seq(seq){}
	~RDGuard()
	{
		free(m_pRD);
		g_NetApp.GetRDMap()->Del(m_seq);
	}
private:
	ReserveData*		m_pRD;
	int					m_seq;
};

#endif//NETAPP__H__
