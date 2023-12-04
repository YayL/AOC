#ifndef TIMER_HEADER_FILE_H
#define TIMER_HEADER_FILE_H

#include <sys/time.h>

#define SEC_TO_MS(sec) ((sec) * 1e3)
/// Convert seconds to microseconds
#define SEC_TO_US(sec) ((sec) * 1e6)
/// Convert seconds to nanoseconds
#define SEC_TO_NS(sec) ((sec) * 1e9)

/// Convert nanoseconds to seconds
#define NS_TO_SEC(ns)   ((ns) / 1e9)
/// Convert nanoseconds to milliseconds
#define NS_TO_MS(ns)    ((ns) / 1e6)
/// Convert nanoseconds to microseconds
#define NS_TO_US(ns)    ((ns) / 1e3)

void start_timer();

unsigned long stop_timer();
#endif
