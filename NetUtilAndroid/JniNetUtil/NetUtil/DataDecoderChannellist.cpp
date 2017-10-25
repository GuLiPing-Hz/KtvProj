#include "DataDecoderChannellist.h"
#include "NetApp.h"
#include "wrap/NUtility.h"
#include "protocol.h"
#include "wrap/crypt.h"
#include "XingguangDef.h"
#include "ResultData.h"
#include "wrap/ClientSocket.h"

int ChannellistDataDecoder::onPackage(NetworkUtil::ClientSocketBase *pClient,const char* buf,unsigned int buflen)
{
	//ChannellistTunnel *pChannellistTunnel = (ChannellistTunnel*)pClient;

	char outbuf[65535];
	int outbuflen = sizeof(outbuf);

	char tempsessionkey[16] = {0};
	strncpy(tempsessionkey,LOBBYFIRSTKEY,sizeof(tempsessionkey));

	if(!NetworkUtil::StreamDecrypt(buf,(int)buflen,outbuf,outbuflen,tempsessionkey,2))
	{
		////TRACE_INFO((Error, "cann't decrypt");
		fprintf(stderr,"cann't decrypt");
		return -1;
	}

	NetworkUtil::BinaryReadStream stream(outbuf,outbuflen);

	char c;
	if(!stream.Read(c))
	{
		////TRACE_INFO((Error, "read c error");
		return -1;
	}

	short cmd;
	if(!stream.Read(cmd))
	{
		////TRACE_INFO((Error, "read cmd error");
		return -1;
	}
	int seq;
	if(!stream.Read(seq))
	{
		////TRACE_INFO((Error, "read seq error");
		return -1;
	}

	switch(cmd)
	{
	//Deprecated
	case CMD_GETSECTIONLIST_C2L2C:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if ( NULL == pRb)
			{
				break;
			}

			// 账号被冻结
			//flag:0--成功,-1--冻结
			//flag:10--版本错误
			int flag;
			if(!stream.Read(flag))
			{
				////TRACE_INFO((Error, "read flag error");
				return -1;
			}

			switch( flag )
			{
			case 0:
				{	
					short count;
					if(!stream.Read(count))
					{
						////TRACE_INFO((Error, "read count error");
						return -1;
					}

					Channel *pC = new Channel[count];
					for(int i = 0;i < count;i++)
					{
						unsigned int len;
						if(!stream.Read(pC[i].sectionid))
						{
							////TRACE_INFO((Error, "read sectionid error");
							delete []pC;
							return -1;
						}
						char sectionname[50] = {0};
						if(!stream.Read(sectionname,sizeof(sectionname),len))
						{
							////TRACE_INFO((Error, "read sectionname error");
							delete []pC;
							return -1;
						}
						if(!stream.Read(pC[i].lobbyid))
						{
							////TRACE_INFO((Error, "read lobbyid error");
							delete []pC;
							return -1;
						}
						char lobbyname[50] = {0};
						if(!stream.Read(lobbyname,sizeof(lobbyname),len))
						{
							////TRACE_INFO((Error, "read lobbyname error");
							delete []pC;
							return -1;
						}
						memset( pC[i].lobbyip_1, 0, sizeof( pC[i].lobbyip_1 ));
						if(!stream.Read(pC[i].lobbyip_1,sizeof(pC[i].lobbyip_1),len))
						{
							////TRACE_INFO((Error, "read lobbyip_1 error");
							delete []pC;
							return -1;
						}
						if(!stream.Read(pC[i].lobbyport))
						{
							////TRACE_INFO((Error, "read lobbyport_1 error");
							delete []pC;
							return -1;
						}
						memset( pC[i].lobbyip_2, 0, sizeof( pC[i].lobbyip_2 ));
						if(!stream.Read(pC[i].lobbyip_2,sizeof(pC[i].lobbyip_2),len))
						{
							////TRACE_INFO((Error, "read lobbyip_2 error");
							delete []pC;
							return -1;
						}
						strncpy(pC[i].sectionname,sectionname,sizeof(sectionname));
						strncpy(pC[i].lobbyname,lobbyname,sizeof(lobbyname));
					}
					ResultData_GetChannellist rd;
					rd.count = count;
					rd.pCh = pC;

					pRb->RecvSectionList(0,LOBBY_NORMAL,&rd,0);
					delete []pC;
				}
				break;

			case 10:
				pRb->RecvSectionList(-1,LOBBY_LOGIN_TOOOLD,NULL,0);
				break;
			case -3:
				int leftlogintime;
				if(!stream.Read(leftlogintime))
				{
					////TRACE_INFO((Error, "read leftlogintime error");
					return -1;
				}
				pRb->RecvSectionList2(-1,LOBBY_USERINFO_INDULGE,leftlogintime,NULL,0);
				break;

			case -1:
			case -2:
			default:
				pRb->RecvSectionList(-1,LOBBY_LOGIN_IDLOCK,NULL,0);
				break;
			}
			//pChannellistTunnel->Close();
		}
		break;
	case CMD_GETALLLOBBYCURNUM_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveDataV **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			ResultData_LobbyOnlineList lobbylist;

			short lobbycount;
			if(!stream.Read(lobbycount))
				return -1;

			lobbylist.lobbycount = lobbycount;

			ResultData_LobbyOnline* pRL=new ResultData_LobbyOnline[lobbycount];

			for (int i=0;i<lobbycount;i++)
			{
				int lobbyid;
				if(!stream.Read(lobbyid))
				{
					delete []pRL;
					return -1;
				}
				int onlinenum;
				if(!stream.Read(onlinenum))
				{
					delete []pRL;
					return -1;
				}

				pRL[i].lobbyid = lobbyid;
				pRL[i].onlinenum = onlinenum;
			}

			lobbylist.Lobbyonline = pRL;
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
				pRb->RecvLobbyOnlineNum(0,LOBBY_NORMAL,&lobbylist);

			delete []pRL;
		}
		break;
	case CMD_VERSION_ERROR_L2C:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvSectionList(-1,LOBBY_LOGIN_TOOOLD,NULL,0);
			}
		}
		break;
	default:
		break;
	}

	return 0;
}
