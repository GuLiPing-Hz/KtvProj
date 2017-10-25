#include "thread_wrapper.h"

#if defined(_WIN32)
#include "thread_win.h"
#else
#include "thread_posix.h"
#endif

#ifdef _WIN32
#include <assert.h>
#include <process.h>
#include <stdio.h>
#include <windows.h>

#include "set_thread_name_win.h"

ThreadWindows::ThreadWindows(ThreadRunFunction func, ThreadObj obj,
							 ThreadPriority prio, const char* thread_name)
							 : ThreadWrapper(),
							 run_function_(func),
							 obj_(obj),
							 alive_(false),
							 dead_(true),
							 do_not_close_handle_(false),
							 prio_(prio),
							 thread_(NULL),
							 id_(0),
							 set_thread_name_(false) {
								 name_[0] = 0;
								 critsect_stop_ = CriticalSectionWrapper::CreateCriticalSection();
								 if (thread_name != NULL) {
									 // Set the thread name to appear in the VS debugger.
									 set_thread_name_ = true;
									 strncpy(name_, thread_name, kThreadMaxNameLength);
								 }
}

ThreadWindows::~ThreadWindows() {
#ifdef _DEBUG
	assert(!alive_);
#endif
	if (thread_) {
		CloseHandle(thread_);
	}
	if (critsect_stop_) {
		delete critsect_stop_;
	}
}

uint32_t ThreadWrapper::GetThreadId() {
	return GetCurrentThreadId();
}

unsigned int WINAPI ThreadWindows::StartThread(LPVOID lp_parameter) {
	ThreadWindows* pThread = static_cast<ThreadWindows*>(lp_parameter);
	if(!pThread)
		return 0;
	if(pThread->m_funStart)
		pThread->m_funStart((unsigned int)pThread);
	//线程运行
	pThread->Run();

	if(pThread->m_funEnd)
		pThread->m_funEnd((unsigned int)pThread);
	return 0;
}

bool ThreadWindows::Start(unsigned int& thread_id) {
	if (!run_function_) {
		return false;
	}
	do_not_close_handle_ = false;

	// Set stack size to 1M
	thread_ = (HANDLE)_beginthreadex(NULL, 1024 * 1024, StartThread, (void*)this,
		CREATE_SUSPENDED, &thread_id);//创建一个暂停的线程
	if (thread_ == NULL) {
		return false;
	}
	id_ = thread_id;

	switch (prio_) {
	case kLowPriority:
		SetThreadPriority(thread_, THREAD_PRIORITY_BELOW_NORMAL);
		break;
	case kNormalPriority:
		SetThreadPriority(thread_, THREAD_PRIORITY_NORMAL);
		break;
	case kHighPriority:
		SetThreadPriority(thread_, THREAD_PRIORITY_ABOVE_NORMAL);
		break;
	case kHighestPriority:
		SetThreadPriority(thread_, THREAD_PRIORITY_HIGHEST);
		break;
	case kRealtimePriority:
		SetThreadPriority(thread_, THREAD_PRIORITY_TIME_CRITICAL);
		break;
	};

	ResumeThread(thread_);//唤起线程
	return true;
}

bool ThreadWindows::SetAffinity(const int* processor_numbers,
								const unsigned int amount_of_processors) {
									DWORD_PTR processor_bit_mask = 0;
									for (unsigned int processor_index = 0;
										processor_index < amount_of_processors;
										++processor_index) {
											// Convert from an array with processor numbers to a bitmask
											// Processor numbers start at zero.
											// TODO(hellner): this looks like a bug. Shouldn't the '=' be a '+='?
											// Or even better |=
											processor_bit_mask = 1 << processor_numbers[processor_index];
									}
									return SetThreadAffinityMask(thread_, processor_bit_mask) != 0;
}

void ThreadWindows::SetNotAlive() {
	alive_ = false;
}

bool ThreadWindows::WaitFor(unsigned int ms)
{
	if(thread_&&alive_)
		return (WAIT_OBJECT_0 == WaitForSingleObject(thread_,ms));
	return true;
}

bool ThreadWindows::Terminate(unsigned long ecode)
{
	if(thread_)
		return !!TerminateThread(thread_,ecode);
	return true;
}

