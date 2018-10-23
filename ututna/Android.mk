LOCAL_PATH := $(call my-dir)
ASM_DIR := out

include $(CLEAR_VARS)
LOCAL_MODULE := ututna
LOCAL_SRC_FILES := \
	main.c \
	dummy.c \
	test_png.c \
	reloc.c \
	ututna.s \
	../stdlib/jemalloc.c \
	../stdlib/jemalloc_exports.s \
	../stdlib/stdlib.c \
	../stdlib/stdlib_test.c \
	../stdlib/stdlib_exports.s \
	$(ASM_DIR)/600_fft.s \
	$(ASM_DIR)/601_gemm.s \
	$(ASM_DIR)/603_map.s \
	$(ASM_DIR)/609_png.s \
	$(ASM_DIR)/614_physics.s \
	$(ASM_DIR)/607_hash.s
LOCAL_CFLAGS := -DDEBUG_RELOC=0
LOCAL_C_INCLUDES := ../stdlib ../stdlib/out
LOCAL_LDFLAGS := ../stdlib/out/jemalloc.o ../stdlib/out/stdlib.o -lz -static
include $(BUILD_EXECUTABLE)
