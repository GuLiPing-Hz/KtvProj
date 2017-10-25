#ifndef THREADMGR__H__
#define THREADMGR__H__
/*
	注释添加以及修改于 2014-4-2 

	封装一个消息循环队列，对发送的信息进行处理。
	消息的的发送有两种方式Post 跟 Send（阻塞等待处理完成）
	提供ProcessMessage纯虚接口，子类实现自己的业务
*/
//#define _STLP_OUTERMOST_HEADER_ID 0x40
#include <list>
#include "thread_wrapper.h"

class EventWrapper;
class ThreadWrapper;
class CriticalSectionWrapper;

enum
{
	MSG_INVALID = 0,
};

struct _tMsg//定义消息结构
{
	bool		isSend;//是否立即发送
	int		id;
	void*	pData;
};

class CThreadMgr
{
public:
	CThreadMgr(void);
	virtual ~CThreadMgr(void);

public:

public:
//创建线程，指定优先级
	bool CreateThread(ThreadPriority iPriority = kNormalPriority);
//资源回收
	bool ReleaseThread();
//投递消息
	void PostMessageOS( int id, void* pData );
//发送消息
	bool SendMessageOS( int id, void* pData );
//消息处理
	virtual void ProcessMessage(int id,void* pData) = 0;
//空闲处理
	virtual void ProcessIdle();
//线程run函数
	bool Run();
protected:
//线程函数
	static bool ThreadProc(ThreadObj pData);

protected:
	volatile bool						m_bStop;
private:
	ThreadWrapper*					m_pObjectThread;
	EventWrapper*					m_pObjectEvent;
	CriticalSectionWrapper*			m_pObjectCS;
	
	std::list<_tMsg>					m_lstMsg;
};
#endif//THREADMGR__H__
