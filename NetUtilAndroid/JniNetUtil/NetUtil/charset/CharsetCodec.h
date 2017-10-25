#ifndef CHARSET_CHARSET__H
#define CHARSET_CHARSET__H
#include <wchar.h>
#include <string.h>
#include <string>

class QChar 
{
public:
	enum SpecialCharacter {
		Null = 0x0000,
		Nbsp = 0x00a0,
		ReplacementCharacter = 0xfffd,
		ObjectReplacementCharacter = 0xfffc,
		ByteOrderMark = 0xfeff,
		ByteOrderSwapped = 0xfffe,
#ifdef QT3_SUPPORT
		null = Null,
		replacement = ReplacementCharacter,
		byteOrderMark = ByteOrderMark,
		byteOrderSwapped = ByteOrderSwapped,
		nbsp = Nbsp,
#endif
		ParagraphSeparator = 0x2029,
		LineSeparator = 0x2028
	};
	static inline unsigned char cell(unsigned short ucs) { return (unsigned char)(ucs & 0xff); }
	static inline unsigned char row(unsigned short ucs) { return (unsigned char)((ucs>>8)&0xff); }

	static inline bool isHighSurrogate(unsigned short ucs) {
		return ((ucs & 0xfc00) == 0xd800);
	}
	static inline bool isLowSurrogate(unsigned short ucs) {
		return ((ucs & 0xfc00) == 0xdc00);
	}

	static inline bool isHighSurrogate(unsigned int ucs4) {
		return ((ucs4 & 0xfffffc00) == 0xd800);
	}
	static inline bool isLowSurrogate(unsigned int ucs4) {
		return ((ucs4 & 0xfffffc00) == 0xdc00);
	}
	static inline bool requiresSurrogates(unsigned int ucs4) {
		return (ucs4 >= 0x10000);
	}
	static inline unsigned int surrogateToUcs4(unsigned short high, unsigned short low) {
		return (((unsigned int)(high))<<10) + low - 0x35fdc00;
	}
	static inline unsigned short highSurrogate(unsigned int ucs4) {
		return (unsigned short)((ucs4>>10) + 0xd7c0);
	}
	static inline unsigned short lowSurrogate(unsigned int ucs4) {
		return (unsigned short)(ucs4%0x400 + 0xdc00);
	}
};

class CCharsetCodec
{
	
public:
	CCharsetCodec(void){}
	virtual ~CCharsetCodec(void){}

	typedef enum _codecType{
		UTF_8,
		GBK,
		GB2312,
		GB18030,
	}CodecType;

	//检查UTF8字符串，因为java要求比较严格，如果检查到非UTF8字符串会导致崩溃
	static unsigned char CheckUtfChar(const char* bytes, const char** errorKind);
	//自己释放返回的字符编码器(delete),失败返回NULL
	static CCharsetCodec* codecForName(const char* name);
	static CCharsetCodec* codecForName(CodecType type);

	//必须要有0作为结束符，对于Android返回的unicode字符串是没有0结尾的，
	//所以不能用第一个convertFromUnicode方法
	static int caculateLength(const unsigned short* uc);
    //必须释放返回的字符串。(delete)
	unsigned short* convertToUnicode(const char *mbc){return convertToUnicode(mbc,mbc?(int)strlen(mbc):0);}
    //必须释放返回的字符串。(delete)
	char* convertFromUnicode(const unsigned short *uc){return convertFromUnicode(uc,caculateLength(uc));}

	//字符编码名字
	static  const char* name() {return "";}
	//字符编码表示ID
	static  int mibEnum() {return 0;}
	//需要自己释放(delete) 返回的指针,失败返回NULL
	virtual unsigned short* convertToUnicode(const char *, int) const = 0;
	//需要自己释放(delete) 返回的指针,失败返回NULL
	virtual char* convertFromUnicode(const unsigned short *, int) const = 0;

public:
	static void UninitCharset();
	static std::string Gbk2Utf8(const char* gbk);
	static std::string Utf82Gbk(const char* utf8);
};


#endif//CHARSET_CHARSET__H


