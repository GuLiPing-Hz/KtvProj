#ifndef THREADINFORMER__H__
#define THREADINFORMER__H__
/*
	注释添加以及修改于 2014-4-4 
	mender: glp

	前前人留下了Udp的消息循环，前人留下了Windows静默窗口的消息循环，
	被我处理掉了因为这个不能跨平台

	我提供了一个新的消息循环队列ProcessIdle来处理业务。
	具体业务处理在私有函数dealMessageInner里面。

	CAvaratData 封装了一个机器人的相关信息，但是被裁掉了。。先保留着。
*/
#include "wrap/ClientSocket.h"
#include "ResultData.h"
#include "wrap/ThreadMgr.h"

enum MSG_DEALNET
{
	MSG_DEAL = MSG_INVALID+1
};

class ThreadInformer : public NetworkUtil::UdpSocket,public CThreadMgr
{
public:
	ThreadInformer();
	ThreadInformer(NetworkUtil::Reactor *pReactor,const char *host = "127.0.0.1",short port = 27876);
	virtual ~ThreadInformer();
	//通知消息队列处理消息,,,这里消息只是充当事件的角色
	void Inform();
	//初始化
	virtual int Init();
	virtual bool UnInit();
	//UdpSocket
	virtual void onFDRead();
	//CThreadMgr
	virtual void ProcessMessage(int id,void* pData){};
	virtual void ProcessIdle();
private:
	void dealMessageInner();
private:
	static bool s_bInit;	
	EventWrapper* m_pObjectEventInform;
};


class CAvaratData
{
public:
	CAvaratData() { ClearData(); }
	virtual ~CAvaratData() {}
	virtual void ClearData();

public:
	bool AddPart( int iKey );
	bool AddLink( int iKey );
	bool AddData( int kind, int key );
	virtual bool CheckData( ResultData_Avatar * new_avarat, ResultData_Avatar * old_avarat );
	bool GetData( void * pBuf, ResultData_Avatar * avarat );

public:
	static bool ChangeData(  void * pBuf, ResultData_Avatar_Updata * avarat );

private:
	int		m_aAvatarData[PART_LEN + LINK_LEN][CNTL_LEN];
	int		m_iIndex;
	char	m_iCount[COUNT_MAX];
};

#endif//THREADINFORMER__H__
