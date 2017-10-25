#ifndef CHARSET_CHINESE__H__
#define CHARSET_CHINESE__H__

#include "CharsetCodec.h"

class QGb18030Codec : public CCharsetCodec {
public:
	QGb18030Codec(){};

	//字符编码名字
	static  const char* name() { return "GB18030"; }
	//字符编码表示ID
	static  int mibEnum() { return 114; }
	//需要自己释放 返回的指针,失败返回NULL
	virtual unsigned short* convertToUnicode(const char *, int) const;
	//需要自己释放 返回的指针,失败返回NULL
	virtual char* convertFromUnicode(const unsigned short *, int) const;

	//QString convertToUnicode(const char *, int, ConverterState *) const;
	//QByteArray convertFromUnicode(const QChar *, int, ConverterState *) const;
};

class QGbkCodec : public QGb18030Codec {
public:
	QGbkCodec():QGb18030Codec(){};

	//字符编码名字
	static  const char* name(){return "GBK";}
	//字符编码表示ID
	static  int mibEnum(){return 113;}
	//需要自己释放 返回的指针,失败返回NULL
	virtual unsigned short* convertToUnicode(const char *, int) const;
	//需要自己释放 返回的指针,失败返回NULL
	virtual char* convertFromUnicode(const unsigned short *, int) const;
};

class QGb2312Codec : public QGb18030Codec {
public:
	QGb2312Codec():QGb18030Codec(){};

	//字符编码名字
	static  const char* name(){return "GB2312";}
	//字符编码表示ID
	static  int mibEnum(){return 2025;}
	//需要自己释放 返回的指针,失败返回NULL
	virtual unsigned short* convertToUnicode(const char *, int) const;
	//需要自己释放 返回的指针,失败返回NULL
	virtual char* convertFromUnicode(const unsigned short *, int) const;
};

#endif//CHARSET_CHINESE__H__

