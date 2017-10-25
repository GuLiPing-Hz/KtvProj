#include "ThreadMgr.h"
#include "event_wrapper.h"
#include "NUtility.h"
#include "sleep.h"

CThreadMgr::CThreadMgr(void)
: m_bStop( false )
,m_pObjectCS(NULL)
,m_pObjectEvent(NULL)
,m_pObjectThread(NULL)
{
	m_pObjectCS = CriticalSectionWrapper::CreateCriticalSection();
	m_pObjectEvent = EventWrapper::Create();
}

CThreadMgr::~CThreadMgr(void)
{
	if(m_pObjectThread)
		delete m_pObjectThread;
	if(m_pObjectEvent)
		delete m_pObjectEvent;
	if(m_pObjectCS)
		delete m_pObjectCS;
}

bool CThreadMgr::CreateThread( ThreadPriority iPriority )
{
	if(m_pObjectThread)
		return true;

	m_pObjectThread = ThreadWrapper::CreateThread(ThreadProc,this,kNormalPriority,"Net Thread");
	if(!m_pObjectThread)
		return false;
	unsigned int thread_id;
	if(!m_pObjectThread->Start(thread_id))
	{
		return false;
	}
	return true;
}

bool CThreadMgr::ReleaseThread()
{
	m_bStop = true;
	if(m_pObjectThread)
		m_pObjectThread->Stop();
	return true;
}

void CThreadMgr::PostMessageOS( int id, void* pData )
{
	_tMsg msg = { false, id, pData };
	
	{
		CriticalSectionScoped lock(m_pObjectCS);
		m_lstMsg.push_back( msg );
	}
}

bool CThreadMgr::SendMessageOS( int id, void* pData )
{
	_tMsg msg = { true, id, pData };

	{
		CriticalSectionScoped lock(m_pObjectCS);
		m_lstMsg.push_front( msg );
	}
	

	if(m_pObjectEvent->Wait(UTIL_EVENT_INFINITE) == kEventSignaled)
	{
		return true;
	}

	return false;
}

void CThreadMgr::ProcessIdle()
{
	SleepMs(50);
}

bool CThreadMgr::ThreadProc(ThreadObj pData)
{
	CThreadMgr * pThis = static_cast<CThreadMgr *>(pData);
	return pThis->Run();
}

bool CThreadMgr::Run()
{
	while ( !m_bStop )
	{
		bool bIsEmpty = false;
		_tMsg msg = { false, MSG_INVALID, 0 };
		
		{
			CriticalSectionScoped lock(m_pObjectCS);
			bIsEmpty = m_lstMsg.empty();
			if ( !bIsEmpty )
			{
				msg = m_lstMsg.front();
				m_lstMsg.pop_front();
			}
		}

		
		if ( !bIsEmpty && msg.id != MSG_INVALID )//如果不为空，并且消息不为MSG_INVALID,
		{
			ProcessMessage(msg.id,msg.pData);
			if ( msg.isSend )
				m_pObjectEvent->Set();
		}
		else//空闲处理
		{
			ProcessIdle();
		}
	}

	return false;//结束线程
}
