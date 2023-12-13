#ifndef TIMER_HEADER_FILE_H
#define TIMER_HEADER_FILE_H

#include <sys/time.h>
#include <time.h>

#define SEC_TO_MS(sec) ((sec) * 1000LL)
/// Convert seconds to microseconds
#define SEC_TO_US(sec) ((sec) * 1000000LL)

/// Convert nanoseconds to seconds
#define NS_TO_SEC(ns)   ((double)(ns) / 1e9)
/// Convert nanoseconds to milliseconds
#define NS_TO_MS(ns)    ((double)(ns) / 1e6)
/// Convert nanoseconds to microseconds
#define NS_TO_US(ns)    ((double)(ns) / 1e3)

void start_timer();

double stop_timer();
#endif
