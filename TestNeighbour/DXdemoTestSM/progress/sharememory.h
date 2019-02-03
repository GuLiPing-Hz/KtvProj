#ifndef __SHAREMEMORY_H__
#define __SHAREMEMORY_H__

//#include "../Opengl.h"
#include <string>

const int BUFFER_HEAD_SIZE = 8;//Ԥ��8���ֽڣ�ǰ��4���ֽڱ������ƫ�ƣ�����4���ֽڱ���д��ƫ��
const int MAX_STRING_LENGTH = 1048576;

class CShareMemory
{
public:
	CShareMemory();
	~CShareMemory();

public:
	// ���������ڴ�
	bool Create( const char * szName );

	// �򿪹����ڴ�
	bool Open( const char * szName );

	bool ReadInt( int & v );

	bool ReadFloat(float &v);

	bool ReadString( std::string & v );

	bool WriteInt( int v );

	bool WriteFloat(float v);

	bool WriteString( const std::string & v );
protected:
	
	// �ӹ����ڴ��ж�ȡָ�����ȵ�����
	bool Read( void * pReadBuffer, DWORD nNumberOfBytesToRead );
	
	// �����ڴ�д��ָ�����ȵ�����
	bool Write( void * pWriteBuffer, DWORD nNumberOfBytesToWrite );

protected:
	// ��־���
	void OutputDebug(const char* pFmt, ...);

private:
	HANDLE		mhShareMem;
	DWORD		mnBufferSize;
	DWORD		mnRealBufferSize;
};

#endif	//__SHAREMEMORY_H__