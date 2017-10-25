#include "netapp.h"
#include "DataDecoderLobby.h"
#include "DataDecoderRoom.h"
#include "DataDecoderAuth.h"
#include "DataDecoderChannellist.h"
#include "wrap/crypt.h"
#include <stdarg.h>
#include <math.h>

float g_Version = 1.00;

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif//max
#ifndef min
#define min(a,b) \
(((a)>(b))?(b):(a))
#endif//min

NetApp g_NetApp;

void KeepLiveLobbyAndRoom()
{
	//心跳包时间计数
	static int time_old = 0;
	static int time_new = 0;
	time_new = (int)time(NULL);
	if( 15 < (time_new-time_old) )
	{
		time_old = time_new;
		//业务
		if( g_NetApp.GetRoomTunnel()->IsTokenLogin() )
		{
			//Room服务器 心跳包
			g_NetApp.PostMessage( (int)VATYPE_ROOMKEEPALIVE, NULL );
			g_NetApp.GetInformer()->Inform();
		}

		if( g_NetApp.GetLobbyTunnel()->IsTokenLogin() )
		{
			//Lobby服务器 心跳包
			g_NetApp.PostMessage( (int)VATYPE_LOBBYKEEPALIVE, NULL );
			g_NetApp.GetInformer()->Inform();
		}
	}
}

NetApp::NetApp() 
	:m_LobbyTunnel(&m_Reactor),
	m_RoomTunnel(&m_Reactor),
	m_AuthTunnel(&m_Reactor),
	m_ChannellistTunnel(&m_Reactor),
	m_RDMap(&m_Reactor),
	m_gHttpUCB(&m_Reactor),
	#ifdef TEST_UDP
	m_Informer(&m_Reactor),
	#endif
	m_bIsAuth(false),
	m_iMyIDx( 0 ),
	m_pResponse(NULL),
	m_nThreadId(0),
	m_pCharsetCodecGBK(NULL),
    m_pCharsetCodecUTF8(NULL),
    m_Tokenlen(0)
{
	//注册空闲处理Http的回调事件。
	m_gHttpUCB.registerIdle();

	m_Reactor.m_funKeepLive = &KeepLiveLobbyAndRoom;

	m_pObjectCS = CriticalSectionWrapper::CreateCriticalSection();
	assert(m_pObjectCS != NULL);

	m_pCharsetCodecGBK = CCharsetCodec::codecForName(CCharsetCodec::GBK);
    m_pCharsetCodecUTF8 = CCharsetCodec::codecForName(CCharsetCodec::UTF_8);
}
NetApp::~NetApp()
{
    if(m_pCharsetCodecUTF8)
        delete m_pCharsetCodecUTF8;
	if (m_pCharsetCodecGBK)
		delete m_pCharsetCodecGBK;
	if(m_pObjectCS)
		delete m_pObjectCS;
	//注销空闲处理Http的回调事件。
	m_gHttpUCB.unRegisterIdle();
}

int SendToSvr(NetworkUtil::ClientSocket* pSvr, NetworkUtil::BinaryWriteStream* stream)
{
	if(!pSvr->IsConnect())
	{
		return -1;
	}

	return pSvr->SendBuf(*stream) ? 0 : -1;
}

void NetApp::SetToken(const char* token,unsigned int tokenlen)
{
	m_bIsAuth = true;
	memset(m_Token,0,sizeof(m_Token));
	int len = min(sizeof(m_Token)-1,tokenlen);
	memcpy(m_Token,token,len);
	m_Tokenlen = len;
}

bool NetApp::Start()
{
	m_bIsAuth = false;
	int id = 0;

	m_AuthTunnel.setTimerID(id++);
	m_AuthTunnel.setDecoder(AuthDataDecoder::Instance());

	m_LobbyTunnel.setTimerID(id++);
	m_LobbyTunnel.setDecoder(LobbyDataDecoder::Instance());

	m_RoomTunnel.setTimerID(id++);
	m_RoomTunnel.setDecoder(RoomDataDecoder::Instance());

	m_ChannellistTunnel.setTimerID(id++);
	m_ChannellistTunnel.setDecoder(ChannellistDataDecoder::Instance());

	m_RDMap.setTimerID(id++);
	m_RDMap.registerTimer(3);

    if(!m_gMgr.initDownload())
        return false;
    
	if(m_Informer.Init() != 0)
		return false;
	return m_Reactor.run();
}

void NetApp::Stop()
{
	m_Reactor.stop();
	m_Informer.UnInit();
}


int NetApp::PostMessage(int type, void *v, int wseq)
{
	CriticalSectionScoped lock(m_pObjectCS);
	if(m_requestlist.size() > 1000)//请求队列最多1000
		return -1;
    
	MSGINFO msg = {0};
	msg.type = type;
	msg.v = v;
	msg.wseq = wseq;
	m_requestlist.push_back(msg);
	return 0;
}
int NetApp::GetMessage(MSGINFO& msg)
{
	CriticalSectionScoped lock(m_pObjectCS);
	if (!m_requestlist.empty())
	{
		msg = m_requestlist.front();
		m_requestlist.pop_front();
		return 0;
	}
	return -1;
}

