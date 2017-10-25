#include "DataDecoderLobby.h"
#include "NetApp.h"
#include "dbresult.h"
#include "wrap/NUtility.h"
#include "protocol.h"
#include "localstorage/DataMgr.h"
#include "DataDecodeUtilty.h"

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
        case CMD_GETUSERLIST_L2C://服务器发送手机用户列表
        {
            int number = 0;
            PROTOCOL_READ(number);
            
			int* pIdx = new int[number];
			if(!pIdx)
			{
				LOGE("idx array pointer new failed\n");
				return -1;
			}

            for (int i = 0; i<number; i++) 
			{
                PROTOCOL_READ(pIdx[i]);
            }
            if(pRb)
            	pRb->RecvKtvIdxList(pIdx, number);
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
			bool isParsed = CDataMgr::ParseChatXml(content,utf8,sizeof(utf8));

            ReserveData** pRD = g_NetApp.GetRDMap()->Get(seq);
            if(pRD)//我发公聊消息回应
            {
                RDGuard guard(*pRD,seq);
                if(pRb)
                	pRb->RecvKtvChatInfo(0, 0,roomid, roomname, isParsed?utf8:content, chattime);
            }
            else//别人发给我的公聊消息
            {
            	if(pRb)
            		pRb->RecvKtvChatInfo(0,idx,roomid, roomname, isParsed?utf8:content, chattime,true,false);
            }
            break;
        }
        case CMD_MPRIVATECHAT_C2L2C://互动私聊
        {
            int idx = 0;
            char content[4096] = {0};
            int chattime;
            PROTOCOL_READ(idx);
            unsigned int len = 0;
            PROTOCOL_READSTR(content, len);
            PROTOCOL_READ(chattime);

			int roomId;
			char roomName[260];
			PROTOCOL_READ(roomId);
			PROTOCOL_READSTR(roomName,len);

			char utf8[4096] = {0};
			bool isParsed = CDataMgr::ParseChatXml(content,utf8,sizeof(utf8));

            ReserveData** pRD = g_NetApp.GetRDMap()->Get(seq);
            if(pRD)//我发给别人私聊消息回应
            {
                RDGuard guard(*pRD,seq);
                if(pRb)
                	pRb->RecvKtvChatInfo(0, idx, roomId, roomName, isParsed?utf8:content, chattime,false);
            }
            else//别人发给我的私聊消息
            {
            	if(pRb)
            		pRb->RecvKtvChatInfo(0, idx, roomId, roomName, isParsed?utf8:content, chattime,false,false);
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
				ResultData_GroupRoomInfo* pInfo = new ResultData_GroupRoomInfo[groupnumber];
				if(!pInfo)
				{
					LOGE("ResultData_GroupRoomInfo OOM\n");
					return -1;
				}
				for (int i=0; i<groupnumber; i++) {
					PROTOCOL_READ(pInfo[i].grpup_id);
					PROTOCOL_READ(pInfo[i].roomnum);
					pInfo[i].roomlist = new int[pInfo[i].roomnum];
					if(!pInfo[i].roomlist)
					{
						LOGE("RoomList OOM for group id = %d\n",pInfo[i].grpup_id);
						return -1;
					}
					for (int j=0; j<pInfo[i].roomnum; j++)
						PROTOCOL_READ(pInfo[i].roomlist[j]);
				}

				if (pRb)
					pRb->RecvRoomListInGroup(groupnumber,pInfo);
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
				char msgContent[500];
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
        default:
            break;
    }
	return 0;
}
