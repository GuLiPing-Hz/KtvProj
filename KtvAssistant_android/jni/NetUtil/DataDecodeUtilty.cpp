#include "DataDecodeUtilty.h"

#include "wrap/config.h"
#ifndef NETUTIL_ANDROID
#define LOGI printf
#define LOGE printf
#endif

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

int _ChangeAvarat( ResultData_UserFullInfo * pInfo, NetworkUtil::BinaryReadStream * pBuf )
{
	if(!pBuf->Read(pInfo->m_AvatarId))
		return -1;

	char maxcount=0;
	if (!pBuf->Read(maxcount))
		return -1;
	pInfo->m_AvaratCount=maxcount;

	if( NULL == pInfo->m_AvaratData )
		return -1;

	for(char i=0; i<maxcount; i++)
	{
		short tmpAvaratData = 0;
		if (!pBuf->Read(tmpAvaratData))
			return -1;

		pInfo->m_AvaratData[i] = tmpAvaratData;
	}

	return 0;
}

int _ReadUserData( NetworkUtil::BinaryReadStream & stream, ResultData_UserFullInfo & pInfo )
{
	unsigned int len;
	if(!stream.Read(pInfo.m_idx))
		return -1;
	if(!stream.Read(pInfo.m_id,sizeof(pInfo.m_id),len))
		return -1;
	if(!stream.Read(pInfo.m_name,sizeof(pInfo.m_name),len))
		return -1;
	if(!stream.Read(pInfo.m_province,sizeof(pInfo.m_province),len))
		return -1;
	if(!stream.Read(pInfo.m_city,sizeof(pInfo.m_city),len))
		return -1;
	if(!stream.Read(pInfo.m_sex,sizeof(pInfo.m_sex),len))
		return -1;
	if(!stream.Read(pInfo.m_age,sizeof(pInfo.m_age),len))
		return -1;
	if(!stream.Read(pInfo.m_level))
		return -1;
	if(!stream.Read(pInfo.m_isVip))
		return -1;
	if(!stream.Read(pInfo.m_stageName,sizeof(pInfo.m_stageName),len))
		return -1;
	if(!stream.Read(pInfo.m_ClubId,sizeof(pInfo.m_ClubId),len))
		return -1;
	if(!stream.Read(pInfo.m_ClubProfession,sizeof(pInfo.m_ClubProfession),len))
		return -1;
	if(!stream.Read(pInfo.m_ClubName,sizeof(pInfo.m_ClubName),len))
		return -1;
	if(!stream.Read(pInfo.m_ExpLevel))
		return -1;
	if(!stream.Read(pInfo.m_LevelTime,sizeof(pInfo.m_LevelTime),len))
		return -1;

	char iscreateavarta=0;
	if( !stream.Read( iscreateavarta ))
		return -1;

	if( iscreateavarta )
	{
		if( 0 != _ChangeAvarat( & pInfo, & stream )) return -1;
	}

	PROTOCOL_READSTR(pInfo.headurl,len);
	PROTOCOL_READ(pInfo.recvcount);
	PROTOCOL_READ(pInfo.songcount);
	PROTOCOL_READSTR(pInfo.topsongname,len);
	PROTOCOL_READ(pInfo.topsongscore);

	return 0;
}
