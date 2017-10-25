#ifndef ROOMDATADECODER__H__
#define ROOMDATADECODER__H__
#include "wrap/datadecoder.h"

using namespace NetworkUtil;

class RoomDataDecoder : public DataDecoder
{
public:
	RoomDataDecoder() : DataDecoder(PROTOCOLTYPE_BINARY,HEADER_LEN_2) {}
	virtual ~RoomDataDecoder(){}
	virtual int onPackage(ClientSocketBase *pClient,const char* buf,unsigned int buflen);
	static RoomDataDecoder* Instance()
	{
		static RoomDataDecoder dd;
		return &dd;
	};

private:
	int OnPackage_Base( short cmd, int seq, void * pStream, void * pTunnel );
	int OnPackage_Practice( short cmd, int seq, void * pStream, void * pTunnel );
	
	int onPackage1(short cmd, int seq, void * pStream, void * pTunnel);
	int onPackage2(short cmd, int seq, void * pStream, void * pTunnel);
};
#endif//ROOMDATADECODER__H__
