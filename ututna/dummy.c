#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAX_MALLOC_SIZE 10240000

static char _buf[MAX_MALLOC_SIZE] = { 0 };
static char *_ptr = _buf;
static size_t _avail = MAX_MALLOC_SIZE;

char *heap_start() {
    return _buf;
}

void heap_reset() {
    _ptr = _buf;
    _avail = MAX_MALLOC_SIZE;
    memset(_buf, 0, sizeof(_buf));
}

void* o_malloc(size_t size) {

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

void* o_Znam(size_t size) {
    return o_malloc(size);
}

void o_ZdaPv(void* ptr) {
    return o_free(ptr);
}

void o_fwrite(void) {
	fprintf(stderr, "unsupported %s", __func__);
}

void o_exit(void) {
	fprintf(stderr, "unsupported %s", __func__);
}
