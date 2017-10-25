/*************************************
*请求处理类 源文件
*Create: 2009/12/21   jinguanfu 
*************************************/
#include "wrap/config.h"
#include "RequestBase.h"
#include <string.h>
#include "NetApp.h"
#include "ResultData.h"
#include "ThreadInformer.h"
#include "Tunnel.h"
#include "wrap/thread_wrapper.h"
#include "XingguangDef.h"
#include <string.h>

#ifdef NETUTIL_ANDROID

#ifndef TEST_NET
#include "../JniResponse.h"
#endif//TEST_NET

#else
 	 #ifndef LOGI
 	 	 #define LOGI printf
 	 #endif
	#ifndef LOGE
		#define LOGE printf
	#endif
#endif

ThreadWrapper* handleNetThread = NULL;
static int s_login_way = 0;

bool RunClient9158ThreadFunc(void* lpParam)
{
	return g_NetApp.Start();
}

void OnThreadStart(unsigned int threadid)
{
	//保存线程ID到g_NetApp中
	g_NetApp.m_nThreadId = threadid;
    
#ifdef CJNIRESPONSE__H__
	CJniResponse* pJniResponse = CJniResponse::getSingleton();
	pJniResponse->attachCurThread(threadid);
#endif
}

void OnTreadEnd(unsigned int threadid)
{
#ifdef CJNIRESPONSE__H__
	CJniResponse* pJniResponse = CJniResponse::getSingleton();
	pJniResponse->detachCurThread();
#endif
}

/************************************************************************/
/*			add by jinguanfu 2010/3/2 <begin> 回调API改造成接口			*/
int RequestBase::StartClient()
{
	LOGI("RequestBase::StartClient\n");
    
#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2,2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if(err != 0) 
		return -1;
#endif

	if(handleNetThread != NULL)
		return -1;

	handleNetThread = ThreadWrapper::CreateThread(RunClient9158ThreadFunc,NULL);
	handleNetThread->SetCallBack(OnThreadStart,OnTreadEnd);
	unsigned int thread_id;
	if(!handleNetThread || !handleNetThread->Start(thread_id))
		return -1;
    m_bInit = true;
	return 0;
}

void RequestBase::StopClient()
{
	g_NetApp.Stop();
	if(handleNetThread)
	{
		if(!handleNetThread->WaitFor(3000))
		{
			handleNetThread->Terminate(0);
		}
		delete handleNetThread;
		handleNetThread = NULL;
	}

#ifdef _WIN32
	WSACleanup();
#endif
}

const char* RequestBase::GetToken()
{
	return g_NetApp.GetToken();
}

unsigned int RequestBase::GetTokenlen()
{
	return g_NetApp.GetTokenlen();
}

bool RequestBase::IsConnectAuth()
{
	return g_NetApp.GetAuthTunnel()->IsConnect();
}

//大厅是否连接
bool RequestBase::IsConnectLobby()
{
	return g_NetApp.GetLobbyTunnel()->IsConnect();
}

//房间是否连接
bool RequestBase::IsConnectRoom()
{
	return g_NetApp.GetRoomTunnel()->IsConnect();
}

//大厅是否token
bool RequestBase::IsTokenLoginLobby()
{
	return g_NetApp.GetLobbyTunnel()->IsTokenLogin();
}

//房间是否token
bool RequestBase::IsTokenLoginRoom()
{
	return g_NetApp.GetRoomTunnel()->IsTokenLogin();
}

