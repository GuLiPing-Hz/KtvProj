#include "CharsetCodec.h"
#include "Utf.h"
#include "Chinese.h"

   unsigned char CCharsetCodec::CheckUtfChar(const char* bytes, const char** errorKind)
   {
        while (*bytes != '\0') {
        	unsigned char utf8 = *(bytes++);
            // Switch on the high four bits.
            switch (utf8 >> 4) {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
                // Bit pattern 0xxx. No need for any extra bytes.
                break;
            case 0x08:
            case 0x09:
            case 0x0a:
            case 0x0b:
            case 0x0f:
                /*
                 * Bit pattern 10xx or 1111, which are illegal start bytes.
                 * Note: 1111 is valid for normal UTF-8, but not the
                 * modified UTF-8 used here.
                 */
                *errorKind = "start";
                return utf8;
            case 0x0e:
                // Bit pattern 1110, so there are two additional bytes.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    *errorKind = "continuation";
                    return utf8;
                }
                break;
                // Fall through to take care of the final byte.
            case 0x0c:
            case 0x0d:
                // Bit pattern 110x, so there is one additional byte.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    *errorKind = "continuation";
                    return utf8;
                }
                break;
            }
        }
        return 0;
    }

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

static CCharsetCodec* gPGbk = nullptr;
static CCharsetCodec* gPUtf8 = nullptr;

void InitCharset()
{
	if (!gPGbk)
		gPGbk = CCharsetCodec::codecForName(CCharsetCodec::GBK);
	if (!gPUtf8)
		gPUtf8 = CCharsetCodec::codecForName(CCharsetCodec::UTF_8);
}
void CCharsetCodec::UninitCharset()
{
	if (gPGbk){
		delete gPGbk;
		gPGbk = nullptr;
	}

	if (gPUtf8){
		delete gPUtf8;
		gPUtf8 = nullptr;
	}
}
std::string CCharsetCodec::Gbk2Utf8(const char* gbk)
{
	InitCharset();

	unsigned short* pU = gPGbk->convertToUnicode(gbk);
	if (!pU)
		return "";

	char* pG = gPUtf8->convertFromUnicode(pU);
	if (!pG){
		delete pU;
		return "";
	}

	std::string ret = pG;
	delete pG;
	delete pU;
	return ret;
}
std::string CCharsetCodec::Utf82Gbk(const char* utf8)
{
	InitCharset();

	unsigned short* pU = gPUtf8->convertToUnicode(utf8);
	if (!pU)
		return "";

	char* pG = gPGbk->convertFromUnicode(pU);
	if (!pG){
		delete pU;
		return "";
	}

	std::string ret = pG;
	delete pG;
	delete pU;
	return ret;
}
