#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <time.h>

#ifndef SEMIHOSTING
#include <sys/mman.h>
#endif

#include <errno.h>
#include <pthread.h>
#include <fcntl.h>

#ifndef DEBUG_JEMALLOC
#define DEBUG_JEMALLOC 0
#endif

#if DEBUG_JEMALLOC
#define debug(format, ...)  fprintf(stderr, format, ##__VA_ARGS__)
#else
#define debug(...)
#endif

#define LIB_START  0x0000000041000000
#define LIB_SIZE   0x00200000

extern const char _binary_jemalloc_bin_start[];
extern const char _binary_jemalloc_bin_end[];
extern const char _binary_jemalloc_bin_size[];

#define DUMMY(func) \
int func() { \
	return 0; \
}

#define UNIMPLEMENTED(func) \
static void func() { \
	fprintf(stderr, "[jemalloc] unimplemented: %s\n", __func__); \
	exit(-1); \
}

DUMMY(s_pthread_atfork)
DUMMY(s_prctl)

static void *thread_value = NULL;

static pthread_t s_pthread_self(void) {
	return 1;
}
static int s_pthread_key_create(pthread_key_t *key, void (*destructor)(void*)) {
	*key = 1;
	return 0;
}
static void *s_pthread_getspecific(pthread_key_t key) {
	return thread_value;
}
static int s_pthread_setspecific(pthread_key_t key, const void *value) {
	thread_value = (void *)value;
	return 0;
}
static int s_pthread_mutex_init(pthread_mutex_t *mutex , pthread_mutexattr_t *attr) {
	return 0;
}
static int s_pthread_mutexattr_init(pthread_mutexattr_t *attr) {
	return 0;
}
static int s_pthread_mutexattr_destroy(pthread_mutexattr_t *attr) {
	return 0;
}
static int s_pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type) {
	return 0;
}
static int s_pthread_mutex_lock(pthread_mutex_t *mutex) {
	return 0;
}
static int s_pthread_mutex_unlock(pthread_mutex_t *mutex) {
	return 0;
}
static int s___open_2(const char *path, int oflag, ... ) {
	/* /proc/sys/vm/overcommit_memory */
	return 3;
}
static ssize_t s___read_chk(int fd, void *buf, size_t nbyte) {
	*(char *)buf = '1';
	return 1;
}
static int s_close(int fd) {
	return 0;
}
static long s_sysconf(int name) {
	/* _SC_NPROCESSORS_ONLN */
	return 1;
}

#ifdef SEMIHOSTING
char *_sbrk(int incr);
#endif

static void *s_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off) {
#ifndef SEMIHOSTING
	void *ptr = mmap(addr, len, prot, flags, fd, off);
#else
	void *ptr = _sbrk(len);
	// todo: memset?
#endif
	debug("mmap: addr=%p, len=%ld, prot=%d, flags=%d, fd=%d, off=%ld, ret=%p\n",
		  addr, len, prot, flags, fd, off, ptr);
	return ptr;
}
static int s_madvise(caddr_t addr, size_t len, int advice) {
	/* MADV_DONTNEED */
#ifndef SEMIHOSTING
	debug("madvise: addr=%p, len=%ld, advice=%d\n",
		  addr, len, advice);
	return madvise(addr, len, advice);
#else
	return 0;
#endif
}

int s_munmap(void *addr, size_t len) {
#ifndef SEMIHOSTING
	return munmap(addr, len);
#else
	return 0;
#endif
}

UNIMPLEMENTED(s_write)

size_t s_strlen(const char *s);
char *s_strchr(const char *s, int c);
long int s_strtol(const char *nptr, char **endptr, int base);
int s_strcmp(const char *s1, const char *s2);
int s_strncmp(const char *s1, const char *s2, size_t n);
void *s_memcpy(void *dest, const void *src, size_t n);
int s_memcmp(const void *s1, const void *s2, size_t n);
void *s_memset(void *s, int c, size_t n);
void *s_memmove(void *dest, const void *src, size_t n);

#ifdef SEMIHOSTING
int clock_gettime(clockid_t clk_id,struct timespec *tp) {
	tp->tv_sec = 0;
	tp->tv_nsec = 0;
	return 0;
}
#endif
#define s_clock_gettime     clock_gettime 

#define s_stderr   0
UNIMPLEMENTED(s_abort)
UNIMPLEMENTED(s___errno)
UNIMPLEMENTED(s_strerror_r)
UNIMPLEMENTED(s___stack_chk_fail)

void *s_calloc0(size_t n, size_t size);

void *s_calloc(size_t n, size_t size) {
	void *ptr = s_calloc0(n, size);
	size_t total = n * size;
	if (total >= 20480) // todo:
		s_memset(ptr, 0, total);

	return ptr;
}

#include "jemalloc_reloc.h"
