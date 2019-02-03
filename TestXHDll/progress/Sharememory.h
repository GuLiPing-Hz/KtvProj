#ifndef __SHAREMEMORY_H__
#define __SHAREMEMORY_H__

#include <string>

const int BUFFER_HEAD_SIZE = 8;//预留8个字节，前面4个字节保存读的偏移，后面4个字节保存写的偏移
//int MAX_STRING_LENGTH /*= 1048576*/;

class CSafePointer
{
public:
	CSafePointer(void* p=NULL):m_p(p){}
	~CSafePointer(){SAFE_DELETE(m_p);}

	operator void*()const{return m_p;}
private:
	void* m_p;
};

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

	bool ReadInt( int & v,bool bPeek=true );

	bool ReadFloat(float &v);

	bool ReadString( std::string & v );

	bool WriteInt( int v );

	bool WriteFloat(float v);

	bool WriteString( const std::string & v );

	template<typename T>
	bool ReadStruct( T *pData ) { return Read(pData,sizeof(T)); }

	template<typename T>
	bool WriteStruct( T *pData ) { return Write(pData,sizeof(T)); }

	// 从共享内存中读取指定长度的数据
	bool Read( void * pReadBuffer, DWORD nNumberOfBytesToRead,bool bPeek=false );

	// 向共享内存写入指定长度的数据
	bool Write( void * pWriteBuffer, DWORD nNumberOfBytesToWrite );

	bool readVideo(int type,void* pReadBuffer, DWORD len);
	bool writeVideo(int type,void* pWriteBuffer,DWORD len);
private:
	//指向共享内存的指针
	char*			m_pBuffer;
	HANDLE		m_hShareMem;
	DWORD		m_nBufferSize;
	DWORD		m_nRealBufferSize;
};

#endif	//__SHAREMEMORY_H__