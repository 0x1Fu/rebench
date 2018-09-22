#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void *kiss_fftr_alloc(int, int, char*, char*);
void kiss_fftr(void*, char*, char*);

void test_600_fft(int loop)
{
	struct timeval tv_start;
	struct timeval tv_end;

	char timedata[0x2000LL] = { 0 };
	char freqdata[0x2000LL] = { 0 };

	void *cfg = kiss_fftr_alloc(1024LL, 0LL, 0LL, 0LL);

	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	for (int i=0; i<loop; i++)
		kiss_fftr(cfg, timedata, freqdata);
	gettimeofday(&end, NULL);

	unsigned long usec = 1000000 * (end.tv_sec -  start.tv_sec) + (end.tv_usec - start.tv_usec);
	double seconds = (double)usec / 1000000.0;

	unsigned int score = (int)((double)loop / seconds);
	printf("600_FFT = %u (loop: %d, duration: %.6fs)\n", score, loop, seconds);
}

int main(int argc, char *argv[])
{
	int arg1 = argc > 1 ? atoi(argv[1]) : 0;
	int arg2 = argc > 2 ? atoi(argv[2]) : 0;

	printf("UTUTNA:\n");
	if (arg1 == 0 || arg1 == 600) test_600_fft(arg2 ? arg2 : 100000);

	return 0;
}
