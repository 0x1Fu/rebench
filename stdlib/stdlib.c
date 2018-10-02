#include <math.h>
#include <memory.h>

int main(int argc, char *argv[]) {
	void *src = (void *)NULL;
	void *dst = (void *)NULL;

	memset(src, 0, 10);
	memcpy(dst, src, 10);

	volatile double ret = 1.0;
	ret = sin(ret);
	ret = cos(ret);
	ret = sqrt(ret);
	ret = pow(ret, ret);

	return 0;
}
