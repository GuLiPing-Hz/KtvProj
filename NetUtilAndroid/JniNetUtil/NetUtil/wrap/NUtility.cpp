#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include "NUtility.h"

#ifdef WIN32
#include <windows.h>
class CriticalSectionWindows : public CriticalSectionWrapper {
public:
	CriticalSectionWindows(){InitializeCriticalSection(&crit);}

	virtual ~CriticalSectionWindows(){DeleteCriticalSection(&crit);}

	virtual void Enter(){EnterCriticalSection(&crit);}
	virtual void Leave(){LeaveCriticalSection(&crit);}

private:
	CRITICAL_SECTION crit;
};

#else 

#include <pthread.h>
class CriticalSectionPosix : public CriticalSectionWrapper {
public:
	CriticalSectionPosix()
	{
		pthread_mutexattr_t attr;
		(void) pthread_mutexattr_init(&attr);
		(void) pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		(void) pthread_mutex_init(&mutex_, &attr);
	}

	virtual ~CriticalSectionPosix(){(void) pthread_mutex_destroy(&mutex_);}

	virtual void Enter(){(void) pthread_mutex_lock(&mutex_);}
	virtual void Leave(){(void) pthread_mutex_unlock(&mutex_);}

private:
	pthread_mutex_t mutex_;
};
#endif//WIN32

CriticalSectionWrapper* CriticalSectionWrapper::CreateCriticalSection() {
#ifdef WIN32
	return new CriticalSectionWindows();
#else
	return new CriticalSectionPosix();
#endif
}

VoidGuard::~VoidGuard(){
		if(m_p)
			free(m_p);
}
