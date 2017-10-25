#ifndef PROTOCOLSTREAM__H__
#define PROTOCOLSTREAM__H__

/*
	注释添加以及修改于 2014-4-2 

	提供一个读写类，封装了对网络2进制数据的读写，
	PACKAGELEN_TYPE 指定长度的字节数（因为默认来着网络，所以net置为true）

	ReadStream 是一个读取的接口类，为了可以读取来自不同的内容，提供了readCom纯虚函数，以便自己实现
	WriteStream 是一个写入的接口类，为了可以写入到不同的地方，提供了writeCom纯虚函数，以便自己实现
*/

#include <string>
#include <string.h>

namespace NetworkUtil
{

	typedef enum _PACKAGELEN_TYPE
	{
		BINARY_PACKLEN_LEN = 2,
		TEXT_PACKLEN_LEN = 4
	}PACKAGELEN_TYPE;

#define PACKAGE_MAXLEN	 0xffff//65535

	class ReadStream
	{
	public:
		ReadStream(PACKAGELEN_TYPE htype_=BINARY_PACKLEN_LEN,bool net_=true):htype(htype_),net(net_){}
		virtual ~ReadStream(){}

		virtual bool readCom(/*out*/void* buffer,/*in*/unsigned int len_to_read,/*out*/unsigned int* len_readed) = 0;
		virtual unsigned int getSize() = 0;
		virtual const char* getData() = 0;
		virtual unsigned int tell() = 0;
		virtual bool isEmpty() const= 0;
		virtual bool isEnd() const= 0;
		//当peek为true时，只是查看下还有多少的内容，够不够offset的大小
		//当peek为false时，跳过指定的offset数据内容。
		virtual bool skip(int offset,bool peek=true)=0;

		bool Read(/* out */ int & i);
		bool Read(/* out */ short & i);
		bool Read(/* out */ char & c);
        //读取字符串，先读取字符串长度，然后根据长度读取到实际的字符串
		bool Read(/* out */ char* str,/* in */  unsigned int strlen, /* out */ unsigned int & len);
		unsigned int readAll(/* out */ char * szBuffer, /*in*/unsigned int iLen);
	private:
		bool readLength(unsigned int & len);
		bool readLengthWithoutOffset(unsigned int & outlen);
	private:
		bool							  net;
	public:
		PACKAGELEN_TYPE htype;
	};
	class WriteStream
	{
	public:
		WriteStream(PACKAGELEN_TYPE htype_=BINARY_PACKLEN_LEN,bool net_=true):htype(htype_),net(net_){}
		virtual ~WriteStream(){}
		bool Write(const char* str, unsigned int length);
		//第二个写入字符串的方法，只是没有写入长度，只有字符串
		bool writeWithoutLength(const char* str, unsigned int length);
		bool Write(int i);
		bool Write(short i);
		bool Write(char c);
		bool writeLength(unsigned int len);

		virtual bool writeCom(const void* buffer,unsigned int len_to_write,unsigned int* len_writed) = 0;
		virtual void flush() = 0;
		virtual const char* getData() const = 0;
		virtual unsigned int getSize() const = 0;
		virtual bool isValid() const = 0;
		//当peek为true时，只是查看下还有多少的内容，够不够offset的大小
		//当peek为false时，跳过指定的offset数据内容。
		virtual bool skip(int offset,bool peek=true)=0;
	protected:
		bool							  net;
		PACKAGELEN_TYPE htype;
	};

	class BinaryReadStream : public ReadStream
	{
	private:

		BinaryReadStream(const BinaryReadStream&);
		BinaryReadStream& operator=(const BinaryReadStream&);

	public:
		BinaryReadStream(const char* ptr_, unsigned int len_,PACKAGELEN_TYPE htype_=BINARY_PACKLEN_LEN);

		//确保buffer的内存大于len_to_read个字节
		virtual bool readCom(/*out*/void* buffer,/*in*/unsigned int len_to_read,/*out*/unsigned int* len_readed);

		virtual unsigned int getSize(){ return len;}
		virtual const char* getData(){return start;}
		virtual unsigned int tell() {return cur-start;};
		virtual bool isEmpty() const {return len<=(unsigned int)htype;}
		virtual bool isEnd() const {return cur == end;}
		virtual bool skip(int offset,bool peek=true);

	private:
		const char* const start;
		const unsigned int len;
		const char* cur;
		const char* end;
	};

	//网络数据的写入类，数据写入的起点是在第三个字节，所有short，int都会转成网络字节序
	//在flush的时候会计算整个stream的长度，写入头两个字节中（short）,必须调用flush
	//字符串的一般处理是先写入short类型的字符串长度，然后写入字符串： Write
	//提供不写入字符串长度的函数： WriteWithoutLength
	class BinaryWriteStream : public WriteStream
	{
	private:
		BinaryWriteStream(const BinaryWriteStream&);
		BinaryWriteStream& operator=(const BinaryWriteStream&);

	public:
		BinaryWriteStream(char* ptr, unsigned int len,PACKAGELEN_TYPE htype_=BINARY_PACKLEN_LEN);
		//确保buffer有len_to_write个字节的内容
		virtual bool writeCom(/*in*/const void* buffer,/*in*/unsigned int len_to_write,/*out*/unsigned int* len_writed);

		virtual bool skip(int offset,bool peek=false);
		virtual unsigned int getSize() const {return (unsigned int)(cur - start);}
		virtual bool isValid() const;
		//写入整个数据的有效长度,包含头两个字节
		virtual void flush();
		virtual const char* getData() const {return start;}
	private:
		char* const start;
		const unsigned int len;
		char*  cur;
		char*  end;
	};
}
#endif//PROTOCOLSTREAM__H__
