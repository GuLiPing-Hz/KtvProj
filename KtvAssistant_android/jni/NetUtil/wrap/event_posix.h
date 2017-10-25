#ifndef EVENT_POSIX__H__
#define EVENT_POSIX__H__

#include "event_wrapper.h"

#include <pthread.h>
#include <time.h>

#include "thread_wrapper.h"

enum State {
  kUp = 1,
  kDown = 2
};

class EventPosix : public EventWrapper {
 public:
  static EventWrapper* Create();

  virtual ~EventPosix();

  virtual EventTypeWrapper Wait(unsigned long max_time);
  virtual bool Set();
  virtual bool Reset();

  virtual bool StartTimer(bool periodic, unsigned long time);
  virtual bool StopTimer();

 private:
  EventPosix();
  int Construct();

  static bool Run(ThreadObj obj);
  bool Process();
  EventTypeWrapper Wait(timespec& wake_at);

 private:
  pthread_cond_t  cond_;
  pthread_mutex_t mutex_;

  ThreadWrapper* timer_thread_;
  EventPosix*    timer_event_;
  timespec       created_at_;

  bool          periodic_;
  unsigned long time_;  // In ms
  unsigned long count_;
  State         state_;
};

#endif  // EVENT_POSIX__H__
