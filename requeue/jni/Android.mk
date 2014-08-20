LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := requeue
LOCAL_CFLAGS := -Wall -Werror
LOCAL_SRC_FILES := main.c

include $(BUILD_EXECUTABLE)
