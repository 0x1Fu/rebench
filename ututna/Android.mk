LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ututna
LOCAL_SRC_FILES := \
    main.c \
    dummy.c \
    test_png.c \
    ututna.s \
    600_fft.s \
    601_gemm.s \
    603_map.s \
    609_png.s \
    614_physics.s \
    607_hash.s
LOCAL_C_INCLUDES := #../stdlib
LOCAL_LDFLAGS := -lz -static
include $(BUILD_EXECUTABLE)
