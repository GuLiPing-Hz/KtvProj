#include "reservedata.h"
#include "NetApp.h"
#include "XingguangDef.h"

void ReserveDataV::OnTimeOut()
{
	ResponseBase *pRb=g_NetApp.GetResponseHandler();

	LOGI("%s : serverid = %d,cmd = %d",__FUNCTION__,serverid,cmd);

	if(serverid == AUTHTUNNEL)
	{
		if(g_NetApp.GetAuthTunnel()->isClosed())
			return ;
		switch(cmd)
		{
			case CMD_GAME_GETTOKEN_BYID://ID认证
			case CMD_GAME_XG_VISTOR://游客认证
			{
				if (pRb)
						pRb->RecvAuth( -1, OPTION_TIMEOUT, repeat);
				break;
			}
		}
	}
	else if (serverid == LOBBYTUNNEL)
	{
        if (g_NetApp.GetLobbyTunnel()->isClosed())
            return ;

        switch (cmd)
        {
            case CMD_MPUBLICCHAT_C2L2C:
            case CMD_MPRIVATECHAT_C2L2C:
            {
                ReserveData_ChatKtv *pRD=(ReserveData_ChatKtv *)this;
				if (pRb)
					pRb->RecvKtvChatInfo(OPTION_TIMEOUT, pRD->to_idx, 0,"", pRD->content,0);
                break;
            }
            case CMD_MSEND_GIFT_C2L2C:
            {
                ReserveData_SendGift *pRD=(ReserveData_SendGift *)this;
				if (pRb)
					pRb->RecvSendFlower(-1,OPTION_TIMEOUT,pRD->r_idx,pRD->item_id,pRD->count,0,0);
                break;
            }
            case CMD_BROAD_LOBBY_C2L2C:
            {
                if(pRb)
                    pRb->RecvSpeakerToLobby(-1,OPTION_TIMEOUT,0,0);
                break;
            }
			case CMD_GETUSERINFOFORSPEAKER_C2L2C:
			{
				ReserveData_GetUserInfo * pRd = (ReserveData_GetUserInfo*)this;
				if(pRb)
					pRb->RecvUserInfoForSpeakerTimeOut(pRd->idx,pRd->count);
				break;
			}
			case CMD_GETUSERINFOWITHVERSION_C2L2C:
			{
				ReserveData_GetUserInfo * pRd = (ReserveData_GetUserInfo*)this;
				if(pRb)
					pRb->RecvUserInfoTimeOut(pRd->idx,pRd->count);
				break;
			}
			case CMD_GETSECTIONLIST_C2L2C:
			{
				if(pRb)
					pRb->RecvSectionList(-1,OPTION_TIMEOUT,NULL,0);
				break;
			}
			case CMD_TOKENLOGIN_C2L2C:
			{
				if(pRb)
					pRb->RecvTokenAuth4Lobby(-1,OPTION_TIMEOUT,NULL,NULL,cseq);
				break;
			}
            default:
                break;
        }
    }
    else if(serverid == ROOMTUNNEL)
	{
		if(g_NetApp.GetRoomTunnel()->isClosed())
			return;

		switch(cmd)
		{
		case ROOM_CMD_TOKEN:
			{
				if (pRb)
					pRb->RecvTokenAuth4Room(-1,OPTION_TIMEOUT);
			}
			break;
		case ROOM_CMD_LOGIN:
			{
				if (pRb)
					pRb->RecvEnterRoom(-1,OPTION_TIMEOUT,NULL,cseq);
			}
			break;
		case ROOM_CMD_UPDATE_CONTENT:
			{
				if (pRb)
					pRb->RecvRoomAffiche(-1,OPTION_TIMEOUT,cseq);
			}
			break;
		case ROOM_CMD_USER_APP_JOINROOM:
			{
				if (pRb)
					pRb->RecvApplyRoomMember(-1,OPTION_TIMEOUT,cseq);
			}
			break;
		case ROOM_CMD_VERIFY_USER_APP:
			{
				if (pRb)
					pRb->RecvAuditAddApply(-1,OPTION_TIMEOUT,0,cseq);
			}
			break;
		case ROOM_CMD_REMOVE_USER:
			{
				if (pRb)
					pRb->RecvUnchainMember(-1,OPTION_TIMEOUT,0,cseq);
			}
			break;
		case ROOM_CMD_GIVE_VJ_A:
			{
				if (pRb)
					pRb->RecvRoomPesideAssistant(-1,OPTION_TIMEOUT,0,cseq);
			}
			break;
		case ROOM_CMD_GIVE_VJ:
			{
				if (pRb)
					pRb->RecvRoomPeside(-1,OPTION_TIMEOUT,0,cseq);
			}
			break;
		case ROOM_CMD_GIVE_OUER_S:
			{
				if (pRb)
					pRb->RecvRoomSubHost(-1,OPTION_TIMEOUT,0,cseq);
			}
			break;
		case ROOM_CMD_SET_ROOM_PWD:
			{
				if (pRb)
					pRb->RecvRoomPassword(-1,OPTION_TIMEOUT,cseq);
			}
			break;
		case ROOM_CMD_SET_ROOM_LOCK:
			{
				if (pRb)
					pRb->RecvRoomSwitch(-1,OPTION_TIMEOUT,0,cseq);
			}
			break;
		case ROOM_CMD_SET_USER_ONLY:
			{
				if (pRb)
					pRb->RecvRoomPrivate(-1,OPTION_TIMEOUT,0,cseq);
			}
			break;
		case ROOM_CMD_SET_USER_INOUT:
			{
				if (pRb)
					pRb->RecvUserInOutSwitch(-1,OPTION_TIMEOUT,0,cseq);
			}
			break;
		case ROOM_CMD_SET_MIC_UPDOWN:
			{
				if (pRb)
					pRb->RecvOnMicSwitch(-1,OPTION_TIMEOUT,0,cseq);
			}
			break;
		case ROOM_CMD_SET_AUTOONMIC:
			{
				if (pRb)
					pRb->RecvGetMicSwitch(-1,OPTION_TIMEOUT,0,cseq);
			}
			break;
		case ROOM_CMD_SET_ROOM_NAME:
			{
				if (pRb)
					pRb->RecvModifyRoomName(-1,OPTION_TIMEOUT,cseq);
			}
			break;
		case ROOM_CMD_SET_CHAT_PUBLIC:
			{
				if (pRb)
					pRb->RecvAllowRoomAllChat(-1,OPTION_TIMEOUT,0,cseq);
			}
			break;
		case ROOM_CMD_SET_ROOM_WELCOME:
			{
				if (pRb)
					pRb->RecvRoomSalutatory(-1,OPTION_TIMEOUT,cseq);
			}
			break;
		case ROOM_CMD_SET_ROOM_LOGO:
			{
				if (pRb)
					pRb->RecvRoomLogo(-1,OPTION_TIMEOUT,cseq);
			}
			break;
		case ROOM_CMD_ROOMAPPLYLIST_C2R2C:
			{
				if (pRb)
					pRb->RecvRoomApplyList(-1,OPTION_TIMEOUT,NULL,cseq);
			}
			break;
		case ROOM_CMD_GETBLACKLIST:
			{
				if (pRb)
					pRb->RecvBlackList(-1,OPTION_TIMEOUT,NULL,cseq);
			}
			break;
		case ROOM_CMD_GETROOMMEMBERLIST:
			{
				if (pRb)
					pRb->RecvRoomMemberList(-1,OPTION_TIMEOUT,NULL,cseq);
			}
			break;
			//add by jinguanfu 2010/3/31 超时重取
		case ROOM_CMD_GET_ALLINFO:
			// add by xchen 重取由上层做
			break;
		case ROOM_CMD_EXITMEMBER:
			{
				if (pRb)
					pRb->RecvExitRoomMember(-1,OPTION_TIMEOUT);
			}
			break;
		case ROOM_CMD_SEND_GIFT:
			{
				ReserveData_SendGift *pRD=(ReserveData_SendGift *)this;
				if (pRb)
					pRb->RecvSendFlower(-1,OPTION_TIMEOUT,pRD->r_idx,pRD->item_id,pRD->count,0,0);
			}
			break;
		}
	}
	
	free((void*)this);
}
