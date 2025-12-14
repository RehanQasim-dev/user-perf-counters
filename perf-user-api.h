#ifndef PERF_COUNTERS_H
#define PERF_COUNTERS_H
typedef enum {
    COUNT_ALL,          
    COUNT_USER_ONLY     
} CounterMode;

int start_performance_counters(CounterMode mode);
void stop_performance_counters(int handle);

#endif
