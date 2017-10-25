#ifndef EVENTHANDLER__H__
#define EVENTHANDLER__H__

/*
	注释添加以及修改于 2014-4-2 
	mender: glp

	封装一个对事件处理的接口类 EventHandler
	IdleEventHandler 提供空闲处理接口
	TMEventHandler 提供超时处理接口
	FDEventHandler 封装了一个描述符 提供fd的相关读写接口
*/

#ifdef WIN32
	#include <Winsock2.h>
	#pragma comment(lib,"Ws2_32.lib")
#else
	typedef int SOCKET;
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#include "unistd.h"
	#include "sys/socket.h"
	#include "arpa/inet.h"
	#include "netinet/in.h"
	#include <sys/types.h>
	#include <fcntl.h>
	#include <sys/stat.h>
	#include <errno.h>
	#include <netdb.h>
#endif

#include <time.h>
#include "noncopyable.h"

namespace NetworkUtil
{
	class Reactor;
	//提供一个反应器
	class EventHandler : public NonCopyable
	{
	public:
		virtual ~EventHandler(){}
		EventHandler():m_pReactor(NULL){}
		void SetReactor(Reactor *pReactor){m_pReactor = pReactor;}
		Reactor* GetReactor(){return m_pReactor;}
		virtual void closeSocket() = 0;
	protected:
		Reactor *m_pReactor;
	};
	//对空闲的处理
	class IdleEventHandler : virtual public EventHandler
	{
	public:
		IdleEventHandler(Reactor *pReactor)
		{
			SetReactor(pReactor);
		}
		virtual ~IdleEventHandler(){}
		virtual void onRun() = 0;
		int registerIdle();
		int unRegisterIdle();
		virtual void close();
		virtual void closeSocket(){}
	};
	//对事件的处理
	class TMEventHandler : virtual public EventHandler
	{
	public:
		TMEventHandler():m_id(0){}
		TMEventHandler(Reactor *pReactor):m_id(0) {SetReactor(pReactor);}
		virtual ~TMEventHandler() {}
		//超时处理函数
		virtual void onTimeOut() = 0;
		int registerTimer(time_t to);
		int unRegisterTimer();
		virtual void closeSocket();
		void setTimerID(int id) {m_id = id;}
		int getTimerID() {return m_id;}
	private:
		int m_id;
	};
	//对 socket事件的处理
	class FDEventHandler : virtual public EventHandler
	{
	public:
		virtual ~FDEventHandler() {}
		FDEventHandler():m_fd(INVALID_SOCKET) {}
		FDEventHandler(Reactor *pReactor):m_fd(INVALID_SOCKET)  {SetReactor(pReactor);}
		inline void setFD(SOCKET fd) {m_fd = fd;}
		//fd 读的时候 可接受
		virtual void onFDRead() = 0;
		//fd 写的时候 可发送
		virtual void onFDWrite() = 0;

		virtual void closeSocket();

		int registerRead();
		int registerWrite();
		int unRegisterRead();
		int unRegisterWrite();

		inline SOCKET getFD() const {return m_fd;}
		//设置非阻塞
		int setNonBlocking();
	protected:
		SOCKET m_fd;
	};
}

#endif//EVENTHANDLER__H__
