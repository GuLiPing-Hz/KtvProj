#include "stdafx.h"
#include "ShareMemory.h"

CShareMemory::CShareMemory()
: mhShareMem(NULL)
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);

	mnBufferSize = info.dwAllocationGranularity * 10;
	mnRealBufferSize = mnBufferSize - BUFFER_HEAD_SIZE;
}

CShareMemory::~CShareMemory()
{
	if ( mhShareMem )
	{
		CloseHandle( mhShareMem );
		mhShareMem = NULL;
	}
}

/*
 * 创建共享内存
 * param[in] : szName 共享内存名称
 * return : 操作是否成功
 */
bool CShareMemory::Create( const char * szName )
{
	TCHAR szFullName[MAX_PATH] = { 0 };
	_tcscpy_s( szFullName, _T("share_memory_") );
	_tcscat_s( szFullName, MAX_PATH - _tcslen(szFullName), szName );

	OutputDebug( "CShareMemory::Create [%s].", szName );

	mhShareMem = ::CreateFileMappingA( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, mnBufferSize, szFullName );
	if ( NULL == mhShareMem )
	{// 创建共享内存失败
		//writeLog("创建共享内存失败,ErrorCode[%d].", ::GetLastError());/*OutputDebug*/
		return false;
	}

	return true;
}

/*
 * 打开共享内存
 * param[in] : szName 共享内存名称
 * return : 操作是否成功
 */
