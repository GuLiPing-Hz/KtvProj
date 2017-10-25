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

int RoomDataDecoder::onPackage2(short cmd, int seq, void * pStream, void * pTunnel)
{
	BinaryReadStream & stream = * (BinaryReadStream *)pStream;
	RoomTunnel * pRoomTunnel = (RoomTunnel *)pTunnel;
	ResponseBase* pRb = g_NetApp.GetResponseHandler();

	switch (cmd)
	{
	
	default:
		break;
	}
	return 0;
}
