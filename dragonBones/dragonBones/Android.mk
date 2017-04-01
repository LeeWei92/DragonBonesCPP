LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../cocos2d)

define walk
$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

#遍历LOCAL_PATH目录
ALLFILES = $(call walk, $(LOCAL_PATH))

#从所有文件中提取出所有的.cpp和.c文件
FILE_LIST += $(filter %.cpp %.c, $(ALLFILES))
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_MODULE := dragonBones_static

LOCAL_MODULE_FILENAME := libdragonBones


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/framework \
						   $(LOCAL_PATH)/dragonBones-cocos2dx \
						   $(LOCAL_PATH)/../cocos2d/external \
						   $(LOCAL_PATH)/../cocos2d/cocos

LOCAL_C_INCLUDES := $(LOCAL_PATH)/framework \
					$(LOCAL_PATH)/dragonBones-cocos2dx \
					$(LOCAL_PATH)/../cocos2d/external \
					$(LOCAL_PATH)/../cocos2d/cocos

include $(BUILD_STATIC_LIBRARY)
