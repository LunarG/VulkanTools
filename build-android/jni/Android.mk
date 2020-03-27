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
LOCAL_SRC_FILES += $(LVL_DIR)/layers/vk_format_utils.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(THIRD_PARTY)/Vulkan-Headers/include \
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

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_device_simulation
LOCAL_SRC_FILES += $(SRC_DIR)/layersvt/device_simulation.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layersvt/vk_layer_table.cpp
LOCAL_SRC_FILES += $(ANDROID_DIR)/third_party/jsoncpp/dist/jsoncpp.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(ANDROID_DIR)/third_party/jsoncpp/dist \
                    $(LOCAL_PATH)/$(THIRD_PARTY)/Vulkan-Headers/include \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers/generated \
                    $(LOCAL_PATH)/$(SRC_DIR)/layersvt \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable -mxgot
LOCAL_CPPFLAGS += -DVK_ENABLE_BETA_EXTENSIONS -DVK_USE_PLATFORM_ANDROID_KHR -fexceptions
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_vktrace_layer
LOCAL_SRC_FILES += $(LAYER_DIR)/include/vktrace_vk_vk.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_trace_packet_utils.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_filelike.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_interconnect.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_platform.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_process.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_settings.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_tracelog.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_pageguard_memorycopy.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_trace.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_helpers.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_vk_exts.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pagestatusarray.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguardmappedmemory.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguardcapture.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguard.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_trim.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_trim_generate.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_trim_statetracker.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_trim_descriptoriterator.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/vktrace/include \
                    $(LOCAL_PATH)/$(THIRD_PARTY)/Vulkan-Headers/include \
                    $(LOCAL_PATH)/$(THIRD_PARTY)/Vulkan-Headers/include/vulkan \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers/generated \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_common \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_layer \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguardmappedmemory.h \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguardcapture.h \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguard.h \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_common/vktrace_pageguard_memorycopy.h
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable -mxgot
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_CPPFLAGS += -DPLATFORM_LINUX=1
LOCAL_CPPFLAGS += -DPAGEGUARD_MEMCPY_USE_PPL_LIB
LOCAL_CPPFLAGS += -DVK_ENABLE_BETA_EXTENSIONS
LOCAL_CFLAGS += -DPLATFORM_LINUX=1
LOCAL_CFLAGS += -DPLATFORM_POSIX=1
LOCAL_CFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_CFLAGS += -DVK_ENABLE_BETA_EXTENSIONS
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := vkreplay
LOCAL_SRC_FILES += $(LAYER_DIR)/include/vkreplay_vk_replay_gen.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_trace_packet_utils.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_filelike.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_interconnect.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_platform.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_process.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_settings.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_tracelog.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_common/vktrace_pageguard_memorycopy.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_replay/vkreplay_factory.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_replay/vkreplay_main.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_replay/vkreplay_seq.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_replay/vkreplay.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_replay/vkreplay_settings.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_replay/vkreplay_vkdisplay.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_replay/vkreplay_vkreplay.cpp
LOCAL_SRC_FILES += $(THIRD_PARTY)/Vulkan-Tools/common/vulkan_wrapper.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layersvt/screenshot_parsing.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/vktrace/include \
                    $(LOCAL_PATH)/$(THIRD_PARTY)/Vulkan-Headers/include \
                    $(LOCAL_PATH)/$(THIRD_PARTY)/Vulkan-Headers/include/vulkan \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers \
                    $(LOCAL_PATH)/$(LVL_DIR)/layers/generated \
                    $(LOCAL_PATH)/$(SRC_DIR)/layersvt \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_common \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_layer \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_replay
LOCAL_STATIC_LIBRARIES += layer_utils android_native_app_glue
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable -mxgot
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR --include=$(THIRD_PARTY)/Vulkan-Tools/common/vulkan_wrapper.h -fexceptions
LOCAL_CPPFLAGS += -DPLATFORM_LINUX=1
LOCAL_CPPFLAGS += -DPAGEGUARD_MEMCPY_USE_PPL_LIB
LOCAL_CPPFLAGS += -DVK_ENABLE_BETA_EXTENSIONS
LOCAL_CFLAGS += -DPLATFORM_LINUX=1
LOCAL_CFLAGS += -DPLATFORM_POSIX=1
LOCAL_CFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_CFLAGS += -DVK_ENABLE_BETA_EXTENSIONS
LOCAL_LDLIBS    := -llog -landroid
LOCAL_LDFLAGS   := -u ANativeActivity_onCreate
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,third_party/googletest)
