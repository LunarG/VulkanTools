# Copyright 2015 The Android Open Source Project
# Copyright (C) 2015 Valve Corporation

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
ANDROID_DIR := $(SRC_DIR)/build-android

include $(CLEAR_VARS)
LOCAL_MODULE := layer_utils
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_layer_config.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_layer_extension_utils.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_layer_utils.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_format_utils.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -DVK_PROTOTYPES -fvisibility=hidden
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_core_validation
LOCAL_SRC_FILES += $(SRC_DIR)/layers/core_validation.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/descriptor_sets.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/buffer_validation.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/shader_validation.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_layer_table.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/xxhash.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils glslang SPIRV-Tools
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -DVK_PROTOTYPES -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_parameter_validation
LOCAL_SRC_FILES += $(LAYER_DIR)/include/parameter_validation.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/parameter_validation_utils.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -DVK_PROTOTYPES -Wall -Werror -Wno-unused-function -Wno-unused-const-variable
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_object_tracker
LOCAL_SRC_FILES += $(LAYER_DIR)/include/object_tracker.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/object_tracker_utils.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -DVK_PROTOTYPES -Wall -Werror -Wno-unused-function -Wno-unused-const-variable
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_threading
LOCAL_SRC_FILES += $(SRC_DIR)/layers/threading.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -DVK_PROTOTYPES -Wall -Werror -Wno-unused-function -Wno-unused-const-variable
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_unique_objects
LOCAL_SRC_FILES += $(SRC_DIR)/layers/unique_objects.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -DVK_PROTOTYPES -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_api_dump
LOCAL_SRC_FILES += $(LAYER_DIR)/include/api_dump.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(SRC_DIR)/layersvt \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable -mxgot
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -DVK_PROTOTYPES -fvisibility=hidden
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_screenshot
LOCAL_SRC_FILES += $(SRC_DIR)/layersvt/screenshot.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layersvt/screenshot_parsing.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(SRC_DIR)/layersvt \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable -mxgot
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -DVK_PROTOTYPES -fvisibility=hidden
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_device_simulation
LOCAL_SRC_FILES += $(SRC_DIR)/layersvt/device_simulation.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layers/vk_layer_table.cpp
LOCAL_SRC_FILES += $(ANDROID_DIR)/third_party/jsoncpp/dist/jsoncpp.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(ANDROID_DIR)/third_party/jsoncpp/dist \
                    $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(SRC_DIR)/layersvt \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable -mxgot
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fexceptions
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayerValidationTests
LOCAL_SRC_FILES += $(SRC_DIR)/tests/layer_validation_tests.cpp \
                   $(SRC_DIR)/tests/vktestbinding.cpp \
                   $(SRC_DIR)/tests/vktestframeworkandroid.cpp \
                   $(SRC_DIR)/tests/vkrenderframework.cpp \
                   $(SRC_DIR)/common/vulkan_wrapper.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(SRC_DIR)/libs \
                    $(LOCAL_PATH)/$(SRC_DIR)/common

LOCAL_STATIC_LIBRARIES := googletest_main layer_utils shaderc
LOCAL_CPPFLAGS += -std=c++11 -DVK_PROTOTYPES -Wall -Werror -Wno-unused-function -Wno-unused-const-variable
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden --include=$(SRC_DIR)/common/vulkan_wrapper.h
LOCAL_LDLIBS := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_EXECUTABLE)

# Note: The following module is similar in name to the executable, but differs so that loader won't enumerate the resulting .so
include $(CLEAR_VARS)
LOCAL_MODULE := VulkanLayerValidationTests
LOCAL_SRC_FILES += $(SRC_DIR)/tests/layer_validation_tests.cpp \
                   $(SRC_DIR)/tests/vktestbinding.cpp \
                   $(SRC_DIR)/tests/vktestframeworkandroid.cpp \
                   $(SRC_DIR)/tests/vkrenderframework.cpp \
                   $(SRC_DIR)/common/vulkan_wrapper.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(SRC_DIR)/libs \
                    $(LOCAL_PATH)/$(SRC_DIR)/common

