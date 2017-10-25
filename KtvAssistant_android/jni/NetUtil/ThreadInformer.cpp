#include "wrap/config.h"
#include "threadinformer.h"
#include "NetApp.h"
#include "protocol.h"
#include "XingguangDef.h"
#include "RequestBase.h"
#include "wrap/event_wrapper.h"

#ifndef NETUTIL_ANDROID
#define LOGI printf
#define LOGE printf
#endif

bool ThreadInformer::s_bInit = false;

ThreadInformer::ThreadInformer()
{
	m_pObjectEventInform = EventWrapper::Create();
}
ThreadInformer::ThreadInformer(NetworkUtil::Reactor *pReactor,const char *host,short port) 
: NetworkUtil::UdpSocket(pReactor,host,port)
,m_pObjectEventInform(NULL)
{
	m_pObjectEventInform = EventWrapper::Create();
}
ThreadInformer::~ThreadInformer()
{
	if(m_pObjectEventInform)
		delete m_pObjectEventInform;
}

int ThreadInformer::Init()
{
#ifndef TEST_UDP
	if (!s_bInit)
		s_bInit = CreateThread();
	return s_bInit?0:-1;
#else
	return UdpSocket::Init();
#endif//TEST_UDP
}

bool ThreadInformer::UnInit()
{
	return ReleaseThread();
}

void ThreadInformer::Inform()
{
#ifndef TEST_UDP
	//通知处理消息
	m_pObjectEventInform->Set();
#else
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(Getport()); ///监听端口
	local.sin_addr.s_addr = inet_addr("127.0.0.1"); ///本机
	sendto(m_fd,"A",1,0,(struct sockaddr*)&local,sizeof(local));
#endif//TEST_UDP
}

void ThreadInformer::ProcessIdle()
{
	if(m_pObjectEventInform->Wait(100) != kEventSignaled)
	{
		return ;
	}

	dealMessageInner();
}

void ThreadInformer::onFDRead()
{
#ifdef TEST_UDP
	// add by cx 10-6-3
	struct sockaddr_in from;
	int fromlen =sizeof(from);
	char buf[64];
	recvfrom(m_fd,buf,sizeof(buf),0,(struct sockaddr*)&from,&fromlen);
#endif//TEST_UDP

	dealMessageInner();
}


