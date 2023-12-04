#include "timer.h"

struct timeval t_start, t_stop;

void start_timer() {
    gettimeofday(&t_start, (void *) 0);
} 

unsigned long stop_timer() {
    gettimeofday(&t_stop, (void *) 0);
    return SEC_TO_MS((t_stop.tv_sec - t_start.tv_sec)) + (t_stop.tv_usec - t_start.tv_usec);
}