bool ThreadWindows::Stop() {
	critsect_stop_->Enter();

	// Prevents the handle from being closed in ThreadWindows::Run()
	do_not_close_handle_ = true;
	alive_ = false;
	bool signaled = false;
	if (thread_ && !dead_) {
		critsect_stop_->Leave();

		// Wait up to 2 seconds for the thread to complete.
		if (WAIT_OBJECT_0 == WaitForSingleObject(thread_, 2000)) {
			signaled = true;
		}
		critsect_stop_->Enter();
	}
	if (thread_) {
		CloseHandle(thread_);
		thread_ = NULL;
	}
	critsect_stop_->Leave();

	if (dead_ || signaled) {
		return true;
	} else {
		return false;
	}
}

void ThreadWindows::Run() {
	alive_ = true;
	dead_ = false;

	// All tracing must be after event_->Set to avoid deadlock in Trace.
	if (set_thread_name_)
	{
		//WEBRTC_TRACE(kTraceStateInfo, kTraceUtility, id_,"Thread with name:%s started ", name_);
		SetThreadName(-1, name_); // -1, set thread name for the calling thread.
	} 
	else 
	{
		//WEBRTC_TRACE(kTraceStateInfo, kTraceUtility, id_,"Thread without name started");
	}

	do 
	{
		if (run_function_) 
		{
			if (!run_function_(obj_))
			{
				alive_ = false;
			}
		} 
		else 
		{
			alive_ = false;
		}
	} while (alive_);

	if (set_thread_name_) {
		//WEBRTC_TRACE(kTraceStateInfo, kTraceUtility, id_,"Thread with name:%s stopped", name_);
	} 
	else
	{
		//WEBRTC_TRACE(kTraceStateInfo, kTraceUtility, id_,"Thread without name stopped");
	}

	critsect_stop_->Enter();

	if (thread_ && !do_not_close_handle_) {
		HANDLE thread = thread_;
		thread_ = NULL;
		CloseHandle(thread);
	}
	dead_ = true;

	critsect_stop_->Leave();
};
#else 
#include <algorithm>

#include <assert.h>
#include <errno.h>
#include <string.h>  // strncpy
#include <unistd.h>
#ifdef NETUTIL_LINUX
#include <sys/types.h>
#include <sched.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <sys/prctl.h>
#endif

#include "NUtility.h"
#include "sleep.h"

int ConvertToSystemPriority(ThreadPriority priority, int min_prio,
							int max_prio) {
								assert(max_prio - min_prio > 2);
								const int top_prio = max_prio - 1;
								const int low_prio = min_prio + 1;

								switch (priority) {
	case kLowPriority:
		return low_prio;
	case kNormalPriority:
		// The -1 ensures that the kHighPriority is always greater or equal to
		// kNormalPriority.
		return (low_prio + top_prio - 1) / 2;
	case kHighPriority:
		return std::max(top_prio - 2, low_prio);
	case kHighestPriority:
		return std::max(top_prio - 1, low_prio);
	case kRealtimePriority:
		return top_prio;
								}
								assert(false);
								return low_prio;
}

extern "C"
{
	static void* StartThread(void* lp_parameter) {
		ThreadPosix* pThread = static_cast<ThreadPosix*>(lp_parameter);
		if(!pThread)
			return 0;
        
        unsigned int* pTId = (unsigned int*)pThread;
		if(pThread->m_funStart)
			pThread->m_funStart(*pTId);
		//线程运行
		pThread->Run();

		if(pThread->m_funEnd)
			pThread->m_funEnd(*pTId);

		return 0;
	}
}

ThreadWrapper* ThreadPosix::Create(ThreadRunFunction func, ThreadObj obj,
								   ThreadPriority prio,
								   const char* thread_name) {
									   ThreadPosix* ptr = new ThreadPosix(func, obj, prio, thread_name);
									   if (!ptr) {
										   return NULL;
									   }
									   const int error = ptr->Construct();
									   if (error) {
										   delete ptr;
										   return NULL;
									   }
									   return ptr;
}

ThreadPosix::ThreadPosix(ThreadRunFunction func, ThreadObj obj,
						 ThreadPriority prio, const char* thread_name)
						 : run_function_(func),
						 obj_(obj),
						 crit_state_(CriticalSectionWrapper::CreateCriticalSection()),
						 alive_(false),
						 dead_(true),
						 prio_(prio),
						 event_(EventWrapper::Create()),
						 name_(),
						 set_thread_name_(false),
