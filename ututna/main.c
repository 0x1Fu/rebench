#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <sys/time.h>

#include "jemalloc.h"
#include "stdlib_.h"
#include "perf.h"

#ifndef SEMIHOSTING
static struct timeval start;
#endif

void tp_start();
void tp_end();

void begin() {
#ifndef SEMIHOSTING
	gettimeofday(&start, NULL);
	perf_begin();
#else
	fprintf(stderr, ".");
	tp_start();
#endif
}

double end() {
#ifndef SEMIHOSTING
	perf_end();

	struct timeval tv;
	gettimeofday(&tv, NULL);

	unsigned long usec = 1000000 * (tv.tv_sec -  start.tv_sec) + (tv.tv_usec - start.tv_usec);
	double seconds = (double)usec / 1000000.0;

	return seconds;
#else
	tp_end();
	fprintf(stderr, ".");
	return 0;
#endif
}

uint64_t StartStopwatch(void *tv);
uint64_t s_StartStopwatch(void *tv) {
#ifndef SEMIHOSTING
	uint64_t ret = StartStopwatch(tv);
	perf_begin();
	return ret;
#else
	fprintf(stderr, ".");
	tp_start();
	return 0;
#endif
}

uint64_t StopStopwatch(uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4);
uint64_t s_StopStopwatch(uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4) {
#ifndef SEMIHOSTING
	perf_end();
	return StopStopwatch(v1, v2, v3, v4);
#else
	tp_end();
	fprintf(stderr, ".");
	return 0;
#endif
}

void print_header(void) {
#ifndef SEMIHOSTING
	fprintf(stderr, "%-15s %7s %8s  %s", "NAME", "SCORE", "LOOPS", "DURATION(s)");
	perf_print_headers();
#endif
}

void print_score(const char *name, double score, int loops, double secs) {
#ifdef SEMIHOSTING
	fprintf(stderr, "%-15s\n", name);
#else
	if (score == 0.0)
		fprintf(stderr, "%-15s %7s %8d  %.6f   ", name, "-", loops, secs);
	else
		fprintf(stderr, "%-15s %7u %8d  %.6f   ", name, (int)(score + 0.5), loops, secs);

	perf_print();
	fprintf(stderr, "\n");
#endif
}

extern void *kiss_fftr_alloc(int, int, char*, char*);
extern void kiss_fftr(void*, char*, char*);

void test_600_fft(int loops) {
	char timedata[0x2000] = { 0 };
	char freqdata[0x2000] = { 0 };

	void *cfg = kiss_fftr_alloc(1024, 0, 0, 0);

	begin();
	for (int i=0; i<loops; i++)
		kiss_fftr(cfg, timedata, freqdata);
	double secs = end();

	double score = (double)loops / secs;
	print_score("600_FFT", score, loops, secs);
}

static void *gemm_ptrs[3] = {0};
static int gemm_idx = 0;

void *s_malloc(size_t size);
void s_free(void *ptr);

void* gemm_malloc(size_t size) {
	if (gemm_idx >= 3) {
		fprintf(stderr, "gemm_malloc error.\n");
		exit(-1);
	}

	void *ptr = s_malloc(size);
	gemm_ptrs[gemm_idx++] = ptr;

	return ptr;
}

void gemm_free(void* ptr) {
}

void gemm_cleanup() {
	for (int i=0; i<3; i++) {
		s_free(gemm_ptrs[i]);
		gemm_ptrs[i] = NULL;
	}
	gemm_idx = 0;
}

extern double test_sgemm(unsigned long long, double);
extern int _Z5sgemmjPfS_S_(unsigned int, void*, void*, void*);

double test_601_sgemm(int loops) {
	double result = test_sgemm(1, 1.0);
	if (result != -1) {
		fprintf(stderr, "sgemm error.\n");
		return 0;
	}

	begin();
	for (int i=0; i<loops; i++)
		_Z5sgemmjPfS_S_(0x100u, gemm_ptrs[0], gemm_ptrs[1], gemm_ptrs[2]);
	return end();
}

extern double test_dgemm(unsigned long long, double);
extern int _Z5dgemmjPdS_S_(unsigned int, void*, void*, void*);

double test_601_dgemm(int loops) {
	double result = test_dgemm(1, 1.0);
	if (result != -1) {
		fprintf(stderr, "dgemm error.\n");
		return 0;
	}

	begin();
	for (int i=0; i<loops; i++) {
		_Z5dgemmjPdS_S_(0x100u, gemm_ptrs[0], gemm_ptrs[1], gemm_ptrs[2]);
	}
	return end();
}

