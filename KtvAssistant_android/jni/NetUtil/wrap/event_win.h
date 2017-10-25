#ifndef EVENT_WIN_H__
#define EVENT_WIN_H__

#include <windows.h>

#include "event_wrapper.h"

#include "typedefs.h"

class EventWindows : public EventWrapper {
 public:
  EventWindows();
  virtual ~EventWindows();

  //miro second
  virtual EventTypeWrapper Wait(unsigned long max_time);
  virtual bool Set();
  virtual bool Reset();

  virtual bool StartTimer(bool periodic, unsigned long time);
  virtual bool StopTimer();

 private:
  HANDLE  event_;
  uint32_t timerID_;
};

#endif  // EVENT_WIN_H__