#if (defined(NETUTIL_LINUX) || defined(NETUTIL_ANDROID))
						 pid_(-1),
#endif
						 attr_(),
						 thread_(0) {
							 if (thread_name != NULL) {
								 set_thread_name_ = true;
								 strncpy(name_, thread_name, kThreadMaxNameLength);
								 name_[kThreadMaxNameLength - 1] = '\0';
							 }
}

uint32_t ThreadWrapper::GetThreadId() {
#if defined(NETUTIL_ANDROID) || defined(NETUTIL_LINUX)
	return static_cast<uint32_t>(syscall(__NR_gettid));
#elif defined(NETUTIL_MAC) || defined(NETUTIL_IOS)
	return pthread_mach_thread_np(pthread_self());
#else
	return reinterpret_cast<uint32_t>(pthread_self());
#endif
}

int ThreadPosix::Construct() {
	int result = 0;
#if !defined(NETUTIL_ANDROID)
	// Enable immediate cancellation if requested, see Shutdown().
	result = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if (result != 0) {
		return -1;
	}
	result = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if (result != 0) {
		return -1;
	}
#endif
	result = pthread_attr_init(&attr_);
	if (result != 0) {
		return -1;
	}
	return 0;
}

ThreadPosix::~ThreadPosix() {
	pthread_attr_destroy(&attr_);
	delete event_;
	delete crit_state_;
}

#define HAS_THREAD_ID !defined(NETUTIL_IOS) && !defined(NETUTIL_MAC)

bool ThreadPosix::Start(unsigned int& thread_id)
{
	int result = pthread_attr_setdetachstate(&attr_, PTHREAD_CREATE_DETACHED);
	// Set the stack stack size to 1M.
	result |= pthread_attr_setstacksize(&attr_, 1024 * 1024);
#ifdef WEBRTC_THREAD_RR
	const int policy = SCHED_RR;
#else
	const int policy = SCHED_FIFO;
#endif
	event_->Reset();
	// If pthread_create was successful, a thread was created and is running.
	// Don't return false if it was successful since if there are any other
	// failures the state will be: thread was started but not configured as
	// asked for. However, the caller of this API will assume that a false
	// return value means that the thread never started.
	result |= pthread_create(&thread_, &attr_, &StartThread, this);
	if (result != 0) {
		return false;
	}
	{
		CriticalSectionScoped cs(crit_state_);
		dead_ = false;
	}

	// Wait up to 10 seconds for the OS to call the callback function. Prevents
	// race condition if Stop() is called too quickly after start.
	if (kEventSignaled != event_->Wait(UTIL_EVENT_10_SEC)) {
		//WEBRTC_TRACE(kTraceError, kTraceUtility, -1,"posix thread event never triggered");
		// Timed out. Something went wrong.
		return true;
	}

#if HAS_THREAD_ID
	thread_id = static_cast<unsigned int>(thread_);
#endif
	sched_param param;

	const int min_prio = sched_get_priority_min(policy);
	const int max_prio = sched_get_priority_max(policy);

	if ((min_prio == EINVAL) || (max_prio == EINVAL)) {
		//WEBRTC_TRACE(kTraceError, kTraceUtility, -1,"unable to retreive min or max priority for threads");
		return true;
	}
	if (max_prio - min_prio <= 2) {
		// There is no room for setting priorities with any granularity.
		return true;
	}
	param.sched_priority = ConvertToSystemPriority(prio_, min_prio, max_prio);
	result = pthread_setschedparam(thread_, policy, &param);
	if (result == EINVAL) {
		//WEBRTC_TRACE(kTraceError, kTraceUtility, -1,"unable to set thread priority");
	}
	return true;
}

