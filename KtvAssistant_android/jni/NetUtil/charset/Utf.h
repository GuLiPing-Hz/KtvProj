#ifndef CHARSET_UTF__H__
#define CHARSET_UTF__H__

#include "CharsetCodec.h"

struct QUtf8
{
	//需要自己释放(delete) 返回的指针,失败返回NULL
	static unsigned short* convertToUnicode(const char *, int);
	//需要自己释放(delete) 返回的指针,失败返回NULL
	static char* convertFromUnicode(const unsigned short *, int);
};

class QUtf8Codec : public CCharsetCodec{
public:
	~QUtf8Codec(){}

	static const char* name(){
		return "UTF-8";
	}
	static int mibEnum(){
		return 106;
	}

	//需要自己释放 返回的指针,失败返回NULL
	virtual unsigned short* convertToUnicode(const char *, int) const;
	//需要自己释放 返回的指针,失败返回NULL
	virtual char* convertFromUnicode(const unsigned short *, int) const;
};

#endif//CHARSET_UTF__H__
