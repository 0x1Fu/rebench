#ifndef __jemalloc_h__
#define __jemalloc_h__

void jemalloc_init();

void *s_malloc(size_t size);
void *s_calloc(size_t n, size_t size);
void *s_realloc(void* ptr, size_t size);
void s_free(void *ptr);

#endif
