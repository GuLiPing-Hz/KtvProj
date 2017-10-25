#include "DataDecoderLobby.h"
#include "NetApp.h"
#include "dbresult.h"
#include "wrap/NUtility.h"
#include "protocol.h"
#include "localstorage/DataMgr.h"
#include "DataDecodeUtilty.h"
#include <vector>
//#include <stl/_vector.h>
#include "ChatCompatibility.h"

#include "wrap/config.h"
#ifndef NETUTIL_ANDROID
#define LOGI printf
#define LOGE printf
#endif

extern CDataMgr g_DM;

#undef PROTOCOL_READ
#define PROTOCOL_READ(x) \
if(!stream.Read(x)) \
{ \
	LOGE("Protocol Read %s, Error in file: %s,line %d\n",#x,__FILE__,__LINE__);\
    return -1; \
}


#undef PROTOCOL_READSTR
#define PROTOCOL_READSTR(x,len) \
if(!stream.Read(x,sizeof(x),len))\
{\
	LOGE("Protocol Read %s, Error in file: %s,line %d\n",#x,__FILE__,__LINE__);\
    return -1;\
}\
x[len] = 0

int LobbyDataDecoder::OnPackage_New( short cmd, int seq, void * pStream, void * pTunnel )
{
	BinaryReadStream & stream = * (BinaryReadStream *)pStream;

    ResponseBase *pRb = g_NetApp.GetResponseHandler();
    
    switch (cmd) {
        //废弃Deprecated
        case CMD_GETUSERLIST_L2C://服务器发送手机用户列表
        {
//            int number = 0;
//            PROTOCOL_READ(number);
//            
//			std::vector<int> list;
//			if(number > 0)
//			{
//				list.resize(number);
//
//				for (int i = 0; i<number; i++) 
//					PROTOCOL_READ(list[i]);
//				if(pRb)
//					pRb->RecvKtvIdxList(&list[0], number);
//			}
            break;
        }
        case CMD_MPUBLICCHAT_C2L2C://手机主页公聊
        {
            int idx = 0;
            int roomid = 0;
            char roomname[256] = {0};
            char content[4096] = {0};
            int chattime;
            PROTOCOL_READ(idx);
            PROTOCOL_READ(roomid);
            unsigned int len = 0;
            PROTOCOL_READSTR(roomname, len);
            PROTOCOL_READSTR(content, len);
            PROTOCOL_READ(chattime);
            
			char utf8[4096] = {0};
			int char_len = 4096;
			int ret = CChatCompatibility::ConvertXml2Normal(utf8,char_len,content);
			if(ret == 0 || ret == -2)//如果转换成功，或者UTF转换失败
				strcpy(content,utf8);
			else if(ret == -1)
				LOGI("chat content no need to conver to utf8\n");

            ReserveData** pRD = g_NetApp.GetRDMap()->Get(seq);
            if(pRD)//我发公聊消息回应
            {
                RDGuard guard(*pRD,seq);
                if(pRb)
                	pRb->RecvKtvChatInfo(0, 0,roomid, roomname, content, chattime);
            }
            else//别人发给我的公聊消息
            {
            	if(pRb)
            		pRb->RecvKtvChatInfo(0,idx,roomid, roomname, content, chattime,true,false);
            }
            break;
        }
        case CMD_MPRIVATECHAT_C2L2C://互动私聊
        {
            int idx = 0;
            char content[4096] = {0};
            int chattime = 0;
            int ret = 0;
            int roomId;
            char roomName[260] = {0};

            PROTOCOL_READ(ret);
            if(ret == 0)
            {
            	PROTOCOL_READ(idx);
				unsigned int len = 0;
				PROTOCOL_READSTR(content, len);
				PROTOCOL_READ(chattime);


				PROTOCOL_READ(roomId);
				PROTOCOL_READSTR(roomName,len);

				char utf8[4096] = {0};
				int char_len = 4096;
				int ret = CChatCompatibility::ConvertXml2Normal(utf8,char_len,content);
				if(ret == 0 || ret == -2)//如果转换成功，或者UTF转换失败
					strcpy(content,utf8);
				else if(ret == -1)
					LOGI("chat content no need to conver to utf8\n");
            }

			ReserveData** pRD = g_NetApp.GetRDMap()->Get(seq);
			if(pRD)//我发给别人私聊消息回应
			{
				RDGuard guard(*pRD,seq);
				if(pRb)
					pRb->RecvKtvChatInfo(ret, idx, roomId, roomName, content, chattime,false);
			}
			else//别人发给我的私聊消息
			{
				if(pRb)
					pRb->RecvKtvChatInfo(ret, idx, roomId, roomName, content, chattime,false,false);
			}

            break;
        }
        case CMD_MSEND_GIFT_C2L2C://送礼物，(大厅送礼物)
        {            
        	ReserveData **pRD = g_NetApp.GetRDMap()->Get(seq);
			if(!pRD)
				return -1;
			RDGuard guard(*pRD,seq);//从回应超时列表中去除，并释放内存

            int  r_idx;    //礼物接受者idx
            int  gift_idx; //礼物id
            int  number;   //礼物个数
            int  DBRESULT_GIFT;  //结果
            PROTOCOL_READ(r_idx);
            PROTOCOL_READ(gift_idx);
            PROTOCOL_READ(number);
            PROTOCOL_READ(DBRESULT_GIFT);
            switch (DBRESULT_GIFT)
            {
                case DBRESULT_GIFT_NOMONEY://余额不足
                {
                    if(pRb)
                        pRb->RecvSendFlower(-1,ROOM_ERROR_GIFT_BALANCE,r_idx,gift_idx,number,0,0);
                    break ;
                }
                case DBRESULT_GIFT_SUCCESS:		//成功
                {
                    int gold;	//金币余额
                    int silver;//银币余额
                    PROTOCOL_READ(gold);
                    PROTOCOL_READ(silver);
                    //更新本地数据库的个人金币信息
                    g_DM.saveMoney(gold,silver);
                    if(pRb)
                        pRb->RecvSendFlower(0,ROOM_NORMAL,r_idx,gift_idx,number,gold,silver);
                    break ;
                }
                case DBRESULT_GIFT_NULL:		//交易金额为0
                {
                    if(pRb)
                        pRb->RecvSendFlower(-1,ROOM_ERROR_GIFT_UNKOWN,r_idx,gift_idx,number,0,0);
                    break ;
                }
                case DBRESULT_GIFT_NOGIFT:		//无此交易类型
                {
                    if(pRb)
                        pRb->RecvSendFlower(-1,ROOM_ERROR_GIFT_NOTEXIST,r_idx,gift_idx,number,0,0);
                    break ;
                }
                case DBRESULT_GIFT_OFFLINE:		//礼物接收者不在大厅内
                {
                    if(pRb)
                        pRb->RecvSendFlower(-1,ROOM_ERROR_GIFT_OFFLINE,r_idx,gift_idx,number,0,0);
                    break ;
                }
            }
            break;
        }
        case CMD_UPDATEMONEY_L2C:
        {
            int gold;
            int silver;
            PROTOCOL_READ(gold);
            PROTOCOL_READ(silver);
            //更新本地数据库
            g_DM.saveMoney(gold, silver);
            //通知界面进行更新
            if(pRb)
                pRb->UpdataMoney(gold,silver);
            break;
        }
        case CMD_UPDATE_RECVCOUNT_L2C:
        {
            int num;
            PROTOCOL_READ(num);
            //更新本地数据库
            g_DM.updateMyRecvGiftCount(num);
            break;
        }
        case CMD_SB_SEND_GIFT_L2C:
        {
            ResultData_SendFlowerInfo sendflower;
			sendflower.s_idx=0;
			PROTOCOL_READ(sendflower.s_idx);
			PROTOCOL_READ(sendflower.r_idx);
            PROTOCOL_READ(sendflower.cate_idx);
            PROTOCOL_READ(sendflower.number);
            PROTOCOL_READ(sendflower.leavings);
			PROTOCOL_READ(sendflower.time);
			PROTOCOL_READ(sendflower.once_num);
            PROTOCOL_READ(sendflower.s_roomid);//发送者房间id    0-大厅
            unsigned int len = 0;
            PROTOCOL_READSTR(sendflower.s_room, len);
            PROTOCOL_READ(sendflower.r_roomid);//接收者id   0-大厅
            PROTOCOL_READSTR(sendflower.r_room, len);

            if(pRb)
                pRb->RecvSendFlowerNotice(0, &sendflower);
            break;
        }
        case CMD_SB_GIFT_VALID_L2C://大厅通知印章有效
        {
			int recv_idx = 0;
			int gift_id = 0;
			PROTOCOL_READ(recv_idx);
			PROTOCOL_READ(gift_id);
			if(pRb)
				pRb->RecvGiftValid(recv_idx,gift_id);
            break;
        }
		case CMD_SB_GIFT_INVALID_L2C://大厅通知印章失效
			{
				int recv_idx = 0;
				int gift_id = 0;
				PROTOCOL_READ(recv_idx);
				PROTOCOL_READ(gift_id);
				if(pRb)
					pRb->RecvGiftInvalid(recv_idx,gift_id);
				break;
			}
        case CMD_MGET_ROOMLIST_IN_GROUP_C2L2C:
			{
				short groupnumber = 0;
				PROTOCOL_READ(groupnumber);

				if(groupnumber > 0)
					LOGE("group numnber is zero\n");
				else
				{
					std::vector<ResultData_GroupRoomInfo> info;
					info.resize(groupnumber);

					for (int i=0; i<groupnumber; i++) 
					{
						if(!stream.Read(info[i].grpup_id)) 
						{ 
							LOGE("Protocol Read grpup_id, Error in file: %s,line %d\n",__FILE__,__LINE__);
							for(int j=0;j<i;j++)//释放之前已经生成的内存
								delete info[j].roomlist;
							return -1; 
						}

						if(!stream.Read(info[i].roomnum)) 
						{ 
							LOGE("Protocol Read roomnum, Error in file: %s,line %d\n",__FILE__,__LINE__);
							for(int j=0;j<i;j++)//释放之前已经生成的内存
								delete info[j].roomlist;
							return -1; 
						}

						info[i].roomlist = new int[info[i].roomnum];
						if(!info[i].roomlist)
						{
							LOGE("roomlist oom\n");
							for(int j=0;j<i;j++)//释放之前已经生成的内存
								delete info[j].roomlist;
							return -1;
						}

						for (int j=0; j<info[i].roomnum; j++)
							PROTOCOL_READ(info[i].roomlist[j]);
					}

					if (pRb)
						pRb->RecvRoomListInGroup(groupnumber,&info[0]);

					for(int k=0;k<groupnumber;k++)//释放内存
						delete info[k].roomlist;
				}
				break;
			}
		case CMD_GETUSERINFOFORSPEAKER_C2L2C:
			{
				int ret = -1;
				PROTOCOL_READ(ret);

				ResultData_UserSampleInfo info = {0};
				PROTOCOL_READ(info.idx);

				ReserveData **pRD = g_NetApp.GetRDMap()->Get(seq);
				if(!pRD)
					return -1;
				ReserveData_GetUserInfo* pUserInfo = (ReserveData_GetUserInfo*) (*pRD);
				bool bFind = false;//是否在超时列表中
				bool bAll = true;//是否已经全部获得回应
				for (int i=0;i<pUserInfo->count;i++)
				{
					if(pUserInfo->idx[i] == 0)
						continue;
					if(pUserInfo->idx[i] == info.idx)
					{
						bFind = true;
						pUserInfo->idx[i] = 0;
					}

					if(pUserInfo->idx[i] != 0)
						bAll = false;
				}

				if(bFind && bAll)//如果全部获得回应
					RDGuard guard(*pRD,seq);//从回应超时列表中去除，并释放内存

				if(ret == RET_OK)
				{
					unsigned int tmp_len=0;
					PROTOCOL_READSTR(info.nickname,tmp_len);
					PROTOCOL_READSTR(info.headurl,tmp_len);
				}

				if (pRb)
					pRb->RecvUserInfoForSpeaker(ret,&info);
				break;
			}
		case CMD_GETUSERINFOWITHVERSION_C2L2C:
			{
				int ret;
				PROTOCOL_READ(ret);

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
					int version_id = -1;
					PROTOCOL_READ(version_id);
					
					g_DM.saveUserInfo(rd,version_id);
					if (pRb)
						pRb->RecvUserInfo(0,LOBBY_NORMAL,&rd);
				}
				else if (ret == RET_NOTEXIST)
				{
					PROTOCOL_READ(rd.m_idx);
					if (pRb)
						pRb->RecvUserInfo(-1,LOBBY_USERINFO_NOTEXIST,&rd);
				}
                else if(ret == RET_NONEEDUPDATE)
                {
                    PROTOCOL_READ(rd.m_idx);
					if (pRb)
						pRb->RecvUserInfo(0,LOBBY_NONEED_UPDATE,&rd);
                }
                else
                {
                    LOGE("%s [line %d]: ret read error",__FUNCTION__,__LINE__);
                    return -1;
                }

				ReserveData **pRD = g_NetApp.GetRDMap()->Get(seq);
				if(!pRD)
					return -1;
				ReserveData_GetUserInfo* pUserInfo = (ReserveData_GetUserInfo*) (*pRD);
				bool bFind = false;//是否在超时列表中
				bool bAll = true;//是否已经全部获得回应

				for (int i=0;i<pUserInfo->count;i++)
				{
					if(pUserInfo->idx[i] == 0)
						continue;
					if(pUserInfo->idx[i] == rd.m_idx)
					{
						bFind = true;
						pUserInfo->idx[i] = 0;
					}

					if(pUserInfo->idx[i] != 0)
						bAll = false;
				}

				if(bFind && bAll)//如果全部获得回应
					RDGuard guard(*pRD,seq);//从回应超时列表中去除，并释放内存
				break;
			}
		case CMD_SB_UPDATEGIFT_L2C:
			{
				//重新加载礼物信息
				g_DM.reinitGift();
				break;
			}
		case CMD_CHANGEHEADPHOTO_L2C:
			{
				int idx = 0;
				char headpic[260] = {0};
				PROTOCOL_READ(idx);
				unsigned int len;
				PROTOCOL_READSTR(headpic,len);

				g_DM.changeUserPic(idx,headpic);

				if(pRb)
					pRb->RecvUserPicChange(idx,headpic);
				break;
			}
		case CMD_CHANGENICKNAME_L2C:
			{
				int idx;
				PROTOCOL_READ(idx);
				char name[51] = {0};
				unsigned int len;
				PROTOCOL_READSTR(name,len);
				if(pRb)
					pRb->RecvUserNameChange(idx,name);
				break;
			}
		case CMD_KTVMESSAGE_L2C:
			{
				int msgId;
				unsigned int len;
				char msgContent[500] = {0};
				PROTOCOL_READ(msgId);
				PROTOCOL_READSTR(msgContent,len);

				if(msgId > g_DM.getCurMsgId())
				{
					if(pRb)
						pRb->RecvNotifyMessage(msgId,msgContent);
					g_DM.setCurMsgId(msgId);
				}
				break;
			}
		case CMD_GETUSERLISTANDLOGINWAY_L2C://服务器发送手机用户列表以及在线方式
			{
				int num = 0;
				std::vector<ResultData_UIdxWithType> list;
				PROTOCOL_READ(num);
				if(num > 0)
				{
					list.resize(num);

					for(int i=0;i<num;i++)
					{
						PROTOCOL_READ(list[i].idx);
						PROTOCOL_READ(list[i].type);
					}

					if(pRb)
						pRb->RecvUserIdxAndType(&list[0],num);
				}
				
				break;
			}
		case CMD_USERLOGIN_NEW_L2C:  //通知给其他用户大厅用户登陆,加type
			{
				int idx;
				char type;
				PROTOCOL_READ(idx);
				PROTOCOL_READ(type);

				if(pRb)
					pRb->RecvUserLoginLobbyNoticeNew(idx,type);
				break;
			}
		case CMD_SEND_REDGOLD_C2L2C:    //发红包
			{
				int ret;
				PROTOCOL_READ(ret);

				ReserveData** ppRD = g_NetApp.GetRDMap()->Get(seq);
				if(!ppRD || !*ppRD)
					return -1;
				RDGuard guard(*ppRD,seq);

				if(pRb)
					pRb->RecvSendHBResult(ret);
				break;
			}
		case CMD_RECV_REDGOLD_L2C:  //收红包
			{
				int idx;
				char hbid[51] = {0};
				char mark[50] = {0};
				char hbtype;

				PROTOCOL_READ(idx);
				if(idx == 0)//为了区分KTV聊天的时候返回的IDX是0，其实是自己的情况，这里用1代表系统
					idx = 1;
				unsigned int hbid_len;
				PROTOCOL_READSTR(hbid,hbid_len);
				unsigned int mark_size;
				PROTOCOL_READSTR(mark,mark_size);
				PROTOCOL_READ(hbtype);

				if(pRb)
					pRb->RecvHongBao(idx,hbid,hbtype,mark);
				break;
			}
		case CMD_VIEFOR_REDGOLD_C2L2C:  //抢红包
			{
				int sendidx; //发红包idx
				char redid[51] = {0};  //红包id
				char redtype; //红包类型 
				char mark[256] = {0};  //备注
				int result;  //结果: 抢红包的结果: 0-没抢到  1-抢到 2-已抢
				int gold;    //抢到金额(没抢到发0)

				PROTOCOL_READ(sendidx);
				unsigned int redid_len;
				PROTOCOL_READSTR(redid,redid_len);
				PROTOCOL_READ(redtype);
				unsigned int mark_size;
				PROTOCOL_READSTR(mark,mark_size);
				PROTOCOL_READ(result);
				PROTOCOL_READ(gold);

				ReserveData** ppRD = g_NetApp.GetRDMap()->Get(seq);
				if(!ppRD || !*ppRD)
					return -1;

				RDGuard guard(*ppRD,seq);
				if(pRb)
					pRb->RecvVieHongBaoResult(result,sendidx,redid,redtype,mark,gold);
				break;
			}
		case CMD_WATCH_REDGOLD_C2L2C:    //查看红包领取记录
			{
				int sendidx;    //发送者idx
				char redtype;   //红包类型
				char mark[256] = {0};  //红包备注
				int totalgold;  //红包总金额
				int totalrednum;   //红包总个数
				int num;        //已抢个数
				int sendtime;//红包发送时间
				std::vector<ResultData_HBDetail> list; 

				PROTOCOL_READ(sendidx);
				PROTOCOL_READ(redtype);
				unsigned int mark_size;
				PROTOCOL_READSTR(mark,mark_size);
				PROTOCOL_READ(totalgold);
				PROTOCOL_READ(totalrednum);
				PROTOCOL_READ(num);
				PROTOCOL_READ(sendtime);

				if(num > 0)
				{
					list.resize(num);
					for(int i=0;i<num;i++)
					{
						PROTOCOL_READ(list[i].idx);
						PROTOCOL_READ(list[i].gold);
						PROTOCOL_READ(list[i].time);
					}
				}

				ReserveData** ppRD = g_NetApp.GetRDMap()->Get(seq);
				if(!ppRD || !*ppRD)
					return -1;

				RDGuard guard(*ppRD,seq);

				if(pRb)
					pRb->RecvHongBaoDetail(0,sendidx,redtype,mark,totalgold,totalrednum,sendtime,num?&list[0]:NULL,num);
				break;
			}
		case CMD_UPDATE_LASTSONG_INFO_L2C:
			{
				char songname[256] = {0};
				char singername[256] = {0};
				int score;

				unsigned int name_len;
				PROTOCOL_READSTR(songname,name_len);
				PROTOCOL_READSTR(singername,name_len);
				PROTOCOL_READ(score);

				if(pRb)
					pRb->RecvSingSongChange(songname,singername,score);
				break;
			}
		case CMD_UPDATE_REDNUM_L2C:
			{
				int rednum = 0;
				PROTOCOL_READ(rednum);

				if(pRb)
					pRb->RecvHongBaoNum(rednum);
				break;
			}
		case CMD_CAN_GET_RED_C2L2C:
		{
			char hbid[51] = {0};
			int ret;
			unsigned int hblen = 0;
			PROTOCOL_READSTR(hbid,hblen);
			PROTOCOL_READ(ret);

			ReserveData** ppRD = g_NetApp.GetRDMap()->Get(seq);
			if(!ppRD || !*ppRD)
				return -1;

			RDGuard guard(*ppRD,seq);

			if(pRb)
				pRb->RecvHongBaoStatus(hbid,ret);
			break;
		}
		case CMD_GET_ROOM_REDPACKET_C2L2C:
			{
				int number;
				std::vector<ResultData_RoomHB> list;

				PROTOCOL_READ(number);
				if(number > 0)
				{
					list.resize(number);
					for(int i=0;i<number;i++)
					{
						PROTOCOL_READ(list[i].idx);
						unsigned int len = 0;
						PROTOCOL_READSTR(list[i].hbid,len);
						len = 0;
						PROTOCOL_READSTR(list[i].mark,len);
						PROTOCOL_READ(list[i].hbtype);
					}
				}
				if(pRb)
					pRb->RecvRoomHB(0,number,number?&list[0]:NULL);
				break;
			}
        default:
            break;
    }
	return 0;
}
