#include "StdAfx.h"
#include "ShareMemoryThreadMgr.h"
#include "XHHead.h"
#include "../XHSMDllHelp.h"

#ifndef FALSE_BREAK
#define FALSE_BREAK(b) \
if (!(b))\
{\
	break;\
}
#endif//FALSE_BREAK


#ifndef DISPATCHMESSAGE
#define DISPATCHMESSAGE(cmd,pdata) \
if(CXHSMDllHelp::m_pDealMessage)\
{\
	CXHSMDllHelp::m_pDealMessage(cmd,(pdata));\
}
#endif//DELIVERMESSAGE

CShareMemoryThreadMgr::CShareMemoryThreadMgr(void)
: m_bC2PShareMemory( false )
, m_bP2CShareMemory( false )
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

//----------------------------------------------------------------------------
// 消息处理
void CShareMemoryThreadMgr::ProcessMessage( int id, void * pData )
{
	CSafePointer safePointer(pData);
	switch ( id )
	{
	case MSG_CREATESHAREMEMORY:
		{
			XHH::DoubleStringB* pdStr = (XHH::DoubleStringB *)pData;
			char * lpszName = pdStr->str1;
			char* exeDir = pdStr->str2;
			bool bNeed = pdStr->needstart;

			std::string strC2PName( XHH::C2P );
			strC2PName += lpszName;
			std::string strP2CName( XHH::P2C );
			strP2CName += lpszName;

			m_hC2PEvent = ::CreateEventA( NULL, FALSE, FALSE, strC2PName.c_str() );
			m_hP2CEvent = ::CreateEventA( NULL, FALSE, FALSE, strP2CName.c_str() );

			m_bC2PShareMemory = m_C2PShareMemory.Create( strC2PName.c_str() );
			m_bP2CShareMemory = m_P2CShareMemory.Create( strP2CName.c_str() );

			if(!bNeed)
				break;
			//唤起另一个进程
			char exe[260] = {0};
			sprintf_s(exe,259,"%s\\%s",exeDir,lpszName);
			STARTUPINFO stinfo; //启动窗口的信息 
			ZeroMemory(&stinfo,sizeof(stinfo));
			stinfo.cb = sizeof(stinfo);/*STARTF_FORCEONFEEDBACK*/
			CreateProcessA(NULL,exe,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,exeDir,&stinfo,&m_procinfo);
			break;
		}
	case MSG_XINGGUANG_CLOSE:
		{
			if(m_bC2PShareMemory)
			{
				FALSE_BREAK(m_C2PShareMemory.WriteInt(XHH::CMD_XINGGUANG_CLOSE));
				informClient();
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

	if ( !m_bP2CShareMemory )
	{
		Sleep( 50 );
		return;
	}

	if ( WAIT_OBJECT_0 != WaitForSingleObject( m_hP2CEvent, 50 ) )
	{
		return;
	}

	int id = 0;
	while ( m_P2CShareMemory.ReadInt( id ) )
	{
		char buf[260] = {0};
		sprintf_s(buf,"xhsmdll: reade int %d\n",id);
		OutputDebugStringA(buf);

		switch(id)
		{
		case XHH::CMD_HOST_DEFAULT:
			{
				m_P2CShareMemory.ReadInt(id,false);
				DISPATCHMESSAGE(XHH::CMD_HOST_DEFAULT,NULL);
				break;
			}
		case XHH::CMD_HOST_NOSING:
			{
				m_P2CShareMemory.ReadInt(id,false);
				DISPATCHMESSAGE(XHH::CMD_HOST_NOSING,NULL);
			}
		case XHH::CMD_HOST_REQUESTC:
			{
				m_P2CShareMemory.ReadInt( id ,false);
				DISPATCHMESSAGE(XHH::CMD_HOST_REQUESTC,NULL);
				break;
			}
		case XHH::CMD_HOST_LEAVE:
			{
				m_P2CShareMemory.ReadInt( id ,false);
				DISPATCHMESSAGE(XHH::CMD_HOST_LEAVE,NULL);
				break;
			}
		case XHH::CMD_HOST_MSG:
			{
				XHH::PMessage pm = {0};
				FALSE_BREAK(m_P2CShareMemory.ReadStruct(&pm));
				DISPATCHMESSAGE(XHH::CMD_HOST_MSG,(void*)&pm);
				break;
			}
		case XHH::CMD_HOST_SONG:
			{
				XHH::DoubleString ds = {0};
				FALSE_BREAK(m_P2CShareMemory.ReadStruct(&ds));
				DISPATCHMESSAGE(XHH::CMD_HOST_SONG,(void*)&ds);
				break;
			}
		case XHH::CMD_HOST_GRADE:
			{
				XHH::OtherGrade og = {0};
				FALSE_BREAK(m_P2CShareMemory.ReadStruct(&og));
				DISPATCHMESSAGE(XHH::CMD_HOST_GRADE,(void*)&og);
				break;
			}
		case XHH::CMD_HOST_VIDEO:
			{
				XHH::VideoInfo vi = {0};
				FALSE_BREAK(m_P2CShareMemory.ReadStruct(&vi));
				vi.name[259] = 0;
				char buf[260] = {0};
				sprintf(buf,"XHSMDll for debug: width: %d,height: %d \n",vi.width,vi.height);
				OutputDebugStringA(buf);
				DISPATCHMESSAGE(XHH::CMD_HOST_VIDEO,(void*)&vi);
				break;
			}
		default:
			{
				OutputDebugStringA("xhsmdll: unknow cmd\n");
				break;
			}
		}
	}
}
