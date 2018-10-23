#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <sys/mman.h>

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

void o_unimplemented();

#define DUMMY(func) \
int func() { \
	return 0; \
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
static void *s_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off) {
	void *ptr = mmap(addr, len, prot, flags, fd, off);
	debug("mmap: addr=%p, len=%ld, prot=%d, flags=%d, fd=%d, off=%ld, ret=%p\n",
		  addr, len, prot, flags, fd, off, ptr);
	return ptr;
}
static int s_madvise(caddr_t addr, size_t len, int advice) {
	/* MADV_DONTNEED */
	debug("madvise: addr=%p, len=%ld, advice=%d\n",
		  addr, len, advice);
	return madvise(addr, len, advice);
}

#define s_strlen   strlen
#define s_strchr   strchr
#define s_strtol   strtol
#define s_strcmp   strcmp
#define s_strncmp  strncmp
#define s_memcpy   memcpy
#define s_memcmp   memcmp
#define s_memset   memset
#define s_memmove  memmove
#define s_munmap   o_unimplemented
#define s_write    o_unimplemented
#define s_fprintf  o_unimplemented

#define s_clock_gettime     clock_gettime 

#define s_stderr   0
#define s_exit     o_unimplemented
#define s_abort    o_unimplemented
#define s___errno  o_unimplemented

#define s_strerror_r        o_unimplemented
#define s___stack_chk_fail  o_unimplemented

#include "jemalloc_reloc.h"
