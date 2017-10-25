#include "DataDecoderRoom.h"
#include "NetApp.h"
#include "wrap/NUtility.h"
#include "protocol.h"
#include "wrap/crypt.h"
#include "XingguangDef.h"
#include "ResultData.h"
//#include "testapi.h"
//#define TESTAPI
#include "ResponseBase.h"
#include "localstorage/DataMgr.h"
#include <vector>

#include "wrap/config.h"
#ifndef NETUTIL_ANDROID
#define LOGI printf
#define LOGE printf
#endif

extern CDataMgr g_DM;

#ifndef PROTOCOL_READ
#define PROTOCOL_READ(x) \
	if(!stream.Read((x)))\
{\
	LOGE("Protocol Read %s, Error in file: %s,line %d\n",#x,__FILE__,__LINE__);\
	return -1;\
}
#endif//STREAM_READ_ERROR

#ifndef PROTOCOL_READSTR
#define PROTOCOL_READSTR(x,len)\
{\
	if(!stream.Read((x),sizeof(x),len))\
{\
	LOGE("Protocol Read %s, Error in file: %s,line %d\n",#x,__FILE__,__LINE__);\
	return -1;\
}\
	x[len] = 0;\
}

#endif//STREAM_READ_STR_ERROR

extern void GetAvIpFromString( char out_ip[][MAXLEN_IP], char & count, char * ip_buf, char key );

int RoomDataDecoder::onPackage1(short cmd, int seq, void * pStream, void * pTunnel)
{
	BinaryReadStream & stream = * (BinaryReadStream *)pStream;
	RoomTunnel * pRoomTunnel = (RoomTunnel *)pTunnel;

	ResponseBase* pRb = g_NetApp.GetResponseHandler();
	//////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_TEST__
	ResponseBase *pRbTmp=g_NetApp.GetResponseHandler();
	if (pRbTmp )
	{
		pRbTmp->AddLog("__DEBUG_TEST__  room cmd is %d",cmd);
	}	
#endif
	//////////////////////////////////////////////////////////////////////////

	switch(cmd)
	{
	case ROOM_CMD_TOKEN:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
				return -1;

			RDGuard guard(*pRD,seq);

			int ret = 0;
			if( ! stream.Read( ret ) )
				return -1;

			if( 0 == ret )
			{
				char sessionkey[16] = {0};
				unsigned int sessionkeylen;
				if(!stream.Read(sessionkey,sizeof(sessionkey),sessionkeylen))
				{
					return -1;
				}

				memset(pRoomTunnel->m_Sessionkey,0,sizeof(pRoomTunnel->m_Sessionkey));
				strncpy(pRoomTunnel->m_Sessionkey,sessionkey,sizeof(pRoomTunnel->m_Sessionkey));
				pRoomTunnel->SetTokenLogin(true);

				/*OnPackage2 *pPackageHandler = g_NetApp.GetRoomPackageHandlerMap()->Get((int)cmd);
				if(pPackageHandler)
				{
					(*pPackageHandler)((void*)*pRD,NULL);
				}*/
#ifdef TESTAPI
				//MessageBox(NULL,"认证成功","",MB_OK);
				inroom();
#endif
				//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
				//if(op)
				//{
				//	op((ReserveDataS*)*pRD,(int)cmd,NULL);
				//}
			}

			if (pRb)
			{
				switch( ret )
				{
				case 0:	//成功
					pRb->RecvTokenAuth4Room( ret, ROOM_NORMAL );
					break;

				case -1: //失败,token超时
				case -2: //失败,该用户未登陆大厅
					LOGE("room token failed ret = %d",ret);
					pRb->RecvTokenAuth4Room( ret, ROOM_TOKEN_ERROR );
					break;
				}
			}
		}
		break;
	case ROOM_CMD_LOGIN:
		{
			LOGI("recv room cmd ROOM_CMD_LOGIN");
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);

			char ret = LOGIN_NONE;
			PROTOCOL_READ(ret);

			char identity = USER_ID_NONE;
			if(ret == LOGIN_SUCESS)
				PROTOCOL_READ(identity);

			ResultData_LoginInfo info;
			info.ret = ret;
			info.identity = identity;
#ifdef TESTAPI
			//MessageBox(NULL,"登录房间成功","",MB_OK);
			SetEvent(room_event);
#endif
			if (pRb)
			{
				switch(ret)
				{
				case LOGIN_NONE:
					pRb->RecvEnterRoom(-1,ROOM_LOGIN_NONE,&info,(*pRD)->cseq);
					break;
				case LOGIN_SUCESS:
					g_DM.changeMyRoom((*pRD)->cseq);
					pRb->RecvEnterRoom(0,ROOM_LOGIN_SUCCESS,&info,(*pRD)->cseq);
					break;
				case LOGIN_AGAIN:
					pRb->RecvEnterRoom(-1,ROOM_LOGIN_AGAIN,&info,(*pRD)->cseq);
					break;
				case LOGIN_NOUIN:
					pRb->RecvEnterRoom(-1,ROOM_LOGIN_NOUIN,&info,(*pRD)->cseq);
					break;
				case LOGIN_FULL:
					pRb->RecvEnterRoom(-1,ROOM_LOGIN_FULL,&info,(*pRD)->cseq);
					break;
				case LOGIN_IFULL:
					pRb->RecvEnterRoom(-1,ROOM_LOGIN_IFULL,&info,(*pRD)->cseq);
					break;
				case LOGIN_EPWD:
					pRb->RecvEnterRoom(-1,ROOM_LOGIN_EPWD,&info,(*pRD)->cseq);
					break;
				case LOGIN_CLOSE:
					pRb->RecvEnterRoom(-1,ROOM_LOGIN_CLOSE,&info,(*pRD)->cseq);
					break;
				case LOGIN_INBLACK:
					pRb->RecvEnterRoom(-1,ROOM_LOGIN_INBLACK,&info,(*pRD)->cseq);
					break;
				case LOGIN_INTBLACK:
					pRb->RecvEnterRoom(-1,ROOM_LOGIN_INTBLACK,&info,(*pRD)->cseq);
					break;
				case LOGIN_NOTEXIST://LOGIN_NOROOM
					pRb->RecvEnterRoom(-1,ROOM_LOGIN_NOTHIS,&info,(*pRD)->cseq);
					break;
				case LOGIN_LOCKED:
					pRb->RecvEnterRoom(-1,ROOM_LOGIN_LOCK,&info,(*pRD)->cseq);
					break;
				case LOGIN_IP_INVALIED:
					pRb->RecvEnterRoom(-1, ROOM_LOGIN_IP_INVALIED, &info, (*pRD)->cseq);
					break;
				}
				if( LOGIN_SUCESS != ret )
				{
					memset( g_NetApp.GetRoomTunnel()->m_Sessionkey, 0, sizeof( g_NetApp.GetRoomTunnel()->m_Sessionkey ));
					g_NetApp.GetRoomTunnel()->SetTokenLogin( false );
				}
			}
		}
		break;
	case ROOM_CMD_SB_ENTERROOM:
		{
			int idx = 0;
			if(!stream.Read(idx))
				return -1;

			char identity = USER_ID_NONE;
			if(!stream.Read(identity))
				return -1;

			ResultData_UserInfo usrinfo;
			usrinfo.idx = idx;
			usrinfo.identity = identity;


			//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			//if(op)
			//{
			//	op(NULL,(int)cmd, (void*)&usrinfo);
			//}
			if (pRb)
			{
				pRb->RecvEnterRoomNotice(0,&usrinfo);
			}
		}
		break;
	case ROOM_CMD_SB_LEAVEROOM:
		{
			int idx = 0;
			if(!stream.Read(idx))
				return -1;

			g_DM.changeRoom(idx,0);
			if (pRb)
			{
				pRb->RecvLeaveRoomNotice(0,idx);
			}
			if( g_NetApp.GetMyIDx() == idx )
			{
				memset( g_NetApp.GetRoomTunnel()->m_Sessionkey, 0, sizeof( g_NetApp.GetRoomTunnel()->m_Sessionkey ));
				g_NetApp.GetRoomTunnel()->SetTokenLogin( false );
			}
		}
		break;
	case ROOM_CMD_GET_ALLINFO:
		{
			ResultData_RoomAllInfo allinfo;//ROOM_ALL_INFO allinfo;

			short num = 0;
			if(!stream.Read(num))
				return -1;

			allinfo.usercount = num;

			if(NULL == (allinfo.userlist = (int *)malloc(num * sizeof(int))))
				return -1;

			//userlist
			for(int i = 0; i < num; i++)
			{
				int idx;
				if(!stream.Read(idx))
					return -1;
				allinfo.userlist[i]=idx;
			}

			short micnum = 0;
			if(!stream.Read(micnum))
			{
				//////TRACE_INFO((Error, "read micnum error");
				return -1;
			}
			allinfo.miccount = micnum;

			//等待麦列表和上麦同时分配
			ResultData_MicInfo *waitmiclist;
			if(NULL == (waitmiclist = (ResultData_MicInfo *)malloc((micnum + 1) * sizeof(ResultData_MicInfo))))
			{
				////TRACE_INFO((Error, "memory err");
				return -1;
			}
			allinfo.pMLI = waitmiclist;
			VoidGuard MLI(waitmiclist);

			unsigned int len = 0;

			//miclist
			for(int i = 0; i < micnum; i++)
			{
				if(!stream.Read(waitmiclist->idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}

				if(!stream.Read(waitmiclist->musicid))
				{
					////TRACE_INFO((Error, "read musicid error");
					return -1;
				}

				if(!stream.Read(waitmiclist->bk))
				{
					////TRACE_INFO((Error, "read bk error");
					return -1;
				}

				waitmiclist->state = 0;

				if(!stream.Read(waitmiclist->pkidx))
				{
					////TRACE_INFO((Error, "read pkidx error");
					return -1;
				}

				if(!stream.Read(waitmiclist->pkmusicid))
				{
					////TRACE_INFO((Error, "read pkmusicid error");
					return -1;
				}

				if(!stream.Read(waitmiclist->pkbk))
				{
					////TRACE_INFO((Error, "read pkbk error");
					return -1;
				}

				waitmiclist->pkstate = 0;

				if(!stream.Read(waitmiclist->level))
				{
					////TRACE_INFO((Error, "read level error");
					return -1;
				}

				if( !stream.Read( waitmiclist->name, sizeof( waitmiclist->name ), len ) )
				{
					////TRACE_INFO((Error, "read name error");
					return -1;
				}
				waitmiclist->name[ len ] = 0;

				if(!stream.Read(waitmiclist->speed))
				{
					////TRACE_INFO((Error, "read pkbk error");
					return -1;
				}

				//if(!stream.Read(waitmiclist->change))
				//{
				//	////TRACE_INFO((Error, "read pkbk error");
				//	return -1;
				//}

				waitmiclist++;
			}

			//vjonmic
			if(!stream.Read(allinfo.vjonmic))
			{
				////TRACE_INFO((Error, "read vj idx error");
				return -1;
			}

			//onmic
			ResultData_MicInfo *ponmic = allinfo.pMLI + allinfo.miccount;
			allinfo.ponmic = ponmic;
			if(!stream.Read(ponmic->idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			if(!stream.Read(ponmic->musicid))
			{
				////TRACE_INFO((Error, "read musicid error");
				return -1;
			}

			if(!stream.Read(ponmic->bk))
			{
				////TRACE_INFO((Error, "read bk error");
				return -1;
			}

			//add by cx 10-6-5
			ponmic->state = 0;
			if(!stream.Read(ponmic->state))
			{
				////TRACE_INFO((Error, "read state error");
				return -1;
			}

			if(!stream.Read(ponmic->pkidx))
			{
				////TRACE_INFO((Error, "read pkidx error");
				return -1;
			}

			if(!stream.Read(ponmic->pkmusicid))
			{
				////TRACE_INFO((Error, "read pkmusicid error");
				return -1;
			}

			if(!stream.Read(ponmic->pkbk))
			{
				////TRACE_INFO((Error, "read pkbk error");
				return -1;
			}

			//add by cx 10-6-5
			ponmic->pkstate = 0;
			if(!stream.Read(ponmic->pkstate))
			{
				////TRACE_INFO((Error, "read pkstate error");
				return -1;
			}

			if(!stream.Read(ponmic->level))
			{
				////TRACE_INFO((Error, "read pkbk error");
				return -1;
			}

			if( !stream.Read( ponmic->name, sizeof( ponmic->name ), len ) )
			{
				////TRACE_INFO((Error, "read name error");
				return -1;
			}
			ponmic->name[ len ] = 0;

			if(!stream.Read( ponmic->speed ))
			{
				////TRACE_INFO((Error, "read speed error");
				return -1;
			}

			//if(!stream.Read( ponmic->change ))
			//{
			//	////TRACE_INFO((Error, "read change error");
			//	return -1;
			//}
			
			//add by jinguanfu 2010/4/20 managerlist 
			short managernum;
			if(!stream.Read(managernum))
			{
				////TRACE_INFO((Error, "read managernum error");
				return -1;
			}
			allinfo.managercount=managernum;
			ResultData_UserInfo *managerinfo;
			if(NULL == (managerinfo = (ResultData_UserInfo *)malloc(managernum * sizeof(ResultData_UserInfo))))
			{
				////TRACE_INFO((Error, "memory err");
				return -1;
			}
			VoidGuard UI(managerinfo);
			allinfo.pMI = managerinfo;

			for(int i=0;i<managernum;i++)
			{	
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				char identity;
				if (!stream.Read(identity))
				{
					////TRACE_INFO((Error, "read identity error");
					return -1;
				}
				managerinfo[i].idx=idx;
				managerinfo[i].identity=identity;
			}

			unsigned int length;

			//房间公告
			char content[65535];
			allinfo.roomcontent = content;
			if(!stream.Read(content, sizeof(content), length))
			{
				////TRACE_INFO((Error, "read pkbk error");
				return -1;
			}
			content[length] = 0;

			//////////////////////////////////////////////////////////////////////////
			char ip_buf[ MAXLEN_ROOM_AVIPCOUNT * MAXLEN_IP ];

			if(!stream.Read( ip_buf, sizeof( ip_buf ), length ))
			{
				////TRACE_INFO((Error, "read ip telcom error");
				return -1;
			}
			ip_buf[length] = 0;
			GetAvIpFromString( allinfo.ip_telcom, allinfo.ip_telcom_count, ip_buf, '|' );

			if( !stream.Read( allinfo.port_telcom ))
			{
				////TRACE_INFO((Error, "read port telcom error");
				return -1;
			}

			if(!stream.Read( ip_buf, sizeof( ip_buf ), length ))
			{
				////TRACE_INFO((Error, "read ip netcom error");
				return -1;
			}
			ip_buf[length] = 0;
			GetAvIpFromString( allinfo.ip_netcom, allinfo.ip_netcom_count, ip_buf, '|' );

			if( !stream.Read( allinfo.port_netcom ))
			{
				////TRACE_INFO((Error, "read port netcom error");
				return -1;
			}

			//////////////////////////////////////////////////////////////////////////

			//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			//if(op)
			//{
			//	op(NULL,(int)cmd, (void*)&allinfo);
			//}
			if (pRb)
				pRb->RecvRoomAllInfo(0,ROOM_NORMAL,&allinfo,0);
		}
		break;
	case ROOM_CMD_SB_WAITMIC:
		{
			ResultData_MicInfo micinfo;
			memset( & micinfo, 0, sizeof( ResultData_MicInfo ) );

			if(!stream.Read(micinfo.idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			if(!stream.Read(micinfo.musicid))
			{
				////TRACE_INFO((Error, "read musicid error");
				return -1;
			}

			if(!stream.Read(micinfo.bk))
			{
				////TRACE_INFO((Error, "read bk error");
				return -1;
			}

			if(!stream.Read(micinfo.level))
			{
				////TRACE_INFO((Error, "read level error");
				return -1;
			}

			unsigned int len = 0;
			if( !stream.Read( micinfo.name, sizeof( micinfo.name ), len ) )
			{
				////TRACE_INFO((Error, "read name error");
				return -1;
			}
			micinfo.name[ len ] = 0;

			if( !stream.Read( micinfo.speed ) )
			{
				////TRACE_INFO((Error, "read name error");
				return -1;
			}

			if (pRb)
				pRb->RecvWaitMicNotice(0,&micinfo);
		}
		break;
	//case ROOM_CMD_SB_ONMIC_READY:
	//	break;
	case ROOM_CMD_SB_ONMIC:
		{
			ResultData_MicInfo onmic;
			if(!stream.Read(onmic.idx))
				return -1;

			if(!stream.Read(onmic.musicid))
			{
				////TRACE_INFO((Error, "read musicid error");
				return -1;
			}

			if(!stream.Read(onmic.bk))
			{
				////TRACE_INFO((Error, "read bk error");
				return -1;
			}

			if(!stream.Read(onmic.pkidx))
			{
				////TRACE_INFO((Error, "read pkidx error");
				return -1;
			}

			if(!stream.Read(onmic.pkmusicid))
			{
				////TRACE_INFO((Error, "read pkmusicid error");
				return -1;
			}

			if(!stream.Read(onmic.pkbk))
			{
				////TRACE_INFO((Error, "read pkbk error");
				return -1;
			}

			if(!stream.Read(onmic.level))
			{
				////TRACE_INFO((Error, "read level error");
				return -1;
			}

			unsigned int len = 0;
			if( !stream.Read( onmic.name, sizeof( onmic.name ), len ) )
			{
				////TRACE_INFO((Error, "read name error");
				return -1;
			}
			onmic.name[ len ] = 0;

			if(!stream.Read(onmic.speed))
			{
				////TRACE_INFO((Error, "read level error");
				return -1;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvOnMicNotice(0,&onmic);
			}
		}
		break;
	//下麦准备过程取消
	//case ROOM_CMD_SB_OFFMIC_READYOK:
	//	{
	//		int idx = 0;
	//		if(!stream.Read(idx))
	//		{
	//			////TRACE_INFO((Error, "read idx error");
	//			return -1;
	//		}

	//		//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
	//		//if(op)
	//		//{
	//		//	op(NULL,(int)cmd, (void*)&idx);
	//		//}
	//	}
	//	break;
	//modify by jinguanfu 2010/1/7
	case ROOM_CMD_SB_OFFMIC:
		{
			if (pRb==NULL)
				return 0;

			char haspk;
			if(!stream.Read(haspk))
			{
				////TRACE_INFO((Error, "read haspk error");
				return -1;
			}

			int idx;
			if(!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			int pkidx = 0;
			//if(!stream.Read(pkidx))
			//{
			//	////TRACE_INFO((Error, "read haspk error");
			//	return -1;
			//}

			int exp = 0;	 //新增经验
			if(!stream.Read(exp))
			{
				////TRACE_INFO((Error, "read exp error");
				return -1;
			}

			char scoreinfo[1024]={0};
			unsigned int len=0;
			if(!stream.Read(scoreinfo,sizeof(scoreinfo),len))
			{
				////TRACE_INFO((Error, "read scoreinfo error");
				return -1;
			}

			pRb->RecvNotice4OffMic( idx, pkidx, exp, scoreinfo, len );
		}
		break;
	case ROOM_CMD_SB_SCORE:
		//{
		//	int epchange = 0;
		//	if(!stream.Read(epchange))
		//	{
		//		////TRACE_INFO((Error, "read epchage error");
		//		return -1;
		//	}
		//	int silverchange = 0;
		//	if(!stream.Read(silverchange))
		//	{
		//		////TRACE_INFO((Error, "read silverchange error");
		//		return -1;
		//	}

		//	ResponseBase *pRb=g_NetApp.GetResponseHandler();
		//	if (pRb)
		//	{
		//		pRb->RecvNotice4ScoreAdd( epchange, silverchange );
		//	}
		//}
		break;
		// add by cx 10-6-2
	case ROOM_CMD_GIVE_VJ_MIC:
		{
			int idx = 0;
			if(!stream.Read(idx))
			{
				return -1;
			}

			char flag = 0;
			if(!stream.Read(flag))
			{
				return -1;
			}
			
			if( NULL != pRb )
				pRb->RecvRoomGivOnevjMic( idx, flag );
		}
		break;
	case ROOM_CMD_REQUEST_GIVE_VJ_MIC:
		{
			int fromidx = 0;
			if( ! stream.Read( fromidx ))
			{
				return -1;
			}

			if( NULL != pRb )
				pRb->RecvRoomGivOnevjMicApply( fromidx );
		}
		break;
		//add by jinguanfu 2010/1/7 打分通知
	case ROOM_CMD_SB_CANCLE_WAITMIC:
		{
			// add by cx 10-5-13
			int opidx = 0;
			if(!stream.Read(opidx))
			{
				////TRACE_INFO((Error, "read opidx error");
				return -1;
			}

			int bopidx = 0;
			if(!stream.Read(bopidx))
			{
				////TRACE_INFO((Error, "read bopidx error");
				return -1;
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			//if(op)
			//{
			//	op(NULL,(int)cmd, (void*)&idx);
			//}
			if (pRb)
				pRb->RecvCancelWaitMicNotice(0,opidx,bopidx);
		}
		break;
	case ROOM_CMD_SB_WAITMIC_PK:
		{
			ResultData_MicInfo onmicpk;
			if(!stream.Read(onmicpk.idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			if(!stream.Read(onmicpk.musicid))
			{
				////TRACE_INFO((Error, "read musicid error");
				return -1;
			}

			if(!stream.Read(onmicpk.bk))
			{
				////TRACE_INFO((Error, "read bk error");
				return -1;
			}

			if(!stream.Read(onmicpk.pkidx))
			{
				////TRACE_INFO((Error, "read pkidx error");
				return -1;
			}

			if(!stream.Read(onmicpk.pkmusicid))
			{
				////TRACE_INFO((Error, "read pkmusicid error");
				return -1;
			}

			if(!stream.Read(onmicpk.pkbk))
			{
				////TRACE_INFO((Error, "read pkbk error");
				return -1;
			}

			if(!stream.Read(onmicpk.level))
			{
				////TRACE_INFO((Error, "read level error");
				return -1;
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			//if(op)
			//{
			//	op(NULL,(int)cmd, (void*)&onmicpk);
			//}
			if (pRb)
				pRb->RecvWaitPKMicNotice(0,&onmicpk);
		}
		break;
	case ROOM_CMD_SB_CANCLE_WAITMIC_PK:
		{
			// add by cx 10-5-13
			int bopidx = 0;
			if(!stream.Read(bopidx))
			{
				////TRACE_INFO((Error, "read bopidx error");
				return -1;
			}

			int opidx;
			if(!stream.Read(opidx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			//if(op)
			//{
			//	op(NULL,(int)cmd, (void*)&idx);
			//}
			if (pRb)
				pRb->RecvCancelWaitPKMicNotice(0,opidx,bopidx);
		}
		break;

	case ROOM_CMD_SB_UP_WAITMIC:
		{
			ResultData_UDMicInfo mic;
			if(!stream.Read(mic.tag))
			{
				////TRACE_INFO((Error, "read tag error");
				return -1;
			}

			if(!stream.Read(mic.idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			if(!stream.Read(mic.flag))
			{
				////TRACE_INFO((Error, "read flag error");
				return -1;
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			//if(op)
			//{
			//	op(NULL,(int)cmd, (void*)&mic);
			//}
			if (pRb)
				pRb->RecvUpWaitMicNotice(0,&mic);
		}
		break;

	case ROOM_CMD_SB_DOWN_WAITMIC:
		{
			ResultData_UDMicInfo mic;
			if(!stream.Read(mic.tag))
			{
				////TRACE_INFO((Error, "read tag error");
				return -1;
			}

			if(!stream.Read(mic.idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			if(!stream.Read(mic.flag))
			{
				////TRACE_INFO((Error, "read flag error");
				return -1;
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			//if(op)
			//{
			//	op(NULL,(int)cmd, (void*)&mic);
			//}
			if (pRb)
			{
				pRb->RecvDownWaitMicNotice(0,&mic);
			}
		}
		break;
	case ROOM_CMD_SB_KICKOUT:
		{
			int bidx;
			if(!stream.Read(bidx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			int idx;
			if(!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			if (pRb)
			{
				pRb->RecvKickOutNotice(idx,bidx);
			}
		}
		break;
	case ROOM_CMD_SB_ONVJ_MIC:
		{
			int idx;
			if(!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}
			if (pRb)
			{
				pRb->RecvOnVJMicNotice(0,idx);
			}
		}
		break;
	case ROOM_CMD_SB_OFFVJ_MIC:
		{
			int idx;
			if(!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}
			if (pRb)
			{
				pRb->RecvOffVJMicNotice(0,idx);
			}
		}
		break;
	case ROOM_CMD_SB_GIVEOFF_MIC:
		{
			int idx;
			if(!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			//if(op)
			//{
			//	op(NULL,(int)cmd, (void*)&idx);
			//}
			if (pRb)
			{
				pRb->RecvGiveOffMicNotice(0,idx);
			}
		}
		break;
	case ROOM_CMD_SB_FORBIDEN:
		{
			ResultData_ForbidenInfo forbinfo;
			if(!stream.Read(forbinfo.idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			if(!stream.Read(forbinfo.flag))
			{
				////TRACE_INFO((Error, "read flag error");
				return -1;
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			//if(op)
			//{
			//	op(NULL,(int)cmd, (void*)&forbinfo);
			//}
			if (pRb)
			{
				pRb->RecvForbidenNotice(0,&forbinfo);
			}
		}
		break;
	//modify by jinguanfu 2009/12/21 <begin>
	case ROOM_CMD_PRIVATE_CHAT:
		{
			int from_idx;
			if(!stream.Read(from_idx))
			{
				return -1;
			}
			char content[MSG_LENGTH]={0};
			unsigned int contentlen;
			if (stream.Read(content,sizeof(content),contentlen))
			{
				return -1;
			}

			if (pRb)
				pRb->RecvChatFromUser(0,from_idx,content,contentlen,0);
			break;
		}
	case ROOM_CMD_PUBLIC_CHAT:
		{
			int ret;
			if(!stream.Read(ret))
			{
				return -1;
			}
			if (ret==1)
			{
				int from_idx;
				if(!stream.Read(from_idx))
				{
					return -1;
				}
				unsigned int contentlen;
				char content[MSG_LENGTH]={0};
				if (!stream.Read(content,sizeof(content),contentlen))
				{
					return -1;
				}
				int time = 0;
				if (!stream.Read(time))
				{
					return -1;
				}
				if (pRb)
					pRb->RecvChatFromRoom(0,ROOM_NORMAL,from_idx,content,contentlen, time );
			}
			else if(ret==0)	//被禁言
			{
				if (pRb)
					pRb->RecvChatFromRoom(-1,ROOM_ERROR_FORBIDEN,0,NULL,0, 0 );
			}
			else if(ret ==-1)//不允许公聊
			{
				if (pRb)
					pRb->RecvChatFromRoom(-1,ROOM_ERROR_NOCHAT,0,NULL,0, 0 );
			}
		}
		break;
	//modify by jinguanfu 2009/12/21 <end>
	case ROOM_CMD_SB_BLACKLIST_UPDATE:
		{
			ResultData_UpBlackList black;
			if(!stream.Read(black.idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			if(!stream.Read(black.flag))
			{
				////TRACE_INFO((Error, "read flag error");
				return -1;
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			//if(op)
			//{
			//	op(NULL,(int)cmd, (void*)&black);
			//}
			if (pRb)
			{
				pRb->RecvUpdateBlackListNotice(0,&black);
			}
		}
		break;
	case ROOM_CMD_SB_MGRLIST_UPDATE:
		{
			ResultData_UpMgrList manager;
			if(!stream.Read(manager.idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			if(!stream.Read(manager.flag))
			{
				////TRACE_INFO((Error, "read flag error");
				return -1;
			}

			if(!stream.Read(manager.identity))
			{
				////TRACE_INFO((Error, "read identity error");
				return -1;
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			//if(op)
			//{
			//	op(NULL,(int)cmd, (void*)&manager);
			//}
			if (pRb)
			{
				pRb->RecvUpdateMGRListNotice(0,&manager);
			}
		}
		break;
	case ROOM_CMD_ROOM_MGR_SUCCESS:
		{

		}
		break;
	/*comment-out by jinguanfu 2010/1/11 使用接口方式返回*/
	//case ROOM_CMD_SB_CONTENT_UPDATE:
	//	{
	//		char content[1024] = {0};
	//		unsigned int length;
	//		if(!stream.Read(content, sizeof(content), length))
	//		{
	//			////TRACE_INFO((Error, "read content error");
	//			return -1;
	//		}
	//		OnPackage2 op = g_NetApp.GetPackageHandler_Room();
	//		if(op)
	//		{
	//			op(NULL,cmd, content);
	//		}
	//	}
	//	break;
	case ROOM_CMD_SB_SEND_GIFT:
		{
			ResultData_SendFlowerInfo sendflower = {0};
			sendflower.s_idx=0;
			if(!stream.Read(sendflower.s_idx))
			{
				////TRACE_INFO((Error, "read s_idx error");
				return -1;
			}

			if(!stream.Read(sendflower.r_idx))
			{
				////TRACE_INFO((Error, "read r_idx error");
				return -1;
			}

			if(!stream.Read(sendflower.cate_idx))
			{
				////TRACE_INFO((Error, "read cate_idx error");
				return -1;
			}

			if(!stream.Read(sendflower.number))
			{
				////TRACE_INFO((Error, "read number error");
				return -1;
			}

			if(!stream.Read(sendflower.leavings))
			{
				////TRACE_INFO((Error, "read leavings error");
				return -1;
			}

			if(!stream.Read(sendflower.time))
			{
				////TRACE_INFO((Error, "read leavings error");
				return -1;
			}
			if (!stream.Read(sendflower.once_num))
			{
				////TRACE_INFO((Error, "read once_num error");
				return -1;
			}

			if (pRb)
			{
				pRb->RecvSendFlowerNotice(0,&sendflower);
			}
		}
		break;
	case ROOM_CMD_SB_SEND_NOTICE_TEMP:
		{

			int from_idx;
			if(!stream.Read(from_idx))
			{
				////TRACE_INFO((Error, "read from_idx error");
				return -1;
			}
			char content[MSG_LENGTH] = {0};
			unsigned int contentlen;
			if (!stream.Read(content,sizeof(content),contentlen))
			{
				////TRACE_INFO((Error, "read content error");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice(0,content,contentlen);
			/*
			ResultData_ChatMessage msg;
			msg.clubid = 0;
			if(!stream.Read(msg.idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}
			char content[MSG_LENGTH] = {0};
			unsigned int length;
			if(!stream.Read(content, sizeof(content), length))
			{
				////TRACE_INFO((Error, "read content error");
				return -1;
			}
			msg.content = content;
			OnPackage2 op = g_NetApp.GetPackageHandler_Room();
			if(op)
			{
				op(NULL,cmd,&msg);
			}	
			*/
		}
		break;
	//add by jinguanfu 2010/1/4 房间管理<Begin>
	case ROOM_CMD_ROOMAPPLYLIST_C2R2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);	//释放pRD

			int count;						//本次接收总数
			if (!stream.Read(count))
			{
				////TRACE_INFO((Error, "read count is NULL");
				return -1;
			}
			ResultDate_RoomApplyList applylist;
			applylist.count=count;

			RoomApply* pRA= new RoomApply[count];

			for (int i=0;i<count;i++)
			{
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read count is NULL");
					delete []pRA;
					return -1;
				}
				int roomid;
				if (!stream.Read(roomid))
				{
					////TRACE_INFO((Error, "read count is NULL");
					delete []pRA;
					return -1;
				}
				int state;
				if (!stream.Read(state))
				{
					////TRACE_INFO((Error, "read count is NULL");
					delete []pRA;
					return -1;
				}
				char time[32]={0};
				unsigned int timelen;
				if (!stream.Read(time,sizeof(time),timelen))
				{
					////TRACE_INFO((Error, "read time is NULL");
					delete []pRA;
					return -1;
				}
				pRA[i].roomid=roomid;
				pRA[i].idx=idx;
				pRA[i].state=state;
				memset(pRA[i].time,0,32);
				strcpy(pRA[i].time,time);
			}
			applylist.pRA = pRA;
			if (pRb)
				pRb->RecvRoomApplyList(0,ROOM_NORMAL,&applylist,(*pRD)->cseq);
			
			delete []pRA;
			
		}
		break;
	case ROOM_CMD_USER_APP_JOINROOM:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			if (pRb)
			{
				switch(ret)
				{
				case SUCCESS:
					pRb->RecvApplyRoomMember(0,ROOM_NORMAL,(*pRD)->cseq);
					break;
				case ALREADY:
					pRb->RecvApplyRoomMember(-1,ROOM_ERROR_ALREADYMEMBER,(*pRD)->cseq);
					break;
				case LISTFULL:
					pRb->RecvApplyRoomMember(-1,ROOM_ERROR_MEMLISTFULL,(*pRD)->cseq);
				case REFUSE:
					pRb->RecvApplyRoomMember(-1,ROOM_ERROR_APPLYREFUSE,(*pRD)->cseq);
					break;
				case IAGAIN:
					pRb->RecvApplyRoomMember(-1,ROOM_ERROR_APPLYAGAIN,(*pRD)->cseq);
					break;
				case APPLYFULL:
					pRb->RecvApplyRoomMember(-1,ROOM_ERROR_APPLYFULL,(*pRD)->cseq);
					break;
				}
			}

		}
		break;
	case ROOM_CMD_VERIFY_USER_APP:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);

			int ret=-1;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			int idx=0;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}

			if (pRb)
			{
				switch(ret)
				{
				case SUCCESS:
					pRb->RecvAuditAddApply(0,ROOM_NORMAL,idx,(*pRD)->cseq);
					break;
				case LISTFULL:
					pRb->RecvAuditAddApply(-1,ROOM_ERROR_MEMLISTFULL,idx,(*pRD)->cseq);
					break;
				case ALREADY:
					pRb->RecvAuditAddApply(-1,ROOM_ERROR_ALREADYMEMBER,idx,(*pRD)->cseq);
					break;
				}
			}

		}
		break;
	case ROOM_CMD_SB_VERIFY_USER_APP:
		{
			int opidx;
			if (!stream.Read(opidx))
			{
				////TRACE_INFO((Error, "read opidx is NULL");
				return -1;
			}

			int bopidx;
			if (!stream.Read(bopidx))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}

			if (pRb)
				pRb->RecvAddClubNoticeByRoom(opidx,bopidx);
		}
		break;
	case ROOM_CMD_REFUSEMEMBER:
		{
			LOGI("recv room cmd ROOM_CMD_REFUSEMEMBER");
			int roomid;
			if (!stream.Read(roomid))
				return -1;

			if (pRb)
				pRb->RecvRoomApplyRefuseMsg(0,roomid);
		}
		break;
	case ROOM_CMD_GETROOMMEMBERLIST:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);

			//int ret;
			//if (!stream.Read(ret))
			//{
			//	////TRACE_INFO((Error, "read ret is NULL");
			//	return -1;
			//}
			short count;
			if (!stream.Read(count))
			{
				////TRACE_INFO((Error, "read count is NULL");
				return -1;
			}
			ResultDate_RoomMemList memlist;
			memlist.count=count;

			RoomUser* pRU= new RoomUser[count];
			for (int i=0;i<count;i++)
			{
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read count is NULL");
					delete []pRU;
					return -1;
				}
				//char online;
				//if (!stream.Read(online))
				//{
				//	////TRACE_INFO((Error, "read online is NULL");
				//	return -1;
				//}
				pRU[i].idx=idx;
				//memlist.userlist[i].online=online;
			}
			memlist.userlist = pRU;
			if (pRb)
				pRb->RecvRoomMemberList(0,ROOM_NORMAL,&memlist,(*pRD)->cseq);

			delete []pRU;
		}
		break;
	case ROOM_CMD_REMOVE_USER:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			if (pRb)
			{
				if(ret==1)
					pRb->RecvUnchainMember(-1,ROOM_ERROR_NOTMEMBER,idx,(*pRD)->cseq);
				else if(ret==0)
					pRb->RecvUnchainMember(0,ROOM_NORMAL,idx,(*pRD)->cseq);
			}
		}
		break;
	case ROOM_CMD_SB_REMOVE_USER:
		{
			int opidx, kickidx;
			if (!stream.Read(opidx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			if (!stream.Read(kickidx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvKickClubNoticeByRoom( opidx, kickidx );
		}
		break;
	case ROOM_CMD_SB_EXITMEMBER:
		{
			int outidx;
			if (!stream.Read(outidx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvExitClubNoticeByRoom( outidx );
		}
		break;
	case ROOM_CMD_GIVE_VJ_A:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			if (pRb)
			{	
				if(ret==0)
					pRb->RecvRoomPesideAssistant(0,ROOM_NORMAL,idx,(*pRD)->cseq);
				else if(ret==-1)
					pRb->RecvRoomPesideAssistant(0,ROOM_ERROR_VJAFULL,idx,(*pRD)->cseq);
			}
		}
		break;
	case ROOM_CMD_SB_GIVE_VJ_A:
		{
			int opidx;
			if (!stream.Read(opidx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			int bopidx;
			if (!stream.Read(bopidx))
			{
				return -1;
			}
			char level;
			if (!stream.Read(level))
			{
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4PesideAssistant(0,opidx,bopidx);
		}
		break;
	case ROOM_CMD_GIVE_VJ:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			if (pRb)
			{
				if(ret==0)
					pRb->RecvRoomPeside(0,ROOM_NORMAL,idx,(*pRD)->cseq);
				else if(ret==-1)
					pRb->RecvRoomPeside(0,ROOM_ERROR_VJFULL,idx,(*pRD)->cseq);
			}
		}
		break;
	case ROOM_CMD_SB_GIVE_VJ:
		{
			int opidx;
			if (!stream.Read(opidx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			int bopidx;
			if (!stream.Read(bopidx))
			{
				return -1;
			}
			char level;
			if (!stream.Read(level))
			{
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4Peside(0,opidx,bopidx);
		}
		break;
	case ROOM_CMD_GIVE_OUER_S:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			if (pRb)
			{
				if(ret==0)
					pRb->RecvRoomSubHost(0,ROOM_NORMAL,idx,(*pRD)->cseq);
				else if(ret==-1)
					pRb->RecvRoomSubHost(0,ROOM_ERROR_OWER2FULL,idx,(*pRD)->cseq);
			}
		}
		break;
	case ROOM_CMD_SB_GIVE_OUER_S:
		{
			int opidx;
			if (!stream.Read(opidx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			int bopidx;
			if (!stream.Read(bopidx))
			{
				return -1;
			}
			char level;
			if (!stream.Read(level))
			{
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4SubHost(0,opidx,bopidx);
			break;
		}
	case ROOM_CMD_GETBLACKLIST:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			//int ret;
			//if (!stream.Read(ret))
			//{
			//	////TRACE_INFO((Error, "read ret is NULL");
			//	return -1;
			//}
			short count;
			if (!stream.Read(count))
			{
				////TRACE_INFO((Error, "read count is NULL");
				return -1;
			}
			ResultDate_RoomBlackList blacklist;
			blacklist.count=count;
			blacklist.list=(int*)malloc(sizeof(int)*count);
			for (int i=0;i<count;i++)
			{
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx is NULL,count=%d",i);
					return -1;
				}
				blacklist.list[i]=idx;
			}
			if (pRb)
				pRb->RecvBlackList(0,ROOM_NORMAL,&blacklist,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_UPDATE_BLACKLIST:
		{
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			char type;
			if (!stream.Read(type))
			{
				////TRACE_INFO((Error, "read type is NULL");
				return -1;
			}
			if (pRb)
			{
				switch(ret)
				{
				case SUCCESS:
					pRb->RecvAddDelBlack(0,ROOM_NORMAL,idx,type);
					break;
				case -1:
					pRb->RecvAddDelBlack(-1,ROOM_ERROR_ADDAGAIN,idx,type);
					break;
				case LISTFULL:
					pRb->RecvAddDelBlack(-1,ROOM_ERROR_BLACKFULL,idx,type);
					break;
				}
			}
		}
		break;
	case ROOM_CMD_SET_ROOM_PWD:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomPassword(0,ret,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_SB_SET_ROOM_PWD:
		{
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			char pwdflag;
			if (!stream.Read(pwdflag))
			{
				////TRACE_INFO((Error, "read pwdflag is error");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4Password(0,idx,pwdflag);
		}
		break;
	case ROOM_CMD_SET_ROOM_LOCK:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			char roomflag;
			if (!stream.Read(roomflag))
			{
				////TRACE_INFO((Error, "read roomflag is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomSwitch(0,ret,roomflag,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_SB_SET_ROOM_LOCK:
		{
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag is error");
				return -1;
			}

			if (pRb)
				pRb->RecvRoomNotice4RoomSwitch(ret,idx,flag);
		}
		break;
	case ROOM_CMD_SET_USER_ONLY:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomPrivate(0,ret,flag,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_SB_SET_USER_ONLY:
		{
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag is error");
				return -1;
			}

			if (pRb)
				pRb->RecvRoomNotice4Private(ret,idx,flag);
		}
		break;
	case ROOM_CMD_UPDATE_CONTENT:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}

			if (pRb)
				pRb->RecvRoomAffiche(0,ret,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_SB_CONTENT_UPDATE:
		{
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			//	TCHAR* content[512]={0};
			//	unsigned int length;
			//	if(!stream.Read(content, sizeof(content), length))
			//		{
			//			////TRACE_INFO((Error, "read content error");
			//			return -1;
			//		}
			if (pRb)
				pRb->RecvRoomAfficheEvent(ret);
		}
		break;
	case ROOM_CMD_SET_USER_INOUT:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvUserInOutSwitch(0,ret,flag,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_SB_SET_USER_INOUT:
		{

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag is error");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4UserSwitch(0,idx,flag);
		}
		break;
	case ROOM_CMD_SET_MIC_UPDOWN:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvGetMicSwitch(0,ret,flag,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_SET_AUTOONMIC:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvOnMicSwitch(0,ret,flag,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_SB_SET_MIC_UPDOWN:
		{
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag is error");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4GetMicSwitch(ret,idx,flag);
		}
		break;
	case ROOM_CMD_SB_SET_AUTOONMIC:
		{
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag is error");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4OnMicSwitch(ret,idx,flag);
		}
		break;
	case ROOM_CMD_SET_ROOM_NAME:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}

			if (pRb)
				pRb->RecvModifyRoomName(0,ret,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_SB_SET_ROOM_NAME:
		{
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			int roomid;
			if (!stream.Read(roomid))
			{
				////TRACE_INFO((Error, "read roomid is error");
				return -1;
			}
			//这里写了TCHAR 不知道是不是多字节
			char roomname[512] = {0};
			unsigned int rnlen;
			if (!stream.Read(roomname,sizeof(roomname),rnlen))
			{
				////TRACE_INFO((Error, "read roomname is error");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomName(ret,roomid,roomname,rnlen);
		}
		break;
	case ROOM_CMD_SET_CHAT_PUBLIC:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvAllowRoomAllChat(0,ret,flag,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_SB_SET_CHAT_PUBLIC:
		{
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4AllChat(ret,idx,flag);
		}
		break;
	case ROOM_CMD_SET_ROOM_WELCOME:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomSalutatory(0,ret,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_SB_SET_ROOM_WELCOME:
		{
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4Salutatory(ret,idx);
		}
		break;
	case ROOM_CMD_SET_ROOM_LOGO:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}

			if (pRb)
				pRb->RecvRoomLogo(0,ret,(*pRD)->cseq);
		}
		break;
	case ROOM_CMD_SB_SET_ROOM_LOGO:
		{
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			if (pRb)
			pRb->RecvRoomNotice4Logo(ret,idx);

		}
		break;
	case ROOM_CMD_GETPANELINFO:
		{
			ResultDate_RoomPanel roompanel;
			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			
			if (!stream.Read(roompanel.pwdflag))
			{
				////TRACE_INFO((Error, "read pwdflag error");
				return -1;
			}

			if (!stream.Read(roompanel.roomflag))
			{
				////TRACE_INFO((Error, "read roomflag error");
				return -1;
			}

			if (!stream.Read(roompanel.accessflag))
			{
				////TRACE_INFO((Error, "read pwdflag error");
				return -1;
			}

			if (!stream.Read(roompanel.inoutflag))
			{
				////TRACE_INFO((Error, "read inoutflag error");
				return -1;
			}

			if (!stream.Read(roompanel.getmicflag))
			{
				////TRACE_INFO((Error, "read inoutflag error");
				return -1;
			}
			//
			unsigned int rnlen=0;
			memset(roompanel.name,0,sizeof(roompanel.name));
			if (!stream.Read(roompanel.name,sizeof(roompanel.name),rnlen))
			{
				////TRACE_INFO((Error, "read name error");
				return -1;
			}
			unsigned int noticelen=0;
			memset(roompanel.notice,0,sizeof(roompanel.notice));
			if (!stream.Read(roompanel.notice,sizeof(roompanel.notice),noticelen))
			{
				////TRACE_INFO((Error, "read notice error");
				return -1;
			}

			if (!stream.Read(roompanel.chatflag))
			{
				////TRACE_INFO((Error, "read chatflag error");
				return -1;
			}

			unsigned int saluLen;
			memset(roompanel.Salutatory,0,sizeof(roompanel.Salutatory));
			if (!stream.Read(roompanel.Salutatory
				,sizeof(roompanel.Salutatory),saluLen))
			{
				////TRACE_INFO((Error, "read Salutatory error");
				return -1;
			}

			unsigned int pathlen;
			memset(roompanel.logopath,0,sizeof(roompanel.logopath));
			if (!stream.Read(roompanel.logopath
				,sizeof(roompanel.logopath),pathlen))
			{
				////TRACE_INFO((Error, "read logopath error");
				return -1;
			}

			unsigned int pwdlen;
			memset(roompanel.pwd,0,sizeof(roompanel.pwd));
			if (!stream.Read(roompanel.pwd,sizeof(roompanel.pwd),pwdlen))
			{
				////TRACE_INFO((Error, "read pwd error");
				return -1;
			}

			if (!stream.Read(roompanel.onmicflag))
			{
				////TRACE_INFO((Error, "read chatflag error");
				return -1;
			}
			if (!stream.Read(roompanel.privatemicflag))
			{
				////TRACE_INFO((Error, "read privatemicflag error");
				return -1;
			}

			////TRACE_INFO((Debug, "roompanel.privatemicflag=%d", roompanel.privatemicflag);

			if (pRb)
				pRb->RecvRoomPanelInfo( 0,ROOM_NORMAL,&roompanel);
		}
		break;
	//add by jinguanfu 2010/4/7 房间踢除重复登录用户
	case ROOM_CMD_KICKUSER:
		{
			if (pRb)
				pRb->RecvRoomKickUser();
		}
		break;
	case ROOM_CMD_EXITMEMBER:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			if (pRb)
			{
				if (ret==1)
					pRb->RecvExitRoomMember(-1,ROOM_ERROR_NOTMEMBER);
				else if(ret==0)
					pRb->RecvExitRoomMember(0,ROOM_NORMAL);
			}
		}
		break;
	case ROOM_CMD_SEND_GIFT:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);
			
			int item_id;	//礼物id
			if(!stream.Read(item_id))
			{
				////TRACE_INFO((Error, "read item_idx error");
				return -1;
			}
			int count;		//礼物数量
			if(!stream.Read(count))
			{
				////TRACE_INFO((Error, "read count error");
				return -1;
			}
			int r_idx;		//收礼者idx
			if(!stream.Read(r_idx))
			{
				////TRACE_INFO((Error, "read r_idx error");
				return -1;
			}
			int ret;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			switch(ret)
			{
			case -1:	//金额不足
				if(pRb)
					pRb->RecvSendFlower(-1,ROOM_ERROR_GIFT_BALANCE,r_idx,item_id,count,0,0);

				break;
			case 0:		//成功
				int gold;	//金币余额
				if(!stream.Read(gold))
				{
					////TRACE_INFO((Error, "read gold error");
					return -1;
				}
				int silver;//银币余额
				if(!stream.Read(silver))
				{
					////TRACE_INFO((Error, "read silver error");
					return -1;
				}
				g_DM.saveMoney(gold,silver);
				if(pRb)
					pRb->RecvSendFlower(0,ROOM_NORMAL,r_idx,item_id,count,gold,silver);

				break;
			case 1:		//交易金额为空
	
				if(pRb)
					pRb->RecvSendFlower(-1,ROOM_ERROR_GIFT_UNKOWN,r_idx,item_id,count,0,0);
				break;
			case 2:		//无此交易类型
	
				if(pRb)
					pRb->RecvSendFlower(-1,ROOM_ERROR_GIFT_NOTEXIST,r_idx,item_id,count,0,0);
				break;
			case 3:		//礼物接收者不在房间内
				if(pRb)
					pRb->RecvSendFlower(-1,ROOM_ERROR_GIFT_OFFLINE,r_idx,item_id,count,0,0);
				break;
			}
			break;
		}

	case ROOM_CMD_GIVE_MEMBER:	//add by jinguanfu 2010/5/13
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			if (pRb)
			{	
				if(ret==0)
					pRb->RecvRoomMember(0,ROOM_NORMAL,idx,(*pRD)->cseq);
				else if(ret==-1)
					pRb->RecvRoomMember(-1,ROOM_ERROR_MEMBER,idx,(*pRD)->cseq);
			}
		}
		break;
	case ROOM_CMD_SB_GIVE_MEMBER:	//add by jinguanfu 2010/5/13
		{
			int opidx;
			if (!stream.Read(opidx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			int bopidx;
			if (!stream.Read(bopidx))
			{
				return -1;
			}
			char level;
			if (!stream.Read(level))
			{
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4Member(0,opidx,bopidx);
		}
		break;
	//add by cx 10-5-20
	case ROOM_CMD_SB_CHANGEAVATAR:
		{
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4ChangeAvarat(0,idx);
		}
		break;
	case ROOM_CMD_SB_LEVEL:
		{
			int idx; //用户idx
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read level up idx is error");
				return -1;
			}

			char level; //用户level
			if(!stream.Read( level ))
			{
				////TRACE_INFO((Error, "read level up level is error");
				return -1;
			}

			if (pRb)
			{
				pRb->RecvNotice4LevelUp( idx, level );
			}
		}
		break;
	case ROOM_CMD_SB_VIP:
		{
			int idx = 0;
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read vip level idx is error");
				return -1;
			}

			short vip_level = 0;
			if(!stream.Read( vip_level ))
			{
				////TRACE_INFO((Error, "read vip level is error");
				return -1;
			}

			char is_vip = 0;
			if(!stream.Read( is_vip ))
			{
				////TRACE_INFO((Error, "read is vip is error");
				return -1;
			}

			if (pRb)
			{
				pRb->RecvVipChange( idx, vip_level, is_vip );
			}
		}
		break;
	case ROOM_CMD_UPDATEMONEY:
		{
			int r_gold;
			if(!stream.Read(r_gold))
			{
				////TRACE_INFO((Error, "read r_gold is error");
				return -1;
			}

			int r_silver;
			if(!stream.Read(r_silver))
			{
				////TRACE_INFO((Error, "read r_silver is error");
				return -1;

			}

            g_DM.saveMoney(r_gold, r_silver);
			if (pRb)
				pRb->UpdataMoney( r_gold, r_silver );
		}
		break;
	case ROOM_CMD_SB_DELETEROOM:
		{
			int room_idx = 0;
			if(!stream.Read(room_idx))
			{
				////TRACE_INFO((Error, "read room_idx is error");
				return -1;
			}
			if (pRb)
			{
				pRb->RecvRoomNotice4Del(room_idx);
			}
		}
		break;
	case ROOM_CMD_SB_LOCKROOM:
		{
			int room_idx = 0;
			if(!stream.Read(room_idx))
			{
				////TRACE_INFO((Error, "read room_idx is error");
				return -1;
			}
			if (pRb)
			{
				//pRb->RecvRoomNotice4RoomSwitch(ret,idx,flag);
				pRb->RecvRoomNotice4Lock(room_idx);
			}
		}
		break;
	case ROOM_CMD_SB_ROOMINFO_UPDATE:
		{
			if (pRb)
				pRb->RecvRoomNotice4ChangeInfo( 0 );
		}
		break;
	case ROOM_CMD_SB_GIVE_OWNER:
		{
			int player_idx = 0;
			if(!stream.Read(player_idx))
			{
				////TRACE_INFO((Error, "read player_idx is error");
				return -1;
			}
			if (pRb)
				pRb->RecvRoomNotice4ChangeMaster( player_idx );
		}
		break;
	case ROOM_CMD_SEND_FIREWORKS:
		{
			if( NULL == pRb )
			{
				return -1;
			}

			int cate_idx = 0;
			if(!stream.Read(cate_idx))
			{
				////TRACE_INFO((Error, "read cate_idx is error");
				return -1;
			}

			int number = 0;
			if(!stream.Read(number))
			{
				////TRACE_INFO((Error, "read number is error");
				return -1;
			}

			int ret = 0;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}

			int s_gold = 0;
			int s_silver = 0;
			switch(ret)
			{
			case 0:		//成功
				if(!stream.Read( s_gold ))
				{
					////TRACE_INFO((Error, "read gold error");
					return -1;
				}

				if(!stream.Read( s_silver ))
				{
					////TRACE_INFO((Error, "read silver error");
					return -1;
				}

				pRb->RecvFireworks( 0, ROOM_NORMAL, cate_idx, number, s_gold, s_silver );
				break;

			case -1:	//金额不足
				pRb->RecvFireworks(-1,ROOM_ERROR_GIFT_BALANCE, cate_idx, number, s_gold, s_silver );
				break;

			case 2:		//无此交易类型
				pRb->RecvFireworks(-1,ROOM_ERROR_GIFT_NOTEXIST,cate_idx, number, s_gold, s_silver );
				break;
			}
		}
		break;
	case ROOM_CMD_SB_SEND_FIREWORKS:
		{
			int player_idx = 0;
			if(!stream.Read(player_idx))
			{
				////TRACE_INFO((Error, "read player_idx is error");
				return -1;
			}

			int cate_idx = 0;
			if(!stream.Read(cate_idx))
			{
				////TRACE_INFO((Error, "read cate_idx is error");
				return -1;
			}

			int count = 0;
			if(!stream.Read(count))
			{
				////TRACE_INFO((Error, "read count is error");
				return -1;
			}

			if (pRb)
			{
				pRb->RecvFireworksNotice( player_idx, cate_idx, count );
			}
		}
		break;
	case ROOM_CMD_SB_LUCKY:
		{
			int player_idx = 0;
			if(!stream.Read(player_idx))
			{
				////TRACE_INFO((Error, "read player_idx is error");
				return -1;
			}

			int cate_idx = 0;
			if(!stream.Read(cate_idx))
			{
				////TRACE_INFO((Error, "read cate_idx is error");
				return -1;
			}

			int lotte = 0;
			if(!stream.Read(lotte))
			{
				////TRACE_INFO((Error, "read lotte is error");
				return -1;
			}

			if (pRb)
			{
				pRb->RecvLucky( player_idx, cate_idx, lotte );
			}
		}
		break;
	case ROOM_CMD_SB_GIFT_INVALID:
		{
			int idx = 0;
			if(!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}

			int GiftID = 0;
			if(!stream.Read(GiftID))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}

			if (pRb)
			{
				pRb->RecvGiftInvalid( idx, GiftID );
			}
		}
		break;
	case ROOM_CMD_SB_GIFT_VALID:
		{
			int idx = 0;
			if(!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}

			int GiftID = 0;
			if(!stream.Read(GiftID))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}

			if (pRb)
			{
				pRb->RecvGiftValid( idx, GiftID );
			}
		}
		break;
	case ROOM_CMD_VIEW_INCOME:
		{
			int income = 0;	//可领取收益值
			if(!stream.Read(income))
			{
				////TRACE_INFO((Error, "read income is error");
				return -1;
			}

			if (pRb)
			{
				pRb->RecvShowRoomIncom( income );
			}
		}
		break;
	case ROOM_CMD_VIEW_INCOME_LOG:
		{
			short rownum = 0; //记录条数
			if(!stream.Read(rownum))
			{
				////TRACE_INFO((Error, "read rownum is error");
				return -1;
			}

			if( rownum > 5 )
			{
				rownum = 5;
			}

			unsigned int len = 0;
			ResultData_IncomeDate data[5] = {0};
			for( short i=0; i<rownum; ++i )
			{
				if(!stream.Read( data[i].income ))
				{
					////TRACE_INFO((Error, "read income is error");
					return -1;
				}

				if(!stream.Read( data[i].date, sizeof( data[i].date ), len ))
				{
					////TRACE_INFO((Error, "read date is error");
					return -1;
				}
			}

			if (pRb)
			{
				pRb->RecvShowRoomGetIncom( data, rownum );
			}
		}
		break;
	case ROOM_CMD_GET_INCOME:
		{
			int ret = 0;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}

			if (pRb)
			{
				pRb->RecvGetRoomIncom( ret );
			}
		}
		break;
	case ROOM_CMD_SB_GIFTINFO_CHANGE:
		{
			//重新加载礼物
			g_DM.reinitGift();
			if (pRb)
				pRb->RecvNotice4ChangeFlower();
		}
		break;
	case ROOM_CMD_SB_RIGHTCONF_CHANGE:
		{
			if (pRb)
				pRb->RecvNotice4ChangePower();
		}
		break;
	case ROOM_CMD_SB_AVSERVER_CHANGE:
		{
			unsigned int len = 0;
			char ip_buf[ MAXLEN_ROOM_AVIPCOUNT * MAXLEN_IP ];

			ResultData_AvChange av_info = {0};

			if(!stream.Read( ip_buf, sizeof( ip_buf ), len ))
			{
				////TRACE_INFO((Error, "read ip telcom is error");
				return -1;
			}
			ip_buf[ len ] = 0;
			GetAvIpFromString( av_info.telcom, av_info.telcom_count, ip_buf, '|' );

			if(!stream.Read( av_info.port_telcom ))
			{
				////TRACE_INFO((Error, "read port telcom is error");
				return -1;
			}

			if(!stream.Read( ip_buf, sizeof( ip_buf ), len ))
			{
				////TRACE_INFO((Error, "read ip telcom is error");
				return -1;
			}
			ip_buf[ len ] = 0;
			GetAvIpFromString( av_info.netcom, av_info.netcom_count, ip_buf, '|' );

			if(!stream.Read( av_info.port_netcom ))
			{
				////TRACE_INFO((Error, "read port telcom is error");
				return -1;
			}

			if (pRb)
			{
				pRb->RecvNotice4AvserverChange( & av_info );
			}
		}
		break;
	case ROOM_CMD_SB_UPDATE_GM:
		{
			int gm_idx = 0;
			if(!stream.Read( gm_idx ))
			{
				////TRACE_INFO((Error, "read port gm idx is error");
				return -1;
			}

			char is_add = 0;
			if(!stream.Read( is_add ))
			{
				////TRACE_INFO((Error, "read port is add is error");
				return -1;
			}

			if( pRb )
			{
				pRb->RecvChangeGM( gm_idx,is_add );
			}
		}
		break;
	case ROOM_CMD_SB_NETSTATUS:
		{
			int player_idx = 0;
			if(!stream.Read( player_idx ))
			{
				////TRACE_INFO((Error, "read buf is error");
				return -1;
			}

			char buf[ MAXLEN_PCINFO ]={0};
			unsigned int len;
			if(!stream.Read(buf,sizeof(buf),len))
			{
				////TRACE_INFO((Error, "read buf is error");
				return -1;
			}

			if( pRb )
			{
				pRb->RecvMicPcInfo( player_idx, buf, ( int )len );
			}
		}
		break;
	case ROOM_CMD_INVITE_MEMBER:
		{
			int ret=-1;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}

			if (pRb)
			{
				switch(ret)
				{
				case SUCCESS:
					pRb->RecvSendJoinRoomInvite(seq, 0, ROOM_NORMAL);
					break;
				case LISTFULL:
					pRb->RecvSendJoinRoomInvite(seq, -1,ROOM_ERROR_MEMLISTFULL);
					break;
				case ALREADY:
					pRb->RecvSendJoinRoomInvite(seq, -1,ROOM_ERROR_ALREADYMEMBER);
					break;
				}
			}
		}
		break;
	case ROOM_CMD_INVITE_NOTIFY:
		{
			int player_idx = 0;
			if(!stream.Read( player_idx ))
			{
				////TRACE_INFO((Error, "read player_idx is error");
				return -1;
			}
			int my_idx = 0;
			if(!stream.Read( my_idx ))
			{
				////TRACE_INFO((Error, "read my_idx is error");
				return -1;
			}
			int room_idx = 0;
			if(!stream.Read( room_idx ))
			{
				////TRACE_INFO((Error, "read room_idx is error");
				return -1;
			}

			if( pRb )
			{
				pRb->RecvInviteJoinRoom( player_idx );
			}
		}
		break;
	case ROOM_CMD_GET_GIFTSEND:
		{
			short rownum;
			if(!stream.Read( rownum ))
			{
				////TRACE_INFO((Error, "read rownum is error");
				return -1;
			}

			ResultData_GiftCount * pInfo = NULL;
			if( 0 < rownum )
			{
				pInfo = new ResultData_GiftCount[ rownum ];
			}

			for( int i=0; i<rownum; ++i )
			{
				if(!stream.Read( pInfo[i].idx ))
				{
					////TRACE_INFO((Error, "read idx is error");
					if( NULL != pInfo ) delete[] pInfo;
					return -1;
				}
				if(!stream.Read( pInfo[i].giftid ))
				{
					////TRACE_INFO((Error, "read giftid is error");
					if( NULL != pInfo ) delete[] pInfo;
					return -1;
				}
				if(!stream.Read( pInfo[i].giftcount ))
				{
					////TRACE_INFO((Error, "read giftcount is error");
					if( NULL != pInfo ) delete[] pInfo;
					return -1;
				}
			}

			if( pRb )
			{
				pRb->RecvGiftSend( rownum, pInfo );
			}
			if( NULL != pInfo ) delete[] pInfo;
		}
		break;
	case ROOM_CMD_GET_GIFTRECV:
		{
			short rownum;
			if(!stream.Read( rownum ))
			{
				////TRACE_INFO((Error, "read rownum is error");
				return -1;
			}

			ResultData_GiftCount * pInfo = NULL;
			if( 0 < rownum )
			{
				pInfo = new ResultData_GiftCount[ rownum ];
			}

			for( int i=0; i<rownum; ++i )
			{
				if(!stream.Read( pInfo[i].idx ))
				{
					////TRACE_INFO((Error, "read idx is error");
					if( NULL != pInfo ) delete[] pInfo;
					return -1;
				}
				if(!stream.Read( pInfo[i].giftid ))
				{
					////TRACE_INFO((Error, "read giftid is error");
					if( NULL != pInfo ) delete[] pInfo;
					return -1;
				}
				if(!stream.Read( pInfo[i].giftcount ))
				{
					////TRACE_INFO((Error, "read giftcount is error");
					if( NULL != pInfo ) delete[] pInfo;
					return -1;
				}
			}

			if( pRb )
			{
				pRb->RecvGiftRecv( rownum, pInfo );
			}
			if( NULL != pInfo ) delete[] pInfo;
		}
		break;
	case ROOM_CMD_DISABLE_IPADDR:			//禁用对方的IP
		{
			int ret = 0;  //0:成功,-1：失败
			if(!stream.Read( ret ))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			int idx = 0;  //被禁者IDX
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			if( pRb )
			{
				pRb->RecvKickUserByIp( ret, idx );
			}
		}
		break;
	case ROOM_CMD_DISABLE_MACADDR:			//禁用对方的MAC
		{
			int ret = 0;  //0:成功,-1：失败
			if(!stream.Read( ret ))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			int idx = 0;  //被禁者IDX
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			if( pRb )
			{
				pRb->RecvKickUserByMac( ret, idx );
			}
		}
		break;
	case ROOM_CMD_ADD_TM:
		{
			int ret = 0;  //0:成功,-1：失败
			if(!stream.Read( ret ))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			int idx = 0;  //被IDX
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			if( pRb )
			{
				pRb->RecvSetMicTime( ret, idx );
			}
		}
		break;
	case ROOM_CMD_SB_ADDTM:
		{
			int idx = 0;  //被IDX
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			int time = 0;
			if(!stream.Read( time ))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			if( pRb )
			{
				pRb->RecvNotice4SetMicTime( idx, time );
			}
		}
		break;
	case ROOM_CMD_GET_WAITMICLIST:
		{
			short micNum = 0;
			if( !stream.Read( micNum ))
			{
				////TRACE_INFO((Error, "read micNum is error");
				return -1;
			}

			int idxs[ MAXLEN_WAITMICLIST ] = {0}; //new int[ micNum ];
			if( MAXLEN_WAITMICLIST < micNum )
				micNum = MAXLEN_WAITMICLIST;
			for( int i=0; i < micNum; ++i )
			{
				if( !stream.Read( idxs[i] ))
				{
					////TRACE_INFO((Error, "read wait mic list is error");
					return -1;
				}
			}
			if( pRb )
			{
				pRb->RecvWaitMicList( micNum, idxs );
			}
		}
		break;
	case ROOM_CMD_ON_PRIVATE_MIC_C2S2C:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}

			if (ret == RET_PRIVATE_VEDIO_SUCCESS)
			{
				int posindex; //视频位置
				if (!stream.Read(posindex))
				{
					////TRACE_INFO((Error, "read posindex error");
					return -1;
				}
				int roomid; //私麦房间号
				if (!stream.Read(roomid))
				{
					////TRACE_INFO((Error, "read roomid error");
					return -1;
				}
				short mictype; //私麦类型
				if (!stream.Read(mictype))
				{
					////TRACE_INFO((Error, "read mictype error");
					return -1;
				}
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}

				////TRACE_INFO((Debug, "posindex=%d, roomid=%d, mictype=%d, idx=%d", posindex, roomid, mictype, idx);
				rb->RecvOpenPrivateMicSuccess(idx, mictype, posindex, roomid);
			}
			else
			{
				int errorcode;
				if (!stream.Read(errorcode))
				{
					////TRACE_INFO((Error, "read errorcode error");
					return -1;
				}
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				int bidx;
				if (!stream.Read(bidx))
				{
					////TRACE_INFO((Error, "read bidx error");
					return -1;
				}

				////TRACE_INFO((Debug, "errorcode=%d, idx=%d, bidx=%d", errorcode, idx, bidx);
				rb->RecvOpenPrivateMicFailed(errorcode, idx, bidx);
			}
		}
		break;
	case ROOM_CMD_SB_ON_PRIVATE_MIC:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}
			short mictype;
			if (!stream.Read(mictype))
			{
				////TRACE_INFO((Error, "read mictype error");
				return -1;
			}

			////TRACE_INFO((Debug, "idx=%d, mictype=%d", idx, mictype);
			rb->RecvOpenPrivateMicNotice(idx, mictype);
		}
		break;
	case ROOM_CMD_OFF_PRIVATE_MIC_C2S2C:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}

			if (ret == RET_PRIVATE_VEDIO_ERROR)
			{
				int errorcode;
				if (!stream.Read(errorcode))
				{
					////TRACE_INFO((Error, "read errorcode error");
					return -1;
				}
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				int bidx;
				if (!stream.Read(bidx))
				{
					////TRACE_INFO((Error, "read bidx error");
					return -1;
				}

				////TRACE_INFO((Debug, "errorcode=%d, idx=%d, bidx=%d", errorcode, idx, bidx);
				rb->RecvClosePrivateMicFailed(errorcode, idx, bidx);
			}
		}
		break;
	case ROOM_CMD_SB_OFF_PRIVATE_MIC:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			////TRACE_INFO((Debug, "idx=%d", idx);
			rb->RecvClosePriveteMicNotice(idx);
		}
		break;
	case ROOM_CMD_GIVEOFF_PRIVATE_MIC_C2S2C:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}

			if (ret == RET_PRIVATE_VEDIO_ERROR)
			{
				int errorcode;
				if (!stream.Read(errorcode))
				{
					////TRACE_INFO((Error, "read errorcode error");
					return -1;
				}
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				int bidx;
				if (!stream.Read(bidx))
				{
					////TRACE_INFO((Error, "read bidx error");
					return -1;
				}

				////TRACE_INFO((Debug, "errorcode=%d, idx=%d, bidx=%d", errorcode, idx, bidx);
				rb->RecvClosedPrivateMicFailed(errorcode, idx, bidx);
			}
		}
		break;
	case ROOM_CMD_SB_GIVEOFF_PRIVATE_MIC:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}
			int bidx;
			if (!stream.Read(bidx))
			{
				////TRACE_INFO((Error, "read bidx error");
				return -1;
			}

			////TRACE_INFO((Debug, "idx=%d, bidx=%d", idx, bidx);

			rb->RecvClosedPriveteMicNotice(idx, bidx);
		}
		break;
	case ROOM_CMD_REQUEST_PRIVATE_MIC_C2S2C:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}

			// 不需要私麦持有者应答
			if (ret == RET_PRIVATE_VEDIO_SUCCESS)
			{
				// 给请求者
				int posindex; //视频位置
				if (!stream.Read(posindex))
				{
					////TRACE_INFO((Error, "read posindex error");
					return -1;
				}
				int roomid; //私麦房间号
				if (!stream.Read(roomid))
				{
					////TRACE_INFO((Error, "read roomid error");
					return -1;
				}
				short mictype; //私麦类型
				if (!stream.Read(mictype))
				{
					////TRACE_INFO((Error, "read mictype error");
					return -1;
				}
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				
				////TRACE_INFO((Debug, "idx=%d, posindex=%d, roomid=%d, mictype=%d", idx, posindex, roomid, mictype);
				rb->RecvWatchPrivateMicSuccess(idx, mictype, posindex, roomid);
			}
			else
			{
				//给请求者
				int errorcode;
				if (!stream.Read(errorcode))
				{
					////TRACE_INFO((Error, "read errorcode error");
					return -1;
				}
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				int bidx;
				if (!stream.Read(bidx))
				{
					////TRACE_INFO((Error, "read bidx error");
					return -1;
				}

				////TRACE_INFO((Debug, "errorcode=%d, idx=%d, bidx=%d", errorcode, idx, bidx);
				rb->RecvWatchPrivateMicFailed(errorcode, idx, bidx);
			}
		}
		break;
	case ROOM_CMD_SEND_REQUEST_PRIVATE_MIC_S2C:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			////TRACE_INFO((Debug, "idx=%d", idx);
			rb->RecvRequestWatchedPrivateMic(idx);
		}
		break;
	case ROOM_CMD_REQUEST_ACK_PRIVATE_MIC_C2S2C:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}

			// 需要私麦持有者应答
			if (ret == RET_PRIVATE_VEDIO_ERROR)
			{
				// 给应答者
				int errorcode;
				if (!stream.Read(errorcode))
				{
					////TRACE_INFO((Error, "read errorcode error");
					return -1;
				}
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				int bidx;
				if (!stream.Read(bidx))
				{
					////TRACE_INFO((Error, "read bidx error");
					return -1;
				}

				////TRACE_INFO((Debug, "errorcode=%d, idx=%d, bidx=%d", errorcode, idx, bidx);
				rb->RecvWatchedPrivateMicFailed(errorcode, idx, bidx);
			}
			else if(ret == RET_PRIVATE_VEDIO_SUCCESS)
			{
				// 给请求者
				int posindex; //视频位置
				if (!stream.Read(posindex))
				{
					////TRACE_INFO((Error, "read posindex error");
					return -1;
				}
				int roomid; //私麦房间号
				if (!stream.Read(roomid))
				{
					////TRACE_INFO((Error, "read roomid error");
					return -1;
				}
				short mictype; //私麦类型
				if (!stream.Read(mictype))
				{
					////TRACE_INFO((Error, "read mictype error");
					return -1;
				}
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				
				////TRACE_INFO((Debug, "idx=%d, posindex=%d, roomid=%d, mictype=%d", idx, posindex, roomid, mictype);
				rb->RecvWatchPrivateMicSuccess(idx, mictype, posindex, roomid);
			}
			else
			{
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				////TRACE_INFO((Debug, "reject idx=%d", idx);
				rb->RecvWatchedPrivateMicReject(idx);
			}
		}
		break;
	case ROOM_CMD_INVITE_PRIVATE_MIC_C2S:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}

			if (ret == RET_PRIVATE_VEDIO_SUCCESS)
			{
				rb->RecvInviteWatchPrivateMicSuccess();
			}
			else
			{
				int errorcode;
				if (!stream.Read(errorcode))
				{
					////TRACE_INFO((Error, "read errorcode error");
					return -1;
				}
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				int bidx;
				if (!stream.Read(bidx))
				{
					////TRACE_INFO((Error, "read bidx error");
					return -1;
				}

				////TRACE_INFO((Debug, "errorcode=%d, idx=%d, bidx=%d", errorcode, idx, bidx);
				rb->RecvInviteWatchPrivateMicFailed(errorcode, idx, bidx);
			}
		}
		break;
	case ROOM_CMD_SEND_INVITE_PRIVATE_MIC_S2C:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			////TRACE_INFO((Debug, "idx=%d", idx);
			rb->RecvRequestInvitedWatchPrivateMic(idx);

		}
		break;
	case ROOM_CMD_INVITE_ACK_PRIVATE_MIC_C2S:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}

			if (ret == RET_PRIVATE_VEDIO_SUCCESS)
			{
				int posindex; //视频位置
				if (!stream.Read(posindex))
				{
					////TRACE_INFO((Error, "read posindex error");
					return -1;
				}
				int roomid; //私麦房间号
				if (!stream.Read(roomid))
				{
					////TRACE_INFO((Error, "read roomid error");
					return -1;
				}
				short mictype; //私麦类型
				if (!stream.Read(mictype))
				{
					////TRACE_INFO((Error, "read mictype error");
					return -1;
				}
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				

				////TRACE_INFO((Debug, "idx=%d, posindex=%d, roomid=%d, mictype=%d", idx, posindex, roomid, mictype);
				rb->RecvInvitedWatchPrivateMicSuccess(idx, mictype, posindex, roomid);
			}
			else if (ret == RET_PRIVATE_VEDIO_ERROR)
			{
				int errorcode;
				if (!stream.Read(errorcode))
				{
					////TRACE_INFO((Error, "read errorcode error");
					return -1;
				}
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
				int bidx;
				if (!stream.Read(bidx))
				{
					////TRACE_INFO((Error, "read bidx error");
					return -1;
				}

				////TRACE_INFO((Debug, "errorcode=%d, idx=%d, bidx=%d", errorcode, idx, bidx);
				rb->RecvInvitedWatchPrivateMicFailed(errorcode, idx, bidx);
			}
			else
			{
				int idx;
				if (!stream.Read(idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}

				////TRACE_INFO((Debug, "reject idx=%d", idx);
				rb->RecvInvitedWatchPrivateMicReject(idx);
			}
		}
		break;
	case ROOM_CMD_UPDATE_PRIVATEMIC:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag error");
				return -1;
			}

			////TRACE_INFO((Debug, "ret=%d, flag=%d", ret, flag);
			rb->RecvOpenRoomPrivateMicFlag(ret, flag);
		}
		break;
	case ROOM_CMD_SB_UPDATE_PRIVATEMIC:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}
			char flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag error");
				return -1;
			}

			////TRACE_INFO((Debug, "idx=%d, flag=%d", idx, flag);
			rb->RecvOpenRoomPrivateMicFlagNotice(idx, flag);
		}
		break;
	case ROOM_CMD_SEND_INFO_PRIVATE_MIC_S2C:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int posindex; //视频位置
			if (!stream.Read(posindex))
			{
				////TRACE_INFO((Error, "read posindex error");
				return -1;
			}
			int roomid; //私麦房间号
			if (!stream.Read(roomid))
			{
				////TRACE_INFO((Error, "read roomid error");
				return -1;
			}
			short mictype; //私麦类型
			if (!stream.Read(mictype))
			{
				////TRACE_INFO((Error, "read mictype error");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			////TRACE_INFO((Debug, "idx=%d, posindex=%d, roomid=%d, mictype=%d", idx, posindex, roomid, mictype);
			rb->RecvPrivateMicWithLoginRoom(idx, mictype, posindex, roomid);
		}
		break;
	case ROOM_CMD_CROWN_KICKOUT_SOMEONE:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			int kickuse;
			if (!stream.Read(kickuse))
			{
				////TRACE_INFO((Error, "read kickuse error");
				return -1;
			}
			int totalkick;
			if (!stream.Read(totalkick))
			{
				////TRACE_INFO((Error, "read totalkick error");
				return -1;
			}
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}


			////TRACE_INFO((Debug, "ret=%d, kickuse=%d, totalkick=%d, idx=%d", ret, kickuse, totalkick, idx);
			rb->RecvCrownKickoutSomeOne(ret, kickuse, totalkick, idx);
		}
		break;
	case ROOM_CMD_DISABLEIP_BYROOM:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			int ret;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}

			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			////TRACE_INFO((Debug, "ret=%d, idx=%d", ret, idx);
			rb->RecvForbidUserIp(ret, idx);
		}
		break;
	case ROOM_CMD_UPLOAD_RECORDDATE:
		{
			ResponseBase* rb = g_NetApp.GetResponseHandler();
			rb->RecvUpdateSong();
		}
		break;
// 	case ROOM_CMD_GET_ONMICUSER_PANEL_C2S2C:
// 		{
// 			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
// 			ReserveData_UserSingInfo **pRD = (ReserveData_UserSingInfo **)(pSeqMap->Get(seq));
// 			if(!pRD)
// 			{
// 				return -1;
// 			}
// 			RDGuard guard(*pRD,seq);
// 
// 			int ret = 0;
// 			STREAM_READ_ERROR(ret,"ROOM_CMD_GET_ONMICUSER_PANEL read ret error");
// 			if (ret == -1)
// 			{
// 				////TRACE_INFO((Error,"ROOM_CMD_GET_ONMICUSER_PANEL read failed");
// 				return ret;
// 			}
// 			
// 			ResultData_UserSingInfo rd_us;
// 			memset(&rd_us,0,sizeof(rd_us));
// 			
// 			STREAM_READ_STR_ERROR(rd_us.headurl,"ROOM_CMD_GET_ONMICUSER_PANEL read head url error");
// 			STREAM_READ_ERROR(rd_us.recvcount,"ROOM_CMD_GET_ONMICUSER_PANEL read gift sum error");
// 			STREAM_READ_ERROR(rd_us.songcount,"ROOM_CMD_GET_ONMICUSER_PANEL read sing times error");
// 			STREAM_READ_STR_ERROR(rd_us.topsongname,"ROOM_CMD_GET_ONMICUSER_PANEL read best song error");
// 			STREAM_READ_ERROR(rd_us.topsongscore,"ROOM_CMD_GET_ONMICUSER_PANEL read best score error");
// 
// 			ResponseBase* rb = g_NetApp.GetResponseHandler();
// 			assert(rb != NULL);
// 			int idx = (*pRD)->idx;
// 			rb->RecvUserSingInfo(ret,idx,&rd_us);
// 			
// 			break;
// 		}

	case ROOM_CMD_SB_UPDATE_RECVCOUNT:
		{
			int recvcount; //用户接收礼物数

			if (!stream.Read(recvcount))
			{
				////TRACE_INFO((Error, "read recvcount error");
				return -1;
			}

			////TRACE_INFO((Debug, "recvcount=%d", recvcount);
			ResponseBase* rb = g_NetApp.GetResponseHandler();

			if (rb == NULL)
			{
				////TRACE_INFO((Error, "rb == NULL");
				return -1;
			}

			rb->RecvGiftCount(recvcount);

		}
		break;
	case ROOM_CMD_SB_UPDATE_TOPSONG:
		{
			if (pRb == NULL)
				return 0;

			int  songcount; //已演唱歌曲数量
			int  topmusicscore;   //演唱最高分
			char topmusicname[MSG_LENGTH] = {0};//演唱分最高歌曲名
			unsigned int namelenth;

			if (!stream.Read(songcount))
				return -1;

			if (!stream.Read(topmusicscore))
				return -1;

			if (!stream.Read(topmusicname,sizeof(topmusicname),namelenth))
			{
				////TRACE_INFO((Error, "read topmusicname error");
				return -1;
			}

			pRb->RecvTopSongInfo(songcount, topmusicscore, topmusicname, namelenth);
		}
		break;
    //@int listcount 歌曲数量
    //@while
    //  @string songname 歌曲名
    //  @string singername 歌手名
    case ROOM_CMD_SB_KTV_CUR_SONGLIST:
    {
        int listcount = 0;
        
        PROTOCOL_READ(listcount);
        std::vector<ResultData_SongList> list;
        if(listcount > 0)
        {
			list.resize(listcount);

			for (int i=0; i<listcount; i++) {
				unsigned int len;
				PROTOCOL_READ(list[i].songid);
				PROTOCOL_READSTR(list[i].songname,len);
				PROTOCOL_READSTR(list[i].singername,len);
			}
        }
        //安全释放
        if(pRb)
            pRb->RecvKtvSongList(&list[0], listcount);
        break;
    }
    //来自KTV服务器的通知，通知当前包厢的状态
    case ROOM_CMD_SB_KTV_ROOM_STATE:
    {
        char state;
        PROTOCOL_READ(state);
        
        if(pRb)
            pRb->RecvKtvRoomState(!state);
        break;
    }
    //来自KTV服务器的通知，通知当前包厢用户演唱的歌曲已经上榜
    case ROOM_CMD_SB_KTV_SONGRANKING:
    {
        if(pRb)
            pRb->RecvKtvSongRanking();
        break;
    }
	case ROOM_CMD_SB_KTV_SONGNORMALOVER:
	{
		int musicId = 0;
		PROTOCOL_READ(musicId);
		
		if(pRb)
			pRb->RecvKtvSongNormalEnd(musicId);
		break;
	}
	default:
		break;
	}

	return 0;
}