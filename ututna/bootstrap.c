#include <stdio.h>
#include <stdlib.h>

#ifdef SEMIHOSTING

static char bootstrap_buffer[10240];
static char *bootstrap_ptr = bootstrap_buffer;
static unsigned int bootstrap_remaining = sizeof(bootstrap_buffer);

void *bootstrap_malloc(size_t size) {
	if (size > bootstrap_remaining)
		abort();

	void *ptr = bootstrap_ptr;
	bootstrap_ptr += size;
	bootstrap_remaining -= size;

	return ptr;
}

void bootstrap_free(void *ptr) {
}

void *bootstrap_realloc(void *ptr, size_t size) {
	abort();
}

void *bootstrap_calloc(size_t nmemb, size_t size) {
	abort();
}

char *_sbrk(int incr);

#define HEAP_START 0x44000000

#endif

void jemalloc_init();
void stdlib_init();

void bootstrap() {
#ifdef SEMIHOSTING
	char *p = _sbrk(0x1000);
	if (p < (char *)(HEAP_START - 0x1000)) {
		p = _sbrk(HEAP_START - 0x1000 - (uint64_t)p);
		p = _sbrk(0);
	}
#endif

	jemalloc_init();
	stdlib_init();

#ifdef SEMIHOSTING
	fprintf(stderr, "HEAP_START: %p 0x%x\n", p, bootstrap_remaining);
#endif
}
