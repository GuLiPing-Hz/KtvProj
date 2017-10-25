#include <assert.h>
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

//typedef OnPackage OnPackage2;

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

int RoomDataDecoder::onPackage(ClientSocketBase *pClient,const char* buf,unsigned int buflen)
{
	RoomTunnel *pRoomTunnel = (RoomTunnel*)pClient;

	char outbuf[65535];
	int outbuflen = sizeof(outbuf);

	if(pRoomTunnel->IsTokenLogin() == true)
	{
		if(buf[2] == TEAFLAG)
		{
			if(!StreamDecrypt(buf,(int)buflen,outbuf,outbuflen,pRoomTunnel->m_Sessionkey))
			{
				//////TRACE_INFO((Error, "cann't decrypt");
				return -1;
			}
		}
		else if(buf[2] == COMPRESSFLAG)
		{
			if(!StreamUnCompress(buf,(int)buflen,outbuf,outbuflen))
			{
				LOGE("cann't decrypt\n");
				return -1;
			}
		}
		else
		{
			LOGE("cann't decrypt\n");
			return -1;
		}
	}
	else
	{
		char tempsessionkey[16] = {0};
		strncpy(tempsessionkey,ROOMFIRSTKEY,sizeof(tempsessionkey));

		if(!StreamDecrypt(buf,(int)buflen,outbuf,outbuflen,tempsessionkey,2))
		{
			LOGE("cann't decrypt");
			return -1;
		}
	}

	BinaryReadStream stream(outbuf,outbuflen);

	if(!stream.skip(1,false))//跳过一字节，标识压缩，加密等信息
	{
		return -1;
	}

	short cmd;
	if(!stream.Read(cmd))
	{
		//////TRACE_INFO((Error, "read cmd error");
		return -1;
	}
	int seq;
	if(!stream.Read(seq))
	{
		//////TRACE_INFO((Error, "read seq error");
		return -1;
	}

	LOGI("read room cmd %d\n",cmd);
	switch( pRoomTunnel->m_connect_type )
	{
	case VATYPE_CONNECTTOROOM_PRACTICE:
		return OnPackage_Practice( cmd, seq, & stream, pRoomTunnel );

	case VATYPE_CONNECTTOROOM_BASE:
		return OnPackage_Base( cmd, seq, & stream, pRoomTunnel );

	default:
		return -1;
	}
}

