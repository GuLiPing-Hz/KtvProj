#include "StdAfx.h"
#include "ShareMemoryThreadMgr.h"
#include "../XGSM.h"
#include <assert.h>

#ifndef FALSE_BREAK
#define FALSE_BREAK(b) \
if (!(b))\
{\
	assert(false);\
	OutputDebugStringA("share memory IO failed\n");\
	break;\
}
#endif//FALSE_BREAK
//MessageBoxA(NULL,"share memory IO failed","error",MB_OK|MB_ICONERROR);

CShareMemoryThreadMgr::CShareMemoryThreadMgr(void)
: m_bC2PShareMemory( false )
, m_bP2CShareMemory( false )
, m_bSmVideo(false)
, m_hC2PEvent( NULL )
, m_hP2CEvent( NULL )
{
	ZeroMemory(&m_procinfo,sizeof(m_procinfo));
}

CShareMemoryThreadMgr::~CShareMemoryThreadMgr(void)
{
	SAFE_CLOSEHANDLE(m_hC2PEvent);
	SAFE_CLOSEHANDLE(m_hP2CEvent);
	SAFE_CLOSEHANDLE(m_procinfo.hProcess);
	SAFE_CLOSEHANDLE(m_procinfo.hThread);
}

CShareMemoryThreadMgr & CShareMemoryThreadMgr::getSingleton()
{
	static CShareMemoryThreadMgr instance;
	return instance;
}

//----------------------------------------------------------------------------
// 消息处理
void CShareMemoryThreadMgr::ProcessMessage( int id, void * pData )
{
	CSafePointer safePointer(pData);
	switch ( id )
	{
	case MSG_OPENSHAREMEMORY:
		{
			XHH::SingleString* pdStr = (XHH::SingleString *)pData;
			char * lpszName = pdStr->str;

			std::string strC2PName( XHH::C2P );
			strC2PName += lpszName;
			std::string strP2CName( XHH::P2C );
			strP2CName += lpszName;

			m_hC2PEvent = ::OpenEventA( EVENT_ALL_ACCESS, FALSE, strC2PName.c_str() );
			m_hP2CEvent = ::OpenEventA( EVENT_ALL_ACCESS, FALSE, strP2CName.c_str() );

			m_bC2PShareMemory = m_C2PShareMemory.Open( strC2PName.c_str() );
			m_bP2CShareMemory = m_P2CShareMemory.Open( strP2CName.c_str() );
			m_bSmVideo = m_smVideo.Open(XHH::VIDEO);
			break;
		}
	case MSG_HOST_NOSING:
		{
			if (m_bP2CShareMemory)
			{
				FALSE_BREAK(m_P2CShareMemory.WriteInt(XHH::CMD_HOST_NOSING));
				informServer();
			}
			break;
		}
	case MSG_HOST_MSG:
		{
			XHH::PMessage* pPM = (XHH::PMessage*) pData;
			if (m_bP2CShareMemory)
			{
				pPM->cmd = XHH::CMD_HOST_MSG;
				FALSE_BREAK(m_P2CShareMemory.WriteStruct(pPM));
				informServer();
			}
			break;
		}
	case MSG_HOST_SONG:
		{
			XHH::DoubleString* pDS = (XHH::DoubleString*) pData;
			if(m_bP2CShareMemory)
			{
				pDS->cmd = XHH::CMD_HOST_SONG;
				FALSE_BREAK(m_P2CShareMemory.WriteStruct(pDS));
				informServer();
			}
			break;
		}
	case MSG_HOST_VIDEO:
		{
			XHH::VideoInfo* pVI = (XHH::VideoInfo*) pData;
			CSafePointer sp2(pVI?pVI->buffer:NULL);//释放视频数据
			if(m_bP2CShareMemory)
			{
				char buf[260] = {0};
				sprintf(buf,"XGSMDll for debug: width: %d,height: %d,name: %s \n",pVI->width,pVI->height,pVI->name);
				OutputDebugStringA(buf);
				pVI->cmd = XHH::CMD_HOST_VIDEO;
				if(pVI->size && pVI->buffer)
				{
					if(m_bSmVideo)
						FALSE_BREAK(m_smVideo.writeVideo(pVI->type,pVI->buffer,pVI->size));
				}
				FALSE_BREAK(m_P2CShareMemory.WriteStruct(pVI));
				informServer();
			}
			break;
		}
	case MSG_HOST_GRADE:
		{
			if(m_bP2CShareMemory)
			{
				XHH::OtherGrade* pOG = (XHH::OtherGrade*) pData;
				pOG->cmd = XHH::CMD_HOST_GRADE;
				FALSE_BREAK(m_P2CShareMemory.WriteStruct(pOG));
				informServer();
			}
			break;
		}
	case MSG_HOST_LEAVE_ROOM:
		{
			if (m_bP2CShareMemory)
			{
				FALSE_BREAK(m_P2CShareMemory.WriteInt(XHH::CMD_HOST_LEAVE));
				informServer();
			}
			break;
		}
	case MSG_HOST_REQC:
		{
			if(m_bP2CShareMemory)
			{
				FALSE_BREAK(m_P2CShareMemory.WriteInt(XHH::CMD_HOST_REQUESTC));
				informServer();
			}
			break;
		}
	case MSG_HOST_DEFAULT:
		{
			if (m_bP2CShareMemory)
			{
				FALSE_BREAK(m_P2CShareMemory.WriteInt(XHH::CMD_HOST_DEFAULT));
				informServer();
			}
			break;
		}
	default:
		{
			OutputDebugStringA("No such msg\n");
			break;
		}
	}
}

//----------------------------------------------------------------------------
// 空闲处理
void CShareMemoryThreadMgr::ProcessIdle()
{
	ReadShareMemory();
}

//----------------------------------------------------------------------------
// 读取ShareMemory
void CShareMemoryThreadMgr::ReadShareMemory()
{
	static char szReadBuffer[102400] = { 0 };
	static char szWriteBuffer[102400] = { 0 };

	if ( !m_bC2PShareMemory )
	{
		Sleep( 50 );
		return;
	}

	if ( WAIT_OBJECT_0 != WaitForSingleObject( m_hC2PEvent, 50 ) )
	{
		return;
	}

	int id = 0;
	while ( m_C2PShareMemory.ReadInt( id ) )
	{
		switch(id)
		{
		case XHH::CMD_XINGGUANG_CLOSE:
			{
				if(CXGSMHelp::m_pDealMessage)
					CXGSMHelp::m_pDealMessage(XHH::CMD_XINGGUANG_CLOSE,NULL);
				break;
			}
		default:
			{
				OutputDebugStringA("unknow cmd\n");
				break;
			}
		}
	}
}
