LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ututna
LOCAL_SRC_FILES := \
    main.c \
    dummy.c \
    600.fft.s \
    601.gemm.s
LOCAL_LDFLAGS := -static
include $(BUILD_EXECUTABLE)
