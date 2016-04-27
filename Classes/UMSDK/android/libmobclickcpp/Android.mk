LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := mobclickcpp_shared
LOCAL_MODULE_FILENAME := libmobclickcpp
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libmobclickcpp.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_SHARED_LIBRARY)
