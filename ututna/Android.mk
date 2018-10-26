LOCAL_PATH := $(call my-dir)
ASM_DIR := out

SRC_TESTS =	\
	$(ASM_DIR)/600_fft.s \
	$(ASM_DIR)/601_gemm.s \
	$(ASM_DIR)/603_map.s \
	$(ASM_DIR)/609_png.s \
	$(ASM_DIR)/614_physics.s \
	$(ASM_DIR)/607_hash.s

include $(CLEAR_VARS)
LOCAL_MODULE := ututna
LOCAL_SRC_FILES := \
	main.c \
	bootstrap.c \
	dummy.c \
	test_png.c \
	reloc.c \
	ututna.s \
	../stdlib/jemalloc.c \
	../stdlib/jemalloc_exports.s \
	../stdlib/stdlib.c \
	../stdlib/stdlib_test.c \
	../stdlib/stdlib_exports.s \
	$(SRC_TESTS)

LOCAL_CFLAGS := -DDEBUG_RELOC=0
LOCAL_C_INCLUDES := ../stdlib ../stdlib/out
LOCAL_LDFLAGS := ../stdlib/out/jemalloc.o ../stdlib/out/stdlib.o -lz -static

ifeq ($(TARGET),dummy)
LOCAL_SRC_FILES := \
	stub.s \
	$(SRC_TESTS)
TARGET_LDLIBS   :=
LOCAL_LDFLAGS   += --verbose -Wl,--entry=main -nostdlib
include $(BUILD_SHARED_LIBRARY)
else
include $(BUILD_EXECUTABLE)
endif
