LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ututna
LOCAL_SRC_FILES := \
    main.c \
    dummy.c \
    600_fft.s \
    601_gemm.s \
    603_map.s \
    609_png.s \
    test_png.c
LOCAL_C_INCLUDES := #../stdlib
LOCAL_LDFLAGS := -lz -static
include $(BUILD_EXECUTABLE)
