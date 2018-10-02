#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAX_MALLOC_SIZE (200 * 1024 * 1024)

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
void* o__Znam(size_t size) {
	return o_malloc(size);
}
void* o__Znwm(size_t size) {
	return o_malloc(size);
}

void o_ZdaPv(void* ptr) {
	return o_free(ptr);
}
void o__ZdlPv(void* ptr) {
	return o_free(ptr);
}
void o__ZdaPv(void *ptr) {
	return o_free(ptr);
}

#define UNIMPLEMENTED(func) \
void func() { \
	fprintf(stderr, "unimplemented: %s\n", __func__); \
	exit(-1); \
}

UNIMPLEMENTED(o_unimplemented)
UNIMPLEMENTED(o_fwrite)
UNIMPLEMENTED(o_exit)

UNIMPLEMENTED(o___stack_chk_fail)
UNIMPLEMENTED(o___sfp_handle_exceptions)
UNIMPLEMENTED(o___cxa_begin_catch)
UNIMPLEMENTED(o___cxa_end_catch)
UNIMPLEMENTED(o___cxa_rethrow)
UNIMPLEMENTED(o__Unwind_Resume)
