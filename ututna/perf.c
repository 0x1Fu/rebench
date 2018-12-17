#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>

#include "perf.h"

#define MAX_COUNTERS 10

#define PMU_L1D_CACHE             0x04
#define PMU_L1D_CACHE_REFILL      0x03
#define PMU_INST_RETIRED          0x08
#define PMU_CPU_CYCLES            0x11
#define PMU_L2D_CACHE             0x16
#define PMU_L2D_CACHE_REFILL      0x17

static int perf_event_open(struct perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags) {
	return syscall(__NR_perf_event_open, attr, pid, cpu, group_fd, flags);
}

struct counters_t {
    uint64_t nr;
    uint64_t vals[6];
};

static int fd = -1;
static char headers[1024];
static int n_counters;
static int ratios[MAX_COUNTERS];
static struct counters_t counters_begin;
static struct counters_t counters_end;
static int counters_valid;

static void perf_open(int type, int event) {
	struct perf_event_attr attr;

    memset(&attr, 0, sizeof(struct perf_event_attr));

	attr.size = sizeof(struct perf_event_attr);
	attr.type = type;
	attr.config = event;
	attr.disabled = 1;
	attr.exclude_kernel = 1;
	attr.exclude_hv = 1;
	attr.read_format = PERF_FORMAT_GROUP;

	// calling process/thread on any CPU
	int ret = perf_event_open(&attr, 0, -1, fd, 0);
    if (ret < 0)
    {
        fprintf(stderr, "perf_event_open(%d, %d) failed, ret = %d\n", type, event, ret);
        exit(-1);
    }

	if (fd == -1)
		fd = ret;

	n_counters++;
}

void perf_config(char type, int event) {
	char buf[128];
	if (type == 'i') {
		if (n_counters + 2 > MAX_COUNTERS) return;

		switch(event) {
		case 0:
			ratios[n_counters + 1] = 1;
			perf_open(PERF_TYPE_RAW, PMU_INST_RETIRED);
			perf_open(PERF_TYPE_RAW, PMU_CPU_CYCLES);
			sprintf(buf, " %12s %12s %5s", "RETIRED", "CYCLES", "IPC");
			break;
		case 1:
			ratios[n_counters + 1] = 100;
			perf_open(PERF_TYPE_RAW, PMU_L1D_CACHE_REFILL);
			perf_open(PERF_TYPE_RAW, PMU_L1D_CACHE);
			sprintf(buf, " %12s %12s %5s", "L1D_REFILL", "L1D", "%");
			break;
		case 2:
			ratios[n_counters + 1] = 100;
			perf_open(PERF_TYPE_RAW, PMU_L2D_CACHE_REFILL);
			perf_open(PERF_TYPE_RAW, PMU_L2D_CACHE);
			sprintf(buf, " %12s %12s %5s", "L2D_REFILL", "L2D", "%");
			break;
		default:
			goto err;
		}
	} else {
		if (n_counters + 1 > MAX_COUNTERS) return;

		perf_open(
			type == 'h' ? PERF_TYPE_HARDWARE :
			type == 'c' ? PERF_TYPE_HW_CACHE :
			PERF_TYPE_RAW,
			event);

		char name[20];
		sprintf(name, " %s(0x%X)", type == 'h' ? "HW" : type == 'c' ? "CACHE" : "RAW", event);
		sprintf(buf, " %12s", name);
	}

	strcat(headers, buf);
	return;

err:
	fprintf(stderr, "unsupported pmu event %c %d\n", type, event);
}

void perf_init() {
	if (n_counters == 0) return;

	ioctl(fd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
}

void perf_begin() {
	if (n_counters == 0) return;

	ioctl(fd, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
	read(fd, &counters_begin, sizeof(uint64_t) * (n_counters + 1));
}

void perf_end() {
	if (n_counters == 0) return;

	read(fd, &counters_end, sizeof(uint64_t) * (n_counters + 1));
	counters_valid = 1;
}

void perf_print_headers() {
	fprintf(stderr, "%s\n", headers);
}

void perf_print() {
	if (!counters_valid) return;

	for (int i=0; i<n_counters; i++) {
		fprintf(stderr, " %12lu", counters_end.vals[i] - counters_begin.vals[i]);
		if (ratios[i]) {
			uint64_t val1 = counters_end.vals[i - 1] - counters_begin.vals[i - 1];
			uint64_t val2 = counters_end.vals[i] - counters_begin.vals[i];
			fprintf(stderr, " %5.2f", (double)val1 / val2 * ratios[i]);
		}
	}

	counters_valid = 0;
}
