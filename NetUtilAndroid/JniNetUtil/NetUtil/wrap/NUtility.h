/*
	注释添加以及修改于 2014-4-3

	提供临界区的封装，以及对临界区友好使用的类CriticalSectionScoped 范围锁：只在生命周期内锁住。
	VoidGuard 防止内存泄露的友好类。是用free释放，所以只能保护malloc之类创建的内存

	提供字符集之间的转换的函数。
*/
#ifndef NTUTILLITY__H__
#define NTUTILLITY__H__

#include "config.h"


class CriticalSectionWrapper {
public:
	// Factory method, constructor disabled
	static CriticalSectionWrapper* CreateCriticalSection();

	virtual ~CriticalSectionWrapper() {}

	// Tries to grab lock, beginning of a critical section. Will wait for the
	// lock to become available if the grab failed.
	virtual void Enter() = 0;

	// Returns a grabbed lock, end of critical section.
	virtual void Leave() = 0;
};

// RAII extension of the critical section. Prevents Enter/Leave mismatches and
// provides more compact critical section syntax.
class CriticalSectionScoped {
public:
	explicit CriticalSectionScoped(CriticalSectionWrapper* critsec)
		: ptr_crit_sec_(critsec) {
				ptr_crit_sec_->Enter();
	}

	~CriticalSectionScoped() {
		if (ptr_crit_sec_) {
			Leave();
		}
	}

private:
	void Leave() {
		ptr_crit_sec_->Leave();
		ptr_crit_sec_ = 0;
	}

	CriticalSectionWrapper* ptr_crit_sec_;//release by users;
};

class VoidGuard
{
public:
	VoidGuard(void *p) : m_p(p){}
	virtual ~VoidGuard();
private:
	void *m_p;
};

template<class Cls>
class SafePointer
{
public:
    SafePointer(Cls* p):m_p(p){}
    ~SafePointer(){if(m_p)delete m_p;}
private:
    Cls* m_p;
};


#endif//NTUTILLITY__H__
