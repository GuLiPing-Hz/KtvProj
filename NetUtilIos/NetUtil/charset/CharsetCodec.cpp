#include "CharsetCodec.h"
#include "Utf.h"
#include "Chinese.h"

CCharsetCodec* CCharsetCodec::codecForName(const char* name)
{
	if(!name)
		return NULL;

	if(strcmp(QUtf8Codec::name(),name) == 0)
		return new QUtf8Codec();
	else if(strcmp(QGb2312Codec::name(),name) == 0)
		return new QGb2312Codec();
	else if(strcmp(QGbkCodec::name(),name) == 0)
		return new QGbkCodec();
	else if(strcmp(QGb18030Codec::name(),name) == 0)
		return new QGb18030Codec();
	//not implement
	return NULL;
}

CCharsetCodec* CCharsetCodec::codecForName(CodecType type)
{
	char name[260] = {0};
	switch(type)
	{
	case UTF_8:
		strcpy(name,"UTF-8");
		break;
	case GB2312:
		strcpy(name,"GB2312");
		break;
	case GBK:
		strcpy(name,"GBK");
		break;
	case GB18030:
		strcpy(name,"GB18030");
		break;
	}

	return codecForName(name);
}

int CCharsetCodec::caculateLength(const unsigned short* uc)
{
	int count = 0;
	if(!uc)
		return count;


	const unsigned short* p = uc;
	while((*p) != 0)
	{
		count++;
		p++;
	}
	return count;
}
