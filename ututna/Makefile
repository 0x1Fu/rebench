UNAME_S := $(shell uname -s)

ifeq ($(ANDROID_NDK_HOME),)
  $(error ANDROID_NDK_HOME is not set)
endif

export PATH := $(ANDROID_NDK_HOME):$(PATH)
SHELL = /bin/bash

NDK_BUILD_PARAMS := NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk

all: android
	@echo done

android:
	@ndk-build $(NDK_BUILD_PARAMS) V=0

semi:
	@make -f Makefile.semi

clean:
	@ndk-build clean $(NDK_BUILD_PARAMS)
	@make -f Makefile.semi clean

.PHONY: all
