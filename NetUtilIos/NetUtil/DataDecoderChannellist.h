#ifndef CHANNELLISTDATADECODER__H__
#define CHANNELLISTDATADECODER__H__
#include "wrap/datadecoder.h"

class ClientSocketBase;
class ChannellistDataDecoder : public NetworkUtil::DataDecoder
{
public:
	ChannellistDataDecoder() : NetworkUtil::DataDecoder(NetworkUtil::PROTOCOLTYPE_BINARY,NetworkUtil::HEADER_LEN_2){}
	virtual ~ChannellistDataDecoder(){}
	virtual int onPackage(NetworkUtil::ClientSocketBase *pClient,const char* buf,unsigned int buflen);
	static ChannellistDataDecoder* Instance()
	{
		static ChannellistDataDecoder dd;
		return &dd;
	};
};

#endif//CHANNELLISTDATADECODER__H__

