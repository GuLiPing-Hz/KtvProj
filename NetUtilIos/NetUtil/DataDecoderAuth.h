#ifndef AUTHDATADECODER__H__
#define AUTHDATADECODER__H__

/*
	注释添加以及修改于 2014-4-2 
	mender: glp

	封装一个对认证数据处理的类 AuthDataDecoder 单件类
	OnPackage 是对认证数据的具体处理
*/

#include "wrap/datadecoder.h"

class AuthDataDecoder : public NetworkUtil::DataDecoder
{
private:
	AuthDataDecoder() : DataDecoder(NetworkUtil::PROTOCOLTYPE_BINARY,NetworkUtil::HEADER_LEN_2){}
public:
	virtual ~AuthDataDecoder(){}
	virtual int onPackage(NetworkUtil::ClientSocketBase *pClient,const char* buf,unsigned int buflen);
	static AuthDataDecoder* Instance()
	{
		static AuthDataDecoder dd;
		return &dd;
	};
};

#endif//AUTHDATADECODER__H__