int RequestBase::ConnectAuth(const char* host,short port,int timeout)
{
    if(!m_bInit)
    {
        LOGE("NetUtil is not initialize\n");
        return -1;
    }

    const char* sIp = NetworkUtil::ClientSocketBase::GetIpv4FromHostname(host);
    if(sIp[0] == 0)
        return -1;
    
	void *mem = malloc(sizeof(VA_ConnectToSvr));
	if(!mem)
		return -1;
	VA_ConnectToSvr *pcts = new(mem) VA_ConnectToSvr;
	strncpy(pcts->host,sIp,sizeof(pcts->host));
	pcts->port = port;
	pcts->to = timeout;

	if(g_NetApp.PostMessage(VATYPE_CONNECTTOAUTH,pcts) != 0)
	{
		free(mem);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();
	return 0;
}
int RequestBase::DisConnectAuth()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(g_NetApp.PostMessage(VATYPE_CLOSEAUTH,NULL) != 0)
	{
		return -1;
	}
	g_NetApp.GetInformer()->Inform();
	return 0;
}
int RequestBase::ConnectLobby(const char* host,short port,int timeout)
{
    if(!m_bInit)
    {
        LOGE("NetUtil is not initialize\n");
        return -1;
    }
    
    const char* sIp = NetworkUtil::ClientSocketBase::GetIpv4FromHostname(host);
    if(sIp[0] == 0)
        return -1;
    
	void *mem = malloc(sizeof(VA_ConnectToSvr));
	if(!mem)
		return -1;
	VA_ConnectToSvr *pcts = new(mem) VA_ConnectToSvr;
	strncpy(pcts->host,sIp,sizeof(pcts->host));
	pcts->port = port;
	pcts->to = timeout;
	if(g_NetApp.PostMessage(VATYPE_CONNECTTOLOBBY,pcts) != 0)
	{
		free(mem);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();

	return 0;
}
int RequestBase::DisConnectLobby()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(g_NetApp.PostMessage(VATYPE_CLOSELOBBY,NULL) != 0)
	{
		return -1;
	}
	g_NetApp.GetInformer()->Inform();
	return 0;
}
int RequestBase::ConnectRoom(const char* host,short port,int type,int timeout)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	int vatype = 0;
	switch( type )
	{
	case ROOMTYPE_BASEG:
		vatype = VATYPE_CONNECTTOROOM_BASE;
		break;

	case ROOMTYPE_PRACTICEG:
		vatype = VATYPE_CONNECTTOROOM_PRACTICE;
		break;

	default:
		return -1;
	}

    const char* sIp = NetworkUtil::ClientSocketBase::GetIpv4FromHostname(host);
    LOGI("RequestBase::ConnectLobby ip=%s;port=%d\n",sIp,port);
    if(sIp[0] == 0)
        return -1;
    
	void *mem = malloc(sizeof(VA_ConnectToSvr));
	if(!mem)
		return -1;
	VA_ConnectToSvr *pcts = new(mem) VA_ConnectToSvr;
	strncpy(pcts->host,sIp,sizeof(pcts->host));
	pcts->port = port;
	pcts->to = timeout;

	if( g_NetApp.PostMessage( vatype ,pcts ) != 0 )
	{
		free(mem);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();
	return 0;
}
int RequestBase::DisConnectRoom()
{
	if(g_NetApp.PostMessage(VATYPE_CLOSEROOM,NULL) != 0)
	{
		return -1;
	}
	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::IdPassAuth(const char* id,const char* passwd,int again)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!id || !passwd)
		return -1;
	void *mem = malloc(sizeof(VA_IdPassAuth));
	if(!mem)
		return -1;
	VA_IdPassAuth *pida = new(mem) VA_IdPassAuth;
	strncpy(pida->id,id,sizeof(pida->id));
	strncpy(pida->passwd,passwd,sizeof(pida->passwd));
	pida->again = again;
	if(g_NetApp.PostMessage((int)VATYPE_IDAUTH,pida) != 0)
	{
		free(mem);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::TouristAuth(int again)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
    void* pMem = malloc(sizeof(VA_TouristAuth));
    if(!pMem)
    	return -1;
    VA_TouristAuth* pTourist = new(pMem) VA_TouristAuth;
    pTourist->again = again;
	if(g_NetApp.PostMessage((int)VATYPE_TOURISTAUTH, NULL ) != 0)
	{
		free(pMem);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::ConfirmToken()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -2;
	if(g_NetApp.PostMessage((int)VATYPE_CONFIRMTOKEN,NULL) != 0)
		return -1;
	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::EnterRoom(int cseq,int roomid, const char* passwd, short type)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!passwd)
		return -1;

	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	void *mem = malloc(sizeof(VA_ROOM));
	if(!mem)
		return -1;
	VA_ROOM *pRoom = new(mem) VA_ROOM;
	pRoom->cseq=cseq;
	pRoom->id = roomid;
	strncpy(pRoom->passwd,passwd,sizeof(pRoom->passwd));
	pRoom->type = type;

	if(g_NetApp.PostMessage((int)VATYPE_ENTERROOM,pRoom) != 0)
	{
		free(mem);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();
	return 0;
}

//取得房间当前运行信息
int RequestBase::GetRoomAllInfomation(int cseq)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int *i = (int*)malloc(sizeof(int));
	*i = cseq;

	if(g_NetApp.PostMessage((int)VATYPE_GETROOMALLINFO,i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

// 取麦序列表
int RequestBase::GetWaitMicList()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if( !g_NetApp.GetRoomTunnel()->IsTokenLogin() )
		return -1;

	if( g_NetApp.PostMessage((int) VATYPE_ROOM_GETWAITMICLIST, NULL ) !=0 )
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//排麦
int RequestBase::RoomWaitMic( int musicid, short bk, char level, const char * name, char speed )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!name)
		return -1;
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	void *mem = malloc(sizeof(VA_MIC_INFO));
	if(!mem)
		return -1;
	VA_MIC_INFO *pMicInfo = new(mem) VA_MIC_INFO;

	pMicInfo->musicid	= musicid;
	pMicInfo->bk		= bk;
	pMicInfo->level		= level;
	//pMicInfo->change	= change;
	pMicInfo->speed		= speed;
	memset(pMicInfo->name, 0, sizeof(pMicInfo->name));
	strncpy(pMicInfo->name, name, sizeof(pMicInfo->name));

	if(g_NetApp.PostMessage((int)VATYPE_ROOMWAITMIC,pMicInfo) != 0)
    {
        free(mem);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//取消排麦
int RequestBase::RoomCancleWaitMic( int idx )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	// add by cx 10-5-14
	int *i = (int*)malloc(sizeof(int));
	*i = idx;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMCANCLEWAITMIC, i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//提升麦序
int RequestBase::RoomUpWaitMic(char tag, int idx, char flag)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	void *mem = malloc(sizeof(VA_UD_MICINFO));
	if(!mem)
		return -1;
	VA_UD_MICINFO *p = new(mem) VA_UD_MICINFO;
	p->tag = tag;
	p->idx = idx;
	p->flag = flag;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMUPWAITMIC,p) != 0)
    {
        free(mem);
        return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//flag==>MIC_UP_DOWN_FLAG
//下降麦序
int RequestBase::RoomDownWaitMic(char tag, int idx, char flag)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	void *mem = malloc(sizeof(VA_UD_MICINFO));
	if(!mem)
		return -1;
	VA_UD_MICINFO *p = new(mem) VA_UD_MICINFO;
	p->tag = tag;
	p->idx = idx;
	p->flag = flag;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMDOWNWAITMIC,p) != 0)
    {
        free(mem);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//flag==>MIC_UP_DOWN_FLAG
//上麦准备ok
//int RequestBase::RoomOnMicReadyOK( short music_len )
//{
//	short * pLen = ( short * )malloc(sizeof( short ));
//	if( ! pLen )
//		return -1;
//
//	* pLen = music_len;
//
//	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
//		return -1;
//
//	if(g_NetApp.PostMessage((int)VATYPE_ROOMONMICREADYOK, pLen ) != 0)
//		return -1;
//
//	g_NetApp.GetInformer()->Inform();
//	return 0;
//}
//准备下麦
//int RequestBase::RoomOffMicReady()
//{
//	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
//		return -1;
//
//	if(g_NetApp.PostMessage((int)VATYPE_ROOMOFFMICREADY,NULL) != 0)
//		return -1;
//
//	g_NetApp.GetInformer()->Inform();
//	return 0;
//}
//把指定用户提出房间
int RequestBase::RoomKickSomeOne(int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int *i = (int*)malloc(sizeof(int));
	*i = idx;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMKICKOUTSOMEONE,i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//禁言某人（或取消禁言）
int RequestBase::RoomForbidenSomeOne(int idx, char flag)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_FORBIDENINFO *pforbideninfo;
	if(NULL == (pforbideninfo = (VA_FORBIDENINFO *)malloc(sizeof(VA_FORBIDENINFO))))
		return -1;

	pforbideninfo->idx = idx;
	pforbideninfo->flag = flag;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMFORBIDENSOMEONE,pforbideninfo) != 0)
    {
        free(pforbideninfo);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//上主持人麦
int RequestBase::RoomOnvjMic()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMONVJMIC,NULL) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//下主持人麦
int RequestBase::RoomOffvjMic()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMOFFVJ_MIC,NULL) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//把主持人麦给某人
int RequestBase::RoomGivOnevjMic(int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int *i = (int*)malloc(sizeof(int));
	*i = idx;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMGIVEVJMIC, i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
// 是否同意上主持人麦 add by cx 10-6-2
int RequestBase::ApplyGetVJmic( int idx, char flag )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_APPLYADDFRIEND *apply;
	if(NULL == (apply = (VA_APPLYADDFRIEND *)malloc(sizeof(VA_APPLYADDFRIEND))))
		return -1;
	apply->idx=idx;
	apply->flag=flag;

	if(g_NetApp.PostMessage((int)VATYPE_APPLYGETVJMIC,apply) != 0)
    {
        free(apply);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//让主持人下麦
int RequestBase::RoomGiveOffvjMic(int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int *i = (int*)malloc(sizeof(int));
	*i = idx;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMGIVEOFFVJMIC, i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//让普通麦上麦用户下麦
int RequestBase::RoomGiveOffMic(int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int *i = (int*)malloc(sizeof(int));
	*i = idx;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMGIVEOFFMIC, i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//flag==>ADD_SUB_FLAG
//跟新房间黑名单列表(单条添加和删除)
int RequestBase::RoomUpdateBlackList(int cseq,int idx, char flag)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_UPBLACKLIST *pblack;
	if(NULL == (pblack = (VA_UPBLACKLIST *)malloc(sizeof(VA_UPBLACKLIST))))
		return -1;

	pblack->idx = idx;
	pblack->flag = flag;
	pblack->cseq=cseq;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMUPDATEBLACKLIST,pblack) != 0)
    {
        free(pblack);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//flag==>ADD_SUB_FLAG
//跟新管理员列表(单条添加和删除)
int RequestBase::RoomUpdateMgrList(int idx, char flag, char identity)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_UPMGRLIST *pmanager;
	if(NULL == (pmanager = (VA_UPMGRLIST *)malloc(sizeof(VA_UPMGRLIST))))
		return -1;

	pmanager->idx = idx;
	pmanager->flag = flag;
	pmanager->identity = identity;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMUPDATEMGRLIST,pmanager) != 0)
    {
        free(pmanager);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//登录大厅
int RequestBase::LoginLobby( const char * mac,int loginway )
{
    if(!m_bInit)
    {
        LOGE("NetUtil is not initialize\n");
        return -1;
    }
	
	if(!mac)
		return -1;
	if(g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_LOGIN_LOBBY * d = (VA_LOGIN_LOBBY*)malloc(sizeof(VA_LOGIN_LOBBY));
	d->len = (int)strlen( mac );
	strncpy( d->mac, mac, d->len );
	d->isautologin = 0;
	d->roomid = 0;
    s_login_way = d->loginway = loginway;

	if(g_NetApp.PostMessage((int)VATYPE_LOGINLOBBY,d) != 0)
    {
        free(d);
		return -1;
    }
	g_NetApp.GetInformer()->Inform();
	return 0;
}

//登录大厅（重连）
int RequestBase::LoginLobbyAuto( const char * mac, int roomid)
{
    if(!m_bInit)
    {
        LOGE("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!mac)
		return -1;
	if (g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_LOGIN_LOBBY * d = (VA_LOGIN_LOBBY*)malloc(sizeof(VA_LOGIN_LOBBY));
	d->len = (int)strlen( mac );
	strncpy( d->mac, mac, d->len );
	d->isautologin = 1;
	d->roomid = roomid;
	d->loginway = s_login_way;

	if(g_NetApp.PostMessage((int)VATYPE_LOGINLOBBY,d) != 0)
    {
        free(d);
		return -1;
    }
	g_NetApp.GetInformer()->Inform();
	return 0;

}

//获取用户资料 参数一用户idx列表,参数二数量
int RequestBase::GetUserInfo(int idxl[],int count)
{
    if(!m_bInit)
    {
        LOGE("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!idxl)
		return -1;
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if(count == 0 || count > 200)
		return -1;

	std::vector<int> *idxlist = new std::vector<int>;
	for(int i = 0;i < count;i++)
	{
		idxlist->push_back(idxl[i]);
	}

	VA_GETUSERINFO *pgetuserinfo;
	if(NULL == (pgetuserinfo = (VA_GETUSERINFO *)malloc(sizeof(VA_GETUSERINFO))))
		return -1;

	pgetuserinfo->idxlist=idxlist;
	if(g_NetApp.PostMessage((int)VATYPE_GETUSERINFO,pgetuserinfo) != 0)
    {
        free(pgetuserinfo);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//获取好友列表
int RequestBase::GetFriendListWithoutUserInfo()
{
    if(!m_bInit)
    {
        LOGE("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_GETFRIENDLISTWITHOUTUSERINFO,NULL) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//获取黑名单列表
int RequestBase::GetBlackListWithoutUserInfo()
{
    if(!m_bInit)
    {
        LOGE("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_GETBLACKLISTWITHOUTUSERINFO,0) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//添加好友
int RequestBase::AddFriend(int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;
	int *i = (int*)malloc(sizeof(int));
	*i = idx;
	if(g_NetApp.PostMessage((int)VATYPE_ADDFRIEND,i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//添加黑名单
int RequestBase::AddBlack(int cseq,int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_ADDBLACK *pAddBlack;
	if(NULL == (pAddBlack = (VA_ADDBLACK *)malloc(sizeof(VA_ADDBLACK))))
		return -1;

	pAddBlack->idx=idx;
	pAddBlack->cseq=cseq;

	//int *i = new int;
	//*i = idx;

	if(g_NetApp.PostMessage((int)VATYPE_ADDBLACK,pAddBlack) != 0)
    {
        free(pAddBlack);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//删除好友
int RequestBase::DelFriend(int cseq,int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_DELFRIEND *pDelFriend;
	if(NULL == (pDelFriend = (VA_DELFRIEND *)malloc(sizeof(VA_DELFRIEND))))
		return -1;

	pDelFriend->idx=idx;
	pDelFriend->cseq=cseq;

	if(g_NetApp.PostMessage((int)VATYPE_DELFRIEND,pDelFriend) != 0)
    {
        free(pDelFriend);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//删除黑名单
int RequestBase::DelBlack(int cseq,int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_DELBLACK *pDelBlack;
	if(NULL == (pDelBlack = (VA_DELBLACK *)malloc(sizeof(VA_DELBLACK))))
		return -1;

	pDelBlack->idx=idx;
	pDelBlack->cseq=cseq;

	if(g_NetApp.PostMessage((int)VATYPE_DELBLACK,pDelBlack) != 0)
    {
        free(pDelBlack);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//获取通道列表
int RequestBase::GetChannellist( int version, const char * mac )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    if(!mac)
		return -1;
	if(!g_NetApp.GetLobbyTunnel()->IsConnect())
		return -1;

	VA_GET_LOBBY_LIST * d = (VA_GET_LOBBY_LIST*)malloc(sizeof(VA_GET_LOBBY_LIST));
	d->version = version;
	d->len = (int)strlen( mac );
	strncpy( d->mac, mac, d->len );

	if(g_NetApp.PostMessage((int)VATYPE_GETCHANNELLIST,d) != 0)
    {
        free(d);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//获取当前大厅的房间列表
int RequestBase::GetRoomList(int cseq)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	int * i = (int*)malloc(sizeof(int));
	*i = cseq;

	if(g_NetApp.PostMessage((int)VATYPE_GETROOMLIST, i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetRoomListEx(int cseq, int type)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if( ROOMTYPE_BASEG == type )
	{
		return GetRoomList( cseq );
	}

	////////////////////////////////////////////////////////////////////////

	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_GET_ROOM_LIST * data;
	if(NULL == (data = (VA_GET_ROOM_LIST *)malloc(sizeof(VA_GET_ROOM_LIST))))
		return -1;
	data->cseq = cseq;
	data->type = (char)type;

	if(g_NetApp.PostMessage((int)VATYPE_GETROOMLISTEX, data) != 0)
    {
        free(data);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//获取房间信息,可以是一组房间
int RequestBase::GetRoomInfo(int cseq,int roomcount, int *roomlist)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!roomlist || roomcount<=0)
		return -1;
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_GETROOMINFOLIST *proominfo;
	if(NULL == (proominfo = (VA_GETROOMINFOLIST *)malloc(sizeof(VA_GETROOMINFOLIST))))
		return -1;

	proominfo->cseq = cseq;
	proominfo->count = roomcount;
	proominfo->list = (int *)malloc(sizeof(int *) * roomcount);
	if(proominfo->list == NULL)
    {
        free(proominfo);
		return -1;
    }
	memcpy(proominfo->list, roomlist, sizeof(int *) * roomcount);

	if(g_NetApp.PostMessage((int)VATYPE_GETROOMINFO,proominfo) != 0)
    {
        free(proominfo->list);
        free(proominfo);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetTmpRoomInfo( int idx )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	int * room_idx = NULL;
	if(NULL == ( room_idx = ( int * )malloc(sizeof( int ))))
		return -1;

	* room_idx = idx;

	if(g_NetApp.PostMessage((int)VATYPE_GETTMPROOMINFO, room_idx ) != 0)
    {
        free(room_idx);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//用户创建头像信息
int RequestBase::CreateAvatar( char * nickname, ResultData_Avatar_Updata * avarat )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_CREATEAVARAT *pInfo=(VA_CREATEAVARAT *)malloc(sizeof(VA_CREATEAVARAT));
	if(pInfo==NULL)
		return -1;

	if(strlen(nickname)>(MAXLEN_NICKNAME-1))
		return -1;

	strcpy(pInfo->nickname,nickname);
	memcpy( pInfo, avarat, sizeof( ResultData_Avatar_Updata ));

	if(g_NetApp.PostMessage((int)VATYPE_CREATEAVATAR,pInfo) != 0)
    {
        free(pInfo);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//用户更改头像信息
int RequestBase::ChangeAvatar( ResultData_Avatar_Updata * avarat )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	ResultData_Avatar_Updata * pInfo=(ResultData_Avatar_Updata *)malloc(sizeof(ResultData_Avatar_Updata));
	if(pInfo==NULL)
		return -1;

	short a = 12;
	htons(a);

	memcpy( pInfo, avarat, sizeof( ResultData_Avatar_Updata ));

	if(g_NetApp.PostMessage((int)VATYPE_CHANGEAVATAR,pInfo) != 0)
    {
        free(pInfo);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::ChangeNickName( int idx, const char * name)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }

	if(!name)
		return -1;
    
	if(strlen(name)>(MAXLEN_NICKNAME-1))
		return -1;

	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	ResultData_ChangeNickName * pInfo=(ResultData_ChangeNickName *)malloc(sizeof(ResultData_ChangeNickName));
	if(pInfo==NULL)
		return -1;

	pInfo->player_idx = idx;
	strncpy( pInfo->nick_name, name, sizeof( pInfo->nick_name ));

	if(g_NetApp.PostMessage((int)VATYPE_CHANGENICKNAME,pInfo) != 0)
    {
        free(pInfo);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//给好友送花
int RequestBase::SendFlower(int s_idx,int r_idx,int cate_id,int count)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;
	void *mem = malloc(sizeof(VA_SENDFLOWER));
	if(!mem)
		return -1;
	VA_SENDFLOWER *p = new(mem) VA_SENDFLOWER;	
	p->r_idx=r_idx;
	p->cate_dx=cate_id;
	p->number=count;
	if(g_NetApp.PostMessage((int)VATYPE_SENDFLOWER,p) != 0)
	{
		free(mem);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();
	return 0;
}
//发送大喇叭
int RequestBase::BroadAll(const char *content)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }

	if(!content)
		return -1;
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	//the max size is MSG_LENGTH - 12;
	unsigned int msglen = (unsigned int)strlen(content);
	if(msglen > MSG_LENGTH - 12)
		msglen = MSG_LENGTH - 12;

	VA_CHAT *msg;
	if(NULL == (msg = (VA_CHAT *)malloc(sizeof(VA_CHAT))))
		return -1;

	msg->msglen = msglen;
	memcpy(msg->content, content, msglen);

	if(g_NetApp.PostMessage((int)VATYPE_BROADALL, msg) != 0)
    {
        free(msg);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//发送小喇叭
int RequestBase::BroadLobby(int lobbyidx,const char *content)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	//the max size is MSG_LENGTH - 12;
	unsigned int msglen = (unsigned int)strlen(content);
	if(msglen > MSG_LENGTH - 12)
		msglen = MSG_LENGTH - 12;

	VA_CHAT *msg;
	if(NULL == (msg = (VA_CHAT *)malloc(sizeof(VA_CHAT))))
		return -1;

	msg->msglen = msglen;
	memcpy(msg->content, content, msglen);
	msg->idx=lobbyidx;

	if(g_NetApp.PostMessage((int)VATYPE_BROADLOBBY, msg) != 0)
    {
        free(msg);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//发送房间临时公告
int RequestBase::SendRoomNotice(const char *content)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	//the max size is MSG_LENGTH - 12;
	unsigned int msglen = (unsigned int)strlen(content);
	if(msglen > MSG_LENGTH - 12)
		msglen = MSG_LENGTH - 12;

	VA_CHAT *msg;
	if(NULL == (msg = (VA_CHAT *)malloc(sizeof(VA_CHAT))))
		return -1;

	msg->msglen = msglen;
	memcpy(msg->content, content, msglen);

	if(g_NetApp.PostMessage((int)VATYPE_SENDROOMNOTIECETEMP, msg) != 0)
    {
        free(msg);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//取得指定大厅信息
int RequestBase::GetLobbyInfo(int cseq,int lobbyid)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_LOBBYINFO *pLobbyInfo;
	if(NULL == (pLobbyInfo = (VA_LOBBYINFO *)malloc(sizeof(VA_LOBBYINFO))))
		return -1;
	pLobbyInfo->cseq = cseq;
	pLobbyInfo->lobbyid = lobbyid;
	
	if(g_NetApp.PostMessage((int)VATYPE_GETLOBBYINFO,pLobbyInfo) != 0)
    {
        free(pLobbyInfo);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
//邀请用户进入表演麦麦序
int RequestBase::InviteMic(int useridx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int *i = (int*)malloc(sizeof(int));
	*i = useridx;

	if(g_NetApp.PostMessage((int)VATYPE_INVITEMIC, i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
/*			add by jinguanfu 2010/3/2 <end> 回调API改造成接口			*/
/************************************************************************/

int RequestBase::SendChatToUser(int to_idx, const char *content)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!content)
		return -1;
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	int contentlen = strlen(content);
	//the max size is MSG_LENGTH - 12;
	//unsigned int msglen = strlen(content);
	//if(msglen > MSG_LENGTH - 12)
	//	msglen = MSG_LENGTH - 12;
	if(contentlen > MSG_LENGTH - 12)
		contentlen = MSG_LENGTH - 12;

	VA_CHAT *msg;
	if(NULL == (msg = (VA_CHAT *)malloc(sizeof(VA_CHAT))))
		return -1;

	msg->idx = to_idx;
	msg->msglen = contentlen;
	memset(msg->content,0,MSG_LENGTH);
	memcpy(msg->content, content, contentlen);

	if(g_NetApp.PostMessage((int)VATYPE_CHATTOUSER, msg) != 0)
    {
        free(msg);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}

int RequestBase::SendChatToClub(const char *content)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!content)
		return -1;

	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	int contentlen = strlen(content);
	//the max size is MSG_LENGTH - 12;
	//unsigned int msglen = strlen(content);
	//if(msglen > MSG_LENGTH - 12)
	//	msglen = MSG_LENGTH - 12;
	if(contentlen > MSG_LENGTH - 12)
		contentlen = MSG_LENGTH - 12;

	VA_CHAT *msg;
	if(NULL == (msg = (VA_CHAT *)malloc(sizeof(VA_CHAT))))
		return -1;

	//msg->clubid = clubid;
	msg->msglen = contentlen;
	memset(msg->content,0,MSG_LENGTH);
	memcpy(msg->content, content, contentlen);

	if(g_NetApp.PostMessage((int)VATYPE_CHATTOCLUB, msg) != 0)
    {
        free(msg);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::SendChatToRoom(const char* content)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }

	if(!content)
		return -1;
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int contentlen = strlen(content);
	if(contentlen > MSG_LENGTH - 12)
		contentlen = MSG_LENGTH - 12;

	VA_CHAT *msg;
	if(NULL == (msg = (VA_CHAT *)malloc(sizeof(VA_CHAT))))
		return -1;

	//msg->idx = to_idx;
	msg->msglen = contentlen;
	memset(msg->content,0,MSG_LENGTH);
	memcpy(msg->content, content, contentlen);
	
	if(g_NetApp.PostMessage((int)VATYPE_ROOMPUBLICCHAT, msg) != 0)
    {
        free(msg);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}

int RequestBase::SendSpeakerToAll(const char* content)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    if(!content)
		return -1;
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	int contentlen = strlen(content);
	//the max size is MSG_LENGTH - 12;
	//unsigned int msglen = strlen(content);
	//if(msglen > MSG_LENGTH - 12)
	//	msglen = MSG_LENGTH - 12;
	if(contentlen > MSG_LENGTH - 12)
		contentlen = MSG_LENGTH - 12;

	VA_CHAT *msg;
	if(NULL == (msg = (VA_CHAT *)malloc(sizeof(VA_CHAT))))
		return -1;

	//msg->clubid = clubid;
	msg->msglen = contentlen;
	memset(msg->content,0,MSG_LENGTH);
	memcpy(msg->content, content, contentlen);

	if(g_NetApp.PostMessage((int)VATYPE_BROADALL, msg) != 0)
    {
        free(msg);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}

int RequestBase::SendSpeakerToLobby(int lobbyid, const char* content)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    if(!content)
		return -1;
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	int contentlen = strlen(content);
	//the max size is MSG_LENGTH - 12;
	//unsigned int msglen = strlen(content);
	//if(msglen > MSG_LENGTH - 12)
	//	msglen = MSG_LENGTH - 12;
	if(contentlen > MSG_LENGTH - 12)
		contentlen = MSG_LENGTH - 12;

	VA_CHAT *msg;
	if(NULL == (msg = (VA_CHAT *)malloc(sizeof(VA_CHAT))))
		return -1;

	msg->idx = lobbyid;
	msg->msglen = contentlen;
	memset(msg->content,0,MSG_LENGTH);
	memcpy(msg->content, content, contentlen);

	if(g_NetApp.PostMessage((int)VATYPE_BROADLOBBY, msg) != 0)
    {
        free(msg);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
/************************************************************************/
/*			add by jinguanfu 2009/12/28 <begin> 好友操作				*/
//查询好友位置
int RequestBase::QueryFriendLocation(int cseq,int friend_idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	//int ret=0;
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;
	
	VA_FRIENDLOCATION *pFriLoca;
	if(NULL == (pFriLoca = (VA_FRIENDLOCATION *)malloc(sizeof(VA_FRIENDLOCATION))))
		return -1;
	pFriLoca->cseq = cseq;
	pFriLoca->idx = friend_idx;
	
	
	if(g_NetApp.PostMessage((int)VATYPE_GETFRIENDLOCATION, pFriLoca) != 0)
    {
        free(pFriLoca);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
/*			add by jinguanfu 2009/12/28 <end> 好友操作					*/
/************************************************************************/
/************************************************************************/
/*			add by jinguanfu 2010/1/4 <begin> 房间管理接口			*/
//请求取得待审核房间会员列表
int RequestBase::GetRoomApplyList (int cseq)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int *i = (int*)malloc(sizeof(int));
	*i=cseq;

	if(g_NetApp.PostMessage((int)VATYPE_GETROOMAPPLYLIST, i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//用户申请房间会员 glp
int RequestBase::ApplyRoomMember(int cseq)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int *i=(int*)malloc(sizeof(int));
	*i=cseq;
	if(g_NetApp.PostMessage((int)VATYPE_APPLYROOMMEMBER, i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//审核会员加入申请
int RequestBase::AuditAddApply (int cseq,int idx,char type)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_OPTION *opt;
	if(NULL == (opt = (VA_OPTION *)malloc(sizeof(VA_OPTION))))
		return -1;

	opt->id=idx;
	opt->type=type;
	opt->cseq=cseq;

	if(g_NetApp.PostMessage((int)VATYPE_AUDITADDAPPLY, opt) != 0)
    {
        free(opt);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//取房间会员列表
int RequestBase::GetRoomMemberList (int cseq)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int *i=(int*)malloc(sizeof(int));
	*i=cseq;

	if(g_NetApp.PostMessage((int)VATYPE_GETROOMMEMBERLIST, i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//解除房间会员资格
int RequestBase::UnchainMember (int cseq,int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_UNCHAINMEMBER *pUm;
	if(NULL == (pUm = (VA_UNCHAINMEMBER *)malloc(sizeof(VA_UNCHAINMEMBER))))
		return -1;
	pUm->idx = idx;
	pUm->cseq = cseq;
	
	if(g_NetApp.PostMessage((int)VATYPE_UNCHAINROOMMEMBER, pUm) != 0)
    {
        free(pUm);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//赋予用户房间主持助理权限
int RequestBase::SetRoomPesideAssistant(int cseq,int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_PESIDEASSISTANT *pPA;
	if(NULL == (pPA = (VA_PESIDEASSISTANT *)malloc(sizeof(VA_PESIDEASSISTANT))))
		return -1;

	pPA->cseq = cseq;
	pPA->idx = idx;

	if(g_NetApp.PostMessage((int)VATYPE_PESIDEASSISTANT, pPA) != 0)
    {
        free(pPA);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//赋予用户房间主持人权限
int RequestBase::SetRoomPeside (int cseq, int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_PESIDE *pP;
	if(NULL == (pP = (VA_PESIDE *)malloc(sizeof(VA_PESIDE))))
		return -1;
	pP->cseq = cseq;
	pP->idx = idx;

	if(g_NetApp.PostMessage((int)VATYPE_PESIDE, pP) != 0)
    {
        free(pP);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//赋予用户房间副室主权限
int RequestBase::SetRoomSubHost(int cseq,int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_SUBHOST *pSH;
	if(NULL == (pSH = (VA_SUBHOST *)malloc(sizeof(VA_SUBHOST))))
		return -1;
	pSH->cseq = cseq;
	pSH->idx = idx;

	if(g_NetApp.PostMessage((int)VATYPE_SUBHOST, pSH) != 0)
    {
        free(pSH);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//设置为房间会员（由管理员变为会员）	//add by jinguanfu 2010/5/13
int RequestBase::SetRoomMember(int cseq,int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_MEMBER *pMB;
	if(NULL == (pMB = (VA_SUBHOST *)malloc(sizeof(VA_MEMBER))))
		return -1;
	pMB->cseq = cseq;
	pMB->idx = idx;


	if(g_NetApp.PostMessage((int)VATYPE_MEMBER, pMB) != 0)
    {
        free(pMB);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//取得房间黑名单
int RequestBase::GetBlackList(int cseq)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int *i=(int*)malloc(sizeof(int));
	*i= cseq;

	if(g_NetApp.PostMessage((int)VATYPE_GETROOMBLACKLIST, i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//黑名单用户添加/删除
int RequestBase::AddDelBlack (int idx ,char type)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_UPBLACKLIST *publ;
	if(NULL == (publ = (VA_UPBLACKLIST *)malloc(sizeof(VA_UPBLACKLIST))))
		return -1;

	publ->idx=idx;
	publ->flag=type;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMUPDATEBLACKLIST, publ) != 0)
    {
        free(publ);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//查看房间设置面板
int RequestBase::GetRoomPanelInfo (int cseq)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;
	
	int *i=(int*)malloc(sizeof(int));
	*i = cseq;

	if(g_NetApp.PostMessage((int)VATYPE_GETROOMPANELINFO, i) != 0)
    {
        free(i);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//设置房间密码
int RequestBase::SetRoomPassword(int cseq,const char *password)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!password)
		return -1;
    
	int ret=0;
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_SETPASSWORD* setpwd;
	if(NULL == (setpwd = (VA_SETPASSWORD *)malloc(sizeof(VA_SETPASSWORD))))
		return -1;

	strcpy(setpwd->password,password);
	setpwd->passwordlen=strlen(password);
	setpwd->cseq = cseq;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMPASSWORD, setpwd) != 0)
    {
        free(setpwd);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return ret;
}
//设置房间开启/关闭(上锁)
int RequestBase::SetRoomSwitch(int cseq,char roomflag)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_SETFLAG* setflag;
	if(NULL == (setflag = (VA_SETFLAG *)malloc(sizeof(VA_SETFLAG))))
		return -1;

	setflag->cseq = cseq;
	setflag->flag = roomflag;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMSWITCH, setflag) != 0)
    {
        free(setflag);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//设置房间仅对会员开放
int RequestBase::SetRoomPrivate(int cseq,char accessflag)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_SETFLAG* setflag;
	if(NULL == (setflag = (VA_SETFLAG *)malloc(sizeof(VA_SETFLAG))))
		return -1;

	setflag->cseq = cseq;
	setflag->flag = accessflag;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMPRIVATE, setflag) != 0)
    {
        free(setflag);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//用户进出信息开启/关闭
int RequestBase::SetUserInOutSwitch(int cseq,char inoutflag)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_SETFLAG* setflag;
	if(NULL == (setflag = (VA_SETFLAG *)malloc(sizeof(VA_SETFLAG))))
		return -1;

	setflag->cseq = cseq;
	setflag->flag = inoutflag;

	if(g_NetApp.PostMessage((int)VATYPE_USERINOUTSWITCH, setflag) != 0)
    {
        free(setflag);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//设置自由上麦开启/关闭
int RequestBase::SetGetMicSwitch( int cseq, char getmicflag )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_SETFLAG* setflag;
	if(NULL == (setflag = (VA_SETFLAG *)malloc(sizeof(VA_SETFLAG))))
		return -1;

	setflag->cseq = cseq;
	setflag->flag = getmicflag;

	if(g_NetApp.PostMessage((int)VATYPE_FREEGETMICSWITCH, setflag) != 0)
    {
        free(setflag);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//设置自由上麦开启/关闭
int RequestBase::SetOnMicSwitch(int cseq,char onmicflag)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_SETFLAG* setflag;
	if(NULL == (setflag = (VA_SETFLAG *)malloc(sizeof(VA_SETFLAG))))
		return -1;

	setflag->cseq = cseq;
	setflag->flag = onmicflag;

	if(g_NetApp.PostMessage((int)VATYPE_FREEONMICSWITCH, setflag) != 0)
    {
        free(setflag);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//房间名称修改
int RequestBase::ModifyRoomName(int cseq,const char* roomname)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int namelen = strlen(roomname);
	if (namelen>50)
		namelen=50;
	
	VA_ROOMNAME *proomname;
	if(NULL == (proomname = (VA_ROOMNAME *)malloc(sizeof(VA_ROOMNAME))))
		return -1;

	proomname->length=namelen;
	memset(proomname->roomname,0,51);
	memcpy(proomname->roomname, roomname, namelen);
	proomname->cseq = cseq;
	
	if(g_NetApp.PostMessage((int)VATYPE_MODIFYROOMNAME, proomname) != 0)
    {
        free(proomname);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//设置房间公告
int RequestBase::SetRoomAffiche (int cseq,const char* content)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    if(!content)
		return -1;
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int contentlen = strlen(content);
	if (contentlen>MSG_LENGTH)
		contentlen=MSG_LENGTH;
	
	VA_ROOMAFFICHE *roomaffiche;
	if(NULL == (roomaffiche = (VA_ROOMAFFICHE *)malloc(sizeof(VA_ROOMAFFICHE))))
		return -1;
	
	roomaffiche->length=contentlen;
	//memset(roomaffiche->content,0,MSG_LENGTH);
	memset( roomaffiche->content, 0, sizeof( roomaffiche->content ));// add by cx 10-5-11
	memcpy(roomaffiche->content,content,contentlen);
	roomaffiche->cseq = cseq;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMUPDATECONTENT, roomaffiche) != 0)
    {
        free(roomaffiche);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//设置允许房间公聊
int RequestBase::AllowRoomAllChat (int cseq ,char chatflag)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_SETFLAG *setflag;
	if(NULL == (setflag = (VA_SETFLAG *)malloc(sizeof(VA_SETFLAG))))
		return -1;

	setflag->cseq = cseq;
	setflag->flag = chatflag;

	if(g_NetApp.PostMessage((int)VATYPE_ALLOWROOMALLCHAT, setflag) != 0)
    {
        free(setflag);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//设置房间欢迎词
int RequestBase::SetRoomSalutatory (int cseq ,const char* content)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!content)
		return -1;

	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int contentlen = strlen(content);
	if (contentlen>MSG_LENGTH)
		return -2;
	VA_ROOMAFFICHE *msg;
	if(NULL == (msg = (VA_ROOMAFFICHE *)malloc(sizeof(VA_ROOMAFFICHE))))
		return -1;

	//memset(msg->content,0,MSG_LENGTH);
	memset( msg->content, 0, sizeof( msg->content )); // add by cx 10-5-11
	memcpy(msg->content,content,contentlen);
	msg->cseq = cseq;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMSALUTATORY, msg) != 0)
    {
        free(msg);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//设置房间logo
int RequestBase::SetRoomLogo(int cseq, const char* path)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    if(!path)
		return -1;
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_ROOMLOGO *roomlogo;
	if(NULL == (roomlogo = (VA_ROOMLOGO *)malloc(sizeof(VA_ROOMLOGO))))
		return -1;

	int pathlen = strlen(path);
	roomlogo->length=pathlen;
	memset(roomlogo->logopath,0,256);
	memcpy(roomlogo->logopath,path,pathlen);
	roomlogo->cseq = cseq;

	if(g_NetApp.PostMessage((int)VATYPE_ROOMLOGO, roomlogo) != 0)
    {
        free(roomlogo);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
/*			add by jinguanfu 2010/1/4 <end> 房间管理接口				*/
/************************************************************************/
/************************************************************************/
/*			add by jinguanfu 2010/1/4 <begin> 角色信息				*/
//发送取得角色信息请求
int RequestBase::GetPartInfo(int cseq,int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_PARTINFO *pPI;
	if(NULL == (pPI = (VA_PARTINFO *)malloc(sizeof(VA_PARTINFO))))
		return -1;

	pPI->cseq = cseq;
	pPI->idx = idx;

	if(g_NetApp.PostMessage((int)VATYPE_GETPARTINFO, pPI) != 0)
    {
        free(pPI);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
/*			add by jinguanfu 2010/1/4 <end> 角色信息					*/
/************************************************************************/
int RequestBase::GetRoomList(int cseq,int lobbyid)
{

    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_ROOMLIST *pRoomList;
	if(NULL == (pRoomList = (VA_ROOMLIST *)malloc(sizeof(VA_ROOMLIST))))
		return -1;
	pRoomList->cseq = cseq;
	pRoomList->lobbyid = lobbyid;

	if(g_NetApp.PostMessage((int)VATYPE_GETROOMLISTBYLOBBY, pRoomList) != 0)
    {
        free(pRoomList);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}

/************************************************************************/
/*			add by jinguanfu 2010/1/14 <begin> 							*/
//离开房间（切换房间时使用）
int RequestBase::LeaveRoom()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_LEAVEROOM, 0) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();

	return 0;
}
/*			add by jinguanfu 2010/1/14 <end> 							*/
/************************************************************************/
/************************************************************************/
/*			add by jinguanfu 2010/1/20 <begin> 							*/
//是否同意添加好友
//参数说明：int		idx		请求添加好友的用户
//			char	flag	0--同意 1--不同意
int RequestBase::ApplyAddFriend(int idx, char flag)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_APPLYADDFRIEND *apply;
	if(NULL == (apply = (VA_APPLYADDFRIEND *)malloc(sizeof(VA_APPLYADDFRIEND))))
		return -1;
	apply->idx=idx;
	apply->flag=flag;

	if(g_NetApp.PostMessage((int)VATYPE_APPLYADDFRIEND,apply) != 0)
    {
        free(apply);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();

	return 0;
}
/*			add by jinguanfu 2010/1/20 <end> 							*/
/************************************************************************/
/*
// 删除角色信息
int RequestBase::DeleteAvarta(int idx)
{
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;
	
	if (idx <=0)
		return -2;

	int* m_idx = new int;
	*m_idx =idx;

	if(g_NetApp.PostMessage((int)VATYPE_DELETEAVARTA, m_idx) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();

	return 0;
}
*/
//设置回调类对象
void RequestBase::SetResponseHandle(ResponseBase* pRb)
{
	g_NetApp.SetResponseHandler(pRb);
}

//add by jinguanfu 2010/4/8 取得简单房间信息
int RequestBase::GetRoomSimpleInfo()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_GETROOMSIMPLEINFO,0) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();

	return 0;
}
int RequestBase::GetRoomSimpleInfoEx( int type )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if( ROOMTYPE_BASEG == type )
	{
		return GetRoomSimpleInfo();
	}
	//////////////////////////////////////////////

	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_GETROOMSIMPLEINFOEX,NULL) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();

	return 0;
}
//add by jinguanfu 2010/4/11 用户退出房间会员
int RequestBase::ExitRoomMember()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_EXITROOMMEMBER,0) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();

	return 0;
}

//add by jinguanfu 2010/4/15 取得大厅在线人数
int RequestBase::GetLobbyOnlineNum()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetChannellistTunnel()->IsConnect())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_GETLOBBYONLINE,0) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//add by cx 10-7-6 购买
int RequestBase::BuyPack( ResultData_Buy * pPack, int count, int type, int idx )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if( MAXBUYCOUNT < count )
	{
		return -1;
	}

	VA_BUYCOUNT * buy;
	if(NULL == (buy = (VA_BUYCOUNT *)malloc(sizeof(VA_BUYCOUNT))))
		return -1;

	buy->idx	= idx;
	buy->count	= (char)count;
	for(int i=0; i<buy->count; ++i)
	{
		buy->pack[i].goods_id	= pPack[i].goods_id;
		buy->pack[i].count		= pPack[i].count;
		strncpy( buy->pack[i].msg, pPack[i].msg, MAXLEN_BUY_MSG );
	}

	switch( type )
	{
	case BUYTYPE_SELFG:
		if(g_NetApp.PostMessage((int)VATYPE_BUYPACK,buy) != 0)
        {
            free(buy);
			return -1;
        }
		break;

	case BUYTYPE_SENDG:
		if(g_NetApp.PostMessage((int)VATYPE_SENDPACK,buy) != 0)
        {
            free(buy);
			return -1;
        }
		break;
	}

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetPack()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_GETPACK,NULL) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::DelPack( int cseq, int * idx, int count )
{
    
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsConnect())
		return -1;

	if( MAXLEN_GLOBALID_CNT < count )
		return -1;

	VA_DELPACP * del;
	if(NULL == (del = (VA_DELPACP *)malloc(sizeof(VA_DELPACP))))
		return -1;

	del->count = count;
	memcpy( del->idx, idx, sizeof( int ) * count );
	del->cseq = cseq;

	if(g_NetApp.PostMessage((int)VATYPE_DELPACK, del ) != 0)
    {
        free(del);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::UsePack( int idx )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsConnect())
		return -1;

	int * pidx = (int*)malloc(sizeof(int));
	* pidx = idx;

	if(g_NetApp.PostMessage((int)VATYPE_USEPACK, pidx ) != 0)
    {
        free(pidx);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::CreateRoom( ResultData_CreateRoom * room )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	VA_CREATE_ROOM * pRoom;
	if(NULL == (pRoom = (VA_CREATE_ROOM *)malloc(sizeof(VA_CREATE_ROOM))))
		return -1;

	strncpy( pRoom->name, room->name, sizeof( pRoom->name ));
	strncpy( pRoom->pwd, room->pwd, sizeof( pRoom->pwd ));
	pRoom->is_pwd = room->is_pwd;
	pRoom->is_score = room->is_score;
	pRoom->is_ballot = room->is_ballot;
	pRoom->count = room->count;

	if(g_NetApp.PostMessage((int)VATYPE_CREATE_ROOM,pRoom) != 0)
    {
        free(pRoom);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetGiftTopList()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if( g_NetApp.PostMessage((int)VATYPE_GET_GIFT_TOPLIST, NULL ) != 0 )
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//取得在线时间
int RequestBase::GetOnlineTime( int idx )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	int * pidx = NULL;
	if( NULL == ( pidx = ( int * )malloc( sizeof( int ))))
		return -1;

	* pidx = idx;

	if( g_NetApp.PostMessage(( int )VATYPE_GET_ONLINE_TIME, pidx ) != 0 )
    {
        free(pidx);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//取得新手任务
int RequestBase::GetNewUserTask()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if( g_NetApp.PostMessage(( int )VATYPE_GET_NEWUSER_TASK_C2L2C, NULL ) != 0 )
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//提交新手任务
int RequestBase::TaskStep( short step )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	short * pStep = NULL;
	if( NULL == ( pStep = ( short * )malloc( sizeof( short ))))
		return -1;

	* pStep = step;

	if( g_NetApp.PostMessage(( int )VATYPE_TASK_STEP_C2L2C, pStep ) != 0 )
    {
        free(pStep);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//提交新手任务
int RequestBase::TaskGift( short step )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	short * pStep = NULL;
	if( NULL == ( pStep = ( short * )malloc( sizeof( short ))))
		return -1;

	* pStep = step;

	if( g_NetApp.PostMessage(( int )VATYPE_TASK_GIFT_C2L2C, pStep ) != 0 )
    {
        free(pStep);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetGiftInfo()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if( g_NetApp.PostMessage(( int )VATYPE_GET_GIFTINFO_C2L2C, NULL ) != 0 )
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetSystemBroad()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if( g_NetApp.PostMessage(( int )VATYPE_GET_SYSTME_SPEAKER_C2L2C, NULL ) != 0 )
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetUserSeel( const int * idx, int count )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	vector<int> *idxlist = new vector<int>;
    if(!idxlist)
        return -1;
    
	for(int i = 0;i < count;i++)
	{
		idxlist->push_back(idx[i]);
	}

	VA_GETUSERINFO *pgetuserinfo;
	if(NULL == (pgetuserinfo = (VA_GETUSERINFO *)malloc(sizeof(VA_GETUSERINFO))))
		return -1;

	pgetuserinfo->idxlist=idxlist;

	if( g_NetApp.PostMessage(( int )VATYPE_GET_USER_SEEL, pgetuserinfo ) != 0 )
    {
        delete idxlist;
        free(pgetuserinfo);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//发送用户pc配置
int RequestBase::SendSysteInfo( char * data )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    if(!data)
		return -1;
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	int len = strlen(data);
	if( 0 >= len || len >= MAXLEN_SYSTEMINFO )
	{
		return -1;
	}

	VA_SYSTEMINFO * info = NULL;
	if( NULL == ( info = ( VA_SYSTEMINFO * ) malloc( sizeof( VA_SYSTEMINFO ))))
		return -1;

	memcpy( info->buf, data, sizeof( char ) * len );
	info->len = len;

	if( g_NetApp.PostMessage( (int)VATYPE_SEND_SYSTEM_INFO, info ) != 0)
    {
        free(info);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//获取分组信息
int RequestBase::GetGroupInfo()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_GET_GROUP_INFO, NULL ))
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetRoomListInGroup( int group_idx )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	int * info = NULL;
	if( NULL == ( info = ( int * ) malloc( sizeof( int ))))
		return -1;

	* info = group_idx;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_GET_ROOMLIST_IN_GROUP, info ))
    {
        free(info);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}
int RequestBase::GetRoomListInGroupBatch(short* groupId,int count)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    if(!groupId)
        return -1;
    
    GroupBatch* pGb = (GroupBatch*)malloc(sizeof(GroupBatch));
    if(!pGb)
        return -1;
    pGb->pId = (short*)malloc(sizeof(short)*count);
    if(pGb->pId)
    {
        free(pGb);
        return -1;
    }
    
    for (int i=0; i<count; i++) {
        pGb->pId[i] = groupId[i];
    }
    
    if(0 != g_NetApp.PostMessage(KTV_GETROOMLIST_FROMGROUP, pGb))
    {
        free(pGb->pId);
        free(pGb);
        return -1;
    }
    return 0;
}
//获取歌曲列表
int RequestBase::GetSongInfoList()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_GET_MUSICEXINFO_C2L2C, NULL ))
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//获取音视频服务器列表
int RequestBase::GetAvServerList( short net_type )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	short * pType = NULL;
	if( NULL == ( pType = ( short * ) malloc( sizeof( short ))))
		return -1;

	* pType = net_type;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_GET_AVSERVERINFO_C2L2C, pType ))
    {
        free(pType);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetOffLineMsg()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_GET_OFFLINEMSG_C2L2C, NULL ))
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetRoomSortInGroup( short group_idx )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	short * p_idx = NULL;
	if( NULL == ( p_idx = ( short * ) malloc( sizeof( short ))))
		return -1;

	* p_idx = group_idx;
	if( 0 != g_NetApp.PostMessage( (int)VATYPE_GET_ROOMSORT_IN_GROUP, p_idx ))
    {
        free(p_idx);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetUserSpeakerInfo()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
		return -1;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_GET_USERSPEAKERINFO_C2L2C, NULL ))
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//////////////////////////////////////////////////////////////////////////

//客户端房间打分
int RequestBase::RoomScore( const char *certify)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }

	if(!certify || *certify==0)
		return -1;

	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int length = strlen(certify);

	void *mem = malloc(sizeof(VA_SCORE));
	if(!mem)
		return -1;
	VA_SCORE *p = new(mem) VA_SCORE;
	p->len = length;
	memcpy(p->certify, certify, length);

	if(g_NetApp.PostMessage((int)VATYPE_ROOMSCORE,p) != 0)
    {
        free(mem);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::SendFireworks( int cate_idx, int count )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_SEND_FIREWORKS * pSendFireworks;
	if(NULL == (pSendFireworks = (VA_SEND_FIREWORKS *)malloc(sizeof(VA_SEND_FIREWORKS))))
		return -1;

	pSendFireworks->idx = cate_idx;
	pSendFireworks->count = count;

	if(g_NetApp.PostMessage((int)VATYPE_SEND_FIREWORKS,pSendFireworks) != 0)
    {
        free(pSendFireworks);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::ShowRoomIncome()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_ROOM_VIEW_INCOME, NULL) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::ShowRoomGetIncome()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_ROOM_VIEW_GET_INCOME, NULL) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::GetRoomIncome()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	if(g_NetApp.PostMessage((int)VATYPE_ROOM_GET_INCOME, NULL) != 0)
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//发送用户pc概况
int RequestBase::SendPcData( char * data )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!data || *data==0)
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int len = strlen(data);
	if( 0 >= len || len >= MAXLEN_PCINFO )
	{
		return -1;
	}

	VA_PCINFO * info = NULL;
	if( NULL == ( info = ( VA_PCINFO * ) malloc( sizeof( VA_PCINFO ))))
		return -1;

	memcpy( info->buf, data, sizeof( char ) * len );
	info->len = len;

	if( g_NetApp.PostMessage( (int)VATYPE_ROOM_SEND_PC_INFO, info ) != 0)
    {
        free(info);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//邀请用户加入房间会员
int RequestBase::InviteJoinRoom( int idx, int wseq )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	if( 0 > wseq )
		return -1;

	int * p_idx = NULL;
	if( NULL == ( p_idx = ( int * ) malloc( sizeof( int ))))
		return -1;
	* p_idx = idx;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_ROOM_INVITE_MEMBER, p_idx, wseq ))
    {
        free(p_idx);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//回应邀请加入房间会员
int RequestBase::ReInviteJoinRoom( char type )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	char * p_type = NULL;
	if( NULL == ( p_type = ( char * ) malloc( sizeof( char ))))
		return -1;
	* p_type = type;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_ROOM_INVITE_REPLY, p_type ))
    {
        free(p_type);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//取得当日礼物赠送记录
int RequestBase::GetGiftSend()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_ROOM_GET_GIFTSEND, NULL ))
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//取得当日礼物接收记录
int RequestBase::GetGiftRecv()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_ROOM_GET_GIFTRECV, NULL ))
		return -1;

	g_NetApp.GetInformer()->Inform();
	return 0;
}

//封mac
int RequestBase::KickUserByMac( int idx )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int * pIdx = NULL;
	if( NULL == ( pIdx = ( int * ) malloc( sizeof( int ))))
		return -1;
	* pIdx = idx;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_ROOM_KICK_BY_MAC, pIdx ))
    {
        free(pIdx);
		return -1;
    }
	g_NetApp.GetInformer()->Inform();
	return 0;
}

//封ip
int RequestBase::KickUserByIp( int idx )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int * pIdx = NULL;
	if( NULL == ( pIdx = ( int * ) malloc( sizeof( int ))))
		return -1;
	* pIdx = idx;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_ROOM_KICK_BY_IP, pIdx ))
    {
        free(pIdx);
		return -1;
    }
	g_NetApp.GetInformer()->Inform();
	return 0;
}

//设置在麦剩余时间
int RequestBase::SetOnMicTemainTime( int idx, int time )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	VA_SET_MICTIME * pTime = NULL;
	if( NULL == ( pTime = ( VA_SET_MICTIME * ) malloc( sizeof( VA_SET_MICTIME ))))
		return -1;
	pTime->idx = idx;
	pTime->time = time;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_ROOM_SET_ONMIC_TIME, pTime ))
    {
        free(pTime);
		return -1;
    }
	g_NetApp.GetInformer()->Inform();
	return 0;
}

//设置在歌曲剩余时间
int RequestBase::SendSongRemnantsTime( short time )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	short * pTime = NULL;
	if( NULL == ( pTime = ( short * ) malloc( sizeof( short ))))
		return -1;
	* pTime = time;

	if( 0 != g_NetApp.PostMessage( (int)VATYPE_ROOM_SET_SONG_TIME, pTime ))
    {
        free(pTime);
		return -1;
    }
	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::OpenPrivateMic(short mictype)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
	{
		//////TRACE_INFO((Error, "is not token login");
		return -1;
	}

	short* type = (short*)malloc(sizeof(short));
	*type = mictype;

	if (g_NetApp.PostMessage(VATYPE_OPEN_PRIVATEMIC, type) != 0)
	{
		////TRACE_INFO((Error, "PostMessage VATYPE_OPEN_PRIVATEMIC error");
        free(type);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();

	return 0;
}

int RequestBase::ClosePrivateMic()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
	{
		////TRACE_INFO((Error, "is not token login");
		return -1;
	}

	if (g_NetApp.PostMessage(VATYPE_CLOSE_PRIVATEMIC, NULL) != 0)
	{
		////TRACE_INFO((Error, "PostMessage VATYPE_CLOSE_PRIVATEMIC error");
		return -1;
	}
	g_NetApp.GetInformer()->Inform();

	return 0;
}

int RequestBase::ClosedPrivateMic(int bidx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
	{
		////TRACE_INFO((Error, "is not token login");
		return -1;
	}

	int* idx = (int*)malloc(sizeof(int));
	*idx = bidx;

	if (g_NetApp.PostMessage(VATYPE_CLOSED_PRIVATEMIC, idx) != 0)
	{
		////TRACE_INFO((Error, "PostMessage VATYPE_CLOSED_PRIVATEMIC error");
        free(idx);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();

	return 0;
}

int RequestBase::RequestWatchPrivateMic(int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
	{
		////TRACE_INFO((Error, "is not token login");
		return -1;
	}

	int* param = (int*)malloc(sizeof(int));
	*param = idx;

	if (g_NetApp.PostMessage(VATYPE_REQUEST_WATCH_PRIVATEMIC, param) != 0)
	{
		////TRACE_INFO((Error, "PostMessage VATYPE_REQUEST_WATCH_PRIVATEMIC error");
        free(param);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();

	return 0;
}

int RequestBase::ResponseWatchPrivateMic(int allow, int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
	{
		////TRACE_INFO((Error, "is not token login");
		return -1;
	}

	int* param = (int*)malloc(sizeof(int)*2);
	*param = allow;
	*(param+1) = idx;

	if (g_NetApp.PostMessage(VATYPE_RESPONSE_WATCH_PRIVATEMIC, param) != 0)
	{
		////TRACE_INFO((Error, "PostMessage VATYPE_RESPONSE_WATCH_PRIVATEMIC error");
        free(param);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();

	return 0;
}
int RequestBase::InviteWatchPrivateMic(int num, int idxs[])
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
	{
		////TRACE_INFO((Error, "is not token login");
		return -1;
	}

	if (num <= 0)
	{
		////TRACE_INFO((Error, "num=%d", num);
		return -1;
	}

	int* param = (int*)malloc(sizeof(int)*(num+1));
	*param = num;
	memcpy((char*)(param+1), (char*)idxs, num*sizeof(int));

	if (g_NetApp.PostMessage(VATYPE_INVITEPRIVATEMIC, param) != 0)
	{
		////TRACE_INFO((Error, "PostMessage VATYPE_INVITEPRIVATEMIC error");
        free(param);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::ResponseInviteWatchPrivateMic(int allow, int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
	{
		////TRACE_INFO((Error, "is not token login");
		return -1;
	}

	int* param = (int*)malloc(sizeof(int)*2);
	*param = allow;
	*(param+1) = idx;

	if (g_NetApp.PostMessage(VATYPE_RESPONSE_INVITEPRIVATEMIC, param) != 0)
	{
		////TRACE_INFO((Error, "PostMessage VATYPE_RESPONSE_INVITEPRIVATEMIC error");
        free(param);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::OpenRoomPrivateMicFlag(char flag)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
	{
		////TRACE_INFO((Error, "is not token login");
		return -1;
	}

	char* param = (char*)malloc(sizeof(char));
	*param = flag;

	if (g_NetApp.PostMessage(VATYPE_OPEN_ROOMPRIVATEMIC_FLAG, param) != 0)
	{
		////TRACE_INFO((Error, "PostMessage VATYPE_OPEN_ROOMPRIVATEMIC_FLAG error");
        free(param);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();

	return 0;
}

int RequestBase::CrownKickoutSomeOne(int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
	{
		////TRACE_INFO((Error, "is not token login");
		return -1;
	}

	int* param = (int*)malloc(sizeof(int));
	*param = idx;

	if (g_NetApp.PostMessage(VATYPE_CROWN_KICKOUT_SOMEONE, param) != 0)
	{
		////TRACE_INFO((Error, "PostMessage VATYPE_CROWN_KICKOUT_SOMEONE error");
        free(param);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();

	return 0;
}

int RequestBase::ForbidUserIp(int idx)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
	{
		////TRACE_INFO((Error, "is not token login");
		return -1;
	}

	int* param = (int*)malloc(sizeof(int));
	*param = idx;

	if (g_NetApp.PostMessage(VATYPE_FORBID_USERIP, param) != 0)
	{
		////TRACE_INFO((Error, "PostMessage VATYPE_FORBID_USERIP error");
        free(param);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();

	return 0;
}

int RequestBase::GetFamilyInfo()
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if (!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
	{
		////TRACE_INFO((Error, "is not token login for lobby");
		return -1;
	}

	if (g_NetApp.PostMessage(VATYPE_GETFAMILYINFO, NULL) != 0)
	{
		////TRACE_INFO((Error, "PostMessage VATYPE_GETFAMILYINFO error");
		return -1;
	}

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::SingerScores( int idx, short scores[], int count )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	if( count > 2000 )
		return -1;

	vector< short > * vec_score = new vector< short >;
    if(!vec_score)
        return -1;
	for(int i = 0;i < count;i++)
		vec_score->push_back( scores[i] );

	VA_SEND_SCORES * pScores;
	if(NULL == (pScores = (VA_SEND_SCORES *)malloc(sizeof(VA_SEND_SCORES))))
    {
        delete vec_score;
		return -1;
    }

	pScores->idx = idx;
	pScores->scores = vec_score;
	if(g_NetApp.PostMessage( (int)VATYPE_SINGER_SCORES, pScores ) != 0)
    {
        delete vec_score;
        free(pScores);
		return -1;
    }

	g_NetApp.GetInformer()->Inform();
	return 0;
}

int RequestBase::UpdateSong( int idx )
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
	if(!g_NetApp.GetRoomTunnel()->IsTokenLogin())
		return -1;

	int* param = (int*)malloc(sizeof(int));
    if(!param)
        return -1;
	*param = idx;

	if (g_NetApp.PostMessage(VATYPE_UPDATESONG, param) != 0)
	{
        free(param);
		return -1;
	}
	g_NetApp.GetInformer()->Inform();

	return 0;
}

//手机KTV用户聊天
/*
 @param toIdx:聊天对象，如果是0，则为公聊
 @param contet:聊天信息 不要超过4096
 */
int RequestBase::SendChatKTV(int toIdx,const char* content)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
        return -1;
    if (!content) {
        return -1;
    }
    KTVCHATINFO* pInfo = (KTVCHATINFO*)malloc(sizeof(KTVCHATINFO));
    if(pInfo == NULL)
    	return -1;
    pInfo->toIdx = toIdx;
    strcpy(pInfo->buf,content);
    if(g_NetApp.PostMessage(KTV_MOBILE_CHAT_C2L2C, pInfo) != 0)
    {
        free(pInfo);
        return -1;
    }
    
    g_NetApp.GetInformer()->Inform();
    return 0;
}

//手机KTV用户在大厅送礼物
/*
 @param toIdx:送礼物的对象IDX
 @param giftId:礼物ID
 @param number:礼物数量
 
 @return:0,成功；-1,失败
 */
int RequestBase::SendGiftInLobby(int toIdx,int giftId,int number,int roomId)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    if(!g_NetApp.GetLobbyTunnel()->IsTokenLogin())
        return -1;
    KTV_SEND_GIFT* pGift = (KTV_SEND_GIFT*)malloc(sizeof(KTV_SEND_GIFT));
	if(!pGift)
		return -1;
	pGift->toIdx = toIdx;
	pGift->giftId = giftId;
	pGift->count = number;
	pGift->roomId = roomId;
    if(g_NetApp.PostMessage(KTV_MOBILE_SENDGIFT_C2L2C, pGift) != 0)
    {
        free(pGift);
        return -1;
    }
    
    g_NetApp.GetInformer()->Inform();
    return 0;
}
/*
	请求发喇叭的用户信息
	@param idx:请求的idx数组
	@param count:请求的idx的数量

	@return:0,成功；-1,失败
	*/
int RequestBase::GetUserInfoForSpeaker(int idx[],int count)
{
	if(!idx || count==0 || count>200)//ReserveData_GetUserInfo
		return -1;

	IdxArray* p = (IdxArray*)malloc(sizeof(IdxArray));
	if(!p)
		return -1;
	p->idx = new int[count];
	if(!p->idx)
	{
		free(p);
		return -1;
	}

	p->count = count;
	memcpy(p->idx,idx,sizeof(int)*count);
	

	if(g_NetApp.PostMessage(VATYPE_GETUSERINFO_FORHORN_C2L2C, p) != 0)
	{
		delete p->idx;
		free(p);
		return -1;
	}

	g_NetApp.GetInformer()->Inform();
	return 0;
}
/*
	请求用户信息
	@param data:请求的idx数组
	@param count:请求的idx的数量

	@return:0,成功；-1,失败
*/
int RequestBase::GetUserInfoWithVersion(RequestData_IdxWithVersion data[],int count)
{
	if(!data || count==0 || count>200)
		return -1;

	IdxVersionArray* p = (IdxVersionArray*)malloc(sizeof(IdxVersionArray));
	if(!p)
		return -1;
	p->idx = new RequestData_IdxWithVersion[count];
	if(!p->idx)
	{
		free(p);
		return -1;
	}

    p->count = count;
	memcpy(p->idx,data,sizeof(RequestData_IdxWithVersion)*count);
    
	if(g_NetApp.PostMessage(VATYPE_GETUSERINFO_WITHVERSION_C2L2C, p) != 0)
	{
		delete p->idx;
		free(p);
		return -1;
	}

	g_NetApp.GetInformer()->Inform();
	return 0;
}
