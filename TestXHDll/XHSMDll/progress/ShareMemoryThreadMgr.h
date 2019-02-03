#pragma once
#include "ThreadMgr.h"
#include "ShareMemory.h"

enum MSG_SHAEREMEMORY
{
	//���������ڴ� wparam (char*) lparam(NULL)
	MSG_CREATESHAREMEMORY = MSG_INVALID+1,
	MSG_XINGGUANG_CLOSE,
};

class CShareMemoryThreadMgr : public CThreadMgr
{
public:
	CShareMemoryThreadMgr(void);
	~CShareMemoryThreadMgr(void);

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
	bool			m_bC2PShareMemory;
	bool			m_bP2CShareMemory;
	HANDLE			m_hC2PEvent;
	HANDLE			m_hP2CEvent;

	PROCESS_INFORMATION m_procinfo; //���̵���Ϣ
};
