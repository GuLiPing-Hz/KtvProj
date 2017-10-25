#ifndef THREAD_WIN__H__
#define THREAD_WIN__H__

#include "thread_wrapper.h"

#include <windows.h>

#include "NUtility.h"

class ThreadWindows : public ThreadWrapper {
 public:
  ThreadWindows(ThreadRunFunction func, ThreadObj obj, ThreadPriority prio,
                const char* thread_name);
  virtual ~ThreadWindows();

  virtual bool Start(unsigned int& id);
  bool SetAffinity(const int* processor_numbers,
                   const unsigned int amount_of_processors);
  virtual bool Stop();
  virtual void SetNotAlive();
  virtual bool WaitFor(unsigned int ms=UTIL_EVENT_INFINITE);
  virtual bool Terminate(unsigned long ecode);

  static unsigned int WINAPI StartThread(LPVOID lp_parameter);

 protected:
  virtual void Run();

 private:
  ThreadRunFunction    run_function_;
  ThreadObj            obj_;

  bool                    alive_;
  bool                    dead_;

  // TODO(hellner)
  // do_not_close_handle_ member seem pretty redundant. Should be able to remove
  // it. Basically it should be fine to reclaim the handle when calling stop
  // and in the destructor.
  bool                    do_not_close_handle_;
  ThreadPriority          prio_;
  CriticalSectionWrapper* critsect_stop_;

  HANDLE                  thread_;
  unsigned int            id_;
  char                    name_[kThreadMaxNameLength];
  bool                    set_thread_name_;

};

#endif  // THREAD_WIN__H__
