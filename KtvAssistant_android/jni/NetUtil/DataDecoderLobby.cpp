#include "DataDecoderLobby.h"
#include "NetApp.h"
#include "wrap/NUtility.h"
#include "wrap/crypt.h"
#include "XingguangDef.h"
#include "dbresult.h"

#include "wrap/config.h"
#ifndef NETUTIL_ANDROID
#define LOGI printf
#define LOGE printf
#endif
//typedef OnPackage OnPackage2;

void GetAvIpFromString( char out_ip[][MAXLEN_IP], char & count, char * ip_buf, char key )
{
	int ip_len = 0;
	char * ip_begin = NULL;
	char * ip_end = ip_buf;

	count = 0;
	for( int i=0; i<MAXLEN_ROOM_AVIPCOUNT; ++i )
	{
		ip_begin = ip_end;
		ip_end = strchr( ip_begin, key );
		if( NULL == ip_end )
		{
			ip_len = strlen( ip_begin );
		}
		else
		{
			ip_len = int(ip_end - ip_begin);
		}
		strncpy( out_ip[i], ip_begin, ip_len );
		out_ip[i][ip_len] = 0;
		count++;
		if( NULL == ip_end )
		{
			break;
		}
		ip_end++;
	}
}


int LobbyDataDecoder::onPackage(ClientSocketBase *pClient,const char* buf,unsigned int buflen)
{
	LobbyTunnel *pLobbyTunnel = (LobbyTunnel*)pClient;

	char outbuf[65535];
	int outbuflen = sizeof(outbuf);

    LOGI("LobbyDataDecode Buffer Size = %d\n",buflen);
	if(pLobbyTunnel->IsTokenLogin() == true)
	{
		if(buf[2] == TEAFLAG)
		{
            LOGI("LobbyDataDecode Buffer TEAFLAG\n");
			if(!StreamDecrypt(buf,(int)buflen,outbuf,outbuflen,pLobbyTunnel->m_Sessionkey))
			{
				LOGE("cann't decrypt");
				return -1;
			}
		}
		else if(buf[2] == COMPRESSFLAG)
		{
            LOGI("LobbyDataDecode Buffer COMPRESSFLAG\n");
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
		strncpy(tempsessionkey,LOBBYFIRSTKEY,sizeof(tempsessionkey));

		if(!StreamDecrypt(buf,(int)buflen,outbuf,outbuflen,tempsessionkey,2))
		{
			LOGE("cann't decrypt\n");
			return -1;
		}
	}

	BinaryReadStream stream(outbuf,outbuflen);

	char c;
	if(!stream.Read(c))
	{
        LOGE("%s: char read error\n",__FUNCTION__);
		return -1;
	}

	short cmd;
	if(!stream.Read(cmd))
	{
        LOGE("%s: cmd read error\n",__FUNCTION__);
		return -1;
	}
	int seq;
	if(!stream.Read(seq))
	{
        LOGE("%s: seq read error\n",__FUNCTION__);
		return -1;
	}

	int ret = 0;
	LOGI("read lobby cmd %d\n",cmd);
	if( CMD_MAX_LOBBY_OLD < cmd )
		ret = OnPackage_New( cmd, seq, & stream, pLobbyTunnel );
	else
		ret = OnPackage_Old( cmd, seq, & stream, pLobbyTunnel );
	return ret;
}
