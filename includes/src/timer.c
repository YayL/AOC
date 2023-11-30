#include "timer.h"

unsigned long millis()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    unsigned long ms =	SEC_TO_MS((unsigned long)ts.tv_sec) + 
						NS_TO_MS((unsigned long)ts.tv_nsec);
    return ms;
}

unsigned long micros() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    unsigned long us =	SEC_TO_US((unsigned long)ts.tv_sec) + 
						NS_TO_US((unsigned long)ts.tv_nsec);
    return us;
}
