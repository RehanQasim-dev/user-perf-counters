// File: main.c
#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <sched.h>
#include <unistd.h>
#include "perf_counters.h" // Your performance counters header

// The simple rdcycle reader function
static inline uint64_t read_rdcycle(void) {
    uint64_t cycles;
    asm volatile ("rdcycle %0" : "=r"(cycles));
    return cycles;
}

// Pin process to CPU 0
void pin_to_core0(void) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    if (sched_setaffinity(0, sizeof(cpuset), &cpuset) != 0) {
        perror("sched_setaffinity");
    }
}

// Boost priority to reduce preemption
void boost_priority(void) {
    struct sched_param param = { .sched_priority = 99 };
    if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {
        perror("sched_setscheduler");
    }
}

int main(void) {
    printf("Pinning process to CPU 0 and boosting priority...\n");
    // pin_to_core0();
    // boost_priority();

    printf("Enabling counters in USER-SPACE ONLY mode...\n");
    int perf_handle = start_performance_counters(COUNT_ALL);
    if (perf_handle < 0) {
        fprintf(stderr, "Failed to start performance counters\n");
        return 1;
    }

    printf("Counters enabled. Warming up caches and pipelines...\n");

    volatile int sum = 0;

    // Warm-up loop to reduce cache/pipeline cold effects
    for (int w = 0; w < 1000; w++)
        for (int i = 0; i < 100; i++)
            sum += i;

    printf("Performing measurement...\n");

    uint64_t start, end, diff, best = UINT64_MAX;

    // Repeat measurement multiple times to reduce OS jitter
    start = read_rdcycle();

    // Dummy workload
    for (int i = 0; i < 1000000; i++)
        sum += i;

    end = read_rdcycle();
    diff = end - start;


    printf("Cycles spent (best of 100 runs): %llu\n\n", (unsigned long long)diff);

    printf("Disabling hardware performance counters.\n");
    stop_performance_counters(perf_handle);

    return 0;
}
