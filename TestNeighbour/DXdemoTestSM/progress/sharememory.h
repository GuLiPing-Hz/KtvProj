#ifndef __SHAREMEMORY_H__
#define __SHAREMEMORY_H__

//#include "../Opengl.h"
#include <string>

const int BUFFER_HEAD_SIZE = 8;//预留8个字节，前面4个字节保存读的偏移，后面4个字节保存写的偏移
const int MAX_STRING_LENGTH = 1048576;

class CShareMemory
{
public:
	CShareMemory();
	~CShareMemory();

public:
	// 创建共享内存
	bool Create( const char * szName );

	// 打开共享内存
	bool Open( const char * szName );

	bool ReadInt( int & v );

	bool ReadFloat(float &v);

	bool ReadString( std::string & v );

	bool WriteInt( int v );

	bool WriteFloat(float v);

	bool WriteString( const std::string & v );
protected:
	
	// 从共享内存中读取指定长度的数据
	bool Read( void * pReadBuffer, DWORD nNumberOfBytesToRead );
	
	// 向共享内存写入指定长度的数据
	bool Write( void * pWriteBuffer, DWORD nNumberOfBytesToWrite );

protected:
	// 日志输出
	void OutputDebug(const char* pFmt, ...);

private:
	HANDLE		mhShareMem;
	DWORD		mnBufferSize;
	DWORD		mnRealBufferSize;
};

#endif	//__SHAREMEMORY_H__