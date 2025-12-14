#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <errno.h>

#include "perf_counters.h"
static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

int start_performance_counters(CounterMode mode) {
    struct perf_event_attr pe;
    int fd;

    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HARDWARE;
    pe.size = sizeof(struct perf_event_attr);
    pe.config = PERF_COUNT_HW_CPU_CYCLES;
    pe.disabled = 1;
    if (mode == COUNT_USER_ONLY) {
        pe.exclude_kernel = 1;
        pe.exclude_hv = 1;
        printf("only user\n");
    } else {
        pe.exclude_kernel = 0;
                pe.exclude_hv = 0;
                printf("ALL\n");

    }
    fd = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd == -1) {
        perror("ERROR: perf_event_open failed");
        fprintf(stderr, "HINT: Try running with sudo or lowering /proc/sys/kernel/perf_event_paranoid to 1.\n");
        return -1;
    }

    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    return fd;
}

void stop_performance_counters(int handle) {
    if (handle < 0) return;
    ioctl(handle, PERF_EVENT_IOC_DISABLE, 0);
    close(handle);
}
