#pragma once

#include <list>
/*#include "../Opengl.h"*/

enum
{
	MSG_INVALID = 0,
};

struct _tMsg//定义消息结构
{
	bool		isSend;//是否立即发送
	int		id;
	void *	wParam;
	void *	lParam;

	_tMsg & operator =( const _tMsg & msg )
	{
		if ( this == &msg )
			return *this;

		isSend = msg.isSend;
		id = msg.id;
		wParam = msg.wParam;
		lParam = msg.lParam;

		return *this;
	}
};

class CThreadMgr
{
public:
	CThreadMgr(void);
	~CThreadMgr(void);

public:

public:
//创建线程，指定优先级
	bool Create(int iPriority = THREAD_PRIORITY_NORMAL);
//资源回收
	bool Release();
//投递消息
	void PostMessage( int id, void * lParam, void * rParam );
//发送消息
	bool SendMessage( int id, void * lParam, void * rParam );
//消息处理
	virtual void ProcessMessage( int id, void * lParam, void * rParam ) = 0;
//空闲处理
	virtual void ProcessIdle();

public:
	bool SetPriority(int iPriority) const;//设置线程有限级

	int GetPriority() const;//获取线程的优先级

	bool Suspend();//暂停线程

	bool Resume();//唤起线程

	bool IsSuspended() const;//线程是否暂停

	bool IsRunning() const;//线程是否在运行

	bool GetExitCode(DWORD * pExitCode) const;//获取退出码

#if(WINVER >= 0x0500)
	//获取线程时间信息
	bool GetThreadTimes(LPFILETIME lpCreationTime, LPFILETIME lpExitTime, LPFILETIME lpKernelTime, LPFILETIME lpUserTime) const;
#endif

protected:
//等待线程结束
	bool WaitForThread(DWORD dwTimeout = INFINITE) const;
//终止线程
	bool Terminate(DWORD dwExitCode = 0) const;
//线程函数
	static DWORD WINAPI ThreadProc(LPVOID pData);
//线程run函数
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
