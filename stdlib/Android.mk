LOCAL_PATH := $(call my-dir)

LIBC=android-26/libc
LIBM=android-26/libm
LIBZ=android-26/libz

include $(CLEAR_VARS)
LOCAL_MODULE := stdlib.elf
LOCAL_SRC_FILES := stdlib_main.c
LOCAL_CFLAGS := \
	-Wno-incompatible-library-redeclaration \
	-Wno-builtin-requires-header
LOCAL_LDFLAGS := \
	$(LIBC)/dmisc.o \
	$(LIBC)/dtoa.o \
	$(LIBC)/fgetwc.o \
	$(LIBC)/flags.o \
	$(LIBC)/fputwc.o \
	$(LIBC)/fvwrite.o \
	$(LIBC)/fwalk.o \
	$(LIBC)/gdtoa.o \
	$(LIBC)/gdtoa_support.o \
	$(LIBC)/gmisc.o \
	$(LIBC)/hdtoa.o \
	$(LIBC)/ldtoa.o \
	$(LIBC)/makebuf.o \
	$(LIBC)/memcmp.o \
	$(LIBC)/memcpy.o \
	$(LIBC)/memmove.o \
	$(LIBC)/memset.o \
	$(LIBC)/misc.o \
	$(LIBC)/parsefloat.o \
	$(LIBC)/refill.o \
	$(LIBC)/rget.o \
	$(LIBC)/stdio.o \
	$(LIBC)/strchr.o \
	$(LIBC)/strcmp.o \
	$(LIBC)/strlen.o \
	$(LIBC)/strncmp.o \
	$(LIBC)/strtol.o \
	$(LIBC)/ungetwc.o \
	$(LIBC)/vfprintf.o \
	$(LIBC)/vfscanf.o \
	$(LIBC)/vfwprintf.o \
	$(LIBC)/vfwscanf.o \
	$(LIBC)/wbuf.o \
	$(LIBC)/wsetup.o \
	$(LIBM)/e_pow.o \
	$(LIBM)/k_cos.o \
	$(LIBM)/k_rem_pio2.o \
	$(LIBM)/k_sin.o \
	$(LIBM)/s_cos.o \
	$(LIBM)/s_scalbn.o \
	$(LIBM)/s_sin.o \
	$(LIBM)/sqrt.o \
	$(LIBZ)/adler32.o \
	$(LIBZ)/crc32.o \
	$(LIBZ)/inflate.o \
	$(LIBZ)/inffast.o \
	$(LIBZ)/inftrees.o \
	$(LIBZ)/zutil.o
LOCAL_LDFLAGS += -nostartfiles -Wl,--entry=main
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := jemalloc.elf
LOCAL_SRC_FILES := jemalloc_main.c
LOCAL_LDFLAGS := \
	$(LIBC)/arena.o \
	$(LIBC)/base.o \
	$(LIBC)/bitmap.o \
	$(LIBC)/chunk.o \
	$(LIBC)/chunk_dss.o \
	$(LIBC)/chunk_mmap.o \
	$(LIBC)/ckh.o \
	$(LIBC)/ctl.o \
	$(LIBC)/extent.o \
	$(LIBC)/huge.o \
	$(LIBC)/jemalloc.o \
	$(LIBC)/mutex.o \
	$(LIBC)/nstime.o \
	$(LIBC)/pages.o \
	$(LIBC)/prof.o \
	$(LIBC)/quarantine.o \
	$(LIBC)/rtree.o \
	$(LIBC)/stats.o \
	$(LIBC)/tcache.o \
	$(LIBC)/tsd.o \
	$(LIBC)/util.o \
	$(LIBC)/witness.o
LOCAL_LDFLAGS += -nostartfiles -Wl,--entry=main
include $(BUILD_EXECUTABLE)
