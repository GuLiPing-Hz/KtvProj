#pragma once

#include <list>
/*#include "../Opengl.h"*/

enum
{
	MSG_INVALID = 0,
};

struct _tMsg//������Ϣ�ṹ
{
	bool		isSend;//�Ƿ���������
	int		id;
	void *	pData;
};

class CThreadMgr
{
public:
	CThreadMgr(void);
	~CThreadMgr(void);

public:

public:
//�����̣߳�ָ�����ȼ�
	bool Create(int iPriority = THREAD_PRIORITY_NORMAL);
//��Դ����
	bool Release();
//Ͷ����Ϣ
	void PostMessage( int id, void * pData );
//������Ϣ
	bool SendMessage( int id, void * pData );
//��Ϣ����
	virtual void ProcessMessage( int id, void * pData ) = 0;
//���д���
	virtual void ProcessIdle();

public:
	bool SetPriority(int iPriority) const;//�����߳����޼�

	int GetPriority() const;//��ȡ�̵߳����ȼ�

	bool Suspend();//��ͣ�߳�

	bool Resume();//�����߳�

	bool IsSuspended() const;//�߳��Ƿ���ͣ

	bool IsRunning() const;//�߳��Ƿ�������

	bool GetExitCode(DWORD * pExitCode) const;//��ȡ�˳���

#if(WINVER >= 0x0500)
	//��ȡ�߳�ʱ����Ϣ
	bool GetThreadTimes(LPFILETIME lpCreationTime, LPFILETIME lpExitTime, LPFILETIME lpKernelTime, LPFILETIME lpUserTime) const;
#endif

protected:
//�ȴ��߳̽���
	bool WaitForThread(DWORD dwTimeout = INFINITE) const;
//��ֹ�߳�
	bool Terminate(DWORD dwExitCode = 0) const;
//�̺߳���
	static DWORD WINAPI ThreadProc(LPVOID pData);
//�߳�run����
	DWORD Run();

protected:
	volatile bool				m_bStop;

private:
	HANDLE						m_hThread;
	bool								m_bSuspended;

	HANDLE						m_hSendEvent;

	CRITICAL_SECTION		m_lstcs;
	std::list<_tMsg>			m_lstMsg;
};