LOCAL_STATIC_LIBRARIES := googletest_main layer_utils shaderc
LOCAL_CPPFLAGS += -std=c++11 -DVK_PROTOTYPES -Wall -Werror -Wno-unused-function -Wno-unused-const-variable
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden -DVALIDATION_APK --include=$(SRC_DIR)/common/vulkan_wrapper.h
LOCAL_WHOLE_STATIC_LIBRARIES += android_native_app_glue
LOCAL_LDLIBS := -llog -landroid
LOCAL_LDFLAGS := -u ANativeActivity_onCreate
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
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_vk_exts.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pagestatusarray.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguardmappedmemory.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguardcapture.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguard.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_trim.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_trim_generate.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_trim_statetracker.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/vktrace/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/include/vulkan \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_common \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_layer \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguardmappedmemory.h \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguardcapture.h \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_layer/vktrace_lib_pageguard.h \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_common/vktrace_pageguard_memorycopy.h
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable -mxgot
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_CPPFLAGS += -DPLATFORM_LINUX=1
LOCAL_CPPFLAGS += -DPAGEGUARD_MEMCPY_USE_PPL_LIB
LOCAL_CFLAGS += -DPLATFORM_LINUX=1
LOCAL_CFLAGS += -DPLATFORM_POSIX=1
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
LOCAL_SRC_FILES += $(SRC_DIR)/common/vulkan_wrapper.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/layersvt/screenshot_parsing.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/vktrace/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/include/vulkan \
                    $(LOCAL_PATH)/$(SRC_DIR)/layers \
                    $(LOCAL_PATH)/$(SRC_DIR)/layersvt \
                    $(LOCAL_PATH)/$(LAYER_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_common \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_layer \
                    $(LOCAL_PATH)/$(SRC_DIR)/vktrace/vktrace_replay \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils android_native_app_glue
LOCAL_CPPFLAGS += -std=c++11 -Wall -Werror -Wno-unused-function -Wno-unused-const-variable -mxgot
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -DAPI_LOWERCASE=\"vulkan\" --include=$(SRC_DIR)/common/vulkan_wrapper.h -fexceptions
LOCAL_CPPFLAGS += -DPLATFORM_LINUX=1
LOCAL_CPPFLAGS += -DPAGEGUARD_MEMCPY_USE_PPL_LIB
LOCAL_CFLAGS += -DPLATFORM_LINUX=1
LOCAL_CFLAGS += -DPLATFORM_POSIX=1
LOCAL_LDLIBS    := -llog -landroid
LOCAL_LDFLAGS   := -u ANativeActivity_onCreate
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := vkjson
LOCAL_SRC_FILES += $(SRC_DIR)/libs/vkjson/vkjson.cc \
                   $(SRC_DIR)/libs/vkjson/vkjson_instance.cc \
                   $(SRC_DIR)/common/vulkan_wrapper.cpp \
                   $(SRC_DIR)/loader/cJSON.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/include \
                    $(LOCAL_PATH)/$(SRC_DIR)/loader

LOCAL_CPPFLAGS += -std=c++11 -DVK_PROTOTYPES -Wall -Werror -Wno-unused-function -Wno-unused-const-variable
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden --include=$(SRC_DIR)/common/vulkan_wrapper.h
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := vkjson_info
LOCAL_SRC_FILES += $(SRC_DIR)/libs/vkjson/vkjson_info.cc \
                   $(SRC_DIR)/common/vulkan_wrapper.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/loader \
                    $(LOCAL_PATH)/$(SRC_DIR)/include

LOCAL_STATIC_LIBRARIES += vkjson
LOCAL_CPPFLAGS += -std=c++11 -DVK_PROTOTYPES -Wall -Werror -Wno-unused-function -Wno-unused-const-variable
LOCAL_CPPFLAGS += -Wno-sign-compare -DVK_USE_PLATFORM_ANDROID_KHR --include=$(SRC_DIR)/common/vulkan_wrapper.h
LOCAL_LDLIBS := -llog
LOCAL_LDFLAGS += -Wl,--exclude-libs,ALL
include $(BUILD_EXECUTABLE)

$(call import-module,android/native_app_glue)
$(call import-module,third_party/googletest)
$(call import-module,third_party/shaderc)