bool CShareMemory::Open( const char * szName )
{
	TCHAR szFullName[MAX_PATH] = { 0 };
	_tcscpy_s( szFullName, _T("share_memory_") );
	_tcscat_s( szFullName, MAX_PATH - _tcslen(szFullName), szName );

	//OutputDebug( "CShareMemory::Open [%s].", szName );

	mhShareMem = OpenFileMappingA( FILE_MAP_ALL_ACCESS, FALSE, szFullName );
	if ( NULL == mhShareMem )
	{// 打开共享内存失败
		//OutputDebug( "打开共享内存失败,ErrorCode[%d].", ::GetLastError() );
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------
bool CShareMemory::ReadInt( int & v )
{
	return Read( &v, sizeof(int) );
}

bool CShareMemory::ReadFloat(float &v)
{
	return Read(&v,sizeof(float));
}

//----------------------------------------------------------------------------
bool CShareMemory::ReadString( std::string & v )
{
	bool ret = false;
	int l = 0;
	if ( Read( &l, sizeof(int) ) )
	{
		if ( (l < MAX_STRING_LENGTH)&&(l>0) )
		{
			char * p = new char[l];
			
			if ( Read( p, l ) )
			{
				v = std::string( p, l );
				ret = true;
			}

			delete []p;
			p = NULL;
		}
		else
		{
			//writeLog( "WriteString error, Invalid length value[%d].", l );
		}
	}
	return ret;
}

//----------------------------------------------------------------------------
bool CShareMemory::WriteInt( int v )
{
	return Write( &v, sizeof(int) );
}

bool CShareMemory::WriteFloat(float v)
{
	return Write(&v,sizeof(float));
}
//----------------------------------------------------------------------------
bool CShareMemory::WriteString( const std::string & v )
{
	int l = (int)v.length();
	if ( (l > MAX_STRING_LENGTH)||(l==0) )
	{
		//writeLog( "WriteString error, Invalid length value[%d].", l );
	}
	if ( Write( &l, sizeof(int) ) )
	{
		if ( Write( (void*)(v.c_str()), l ) )
		{
			return true;
		}
	}

	return false;
}

/*
 * 从共享内存中读取指定长度的数据
 * param[in out] : szReadBuffer 读取的buffer
 * param[in] : nNumberOfBytesToRead 读取的buffer长度
 * param[in out] : lpNumberOfBytesToRead 已读取的buffer长度
 * return : 操作是否成功
 */
void writeLog(const char *msg,...)
{
	if (msg == NULL)
	{
		return;
	}
	SYSTEMTIME st;
	GetLocalTime(&st);

	FILE * fp = fopen("F:\\dx.log","a");
	if (fp == NULL)
	{
		return;
	}
	fprintf(fp,"[%02d:%02d:%02d.%05d]:",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	va_list arg_ptr;
	//const char* str = msg;
	va_start(arg_ptr, msg);
	vfprintf(fp,msg,arg_ptr);
	va_end(arg_ptr);
	fprintf(fp,"\n");
	fclose(fp);
}

bool CShareMemory::Read( void * pReadBuffer, DWORD nNumberOfBytesToRead )
{
	char* pBuffer = (char*)MapViewOfFile( mhShareMem, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
	if ( NULL == pBuffer )
	{
		//writeLog( "CShareMemory::Read MapViewOfFile,ErrorCode[%d].", ::GetLastError() );
		return false;
	}

	bool ret = false;
	int nReadOffset = *((int*)pBuffer);
	int nWriteOffset = *(((int*)pBuffer)+1);

	writeLog( "CShareMemory::Read [%d][%d]", nReadOffset, nWriteOffset );

	if ( nReadOffset != nWriteOffset )
	{// buffer不为空
		int nCanReadBufferSize = 0;

		int nLast_Read_Size = mnRealBufferSize - nReadOffset;
		if ( nReadOffset > nWriteOffset )
		{
			nCanReadBufferSize = nLast_Read_Size + nWriteOffset;
		}
		else
		{
			nCanReadBufferSize = nWriteOffset - nReadOffset;
		}

		char * pRingBuffer = pBuffer+BUFFER_HEAD_SIZE;
		if ( nCanReadBufferSize >= nNumberOfBytesToRead )
		{
			if ( ( nLast_Read_Size ) >= nNumberOfBytesToRead )
			{
				memcpy( pReadBuffer, pRingBuffer + nReadOffset, nNumberOfBytesToRead );
			}
			else
			{
				memcpy( pReadBuffer, pRingBuffer + nReadOffset, nLast_Read_Size );
				memcpy( pReadBuffer, pRingBuffer, nNumberOfBytesToRead -  nLast_Read_Size  );
			}
			nReadOffset = ( nReadOffset + nNumberOfBytesToRead ) % mnRealBufferSize;
			*((int*)pBuffer) = nReadOffset;//保存下次读的位置
			ret = true;
		}
	}

	UnmapViewOfFile(pBuffer);

	return ret;
}

/*
 * 向共享内存写入指定长度的数据
 * param[in] : szWriteBuffer 写入的buffer
 * param[in] : nNumberOfBytesToWrite 写入的buffer长度
 * param[in out] : lpNumberOfBytesWritten 已写入的buffer长度
 * return : 操作是否成功
 */
bool CShareMemory::Write( void * pWriteBuffer, DWORD nNumberOfBytesToWrite )
{
	char* pBuffer = (char*)MapViewOfFile( mhShareMem, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
	if ( NULL == pBuffer )
	{
		//writeLog( "CShareMemory::Write MapViewOfFile,ErrorCode[%d].", ::GetLastError() );
		return false;
	}

	bool ret = false;
	int nReadOffset = *((int*)pBuffer);
	int nWriteOffset = *(((int*)pBuffer)+1);

	if ( (nWriteOffset+1)%mnRealBufferSize != nReadOffset )//(nWriteOffset != (nReadOffset-1))
	{// buffer没满
		int nCanWriteBufferSize = 0;
		int nLast_Write_Size = mnRealBufferSize - nWriteOffset;
		if ( nReadOffset > nWriteOffset )
		{
			nCanWriteBufferSize = nReadOffset - nWriteOffset;
		}
		else
		{
			nCanWriteBufferSize = nLast_Write_Size + nReadOffset;
		}

		char * pRingBuffer = pBuffer+BUFFER_HEAD_SIZE;
		if ( nCanWriteBufferSize > nNumberOfBytesToWrite )
		{
			if ( ( nLast_Write_Size ) >= nNumberOfBytesToWrite )
			{
				memcpy( pRingBuffer + nWriteOffset, pWriteBuffer, nNumberOfBytesToWrite );
			}
			else
			{
				memcpy( pRingBuffer + nWriteOffset, pWriteBuffer, nLast_Write_Size );
				char* pWriteBufferChar = (char*)pWriteBuffer;
				memcpy( pRingBuffer, pWriteBufferChar+nLast_Write_Size, nNumberOfBytesToWrite - nLast_Write_Size );
			}
			nWriteOffset = ( nWriteOffset + nNumberOfBytesToWrite ) % mnRealBufferSize;
			*((int*)pBuffer+1) = nWriteOffset;//保存下次写的位置
			ret = true;
		}
	}

	UnmapViewOfFile(pBuffer);

	return ret;
}

// 日志输出
void CShareMemory::OutputDebug(const char* pFmt, ...)
{
	char szLog[4096] = { 0 };
	va_list args;
	va_start(args, pFmt);
	_vsnprintf(szLog, sizeof(szLog), pFmt, args );
	va_end(args);

	strcat_s( szLog, "\r\n" );
	OutputDebugStringA( szLog );
}
