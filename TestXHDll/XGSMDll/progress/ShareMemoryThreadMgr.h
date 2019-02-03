#pragma once
#include "ThreadMgr.h"
#include "ShareMemory.h"
#include "XHHead.h"

enum MSG_SHAEREMEMORY
{
	//���������ڴ� wparam (char*) lparam(NULL)
	MSG_OPENSHAREMEMORY = MSG_INVALID+1,
	MSG_HOST_VIDEO,
	MSG_HOST_GRADE,
	MSG_HOST_SONG,
	MSG_HOST_MSG,
	//MSG_HOST_ENTER_ROOM,//deprecated
	MSG_HOST_NOSING,
	MSG_HOST_LEAVE_ROOM,
	MSG_HOST_REQC,
	MSG_HOST_DEFAULT
};

class CShareMemoryThreadMgr : public CThreadMgr
{
public:
	CShareMemoryThreadMgr(void);
	~CShareMemoryThreadMgr(void);

	static CShareMemoryThreadMgr & getSingleton();

public:

public:
	// ��Ϣ����
	virtual void ProcessMessage( int id, void * pData );

	// ���д���
	virtual void ProcessIdle();

protected:
	// ��ȡShareMemory
	void ReadShareMemory();
	void informClient(){::SetEvent(m_hC2PEvent);}
	void informServer(){::SetEvent(m_hP2CEvent);}
private:
	CShareMemory	m_C2PShareMemory;
	CShareMemory	m_P2CShareMemory;
	bool						m_bC2PShareMemory;
	bool						m_bP2CShareMemory;
	HANDLE				m_hC2PEvent;
	HANDLE				m_hP2CEvent;

	CShareMemory  m_smVideo;
	bool						m_bSmVideo;

	PROCESS_INFORMATION m_procinfo; //���̵���Ϣ
};