void test_601_gemm(int loops) {
	int loops_dgemm = loops > 1 ? loops / 2 : 1;

	double sgemm = test_601_sgemm(loops);
	double score_sgemm = (double)loops * 335.54432 / sgemm;
	print_score("[SGEMM]", score_sgemm, loops, sgemm);
	gemm_cleanup();

	double dgemm = test_601_dgemm(loops_dgemm);
	double score_dgemm = (double)loops_dgemm * 335.54432 / dgemm;
	print_score("[DGEMM]", score_dgemm, loops_dgemm, dgemm);
	gemm_cleanup();

	print_score("601_GEMM", score_sgemm + score_dgemm, loops, sgemm + dgemm);
}

extern double test_map(int loops);
uint64_t dummy = 0;
static int test_map_idx = 0;
static int loops_unordered = 0;
static double test_map_results[2];
typedef long (* maptest_func1_t)(void *, void *);
typedef long (* maptest_func2_t)(void *);

double _Z15test_const_timeR8CMapTestd(void* map, double tm) {
	maptest_func1_t func1 = (maptest_func1_t)*(uint64_t *)(*(uint64_t *)map + 8);
	maptest_func2_t func2 = (maptest_func2_t)*(uint64_t *)(*(uint64_t *)map + 24);

	int i;
	int loops = (int)tm;
	if (test_map_idx == 1)
		loops = loops_unordered;

	begin();
	for (i=0; i<loops; i++) {
		if (!func1(map, &dummy))
			break;
	}
	double secs = end();

	double score = i * func2(map) / secs / 1000000.0;
	test_map_results[test_map_idx] = secs;

	if (test_map_idx == 0)
		print_score("[MAP.ORDERED]", 0, loops, test_map_results[0]);
	else
		print_score("[MAP.UNORDERED]", 0, loops_unordered, test_map_results[1]);

	test_map_idx++;

	return score;
}

void test_603_map(int loops) {
	test_map_idx = 0;
	loops_unordered = loops > 1 ? loops * 10 : 1;
	double score = test_map(loops * 2) * 10000.0;

	print_score("603_MAP", score, loops, test_map_results[0] + test_map_results[1]);
}

extern double _Z10loadMemPNGPhjP7BmpData(void *, int, void *);
extern unsigned char test_png[];
extern unsigned int test_png_len;

void test_609_png(int loops) {
	void *ret[10];
	double secs = 0;
	begin(); // todo:
	for (int i=0; i<loops; i++) {
		ret[0] = 0;
		secs += _Z10loadMemPNGPhjP7BmpData(test_png, test_png_len, ret);
		if (ret[0]) s_free(ret[0]);
	}
	end();

	double score = (double)loops * 1000.0 / secs;
	print_score("609_PNG", score, loops, secs);
}

extern double test_physics();
double L14f590;

void test_614_physics(int loops) {
	L14f590 = 5.0 / loops;

	double score = test_physics();
	double secs = 60.0 / score;
	score = loops / secs * 1000.0;

	print_score("614_PHYSICS", score, loops, secs);
}

extern void *generate_test_data(int);
extern int md5_sum_data(void *, int, void *);
extern int sha1_sum_data(void *, int, void *);
extern int sha2_sum_data(void *, int, void *);
extern int crc_sum_data(void *, int, void *);

void test_607_hash(int loops) {
	void *data = generate_test_data(0x5000);
	char buf[128] = {0};

	begin();
	int i;
	for (i=0; i<loops; i++) {
		if ( md5_sum_data(data, 0x5000, buf) ||
			sha1_sum_data(data, 0x5000, buf) ||
			sha2_sum_data(data, 0x5000, buf) ||
			 crc_sum_data(data, 0x5000, buf))
			break;
	}
	double secs = end();

	double score = (i == loops ? (double)loops * 6 / secs : 0);
	print_score("607_HASH", score, loops, secs);

	s_free(data);
}

void bootstrap();

int main(int argc, char *argv[]) {
	int id = 0;
	int loops = 0;

	bootstrap();

	int c;
	while ((c = getopt(argc, argv, "h:c:r:i:?")) != -1) {
		perf_config(c, strtoul(optarg, NULL, 16));
	}

	if (optind < argc)
		id = atoi(argv[optind++]);
	if (optind < argc)
		loops = atoi(argv[optind++]);

	perf_init();

	fprintf(stderr, "UTUTNA\n");

	if (loops != 0)
		fprintf(stderr, "LOOPS: %d\n", loops);
	print_header();

	if (id == 0 || id == 600) test_600_fft(loops ? loops : 200000);
	if (id == 0 || id == 601) test_601_gemm(loops ? loops : 1000);
	if (id == 0 || id == 603) test_603_map(loops ? loops : 500);
	if (id == 0 || id == 609) test_609_png(loops ? loops : 1000);
	if (id == 0 || id == 614) test_614_physics(loops ? loops : 600);
	if (id == 0 || id == 607) test_607_hash(loops ? loops : 5000);

	if (id == 1000) stdlib_test();

	fprintf(stderr, "done\n");

	return 0;
}
