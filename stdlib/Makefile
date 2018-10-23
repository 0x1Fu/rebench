UNAME_S := $(shell uname -s)

ifeq ($(ANDROID_NDK_HOME),)
  $(error ANDROID_NDK_HOME is not set)
endif

export PATH := $(ANDROID_NDK_HOME):$(PATH)
SHELL = /bin/bash

NDK_BUILD_PARAMS := NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk

all: build
	@echo done

build:
	@ndk-build $(NDK_BUILD_PARAMS) V=0
	./stdlib

clean:
	@ndk-build clean $(NDK_BUILD_PARAMS)
	rm out/*

.PHONY: all