int RoomDataDecoder::OnPackage_Practice( short cmd, int seq, void * pStream, void * pTunnel )
{
	BinaryReadStream & stream = * (BinaryReadStream *)pStream;
	//LobbyTunnel * pRoomTunnel = (LobbyTunnel *)pTunnel;

	switch( cmd )
	{
	case ROOM_CMD_GET_ALLINFO:
		{
			ResultData_RoomAllInfo allinfo;//ROOM_ALL_INFO allinfo;

			short num = 0;
			if(!stream.Read(num))
			{
				//////TRACE_INFO((Error, "read usernum error");
				return -1;
			}
			allinfo.usercount = num;

			if(NULL == (allinfo.userlist = (int *)malloc(num * sizeof(int))))
			{
				//////TRACE_INFO((Error, "memory err");
				return -1;
			}

			//userlist
			for(int i = 0; i < num; i++)
			{
				int idx;
				if(!stream.Read(idx))
				{
					//////TRACE_INFO((Error, "read idx error");
					return -1;
				}
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
				//////TRACE_INFO((Error, "memory err");
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
					//////TRACE_INFO((Error, "read idx error");
					return -1;
				}

				if(!stream.Read(waitmiclist->musicid))
				{
					//////TRACE_INFO((Error, "read musicid error");
					return -1;
				}

				if(!stream.Read(waitmiclist->bk))
				{
					//////TRACE_INFO((Error, "read bk error");
					return -1;
				}

				waitmiclist->state = 0;

				if(!stream.Read(waitmiclist->pkidx))
				{
					//////TRACE_INFO((Error, "read pkidx error");
					return -1;
				}

				if(!stream.Read(waitmiclist->pkmusicid))
				{
					//////TRACE_INFO((Error, "read pkmusicid error");
					return -1;
				}

				if(!stream.Read(waitmiclist->pkbk))
				{
					//////TRACE_INFO((Error, "read pkbk error");
					return -1;
				}

				waitmiclist->pkstate = 0;

				if(!stream.Read(waitmiclist->level))
				{
					//////TRACE_INFO((Error, "read pkbk error");
					return -1;
				}

				if( !stream.Read( waitmiclist->name, sizeof( waitmiclist->name ), len ) )
				{
					//////TRACE_INFO((Error, "read name error");
					return -1;
				}
				waitmiclist->name[ len ] = 0;

				if(!stream.Read(waitmiclist->speed))
				{
					//////TRACE_INFO((Error, "read pkbk error");
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
			allinfo.vjonmic = 0;

			//onmic
			ResultData_MicInfo *ponmic = allinfo.pMLI + allinfo.miccount;
			allinfo.ponmic = ponmic;
			if(!stream.Read(ponmic->idx))
			{
				//////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			if(!stream.Read(ponmic->musicid))
			{
				//////TRACE_INFO((Error, "read musicid error");
				return -1;
			}

			if(!stream.Read(ponmic->bk))
			{
				//////TRACE_INFO((Error, "read bk error");
				return -1;
			}

			//add by cx 10-6-5
			ponmic->state = 0;
			if(!stream.Read(ponmic->state))
			{
				//////TRACE_INFO((Error, "read state error");
				return -1;
			}

			if(!stream.Read(ponmic->pkidx))
			{
				//////TRACE_INFO((Error, "read pkidx error");
				return -1;
			}

			if(!stream.Read(ponmic->pkmusicid))
			{
				//////TRACE_INFO((Error, "read pkmusicid error");
				return -1;
			}

			if(!stream.Read(ponmic->pkbk))
			{
				//////TRACE_INFO((Error, "read pkbk error");
				return -1;
			}

			//add by cx 10-6-5
			ponmic->pkstate = 0;
			if(!stream.Read(ponmic->pkstate))
			{
				//////TRACE_INFO((Error, "read pkstate error");
				return -1;
			}

			if(!stream.Read(ponmic->level))
			{
				//////TRACE_INFO((Error, "read pkbk error");
				return -1;
			}

			if( !stream.Read( ponmic->name, sizeof( ponmic->name ), len ) )
			{
				//////TRACE_INFO((Error, "read name error");
				return -1;
			}
			ponmic->name[ len ] = 0;

			if(!stream.Read( ponmic->speed ))
			{
				//////TRACE_INFO((Error, "read pkbk error");
				return -1;
			}

			//if(!stream.Read( ponmic->change ))
			//{
			//	////TRACE_INFO((Error, "read pkbk error");
			//	return -1;
			//}

			//////////////////////////////////////////////////////////////////////////
			unsigned int length;
			char ip_buf[ MAXLEN_ROOM_AVIPCOUNT * MAXLEN_IP ];

			if(!stream.Read( ip_buf, sizeof( ip_buf ), length ))
			{
				//////TRACE_INFO((Error, "read ip telcom error");
				return -1;
			}
			ip_buf[length] = 0;
			GetAvIpFromString( allinfo.ip_telcom, allinfo.ip_telcom_count, ip_buf, '|' );

			if( !stream.Read( allinfo.port_telcom ))
			{
				//////TRACE_INFO((Error, "read port telcom error");
				return -1;
			}

			if(!stream.Read( ip_buf, sizeof( ip_buf ), length ))
			{
				//////TRACE_INFO((Error, "read ip netcom error");
				return -1;
			}
			ip_buf[length] = 0;
			GetAvIpFromString( allinfo.ip_netcom, allinfo.ip_netcom_count, ip_buf, '|' );

			if( !stream.Read( allinfo.port_netcom ))
			{
				//////TRACE_INFO((Error, "read port netcom error");
				return -1;
			}

			//////////////////////////////////////////////////////////////////////////

			//管理员
			allinfo.managercount = 0;
			allinfo.pMI = NULL;

			//房间公告
			char content[1] = {0};
			allinfo.roomcontent = content;

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvRoomAllInfo(0,ROOM_NORMAL,&allinfo,0);
			}
		}
		break;

	case ROOM_CMD_GETPANELINFO:
		{
			ResultDate_RoomPanel roompanel;

			int ret;
			if (!stream.Read(ret))
			{
				//////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}

			if (!stream.Read(roompanel.pwdflag))
			{
				//////TRACE_INFO((Error, "read pwdflag error");
				return -1;
			}

			roompanel.roomflag = 0;
			roompanel.accessflag = 0;
			roompanel.inoutflag = 1;
			roompanel.onmicflag = 1;
			roompanel.getmicflag = 1;

			char isFree = 0;
			if (!stream.Read(isFree))
			{
				//////TRACE_INFO((Error, "read isFree error");
				return -1;
			}

			char isScore = 0;
			if (!stream.Read(isScore))
			{
				//////TRACE_INFO((Error, "read isScore error");
				return -1;
			}

			char isVote = 0;
			if (!stream.Read(isVote))
			{
				//////TRACE_INFO((Error, "read isFree error");
				return -1;
			}

			unsigned int rnlen=0;

			memset(roompanel.name,0,sizeof(roompanel.name));
			if (!stream.Read(roompanel.name,sizeof(roompanel.name),rnlen))
			{
				//////TRACE_INFO((Error, "read name error");
				return -1;
			}

			memset(roompanel.pwd,0,sizeof(roompanel.pwd));
			if (!stream.Read(roompanel.pwd,sizeof(roompanel.pwd),rnlen))
			{
				//////TRACE_INFO((Error, "read pwd error");
				return -1;
			}

			memset(roompanel.notice,0,sizeof(roompanel.notice));
			roompanel.chatflag = 1;
			memset(roompanel.Salutatory,0,sizeof(roompanel.Salutatory));
			memset(roompanel.logopath,0,sizeof(roompanel.logopath));

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
				pRb->RecvRoomPanelInfo(0,ROOM_NORMAL,&roompanel);
		}
		break;

	default:
		return OnPackage_Base( cmd, seq, pStream, pTunnel );
	}

	return 0;
}

int RoomDataDecoder::OnPackage_Base( short cmd, int seq, void * pStream, void * pTunnel )
{
	if(cmd < ROOM_CMD_SPLITER_1)
		return onPackage1(cmd,seq,pStream,pTunnel);
	else
		return onPackage2(cmd,seq,pStream,pTunnel);
}