void ThreadInformer::dealMessageInner()
{
	//从队列中读出数据处理
	//LOGI("ThreadInformer::dealMessageInner\n");

	NetApp::MSGINFO _msg = {0};
	while(g_NetApp.GetMessage(_msg) == 0)
	{
		VoidGuard g( (_msg.v) );
		switch(_msg.type)
		{
		case VATYPE_CONNECTTOLOBBY:
			{
				VA_ConnectToSvr *pcts = (VA_ConnectToSvr *)(_msg.v);

				//TRACE_INFO((Info, "conn lobby: host=%s port=%d, lobbyaddress=%d", pcts->host, pcts->port, (int)g_NetApp.GetLobbyTunnel());
				g_NetApp.GetLobbyTunnel()->Connect(pcts->host,pcts->port,pcts->to);
			}
			break;
		case VATYPE_CONNECTTOROOM_BASE:
		case VATYPE_CONNECTTOROOM_PRACTICE:
			{
				VA_ConnectToSvr *pcts = (VA_ConnectToSvr *)(_msg.v);
				g_NetApp.GetRoomTunnel()->m_connect_type = _msg.type;

				//TRACE_INFO((Info, "conn room: host=%s port=%d", pcts->host, pcts->port );
				g_NetApp.GetRoomTunnel()->Connect(pcts->host,pcts->port,pcts->to);
			}
			break;
		case VATYPE_CONNECTTOAUTH:
			{
				VA_ConnectToSvr *pcts = (VA_ConnectToSvr *)(_msg.v);

				//TRACE_INFO((Info, "conn auth: host=%s port=%d, authaddress=%d", pcts->host, pcts->port, int(g_NetApp.GetAuthTunnel()));
				g_NetApp.GetAuthTunnel()->Connect(pcts->host,pcts->port,pcts->to);
			}
			break;
		case VATYPE_CONNECTCHANNELLIST:
			{
				VA_ConnectToSvr *pcts = (VA_ConnectToSvr *)(_msg.v);

				//TRACE_INFO((Info, "conn channel list: host=%s port=%d", pcts->host, pcts->port );
				g_NetApp.GetChannellistTunnel()->Connect(pcts->host,pcts->port,pcts->to);
			}
			break;
		case VATYPE_CLOSECHANNELLIST:
			g_NetApp.GetChannellistTunnel()->closeSocket();
			break;
		case VATYPE_CLOSELOBBY:
			g_NetApp.GetLobbyTunnel()->closeSocket();
		    break;
		case VATYPE_CLOSEROOM:
			g_NetApp.GetRoomTunnel()->closeSocket();
		    break;
		case VATYPE_CLOSEAUTH:
			g_NetApp.GetAuthTunnel()->closeSocket();
			break;
		case VATYPE_IDAUTH:
			{
				VA_IdPassAuth *pida = (VA_IdPassAuth*)(_msg.v);
				char sendbuf[256];
				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write((short)0);
				writestream.Write((short)CMD_GAME_GETTOKEN_BYID);
				writestream.Write(wseq);
				writestream.Write(pida->id,strlen(pida->id));
				writestream.Write(pida->passwd,strlen(pida->passwd));
				writestream.flush();

				if(SendToSvr(g_NetApp.GetAuthTunnel(),&writestream) == 0)
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
						return;
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = AUTHTUNNEL;
					pRD->cmd = (short)CMD_GAME_GETTOKEN_BYID;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->repeat = pida->again;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_TOURISTAUTH:
			{
				VA_TouristAuth *pTourist = (VA_TouristAuth*)(_msg.v);
				char sendbuf[32];
				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write((short)0);
				writestream.Write((short)CMD_GAME_XG_VISTOR);
				writestream.Write(wseq);
				writestream.flush();

				if(SendToSvr(g_NetApp.GetAuthTunnel(),&writestream) == 0)
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
						return;
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = AUTHTUNNEL;
					pRD->cmd = (short)CMD_GAME_XG_VISTOR;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->repeat = pTourist->again;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_LOGINLOBBY:
			{
				VA_LOGIN_LOBBY * d = (VA_LOGIN_LOBBY*)((_msg.v));
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[512];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write((short)CMD_TOKENLOGIN_C2L2C);
				writestream.Write(wseq);
				writestream.Write(g_NetApp.GetToken(),g_NetApp.GetTokenlen());
				char strversion[10];
				sprintf(strversion,"%f",g_Version);
				writestream.Write(strversion,strlen(strversion));
				writestream.Write( d->mac, d->len );
				writestream.Write( d->isautologin );
				writestream.Write( d->roomid );
                writestream.Write(d->loginway);
				writestream.flush();

				if(SendToSvr(g_NetApp.GetLobbyTunnel(),&writestream) == 0)
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
						return;
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_TOKENLOGIN_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = 0;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_CONFIRMTOKEN:
			{
				char sendbuf[512];
				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write((short)ROOM_CMD_TOKEN);
				writestream.Write(wseq);				
				writestream.Write(g_NetApp.GetToken(),g_NetApp.GetTokenlen());
				char strversion[10];
				sprintf(strversion,"%f",g_Version);
				
				writestream.Write(strversion,strlen(strversion));
				writestream.flush();

				if(SendToSvr(g_NetApp.GetRoomTunnel(),&writestream) == 0)
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
						return;
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_TOKEN;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_ENTERROOM:
			{
				char sendbuf[512];
				int wseq = g_NetApp.GetCounter()->Get();
				VA_ROOM* proom  = (VA_ROOM*)(_msg.v);
				int roomid = proom->id;

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);//预留一个字节用以表示是压缩，还是加密
				writestream.Write((short)ROOM_CMD_LOGIN);
				writestream.Write(wseq);
				writestream.Write(roomid);
				writestream.Write(proom->passwd,strlen(proom->passwd));
				writestream.Write(proom->type);
				writestream.flush();

				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
						return;
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_LOGIN;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq=proom->cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_GETROOMALLINFO:
			{
				char sendbuf[512];
				int wseq = g_NetApp.GetCounter()->Get();
				//int cseq = *(int*)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GET_ALLINFO);
				writestream.Write(wseq);
				writestream.flush();
                g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
        case VATYPE_ROOMKEEPALIVE:
            {
                char sendbuf[512];
                int wseq = g_NetApp.GetCounter()->Get();

                NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
                char type = 65;
                writestream.Write(type);
                writestream.Write((short)ROOM_CMD_KEEPALIVE);
                writestream.Write(wseq);
                writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					//TRACE_INFO((Debug,  "send room keep alive!" );
				}				
            }
            break;
		case VATYPE_ROOM_GETWAITMICLIST:
			{
				char sendBuf[32];
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream( sendBuf, sizeof( sendBuf ));
				writestream.Write( '[' );
				writestream.Write((short) ROOM_CMD_GET_WAITMICLIST );
				writestream.Write( wseq );
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf( writestream );
			}
			break;
        case VATYPE_ROOMWAITMIC:
            {
                char sendbuf[512];
                int wseq = g_NetApp.GetCounter()->Get();
                VA_MIC_INFO* pmic  = (VA_MIC_INFO*)(_msg.v);

                NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
                char type = 65;
                writestream.Write(type);
                writestream.Write((short)ROOM_CMD_WAITMIC);
                writestream.Write(wseq);
                writestream.Write(pmic->musicid);
                writestream.Write(pmic->bk);
				writestream.Write(pmic->level);
				writestream.Write(pmic->name,strlen(pmic->name));
				writestream.Write(pmic->speed);
				//writestream.Write(pmic->change);
                writestream.flush();
                g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
            }
            break;
        case VATYPE_ROOMCANCLEWAITMIC:
            {
                char sendbuf[512];
                int wseq = g_NetApp.GetCounter()->Get();

				// add by cx 10-5-13
				int * i = (int *)(_msg.v);

                NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
                char type = 65;
                writestream.Write(type);
                writestream.Write((short)ROOM_CMD_CANCLE_WAITMIC);
                writestream.Write(wseq);
				writestream.Write( * i ); // add by cx 10-5-13
                writestream.flush();
                g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
            }
            break;
        case VATYPE_ROOMWAITMICPK:
            {
                char sendbuf[512];
                int wseq = g_NetApp.GetCounter()->Get();
                VA_MIC_INFO* pmic  = (VA_MIC_INFO*)(_msg.v);

                NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
                char type = 65;
                writestream.Write(type);
                writestream.Write((short)ROOM_CMD_WAITMIC_PK);
                writestream.Write(wseq);
                writestream.Write(pmic->idx);
                writestream.Write(pmic->pkmusicid);
                writestream.Write(pmic->pkbk);
                writestream.flush();
                g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
            }
            break;
        case VATYPE_ROOMCANCLEWAITMICPK:
            {
                char sendbuf[512];
                int wseq = g_NetApp.GetCounter()->Get();

		// add by cx 10-5-13
		int * i = (int *)(_msg.v);

                NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
                char type = 65;
                writestream.Write(type);
                writestream.Write((short)ROOM_CMD_CANCLE_WAITMIC_PK);
                writestream.Write(wseq);
		writestream.Write( * i ); // add by cx 10-5-13
                writestream.flush();
                g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
            }
            break;
        case VATYPE_ROOMUPWAITMIC:
            {
                char sendbuf[512];
                int wseq = g_NetApp.GetCounter()->Get();
                VA_UD_MICINFO* pmic  = (VA_UD_MICINFO*)(_msg.v);

                NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
                char type = 65;
                writestream.Write(type);
                writestream.Write((short)ROOM_CMD_UP_WAITMIC);
				writestream.Write(wseq);
				writestream.Write(pmic->tag);
                writestream.Write(pmic->idx);
                writestream.Write(pmic->flag);
                writestream.flush();
                g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
            }
            break;
        case VATYPE_ROOMDOWNWAITMIC:
            {
                char sendbuf[512];
                int wseq = g_NetApp.GetCounter()->Get();
                VA_UD_MICINFO* pmic  = (VA_UD_MICINFO*)(_msg.v);

                NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
                char type = 65;
                writestream.Write(type);
                writestream.Write((short)ROOM_CMD_DOWN_WAITMIC);
                writestream.Write(wseq);
				writestream.Write(pmic->tag);
                writestream.Write(pmic->idx);
                writestream.Write(pmic->flag);
                writestream.flush();
                g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
            }
            break;
		case VATYPE_ROOMSCORE:
			{
				struct _tNetSingScore
				{
					_tNetSingScore()
						: score(0)
						, combo_num(0)
						, perfect_num(0)
						, great_num(0)
						, good_num(0)
						, poor_num(0)
						, miss_num(0)
					{
					}

					int								score;// 演唱分数
					int								combo_num;// combo数量
					int								perfect_num;
					int								great_num;
					int								good_num;
					int								poor_num;
					int								miss_num;
				};

				char sendbuf[512];
				VA_SCORE *prs = (VA_SCORE *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SCORE);
				writestream.Write(1);
				//writestream.Write(prs->score);
// 				/*for debug*/
// 				_tNetSingScore* score = (_tNetSingScore*)(prs->certify);
// 				char buf[260] = {0};
// 				sprintf_s(buf,"glpglp score %d,\n",score->score);
// 				OutputDebugStringA(buf);
// 				score->score = 903;
				writestream.Write(prs->certify, prs->len);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		case VATYPE_ROOMKICKOUTSOMEONE:
			{
				char sendbuf[64];
				int idx = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_KICKOUT_SOMEONE);
				writestream.Write(1);
				writestream.Write(idx);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		case VATYPE_ROOMFORBIDENSOMEONE:
			{
				char sendbuf[64];
				VA_FORBIDENINFO *pvfi = (VA_FORBIDENINFO *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_FORBIDEN_SOMEONE);
				writestream.Write(1);
				writestream.Write(pvfi->idx);
				writestream.Write(pvfi->flag);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		case VATYPE_ROOMPRIVATECHAT:
			{
				char sendbuf[65535];
				VA_CHAT *msg = (VA_CHAT *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_PRIVATE_CHAT);
				writestream.Write(1);
				writestream.Write(msg->idx);
				writestream.Write(msg->content, msg->msglen);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOMPUBLICCHAT:
			{
				char sendbuf[65535];
				VA_CHAT *msg = (VA_CHAT *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_PUBLIC_CHAT);
				writestream.Write(1);
				writestream.Write(msg->content, msg->msglen);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOMONVJMIC:
			{
				char sendbuf[64];

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_ONVJ_MIC);
				writestream.Write(1);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		case VATYPE_ROOMOFFVJ_MIC:
			{
				char sendbuf[64];

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_OFFVJ_MIC);
				writestream.Write(1);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		case VATYPE_ROOMGIVEVJMIC:
			{
				char sendbuf[64];
				int idx = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GIVE_VJ_MIC);
				writestream.Write(1);
				writestream.Write(idx);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		//add by cx 10-6-2
		case VATYPE_APPLYGETVJMIC:		// 是否同意上主持人麦
			{
				char sendbuf[64];
				VA_APPLYADDFRIEND *apply=(VA_APPLYADDFRIEND*)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_REQUEST_GIVE_VJ_MIC);
				writestream.Write(1);
				writestream.Write(apply->idx);	//邀请我上麦的人
				writestream.Write(apply->flag);	//0:同意,1:不同意
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		case VATYPE_ROOMGIVEOFFVJMIC:
			{
				char sendbuf[64];
				int idx = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GIVEOFF_VJ_MIC);
				writestream.Write(1);
				writestream.Write(idx);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		case VATYPE_ROOMGIVEOFFMIC:
			{
				char sendbuf[64];
				int idx = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GIVEOFF_MIC);
				writestream.Write(1);
				writestream.Write(idx);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		case VATYPE_ROOMUPDATEBLACKLIST:
			{
				char sendbuf[64];
				VA_UPBLACKLIST *publ = (VA_UPBLACKLIST *)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_UPDATE_BLACKLIST);
				writestream.Write(wseq);
				writestream.Write(publ->idx);
				writestream.Write(publ->flag);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);

			}
			break;
		case VATYPE_ROOMUPDATEMGRLIST:
			{
				char sendbuf[64];
				VA_UPMGRLIST *pml = (VA_UPMGRLIST *)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_UPDATE_MGRLIST);
				writestream.Write(wseq);
				writestream.Write(pml->idx);
				writestream.Write(pml->flag);
				writestream.Write(pml->identity);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		case VATYPE_ROOMUPDATECONTENT:
			{
				char sendbuf[1024];

				//char *content = (char *)(_msg.v);
				VA_ROOMAFFICHE *pRoomAffiche=(VA_ROOMAFFICHE*)(_msg.v);
				int cseq = pRoomAffiche->cseq;
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_UPDATE_CONTENT);
				writestream.Write(wseq);
				writestream.Write(pRoomAffiche->content, pRoomAffiche->length);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = ROOMTUNNEL;
						pRD->cmd = ROOM_CMD_UPDATE_CONTENT;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->cseq = cseq;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_GETUSERINFO:
			{
				VA_GETUSERINFO *pgetuserinfo=(VA_GETUSERINFO*)(_msg.v);
				vector<int> *idxlist=(vector<int> *)pgetuserinfo->idxlist;

				//vector<int> *idxlist = (vector<int> *)(_msg.v);
				char sendbuf[65535]={0};
				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETUSERINFO_C2L2C);
				writestream.Write(wseq);
				writestream.Write((int)idxlist->size());
				for(int i = 0;i < (int)idxlist->size();i++)
				{
					int curIdx = (*idxlist)[i];//(*idxlist)[i]
					writestream.Write(curIdx);
				}
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
				delete idxlist;
			}
			break;
		case VATYPE_GETFRIENDLISTWITHUSERINFO:
			{
				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETFRIENDLISTSTATUS_C2L2C);
				writestream.Write(0);
				writestream.Write((char)1);
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_GETFRIENDLISTWITHOUTUSERINFO:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETFRIENDLISTSTATUS_C2L2C);
				//writestream.Write(0);
				writestream.Write(wseq);
				writestream.Write((char)0);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_AddFriend));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = LOBBYTUNNEL;
						pRD->cmd = CMD_GETFRIENDLISTSTATUS_C2L2C;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->cseq = 0;
						pRD->repeat = 0;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_GETBLACKLISTWITHUSERINFO:
			{
				char sendbuf[64];

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETBLACKLIST_C2L2C);
				writestream.Write(0);
				writestream.Write((char)1);
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);

			}
			break;
		case VATYPE_GETBLACKLISTWITHOUTUSERINFO:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETBLACKLIST_C2L2C);
				//writestream.Write(0);
				writestream.Write(wseq);
				writestream.Write((char)0);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_AddFriend));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = LOBBYTUNNEL;
						pRD->cmd = CMD_GETBLACKLIST_C2L2C;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->cseq = 0;
						pRD->repeat = 0;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_GETCLUBLISTWITHUSERINFO:
			{
				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETCLUBLISTSTATUS_C2L2C);
				writestream.Write(0);
				writestream.Write((char)1);
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_GETCLUBLISTWITHOUTUSERINFO:
			{
				char sendbuf[64];

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETCLUBLISTSTATUS_C2L2C);
				writestream.Write(0);
				writestream.Write((char)0);
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_LOBBYKEEPALIVE:
			{
				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_KEEPALIVE_C2L);
				writestream.Write(0);
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);

				//TRACE_INFO((Debug,  "send lobby keep alive!" );
			}
			break;
		case VATYPE_ADDFRIEND:
			{
				int *idx = (int*)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();
				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_ADDFRIEND_C2L2C);
				writestream.Write(wseq);
				writestream.Write(*idx);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					//void *mem = malloc(sizeof(ReserveData_AddFriend));
					//if(mem)
					//{
					//	ReserveData_AddFriend *pRD = new(mem) ReserveData_AddFriend;
					//	pRD->serverid = LOBBYTUNNEL;
					//	pRD->cmd = CMD_ADDFRIEND_C2L2C;
					//	pRD->seq = wseq;
					//	//pRD->t = time(NULL);
					//	pRD->idx = *idx;
					//	g_NetApp.GetRDMap()->Put(wseq,pRD);
					//}
				}
			}
			break;
		case VATYPE_ADDBLACK:
			{
				//int *idx = (int*)(_msg.v);
				VA_ADDBLACK *pAddBlack=(VA_ADDBLACK*)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();
				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_ADDBLACK_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pAddBlack->idx);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_AddBlack));
					if(mem)
					{
						ReserveData_AddBlack *pRD = new(mem) ReserveData_AddBlack;
						pRD->serverid = LOBBYTUNNEL;
						pRD->cmd = CMD_ADDBLACK_C2L2C;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->idx = pAddBlack->idx;
						pRD->cseq = pAddBlack->cseq;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_DELFRIEND:
			{
				//int *idx = (int*)(_msg.v);
				VA_DELFRIEND *pDelFriend=(VA_DELFRIEND*)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();
				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_DELFRIEND_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pDelFriend->idx);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_DelFriend));
					if(mem)
					{
						ReserveData_DelFriend *pRD = new(mem) ReserveData_DelFriend;
						pRD->serverid = LOBBYTUNNEL;
						pRD->cmd = CMD_DELFRIEND_C2L2C;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->idx = pDelFriend->idx;
						pRD->cseq = pDelFriend->cseq;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_DELBLACK:
			{
				//int *idx = (int*)(_msg.v);
				VA_DELBLACK *pDelBlack=(VA_DELBLACK*)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();
				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_DELBLACK_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pDelBlack->idx);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_DelFriend));
					if(mem)
					{
						ReserveData_DelBlack *pRD = new(mem) ReserveData_DelFriend;
						pRD->serverid = LOBBYTUNNEL;
						pRD->cmd = CMD_DELBLACK_C2L2C;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->idx = pDelBlack->idx;
						pRD->cseq = pDelBlack->cseq;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_GETCHANNELLIST:
			{
				VA_GET_LOBBY_LIST * d = (VA_GET_LOBBY_LIST *)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();
				char sendbuf[256];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				//writestream.Write('[');
				writestream.Write((short)CMD_GETSECTIONLIST_C2L2C);
				writestream.Write(wseq);
				writestream.Write(g_NetApp.GetToken(),g_NetApp.GetTokenlen());
				writestream.Write(d->version);
				writestream.Write(d->mac, d->len);
				writestream.flush();

				if(SendToSvr(g_NetApp.GetLobbyTunnel(),&writestream) == 0)
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = LOBBYTUNNEL;
						pRD->cmd = CMD_GETSECTIONLIST_C2L2C;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		
		case VATYPE_CHATTOUSER:
			{
				//int wseq = g_NetApp.GetCounter()->Get();

				VA_CHAT *msg = (VA_CHAT *)(_msg.v);
				char sendbuf[65535];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_USERCHAT_C2L2C);
				writestream.Write(int(0));
				writestream.Write(msg->idx);
				writestream.Write(msg->content, msg->msglen);
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_CHATTOCLUB:
			{
				VA_CHAT *msg = (VA_CHAT *)(_msg.v);
				char sendbuf[65535];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_CLUBCHAT_C2L2C);
				writestream.Write(int(0));
				//writestream.Write(msg->clubid);
				writestream.Write(msg->content, msg->msglen);
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_CHATTOLOBBY:
			{
				VA_CHAT *msg = (VA_CHAT *)(_msg.v);
				char sendbuf[65535];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_LOBBYCHAT_C2L2C);
				writestream.Write(int(0));
				writestream.Write(msg->content, msg->msglen);
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_CHATTOALL:
			{
				VA_CHAT *msg = (VA_CHAT *)(_msg.v);
				char sendbuf[65535];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_ALLCHAT_C2L2C);
				writestream.Write(int(0));
				writestream.Write(msg->content, msg->msglen);
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_CLUBAPPLYADD:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();

				VA_ADDCLUB *pAddClub=(VA_ADDCLUB*)(_msg.v);
				//int clubid = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_APPLYADDCLUB_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pAddClub->clubid);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_ApplyClub));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData_ApplyClub *pRD = new(mem) ReserveData_ApplyClub;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_APPLYADDCLUB_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->clubid = pAddClub->clubid;
					pRD->cseq = pAddClub->cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_CLUBAPPLYEXIT:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();
				VA_EXITCLUB *pExitClub=(VA_EXITCLUB*)(_msg.v);
				//int clubid = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_EXITCLUB_C2L2C);
				writestream.Write(wseq);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_ApplyClub));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData_ApplyClub *pRD = new(mem) ReserveData_ApplyClub;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_EXITCLUB_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->clubid = pExitClub->clubid;
					pRD->cseq = pExitClub->cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_CLUBGETAPPLYLISTCOUNT:
			{
				char sendbuf[128];
				int wseq = g_NetApp.GetCounter()->Get();
				VA_GETCLUB *pgc = (VA_GETCLUB *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETAPPLYLISTCOUNT_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pgc->day);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_ApplyClub));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData_ApplyClub *pRD = new(mem) ReserveData_ApplyClub;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_GETAPPLYLISTCOUNT_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->clubid = pgc->clubid;
					pRD->cseq=pgc->cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_CLUBGETAPPLYLIST:
			{
				char sendbuf[128];
				int wseq = g_NetApp.GetCounter()->Get();
				VA_GETCLUB *pgc = (VA_GETCLUB *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETAPPLYLIST_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pgc->pageindex);
				writestream.Write(pgc->pagesize);
				writestream.Write(pgc->day);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_ApplyClub));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData_ApplyClub *pRD = new(mem) ReserveData_ApplyClub;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_GETAPPLYLIST_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->clubid = pgc->clubid;
					pRD->cseq = pgc->cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_CLUBAPPROVALRESULT:
			{
				char sendbuf[2048];
				VA_APPROVALRESULT *par = (VA_APPROVALRESULT *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				if(par->flag == ADD_FLAG)
					writestream.Write((short)CMD_APPLYADDCLUBOK_C2L2C);
				else
					writestream.Write((short)CMD_DELAPPLYLIST_C2L2C);
				writestream.Write(int(0));
				int count = par->count;
				writestream.Write(count);
				int *pi = par->idlist;
				for(int i=0; i<count; i++)
					writestream.Write(pi[i]);
				writestream.flush();
				free(pi);
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_CLUBCREATE:
			{
				char sendbuf[sizeof(VA_CLUBINFO) + 100];
				int wseq = g_NetApp.GetCounter()->Get();
				VA_CLUBINFO *pci = (VA_CLUBINFO *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_CREATECLUB_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pci->name, strlen(pci->name));
				writestream.Write(pci->slogan, strlen(pci->slogan));
				writestream.Write(pci->admintele, strlen(pci->admintele));
				writestream.Write(pci->adminQQ, strlen(pci->adminQQ));
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_ApplyClub));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData_ApplyClub *pRD = new(mem) ReserveData_ApplyClub;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_CREATECLUB_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->clubid = pci->clubid;
					pRD->cseq = pci->cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_CLUBDISMISS:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();
				VA_DISMCLUB *pDismClub = (VA_DISMCLUB*)(_msg.v);
				//int clubid = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_DISMISSCLUB_C2L2C);
				writestream.Write(wseq);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_ApplyClub));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData_ApplyClub *pRD = new(mem) ReserveData_ApplyClub;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_DISMISSCLUB_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->clubid = pDismClub->clubid;
					pRD->cseq = pDismClub->cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_CLUBGETINFO:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();
				VA_GETCLUBINFO *pGetclubinfo=(VA_GETCLUBINFO*)(_msg.v);
				//int clubid = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETCLUBINFO_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pGetclubinfo->clubid);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_ApplyClub));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData_ApplyClub *pRD = new(mem) ReserveData_ApplyClub;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_GETCLUBINFO_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->clubid = pGetclubinfo->clubid;
					pRD->cseq = pGetclubinfo->cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_GETROOMLIST:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETROOMLIST_C2L2C);
				writestream.Write(wseq);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					//void *mem = malloc(sizeof(ReserveData));
					//if(!mem)
					//{
					//	//TRACE_INFO((Error, "memory err");
					//	return;
					//}
					//ReserveData *pRD = new(mem) ReserveData;
					//pRD->serverid = LOBBYTUNNEL;
					//pRD->cmd = (short)CMD_GETROOMLIST_C2L2C;
					//pRD->seq = wseq;
					//pRD->t = time(NULL);
					//pRD->cseq = *(int*)(_msg.v);
					//g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_GETROOMLISTEX:
			{
				VA_GET_ROOM_LIST * data = (VA_GET_ROOM_LIST*)(_msg.v);

				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETPRACTICEROOMLIST_C2L2C);
				writestream.Write(wseq);
				writestream.Write(data->type);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					//void *mem = malloc(sizeof(ReserveData));
					//if(!mem)
					//{
					//	//TRACE_INFO((Error, "memory err");
					//	return;
					//}
					//ReserveData *pRD = new(mem) ReserveData;
					//pRD->serverid = LOBBYTUNNEL;
					//pRD->cmd = (short)CMD_GETPRACTICEROOMList_C2L2C;
					//pRD->seq = wseq;
					//pRD->t = time(NULL);
					//pRD->cseq = data->cseq;
					//g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_GETROOMINFO:
			{
				char sendbuf[512];
				VA_GETROOMINFOLIST *roomlist = (VA_GETROOMINFOLIST *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETROOMINFO_C2L2C);
				writestream.Write(int(0));
				writestream.Write(roomlist->count);
				for(int i=0; i<roomlist->count; i++)
					writestream.Write(roomlist->list[i]);
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
				free(roomlist->list);
			}
			break;
		case VATYPE_GETTMPROOMINFO:
			{
				char sendbuf[64];
				int * idx = (int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETPRACTICEROOMINFO_C2L2C);
				writestream.Write(int(0));
				writestream.Write( * idx );
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_CREATEAVATAR:
			{
				int wseq = g_NetApp.GetCounter()->Get();
				VA_CREATEAVARAT * pInfo = (VA_CREATEAVARAT *)(_msg.v);

				char sendbuf[1024];
				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_CREATEAVATAR_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pInfo->nickname,strlen(pInfo->nickname));
				writestream.Write(pInfo->avaratid);
				CAvaratData::ChangeData(  & writestream, pInfo );
				writestream.flush();

				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_CREATEAVATAR_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		// add by cx 10-5-20
		case VATYPE_CHANGEAVATAR:
			{
				char sendbuf[512] = {0};
				int wseq = g_NetApp.GetCounter()->Get();
				ResultData_Avatar_Updata * pInfo = (ResultData_Avatar_Updata *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_CHANGEAVATAR_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pInfo->avaratid);
				CAvaratData::ChangeData( & writestream, pInfo );
				writestream.flush();

				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_CHANGEAVATAR_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_CHANGENICKNAME:
			{
				char sendbuf[128] = {0};
				int wseq = g_NetApp.GetCounter()->Get();
				ResultData_ChangeNickName * pInfo = (ResultData_ChangeNickName *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_CHANGENICKNAME_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pInfo->player_idx);
				writestream.Write(pInfo->nick_name, strlen( pInfo->nick_name ));
				writestream.flush();

				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_CHANGENICKNAME_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;

		/*case VATYPE_CREATEROOMOK:
			{
				char sendbuf[64];
				int roomid = *(int *)(_msg.v);

				ac::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_CREATEROOM_OK);
				writestream.Write(0);
				writestream.Write(roomid);
				writestream.Flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_DELETEROOMOK:
			{
				char sendbuf[64];
				int roomid = *(int *)(_msg.v);

				ac::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_DELROOM_OK);
				writestream.Write(0);
				writestream.Write(roomid);
				writestream.Flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_LOCKROOM:
			{
				char sendbuf[64];
				int roomid = *(int *)(_msg.v);

				ac::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_LOCKROOM);
				writestream.Write(0);
				writestream.Write(roomid);
				writestream.Flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;*/
		case VATYPE_SENDFLOWER:
			{
				char sendbuf[64];
				VA_SENDFLOWER* pmic=(VA_SENDFLOWER*)(_msg.v);	
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SEND_GIFT);
				writestream.Write(wseq);				
				writestream.Write(pmic->r_idx);
				writestream.Write(pmic->cate_dx);
				writestream.Write(pmic->number);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_SendGift));
					if(mem)
					{
						ReserveData_SendGift *pRD = new(mem) ReserveData_SendGift;
						pRD->serverid = ROOMTUNNEL;
						pRD->cmd = (short)ROOM_CMD_SEND_GIFT;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->r_idx=pmic->r_idx;
						pRD->item_id=pmic->cate_dx;
						pRD->count=pmic->number;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}

				}
				
			}
			break;
		case VATYPE_BROADALL:
			{
				VA_CHAT *msg = (VA_CHAT *)(_msg.v);
				char sendbuf[65535];
				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_BROAD_ALL_C2L2C);
				writestream.Write(wseq);
				writestream.Write(msg->content, msg->msglen);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_SendBroad));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData_SendBroad *pRD = new(mem) ReserveData_SendBroad;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_BROAD_ALL_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					strncpy(pRD->content,msg->content,sizeof(pRD->content));
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}

			}
			break;
		case VATYPE_BROADLOBBY:
			{
				VA_CHAT *msg = (VA_CHAT *)(_msg.v);
				char sendbuf[65535];
				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_BROAD_LOBBY_C2L2C);
				writestream.Write(wseq);	
				writestream.Write((int)msg->idx);
				writestream.Write(msg->content, msg->msglen);			
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_SendBroad));
					if(!mem)
					{
						LOGE("ReserveData_SendBroad oom\n");
						return;
					}
					ReserveData_SendBroad *pRD = new(mem) ReserveData_SendBroad;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_BROAD_LOBBY_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					strncpy(pRD->content,msg->content,sizeof(pRD->content));
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}

			}
			break;
		case VATYPE_SENDROOMNOTIECETEMP:
			{
				char sendbuf[65535];
				VA_CHAT *msg = (VA_CHAT *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SEND_NOTICE_TEMP);
				writestream.Write(1);
				writestream.Write(msg->content, msg->msglen);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);			

			}
			break;
		case VATYPE_GETLOBBYINFO:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();

				VA_LOBBYINFO *pLobbyInfo = (VA_LOBBYINFO*)(_msg.v);
				//int lobbyidx = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETLOBBYINFO_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pLobbyInfo->lobbyid);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_LobbyInfo));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData_LobbyInfo *pRD = new(mem) ReserveData_LobbyInfo;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_GETLOBBYINFO_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->lobbyid = pLobbyInfo->lobbyid;
					pRD->cseq = pLobbyInfo->cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_INVITEMIC:
			{
				char sendbuf[64];
				int idx = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GIVEOFF_MIC);
				writestream.Write(1);
				writestream.Write(idx);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		//add by jinguanfu 2010/1/4 <begin>
		case VATYPE_GETFRIENDLOCATION:
			{
				char sendbuf[64];
				VA_FRIENDLOCATION *pFriloca = (VA_FRIENDLOCATION*)(_msg.v);
				//int friendidx=*(int *)(_msg.v);

				int wseq=g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETFRIENDLOCATION_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pFriloca->idx);
				writestream.flush();
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData_AddFriend));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData_AddFriend *pRD = new(mem) ReserveData_AddFriend;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_GETFRIENDLOCATION_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->idx = pFriloca->idx;
					pRD->cseq = pFriloca->cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_GETROOMAPPLYLIST:
			{
				char sendbuf[64];
				int cseq = *(int *)(_msg.v);

				int wseq=g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_ROOMAPPLYLIST_C2R2C);
				writestream.Write(wseq);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_ROOMAPPLYLIST_C2R2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_APPLYROOMMEMBER:
			{
				char sendbuf[64];
				int cseq = *(int *)(_msg.v);
				int wseq=g_NetApp.GetCounter()->Get();
				//int roomid = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_USER_APP_JOINROOM);
				writestream.Write(wseq);
				//writestream.Write(roomid);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_USER_APP_JOINROOM;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_AUDITADDAPPLY:
			{
				char sendbuf[64];
				VA_OPTION *option=(VA_OPTION*)(_msg.v);
				int wseq=g_NetApp.GetCounter()->Get();
				//int id = *(int *)(_msg.v);
				int id=option->id;
				int optiontype=option->type;
				int cseq=option->cseq;

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_VERIFY_USER_APP);
				writestream.Write(wseq);
				writestream.Write(id);
				writestream.Write(optiontype);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_VERIFY_USER_APP;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_GETROOMMEMBERLIST:
			{
				char sendbuf[64];

				int cseq=*(int*)(_msg.v);
				int wseq=g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GETROOMMEMBERLIST);
				writestream.Write(wseq);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_GETROOMMEMBERLIST;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_UNCHAINROOMMEMBER:
			{
				char sendbuf[64];

				int wseq=g_NetApp.GetCounter()->Get();
				VA_UNCHAINMEMBER *pUm=(VA_UNCHAINMEMBER*)(_msg.v);
				//int idx = *(int *)(_msg.v);
				int idx = pUm->idx;
				int cseq = pUm->cseq;

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_REMOVE_USER);
				writestream.Write(wseq);
				writestream.Write(idx);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_REMOVE_USER;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_PESIDEASSISTANT:
			{
				char sendbuf[64];

				int wseq=g_NetApp.GetCounter()->Get();
				VA_PESIDEASSISTANT *pPA=(VA_PESIDEASSISTANT*)(_msg.v);
				int cseq=pPA->cseq;
				int idx=pPA->idx;
				//int idx = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GIVE_VJ_A);
				writestream.Write(wseq);
				writestream.Write(idx);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = ROOMTUNNEL;
						pRD->cmd = (short)ROOM_CMD_GIVE_VJ_A;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->cseq = cseq;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_PESIDE:
			{
				char sendbuf[64];
				int wseq=g_NetApp.GetCounter()->Get();
				VA_PESIDE *pP=(VA_PESIDE*)(_msg.v);
				int cseq=pP->cseq;
				int idx=pP->idx;

				//int idx = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GIVE_VJ);
				writestream.Write(wseq);
				writestream.Write(idx);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = ROOMTUNNEL;
						pRD->cmd = (short)ROOM_CMD_GIVE_VJ;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->cseq = cseq;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_SUBHOST:
			{
				char sendbuf[64];

				int wseq=g_NetApp.GetCounter()->Get();
				VA_SUBHOST *pSH=(VA_SUBHOST*)(_msg.v);
				int cseq=pSH->cseq;
				int idx=pSH->idx;
				//int idx = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GIVE_OUER_S);
				writestream.Write(wseq);
				writestream.Write(idx);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = ROOMTUNNEL;
						pRD->cmd = (short)ROOM_CMD_GIVE_OUER_S;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->cseq = cseq;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}

				}
			}
			break;
		case VATYPE_GETROOMBLACKLIST:
			{
				char sendbuf[64];
				int cseq = *(int *)(_msg.v);

				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GETBLACKLIST);
				writestream.Write(wseq);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_GETBLACKLIST;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_ADDDELROOMBLACK:
			{
				char sendbuf[64];
				VA_OPTION *option=(VA_OPTION*)(_msg.v);
				int id=option->id;
				int optiontype=option->type;

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_ROOMAPPLYLIST_C2R2C);
				writestream.Write(1);
				writestream.Write(id);	//idx
				writestream.Write(optiontype);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		case VATYPE_GETPARTINFO:
			{
				char sendbuf[512];
				//int idx = *(int *)(_msg.v);
				VA_PARTINFO *pPI=(VA_PARTINFO*)(_msg.v);
				int idx = pPI->idx;
				int cseq = pPI->cseq;

				int wseq = g_NetApp.GetCounter()->Get();

                NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
//                writestream.Write((short)CMD_TOKENLOGIN_C2L2C);
//                writestream.Write(wseq);
//                writestream.Write(g_NetApp.GetToken(),g_NetApp.GetTokenlen());
//                char strversion[10];
//                sprintf(strversion,"%f",g_Version);
//                writestream.Write(strversion,strlen(strversion));
//                writestream.Write( "123456", 6 );
//                writestream.Write( (short)0 );
//                writestream.Write( 0 );
//                writestream.Write( 2);
//                writestream.flush();
//                g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
//                break;
                
				writestream.Write('[');
				writestream.Write((short)CMD_GETPARTINFO_C2L2C);
				writestream.Write(wseq);
				writestream.Write(idx);	//idx
				writestream.flush();

                LOGI("send cmd = CMD_GETPARTINFO_C2L2C = %d\n",CMD_GETPARTINFO_C2L2C);
				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
                    LOGI("send cmd = CMD_GETPARTINFO_C2L2C true\n");
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = LOBBYTUNNEL;
					pRD->cmd = (short)CMD_GETPARTINFO_C2L2C;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			
			}
			break;
		case VATYPE_GETROOMPANELINFO:
			{
				char sendbuf[64];
				
				// cseq = *(int*)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GETPANELINFO);
				writestream.Write(wseq);
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendTEABuf(writestream);
			}
			break;
		case VATYPE_ROOMPASSWORD:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();
				VA_SETPASSWORD *setpwd=(VA_SETPASSWORD*)(_msg.v);
				int passwordlen=setpwd->passwordlen;
				char password[33]={0};
				strcpy(password,setpwd->password);
				int cseq = setpwd->cseq;

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SET_ROOM_PWD);
				writestream.Write(wseq);
				//writestream.Write(password,strlen(password));
				writestream.Write(password,passwordlen);
				//writestream.Write(passwordlen);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_SET_ROOM_PWD;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_ROOMSWITCH:
			{
				char sendbuf[64];
				VA_SETFLAG *pSetflag=(VA_SETFLAG*)(_msg.v);
				int flag = pSetflag->flag;
				int cseq = pSetflag->cseq;
				//char flag=*(char*)(_msg.v);

				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SET_ROOM_LOCK);
				writestream.Write(wseq);
				writestream.Write(flag);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_SET_ROOM_LOCK;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_ROOMPRIVATE:
			{
				char sendbuf[64];
				VA_SETFLAG *pSetflag=(VA_SETFLAG*)(_msg.v);
				int flag = pSetflag->flag;
				int cseq = pSetflag->cseq;
				//char flag=*(char*)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SET_USER_ONLY);
				writestream.Write(wseq);
				writestream.Write(flag);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_SET_USER_ONLY;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_USERINOUTSWITCH:
			{
				char sendbuf[64];

				VA_SETFLAG *pSetflag=(VA_SETFLAG*)(_msg.v);
				int flag = pSetflag->flag;
				int cseq = pSetflag->cseq;
				/*char flag=*(char*)(_msg.v);*/
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SET_USER_INOUT);
				writestream.Write(wseq);
				writestream.Write(flag);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_SET_USER_INOUT;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_FREEGETMICSWITCH:
			{
				char sendbuf[64];
				VA_SETFLAG *pSetflag=(VA_SETFLAG*)(_msg.v);
				int flag = pSetflag->flag;
				int cseq = pSetflag->cseq;
				/*char flag=*(char*)(_msg.v);*/
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SET_MIC_UPDOWN);
				writestream.Write(wseq);
				writestream.Write((char)flag);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_SET_MIC_UPDOWN;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_FREEONMICSWITCH:
			{
				char sendbuf[64];
				VA_SETFLAG *pSetflag=(VA_SETFLAG*)(_msg.v);
				int flag = pSetflag->flag;
				int cseq = pSetflag->cseq;
				/*char flag=*(char*)(_msg.v);*/
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SET_AUTOONMIC);
				writestream.Write(wseq);
				writestream.Write((char)flag);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_SET_AUTOONMIC;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_MODIFYROOMNAME:
			{
				char sendbuf[1024];
				VA_ROOMNAME *roomname=(VA_ROOMNAME*)(_msg.v);
				int cseq = roomname->cseq;
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SET_ROOM_NAME);
				writestream.Write(wseq);
				writestream.Write(roomname->roomname,roomname->length);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_SET_ROOM_NAME;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_ALLOWROOMALLCHAT:
			{
				char sendbuf[64];

				VA_SETFLAG *pSetflag=(VA_SETFLAG*)(_msg.v);
				int flag = pSetflag->flag;
				int cseq = pSetflag->cseq;
				/*char flag=*(char*)(_msg.v);*/
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SET_CHAT_PUBLIC);
				writestream.Write(wseq);
				writestream.Write(flag);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_SET_CHAT_PUBLIC;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_ROOMSALUTATORY:
			{
				char sendbuf[1024];
				VA_ROOMAFFICHE *salutatory=(VA_ROOMAFFICHE*)(_msg.v);
				int cseq = salutatory->cseq;
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SET_ROOM_WELCOME);
				writestream.Write(wseq);
				writestream.Write(salutatory->content,strlen(salutatory->content));
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(!mem)
					{
						//TRACE_INFO((Error, "memory err");
						return;
					}
					ReserveData *pRD = new(mem) ReserveData;
					pRD->serverid = ROOMTUNNEL;
					pRD->cmd = (short)ROOM_CMD_SET_ROOM_WELCOME;
					pRD->seq = wseq;
					pRD->t = time(NULL);
					pRD->cseq = cseq;
					g_NetApp.GetRDMap()->Put(wseq,pRD);
				}
			}
			break;
		case VATYPE_ROOMLOGO:
			{
				char sendbuf[512];
				VA_ROOMLOGO *roomlogo=(VA_ROOMLOGO*)(_msg.v);
				int cseq = roomlogo->cseq;
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_SET_ROOM_LOGO);
				writestream.Write(wseq);
				writestream.Write(roomlogo->logopath,roomlogo->length);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = ROOMTUNNEL;
						pRD->cmd = (short)ROOM_CMD_SET_ROOM_LOGO;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->cseq = cseq;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_GETROOMLISTBYLOBBY:
			{
				char sendbuf[64];
				int lobbyid=*(int *)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETROOMLISTBYLOBBY_C2L2C);
				writestream.Write(wseq);
				writestream.Write(lobbyid);	//idx
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		//add by jinguanfu 2010/1/14 离开房间（切换房间）
		case VATYPE_LEAVEROOM:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_LEAVEROOM);
				writestream.Write(wseq);
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		//add by jinguanfu 2010/1/21
		case VATYPE_APPLYADDFRIEND:
			{
				char sendbuf[64];
				VA_APPLYADDFRIEND *apply=(VA_APPLYADDFRIEND*)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_REQUESTADDFRIEND_L2C2L);
				writestream.Write(wseq);
				writestream.Write(apply->idx);	//请求添加好友的用户
				writestream.Write(apply->flag);	//0:同意,1:不同意
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		//add by jinguanfu 2010/1/27
		case VATYPE_DELETEAVARTA:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();
				int idx = *(int *)(_msg.v);

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_DELETEAVARTA_C2L2C);
				writestream.Write(wseq);
				writestream.Write(idx);	
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_GETROOMSIMPLEINFO:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETROOMSIMPLEINFOS_C2L2C);
				writestream.Write(wseq);
				writestream.flush();

				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = LOBBYTUNNEL;
						pRD->cmd = (short)CMD_GETROOMSIMPLEINFOS_C2L2C;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_GETROOMSIMPLEINFOEX:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETPRACTICEROOMSimple_C2L2C);
				writestream.Write(wseq);
				writestream.flush();

				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					//void *mem = malloc(sizeof(ReserveData));
					//if(mem)
					//{
					//	ReserveData *pRD = new(mem) ReserveData;
					//	pRD->serverid = LOBBYTUNNEL;
					//	pRD->cmd = (short)CMD_GETPRACTICEROOMSimple_C2L2C;
					//	pRD->seq = wseq;
					//	pRD->cseq = type;
					//	pRD->t = time(NULL);
					//	g_NetApp.GetRDMap()->Put(wseq,pRD);
					//}
				}
			}
			break;
		case VATYPE_EXITROOMMEMBER:
			{
				char sendbuf[64];
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_EXITMEMBER);
				writestream.Write(wseq);
				writestream.flush();

				if(g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = ROOMTUNNEL;
						pRD->cmd = (short)ROOM_CMD_EXITMEMBER;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_GETLOBBYONLINE:
			{
				char sendbuf[256];
				int wseq = g_NetApp.GetCounter()->Get();

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				//writestream.Write('[');
				writestream.Write((short)CMD_GETALLLOBBYCURNUM_C2L2C);
				writestream.Write(wseq);
				writestream.Write(g_NetApp.GetToken(),g_NetApp.GetTokenlen());
				writestream.flush();

				if(SendToSvr(g_NetApp.GetChannellistTunnel(),&writestream) == 0)
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = CHANNELLISTTUNNEL;
						pRD->cmd = (short)CMD_GETALLLOBBYCURNUM_C2L2C;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_MEMBER:
			{
				char sendbuf[64];

				int wseq=g_NetApp.GetCounter()->Get();
				VA_MEMBER *pMB=(VA_MEMBER*)(_msg.v);
				int cseq=pMB->cseq;
				int idx=pMB->idx;

				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				char type = 65;
				writestream.Write(type);
				writestream.Write((short)ROOM_CMD_GIVE_MEMBER);
				writestream.Write(wseq);
				writestream.Write(idx);
				writestream.flush();
				if(g_NetApp.GetRoomTunnel()->SendTEABuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = ROOMTUNNEL;
						pRD->cmd = (short)ROOM_CMD_GIVE_MEMBER;
						pRD->seq = wseq;
						pRD->t = time(NULL);
						pRD->cseq = cseq;
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		//add by cx 2010-7-6
		case VATYPE_BUYPACK:
			{
				VA_BUYCOUNT * pMB=(VA_BUYCOUNT*)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[512];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_BUYGOODS_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pMB->count);
				for(int i=0; i<pMB->count;++i)
				{
					writestream.Write(pMB->pack[i].goods_id);
					writestream.Write(pMB->pack[i].count);
				}
				writestream.flush();

				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					//void *mem = malloc(sizeof(ReserveData));
					//if(mem)
					//{
					//	ReserveData *pRD = new(mem) ReserveData;
					//	pRD->serverid = LOBBYTUNNEL;
					//	pRD->cmd = (short)CMD_BUYGOODS_C2L2C;
					//	pRD->seq = wseq;
					//	pRD->t = time(NULL);
					//	g_NetApp.GetRDMap()->Put(wseq,pRD);
					//}
				}
			}
			break;
		case VATYPE_SENDPACK:
			{
				VA_BUYCOUNT * pMB=(VA_BUYCOUNT*)(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[512];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_SENDGOODS_C2L2C);
				writestream.Write(wseq);
				writestream.Write(pMB->idx);
				writestream.Write(pMB->pack[0].goods_id);
				writestream.Write(pMB->pack[0].msg, strlen( pMB->pack[0].msg ));
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_GETPACK:
			{
				//(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETPACKINFO_C2L2C);
				writestream.Write(wseq);
				writestream.flush();

				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					//void *mem = malloc(sizeof(ReserveData));
					//if(mem)
					//{
					//	ReserveData *pRD = new(mem) ReserveData;
					//	pRD->serverid = LOBBYTUNNEL;
					//	pRD->cmd = (short)CMD_GETPACKINFO_C2L2C;
					//	pRD->seq = wseq;
					//	pRD->t = time(NULL);
					//	g_NetApp.GetRDMap()->Put(wseq,pRD);
					//}
				}

				//TRACE_INFO(( Debug, "send cmd:CMD_GETPACKINFO_C2L2C wseq:%d", wseq );
			}
			break;
		case VATYPE_DELPACK:
			{
				VA_DELPACP * del = ( VA_DELPACP * )(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_DELPACKINFO_C2L2C);
				writestream.Write(wseq);
				writestream.Write( del->count );
				for( int i=0; i<del->count; ++i )
				{
					writestream.Write( del->idx[i] );
				}
				writestream.flush();

				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = LOBBYTUNNEL;
						pRD->cmd = (short)CMD_DELPACKINFO_C2L2C;
						pRD->seq = wseq;
						pRD->cseq = del->cseq;
						pRD->t = time(NULL);
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_USEPACK:
			{
				//*( int * )(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_USEGOODS_C2L2C);
				writestream.Write(wseq);
				writestream.Write(*( int * )(_msg.v));
				writestream.flush();

				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					void *mem = malloc(sizeof(ReserveData));
					if(mem)
					{
						ReserveData *pRD = new(mem) ReserveData;
						pRD->serverid = LOBBYTUNNEL;
						pRD->cmd = (short)CMD_USEGOODS_C2L2C;
						pRD->seq = wseq;
						pRD->cseq = 0;
						pRD->t = time(NULL);
						pRD->cseq = *( int * )(_msg.v);
						g_NetApp.GetRDMap()->Put(wseq,pRD);
					}
				}
			}
			break;
		case VATYPE_CREATE_ROOM:
			{
				VA_CREATE_ROOM * room = ( VA_CREATE_ROOM* )(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_CREATE_PRACTICEROOM_C2L2C);
				writestream.Write(wseq);
				writestream.Write(room->name, strlen( room->name ));
				writestream.Write(room->is_score);
				writestream.Write(room->is_ballot);
				writestream.Write(room->count);
				writestream.Write(room->is_pwd);
				writestream.Write(room->pwd, strlen( room->pwd ));
				writestream.flush();

				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					//void *mem = malloc(sizeof(ReserveData));
					//if(mem)
					//{
					//	ReserveData *pRD = new(mem) ReserveData;
					//	pRD->serverid = LOBBYTUNNEL;
					//	pRD->cmd = (short)CMD_CREATE_PRACTICEROOM_C2L2C;
					//	pRD->seq = wseq;
					//	pRD->t = time(NULL);
					//	g_NetApp.GetRDMap()->Put(wseq,pRD);
					//}
				}
			}
			break;
		case VATYPE_GET_GIFT_TOPLIST:
			{
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GIFT_TOPLIST_C2L2C);
				writestream.Write(wseq);
				writestream.flush();

				if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
				}
			}
			break;
		case VATYPE_GET_ONLINE_TIME:
			{
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETONLINETIME_C2L2C);
				writestream.Write(wseq);
				writestream.Write(*( int* )(_msg.v));
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_GET_NEWUSER_TASK_C2L2C:
			{
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GET_NEWUSER_TASK_C2L2C);
				writestream.Write(wseq);
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_TASK_STEP_C2L2C:
			{
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_TASK_STEP_C2L2C);
				writestream.Write(wseq);
				writestream.Write(*( short* )(_msg.v));
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_TASK_GIFT_C2L2C:
			{
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_TASK_GIFT_C2L2C);
				writestream.Write(wseq);
				writestream.Write(*( short* )(_msg.v));
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_GET_GIFTINFO_C2L2C:
			{
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GET_GIFTINFO_C2L2C);
				writestream.Write(wseq);
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_GET_SYSTME_SPEAKER_C2L2C:
			{
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GET_SYSTME_SPEAKER_C2L2C);
				writestream.Write(wseq);
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_GET_USER_SEEL:
			{
				VA_GETUSERINFO *pgetuserinfo=(VA_GETUSERINFO*)(_msg.v);
				vector<int> *idxlist=(vector<int> *)pgetuserinfo->idxlist;

				char sendbuf[65535]={0};
				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GET_SEELINFO_C2L2C);
				writestream.Write(wseq);
				writestream.Write((int)idxlist->size());
				for(int i = 0;i < (int)idxlist->size();i++)
				{
					int curIdx = (*idxlist)[i];//(*idxlist)[i]
					writestream.Write(curIdx);
				}
				writestream.flush();
				g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
                delete idxlist;
			}
			break;
		case VATYPE_SEND_SYSTEM_INFO:
			{
				VA_SYSTEMINFO * info =(VA_SYSTEMINFO*)(_msg.v);

				char sendbuf[65535]={0};
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_SYSINFO_C2L);
				writestream.Write( ( int )1 );
				writestream.Write( info->buf, info->len );
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf( writestream );
			}
			break;
		case VATYPE_GET_GROUP_INFO:
			{
				char sendbuf[32]={0};
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GET_GROUPINFO_C2L2C);
				writestream.Write( ( int )1 );
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf( writestream );
			}
			break;
		case VATYPE_GET_ROOMSORT_IN_GROUP:
			{
				short * p_idx =(short*)(_msg.v);

				char sendbuf[32]={0};
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_UPDATE_ROOMSORTINFO_C2L2C);
				writestream.Write( ( int )1 );
				writestream.Write( * p_idx );
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf( writestream );
			}
			break;
		case VATYPE_GET_USERSPEAKERINFO_C2L2C:
			{
				char sendbuf[32]={0};
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETUSERSPEAKINFO_C2L2C);
				writestream.Write( ( int )1 );
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf( writestream );
			}
			break;
		case VATYPE_GET_ROOMLIST_IN_GROUP:
			{
				int * info =(int*)(_msg.v);

				char sendbuf[64]={0};
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GET_ROOMLIST_IN_GROUP_C2L2C);
				writestream.Write( ( int )1 );
				writestream.Write(( short )( * info ));
				writestream.flush();

				if(!g_NetApp.GetLobbyTunnel()->SendRightBuf( writestream ))
                    printf("Request Group for id %d Failed\n",*info);
                else
                    printf("Request Group for id %d Success\n",*info);
			}
			break;
		case VATYPE_GET_MUSICEXINFO_C2L2C:
			{
				char sendbuf[64]={0};
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GET_MUSICEXINFO_C2L2C);
				writestream.Write( ( int )1 );
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf( writestream );
			}
			break;
		case VATYPE_GET_AVSERVERINFO_C2L2C:
			{
				short * net_type =(short*)(_msg.v);

				char sendbuf[64]={0};
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GETAVSERVERINFO_C2L2C);
				writestream.Write( ( int )1 );
				writestream.Write( * net_type );
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf( writestream );
			}
			break;
		case VATYPE_GET_OFFLINEMSG_C2L2C:
			{
				char sendbuf[64]={0};
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GET_OFFLINEMSG_C2L2C);
				writestream.Write( ( int )1 );
				writestream.flush();

				g_NetApp.GetLobbyTunnel()->SendRightBuf( writestream );
			}
			break;
			//////////////////////////////////////////////////////////////////////////
		case VATYPE_SEND_FIREWORKS:
			{
				VA_SEND_FIREWORKS * pSendFireworks = ( VA_SEND_FIREWORKS* )(_msg.v);
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_SEND_FIREWORKS);
				writestream.Write(wseq);
				writestream.Write(pSendFireworks->idx);
				writestream.Write(pSendFireworks->count);
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_VIEW_INCOME:
			{
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_VIEW_INCOME);
				writestream.Write(wseq);
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_VIEW_GET_INCOME:
			{
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_VIEW_INCOME_LOG);
				writestream.Write(wseq);
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_GET_INCOME:
			{
				int wseq = g_NetApp.GetCounter()->Get();

				char sendbuf[64];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_GET_INCOME);
				writestream.Write(wseq);
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_SEND_PC_INFO:
			{
				VA_PCINFO * info = (VA_PCINFO *)(_msg.v);

				char sendbuf[ MAXLEN_PCINFO + 16 ];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_UP_NETSTATUS);
				writestream.Write( (int)1 );
				writestream.Write( info->buf, info->len );
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_INVITE_MEMBER:
			{
				int * p_idx = ( int * )(_msg.v);

				char sendbuf[ MAXLEN_PCINFO + 16 ];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_INVITE_MEMBER);
				writestream.Write( _msg.wseq );
				writestream.Write( * p_idx );
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_INVITE_REPLY:
			{
				char * p_type = ( char * )(_msg.v);

				char sendbuf[ MAXLEN_PCINFO + 16 ];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_INVITE_REPLY);
				writestream.Write( _msg.wseq );
				writestream.Write( * p_type );
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_GET_GIFTSEND:
			{
				//char * p_type = ( char * )(_msg.v);

				char sendbuf[ 32 ];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_GET_GIFTSEND);
				writestream.Write( (int)1 );
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_GET_GIFTRECV:
			{
				//char * p_type = ( char * )(_msg.v);

				char sendbuf[ 32 ];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_GET_GIFTRECV);
				writestream.Write( (int)1 );
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_KICK_BY_MAC:
			{
				int * p_idx = ( int * )(_msg.v);

				char sendbuf[ 32 ];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_DISABLE_MACADDR);
				writestream.Write( (int)1 );
				writestream.Write( * p_idx );
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_KICK_BY_IP:
			{
				int * p_idx = ( int * )(_msg.v);

				char sendbuf[ 32 ];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_DISABLE_IPADDR);
				writestream.Write( (int)1 );
				writestream.Write( * p_idx );
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_SET_ONMIC_TIME:
			{
				VA_SET_MICTIME * pTmp = ( VA_SET_MICTIME * )(_msg.v);

				char sendbuf[ 32 ];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_ADD_TM);
				writestream.Write( (int)1 );
				writestream.Write( pTmp->idx );
				writestream.Write( pTmp->time );
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_ROOM_SET_SONG_TIME:
			{
				short * pTime = ( short * )(_msg.v);

				char sendbuf[ 32 ];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_UPDATE_MUSICTIME);
				writestream.Write( (int)1 );
				writestream.Write( * pTime );
				writestream.flush();

				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
		case VATYPE_OPEN_PRIVATEMIC:
			{
				short* type = (short *)(_msg.v);

				char sendbuf[32];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_ON_PRIVATE_MIC_C2S2C);
				writestream.Write((int)0);
				writestream.Write(*type);
				writestream.flush();

				if (!g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					//TRACE_INFO((Error, "send ROOM_CMD_ON_PRIVATE_MIC_C2S2C error");
				}
			}
			break;
		case VATYPE_CLOSE_PRIVATEMIC:
			{
				char sendbuf[32];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_OFF_PRIVATE_MIC_C2S2C);
				writestream.Write((int)0);
				writestream.flush();

				if (!g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					//TRACE_INFO((Error, "send VATYPE_CLOSE_PRIVATEMIC error");
				}
			}
			break;
		case VATYPE_CLOSED_PRIVATEMIC:
			{
				int* idx = (int *)(_msg.v);

				char sendbuf[32];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_GIVEOFF_PRIVATE_MIC_C2S2C);
				writestream.Write((int)0);
				writestream.Write(*idx);
				writestream.flush();

				if (!g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					//TRACE_INFO((Error, "send ROOM_CMD_GIVEOFF_PRIVATE_MIC_C2S2C error");
				}
			}
			break;
		case VATYPE_REQUEST_WATCH_PRIVATEMIC:
			{
				int* idx = (int *)(_msg.v);
				
				char sendbuf[32];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_REQUEST_PRIVATE_MIC_C2S2C);
				writestream.Write((int)0);
				writestream.Write(*idx);
				writestream.flush();

				if (!g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					//TRACE_INFO((Error, "send ROOM_CMD_REQUEST_PRIVATE_MIC_C2S2C error");
				}
			}
			break;
		case VATYPE_RESPONSE_WATCH_PRIVATEMIC:
			{
				int* param = (int *)(_msg.v);
				int allow = *param;
				int idx = *(param+1);

				char sendbuf[32];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_REQUEST_ACK_PRIVATE_MIC_C2S2C);
				writestream.Write((int)0);
				writestream.Write(allow);
				writestream.Write(idx);
				writestream.flush();

				if (!g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					//TRACE_INFO((Error, "send ROOM_CMD_REQUEST_ACK_PRIVATE_MIC_C2S2C error");
				}
			}
			break;
		case VATYPE_INVITEPRIVATEMIC:
			{
				int* param = (int*)(_msg.v);
				int num = *param;

				char sendbuf[2048];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_INVITE_PRIVATE_MIC_C2S);
				writestream.Write((int)0);
				writestream.Write(num);
				for (int i=1; i<= num; i++)
				{
					writestream.Write(param[i]);
				}
				writestream.flush();

				if (!g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					//TRACE_INFO((Error, "send ROOM_CMD_INVITE_PRIVATE_MIC_C2S error");
				}
			}
			break;
		case VATYPE_RESPONSE_INVITEPRIVATEMIC:
			{
				int* param = (int *)(_msg.v);
				int allow = *param;
				int idx = *(param+1);

				char sendbuf[32];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_INVITE_ACK_PRIVATE_MIC_C2S);
				writestream.Write((int)0);
				writestream.Write(allow);
				writestream.Write(idx);
				writestream.flush();

				//TRACE_INFO((Debug, "ROOM_CMD_INVITE_ACK_PRIVATE_MIC_C2S allow=%d, idx=%d", allow, idx);

				if (!g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					//TRACE_INFO((Error, "send ROOM_CMD_INVITE_ACK_PRIVATE_MIC_C2S error");
				}
			}
			break;
		case VATYPE_OPEN_ROOMPRIVATEMIC_FLAG:
			{
				char* param = (char *)(_msg.v);
				char flag = *param;

				char sendbuf[32];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_UPDATE_PRIVATEMIC);
				writestream.Write((int)0);
				writestream.Write(flag);
				writestream.flush();

				if (!g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					//TRACE_INFO((Error, "send ROOM_CMD_UPDATE_PRIVATEMIC error");
				}
			}
			break;
		case VATYPE_CROWN_KICKOUT_SOMEONE:
			{
				int* idx = (int *)(_msg.v);

				char sendbuf[32];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_CROWN_KICKOUT_SOMEONE);
				writestream.Write((int)0);
				writestream.Write(*idx);
				writestream.flush();

				if (!g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					//TRACE_INFO((Error, "send ROOM_CMD_CROWN_KICKOUT_SOMEONE error");
				}
			}
			break;
		case VATYPE_FORBID_USERIP:
			{
				int* idx = (int *)(_msg.v);

				char sendbuf[32];
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_DISABLEIP_BYROOM);
				writestream.Write((int)0);
				writestream.Write(*idx);
				writestream.flush();

				if (!g_NetApp.GetRoomTunnel()->SendRightBuf(writestream))
				{
					//TRACE_INFO((Error, "send ROOM_CMD_DISABLEIP_BYROOM error");
				}
			}
			break;
		case VATYPE_GETFAMILYINFO:
			{
				char sendbuf[32];
				NetworkUtil::BinaryWriteStream writestream(sendbuf, sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)CMD_GET_FAMILYINFO_ALL_C2L);
				writestream.Write((int)0);
				writestream.flush();

				if (!g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
				{
					//TRACE_INFO((Error, "send CMD_GET_FAMILYINFO_ALL_C2L error");
				}
			}
			break;
		case VATYPE_SINGER_SCORES:
			{
				VA_SEND_SCORES * pScores = (VA_SEND_SCORES*)(_msg.v);
				vector< short > * vec_socres = ( vector< short > *)pScores->scores;

				char sendbuf[65535]={0};
				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_UPLOAD_SECTIONSCORE);
				writestream.Write(wseq);
				writestream.Write(pScores->idx);
				writestream.Write((short)vec_socres->size());
				for(int i = 0;i < (int)vec_socres->size();i++)
				{
					short curscores = (*vec_socres)[i];//(*idxlist)[i]
					writestream.Write(curscores);
				}
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);

				delete vec_socres;
			}
			break;
		case VATYPE_UPDATESONG:
			{
				int * idx = (int *)(_msg.v);

				char sendbuf[32]={0};
				int wseq = g_NetApp.GetCounter()->Get();
				NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
				writestream.Write('[');
				writestream.Write((short)ROOM_CMD_UPLOAD_RECORDDATE);
				writestream.Write(wseq);
				writestream.Write( * idx );
				writestream.flush();
				g_NetApp.GetRoomTunnel()->SendRightBuf(writestream);
			}
			break;
        case KTV_GETROOMLIST_FROMGROUP:
            {
                GroupBatch* pGb = (GroupBatch*)_msg.v;
                VoidGuard g(pGb->pId);
                
                int wseq = g_NetApp.GetCounter()->Get();
                char sendbuf[1024] = {0};
                NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
                writestream.Write('T');
                writestream.Write((short)CMD_MGET_ROOMLIST_IN_GROUP_C2L2C);
                writestream.Write(wseq);
                writestream.Write(pGb->count);
                for (int i=0; i<pGb->count; i++)
                    writestream.Write(pGb->pId[i]);
                writestream.flush();
                g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
                
                break;
            }
        case KTV_MOBILE_CHAT_C2L2C:
            {
                KTVCHATINFO* info = (KTVCHATINFO*)_msg.v;
                
                if (info) {
                    char sendbuf[65535] = {0};
                    int wseq = g_NetApp.GetCounter()->Get();
                    NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
                    writestream.Write('T');
                    if (info->toIdx == 0) {//公聊
                        writestream.Write((short)CMD_MPUBLICCHAT_C2L2C);
                        writestream.Write(wseq);
                        writestream.Write(info->buf, strlen(info->buf));
                    }
                    else//私聊
                    {
                        writestream.Write((short)CMD_MPRIVATECHAT_C2L2C);
                        writestream.Write(wseq);
                        writestream.Write(info->toIdx);
                        writestream.Write(info->buf, strlen(info->buf));
                    }
                    writestream.flush();
                    if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
                    {
                        //加入回应超时列表中
                        void* mem = malloc(sizeof(ReserveData_ChatKtv));
                        if(!mem)
                            return ;
                        ReserveData_ChatKtv* pRD = new(mem) ReserveData_ChatKtv;
                        pRD->serverid = LOBBYTUNNEL;
                        pRD->cmd = (info->toIdx==0)?CMD_MPUBLICCHAT_C2L2C:CMD_MPRIVATECHAT_C2L2C;
                        pRD->seq = wseq;
                        pRD->t = time(NULL);
                        pRD->to_idx = info->toIdx;
                        strcpy(pRD->content,info->buf);
                        
                        g_NetApp.GetRDMap()->Put(wseq, pRD);
                    }
                }
                break;
            }
            case KTV_MOBILE_SENDGIFT_C2L2C:
            {
                KTV_SEND_GIFT* info = (KTV_SEND_GIFT*)_msg.v;
                if (info)
                {
                    int wseq = g_NetApp.GetCounter()->Get();
                    char sendbuf[50];
                    NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
                    writestream.Write('T');
                    writestream.Write((short)CMD_MSEND_GIFT_C2L2C);
                    writestream.Write(wseq);
                    writestream.Write(info->toIdx);
                    writestream.Write(info->giftId);
                    writestream.Write(info->count);
                    writestream.Write(info->roomId);
                    writestream.flush();
                    //加入超时列表
                    if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
                    {
                        void *mem = malloc(sizeof(ReserveData_SendGift));
                        if(mem)
                        {
                            ReserveData_SendGift *pRD = new(mem) ReserveData_SendGift;
                            pRD->serverid = LOBBYTUNNEL;
                            pRD->cmd = (short)ROOM_CMD_SEND_GIFT;
                            pRD->seq = wseq;
                            pRD->t = time(NULL);
                            pRD->r_idx=info->toIdx;
                            pRD->item_id=info->giftId;
                            pRD->count=info->count;
                            g_NetApp.GetRDMap()->Put(wseq,pRD);//推入超时列表
                        }
                    }
                }
                break;
            }
			case VATYPE_GETUSERINFO_FORHORN_C2L2C:
			{
				IdxArray* p = (IdxArray*)_msg.v;

				SafePointer sp(p->idx);
				if(p && p->idx)
				{
					char sendbuf[65535]={0};
					int wseq = g_NetApp.GetCounter()->Get();
					NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
					writestream.Write('T');
					writestream.Write((short)CMD_GETUSERINFOFORSPEAKER_C2L2C);
					writestream.Write(wseq);
					writestream.Write(p->count);
					for(int i = 0;i < p->count;i++)
						writestream.Write(p->idx[i]);
					writestream.flush();
					if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
					{
						void *mem = malloc(sizeof(ReserveData_GetUserInfo));
						if(mem)
						{
							ReserveData_GetUserInfo *pRD = new(mem) ReserveData_GetUserInfo;
							pRD->serverid = LOBBYTUNNEL;
							pRD->cmd = (short)CMD_GETUSERINFOFORSPEAKER_C2L2C;
							pRD->seq = wseq;
							pRD->t = time(NULL);
							assert(p->count <= IDX_MAX_LEN);
							memcpy(pRD->idx,p->idx,sizeof(int)*p->count);
							pRD->count = p->count;
							g_NetApp.GetRDMap()->Put(wseq,pRD);//推入超时列表
						}
					}
				}
				break;
			}
			case VATYPE_GETUSERINFO_WITHVERSION_C2L2C:
			{
				IdxVersionArray* p = (IdxVersionArray*)_msg.v;

				SafePointer sp(p->idx);
				if(p && p->idx)
				{
					char sendbuf[2000]={0};
					int wseq = g_NetApp.GetCounter()->Get();
					NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
					writestream.Write('T');
					writestream.Write((short)CMD_GETUSERINFOWITHVERSION_C2L2C);
					writestream.Write(wseq);
					writestream.Write(p->count);

					void *mem = malloc(sizeof(ReserveData_GetUserInfo));
					ReserveData_GetUserInfo *pRD = NULL;
					if(mem)
						pRD = new(mem) ReserveData_GetUserInfo;
					
					assert(p->count <= IDX_MAX_LEN);
					for(int i = 0;i < p->count;i++)
					{
						writestream.Write(p->idx[i].id);
						writestream.Write(p->idx[i].version);
						if(pRD)
							pRD->idx[i] = p->idx[i].id;
					}
					writestream.flush();
					if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
					{
						if(pRD)
						{
							pRD->serverid = LOBBYTUNNEL;
							pRD->cmd = (short)CMD_GETUSERINFOWITHVERSION_C2L2C;
							pRD->seq = wseq;
							pRD->t = time(NULL);
							pRD->count = p->count;
							g_NetApp.GetRDMap()->Put(wseq,pRD);//推入超时列表
						}
					}
				}
				break;
			}
			case KTV_MOBILE_SENDHB_C2L2C:
				{
					SendHB* pData = (SendHB*) _msg.v;

					if(pData)
					{
						short request_cmd = (short)CMD_SEND_REDGOLD_C2L2C;

						char sendbuf[500]={0};
						int wseq = g_NetApp.GetCounter()->Get();
						NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
						writestream.Write('T');
						writestream.Write(request_cmd);
						writestream.Write(wseq);
						writestream.Write(pData->hbtype);
						writestream.Write(pData->number);
						writestream.Write(pData->gold);
						writestream.Write(pData->mark,strlen(pData->mark));
						writestream.Write(pData->extradata);
						writestream.flush();

						if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
						{
							ReserveData* pRD = NULL;
							void* mem = malloc(sizeof(ReserveData));
							if(mem)
								pRD = new(mem) ReserveData;

							if(pRD)
							{
								pRD->serverid = LOBBYTUNNEL;
								pRD->cmd = request_cmd;
								pRD->seq = wseq;
								pRD->t = time(NULL);
								g_NetApp.GetRDMap()->Put(wseq,pRD);//推入超时列表
							}
						}
					}
					break;
				}
			case KTV_MOBILE_LOOKHB_C2L2C:
				{
					const char* pData = (const char*) _msg.v;
					if(pData)
					{
						const char* hbid = pData;

						short send_cmd = (short)CMD_CAN_GET_RED_C2L2C;
						int server_id = LOBBYTUNNEL;

						char sendbuf[500]={0};
						int wseq = g_NetApp.GetCounter()->Get();
						NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
						writestream.Write('T');
						writestream.Write(send_cmd);
						writestream.Write(wseq);
						writestream.Write(hbid,strlen(hbid));
						writestream.flush();

						bool ret = g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);

						if(ret )
						{
							ReserveData* pRD = NULL;
							void* mem = malloc(sizeof(ReserveData));
							if(mem)
								pRD = new(mem) ReserveData;

							if(pRD)
							{
								pRD->serverid = server_id;
								pRD->cmd = send_cmd;
								pRD->seq = wseq;
								pRD->t = time(NULL);
								g_NetApp.GetRDMap()->Put(wseq,pRD);//推入超时列表
							}
						}
					}
					break;
				}
			case KTV_MOBILE_VIEHB_C2L2C:
				{
					const char* pData = (const char*) _msg.v;
					if(pData)
					{
						const char* hbid = pData;

						short send_cmd = (short)CMD_VIEFOR_REDGOLD_C2L2C;
						int server_id = LOBBYTUNNEL;

						char sendbuf[500]={0};
						int wseq = g_NetApp.GetCounter()->Get();
						NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
						writestream.Write('T');
						writestream.Write(send_cmd);
						writestream.Write(wseq);
						writestream.Write(hbid,strlen(hbid));
						writestream.flush();

						bool ret = g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream);
						
						if(ret )
						{
							ReserveData* pRD = NULL;
							void* mem = malloc(sizeof(ReserveData));
							if(mem)
								pRD = new(mem) ReserveData;

							if(pRD)
							{
								pRD->serverid = server_id;
								pRD->cmd = send_cmd;
								pRD->seq = wseq;
								pRD->t = time(NULL);
								g_NetApp.GetRDMap()->Put(wseq,pRD);//推入超时列表
							}
						}
					}
					break;
				}
			case KTV_MOBILE_HBDETAIL_C2L2C:
				{
					const char* pData = (const char*) _msg.v;
					if(pData)
					{
						char sendbuf[500]={0};
						int wseq = g_NetApp.GetCounter()->Get();
						NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
						writestream.Write('T');
						writestream.Write((short)CMD_WATCH_REDGOLD_C2L2C);
						writestream.Write(wseq);
						writestream.Write(pData,strlen(pData));
						writestream.flush();

						if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
						{
							ReserveData* pRD = NULL;
							void* mem = malloc(sizeof(ReserveData));
							if(mem)
								pRD = new(mem) ReserveData;

							if(pRD)
							{
								pRD->serverid = LOBBYTUNNEL;
								pRD->cmd = CMD_WATCH_REDGOLD_C2L2C;
								pRD->seq = wseq;
								pRD->t = time(NULL);
								g_NetApp.GetRDMap()->Put(wseq,pRD);//推入超时列表
							}
						}
					}
					break;
				}
			case KTV_MOBILE_ROOMHB_C2L2C:
				{
					int* roomid = (int*)_msg.v;
					if(roomid)
					{
						char sendbuf[500]={0};
						int wseq = g_NetApp.GetCounter()->Get();
						NetworkUtil::BinaryWriteStream writestream(sendbuf,sizeof(sendbuf));
						writestream.Write('T');
						writestream.Write((short)CMD_GET_ROOM_REDPACKET_C2L2C);
						writestream.Write(wseq);
						writestream.Write(*roomid);
						writestream.flush();

						if(g_NetApp.GetLobbyTunnel()->SendRightBuf(writestream))
						{
							ReserveData* pRD = NULL;
							void* mem = malloc(sizeof(ReserveData));
							if(mem)
								pRD = new(mem) ReserveData;

							if(pRD)
							{
								pRD->serverid = LOBBYTUNNEL;
								pRD->cmd = CMD_GET_ROOM_REDPACKET_C2L2C;
								pRD->seq = wseq;
								pRD->t = time(NULL);
								g_NetApp.GetRDMap()->Put(wseq,pRD);//推入超时列表
							}
						}
					}
					break;
				}
			//////////////////////////////////////////////////////////////////////////
		default:
		    break;
		}
	}
}


///////////////////////////////////////////////////////////////////////

bool CAvaratData::ChangeData( void * pBuf, ResultData_Avatar_Updata * avarat )
{
	if( NULL == avarat )
	{
		return false;
	}

	NetworkUtil::BinaryWriteStream * pTmpBuf = ( NetworkUtil::BinaryWriteStream * )pBuf;

	pTmpBuf->Write( avarat->len );

	char buf[128];

	int k = 0;
	for( int i=0; i<PART_LEN+LINK_LEN; ++i )
	{
		int key = avarat->count[i][COUNT_KEY];
		int cnt = avarat->count[i][COUNT_LEN];
		for( int j=0; j<cnt; ++j )
		{
			pTmpBuf->Write( avarat->goods_id[k] );
			sprintf( buf, "%d:", avarat->goods_id[k] );
			pTmpBuf->Write( (short)key );
			sprintf( buf, "%d", (short)key );
			if( 0 != avarat->global_id[k] )
			{
				pTmpBuf->Write( avarat->global_id[k] );
				sprintf( buf, ":%d", avarat->global_id[k] );
			}
			++k;
			if( key == avarat->len )
			{
				return true;
			}
		}
	}

	return false;
}
