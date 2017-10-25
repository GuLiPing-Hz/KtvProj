#include "DataDecoderAuth.h"
#include "NetApp.h"
#include "protocol.h"
#include "XingguangDef.h"

#ifndef NETUTIL_ANDROID
#define LOGI printf
#endif

int AuthDataDecoder::onPackage(NetworkUtil::ClientSocketBase *pClient,const char* buf,unsigned int buflen)
{
	//AuthTunnel *pAuthTunnel = (AuthTunnel*)pClient;
	NetworkUtil::BinaryReadStream stream(buf,buflen);

	short cmd_yewu;
	if(!stream.Read(cmd_yewu))
	{
		////TRACE_INFO(((Error, "read cmd_yewu error");
		return -1;
	}
	short cmd;
	if(!stream.Read(cmd))
	{
		////TRACE_INFO(((Error, "read cmd error");
		return -1;
	}
	int seq;
	if(!stream.Read(seq))
	{
		////TRACE_INFO(((Error, "read seq error");
		return -1;
	}
	//LOGI("AuthDataDecoder::OnPackage: read cmd %d\n",cmd);
	switch(cmd)
	{
	case CMD_GAME_GETTOKEN_BYID:
	//case CMD_GAME_GETTOKEN_BYIDX:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveDataV **ppRD = pSeqMap->Get(seq);
			ReserveDataV* pRD = *ppRD;
			if(!ppRD || !pRD)//程序异常
				return -1;
			RDGuard guard(*ppRD,seq);

			int ret;
			if(!stream.Read(ret))
				return -1;
			char token[ MAXLEN_TOKEN ];
			unsigned int tokenlen;
			if(ret == RET_OK)
			{
				if(!stream.Read(token,sizeof(token),tokenlen))
					return -1;
				g_NetApp.SetToken(token,tokenlen);
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				if (ret == RET_OK)
					pRb->RecvAuth( 0, AUTH_NORMAL, pRD->repeat);
				else if (ret == RET_NOTEXIST)
					pRb->RecvAuth( -1, AUTH_NOTEXIST, pRD->repeat);
				else if (ret == RET_PASSWDERROR)
					pRb->RecvAuth( -1, AUTH_PWDERROR, pRD->repeat);
				else
					pRb->RecvAuth( -1, AUTH_CACHEERROR, pRD->repeat);
			}
		}
		break;
	case CMD_GAME_XG_VISTOR:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveDataV **ppRD = pSeqMap->Get(seq);
			ReserveDataV* pRD = *ppRD;
			if(!ppRD || !pRD)//程序异常
				return -1;
			RDGuard guard(*ppRD,seq);

			int ret;
			if(!stream.Read(ret))
				return -1;

			char token[ MAXLEN_TOKEN ];
			unsigned int tokenlen;
			if(ret == RET_OK)
			{
				if(!stream.Read(token,sizeof(token),tokenlen))
					return -1;
				g_NetApp.SetToken(token,tokenlen);
			}

			ResponseBase *pRb=g_NetApp.GetResponseHandler();
			if (pRb)
			{
				if (ret == RET_OK)
					pRb->RecvAuth( 0, AUTH_NORMAL, pRD->repeat );
				else
					pRb->RecvAuth( -1, AUTH_TOURISTERROR, pRD->repeat );
			}
		}
		break;
	case CMD_GETNEWESTTOKEN:
		{
			TMSeqMap *pSeqMap = g_NetApp.GetRDMap();
			ReserveDataV **pRD = pSeqMap->Get(seq);
			if(!pRD)
				return -1;//被超时机制排除了
			RDGuard guard(*pRD,seq);

			int ret;
			if(!stream.Read(ret))
				return -1;
			if(ret == RET_OK)
			{
				char token[256];
				unsigned int tokenlen;
				if(!stream.Read(token,sizeof(token),tokenlen))
					return -1;
				g_NetApp.SetToken(token,tokenlen);

				ResponseBase *pRb=g_NetApp.GetResponseHandler();
				if (pRb)
					pRb->RecvNewToken(0,AUTH_NORMAL);
			}
			else
			{
				ResponseBase *pRb=g_NetApp.GetResponseHandler();
				if (pRb)
					pRb->RecvNewToken(-1,AUTH_TOKENERROR);
			}
		}
		break;
	default:
		break;
	}

	return 0;
}
