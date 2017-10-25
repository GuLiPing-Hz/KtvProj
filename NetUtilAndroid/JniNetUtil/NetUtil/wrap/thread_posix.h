#ifndef THREAD_POSIX__H__
#define THREAD_POSIX__H__

#include "config.h"
#include "thread_wrapper.h"

#include <pthread.h>

class CriticalSectionWrapper;
class EventWrapper;

int ConvertToSystemPriority(ThreadPriority priority, int min_prio,
                            int max_prio);

class ThreadPosix : public ThreadWrapper {
 public:
  static ThreadWrapper* Create(ThreadRunFunction func, ThreadObj obj,
                               ThreadPriority prio, const char* thread_name);

  ThreadPosix(ThreadRunFunction func, ThreadObj obj, ThreadPriority prio,
              const char* thread_name);
  ~ThreadPosix();

  // From ThreadWrapper.
  virtual void SetNotAlive();
  virtual bool Start(unsigned int& id);
  // Not implemented on Mac.
  virtual bool SetAffinity(const int* processor_numbers,
                           unsigned int amount_of_processors);
  virtual bool Stop();
  virtual bool WaitFor(unsigned int ms=UTIL_EVENT_INFINITE);
  virtual bool Terminate(unsigned long ecode);

  void Run();

 private:
  int Construct();

 private:
  ThreadRunFunction   run_function_;
  ThreadObj           obj_;

  // Internal state.
  CriticalSectionWrapper* crit_state_;  // Protects alive_ and dead_
  bool                    alive_;
  bool                    dead_;
  ThreadPriority          prio_;
  EventWrapper*           event_;

  // Zero-terminated thread name string.
  char                    name_[kThreadMaxNameLength];
  bool                    set_thread_name_;

  // Handle to thread.
#if (defined(NETUTIL_LINUX) || defined(NETUTIL_ANDROID))
  pid_t                   pid_;
#endif
  pthread_attr_t          attr_;
  pthread_t               thread_;
};

#endif  // THREAD_POSIX__H__
