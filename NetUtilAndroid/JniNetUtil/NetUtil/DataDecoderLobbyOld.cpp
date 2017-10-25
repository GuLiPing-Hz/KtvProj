#include "DataDecoderLobby.h"
#include "NetApp.h"
#include "dbresult.h"
#include "wrap/NUtility.h"
#include "protocol.h"
#include "localstorage/DataMgr.h"
#include "DataDecodeUtilty.h"
#include "ChatCompatibility.h"

#include "wrap/config.h"
#ifndef NETUTIL_ANDROID
#define LOGI printf
#define LOGE printf
#endif

extern CDataMgr g_DM;

extern void GetAvIpFromString( char out_ip[][MAXLEN_IP], char & count, char * ip_buf, char key );

#ifndef PROTOCOL_READSTR
#define PROTOCOL_READSTR(x,len) \
if(!stream.Read((x),sizeof((x)),len))\
{\
    LOGE("Protocol Read %s, Error in file: %s,line %d\n",#x,__FILE__,__LINE__);\
	return -1;\
}\
(x)[len] = 0
#endif//STREAM_READSTR

#ifndef PROTOCOL_READ
#define PROTOCOL_READ(x) \
if(!stream.Read((x)))\
{\
    LOGE("Protocol Read %s, Error in file: %s,line %d\n",#x,__FILE__,__LINE__);\
	return -1;\
}
#endif//STREAM_READ

int _ReadTmpRoomData( BinaryReadStream & stream, RoomListEx * pInfo )
{
	unsigned int len;

	//房间player
	if(!stream.Read( pInfo->playerid ))
	{
		////TRACE_INFO((Error, "read create room playerid is error");
		return -1;
	}

	//房间id
	if(!stream.Read( pInfo->roomid ))
	{
		////TRACE_INFO((Error, "read create room id is error");
		return -1;
	}

	//ip
	memset( pInfo->ip_1, 0, sizeof( pInfo->ip_1 ));
	if(!stream.Read( pInfo->ip_1, sizeof( pInfo->ip_1 ), len ))
	{
		////TRACE_INFO((Error, "read create room ip is error");
		return -1;
	}

	//端口
	if(!stream.Read( pInfo->port ))
	{
		////TRACE_INFO((Error, "read create room port is error");
		return -1;
	}

	//名称
	memset( pInfo->m_name, 0, sizeof( pInfo->m_name ));
	if(!stream.Read( pInfo->m_name, sizeof( pInfo->m_name ), len ))
	{
		////TRACE_INFO((Error, "read create room name is error");
		return -1;
	}

	//是否有密码
	if(!stream.Read( pInfo->haspasswd ))
	{
		////TRACE_INFO((Error, "read create room has pwd is error");
		return -1;
	}

	//是否开评分
	if(!stream.Read( pInfo->scoreflag ))
	{
		////TRACE_INFO((Error, "read create room has pwd is error");
		return -1;
	}

	//是否开评论
	if(!stream.Read( pInfo->voteflag ))
	{
		////TRACE_INFO((Error, "read create room has pwd is error");
		return -1;
	}

	//最大人数
	if(!stream.Read( pInfo->nmaxplayernum ))
	{
		////TRACE_INFO((Error, "read create room max is error");
		return -1;
	}

	//ip2
	memset( pInfo->ip_2, 0, sizeof( pInfo->ip_2 ));
	if(!stream.Read( pInfo->ip_2, sizeof( pInfo->ip_2 ), len ))
	{
		////TRACE_INFO((Error, "read create room ip is error");
		return -1;
	}

	pInfo->playernum = 1;

	return 0;
}

int _ReadTmpRoomDataList( BinaryReadStream & stream, RoomListEx * pInfo )
{
	unsigned int len;

	//房间id
	if(!stream.Read( pInfo->roomid ))
	{
		////TRACE_INFO((Error, "read create room id is error");
		return -1;
	}

	//最大人数
	if(!stream.Read( pInfo->nmaxplayernum ))
	{
		////TRACE_INFO((Error, "read create room max is error");
		return -1;
	}

	//ip
	memset( pInfo->ip_1, 0, sizeof( pInfo->ip_1 ));
	if(!stream.Read( pInfo->ip_1, sizeof( pInfo->ip_1 ), len ))
	{
		////TRACE_INFO((Error, "read create room ip is error");
		return -1;
	}

	//端口
	if(!stream.Read( pInfo->port ))
	{
		////TRACE_INFO((Error, "read create room port is error");
		return -1;
	}

	//名称
	memset( pInfo->m_name, 0, sizeof( pInfo->m_name ));
	if(!stream.Read( pInfo->m_name, sizeof( pInfo->m_name ), len ))
	{
		////TRACE_INFO((Error, "read create room name is error");
		return -1;
	}

	//是否有密码
	if(!stream.Read( pInfo->haspasswd ))
	{
		////TRACE_INFO((Error, "read create room has pwd is error");
		return -1;
	}

	if(!stream.Read( pInfo->scoreflag ))
	{
		////TRACE_INFO((Error, "read create room scoreflag is error");
		return -1;
	}

	if(!stream.Read( pInfo->voteflag ))
	{
		////TRACE_INFO((Error, "read create room voteflag is error");
		return -1;
	}

	if(!stream.Read( pInfo->playernum ))
	{
		////TRACE_INFO((Error, "read create room has pwd is error");
		return -1;
	}

	//ip2
	memset( pInfo->ip_2, 0, sizeof( pInfo->ip_2 ));
	if(!stream.Read( pInfo->ip_2, sizeof( pInfo->ip_2 ), len ))
	{
		////TRACE_INFO((Error, "read create room ip is error");
		return -1;
	}

	return 0;
}

int _ParseGlobalInfo( char * str, short * partlist, int * globallist, int listmax )
{
	if(str == NULL)
		return 0;

	int length = strlen(str);
	if(length == 0)
		return 0;

	int id = 0;
	int globalid = 0;
	int goodsid = 0;
	char * pstart = str;
	char * pend = str;

	while((pend = strchr(pstart,'[')) != NULL)
	{
		if(id >= listmax)
			return 0;

		pend ++;
		pstart = pend;
		if((pend = strchr(pstart,']')) != NULL)
		{
			char vbuf[200];
			int vlen = (int)(pend - pstart);
			if(vlen != 0)
			{
				memcpy(vbuf, pstart, vlen);
				vbuf[vlen] = 0;

				char* pvstart = vbuf;
				char* pvend = vbuf;

				char vvbuf[20];
				int vvlen = 0;
				if((pvend = strchr(pvstart,',')) != NULL)
				{
					vvlen = (int)(pvend - pvstart);
					memcpy(vvbuf, pvstart, vvlen);
					vvbuf[vvlen] = 0;
					globalid = atoi(vvbuf);
					if(globallist)
						globallist[id] = globalid;
					pvend ++;
					pvstart = pvend;
				}
				vvlen = vbuf + vlen - pvstart;
				if(vvlen > 0)
				{
					memcpy(vvbuf, pvstart, vvlen);
					vvbuf[vvlen] = 0;
					goodsid = atoi(vvbuf);					
				}
				partlist[id] = (short)goodsid;		
			}
			else
			{
				//
			}
			id++;
			pend ++;
			pstart = pend;
		}
		else
		{
			return 0;
		}
	}
	return id;
};

int LobbyDataDecoder::OnPackage_Old( short cmd, int seq, void * pStream, void * pTunnel )
{
	BinaryReadStream & stream = * (BinaryReadStream *)pStream;
	LobbyTunnel * pLobbyTunnel = (LobbyTunnel *)pTunnel;

	//////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_TEST__
	ResponseBase *pRbTmp=g_NetApp.GetResponseHandler();
	if (pRbTmp )
	{
		pRbTmp->AddLog("__DEBUG_TEST__  lobby cmd is %d",cmd);
	}	
#endif
	//////////////////////////////////////////////////////////////////////////


	switch(cmd)
	{
	case CMD_GETSECTIONLIST_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
				return -1;
			RDGuard guard(*pRD,seq);

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if ( NULL == pRb )
			{
				break;
			}
			//add by jinguanfu 2010/3/12 账号被冻结
			//flag:0--成功,-1--冻结 -2--mac
			//10-版本过低
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

					//OnPackage2 op = g_NetApp.GetPackageHandler_Channellist();
					//if(op)
					//{
					//	op(NULL,cmd,&rd);
					//}
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
				pRb->RecvSectionList2(-3,LOBBY_USERINFO_INDULGE,leftlogintime,NULL,0);
				break;

			default:
				pRb->RecvSectionList(-1,LOBBY_LOGIN_IDLOCK,NULL,0);
				break;
			}
		}
		break;
	case CMD_TOKENLOGIN_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
				return -1;
			RDGuard guard(*pRD,seq);

			int ret;
			if(!stream.Read(ret))
				return -1;

			ResultData_LobbyLogin rd;
			memset(&rd,0,sizeof(ResultData_LobbyLogin));
			rd.ret = ret;

			if(ret == RET_OK||ret==RET_NOAVATAR)
			{
				char sessionkey[16] = {0};
				unsigned int sessionkeylen;
				if(!stream.Read(sessionkey,sizeof(sessionkey),sessionkeylen))
					return -1;

				//////////////////////////////////////////////////////////////////////////
				short aAvaratData[PART_LEN+LINK_LEN*10] = {0};
				rd.m_AvaratData = aAvaratData;
				_ReadUserData( stream, rd );
				//////////////////////////////////////////////////////////////////////////

				if(!stream.Read( rd.roomcount ))
					return -1;

				//接收房间列表时,增加IP和Port
				ResultData_RoomIPPort* pRoomList;
				if((pRoomList = (ResultData_RoomIPPort *)malloc(sizeof(ResultData_RoomIPPort) * rd.roomcount )) == NULL)
					return -1;

				unsigned int len;
				for(int i=0; i<rd.roomcount; i++)
				{
                    PROTOCOL_READ(pRoomList[i].roomid);
                    
					memset(pRoomList[i].ip_1,0,MAXLEN_IP);
					if(!stream.Read(pRoomList[i].ip_1,sizeof(pRoomList[i].ip_1),len))
						return -1;

					pRoomList[i].port_1=0;
					if(!stream.Read(pRoomList[i].port_1))
						return -1;

					pRoomList[i].status=0;
					if(!stream.Read(pRoomList[i].status))
						return -1;

					memset( pRoomList[i].ip_2, 0, MAXLEN_IP );
					if( !stream.Read( pRoomList[i].ip_2, sizeof(pRoomList[i].ip_2), len ))
						return -1;
				}
				rd.roomlist=pRoomList;

				//self data start
				ResultData_SelfData sd = {0};
                PROTOCOL_READ(sd.is_adult);
				if( 100 <= rd.m_level )
				{
                    PROTOCOL_READ(sd.m_remind);
                    PROTOCOL_READ(sd.m_consume);
				}
				//self data end

                PROTOCOL_READ(rd.group_versionid);
                PROTOCOL_READ(rd.m_singerlevel);
                PROTOCOL_READ(rd.m_singer_expvalue);
                PROTOCOL_READ(rd.m_wealthlevel);
				PROTOCOL_READ(rd.m_wealth_expvalue);
                PROTOCOL_READ(rd.m_roomright_versionid);

				unsigned int name_len;
				PROTOCOL_READSTR(rd.songname,name_len);
				PROTOCOL_READSTR(rd.singername,name_len);
				PROTOCOL_READ(rd.score);

				////TRACE_INFO((Debug, "singerlevel=%d, singer_expvalue=%d, wealthlevel=%d, wealth_expvalue=%d", rd.m_singerlevel, rd.m_singer_expvalue, rd.m_wealthlevel, rd.m_wealth_expvalue);

				memset(pLobbyTunnel->m_Sessionkey,0,sizeof(pLobbyTunnel->m_Sessionkey));
				strncpy(pLobbyTunnel->m_Sessionkey,sessionkey,sizeof(pLobbyTunnel->m_Sessionkey));
				pLobbyTunnel->SetTokenLogin(true);

                //保存信息到本地。
                //save my info into data mananger
                g_DM.saveMyIdx(rd.m_idx);
                g_DM.saveSelfData(&sd);
                
                USER_DATA_BASE tmpUser;
                memset(&tmpUser,0,sizeof(tmpUser));
                tmpUser.location_state = IF_STATE_OK;
                tmpUser.user_info.lobbyid = 0;
                tmpUser.user_info.roomid = 0;
                //copy
                memcpy(&(tmpUser.user_info.base),&rd,sizeof(ResultData_UserFullInfo));
                tmpUser.user_info.relationship = RELATIONSHIP_SELF;
                                
                tmpUser.user_info.base.m_AvatarId = -1;
                tmpUser.user_info.base.m_AvaratCount = 0;
                tmpUser.user_info.base.m_AvaratData = NULL;
                
                tmpUser.name_state = IF_STATE_OK;
                tmpUser.info_state = IF_STATE_OK;
				tmpUser.version = -2;//-2 定义不需要保存到本地文件中。
                g_DM.saveUserInfo(tmpUser);
                
                for(int i=0;i<rd.roomcount;i++)
                {
                    ROOM_DATA tmpRoom={0};
                    tmpRoom.base.idx = rd.roomlist[i].roomid;
                    tmpRoom.base.state = rd.roomlist[i].status;
                    strcpy(tmpRoom.base.ip_1,rd.roomlist[i].ip_1);
                    tmpRoom.base.port = rd.roomlist[i].port_1;
                    strcpy(tmpRoom.base.ip_2,rd.roomlist[i].ip_2);
                    //保存房间信息
                    g_DM.saveRoomInfo(tmpRoom);
                }
                g_DM.changeMyLobby(1);
				
				ResponseBase *pRb=g_NetApp.GetResponseHandler();
				if (pRb)
				{
					if( RET_OK == ret )
					{
						g_NetApp.SetMyIDx( rd.m_idx );
						pRb->RecvTokenAuth4Lobby( 0, LOBBY_NORMAL, & rd, & sd, (*pRD)->cseq);
					}
					else if( RET_NOAVATAR == ret )
					{
						pRb->RecvTokenAuth4Lobby( 0, LOBBY_LOGIN_NOAVATAR, & rd, & sd, (*pRD)->cseq );
					}
				}
			}else if( RET_NOTEXIST == ret ){
				////TRACE_INFO((Error, "CMD_TOKENLOGIN_C2L2C ret=%d", ret);
				ResponseBase *pRb=g_NetApp.GetResponseHandler();
				if (pRb)
					pRb->RecvTokenAuth4Lobby( -1, LOBBY_LOGIN_NOTEXIST, NULL, NULL, (*pRD)->cseq );
			}

			free(rd.roomlist);
		}
		break;
	case CMD_USERLOGIN_L2C:
		{
// 			ResultData_LobbyLoginUser rlu;
// 			memset(&rlu,0,sizeof(ResultData_LobbyLoginUser));
// 
// 			if(!stream.Read(rlu.idx))
// 				return -1;
// 
// 			if(!stream.Read(rlu.relation))
// 				return -1;
// 
// 			if(!stream.Read(rlu.lobbyid))
// 				return -1;
// 
// 			ResponseBase *pRb=g_NetApp.GetResponseHandler();
// 			if (pRb)
// 				pRb->RecvUserLoginLobbyNotice(0,&rlu);
		}
		break;
	case CMD_USERLOGOUT_L2C:
		{
			ResultData_LobbyLoginUser rlu;
			memset(&rlu,0,sizeof(ResultData_LobbyLoginUser));

			PROTOCOL_READ(rlu.idx);
			PROTOCOL_READ(rlu.relation);
			PROTOCOL_READ(rlu.lobbyid);

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
				pRb->RecvUserLogoutLobbyNotice(0,&rlu);
		}
		break;
	case CMD_GETUSERINFO_C2L2C:
		{
			int ret;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}

			ResultData_UserFullInfo rd;
			memset(&rd,0,sizeof(ResultData_UserFullInfo));
			rd.ret = ret;

			if(ret == RET_OK)
			{
				//////////////////////////////////////////////////////////////////////////
				short aAvaratData[PART_LEN+LINK_LEN*10] = {0};
				rd.m_AvaratData = aAvaratData;
				_ReadUserData( stream, rd );

				PROTOCOL_READ(rd.m_singerlevel);
				rd.m_singer_expvalue = -1;
				PROTOCOL_READ(rd.m_wealthlevel);
				rd.m_wealth_expvalue = -1;

				g_DM.saveUserInfo(rd,0);//deprecated
				ResponseBase *pRb=g_NetApp.GetResponseHandler();
				if (pRb)
				{
					pRb->RecvUserInfo(0,LOBBY_NORMAL,&rd);
				}
			}
			else if (ret == RET_NOTEXIST)
			{
				ResponseBase *pRb=g_NetApp.GetResponseHandler();
				if(!stream.Read(rd.m_idx))
					return -1;
				if (pRb)
				{
					pRb->RecvUserInfo(-1,LOBBY_USERINFO_NOTEXIST,&rd);
				}
			}
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op(NULL,(int)cmd,&rd);
			//}
		}
		break;
	case CMD_GETFRIENDLISTSTATUS_C2L2C:
	case CMD_GETBLACKLIST_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}
			RDGuard guard(*pRD,seq);

			char buf2[65535];
			unsigned int buf2len;
			if(!stream.Read(buf2,sizeof(buf2),buf2len))
			{
				////TRACE_INFO((Error, "read buf2 error");
				return -1;
			}
			BinaryReadStream stream(buf2,buf2len);

			int count;
			if(!stream.Read(count))
			{
				////TRACE_INFO((Error, "read count error");
				return -1;
			}
			UserStatus *pUS = new UserStatus[count];
			for(int i = 0;i< count;i++)
			{
				if(!stream.Read(pUS[i].idx))
				{
					////TRACE_INFO((Error, "read idx error");
					delete []pUS; 
					return -1;
				}
				if(!stream.Read(pUS[i].lobbyid))
				{
					////TRACE_INFO((Error, "read lobbyid error");
					delete []pUS;
					return -1;
				}
				if(!stream.Read(pUS[i].roomid))
				{
					////TRACE_INFO((Error, "read roomid error");
					delete []pUS;
					return -1;
				}
			}
			ResultData_GetFriendlist rd;
			rd.count = count;
			rd.pUS = pUS;
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op(NULL,(int)cmd,&rd);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				if (cmd==CMD_GETFRIENDLISTSTATUS_C2L2C)
					pRb->RecvFriendStatus(0,LOBBY_NORMAL,&rd);
				else if(cmd==CMD_GETBLACKLIST_C2L2C)
					pRb->RecvBlackStatus(0,LOBBY_NORMAL,&rd);
			}
			delete []pUS;
		}
		break;
	case CMD_GETCLUBLISTSTATUS_C2L2C:
		{

			char buf2[65535];
			unsigned int buf2len;
			if(!stream.Read(buf2,sizeof(buf2),buf2len))
			{
				////TRACE_INFO((Error, "read buf2 error");
				return -1;
			}
			BinaryReadStream stream(buf2,buf2len);

			int count;
			if(!stream.Read(count))
			{
				////TRACE_INFO((Error, "read count error");
				return -1;
			}
			ClubUserStatus *pCUS = new ClubUserStatus[count];
			for(int i = 0;i< count;i++)
			{
				if(!stream.Read(pCUS[i].idx))
				{
					////TRACE_INFO((Error, "read idx error");
					delete []pCUS;
					return -1;
				}
				if(!stream.Read(pCUS[i].pro))
				{
					////TRACE_INFO((Error, "read pro error");
					delete []pCUS;
					return -1;
				}
				if(!stream.Read(pCUS[i].lobbyid))
				{
					////TRACE_INFO((Error, "read lobbyid error");
					delete []pCUS;
					return -1;
				}
				if(!stream.Read(pCUS[i].roomid))
				{
					////TRACE_INFO((Error, "read roomid error");
					delete []pCUS;
					return -1;
				}
			}
			ResultData_GetClublist rd;
			rd.count = count;
			rd.pCUS = pCUS;
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op(NULL,(int)cmd,&rd);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvClubStatus(0,LOBBY_NORMAL,&rd);
			}
			delete []pCUS;
		}
		break;
	case CMD_ADDFRIEND_C2L2C:
		{
			//TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			//ReserveData **pRD = pSeqMap->Get(seq);
			//if(!pRD)
			//{
			//	return 0;
			//}

			//RDGuard guard(*pRD,seq);
			//ReserveData_AddFriend *pRDAf = (ReserveData_AddFriend *)(*pRD);
			//ReserveData_AddFriend RDAf;
			int ret;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			int idx;
			if(!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}
			//RDAf.idx=idx;
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op((ReserveDataS *)&RDAf,cmd,&ret);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				switch( ret )
				{
				case STATUS_NORMAL:
					pRb->RecvAddFriend(0,LOBBY_ADDFRIEND_NORMAL,idx);
					break;
				case STATUS_INPROCESS:
					pRb->RecvAddFriend(-1,LOBBY_ADDFRIEND_INPROCESS,idx);
					break;
				case STATUS_BLACK:
					pRb->RecvAddFriend(-1,LOBBY_ADDFRIEND_REPEAT,idx);
					break;
				case STATUS_FRIEND:
					pRb->RecvAddFriend(-1,LOBBY_ADDFRIEND_FRIEND,idx);
					break;
				case STATUS_REFUSE:
					pRb->RecvAddFriend(-1,LOBBY_ADDFRIEND_REFUSE,idx);
					break;
				case STATUS_USER_NOTEXIST:
					pRb->RecvAddFriend(-1,LOBBY_ADDFRIEND_OFFLINE,idx);
					break;
				case ADDFRIBLK_ADDSELF:
					pRb->RecvAddFriend(-1,LOBBY_ADDFRIEND_ADDSELF,idx);
					break;
				case ADDFRIBLK_ALREADY:
					pRb->RecvAddFriend(-1,LOBBY_ADDFRIEND_ALREADY,idx);
					break;
				case ADDFRIBLK_NOTEXIST:
					pRb->RecvAddFriend(-1,LOBBY_ADDFRIEND_NOTEXIST,idx);
					break;
				case ADDFRIBLK_MAXCOUNT:
					pRb->RecvAddFriend(-1,LOBBY_ADDFRIEND_MAXCOUNT,idx);
					break;
				case ADDFRIBLK_MAXCOUNT2:
					pRb->RecvAddFriend(-1,LOBBY_ADDFRIEND_MAXCOUNT2,idx);
					break;
				case XGC_STATUS_HAS_BLACKED:
					pRb->RecvAddFriend(-1,LOBBY_ADDFRIEND_INBLACK,idx);
					break;
				}
			}
		}
		break;
	case CMD_APPLY_ADDFRIEND_L2C:
		{
			int idx;
			if(!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvAddFriend( -1, LOBBY_ADDFRIEND_WAIT, idx );
			}
		}
		break;
	case CMD_ADDBLACK_C2L2C:
	case CMD_DELFRIEND_C2L2C:
	case CMD_DELBLACK_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			ReserveData_AddFriend *pRDAf = (ReserveData_AddFriend *)(*pRD);
			int ret;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op((ReserveDataS *)pRDAf,cmd,&ret);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				if (cmd==CMD_ADDBLACK_C2L2C)
				{
					switch (ret)
					{
					case ADDFRIBLK_ADDSELF:
						pRb->RecvAddBlack(-1,LOBBY_ADDBLACK_ADDSELF,pRDAf->cseq);
						break;
					case ADDFRIBLK_ALREADY:
						pRb->RecvAddBlack(-1,LOBBY_ADDBLACK_ALREADY,pRDAf->cseq);
						break;
					case ADDFRIBLK_NOTEXIST:
						pRb->RecvAddBlack(-1,LOBBY_ADDBLACK_NOTEXIST,pRDAf->cseq);
						break;
					case ADDFRIBLK_MAXCOUNT:
						pRb->RecvAddBlack(-1,LOBBY_ADDBLACK_MAXCOUNT,pRDAf->cseq);
						break;
					default:
						pRb->RecvAddBlack(0,LOBBY_ADDBLACK_NORMAL,pRDAf->cseq);
					}
				}
				else if(cmd==CMD_DELFRIEND_C2L2C)
				{
					if (ret>0)
						pRb->RecvDelFriend(0,LOBBY_ADDFRIEND_NORMAL,pRDAf->cseq);
					else
						pRb->RecvDelFriend(-1,LOBBY_DELFRIEND_NOTEXIST,pRDAf->cseq);
				}
				else if(cmd==CMD_DELBLACK_C2L2C)
				{	
					if (ret>0)
						pRb->RecvDelBlack(0,LOBBY_ADDBLACK_NORMAL,pRDAf->cseq);
					else
						pRb->RecvDelBlack(-1,LOBBY_DELBLACK_NOTEXIST,pRDAf->cseq);
				}
			}
		}
		break;
		// modify by jinguanfu 2009/12/21 <begin>
	case CMD_USERCHAT_C2L2C:
	case CMD_LOBBYCHAT_C2L2C:
	case CMD_ALLCHAT_C2L2C:
		{
			ResponseBase* pRb=g_NetApp.GetResponseHandler();
			int from_idx;
			if(!stream.Read(from_idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}
			char content[MSG_LENGTH]={0};
			unsigned int contentlen;
			if(!stream.Read(content, sizeof(content), contentlen))
			{
				////TRACE_INFO((Error, "read message error");
				return -1;
			}
			int time = 0;
			if(!stream.Read( time ))
			{
				////TRACE_INFO((Error, "read message error");
				return -1;
			}

			pRb->RecvChatFromUser(0,from_idx,content,contentlen, time );
		}
		break;
	case CMD_USERCHATACK_L2C:
		{
			ResponseBase* pRb=g_NetApp.GetResponseHandler();
			int to_idx;
			if(!stream.Read(to_idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}
			char content[MSG_LENGTH]={0};
			unsigned int contentlen;
			if(!stream.Read(content, sizeof(content), contentlen))
			{
				////TRACE_INFO((Error, "read message error");
				return -1;
			}
			int time = 0;
			if(!stream.Read( time ))
			{
				////TRACE_INFO((Error, "read message error");
				return -1;
			}

			pRb->SendChatFromUser(0,to_idx,content,contentlen, time );
		}
		break;
	case CMD_FATIGUEUSER_L2C:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				//pRb->AddLog( "Time is up" );
				pRb->RecvIndulgeTimeUp();
			}
		}
		break;
	case CMD_CLUBCHAT_C2L2C:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb==NULL)
			{
				////TRACE_INFO((Error, "ResponseBase is NULL");
				return -1;
			}
			int from_idx;
			if(!stream.Read(from_idx))
			{
				////TRACE_INFO((Error, "read idx error");
				pRb->RecvChatFromClub(-1,0,"",0);
				return -1;
			}
			int clubid;
			if(!stream.Read(clubid))
			{
				////TRACE_INFO((Error, "read clubid error");
				pRb->RecvChatFromClub(-1,0,"",0);
				return -1;
			}
			char content[MSG_LENGTH]={0};
			unsigned int contentlen;
			if(!stream.Read(content, sizeof(content), contentlen))
			{
				////TRACE_INFO((Error, "read message error");
				pRb->RecvChatFromClub(-1,0,"",0);
				return -1;
			}
			pRb->RecvChatFromClub(0,from_idx,content,contentlen);
			/*
			ResultData_ChatMessage msg;
			if(!stream.Read(msg.idx))
			{
			////TRACE_INFO((Error, "read idx error");
			return -1;
			}
			if(!stream.Read(msg.clubid))
			{
			////TRACE_INFO((Error, "read idx error");
			return -1;
			}
			char content[MSG_LENGTH] = {0};
			unsigned int length;
			if(!stream.Read(content, sizeof(content), length))
			{
			////TRACE_INFO((Error, "read message error");
			return -1;
			}
			msg.content = content;
			OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			if(op)
			{
			op(NULL,cmd,&msg);
			}
			*/
		}
		break;
		// modify by jinguanfu 2009/12/21 <end>
	case CMD_APPLYADDCLUB_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			ReserveData_ApplyClub *pRAC = (ReserveData_ApplyClub*)(*pRD);
			int ret;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op((ReserveDataS *)pRAC,cmd,&ret);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvApplyClub(0,ret,pRAC->cseq);
			}
		}
		break;
	case CMD_EXITCLUB_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			ReserveData_ApplyClub *pRAC = (ReserveData_ApplyClub*)(*pRD);
			int ret;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op((ReserveDataS *)pRAC,cmd,&ret);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvExitClub(0,ret,pRAC->cseq);
			}

		}
		break;
	case CMD_GETAPPLYLISTCOUNT_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			ReserveData_ApplyClub *pRAC = (ReserveData_ApplyClub*)(*pRD);
			int count;
			if(!stream.Read(count))
			{
				////TRACE_INFO((Error, "read count error");
				return -1;
			}
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op((ReserveDataS *)pRAC,cmd,&count);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvApplyListCount(0,0,count,pRAC->cseq);
			}
		}
		break;
	case CMD_GETAPPLYLIST_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			ReserveData_ApplyClub *pRAC = (ReserveData_ApplyClub*)(*pRD);
			ResultData_ClubApplyList applylist;
			int count;
			if(!stream.Read(count))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			applylist.count = count;

			ClubApply *pCA;
			if(NULL == (pCA = (ClubApply *)malloc(sizeof(ClubApply) * count)))
				return -1;

			applylist.pCA = pCA;
			VoidGuard CA(pCA);

			unsigned int len;
			for(int i=0; i<count; i++)
			{
				if(!stream.Read(pCA->id))
				{
					////TRACE_INFO((Error, "read id error");
					return -1;
				}

				if(!stream.Read(pCA->idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}

				if(!stream.Read(pCA->time, sizeof(pCA->time), len))
				{
					////TRACE_INFO((Error, "read time error");
					return -1;
				}
				pCA->time[len] = 0;
				pCA++;
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op((ReserveDataS *)pRAC,cmd,&applylist); 
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvApplyList(0,0,&applylist,pRAC->cseq);
			}
		}
		break;
	case CMD_APPLYADDCLUBOK_C2L2C:
		{
			ResultData_Approvelresult rar;
			if(!stream.Read(rar.ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			if(!stream.Read(rar.id))
			{
				////TRACE_INFO((Error, "read id error");
				return -1;
			}
			if(rar.ret>=0)
			{
				if(!stream.Read(rar.idx))
				{
					////TRACE_INFO((Error, "read idx error");
					return -1;
				}
			}
			else
				rar.idx = 0;
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op(NULL,cmd,&rar);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvAddClubOk(0,&rar);
			}
		}
		break;
	case CMD_DELAPPLYLIST_C2L2C:
		{
			ResultData_Approvelresult rar;
			if(!stream.Read(rar.ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			if(!stream.Read(rar.id))
			{
				////TRACE_INFO((Error, "read id error");
				return -1;
			}
			rar.idx = 0;
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op(NULL,cmd,&rar);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvDelApplyList(0,&rar);
			}
		}
		break;
		/*case CMD_INFORMUSERADDCLUB_L2C:
		{
		int idx;
		if(!stream.Read(idx))
		{
		////TRACE_INFO((Error, "read idx errer");
		return -1;
		}
		ResponseBase *pRb=g_NetApp.GetResponseHandler();
		if (pRb)
		{
		pRb->RecvAddClubNotice(0,idx);
		}
		}
		break;*/
	case CMD_CREATECLUB_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			ReserveData_ApplyClub *pRAC = (ReserveData_ApplyClub*)(*pRD);
			int clubid;
			if(!stream.Read(clubid))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op((ReserveDataS *)pRAC,cmd,&clubid);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvCreateClub(0,0,clubid,pRAC->cseq);
			}
		}
		break;
	case CMD_DISMISSCLUB_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			ReserveData_ApplyClub *pRAC = (ReserveData_ApplyClub*)(*pRD);
			int ret;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op((ReserveDataS *)pRAC,cmd,&ret);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvDismissClub(ret,ret,pRAC->cseq);
			}
		}
		break;
	case CMD_GETCLUBINFO_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			ReserveData_ApplyClub *pRAC = (ReserveData_ApplyClub*)(*pRD);

			char buf[65535];
			unsigned int buflen;
			if(!stream.Read(buf,sizeof(buf),buflen))
			{
				return -1;
			}
			BinaryReadStream stream2(buf,buflen);

			ResultSet dbresult;
			if(dbresult.ParseResult(buf, buflen) == false)
			{
				return -1;
			}

			ResultSet *prs = &dbresult;
			if(prs->rownum != 1)
				return -1;

			ResultData_ClubFullInfo rcfi;
			if(prs->Read(0, 0, rcfi.name, sizeof(rcfi.name)) ==false)
				return -1;
			if(prs->Read(0, 1, rcfi.maxnumber) == false)
				return -1;
			if(prs->Read(0, 2, rcfi.slogan, sizeof(rcfi.slogan)) == false)
				return -1;
			if(prs->Read(0, 3, rcfi.notice, sizeof(rcfi.notice)) == false)
				return -1;
			if(prs->Read(0, 4, rcfi.picture, sizeof(rcfi.picture)) == false)
				return -1;
			if(prs->Read(0, 5, rcfi.adminidx) == false)
				return -1;
			if(prs->Read(0, 6, rcfi.admintele, sizeof(rcfi.admintele)) == false)
				return -1;
			if(prs->Read(0, 7, rcfi.adminQQ, sizeof(rcfi.adminQQ)) == false)
				return -1;

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvClubInfo(0,0,&rcfi,pRAC->cseq);
			}

			//short count = prs->rownum;
			//short colcount = prs->colnum;
		}
		break;
	case CMD_GETROOMLIST_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			//ReserveData *pRAC = (ReserveData*)(*pRD);
			ResultDate_RoomList roomlist;
			int count;
			if(!stream.Read(count))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			roomlist.count = count;

			int *plist;
			if(NULL == (plist = (int *)malloc(sizeof(int) * count)))
				return -1;

			//roomlist.list = plist;
			VoidGuard RI(plist);

			for(int i=0; i<count; i++)
			{
				if(!stream.Read(plist[i]))
				{
					////TRACE_INFO((Error, "read id error");
					return -1;
				}
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op((ReserveDataS *)pRAC,cmd,&roomlist); 
			//}
			//ResponseBase *pRb=g_NetApp.GetResponseHandler();
			//if (pRb)
			//{
			//	pRb->RecvRoomList(0,&roomlist);
			//}
		}
		break;
	case CMD_GETPRACTICEROOMLIST_C2L2C:
		{
			ResultDate_RoomListEx list;
			if(!stream.Read(list.count))
			{
				////TRACE_INFO((Error, "read roomcount error");
				return -1;
			}

			if( MAXLEN_TMPROOM_COUNT < list.count )
			{
				////TRACE_INFO((Error, "roomcount error");
				return -1;
			}

			list.pRL = new RoomListEx[list.count];
			for( int i=0; i<list.count; ++i )
			{
				if( 0 != _ReadTmpRoomDataList( stream, list.pRL + i ))
				{
					delete[] list.pRL ;
					return -1;
				}
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvRoomListEx( 0, & list );
			}

			delete[] list.pRL ;
		}
		break;
	case CMD_GETROOMINFO_C2L2C:
		{
			ResultData_RoomInfo roominfo;
			unsigned int len;
			if(!stream.Read(roominfo.idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}
			if(!stream.Read(roominfo.hallid))
			{
				////TRACE_INFO((Error, "read hallid error");
				return -1;
			}
			if(!stream.Read(roominfo.name, sizeof(roominfo.name) - 1, len))
			{
				////TRACE_INFO((Error, "read name error");
				return -1;
			}
			roominfo.name[len] = 0;
			//modify by jinguanfu 2010/4/8 <begin> 由接收密码改为密码标识
			roominfo.pwdflag = 0;
			if(!stream.Read(roominfo.pwdflag))
			{
				////TRACE_INFO((Error, "read pwdflag error");
				return -1;
			}
			//modify by jinguanfu 2010/4/8 <end>
			if(!stream.Read(roominfo.type))
			{
				////TRACE_INFO((Error, "read type error");
				return -1;
			}

			if(!stream.Read(roominfo.state))
			{
				////TRACE_INFO((Error, "read state error");
				return -1;
			}
			if(!stream.Read(roominfo.masterid))
			{
				////TRACE_INFO((Error, "read masterid error");
				return -1;
			}
			if(!stream.Read(roominfo.createdate, sizeof(roominfo.createdate) - 1, len))
			{
				////TRACE_INFO((Error, "read createdate error");
				return -1;
			}
			roominfo.createdate[len] = 0;
			if(!stream.Read(roominfo.sortflag, sizeof(roominfo.sortflag) - 1, len))
			{
				////TRACE_INFO((Error, "read sortflag error");
				return -1;
			}
			roominfo.sortflag[len] = 0;
			if(!stream.Read(roominfo.maxusernum))
			{
				////TRACE_INFO((Error, "read maxusernum error");
				return -1;
			}
			if(!stream.Read(roominfo.deadline, sizeof(roominfo.deadline) - 1, len))
			{
				////TRACE_INFO((Error, "read deadline error");
				return -1;
			}
			roominfo.deadline[len] = 0;
			if(!stream.Read(roominfo.ispassaudit, sizeof(roominfo.ispassaudit) - 1, len))
			{
				////TRACE_INFO((Error, "read ispassaudit error");
				return -1;
			}
			roominfo.ispassaudit[len] = 0;
			if(!stream.Read(roominfo.introduce, sizeof(roominfo.introduce) - 1, len))
			{
				////TRACE_INFO((Error, "read introduce error");
				return -1;
			}
			roominfo.introduce[len] = 0;
			if(!stream.Read(roominfo.ip_1, sizeof(roominfo.ip_1) - 1, len))
			{
				////TRACE_INFO((Error, "read ip error");
				return -1;
			}
			roominfo.ip_1[len] = 0;
			if(!stream.Read(roominfo.port))
			{
				////TRACE_INFO((Error, "read port error");
				return -1;
			}
			if(!stream.Read(roominfo.content, sizeof(roominfo.content) - 1, len))
			{
				////TRACE_INFO((Error, "read content error");
				return -1;
			}
			roominfo.content[len] = 0;
			if(!stream.Read(roominfo.updatedate, sizeof(roominfo.updatedate) - 1, len))
			{
				////TRACE_INFO((Error, "read updatedate error");
				return -1;
			}
			roominfo.updatedate[len] = 0;

			if(!stream.Read(roominfo.logo, sizeof(roominfo.logo) - 1, len))
			{
				////TRACE_INFO((Error, "read logo error");
				return -1;
			}
			roominfo.logo[len] = 0;

			//add by jinguanfu 2010/4/1 <begin>
			roominfo.chatflag = 0;
			if (!stream.Read(roominfo.chatflag))
			{
				////TRACE_INFO((Error, "read chatflag error");
				return -1;
			}
			roominfo.inoutflag = 0;
			if (!stream.Read(roominfo.inoutflag))
			{
				////TRACE_INFO((Error, "read inoutflag error");
				return -1;
			}
			//add by jinguanfu 2010/4/1 <end>

			roominfo.useronlyflag=0;
			if (!stream.Read(roominfo.useronlyflag))
			{
				////TRACE_INFO((Error, "read useronlyflag error");
				return -1;
			}
			roominfo.closeflag=0;
			if (!stream.Read(roominfo.closeflag))
			{
				////TRACE_INFO((Error, "read closeflag error");
				return -1;
			}
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op(NULL,cmd,&roominfo); 
			//}
			if(!stream.Read( roominfo.maxusernum ))
			{
				////TRACE_INFO((Error, "read ip error");
				return -1;
			}
			if(!stream.Read(roominfo.ip_2, sizeof(roominfo.ip_2) - 1, len))
			{
				////TRACE_INFO((Error, "read ip error");
				return -1;
			}
			roominfo.ip_2[len] = 0;

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvRoomInfo(0,&roominfo);
			}
		}
		break;
	case CMD_CREATEAVATAR_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			int ret;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op((ReserveDataS *)*pRD,cmd,&ret);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				switch( ret )
				{
				case 0:
					pRb->RecvCreateAvatar( 0, 0 );
					break;

				case 1:
					pRb->RecvCreateAvatar( -1, LOBBY_CREATEAVARAT_ERROR );
					break;
				}
			}
		}
		break;
		//add by cx 10-5-20
	case CMD_CHANGEAVATAR_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return 0;
			}

			RDGuard guard(*pRD,seq);
			int ret;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				switch( ret )
				{
				case 0:
					pRb->RecvChangeAvatar( 0, 0 );
					break;

				default:
					pRb->RecvChangeAvatar( -1, LOBBY_CHANGEAVARAT_NOFIND );
					break;
				}
			}
		}
		break;
	case CMD_CHANGENICKNAME_C2L2C:
		{
			int ret;
			if(!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret error");
				return -1;
			}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				switch( ret )
				{
				case 0:
					{
						char name[MAXLEN_NICKNAME]={0};
						unsigned int name_len;
						if (!stream.Read(name,sizeof(name),name_len))
						{
							////TRACE_INFO((Error, "read name error");
							return -1;
						}

						pRb->RecvChangeNickName( 0, 0, name, name_len );
					}
					break;

				default:
					pRb->RecvChangeNickName( -1, LOBBY_CHANGENICKNAME_ERR, NULL, 0 );
					break;
				}
			}
		}
		break;
	case CMD_USERENTERROOM_L2C:
		{
			ResultData_LobbyRoomEnter rlre;
			memset(&rlre,0,sizeof(ResultData_LobbyRoomEnter));

			if(!stream.Read(rlre.idx))
			{
				////TRACE_INFO((Error, "read idx error");
				return -1;
			}

			if(!stream.Read(rlre.relation))
			{
				////TRACE_INFO((Error, "read relation error");
				return -1;
			}

			if(!stream.Read(rlre.lobbyid))
			{
				////TRACE_INFO((Error, "read lobbyid error");
				return -1;
			}

			if(!stream.Read(rlre.roomid))
			{
				////TRACE_INFO((Error, "read lobbyid error");
				return -1;
			}

			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op(NULL,(int)cmd,&rlre);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvLobbyEnterRoom(0,&rlre);
			}
		}
		break;
	case CMD_USERLEAVEROOM_L2C:
		{
			ResultData_LobbyRoomEnter rlrl;
			memset(&rlrl,0,sizeof(ResultData_LobbyRoomEnter));

			if(!stream.Read(rlrl.idx))
			{
				return -1;
			}

			if(!stream.Read(rlrl.relation))
			{
				return -1;
			}

			if(!stream.Read(rlrl.lobbyid))
			{
				return -1;
			}

			if(!stream.Read(rlrl.roomid))
			{
				return -1;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvLobbyLeaveRoom(0,&rlrl);
			}
		}
		break;
	case CMD_ADDFRIENDINFORM_L2C:
		{
			int idx;
			if(!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx errer");
				return -1;
			}
			//OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			//if(op)
			//{
			//	op(NULL,cmd,&idx);
			//}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvAddFriendNotice(0,idx);
			}

		}
		break;
		//add by jinguanfu 2009/12/18 <begin>
	case CMD_BROAD_ALL_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb==NULL)
			{
				////TRACE_INFO((Error, "ResponseBase is NULL");
				return -1;
			}
			int ret ;
			if(!stream.Read(ret ))
			{
				////TRACE_INFO((Error, "read ret  error");
				return -1;
			}

			if(ret==RET_OK)	//喇叭发送成功
			{
				int gold;
				if(!stream.Read(gold))
				{
					return -1;
				}
				int silver;
				if(!stream.Read(silver))
				{
					return -1;
				}

				pRb->RecvSpeakerToAll(0,LOBBY_NORMAL,gold,silver);
			}
			else if(ret==-1)	//发送失败、余额不足
			{
				pRb->RecvSpeakerToAll(-1,LOBBY_SPEAKER_BALANCE,0,0);
			}
		}
		break;
	case CMD_BROAD_SB_ALL_L2C:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb==NULL)
			{
				////TRACE_INFO((Error, "ResponseBase is NULL");
				return -1;
			}
			int ret ;
			if(!stream.Read(ret ))
			{
				////TRACE_INFO((Error, "read ret  error");
				pRb->RecvSpeakerFromAll(-1,0,0,"",0);
				return -1;
			}
			int from_idx;
			if(!stream.Read(from_idx))
			{
				////TRACE_INFO((Error, "read from_id  error");
				pRb->RecvSpeakerFromAll(-1,0,0,"",0);
				return -1;
			}
			char content[MSG_LENGTH]={0};
			unsigned int contentlen;
			if (!stream.Read(content,sizeof(content),contentlen))
			{
				////TRACE_INFO((Error, "read content  error");
				pRb->RecvSpeakerFromAll(-1,0,0,"",0);
				return -1;
			}

			pRb->RecvSpeakerFromAll(0,SPEAKER_USER,from_idx,content,contentlen);

		}
		break;
	case CMD_BROAD_LOBBY_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
				return -1;
			RDGuard guard(*pRD,seq);

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb==NULL)
			{
				LOGE("ResponseBase is NULL\n");
				return -1;
			}
			int ret ;
            PROTOCOL_READ(ret);

			if(ret==RET_OK)	//喇叭发送成功
			{
				int gold;
                PROTOCOL_READ(gold);
				int silver;
                PROTOCOL_READ(silver);
                //更新本地数据库的个人金币信息
                g_DM.saveMoney(gold,silver);

				pRb->RecvSpeakerToLobby(0,LOBBY_NORMAL,gold,silver);
			}
			else if(ret==-1)	//发送失败、余额不足
			{
				pRb->RecvSpeakerToLobby(-1,LOBBY_SPEAKER_BALANCE,0,0);
			}			
		}
		break;
	case CMD_BROAD_SB_LOBBY_L2C:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb==NULL)
				return -1;
			int ret ;
			PROTOCOL_READ(ret);

			ResultData_UserSpeakerInfo spkInfo = {0};
			PROTOCOL_READ(spkInfo.fromidx);
			
			unsigned int tmp_len = 0;
			PROTOCOL_READSTR(spkInfo.chat,tmp_len);
			PROTOCOL_READ(spkInfo.roomId);
			PROTOCOL_READSTR(spkInfo.roomName,tmp_len);
			PROTOCOL_READ(spkInfo.time);
			int type = 0;//BROADTYPE type
			PROTOCOL_READ(type);

			if(type == BT_BROAD)
			{
				char chatNoraml[MAXLEN_USERSPEAKER_INFO] = {0};// 小喇叭内容
				int len = MAXLEN_USERSPEAKER_INFO;
				int ret = CChatCompatibility::ConvertXml2Normal(chatNoraml,len,spkInfo.chat);
				if(ret == 0 || ret == -2)//如果转换成功，或者UTF转换失败
					strcpy(spkInfo.chat,chatNoraml);
			}

			pRb->RecvSpeakerFromLobby( 0, &spkInfo,type,false);
		}
		break;
	case CMD_GETLOBBYINFO_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);

			ReserveData_LobbyInfo *pRDLI=(ReserveData_LobbyInfo*)*pRD;

			ResultData_LobbyName rlrl;
			memset(&rlrl,0,sizeof(ResultData_LobbyName));
			unsigned int len;

			if(!stream.Read(rlrl.lobbyid))
			{
				////TRACE_INFO((Error, "read lobbyid error");
				return -1;
			}			
			if(!stream.Read(rlrl.lobbyname, sizeof(rlrl.lobbyname) - 1, len))
			{
				////TRACE_INFO((Error, "read lobbyname error");
				return -1;
			}

			/*			OnPackage2 op = g_NetApp.GetPackageHandler_Lobby();
			if(op)
			{
			op(NULL,(int)cmd,&rlrl);
			}	*/	
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvLobbyInfo(0,0,&rlrl,pRDLI->cseq);
			}
		}
		break;
	case CMD_SYSTEMMSG_L2C:
		{
			char content[MAXLEN_SYSTEM_MSG] = {0};
			unsigned int length;
			if(!stream.Read(content, sizeof(content), length))
			{
				////TRACE_INFO((Error, "read message error");
				return -1;
			}

			int oneshot = 0;
			if(!stream.Read( oneshot ))
			{
				////TRACE_INFO((Error, "read oneshot error");
				return -1;
			}

			int interval_minuter = 0;
			if(!stream.Read( interval_minuter ))
			{
				////TRACE_INFO((Error, "read interval_minuter error");
				return -1;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if( NULL != pRb )
				pRb->RecvSystemNotice( content, ( int )length, 1 == oneshot, interval_minuter );
		}
		break;
	case CMD_SYSTEMSPEAK_L2C:
		{
			char content[MAXLEN_SYSTEM_BROAD] = {0};
			unsigned int length;
			if(!stream.Read(content, sizeof(content), length))
			{
				////TRACE_INFO((Error, "read message error");
				return -1;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if( NULL != pRb )
				pRb->RecvSystemBroad( 2, content, ( int )length );
		}
		break;
	case CMD_GET_SYSTME_SPEAKER_C2L2C:
		{
			char content[MAXLEN_SYSTEM_MSG] = {0};
			unsigned int length;
			ResponseBase *pRb=g_NetApp.GetResponseHandler();

			char bspeak = 0;
			if(!stream.Read(bspeak))
			{
				////TRACE_INFO((Error, "read bspeak error");
				return -1;
			}

			if( 0 != bspeak )
			{
				memset( content, 0, sizeof( content ));
				if(!stream.Read(content, sizeof(content), length))
				{
					////TRACE_INFO((Error, "read message error");
					return -1;
				}

				if( NULL != pRb )
					pRb->RecvSystemBroad( 1, content, ( int )length );
			}
			else
			{
				if( NULL != pRb )
					pRb->RecvSystemBroad( 0, NULL, 0 );
			}

			bspeak = 0;
			if(!stream.Read(bspeak))
			{
				////TRACE_INFO((Error, "read bspeak error");
				return -1;
			}

			if( 0 != bspeak )
			{
				memset( content, 0, sizeof( content ));
				if(!stream.Read(content, sizeof(content), length))
				{
					////TRACE_INFO((Error, "read message error");
					return -1;
				}

				int interval_minuter = 0;
				if(!stream.Read( interval_minuter ))
				{
					////TRACE_INFO((Error, "read interval_minuter error");
					return -1;
				}

				if( NULL != pRb )
					pRb->RecvSystemNotice( content, ( int )length, true, interval_minuter );
			}
		}
		break;
	case CMD_PRESENTBROADCAST_L2C:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb==NULL)
			{
				////TRACE_INFO((Error, "ResponseBase is NULL");
				return -1;
			}

			ResultData_GiftInfo giftInfo;
			if (!stream.Read(giftInfo.from_idx))//发送者idx
			{
				////TRACE_INFO((Error, "read from_idx error");
				return -1;
			}
			if (!stream.Read(giftInfo.to_id))//接收者idx
			{
				////TRACE_INFO((Error, "read to_id error");
				return -1;
			}
			if (!stream.Read(giftInfo.number))//礼物个数
			{
				////TRACE_INFO((Error, "read number error");
				return -1;
			}
			if (!stream.Read(giftInfo.pid))//礼物ID
			{
				////TRACE_INFO((Error, "read pid error");
				return -1;
			}
			if (!stream.Read(giftInfo.type)) //是否为烟花，0-非烟花，1-烟花
			{
				////TRACE_INFO((Error, "read type error");
				return -1;
			}
			if (!stream.Read(giftInfo.time))
			{
				////TRACE_INFO((Error, "read time error");
				return -1;
			}
			if (!stream.Read(giftInfo.runway))
			{
				////TRACE_INFO((Error, "read runway error");
				return -1;
			}
			if (!stream.Read(giftInfo.roomid))
			{
				////TRACE_INFO((Error, "read roomid error");
				return -1;
			}
			unsigned int roomnamelen;
			if (!stream.Read(giftInfo.roomname, sizeof(giftInfo.roomname), roomnamelen))
			{
				////TRACE_INFO((Error, "read roomname error");
				return -1;
			}
			giftInfo.roomname[roomnamelen] = 0;

			////TRACE_INFO((Debug, "from_idx=%d, to_id=%d, number=%d, pid=%d, type=%d, runway=%d, roomid=%d, roomname=%s", 
				//giftInfo.from_idx, giftInfo.to_id, giftInfo.number, giftInfo.pid, giftInfo.type, giftInfo.runway, giftInfo.roomid, giftInfo.roomname);
			if (giftInfo.runway == 1)
			{
				pRb->RecvPresentBroad(0, NULL, &giftInfo);
			}
			else if (giftInfo.runway == 2)
			{
				pRb->RecvPresentBroad(0, &giftInfo, NULL);
			}
		}
		break;
	case CMD_GET_GIFTINFO_C2L2C:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb==NULL)
			{
				////TRACE_INFO((Error, "ResponseBase is NULL");
				return -1;
			}
			// 贵族礼物信息
			ResultData_GiftInfo guestGiftInfo;
			if (!stream.Read(guestGiftInfo.from_idx))//发送者idx
			{
				////TRACE_INFO((Error, "read from_idx error");
				return -1;
			}
			if (!stream.Read(guestGiftInfo.to_id))//接收者idx
			{
				////TRACE_INFO((Error, "read to_id error");
				return -1;
			}
			if (!stream.Read(guestGiftInfo.number))//礼物个数
			{
				////TRACE_INFO((Error, "read number error");
				return -1;
			}
			if (!stream.Read(guestGiftInfo.pid))//礼物ID
			{
				////TRACE_INFO((Error, "read pid error");
				return -1;
			}
			if( ! stream.Read( guestGiftInfo.type )) //是否为烟花，0-非烟花，1-烟花
			{
				////TRACE_INFO((Error, "read type error");
				return -1;
			}
			if( ! stream.Read( guestGiftInfo.time ))
			{
				////TRACE_INFO((Error, "read time error");
				return -1;
			}
			if (!stream.Read(guestGiftInfo.roomid))
			{
				////TRACE_INFO((Error, "read roomid error");
				return -1;
			}
			unsigned int roomnamelen;
			if (!stream.Read(guestGiftInfo.roomname, sizeof(guestGiftInfo.roomname), roomnamelen))
			{
				////TRACE_INFO((Error, "read roomname error");
				return -1;
			}
			guestGiftInfo.roomname[roomnamelen] = 0;

			guestGiftInfo.runway = 2;
			////TRACE_INFO((Debug, "from_idx=%d, to_id=%d, number=%d, pid=%d, type=%d, runway=%d, roomid=%d, roomname=%s", 
				//guestGiftInfo.from_idx, guestGiftInfo.to_id, guestGiftInfo.number, guestGiftInfo.pid, guestGiftInfo.type, guestGiftInfo.runway, guestGiftInfo.roomid, guestGiftInfo.roomname);

			// 普通礼物信息
			ResultData_GiftInfo commGiftInfo;
			if (!stream.Read(commGiftInfo.from_idx))//发送者idx
			{
				////TRACE_INFO((Error, "read from_idx error");
				return -1;
			}
			if (!stream.Read(commGiftInfo.to_id))//接收者idx
			{
				////TRACE_INFO((Error, "read to_id error");
				return -1;
			}
			if (!stream.Read(commGiftInfo.number))//礼物个数
			{
				////TRACE_INFO((Error, "read number error");
				return -1;
			}
			if (!stream.Read(commGiftInfo.pid))//礼物ID
			{
				////TRACE_INFO((Error, "read pid error");
				return -1;
			}
			if (!stream.Read( commGiftInfo.type )) //是否为烟花，0-非烟花，1-烟花
			{
				////TRACE_INFO((Error, "read type error");
				return -1;
			}
			if (!stream.Read( commGiftInfo.time ))
			{
				////TRACE_INFO((Error, "read time error");
				return -1;
			}
			if (!stream.Read(commGiftInfo.roomid))
			{
				////TRACE_INFO((Error, "read roomid error");
				return -1;
			}
			if (!stream.Read(commGiftInfo.roomname, sizeof(commGiftInfo.roomname), roomnamelen))
			{
				////TRACE_INFO((Error, "read roomname error");
				return -1;
			}
			commGiftInfo.roomname[roomnamelen] = 0;
			commGiftInfo.runway = 1;

			////TRACE_INFO((Debug, "from_idx=%d, to_id=%d, number=%d, pid=%d, type=%d, runway=%d, roomid=%d, roomname=%s", 
				//commGiftInfo.from_idx, commGiftInfo.to_id, commGiftInfo.number, commGiftInfo.pid, commGiftInfo.type, commGiftInfo.runway, commGiftInfo.roomid, commGiftInfo.roomname);
			pRb->RecvPresentBroad(0, &guestGiftInfo, &commGiftInfo);
		}
		break;
	case CMD_SYSTEMSPEAKER_L2C:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb==NULL)
			{
				////TRACE_INFO((Error, "ResponseBase is NULL");
				return -1;
			}
			int from_idx;
			if (!stream.Read(from_idx))
			{
				////TRACE_INFO((Error, "read from_idx is NULL");
				pRb->RecvSpeakerFromAll(-1,SPEAKER_SYSTEM,0,"",0);
				return -1;
			}
			char content[MSG_LENGTH]={0};
			unsigned int contentlen;
			if(!stream.Read(content, sizeof(content), contentlen))
			{
				////TRACE_INFO((Error, "read message error");
				pRb->RecvSpeakerFromAll(-1,SPEAKER_SYSTEM,0,"",0);
				return -1;
			}
			pRb->RecvSpeakerFromAll(0,SPEAKER_SYSTEM,from_idx,content,contentlen);
		}
		break;
		//add by jinguanfu 2009/12/18 <end>
		//add by jinguanfu 2010/1/4 取得好友位置 <begin>
	case CMD_GETFRIENDLOCATION_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);

			ReserveData_AddFriend *pRDFL=(ReserveData_AddFriend*)*pRD;

			int friend_idx;
			if (!stream.Read(friend_idx))
			{
				////TRACE_INFO((Error, "read friend_idx is NULL");
				return -1;
			}
			int lobbyid;
			if (!stream.Read(lobbyid))
			{
				////TRACE_INFO((Error, "read lobbyid is NULL");
				return -1;
			}
			int roomid;
			if (!stream.Read(roomid))
			{
				////TRACE_INFO((Error, "read roomid is NULL");
				return -1;
			}
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
				pRb->RecvFriendLocation(0,0,friend_idx,lobbyid,roomid,pRDFL->cseq);

		}
		break;
	case CMD_GETPARTINFO_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb==NULL)
			{
				////TRACE_INFO((Error, "ResponseBase is NULL");
				return -1;
			}

			int ret=-1;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				return -1;
			}

			int from_idx;
			if (!stream.Read(from_idx))
			{
				////TRACE_INFO((Error, "read from_idx is NULL");
				return -1;
			}

			ResultData_PartInfo part_info = {0};
			if (!stream.Read(part_info.idx))
			{
				////TRACE_INFO((Error, "read idx is NULL");
				return -1;
			}
			if (!stream.Read(part_info.sex))
			{
				////TRACE_INFO((Error, "read sex is NULL");
				return -1;
			}
			if (!stream.Read(part_info.age))
			{
				////TRACE_INFO((Error, "read age is NULL");
				return -1;
			}
			unsigned int plen;
			if (!stream.Read(part_info.province,sizeof(part_info.province),plen))
			{
				////TRACE_INFO((Error, "read province is NULL");
				return -1;
			}
			unsigned int clen;
			if (!stream.Read(part_info.city,sizeof(part_info.city),clen))
			{
				////TRACE_INFO((Error, "read city is NULL");
				return -1;
			}
			unsigned int nlen;
			if (!stream.Read(part_info.nick,sizeof(part_info.nick),nlen))
			{
				////TRACE_INFO((Error, "read nick is NULL");
				return -1;
			}
			if (!stream.Read(part_info.singlevel))
			{
				////TRACE_INFO((Error, "read singlevel is NULL");
				return -1;
			}
			//取得自身角色信息
			if (from_idx==part_info.idx)
			{
				if (!stream.Read(part_info.gold))
				{
					////TRACE_INFO((Error, "read gold is NULL");
					return -1;
				}
				if (!stream.Read(part_info.silver))
				{
					////TRACE_INFO((Error, "read silver is NULL");
					return -1;
				}
				if (!stream.Read(part_info.nexp))
				{
					////TRACE_INFO((Error, "read silver is NULL");
					return -1;
				}
                //保存认证信息
                g_DM.saveMyPartInfo(& part_info);
				pRb->RecvPartInfoBySelf( 0, ret, & part_info, (*pRD)->cseq );
			}
			else
			{
                g_DM.savePartInfo(& part_info);
				pRb->RecvPartInfo( 0, ret, & part_info, (*pRD)->cseq );
			}
		}
		break;
	case CMD_GETROOMLISTBYLOBBY_C2L2C:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb==NULL)
			{
				////TRACE_INFO((Error, "ResponseBase is NULL");
				return -1;
			}
			int ret=-1;
			if (!stream.Read(ret))
			{
				////TRACE_INFO((Error, "read ret is NULL");
				pRb->RecvRoomList(ret,NULL);
				return -1;
			}
			ResultDate_RoomList roomlist;
			memset(&roomlist,0,sizeof(ResultDate_RoomList));

			if (!stream.Read(roomlist.lobbyid))
			{
				////TRACE_INFO((Error, "read lobbyid is NULL");
				pRb->RecvRoomList(-1,NULL);
				return -1;
			}
			if (!stream.Read(roomlist.count))
			{
				////TRACE_INFO((Error, "read count is NULL");
				pRb->RecvRoomList(-1,NULL);
				return -1;
			}
			roomlist.pRL=(RoomList*)malloc(sizeof(RoomList)*roomlist.count);
			if (roomlist.pRL==NULL)
			{
				////TRACE_INFO((Error, "memory error");
				return -1;
			}
			//roomlist.count=count;
			for (int i=0;i<roomlist.count;i++)
			{
				roomlist.pRL[i].roomid=0;
				if (!stream.Read(roomlist.pRL[i].roomid))
				{
					////TRACE_INFO((Error, "read roomid is NULL");
					pRb->RecvRoomList(-1,NULL);
					return -1;
				}
				memset(roomlist.pRL[i].roomname,0,50);
				unsigned int rlen;
				if (!stream.Read(roomlist.pRL[i].roomname,
					sizeof(roomlist.pRL[i].roomname),rlen))
				{
					////TRACE_INFO((Error, "read roomname is NULL");
					pRb->RecvRoomList(-1,NULL);
					return -1;
				}
				//roomlist.pRL[i].roomid=roomid;
				//strcpy(roomlist.pRL[i].roomname,roomname);
			}
			pRb->RecvRoomList(0,&roomlist);
			free(roomlist.pRL);
		}
		break;
		//add by jinguanfu 2010/1/4 取得好友位置 <end>
		//add by jinguanfu 2010/1/20 <begin>
	case CMD_REQUESTADDFRIEND_L2C2L:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb==NULL)
			{
				////TRACE_INFO((Error, "ResponseBase is NULL");
				return -1;
			}
			int idx;	//添加好友者idx
			if(!stream.Read(idx))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}
			pRb->RecvAddFriendApply(idx);
		}
		break;
		//add by jinguanfu 2010/1/20 <end>
		//add by jinguanfu 2010/1/27
		//case CMD_DELETEAVARTA_C2L2C:
		//	{
		//		ResponseBase *pRb=g_NetApp.GetResponseHandler();
		//		if (pRb==NULL)
		//		{
		//			////TRACE_INFO((Error, "ResponseBase is NULL");
		//			return -1;
		//		}

		//		int ret;
		//		if (!stream.Read(ret))
		//		{	
		//			////TRACE_INFO((Error, "read ret is error");
		//			return -1;
		//		}
		//		
		//		//pRb->RecvDeleteAvarta(ret);
		//	}
	case CMD_LOGIN_AGAIN_L2C:
		{
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				//pRb->AddLog( "kick out game for login again!" );
				pRb->RecvLobbyKickUser();
			}
		}
		break;
	case CMD_GETROOMSIMPLEINFOS_C2L2C:
		{
			//TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			//ReserveData **pRD = pSeqMap->Get(seq);
			//if(!pRD)
			//{
			//	return -1;
			//}
			//RDGuard guard(*pRD,seq);

			ResultData_RoomSimpleInfoList roomlist;

			roomlist.roomcount=0;
			if (!stream.Read(roomlist.roomcount))
			{	
				////TRACE_INFO((Error, "read roomcount is error");
				return -1;
			}
			ResultData_RoomSimpleInfo* pRS = new ResultData_RoomSimpleInfo[roomlist.roomcount];

			for(int i=0;i<roomlist.roomcount;i++)
			{
				int roomid=0;
				if(!stream.Read(roomid))
				{
					////TRACE_INFO((Error, "read roomid is error");
					delete[] pRS;
					return -1;
				}
				char roomname[50]={0};
				unsigned int rlen;
				if(!stream.Read(roomname,sizeof(roomname),rlen))
				{
					////TRACE_INFO((Error, "read roomname is error");
					delete[] pRS;
					return -1;
				}
				char pwdflag;
				if(!stream.Read(pwdflag))
				{
					////TRACE_INFO((Error, "read pwdflag is error");
					delete[] pRS;
					return -1;
				}
				short clientnum;
				if(!stream.Read(clientnum))
				{
					////TRACE_INFO((Error, "read clientnum is error");
					delete[] pRS;
					return -1;
				}
				short boy = 0;
				if(!stream.Read(boy))
				{
					////TRACE_INFO((Error, "read boy is error");
					delete[] pRS;
					return -1;
				}
				short girl = 0;
				if(!stream.Read(girl))
				{
					////TRACE_INFO((Error, "read boy is error");
					delete[] pRS;
					return -1;
				}
				int isvip;
				if(!stream.Read(isvip))
				{
					////TRACE_INFO((Error, "read isvip is error");
					delete[] pRS;
					return -1;
				}
				int isclose;
				if(!stream.Read(isclose))
				{
					////TRACE_INFO((Error, "read isclose is error");
					delete[] pRS;
					return -1;
				}
				int state;
				if(!stream.Read(state))
				{
					////TRACE_INFO((Error, "read isclose is error");
					delete[] pRS;
					return -1;
				}
				int maxplayer;
				if(!stream.Read(maxplayer))
				{
					////TRACE_INFO((Error, "read isclose is error");
					delete[] pRS;
					return -1;
				}
				short re_player_num;
				if(!stream.Read(re_player_num))
				{
					////TRACE_INFO((Error, "read isclose is error");
					delete[] pRS;
					return -1;
				}

				pRS[i].roomid		= roomid;
				strcpy( pRS[i].roomname, roomname );
				pRS[i].pwdflag		= pwdflag;
				pRS[i].clientnum	= clientnum;
				pRS[i].isvip		= isvip;
				pRS[i].isclose		= isclose;
				pRS[i].state		= state;
				pRS[i].boy			= boy;
				pRS[i].girl			= girl;
				pRS[i].maxplayer	= maxplayer;
				pRS[i].re_player_num= re_player_num;
			}
			roomlist.RoomSimpleInfoList = pRS;
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
				pRb->RecvRoomSimpleInfo(0,LOBBY_NORMAL,&roomlist);
			delete []pRS;
		}
		break;
	case CMD_GETPRACTICEROOMSimple_C2L2C:
		{
			//TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			//ReserveData **pRD = pSeqMap->Get(seq);
			//if(!pRD)
			//{
			//	return -1;
			//}
			//RDGuard guard(*pRD,seq);

			ResultData_RoomSimpleInfoList roomlist;

			roomlist.roomcount=0;
			if (!stream.Read(roomlist.roomcount))
			{	
				////TRACE_INFO((Error, "read roomcount is error");
				return -1;
			}

			roomlist.RoomSimpleInfoList = new ResultData_RoomSimpleInfo[roomlist.roomcount];
			for(int i=0;i<roomlist.roomcount;i++)
			{
				if(!stream.Read(roomlist.RoomSimpleInfoList[i].roomid))
				{
					////TRACE_INFO((Error, "read room id is error");
					delete[] roomlist.RoomSimpleInfoList;
					return -1;
				}

				char all = 0;
				if(!stream.Read(all))
				{
					////TRACE_INFO((Error, "read room id is error");
					delete[] roomlist.RoomSimpleInfoList;
					return -1;
				}
				char boy = 0;
				if(!stream.Read(boy))
				{
					////TRACE_INFO((Error, "read boy is error");
					delete[] roomlist.RoomSimpleInfoList;
					return -1;
				}
				char girl = 0;
				if(!stream.Read(girl))
				{
					////TRACE_INFO((Error, "read boy is error");
					delete[] roomlist.RoomSimpleInfoList;
					return -1;
				}

				roomlist.RoomSimpleInfoList[i].isclose		= 0;
				roomlist.RoomSimpleInfoList[i].isvip		= 0;
				roomlist.RoomSimpleInfoList[i].pwdflag		= 0;
				memset( roomlist.RoomSimpleInfoList[i].roomname, 0 , sizeof( roomlist.RoomSimpleInfoList[i].roomname ));
				roomlist.RoomSimpleInfoList[i].clientnum	= all;
				roomlist.RoomSimpleInfoList[i].boy			= boy;
				roomlist.RoomSimpleInfoList[i].girl			= girl;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
				pRb->RecvRoomSimpleInfoEx(0,LOBBY_NORMAL,&roomlist,ROOMTYPE_PRACTICEG);
			delete[] roomlist.RoomSimpleInfoList;
		}
		break;
	case CMD_DENY_APPLYCLUB_L2C:
		{
			int clubid;
			if(!stream.Read(clubid))
				return -1;

			char clubname[50];
			unsigned int len=0;
			if(!stream.Read(clubname,sizeof(clubname),len))
				return -1;

			int bopidx;
			if(!stream.Read(bopidx))
				return -1;

			int opidx;
			if(!stream.Read(opidx))
				return -1;

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
				pRb->RecvDenyClubNoticeByLobby(0,clubid,clubname,opidx);
		}
		break;
	case CMD_CLUBMSG_L2C:
		{
			int clubid;
			if(!stream.Read(clubid))
				return -1;

			char clubname[50]={0};
			unsigned int len=0;
			if(!stream.Read(clubname,sizeof(clubname),len))
				return -1;

			int opidx;				//操作者idx
			if(!stream.Read(opidx))
				return -1;

			int bopidx;				//被操作者idx
			if(!stream.Read(bopidx))
				return -1;

			char type;
			if(!stream.Read(type))
				return -1;

			switch(type)
			{
			case CLUB_MSG_APPLY:
				{
					ResponseBase *pRb=g_NetApp.GetResponseHandler();
					if (pRb)
						pRb->RecvAddClubNoticeByLobby(0,clubid,clubname,opidx,bopidx);
				}
				break;
			case CLUB_MSG_EXIT:
				{
					ResponseBase *pRb=g_NetApp.GetResponseHandler();
					if (pRb)
						pRb->RecvExitClubNoticeByLobby(0,clubid,clubname,bopidx);
				}
				break;
			case CLUB_MSG_DEL:
				{
					ResponseBase *pRb=g_NetApp.GetResponseHandler();
					if (pRb)
						pRb->RecvKickClubNoticeByLobby(0,clubid,clubname,opidx,bopidx);
				}
				break;
			}
		}
		break;
	case CMD_MALLINFO_L2C:
		{
			char context[65535]={0};
			unsigned int length=0;
			if(!stream.Read(context,sizeof(context),length))
				return -1;

			if(length==0)
				return -1;

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
				pRb->RecvMailInfo(0,context,length);
		}
		break;
	case CMD_CHANGEPOWER_C2L2C:
		{
			int optionidx;
			if(!stream.Read(optionidx))
			{
				return -1;
			}
			char identity;
			if(!stream.Read(identity))
			{
				return -1;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			pRb->RecvChangePower(optionidx,identity);
		}
		break;
		//add by cx 2010-7-6
	case CMD_BUYGOODS_C2L2C:
		{
			//TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			//ReserveData **pRD = pSeqMap->Get(seq);
			//if(!pRD)
			//{
			//	return -1;
			//}
			//RDGuard guard(*pRD,seq);

			int ret = 0;
			if(!stream.Read( ret ))
			{
				////TRACE_INFO((Error, "read buy data ret is error");
				return -1;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if( NULL == pRb )
			{
				////TRACE_INFO((Error, "get buy data ResponseBase is error");
				return -1;
			}

			switch( ret )
			{
			case RET_OK:
				{
					int gold = 0;
					if(!stream.Read( gold ))
					{
						////TRACE_INFO((Error, "read buy data gold is error");
						return -1;
					}

					int silver = 0;
					if(!stream.Read( silver ))
					{
						////TRACE_INFO((Error, "read buy data silver is error");
						return -1;
					}

					char buy_info[16000] = {0};
					unsigned int len=0;
					if(!stream.Read( buy_info, sizeof(buy_info), len ))
						return -1;
					short goods_id[1000] = {0};
					int globl_id[1000] = {0};
					int count = _ParseGlobalInfo( buy_info, goods_id, globl_id, 1000 );
					if( 0 == count )
						return -1;

					char buy_day[ MAXLEN_TIMESTR ] = {0};
					if(!stream.Read( buy_day, sizeof(buy_day), len ))
					{
						////TRACE_INFO((Error, "read buy data buy day is error");
						return -1;
					}

					pRb->RecvBuyPack( 0, 0, gold, silver, goods_id, globl_id, count, buy_day );
				}
				break;

			case -1://金银币不足
				pRb->RecvBuyPack( -1, LOBBY_BUY_NOMONEY, 0, 0, NULL, NULL, 0, NULL );
				break;
			case -2://性别不符
				pRb->RecvBuyPack( -1, LOBBY_BUY_SEXERROR, 0, 0, NULL, NULL, 0, NULL );
				break;
			case -3://已有终身卡 购买vip卡时
				pRb->RecvBuyPack( -1, LOBBY_BUY_ISVIP, 0, 0, NULL, NULL, 0, NULL );
				break;
			case -4://背包空间不足
				pRb->RecvBuyPack( -1, LOBBY_BUY_NOSPACE, 0, 0, NULL, NULL, 0, NULL );
				break;
			case -5://其他
			default:
				pRb->RecvBuyPack( -1, LOBBY_BUY_ERROR, 0, 0, NULL, NULL, 0, NULL );
				break;
			}
		}
		break;
	case CMD_SENDGOODS_C2L2C:
		{
			int ret = 0;
			if(!stream.Read( ret ))
			{
				////TRACE_INFO((Error, "read buy data ret is error");
				return -1;
			}

			int gold = 0;
			if(!stream.Read( gold ))
			{
				////TRACE_INFO((Error, "read buy data gold is error");
				return -1;
			}

			int silver = 0;
			if(!stream.Read( silver ))
			{
				////TRACE_INFO((Error, "read buy data silver is error");
				return -1;
			}

			int idx = 0;
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read buy idx is error");
				return -1;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if( NULL == pRb )
			{
				////TRACE_INFO((Error, "get buy data ResponseBase is error");
				return -1;
			}

			switch( ret )
			{
			case RET_OK:
				pRb->RecvBuyForSend( 0, 0, idx, gold, silver );
				break;

			case 1://金银币不足
				pRb->RecvBuyForSend( -1, LOBBY_BUY_NOMONEY, idx, gold, silver );
				break;
			case 2://性别不符
				pRb->RecvBuyForSend( -1, LOBBY_BUY_SEXERROR, idx, gold, silver );
				break;
			case 3://已有终身卡 购买vip卡时
				pRb->RecvBuyForSend( -1, LOBBY_BUY_ISVIP, idx, gold, silver );
				break;
			case 4://背包空间不足
				pRb->RecvBuyForSend( -1, LOBBY_BUY_NOSPACE, idx, gold, silver );
				break;
			case 5://对方已拥有该永久商品
				pRb->RecvBuyForSend( -1, LOBBY_BUY_ISFULL, idx, gold, silver );
				break;
			default://其他
				pRb->RecvBuyForSend( -1, LOBBY_BUY_ERROR, idx, gold, silver );
				break;
			}
		}
		break;
	case CMD_DELPACKINFO_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);

			int ret = 0;
			if(!stream.Read( ret ))
			{
				////TRACE_INFO((Error, "read del data ret is error");
				return -1;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if(ret == RET_OK)
			{
				if (pRb)
				{
					pRb->RecvDelPack( 0, 0, (*pRD)->cseq );
				}
			}
			else
			{
				if (pRb)
				{
					pRb->RecvDelPack( -1, LOBBY_DELPACK_ERROR, (*pRD)->cseq );
				}
			}
		}
		break;
	case CMD_USEGOODS_C2L2C:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveData **pRD = pSeqMap->Get(seq);
			if(!pRD)
			{
				return -1;
			}
			RDGuard guard(*pRD,seq);

			int ret = 0;
			if(!stream.Read( ret ))
			{
				////TRACE_INFO((Error, "read del data ret is error");
				return -1;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if(ret == RET_OK)
			{
				if (pRb)
				{
					pRb->RecvUsePack( 0, 0, (*pRD)->cseq );
				}
			}
			else
			{
				if (pRb)
				{
					pRb->RecvUsePack( -1, LOBBY_USEPACK_ERROR, (*pRD)->cseq );
				}
			}
		}
		break;
	case CMD_GETPACKINFO_C2L2C:
		{
			//TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			//ReserveData **pRD = pSeqMap->Get(seq);
			//if(!pRD)
			//{
			//	return -1;
			//}
			//RDGuard guard(*pRD,seq);

			ResponseBase *pRb=g_NetApp.GetResponseHandler();

			if( true )
			{
				unsigned int len;
				ResultData_Pack pack[100];

				int all_count = 0;
				if(!stream.Read( all_count ))
				{
					////TRACE_INFO((Error, "read pack data count is error");
					return -1;
				}
				////TRACE_INFO(( Debug, "cmd:CMD_GETPACKINFO_C2L2C all_count:%d", all_count );

				int send_count = 0;
				if(!stream.Read( send_count ))
				{
					////TRACE_INFO((Error, "read pack data count is error");
					return -1;
				}
				////TRACE_INFO(( Debug, "cmd:CMD_GETPACKINFO_C2L2C send_count:%d", send_count );

				for( int i=0; i<send_count; ++i )
				{
					if(!stream.Read( pack[i].global_id ))
					{
						////TRACE_INFO((Error, "read pack data globalid is error");
						return -1;
					}
					////TRACE_INFO(( Debug, "cmd:CMD_GETPACKINFO_C2L2C global_id:%d", pack[i].global_id );

					if(!stream.Read( pack[i].goods_id ))
					{
						////TRACE_INFO((Error, "read pack data goods_id is error");
						return -1;
					}
					////TRACE_INFO(( Debug, "cmd:CMD_GETPACKINFO_C2L2C goods_id:%d", pack[i].goods_id );

					if(!stream.Read( pack[i].status ))
					{
						////TRACE_INFO((Error, "read pack data status is error");
						return -1;
					}

					if(!stream.Read( pack[i].day_num ))
					{
						////TRACE_INFO((Error, "read pack data day_num is error");
						return -1;
					}

					if(!stream.Read( pack[i].buy_day,sizeof( pack[i].buy_day ), len))
					{
						////TRACE_INFO((Error, "read pack data buy day is error");
						return -1;
					}

					if(!stream.Read( pack[i].is_use ))
					{
						////TRACE_INFO((Error, "read pack data is_use is error");
						return -1;
					}

					pack[i].count = 0;
					if(!stream.Read( pack[i].count ))
					{
						////TRACE_INFO((Error, "read pack count is error");
						return -1;
					}
				}
				if (pRb)
				{
					pRb->RecvGetPack( 0, 0, pack, send_count, all_count );
				}
			}
			else
			{
				if (pRb)
				{
					pRb->RecvGetPack( -1, LOBBY_GETPACK_ERROR, NULL, 0, 0 );
				}
			}
		}
		break;
	case CMD_GOODS_DATED_L2C:
		{
			int count = 0;
			if(!stream.Read( count ))
			{
				////TRACE_INFO((Error, "read pack timeout count is error");
				return -1;
			}

			int global_id[100] = {0};
			for( int i=0; i<count; ++i )
			{
				if(!stream.Read( global_id[i] ))
				{
					////TRACE_INFO((Error, "read pack timeout id is error");
					return -1;
				}
			}

			//stream
			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvTimeoutPack( count, global_id );
			}
		}
		break;
	case CMD_SENDTOME_L2C:
		{
			int player_id = 0;
			if(!stream.Read( player_id ))
			{
				////TRACE_INFO((Error, "read pack send player id is error");
				return -1;
			}

			short goods_id = 0;
			if(!stream.Read( goods_id ))
			{
				////TRACE_INFO((Error, "read pack send goods_id is error");
				return -1;
			}

			int global_id = 0;
			if(!stream.Read( global_id ))
			{
				////TRACE_INFO((Error, "read pack send global_id is error");
				return -1;
			}

			unsigned int len=0;
			char buy_day[ MAXLEN_TIMESTR ] = {0};
			if(!stream.Read( buy_day, sizeof(buy_day), len ))
			{
				////TRACE_INFO((Error, "read buy data buy day is error");
				return -1;
			}

			char buy_msg[ MAXLEN_BUY_MSG ] = {0};
			if( ! stream.Read( buy_msg, sizeof( buy_msg ), len ) )
			{
				////TRACE_INFO((Error, "read buy data buy msg is error");
				return -1;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvSendPack( player_id, global_id, goods_id, buy_day, buy_msg );
			}
		}
		break;
	case CMD_CREATE_PRACTICEROOM_C2L2C:
	case CMD_GETPRACTICEROOMINFO_C2L2C:
		{
			typedef void ( ResponseBase::* PFNC)( int ret, int errcode, RoomListEx * info );

			PFNC pFnc = NULL;
			if( CMD_CREATE_PRACTICEROOM_C2L2C == cmd )
			{
				pFnc = & ResponseBase::RecvCreateRoom;
			}
			else
			{
				pFnc = & ResponseBase::RecvTmpRoomInfo;
			}

			//////////////////////////////////////////////////////////////////////////

			//TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			//ReserveData **pRD = pSeqMap->Get(seq);
			//if(!pRD)
			//{
			//	return -1;
			//}
			//RDGuard guard(*pRD,seq);

			int ret = 0;
			if(!stream.Read( ret ))
			{
				////TRACE_INFO((Error, "read create room ret is error");
				return -1;
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				if( RET_OK == ret )
				{
					RoomListEx info;
					if( 0 != _ReadTmpRoomData( stream, & info ))
					{
						return -1;
					}
					(pRb->* pFnc)( 0, 0, & info );
				}
				else
				{
					(pRb->* pFnc)( -1, ROOM_TMP_CREATE_ERROR, NULL );
				}
			}
		}
		break;
	case CMD_CREATE_PRACTICEROOM_L2C:
		{
			RoomListEx info;
			if( 0 != _ReadTmpRoomData( stream, & info ))
			{
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvRoomNotice4Add( & info );
			}
		}
		break;
	case CMD_DELETE_PRACTICEROOM_L2C:
		{
			int roomid; //房间id
			if(!stream.Read( roomid ))
			{
				////TRACE_INFO((Error, "read del room id is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvRoomNotice4Del( roomid );
			}
		}
		break;
	case CMD_EXPLEVELUP_L2C:
		{
			int idx; //用户id
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read level up idx is error");
				return -1;
			}

			char level;// 等级
			if(!stream.Read( level ))
			{
				////TRACE_INFO((Error, "read level up level is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvNotice4LevelUp( idx, level );
			}
		}
		break;
	case CMD_VIPLEVEL_L2C:
		{
			int idx = 0;
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read vip level idx is error");
				return -1;
			}

			int vip_level = 0;
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

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvVipChange( idx, vip_level, is_vip );
			}
		}
		break;
	case CMD_EXPCHG_L2C:
		{
			int nexp = 0;
			if(!stream.Read( nexp ))
			{
				////TRACE_INFO((Error, "read level nexp is error");
				return -1;
			}

			int level = 0;
			if(!stream.Read( level ))
			{
				////TRACE_INFO((Error, "read level level is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvExpChange( nexp, level );
			}
		}
		break;
	case CMD_CREATE_ROOM_L2C:
		{
			int room_idx = 0;
			if(!stream.Read( room_idx ))
			{
				////TRACE_INFO((Error, "read room_idx is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvRoomNotice4Add( room_idx );
			}
		}
		break;
	case CMD_DELETE_ROOM_L2C:
		{
			int room_idx = 0;
			if(!stream.Read( room_idx ))
			{
				////TRACE_INFO((Error, "read room_idx is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvRoomNotice4Del( room_idx );
			}
		}
		break;
	case CMD_WEBCHGUSERINFO_L2C:
		{
			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvUpdataMyInfo();
			}
		}
		break;
	case CMD_GIFT_TOPLIST_C2L2C:
		{
			ResultData_GiftList gift_list = {0};

			unsigned int len=0;
			if(!stream.Read( gift_list.curtime, sizeof( gift_list.curtime ), len ))
			{
				////TRACE_INFO((Error, "read room_idx is error");
				return -1;
			}

			if(!stream.Read( gift_list.gift_count ))
			{
				////TRACE_INFO((Error, "read gift_count is error");
				return -1;
			}

			for( int i=0; i<gift_list.gift_count; ++i )
			{
				if(!stream.Read( gift_list.list[i].gift_type ))
				{
					////TRACE_INFO((Error, "read gift_type is error");
					return -1;
				}

				if(!stream.Read( gift_list.list[i].recv_idx ))
				{
					////TRACE_INFO((Error, "read idx is error");
					return -1;
				}

				if(!stream.Read( gift_list.list[i].recv_id, sizeof(gift_list.list[i].recv_id), len ))
				{
					////TRACE_INFO((Error, "read room_idx is error");
					return -1;
				}

				if(!stream.Read( gift_list.list[i].recv_num ))
				{
					////TRACE_INFO((Error, "read room_idx is error");
					return -1;
				}
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if (pRb)
			{
				pRb->RecvGiftTopList( & gift_list );
			}
		}
		break;
	case CMD_GETONLINETIME_C2L2C:
		{
			int idx=0;
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}

			int current_day_time = 0;
			if(!stream.Read( current_day_time ))
			{
				////TRACE_INFO((Error, "read current_day_time is error");
				return -1;
			}

			int current_online_time = 0;
			if(!stream.Read( current_online_time ))
			{
				////TRACE_INFO((Error, "read current_online_time is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvOnlineTime( idx, current_day_time, current_online_time );
			}
		}
		break;
	case CMD_LUCK_GIFT_L2C:
		{
			int idx = 0;
			PROTOCOL_READ(idx);
			int roomid = 0;
			PROTOCOL_READ( roomid );
			int giftid = 0;
			PROTOCOL_READ( giftid );
			int luckmultiple = 0;
			PROTOCOL_READ( luckmultiple );
			char nick_name[ MAXLEN_NICKNAME ] = {0};
			unsigned int nicknamelen=0;
			PROTOCOL_READSTR(nick_name,nicknamelen);
			short flag = 0;
			PROTOCOL_READ(flag);

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvSystemBroad2( nick_name, nicknamelen, idx, roomid, giftid, luckmultiple );
				//超过10倍奖励,增加在系统公告中显示
				if( 1 == flag )
				{
					pRb->RecvSystemNotice2( nick_name, nicknamelen, idx, roomid, giftid, luckmultiple );
				}
			}
		}
		break;
	case CMD_GET_NEWUSER_TASK_C2L2C:
		{
			short step = 0;
			if(!stream.Read( step ))
			{
				////TRACE_INFO((Error, "read step is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvGetNewUserTask( step );
			}
		}
		break;
	case CMD_TASK_STEP_C2L2C:
		{
			int ret = 0, gold = 0, silver = 0, total_gold = 0, total_silver = 0;

			if(!stream.Read( ret ))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			if( 0 == ret )
			{
				if(!stream.Read( gold ))
				{
					////TRACE_INFO((Error, "read step is error");
					return -1;
				}
				if(!stream.Read( silver ))
				{
					////TRACE_INFO((Error, "read step is error");
					return -1;
				}
				if(!stream.Read( total_gold ))
				{
					////TRACE_INFO((Error, "read step is error");
					return -1;
				}
				if(!stream.Read( total_silver ))
				{
					////TRACE_INFO((Error, "read step is error");
					return -1;
				}
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				switch( ret )
				{
				case 0:
					pRb->RecvTaskStep( 0, 0, gold, silver, total_gold, total_silver );
					break;

				case 1:
					pRb->RecvTaskStep( 0, 0, 0, 0, 0, 0 );
					break;

				case 2:
					pRb->RecvTaskStep( ret, LOBBY_TASK_STEP_ERRSTEP, gold, silver, total_gold, total_silver );
					break;

				case 3:
					pRb->RecvTaskStep( ret, LOBBY_TASK_STEP_ERRPRIZE, gold, silver, total_gold, total_silver );
					break;
				}
			}
		}
		break;
	case CMD_TASK_GIFT_C2L2C:
		{
			int ret = 0;
			int item_id = 0;
			int object_id = 0;
			char time_buf[ MAXLEN_TIMESTR ] = {0};

			if(!stream.Read( ret ))
			{
				////TRACE_INFO((Error, "read ret is error");
				return -1;
			}
			if( 0 == ret )
			{
				if(!stream.Read( item_id ))
				{
					////TRACE_INFO((Error, "read item_id is error");
					return -1;
				}
				if(!stream.Read( object_id ))
				{
					////TRACE_INFO((Error, "read object_id is error");
					return -1;
				}
				unsigned int len=0;
				if(!stream.Read( time_buf, sizeof( time_buf ), len ))
				{
					////TRACE_INFO((Error, "read time_buf is error");
					return -1;
				}
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				switch( ret )
				{
				case 0:
					pRb->RecvTaskGift( ret, 0, item_id, object_id, time_buf );
					break;
				default:
					pRb->RecvTaskGift( ret, LOBBY_TASK_STEP_ERRPRIZE, item_id, object_id, time_buf );
					break;
				}
			}
		}
		break;
	case CMD_UPDATE_ROOM_L2C:
		{
			int room_idx = 0;
			if(!stream.Read( room_idx ))
			{
				////TRACE_INFO((Error, "read item_id is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvRoomNotice4ChangeInfo( room_idx );
			}
		}
		break;
	case CMD_UPDATE_CITYGOODS_L2C:
		{
			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvNotice4ChangeGoods();
			}
		}
		break;
	case CMD_MOVE_ROOMSRV_L2C:
		{
			ResultData_RoomMoveData mr = {0};
			mr.add_room = NULL;
			mr.del_room = NULL;

			if(!stream.Read( mr.del_count ))
			{
				////TRACE_INFO((Error, "read del room count is error");
				return -1;
			}

			if( 0 < mr.del_count )
			{
				mr.del_room = new int[mr.del_count];
				for( int i=0; i<mr.del_count; ++i )
				{
					if( stream.Read( mr.del_room[i] ))
					{
						continue;
					}

					////TRACE_INFO((Error, "read del room idx is error");
					if( NULL != mr.add_room ) delete [] mr.add_room;
					if( NULL != mr.del_room ) delete [] mr.del_room;
					return -1;
				}
			}

			if(!stream.Read( mr.add_count ))
			{
				////TRACE_INFO((Error, "read add room count is error");
				return -1;
			}

			if( 0 < mr.add_count )
			{
				mr.add_room = new int[mr.add_count];
				for( int i=0; i<mr.add_count; ++i )
				{
					if( stream.Read( mr.add_room[i] ))
					{
						continue;
					}

					////TRACE_INFO((Error, "read add room idx is error");
					if( NULL != mr.add_room ) delete [] mr.add_room;
					if( NULL != mr.del_room ) delete [] mr.del_room;
					return -1;
				}
			}

			if( 0 == mr.del_count && 0 == mr.add_count )
			{
				if( NULL != mr.add_room ) delete [] mr.add_room;
				if( NULL != mr.del_room ) delete [] mr.del_room;
				break;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if( NULL == pRb)
			{
				if( NULL != mr.add_room ) delete [] mr.add_room;
				if( NULL != mr.del_room ) delete [] mr.del_room;
				return -1;
			}

			typedef void( ResponseBase::* PFNC )( int room_idx );
			PFNC pFnc = NULL;
			int * room_idx = NULL;
			int room_count = 0;
			if( mr.add_count == mr.del_count )
			{
				pFnc = & ResponseBase::RecvRoomNotice4ChangeInfo;
				room_idx = mr.add_room;
				room_count = mr.add_count;
			}
			else if( 0 < mr.add_count )
			{
				pFnc = & ResponseBase::RecvRoomNotice4Add;
				room_idx = mr.add_room;
				room_count = mr.add_count;
			}
			else if( 0 < mr.del_count )
			{
				pFnc = & ResponseBase::RecvRoomNotice4Del;
				room_idx = mr.del_room;
				room_count = mr.del_count;
			}

			for( int i=0; i<room_count; ++i )
			{
				( pRb->* pFnc )( room_idx[i] );
			}
			room_idx = NULL;
			room_count = 0;

			if( NULL != mr.add_room ) delete [] mr.add_room;
			if( NULL != mr.del_room ) delete [] mr.del_room;
		}
		break;
	case CMD_UPDATE_PACK_L2C:
		{
			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if( NULL != pRb)
			{
				pRb->RecvGetBagAgain();
			}
		}
		break;
	case CMD_GET_SEELINFO_C2L2C:
		{
			int user_idx = 0;
			if(!stream.Read( user_idx ))
			{
				////TRACE_INFO((Error, "read user idx is error");
				return -1;
			}

			int deel_idx = 0;
			if(!stream.Read( deel_idx ))
			{
				////TRACE_INFO((Error, "read gift idx is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvUserSeel( user_idx, deel_idx );
			}
		}
		break;
	case CMD_REMOVE_SEELINFO_L2C:
		{
			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvUserSeel( 0, -1 );
			}
		}
		break;
	case CMD_ENTHRALLMENT_STATE_L2C:
		{
			int idx = 0;
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}

			short state = 0;
			if(!stream.Read( state ))
			{
				////TRACE_INFO((Error, "read state is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->EnterAllMentState( idx, state );
			}
		}
		break;
	case CMD_ROOM_LOGO_L2C:
		{
			int idx = 0;
			if(!stream.Read( idx ))
			{
				////TRACE_INFO((Error, "read idx is error");
				return -1;
			}

			unsigned int len=0;
			char loge[ 128 ] = {0};
			if(!stream.Read( loge, sizeof( loge ), len ))
			{
				////TRACE_INFO((Error, "read loge is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvRoomLoge( idx, loge, ( int )len );
			}
		}
		break;
	case CMD_UPDATE_GROUPVERSION_L2C:
		{
			int versionid = 0;
			if(!stream.Read( versionid ))
			{
				////TRACE_INFO((Error, "read versionid is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvNewVersionId( versionid );
			}
		}
		break;
	case CMD_GET_GROUPINFO_C2L2C:
		{
			short group_num = 0;
			if(!stream.Read( group_num ))
			{
				////TRACE_INFO((Error, "read group_num is error");
				return -1;
			}

			ResultData_GroupInfo * pInfo = new ResultData_GroupInfo[ group_num ];
			unsigned int len = 0;
			for( int i=0; i<group_num; ++i )
			{
				//short group_id = 0;
				if(!stream.Read( pInfo[i].group_id ))
				{
					////TRACE_INFO((Error, "read group_id is error");
					delete[] pInfo;
					return -1;
				}

				//char group_name[ MAXLEN_ROOM_TYPENAME ] = {0};
				if(!stream.Read( pInfo[i].group_name, MAXLEN_ROOM_TYPENAME, len ))
				{
					////TRACE_INFO((Error, "read group_name is error");
					delete[] pInfo;
					return -1;
				}
				pInfo[i].group_name[ len ] = 0;

				//short disable = 0; //0：启用 1：禁用
				if(!stream.Read( pInfo[i].disable ))
				{
					////TRACE_INFO((Error, "read disable is error");
					delete[] pInfo;
					return -1;
				}
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvGroupInfo( group_num, pInfo );
			}

			delete[] pInfo;
		}
		break;
	case CMD_GET_ROOMLIST_IN_GROUP_C2L2C:
		{
			short group_idx = 0;
			if(!stream.Read( group_idx ))
			{
				////TRACE_INFO((Error, "read group_num is error");
				return -1;
			}

			short room_num = 0;
			if(!stream.Read( room_num ))
			{
				////TRACE_INFO((Error, "read room_num is error");
				return -1;
			}

			int * list = new int[ room_num ];
			//unsigned int len = 0;
			for( int i=0; i<room_num; ++i )
			{
				int room_idx = 0;
				if(!stream.Read( room_idx ))
				{
					////TRACE_INFO((Error, "read list is error");
					delete[] list;
					return -1;
				}

				list[i] = room_idx;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvRoomListInGroup( group_idx, room_num, list );
			}

			delete[] list;
		}
		break;
	case CMD_UPDATE_ROOMSORTINFO_C2L2C:
		{
			short group_idx = 0;
			if(!stream.Read( group_idx ))
			{
				////TRACE_INFO((Error, "read group_num is error");
				return -1;
			}

			short room_num = 0;
			if(!stream.Read( room_num ))
			{
				////TRACE_INFO((Error, "read room_num is error");
				return -1;
			}

			if( 0 < room_num )
			{
				ResultData_RoomSort * list = NULL;
				if( 0 < room_num )
				{
					list = new ResultData_RoomSort[ room_num ];
				}

				for( int i=0; i<room_num; ++i )
				{
					list[ i ].idx = 0;
					if(!stream.Read( list[ i ].idx ))
					{
						////TRACE_INFO((Error, "read idx is error");
						delete[] list;
						return -1;
					}

					list[ i ].sort = 0;
					if(!stream.Read( list[ i ].sort ))
					{
						////TRACE_INFO((Error, "read sort is error");
						delete[] list;
						return -1;
					}

					if( 10 < list[ i ].sort || 1 >  list[ i ].sort )
					{
						////TRACE_INFO((Error, "read sort is error");
						delete[] list;
						return -1;
					}
				}

				ResponseBase * pRb = g_NetApp.GetResponseHandler();
				if(pRb)
				{
					pRb->RecvRoomSortInGroup( group_idx, room_num, list );
				}

				if( NULL != list )
				{
					delete[] list;
				}
			}
		}
		break;
	case CMD_UPDATE_ROOMSORT_L2C:
		{
			short group_idx = 0;
			if(!stream.Read( group_idx ))
			{
				////TRACE_INFO((Error, "read group_num is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvRoomSortChange( group_idx );
			}
		}
		break;
	case CMD_GET_MUSICEXINFO_C2L2C:
		{
			short totalnum; //总的歌曲数量
			if(!stream.Read( totalnum ))
			{
				////TRACE_INFO((Error, "read totalnum is error");
				return -1;
			}

			short curnum;  //这个包的歌曲数量
			if(!stream.Read( curnum ))
			{
				////TRACE_INFO((Error, "read curnum is error");
				return -1;
			}

			ResultData_SongInfo * pInfo = new ResultData_SongInfo[ curnum ];

			for( int i=0; i<curnum; ++i )
			{
				if(!stream.Read( pInfo[i].m_musicid ))
				{
					////TRACE_INFO((Error, "read m_musicid is error");
					delete[] pInfo;
					return -1;
				}

				if(!stream.Read( pInfo[i].m_topscores ))
				{
					////TRACE_INFO((Error, "read m_topscores is error");
					delete[] pInfo;
					return -1;
				}

				if(!stream.Read( pInfo[i].m_topidx ))
				{
					////TRACE_INFO((Error, "read m_topidx is error");
					delete[] pInfo;
					return -1;
				}

				unsigned int len=0;
				if(!stream.Read( pInfo[i].m_topname, sizeof( pInfo[i].m_topname ), len ))
				{
					////TRACE_INFO((Error, "read m_topname is error");
					delete[] pInfo;
					return -1;
				}
				pInfo[i].m_topname[ len ] = 0;

				if(!stream.Read( pInfo[i].m_dayclicks ))
				{
					////TRACE_INFO((Error, "read m_dayclicks is error");
					delete[] pInfo;
					return -1;
				}

				if(!stream.Read( pInfo[i].m_weekclicks ))
				{
					////TRACE_INFO((Error, "read m_weekclicks is error");
					delete[] pInfo;
					return -1;
				}

				if(!stream.Read( pInfo[i].m_monthclicks ))
				{
					////TRACE_INFO((Error, "read m_monthclicks is error");
					delete[] pInfo;
					return -1;
				}

				if(!stream.Read( pInfo[i].m_totalclicks ))
				{
					////TRACE_INFO((Error, "read m_totalclicks is error");
					delete[] pInfo;
					return -1;
				}
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvSongInfoList( totalnum, curnum, pInfo );
			}
			delete[] pInfo;
		}
		break;
	case CMD_COLLECT_DATA_L2C:
		{
			unsigned int len=0;

			char server_ip[ MAXLEN_IP ] = {0};
			if(!stream.Read( server_ip, MAXLEN_IP, len ))
			{
				////TRACE_INFO((Error, "read server ip is error");
				return -1;
			}

			short server_port = 0;
			if(!stream.Read( server_port ))
			{
				////TRACE_INFO((Error, "read server_port is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvNotice4CollectData( server_ip, server_port );
			}
		}
		break;
	case CMD_GETAVSERVERINFO_C2L2C:
		{
			short net_type = 0;
			if(!stream.Read( net_type ))
			{
				////TRACE_INFO((Error, "read net type is error");
				return -1;
			}

			short totalnum;
			if(!stream.Read( totalnum ))
			{
				////TRACE_INFO((Error, "read totalnum is error");
				return -1;
			}

			ResultData_AvServer * pList = new ResultData_AvServer[ totalnum * MAXLEN_ROOM_AVIPCOUNT ];
			memset( pList, 0, sizeof( ResultData_AvServer ) * totalnum );

			unsigned int len;
			char ip_buf[ MAXLEN_ROOM_AVIPCOUNT * MAXLEN_IP ];

			short port = 0;

			char count;
			char ip_list[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];

			int server_count = 0;
			for( int i=0; i<totalnum; ++i )
			{
				if(!stream.Read( ip_buf, sizeof( ip_buf ), len ))
				{
					////TRACE_INFO((Error, "read ip is error");
					delete[] pList;
					return -1;
				}
				ip_buf[ len ] = 0;

				if(!stream.Read( port ))
				{
					////TRACE_INFO((Error, "read totalnum is error");
					delete[] pList;
					return -1;
				}

				GetAvIpFromString( ip_list, count, ip_buf, '|' );
				for( int j=0; j<count; ++j )
				{
					strcpy( pList[ server_count ].ip, ip_list[j] );
					pList[ server_count ].port = port;
					server_count += 1;
				}
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvAvServerList( pList, server_count );
			}

			delete[] pList;
		}
		break;
	case CMD_UPDATE_AVSRV_L2C:
		{
			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->UpdataAvServerList();
			}
		}
		break;
	case CMD_KICK_USER_L2C:
		{
			char result;
			if(!stream.Read( result ))
			{
				////TRACE_INFO((Error, "read result is error");
				return -1;
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				switch( result )
				{
				case 1:
					//pRb->AddLog( "kick out game by idx!" );
					pRb->RecvCheckOutGame( LOBBY_KICK_BY_IDX );
					break;

				case 2:
					//pRb->AddLog( "kick out game by net!" );
					pRb->RecvCheckOutGame( LOBBY_KICK_BY_NET );
					break;
				}
			}
		}
		break;
	case CMD_GET_OFFLINEMSG_C2L2C:
		{
			short mun = 0;
			if(!stream.Read( mun ))
			{
				////TRACE_INFO((Error, "read pack send player id is error");
				return -1;
			}

			ResultData_OffLineMsg * p = NULL;
			
			if( 0 < mun )
			{
				p = new ResultData_OffLineMsg[ mun ];
				memset( p, 0, sizeof( ResultData_OffLineMsg ) * mun );

				unsigned int len=0;
				for( int i=0; i<mun; ++i )
				{
					if(!stream.Read( p[i].idx ))
					{
						////TRACE_INFO((Error, "read pack send player id is error");
						delete[] p;
						return -1;
					}

					if(!stream.Read( p[i].buy_day, sizeof( p[i].buy_day), len ))
					{
						////TRACE_INFO((Error, "read buy data buy day is error");
						delete[] p;
						return -1;
					}

					if( ! stream.Read( p[i].buy_msg, sizeof( p[i].buy_msg ), len ) )
					{
						////TRACE_INFO((Error, "read buy data buy msg is error");
						delete[] p;
						return -1;
					}
				}
			}

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvOffLineMsg( mun, p );
			}

			if( NULL != p )
			{
				delete[] p;
			}
		}
		break;
	case CMD_GETUSERSPEAKINFO_C2L2C:
		{
			int num = 0;
            PROTOCOL_READ(num);
			
			ResultData_UserSpeakerInfo userSpkInfo ={0};
			unsigned int content_len = 0;
			for( int i = 0; i < num; ++i )
			{
                PROTOCOL_READ(userSpkInfo.time);
                PROTOCOL_READ(userSpkInfo.fromidx);
                PROTOCOL_READSTR(userSpkInfo.chat, content_len);
                PROTOCOL_READ(userSpkInfo.roomId);
                unsigned int len = 0;
                PROTOCOL_READSTR(userSpkInfo.roomName, len);

				char chatNoraml[MAXLEN_USERSPEAKER_INFO] = {0};// 小喇叭内容
				int char_len = MAXLEN_USERSPEAKER_INFO;
				int ret = CChatCompatibility::ConvertXml2Normal(chatNoraml,char_len,userSpkInfo.chat);
				if(ret == 0 || ret == -2)//如果转换成功，或者UTF转换失败
					strcpy(userSpkInfo.chat,chatNoraml);

				ResponseBase * pRb = g_NetApp.GetResponseHandler();
				if(pRb)
					pRb->RecvSpeakerFromLobby( 0, &userSpkInfo,BT_BROAD,true);
			}
		}
		break;
	case CMD_SINGERLEVEL_L2C:
		{
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "CMD_SINGERLEVEL_L2C read idx error");
				return -1;
			}
			short level;
			if (!stream.Read(level))
			{
				////TRACE_INFO((Error, "CMD_SINGERLEVEL_L2C read level error");
				return -1;
			}
			int expvalue;
			if (!stream.Read(expvalue))
			{
				////TRACE_INFO((Error, "CMD_SINGERLEVEL_L2C read expvalue error");
				return -1;
			}

			////TRACE_INFO((Debug, "idx=%d, level=%d, expvalue=%d", idx, level, expvalue);

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvSingerLevel(idx, level, expvalue);
			}
		}
		break;
	case CMD_WEALTHLEVEL_L2C:
		{
			int idx;
			if (!stream.Read(idx))
			{
				////TRACE_INFO((Error, "CMD_WEALTHLEVEL_L2C read idx error");
				return -1;
			}
			short level;
			if (!stream.Read(level))
			{
				////TRACE_INFO((Error, "CMD_WEALTHLEVEL_L2C read level error");
				return -1;
			}
			int expvalue;
			if (!stream.Read(expvalue))
			{
				////TRACE_INFO((Error, "CMD_WEALTHLEVEL_L2C read expvalue error");
				return -1;
			}

			////TRACE_INFO((Debug, "idx=%d, level=%d, expvalue=%d", idx, level, expvalue);

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if(pRb)
			{
				pRb->RecvWealthLevel(idx, level, expvalue);
			}
		}
		break;
	case CMD_FAMILYINFO_BYID_L2C:
		{
			////TRACE_INFO((Debug, "recv a CMD_FAMILYINFO_BYID_L2C");
			
			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if (pRb == NULL)
			{
				////TRACE_INFO((Error, "GetResponseHandler error");
				return -1;
			}

			short flag;
			if (!stream.Read(flag))
			{
				////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read flag error");
				return -1;
			}
			if (flag == FLAG_UPDATE)
			{
				short familyNum;
				if (!stream.Read(familyNum))
				{
					////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read familyNum error");
					return -1;
				}
				
				ResultData_Family* familys = (ResultData_Family*)malloc(sizeof(ResultData_Family)*familyNum);
				VoidGuard vg(familys);
				for (int i=0; i < familyNum; i++)
				{
					if (!stream.Read(familys[i].familyId))
					{
						////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read familyId error");
						return -1;
					}

					unsigned int familyNameLen = 0;
					if (!stream.Read(familys[i].familyName, sizeof(familys[i].familyName), familyNameLen))
					{
						////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read familyName error");
						return -1;
					}
					familys[i].familyName[familyNameLen] = 0;

					if (!stream.Read(familys[i].familyManageIdx))
					{
						////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read familyManageIdx error");
						return -1;
					}

					if (!stream.Read(familys[i].familyState))
					{
						////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read familyState error");
						return -1;
					}

					if (!stream.Read(familys[i].familySort))
					{
						////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read familySort error");
						return -1;
					}

					unsigned int familyIntroduceLen = 0;
					if (!stream.Read(familys[i].familyIntroduce, sizeof(familys[i].familyIntroduce), familyIntroduceLen))
					{
						////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read familyIntroduce error");
						return -1;
					}
					familys[i].familyIntroduce[familyIntroduceLen] = 0;

					unsigned int familyImageLen = 0;
					if (!stream.Read(familys[i].familyImage, sizeof(familys[i].familyImage), familyImageLen))
					{
						////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read familyImageLen error");
						return -1;
					}
					familys[i].familyImage[familyImageLen] = 0;

					unsigned int familyLogoLen = 0;
					if (!stream.Read(familys[i].familyLogo, sizeof(familys[i].familyLogo), familyLogoLen))
					{
						////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read familyLogo error");
						return -1;
					}
					familys[i].familyLogo[familyLogoLen] = 0;

					////TRACE_INFO((Debug, "family Id=%d, ManageIdx=%d, State=%d, Name=%s, Sort=%d, Introduce=%s, Image=%s, Logo=%s", 
						//familys[i].familyId, familys[i].familyManageIdx, familys[i].familyState, familys[i].familyName, 
						//familys[i].familySort, familys[i].familyIntroduce, familys[i].familyImage, familys[i].familyLogo);

					short roomsize;
					if (!stream.Read(roomsize))
					{
						////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read roomsize error");
						return -1;
					}
					
					familys[i].roomsize = roomsize;
					for (int j=0; j < roomsize; j++)
					{
						int roomid;
						if (!stream.Read(roomid))
						{
							////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read roomid error");
							return -1;
						}
						familys[i].roomids[j] = roomid;
						////TRACE_INFO((Debug, "familyid=%d, roomid=%d", familys[i].familyId, roomid);
					}
				}

				pRb->RecvFamilyInfo(flag, familyNum, familys);
			}
			else if (flag == FLAG_DELETE)
			{
				int familyid;
				if (!stream.Read(familyid))
				{
					////TRACE_INFO((Error, "CMD_FAMILYINFO_BYID_L2C read familyid error");
					return -1;
				}

				////TRACE_INFO((Debug, "delete family, id=%d", familyid);

				ResultData_Family family;
				family.familyId = familyid;
				pRb->RecvFamilyInfo(flag, 1, &family);
			}
		}
		break;
	case CMD_UPDATE_AVCONFIG_L2C:
		{
			////TRACE_INFO((Debug, "recv a CMD_UPDATE_AVCONFIG_L2C");

			ResponseBase * pRb = g_NetApp.GetResponseHandler();
			if (pRb == NULL)
			{
				////TRACE_INFO((Error, "GetResponseHandler error");
				return -1;
			}

			char avconfig[1024];
			unsigned int avconfig_len;
			if (!stream.Read(avconfig, sizeof(avconfig), avconfig_len))
			{
				////TRACE_INFO((Error, "read avconfig error");
				return -1;
			}

			pRb->RecvAVConfigInfo(avconfig_len, avconfig);
		}
		break;
	default:
		break;
	}

	return 0;
}
