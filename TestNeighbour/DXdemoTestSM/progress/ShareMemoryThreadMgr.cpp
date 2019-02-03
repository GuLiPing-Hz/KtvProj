#include "StdAfx.h"
#include "ShareMemoryThreadMgr.h"
#include "processcommunicate.h"

extern HWND			g_hWnd;

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

CShareMemoryThreadMgr & CShareMemoryThreadMgr::getSingleton()
{
	static CShareMemoryThreadMgr instance;
	return instance;
}

//----------------------------------------------------------------------------
// 消息处理
void CShareMemoryThreadMgr::ProcessMessage( int id, void * wParam, void * lParam )
{
	switch ( id )
	{
	case MSG_CREATESHAREMEMORY:
		{
			char * lpszName = (char*)wParam;

			std::string strC2PName( C2P );
			strC2PName += lpszName;
			std::string strP2CName( P2C );
			strP2CName += lpszName;

			m_hC2PEvent = ::CreateEventA( NULL, FALSE, FALSE, strC2PName.c_str() );
			m_hP2CEvent = ::CreateEventA( NULL, FALSE, FALSE, strP2CName.c_str() );

			m_bC2PShareMemory = m_C2PShareMemory.Create( strC2PName.c_str() );
			m_bP2CShareMemory = m_P2CShareMemory.Create( strP2CName.c_str() );
			//writeLog("[CShareMemoryThreadMgr::ProcessMessage] MSG_CREATESHAREMEMORY : C2S[%s] S2C[%s].", (m_bC2PShareMemory ? "TRUE" : "FALSE"), (m_bP2CShareMemory ? "TRUE" : "FALSE") );
			break;
			//唤起另一个进程
			STARTUPINFO stinfo; //启动窗口的信息 
			ZeroMemory(&stinfo,sizeof(stinfo));
			stinfo.cb = sizeof(stinfo);/*STARTF_FORCEONFEEDBACK*/
			CreateProcess(NULL,"C:\\Users\\Administrator\\Documents\\Visual Studio 2005\\Projects\\DXDPdemo\\debug\\xingguang.exe"
				,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL
				,"C:\\Users\\Administrator\\Documents\\Visual Studio 2005\\Projects\\DXDPdemo\\debug"
				,&stinfo,&m_procinfo);
			//m_procinof 中的句柄需要关闭
			break;
		}
	case MSG_SOUND:
		{
			int* pInt = (int*)wParam;
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_ATMOSPHERE);
				m_C2PShareMemory.WriteInt(*pInt);
				::SetEvent(m_hC2PEvent);
			}
			delete pInt;
			break;
		}
	case MSG_NEXTSONG:
		{
			char* pNextSong = (char*)wParam;
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_NEXT_SONGNAME);
				m_C2PShareMemory.WriteString(pNextSong);
				::SetEvent(m_hC2PEvent);
			}
			delete pNextSong;
			break;
		}
	case MSG_NOFIREWARNING:
		{
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_NOFIREWARNING);
				::SetEvent(m_hC2PEvent);
			}
			break;
		}
	case MSG_FIREWARNING:
		{
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_FIREWARNING);
				::SetEvent(m_hC2PEvent);
			}
			break;
		}
	case MSG_MESSAGE:
		{
			char* pMessage = (char*)wParam;
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_MESSAGE);
				m_C2PShareMemory.WriteString(pMessage);
				::SetEvent(m_hC2PEvent);
			}
			delete pMessage;
			break;
		}
	case MSG_MUTE_OPEN:
		{
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_MUTE_OPEN);
				::SetEvent(m_hC2PEvent);
			}
			break;
		}
	case MSG_MUTE_CLOSE:
		{
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_MUTE_CLOSE);
				::SetEvent(m_hC2PEvent);
			}
			break;
		}
	case MSG_SET_SUB_TONE:
		{
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_SUBTONEVALUE);
				::SetEvent(m_hC2PEvent);
			}
			break;
		}
	case MSG_SET_ADD_TONE:
		{
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_ADDTONEVALUE);
				::SetEvent(m_hC2PEvent);
			}
			break;
		}
	case MSG_SET_VOLUME:
		{
			int *pInt = (int*)wParam;
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_SETMUSICVOLUME);
				m_C2PShareMemory.WriteInt(*pInt);
				::SetEvent(m_hC2PEvent);
			}
			if (pInt)
			{
				delete pInt;
			}
			break;
		}
	case MSG_SWITCH_SONG:
		{
			char *pfileName = (char*)wParam;
			unsigned int *pstaff_pos_height = (unsigned int*)lParam;
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_SWITCHSONG);
				m_C2PShareMemory.WriteString(pfileName);
				m_C2PShareMemory.WriteInt(*pstaff_pos_height);
				::SetEvent(m_hC2PEvent);//通知另一个进程
			}
			else
			{
				//writeLog("CShareMemoryThreadMgr::ProcessMessage m_bP2CShareMemory is false");
			}
			if (pfileName)
			{
				delete pfileName;
			}
			if (pstaff_pos_height)
			{
				delete pstaff_pos_height;
			}
			break;
		}
	case MSG_PLAY_GRADE:
		{
			int* pInt = (int*)wParam;
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_GRADE);
				m_C2PShareMemory.WriteInt(*pInt);
				::SetEvent(m_hC2PEvent);
			}
			if (pInt)
			{
				delete pInt;
			}
			break;
		}
	case MSG_PLAY_CONTROL_PAUSE:
		{
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_CONTROL_PAUSE);
				::SetEvent(m_hC2PEvent);
			}
			break;
		}
	case MSG_PLAY_CONTROL_RESUME:
		{
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_CONTROL_RESUME);
				::SetEvent(m_hC2PEvent);
			}
			break;
		}
	case MSG_PLAY_ACCOMPANY:
		{
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_ACCOMPANY);
				::SetEvent(m_hC2PEvent);
			}
			break;
		}
	case MSG_DXPLAYER_CLOSE:
		{
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_CLOSE);
				::SetEvent(m_hC2PEvent);
			}
			break;
		}
	case MSG_SONG_STARTEND:
		{
			int *psong_start = (int*)wParam;
			int *psong_end = (int*)lParam;
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_SONGSTARTEND);
				m_C2PShareMemory.WriteInt(*psong_start);
				m_C2PShareMemory.WriteInt(*psong_end);
				::SetEvent(m_hC2PEvent);
			}
			if (psong_start)
			{
				delete psong_start;
			}
			if (psong_end)
			{
				delete psong_end;
			}
			break;
		}
	case MSG_SONG_INFO:
		{
			char *psingerName = (char*)wParam;
			char *psongName = (char*)lParam;
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_SONGINFO);
				m_C2PShareMemory.WriteString(psingerName);
				m_C2PShareMemory.WriteString(psongName);
				::SetEvent(m_hC2PEvent);
			}
			if (psingerName)
			{
				delete psingerName;
			}
			if (psongName)
			{
				delete psongName;
			}
			break;
		}
	case MSG_SINGER_PIC:
		{
			char* psingerpic = (char*)wParam;
			if (m_bC2PShareMemory)
			{
				m_C2PShareMemory.WriteInt(CMD_DXPLAYER_SINGERPIC);
				m_C2PShareMemory.WriteString(psingerpic);
				::SetEvent(m_hC2PEvent);
			}
			if (psingerpic)
			{
				delete psingerpic;
			}
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

	if ( !m_bC2PShareMemory || !g_hWnd )
	{
		Sleep( 50 );
		return;
	}

	if ( WAIT_OBJECT_0 != WaitForSingleObject( m_hP2CEvent, 50 ) )
	{
		return;
	}

	//writeLog("ProcessIdle" );

	int id = 0;
	std::string strFuncName;

// 	if (!m_C2PShareMemory.ReadInt( id ))
// 	{
// 		Sleep(50);
// 		return ;
// 	}
// 	do
	while ( m_P2CShareMemory.ReadInt( id ) )
	{
		char szLog[4096] = { 0 };
		sprintf_s( szLog, "m_C2SShareMemory.ReadInt[%d].\r\n", id );
		OutputDebugStringA( szLog );

		switch(id)
		{
		case CMD_DXPLAYER_ENDSONG:
			{
				::PostMessage(g_hWnd,WM_DXPLAYER_ENDSONG,0,0);
				break;
			}
		case CMD_DXPLAYER_SCORE_STARNUM:
			{
				int *nScore = new int;
				int *nStarNum = new int;
				if (m_P2CShareMemory.ReadInt(*nScore))
				{
					if (m_P2CShareMemory.ReadInt(*nStarNum))
					{
						::PostMessage(g_hWnd,WM_DXPLAYER_SCORE_STARNUM,(WPARAM)nScore,(LPARAM)nStarNum);
					}
				}
			}
		}
	}
}
