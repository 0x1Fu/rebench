#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

char *heap_start();
void heap_reset();

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

void *kiss_fftr_alloc(int, int, char*, char*);
void kiss_fftr(void*, char*, char*);

void test_600_fft(int loops) {
	heap_reset();

	char timedata[0x2000] = { 0 };
	char freqdata[0x2000] = { 0 };

	void *cfg = kiss_fftr_alloc(1024, 0, 0, 0);

	begin();
	for (int i=0; i<loops; i++)
		kiss_fftr(cfg, timedata, freqdata);
	double secs = end();

	unsigned int score = (unsigned int)((double)loops / secs + 0.5);
	fprintf(stderr, "600_FFT: %u (loops: %d, %.6fs)\n", score, loops, secs);
}

double  test_sgemm(unsigned long long, double);
int _Z5sgemmjPfS_S_(unsigned int, void*, void*, void*);

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

double  test_dgemm(unsigned long long, double);
int _Z5dgemmjPdS_S_(unsigned int, void*, void*, void*);

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
	double sgemm = test_601_sgemm(loops);
	double dgemm = test_601_dgemm(loops / 2);

	unsigned int score_sgemm = (unsigned int)((double)loops * 335.54432 / sgemm + 0.5);
	unsigned int score_dgemm = (unsigned int)((double)loops * 335.54432 / dgemm + 0.5);

	fprintf(stderr, "601_GEMM: %u (loops: %d, %.6fs)\n",
		   score_sgemm + score_dgemm, loops, sgemm + dgemm);
	fprintf(stderr, "  6011_SGEMM: %u (loops: %d, %.6fs)\n",
		   score_sgemm, loops, sgemm);
	fprintf(stderr, "  6012_DGEMM: %u (loops: %d, %.6fs)\n",
		   score_dgemm, loops / 2, dgemm);
}

uint64_t dummy = 0;
static int test_map_idx = 0;
static double test_map_results[2];
double test_map(int loops);
typedef long (* maptest_func1_t)(void *, void *);
typedef long (* maptest_func2_t)(void *);

double _Z15test_const_timeR8CMapTestd(void* map, double tm) {
	maptest_func1_t func1 = (maptest_func1_t)*(uint64_t *)(*(uint64_t *)map + 8);
	maptest_func2_t func2 = (maptest_func2_t)*(uint64_t *)(*(uint64_t *)map + 24);

	int i;
	int loops = (int)tm;
	if (test_map_idx == 1)
		loops *= 10;

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
	double score = test_map(loops * 2) * 10000.0 + 0.5;
	fprintf(stderr, "603_MAP: %d\n", (int)score);
	fprintf(stderr, "  6031_MAP: (loops: %d, %.6fs)\n", loops, test_map_results[0]);
	fprintf(stderr, "  6031_MAP: (loops: %d, %.6fs)\n", loops * 10, test_map_results[1]);
}

int main(int argc, char *argv[]) {
	int arg1 = argc > 1 ? atoi(argv[1]) : 0;
	int arg2 = argc > 2 ? atoi(argv[2]) : 0;

	fprintf(stderr, "UTUTNA:\n");
	if (arg1 == 0 || arg1 == 600) test_600_fft(arg2 ? arg2 : 100000);
	if (arg1 == 0 || arg1 == 601) test_601_gemm(arg2 ? arg2 : 500);
	if (arg1 == 0 || arg1 == 603) test_603_map(arg2 ? arg2 : 250);

	return 0;
}
