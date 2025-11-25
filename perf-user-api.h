// File: perf_counters.h

#ifndef PERF_COUNTERS_H
#define PERF_COUNTERS_H

// Enum to define the counting modes
typedef enum {
    COUNT_ALL,          // Count cycles in all privilege levels (U+S)
    COUNT_USER_ONLY     // Count cycles only when in User-space
} CounterMode;

/**
 * @brief Enables the hardware CPU cycle counter for the current process.
 *
 * @param mode Selects whether to count all cycles or only user-space cycles.
 *             COUNT_USER_ONLY is recommended for noise reduction in benchmarks.
 * @return On success, returns a non-negative file descriptor handle.
 *         On failure, returns -1.
 */
int start_performance_counters(CounterMode mode);

/**
 * @brief Disables the hardware CPU cycle counter.
 *
 * @param handle The file descriptor handle returned by start_performance_counters().
 */
void stop_performance_counters(int handle);

#endif // PERF_COUNTERS_H
