#include "stdafx.h"
#include "ShareMemory.h"
#include <assert.h>
#define  LOG_ERN0(x) //MessageBoxA(NULL,x,"ERROR",MB_ICONERROR|MB_OK)
#define  LOG_ERS2(x) //MessageBoxA(NULL,x,"ERROR",MB_ICONERROR|MB_OK)

#define SEPERATOR_SIZE 230400

CShareMemory::CShareMemory()
:m_hShareMem(NULL)
,m_pBuffer(NULL)
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);

	//m_nBufferSize = info.dwAllocationGranularity * 100;
	m_nBufferSize = 1048576;//400;for debug 5M
	m_nRealBufferSize = m_nBufferSize - BUFFER_HEAD_SIZE;
	//MAX_STRING_LENGTH = mnRealBufferSize;
}

CShareMemory::~CShareMemory()
{
	if (m_pBuffer)
	{
		UnmapViewOfFile(m_pBuffer);
		m_pBuffer = NULL;
	}
	SAFE_CLOSEHANDLE(m_hShareMem);
}

/*
 * ���������ڴ�
 * param[in] : szName �����ڴ�����
 * return : �����Ƿ�ɹ�
 */
bool CShareMemory::Create( const char * szName )
{
	TCHAR szFullName[MAX_PATH] = { 0 };
	_tcscpy_s( szFullName, _T("share_memory_") );
	_tcscat_s( szFullName, MAX_PATH - _tcslen(szFullName), szName );

	m_hShareMem = ::CreateFileMappingA( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, m_nBufferSize, szFullName );
	if ( NULL == m_hShareMem )
	{// ���������ڴ�ʧ��
		//w riteLog("[DXLOG CShareMemory::Create]:���������ڴ�ʧ��,ErrorCode[%d].", ::GetLastError());/*OutputDebug*/
		LOG_ERS2("create share memory failed,Errorcode",::GetLastError());
		return false;
	}
	if (m_pBuffer)
	{
		UnmapViewOfFile(m_pBuffer);
		m_pBuffer = NULL;
	}
	m_pBuffer = (char*)MapViewOfFile( m_hShareMem, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
	if (!m_pBuffer)
	{
		//w riteLog( "[DXLOG CShareMemory::Create]: MapViewOfFile,ErrorCode[%d].", ::GetLastError() );
		LOG_ERS2("map vide of file failed,Errorcode",::GetLastError());
		return false;
	}
	*((int*)m_pBuffer) = 0;
	*((int*)m_pBuffer+1) = 0;

	return true;
}

/*
 * �򿪹����ڴ�
 * param[in] : szName �����ڴ�����
 * return : �����Ƿ�ɹ�
 */
bool CShareMemory::Open( const char * szName )
{
	TCHAR szFullName[MAX_PATH] = { 0 };
	_tcscpy_s( szFullName, _T("share_memory_") );
	_tcscat_s( szFullName, MAX_PATH - _tcslen(szFullName), szName );

	//OutputDebug( "CShareMemory::Open [%s].", szName );

	m_hShareMem = OpenFileMappingA( FILE_MAP_ALL_ACCESS, FALSE, szFullName );
	if ( NULL == m_hShareMem )
	{// �򿪹����ڴ�ʧ��
		//OutputDebug( "�򿪹����ڴ�ʧ��,ErrorCode[%d].", ::GetLastError() );
		LOG_ERS2("open share memory failed,ErrorCode",::GetLastError());
		LOG_ERN0(szFullName);
		return false;
	}
	if (m_pBuffer)
	{
		UnmapViewOfFile(m_pBuffer);
		m_pBuffer = NULL;
	}
	m_pBuffer = (char*) MapViewOfFile(m_hShareMem,FILE_MAP_ALL_ACCESS,0,0,0);
	if (!m_pBuffer)
	{
		//w riteLog( "[DXLOG CShareMemory::Create]: MapViewOfFile,ErrorCode[%d].", ::GetLastError() );
		LOG_ERS2("map view of file failed,ErrorCode",::GetLastError());
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------
bool CShareMemory::ReadInt( int & v,bool bPeek )
{
	return Read( &v, sizeof(int),bPeek );
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
		if ( l>0 &&l < m_nRealBufferSize/*MAX_STRING_LENGTH*/ )
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
			//w riteLog( "[DXLOG CShareMemory::ReadString]:WriteString error, Invalid length value[%d].", l );
			LOG_ERS2("WriteString error, Invalid length value",l);
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
	if ( (l > m_nRealBufferSize)||(l==0)/*MAX_STRING_LENGTH*/ )
	{
		LOG_ERS2("WriteString error, Invalid length value",l);
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
 * �ӹ����ڴ��ж�ȡָ�����ȵ�����
 * param[in out] : szReadBuffer ��ȡ��buffer
 * param[in] : nNumberOfBytesToRead ��ȡ��buffer����
 * param[in out] : lpNumberOfBytesToRead �Ѷ�ȡ��buffer����
 * return : �����Ƿ�ɹ�
 */
bool CShareMemory::Read( void * pReadBuffer, DWORD nNumberOfBytesToRead,bool bPeek )
{
	OutputDebugStringA("xhsmdll read\n");
	if (!pReadBuffer)
	{
		return false;
	}

	if (!m_pBuffer)
	{
		LOG_ERN0("the address of the sharedmemory is NULL");
		return false;
	}

	bool ret = false;
	int nReadOffset = *((int*)m_pBuffer);
	int nWriteOffset = *(((int*)m_pBuffer)+1);
	char buf[260] = {0};
	sprintf(buf,"XHSMDll CShareMemory::Read ReadOffset : %d,WriteOffset : %d\n",nReadOffset,nWriteOffset);
	OutputDebugStringA(buf);
	if ( nReadOffset != nWriteOffset )//��Ϊ����ȵ�ʱ����Ƿǿ�
	{// buffer��Ϊ��
		int nCanReadBufferSize = 0;

		int nLast_Read_Size = m_nRealBufferSize - nReadOffset;//���ɶ�����
		if ( nReadOffset > nWriteOffset )
		{//���1
			nCanReadBufferSize = nLast_Read_Size + nWriteOffset;
		}
		else
		{//���2
			nCanReadBufferSize = nWriteOffset - nReadOffset;
			nLast_Read_Size = nCanReadBufferSize;//�޸ĺ��ɶ����ֵ�ֵ
		}

		char * pRingBuffer = m_pBuffer+BUFFER_HEAD_SIZE;
		if ( nCanReadBufferSize >= nNumberOfBytesToRead )
		{
			if ( nLast_Read_Size >= nNumberOfBytesToRead )
			{//���2 ������������в���
				memcpy( pReadBuffer, pRingBuffer + nReadOffset, nNumberOfBytesToRead );
			}
			else
			{//���1 ���ܻ����������
				memcpy( pReadBuffer, pRingBuffer + nReadOffset, nLast_Read_Size );
				char* pReadBufferChar = (char*)pReadBuffer;
				memcpy( pReadBufferChar+nLast_Read_Size, pRingBuffer, nNumberOfBytesToRead -  nLast_Read_Size  );
			}
			if(!bPeek)
			{
				nReadOffset = ( nReadOffset + nNumberOfBytesToRead ) % m_nRealBufferSize;
				*((int*)m_pBuffer) = nReadOffset;//�����´ζ���λ��
			}
			ret = true;
		}
		else
		{
			LOG_ERN0("the ring buffer doesn't have enough buffer to read");
			OutputDebugStringA("the ring buffer doesn't have enough buffer to read\n");
		}
	}

	return ret;
}

/*
 * �����ڴ�д��ָ�����ȵ�����
 * param[in] : szWriteBuffer д���buffer
 * param[in] : nNumberOfBytesToWrite д���buffer����
 * param[in out] : lpNumberOfBytesWritten ��д���buffer����
 * return : �����Ƿ�ɹ�
 */
bool CShareMemory::Write( void * pWriteBuffer, DWORD nNumberOfBytesToWrite )
{
	if (!pWriteBuffer)
	{
		return false;
	}

	if (!m_pBuffer)
	{
		LOG_ERN0("the address of the sharedmemory is NULL");
		return false;
	}

	bool ret = false;
	int nReadOffset = *((int*)m_pBuffer);
	int nWriteOffset = *(((int*)m_pBuffer)+1);

	int nFutureWriteOffset = nWriteOffset+nNumberOfBytesToWrite;//�µ�Write Offset

	if (((nWriteOffset < nReadOffset) && (nFutureWriteOffset >=nReadOffset))//�����д��ȣ��������������ʱ����Ϊ�ǿյ��쳣�����Բ�����ȡ�
		||((nWriteOffset > nReadOffset) 
			&& ((nFutureWriteOffset/m_nRealBufferSize >=2)//��ֹ��������buffer��д�����
				|| (((nFutureWriteOffset/m_nRealBufferSize) == 1 )&& ((nFutureWriteOffset%m_nRealBufferSize) >= nReadOffset)) )))
	{
		LOG_ERN0("the ring buffer doesn't have enough buffer to write");
		return ret;
	}

	{// buffer����
		int nCanWriteBufferSize = 0;
		int nLast_Write_Size = m_nRealBufferSize - nWriteOffset;//����д����
		if ( nReadOffset > nWriteOffset )
		{//���1
			nCanWriteBufferSize = nReadOffset - nWriteOffset;
			nLast_Write_Size = nCanWriteBufferSize;//�޸ĺ���д����
		}
		else
		{//���2
			nCanWriteBufferSize = nLast_Write_Size + nReadOffset;
		}

		char * pRingBuffer = m_pBuffer+BUFFER_HEAD_SIZE;
		if ( nCanWriteBufferSize >= nNumberOfBytesToWrite )
		{
			if ( nLast_Write_Size >= nNumberOfBytesToWrite )
			{//���1 �������������
				memcpy( pRingBuffer + nWriteOffset, pWriteBuffer, nNumberOfBytesToWrite );
			}
			else
			{//���2 ���ܻ����������
				memcpy( pRingBuffer + nWriteOffset, pWriteBuffer, nLast_Write_Size );
				char* pWriteBufferChar = (char*)pWriteBuffer;
				memcpy( pRingBuffer, pWriteBufferChar+nLast_Write_Size, nNumberOfBytesToWrite - nLast_Write_Size );//modified by glp
			}
			nWriteOffset = ( nWriteOffset + nNumberOfBytesToWrite ) % m_nRealBufferSize;
			*((int*)m_pBuffer+1) = nWriteOffset;//�����´�д��λ��
			ret = true;
		}
		else
		{
			LOG_ERN0("the ring buffer is too small!");
		}
	}

	return ret;
}

bool CShareMemory::readVideo(int type,void* pReadBuffer, DWORD len)
{
	if (!pReadBuffer)
	{
		return false;
	}

	if (!m_pBuffer)
	{
		LOG_ERN0("the address of the sharedmemory is NULL");
		return false;
	}

	int nReadFlag;
	if(type == 0)
	{
		nReadFlag = *((int*)m_pBuffer);

		if(nReadFlag == 0)
		{
			memcpy(pReadBuffer,m_pBuffer+BUFFER_HEAD_SIZE,len);
			*((int*)m_pBuffer) = 1;
		}
		else
		{
			memcpy(pReadBuffer,m_pBuffer+BUFFER_HEAD_SIZE+SEPERATOR_SIZE,len);
			*((int*)m_pBuffer) = 0;
		}
	}
	else
	{
		nReadFlag = *((int*)m_pBuffer+1);
		static int nOffset = BUFFER_HEAD_SIZE+SEPERATOR_SIZE*2;
		if(nReadFlag == 0)
		{
			memcpy(pReadBuffer,m_pBuffer+nOffset,len);
			*((int*)m_pBuffer+1) = 1;
		}
		else
		{
			memcpy(pReadBuffer,m_pBuffer+nOffset+SEPERATOR_SIZE,len);
			*((int*)m_pBuffer+1) = 0;
		}
	}
	Sleep(5);
	return true;

}

bool CShareMemory::writeVideo(int type,void* pWriteBuffer,DWORD len)
{
	assert (SEPERATOR_SIZE >= len);
	if (!pWriteBuffer)
	{
		return false;
	}

	if (!m_pBuffer)
	{
		LOG_ERN0("the address of the sharedmemory is NULL");
		return false;
	}

	int nReadFlag;
	if (type == 0)
	{
		nReadFlag = *((int*)m_pBuffer);
		if(nReadFlag == 0)
		{
			memcpy(m_pBuffer+BUFFER_HEAD_SIZE+SEPERATOR_SIZE,pWriteBuffer,len);
		}
		else
		{
			memcpy(m_pBuffer+BUFFER_HEAD_SIZE,pWriteBuffer,len);
		}
	}
	else
	{
		nReadFlag = *((int*)m_pBuffer+1);
		static int nOffset = BUFFER_HEAD_SIZE+SEPERATOR_SIZE*2;
		if(nReadFlag == 0)
		{
			memcpy(m_pBuffer+nOffset+SEPERATOR_SIZE,pWriteBuffer,len);
		}
		else
		{
			memcpy(m_pBuffer+nOffset,pWriteBuffer,len);
		}
	}

	return true;
}