// CPU_ZERO and CPU_SET are not available in NDK r7, so disable
// SetAffinity on Android for now.
#if (defined(NETUTIL_LINUX) && (!defined(NETUTIL_ANDROID)))
bool ThreadPosix::SetAffinity(const int* processor_numbers,
							  const unsigned int amount_of_processors) {
								  if (!processor_numbers || (amount_of_processors == 0)) {
									  return false;
								  }
								  cpu_set_t mask;
								  CPU_ZERO(&mask);

								  for (unsigned int processor = 0;
									  processor < amount_of_processors;
									  ++processor) {
										  CPU_SET(processor_numbers[processor], &mask);
								  }
#if defined(NETUTIL_ANDROID)
								  // Android.
								  const int result = syscall(__NR_sched_setaffinity,
									  pid_,
									  sizeof(mask),
									  &mask);
#else
								  // "Normal" Linux.
								  const int result = sched_setaffinity(pid_,
									  sizeof(mask),
									  &mask);
#endif
								  if (result != 0) {
									  return false;
								  }
								  return true;
}

#else
// NOTE: On Mac OS X, use the Thread affinity API in
// /usr/include/mach/thread_policy.h: thread_policy_set and mach_thread_self()
// instead of Linux gettid() syscall.
bool ThreadPosix::SetAffinity(const int* , const unsigned int) {
	return false;
}
#endif

void ThreadPosix::SetNotAlive() {
	CriticalSectionScoped cs(crit_state_);
	alive_ = false;
}

bool ThreadPosix::Stop() {
	bool dead = false;
	{
		CriticalSectionScoped cs(crit_state_);
		alive_ = false;
		dead = dead_;
	}

	// TODO(hellner) why not use an event here? pthread_join(thread_,NULL);//等待结束pthread_cancel(tcpengineinfo.hdl);
	// Wait up to 10 seconds for the thread to terminate
	if(!dead)
	{
		event_->Wait(UTIL_EVENT_10_SEC);
		{
			CriticalSectionScoped cs(crit_state_);
			dead = dead_;
		}
	}
// 	for (int i = 0; i < 1000 && !dead; ++i) {
// 		SleepMs(10);
// 		{
// 			CriticalSectionScoped cs(crit_state_);
// 			dead = dead_;
// 		}
// 	}
	if (dead) {
		return true;
	} else {
		return false;
	}
}

bool ThreadPosix::WaitFor(unsigned int ms)
{
	CriticalSectionScoped cs(crit_state_);
	if(alive_)
		return (kEventSignaled == event_->Wait(ms));
	return true;
}

bool ThreadPosix::Terminate(unsigned long ecode)
{
	if(alive_)
		return Stop();
	return true;
}

void ThreadPosix::Run() {
	{
		CriticalSectionScoped cs(crit_state_);
		alive_ = true;
	}
#if (defined(NETUTIL_LINUX) || defined(NETUTIL_ANDROID))
	pid_ = GetThreadId();
#endif
	// The event the Start() is waiting for.
	event_->Set();

	if (set_thread_name_) {
#ifdef NETUTIL_LINUX
		prctl(PR_SET_NAME, (unsigned long)name_, 0, 0, 0);
#endif
		//WEBRTC_TRACE(kTraceStateInfo, kTraceUtility, -1,"Thread with name:%s started ", name_);
	} else {
		//WEBRTC_TRACE(kTraceStateInfo, kTraceUtility, -1,"Thread without name started");
	}
	bool alive = true;
	bool run = true;
	while (alive) {
		run = run_function_(obj_);
		CriticalSectionScoped cs(crit_state_);
		if (!run) {
			alive_ = false;
		}
		alive = alive_;
	}

	if (set_thread_name_) {
		// Don't set the name for the trace thread because it may cause a
		// deadlock. TODO(hellner) there should be a better solution than
		// coupling the thread and the trace class like this.
		if (strcmp(name_, "Trace")) {
			//WEBRTC_TRACE(kTraceStateInfo, kTraceUtility, -1,"Thread with name:%s stopped", name_);
		}
	} else {
		//WEBRTC_TRACE(kTraceStateInfo, kTraceUtility, -1,"Thread without name stopped");
	}
	{
		CriticalSectionScoped cs(crit_state_);
		dead_ = true;
	}
	//inform the stop
	event_->Set();
}
#endif

ThreadWrapper* ThreadWrapper::CreateThread(ThreadRunFunction func,
                                           ThreadObj obj, ThreadPriority prio,
                                           const char* thread_name) 
{
#if defined(_WIN32)
  return new ThreadWindows(func, obj, prio, thread_name);
#else
  return ThreadPosix::Create(func, obj, prio, thread_name);
#endif
}

