#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>

#define LIB_START  0x0000000042000000
#define LIB_SIZE   0x00200000

extern const char _binary_stdlib_bin_start[];
extern const char _binary_stdlib_bin_end[];
extern const char _binary_stdlib_bin_size[];

void s_free();

#define UNIMPLEMENTED(func) \
static void func() { \
	fprintf(stderr, "[stdlib] unimplemented: %s\n", __func__); \
	exit(-1); \
}

UNIMPLEMENTED(s_abort)
UNIMPLEMENTED(s_calloc)
UNIMPLEMENTED(s_fstat)
UNIMPLEMENTED(s_iswdigit)
UNIMPLEMENTED(s_lseek64)
UNIMPLEMENTED(s_nl_langinfo)
UNIMPLEMENTED(s_open)
UNIMPLEMENTED(s_read)
UNIMPLEMENTED(s_realloc)
UNIMPLEMENTED(s_strerror)
UNIMPLEMENTED(s_strnlen)
UNIMPLEMENTED(s_ungetc)
UNIMPLEMENTED(s_vsscanf)
UNIMPLEMENTED(s_vswscanf)
UNIMPLEMENTED(s_wcsnlen)
UNIMPLEMENTED(s_wcsrtombs)
UNIMPLEMENTED(s_wcstof)
UNIMPLEMENTED(s_wcstoumax)

UNIMPLEMENTED(s__Znam)
UNIMPLEMENTED(s___errno)
UNIMPLEMENTED(s___stack_chk_fail)
UNIMPLEMENTED(s_funlockfile)
UNIMPLEMENTED(s_isatty)
UNIMPLEMENTED(s_iswupper)
UNIMPLEMENTED(s_isxdigit)
UNIMPLEMENTED(s_mbrtowc)
UNIMPLEMENTED(s_memchr)
UNIMPLEMENTED(s_mmap)
UNIMPLEMENTED(s_pthread_mutex_lock)
UNIMPLEMENTED(s_rmdir)
UNIMPLEMENTED(s_setvbuf)
UNIMPLEMENTED(s_unlink)
UNIMPLEMENTED(s_wcslen)
UNIMPLEMENTED(s_wcstod)
UNIMPLEMENTED(s_wcstoimax)
UNIMPLEMENTED(s_wctob)
UNIMPLEMENTED(s_write)

UNIMPLEMENTED(s___ctype_get_mb_cur_max)
UNIMPLEMENTED(s_btowc)
UNIMPLEMENTED(s_close)
UNIMPLEMENTED(s_dup3)
UNIMPLEMENTED(s_fcntl)
UNIMPLEMENTED(s_flockfile)
UNIMPLEMENTED(s_getdelim)
UNIMPLEMENTED(s_getpagesize)
UNIMPLEMENTED(s_isalnum)
UNIMPLEMENTED(s_iswspace)
UNIMPLEMENTED(s_malloc)
UNIMPLEMENTED(s_munmap)
UNIMPLEMENTED(s_strtod)
UNIMPLEMENTED(s_strtold)
UNIMPLEMENTED(s_vasprintf)
UNIMPLEMENTED(s_vdprintf)
UNIMPLEMENTED(s_vswprintf)

UNIMPLEMENTED(s_iswalnum)
UNIMPLEMENTED(s_iswxdigit)
UNIMPLEMENTED(s_mbrlen)
UNIMPLEMENTED(s_pthread_mutex_unlock)
UNIMPLEMENTED(s_strtof)
UNIMPLEMENTED(s_wcrtomb)
UNIMPLEMENTED(s_wcstold)

#include "stdlib_reloc.h"
