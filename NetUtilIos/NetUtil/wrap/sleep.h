#ifndef SLEEP__H__
#define SLEEP__H__

// This function sleeps for the specified number of milliseconds.
// It may return early if the thread is woken by some other event,
// such as the delivery of a signal on Unix.
void SleepMs(int msecs);


#endif  // WEBRTC_SYSTEM_WRAPPERS_INTERFACE_SLEEP_H_
