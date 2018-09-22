#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAX_MALLOC_SIZE 102400

void* o_malloc(size_t size) {
	static char _buf[MAX_MALLOC_SIZE] = { 0 };
	static char *_ptr = _buf;
	static size_t _avail = sizeof(_buf);

	if (size > _avail) {
		fprintf(stderr, "malloc(%lu), no mem\n", size);
		return NULL;
	}

	void *ptr = _ptr;

	_ptr += size;
	_avail -= size;

	return ptr;
}
void o_free(void* ptr) {
}
void o_fwrite(void) {
	fprintf(stderr, "unsupported %s", __func__);
}
void o_exit(void) {
	fprintf(stderr, "unsupported %s", __func__);
}
