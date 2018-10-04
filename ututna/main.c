#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

extern char *heap_start();
extern void heap_reset();

static struct timeval start;

void begin() {
	gettimeofday(&start, NULL);
}

double end() {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	unsigned long usec = 1000000 * (tv.tv_sec -  start.tv_sec) + (tv.tv_usec - start.tv_usec);
	double seconds = (double)usec / 1000000.0;

	return seconds;
}

void print_header(void) {
	fprintf(stderr, "%-12s %7s %8s  %s\n", "NAME", "SCORE", "LOOPS", "DURATION(s)");
}
void print_score(const char *name, double score, int loops, double secs) {
	if (score == 0.0)
		fprintf(stderr, "%-12s %7s %8d  %.6f\n", name, "-", loops, secs);
	else
		fprintf(stderr, "%-12s %7u %8d  %.6f\n", name, (int)(score + 0.5), loops, secs);
}

extern void *kiss_fftr_alloc(int, int, char*, char*);
extern void kiss_fftr(void*, char*, char*);

void test_600_fft(int loops) {
	heap_reset();

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

extern double test_sgemm(unsigned long long, double);
extern int _Z5sgemmjPfS_S_(unsigned int, void*, void*, void*);

double test_601_sgemm(int loops) {
	heap_reset();

	double result = test_sgemm(1, 1.0);
	if (result != -1) {
		fprintf(stderr, "sgemm error.\n");
		return 0;
	}

	char *p1 = heap_start();
	char *p2 = p1 + 0x40000;
	char *p3 = p2 + 0x40000;

	begin();
	for (int i=0; i<loops; i++)
		_Z5sgemmjPfS_S_(0x100u, p1, p2, p3);
	return end();
}

extern double test_dgemm(unsigned long long, double);
extern int _Z5dgemmjPdS_S_(unsigned int, void*, void*, void*);

double test_601_dgemm(int loops) {
	heap_reset();

	double result = test_dgemm(1, 1.0);
	if (result != -1) {
		fprintf(stderr, "dgemm error.\n");
		return 0;
	}

	char *p1 = heap_start();
	char *p2 = p1 + 0x80000;
	char *p3 = p2 + 0x80000;

	begin();
	for (int i=0; i<loops; i++)
		_Z5dgemmjPdS_S_(0x100u, p1, p2, p3);
	return end();
}

void test_601_gemm(int loops) {
	int loops_dgemm = loops > 1 ? loops / 2 : 1;

	double sgemm = test_601_sgemm(loops);
	double dgemm = test_601_dgemm(loops_dgemm);

	double score_sgemm = (double)loops * 335.54432 / sgemm;
	double score_dgemm = (double)loops * 335.54432 / dgemm;

	print_score("601_GEMM",   score_sgemm + score_dgemm, loops, sgemm + dgemm);
	print_score(" SGEMM", score_sgemm, loops, sgemm);
	print_score(" DGEMM", score_dgemm, loops_dgemm, dgemm);
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
	test_map_results[test_map_idx++] = secs;

	return score;
}

void test_603_map(int loops) {
	heap_reset();

	test_map_idx = 0;
	loops_unordered = loops > 1 ? loops * 10 : 1;
	double score = test_map(loops * 2) * 10000.0;

	print_score("603_MAP", score, loops, test_map_results[0] + test_map_results[1]);
	print_score(" ORDERED", 0, loops, test_map_results[0]);
	print_score(" UNORDERED", 0, loops_unordered, test_map_results[1]);
}

extern double _Z10loadMemPNGPhjP7BmpData(void *, int, void *);
extern unsigned char test_png[];
extern unsigned int test_png_len;

void test_609_png(int loops) {
	heap_reset();

	char buf[128] = { 0 };
	double secs = 0;
	for (int i=0; i<loops; i++)
		secs += _Z10loadMemPNGPhjP7BmpData(test_png, test_png_len, buf);

	double score = (double)loops * 1000.0 / secs;
	print_score("603_PNG", score, loops, secs);
}

int main(int argc, char *argv[]) {
	int arg1 = argc > 1 ? atoi(argv[1]) : 0;
	int arg2 = argc > 2 ? atoi(argv[2]) : 0;

	fprintf(stderr, "UTUTNA:\n");
	print_header();
	if (arg1 == 0 || arg1 == 600) test_600_fft(arg2 ? arg2 : 200000);
	if (arg1 == 0 || arg1 == 601) test_601_gemm(arg2 ? arg2 : 1000);
	if (arg1 == 0 || arg1 == 603) test_603_map(arg2 ? arg2 : 500);
	if (arg1 == 0 || arg1 == 609) test_609_png(arg2 ? arg2 : 1000);

	return 0;
}
