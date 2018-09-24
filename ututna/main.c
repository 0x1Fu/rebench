#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

char *heap_start();
void heap_reset();

void *kiss_fftr_alloc(int, int, char*, char*);
void kiss_fftr(void*, char*, char*);

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

void test_600_fft(int loop)
{
	heap_reset();

	char timedata[0x2000] = { 0 };
	char freqdata[0x2000] = { 0 };

	void *cfg = kiss_fftr_alloc(1024, 0, 0, 0);

	begin();
	for (int i=0; i<loop; i++)
		kiss_fftr(cfg, timedata, freqdata);
	double secs = end();

	unsigned int score = (unsigned int)((double)loop / secs);
	printf("600_FFT: %u (loop: %d, %.6fs)\n", score, loop, secs);
}

double  test_sgemm(unsigned long long, double);
int _Z5sgemmjPfS_S_(unsigned int, void*, void*, void*);
double test_601_sgemm(int loop)
{
	heap_reset();

	double result = test_sgemm(1, 1.0);
	if (result != -1) {
		printf("sgemm error.\n");
		return 0;
	}

	char *p1 = heap_start();
	char *p2 = p1 + 0x40000;
	char *p3 = p2 + 0x40000;

	begin();
	for (int i=0; i<loop; i++)
		_Z5sgemmjPfS_S_(0x100u, p1, p2, p3);
	return end();
}

double  test_dgemm(unsigned long long, double);
int _Z5dgemmjPdS_S_(unsigned int, void*, void*, void*);
double test_601_dgemm(int loop)
{
	heap_reset();

	double result = test_dgemm(1, 1.0);
	if (result != -1) {
		printf("dgemm error.\n");
		return 0;
	}

	char *p1 = heap_start();
	char *p2 = p1 + 0x80000;
	char *p3 = p2 + 0x80000;

	begin();
	for (int i=0; i<loop; i++)
		_Z5dgemmjPdS_S_(0x100u, p1, p2, p3);
	return end();
}

void test_601_gemm(int loop)
{
	double sgemm = test_601_sgemm(loop);
	double dgemm = test_601_dgemm(loop);

	unsigned int score_sgemm = (unsigned int)((double)loop * 335.54432 / sgemm);
	unsigned int score_dgemm = (unsigned int)((double)loop * 335.54432 / dgemm);

	printf("601_GEMM: %u (loop: %d, %.6fs, SGEMM: %u, DGEMM: %u)\n",
		   score_sgemm + score_dgemm, loop, sgemm + dgemm,
		   score_sgemm, score_dgemm);
}

int main(int argc, char *argv[])
{
	int arg1 = argc > 1 ? atoi(argv[1]) : 0;
	int arg2 = argc > 2 ? atoi(argv[2]) : 0;

	printf("UTUTNA:\n");
	if (arg1 == 0 || arg1 == 600) test_600_fft(arg2 ? arg2 : 100000);
	if (arg1 == 0 || arg1 == 601) test_601_gemm(arg2 ? arg2 : 500);

	return 0;
}
