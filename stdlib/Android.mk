LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := stdlib
LOCAL_SRC_FILES := stdlib.c
LOCAL_LDFLAGS := -static
include $(BUILD_EXECUTABLE)
