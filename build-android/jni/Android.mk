# Copyright 2015-2019 The Android Open Source Project
# Copyright (C) 2015-2019 Valve Corporation
# Copyright (C) 2015-2019 LunarG, Inc.

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#      http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)
SRC_DIR := ../..
LAYER_DIR := ../generated
THIRD_PARTY := ../third_party
LVL_DIR := $(THIRD_PARTY)/Vulkan-ValidationLayers
ANDROID_DIR := $(SRC_DIR)/build-android

include $(CLEAR_VARS)
LOCAL_MODULE := layer_utils
LOCAL_SRC_FILES += $(LVL_DIR)/layers/vk_layer_config.cpp
LOCAL_SRC_FILES += $(LVL_DIR)/layers/vk_layer_extension_utils.cpp
LOCAL_SRC_FILES += $(LVL_DIR)/layers/vk_layer_utils.cpp
LOCAL_SRC_FILES += $(LVL_DIR)/layers/generated/vk_format_utils.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vku/vk_layer_settings.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(THIRD_PARTY)/Vulkan-Headers/include \
                    $(SRC_DIR)/vku \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers/generated
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable
LOCAL_CPPFLAGS += -DVK_ENABLE_BETA_EXTENSIONS -DVK_USE_PLATFORM_ANDROID_KHR -DVK_PROTOTYPES -fvisibility=hidden
include $(BUILD_STATIC_LIBRARY)

# Pick up VLF layers
include $(LOCAL_PATH)/$(LAYER_DIR)/include/Android.mk

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_api_dump
LOCAL_SRC_FILES += $(LAYER_DIR)/include/api_dump.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layersvt/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(THIRD_PARTY)/Vulkan-Headers/include \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers/generated \
                    $(LOCAL_PATH)/$(SRC_DIR)/layersvt \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable -mxgot
LOCAL_CPPFLAGS += -DVK_ENABLE_BETA_EXTENSIONS -DVK_USE_PLATFORM_ANDROID_KHR -DVK_PROTOTYPES -fvisibility=hidden
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_screenshot
LOCAL_SRC_FILES += $(SRC_DIR)/layersvt/screenshot.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layersvt/screenshot_parsing.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layersvt/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(THIRD_PARTY)/Vulkan-Headers/include \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers/generated \
                    $(LOCAL_PATH)/$(SRC_DIR)/layersvt \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable -mxgot
LOCAL_CPPFLAGS += -DVK_ENABLE_BETA_EXTENSIONS -DVK_USE_PLATFORM_ANDROID_KHR -DVK_PROTOTYPES -fvisibility=hidden
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,third_party/googletest)
