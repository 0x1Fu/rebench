#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>

#include "stdlib_.h"

#define TEST(test) \
	if (ret) { \
		ret = (test); \
		if (!ret) { \
			fprintf(stderr, "%-15s:%3d failed.\n", __func__, __LINE__); \
		} \
	}
#define END \
	if (ret) { \
		fprintf(stderr, "%-15s:%3d pass\n", __func__, __LINE__); \
	}

void test_memset() {
	int ret = 1;
	char buf[10];
	memset(buf, 10, 10);
	memset(buf, 5, 5);
	TEST(buf[0] == 5)
	TEST(buf[4] == 5)
	TEST(buf[5] == 10)
	END
	if (!ret) {
		for (int i=0; i<sizeof(buf); i++) {
			fprintf(stderr, "buf[%d] = %d\n", i, buf[i]);
		}
	}
}

void stdlib_test() {
	test_memset();
}

