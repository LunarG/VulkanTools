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

LOCAL_PATH := $(abspath $(call my-dir))
SRC_DIR := $(LOCAL_PATH)/../../
LAYER_DIR := $(LOCAL_PATH)/../generated
ANDROID_DIR := $(SRC_DIR)/build-android

include $(CLEAR_VARS)
LOCAL_MODULE := layer_utils
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_config.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_extension_utils.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_utils.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(SRC_DIR)/loader
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_core_validation
LOCAL_SRC_FILES += $(LAYER_DIR)/layer-src/core_validation/core_validation.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/layer-src/core_validation/descriptor_sets.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(LAYER_DIR)/include \
                    $(SRC_DIR)/loader \
                    $(ANDROID_DIR)/external/glslang \
                    $(ANDROID_DIR)/external/spirv-tools/include
LOCAL_STATIC_LIBRARIES += layer_utils SPIRV-Tools-prebuilt
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_image
LOCAL_SRC_FILES += $(LAYER_DIR)/layer-src/image/image.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(LAYER_DIR)/include \
                    $(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_parameter_validation
LOCAL_SRC_FILES += $(LAYER_DIR)/layer-src/parameter_validation/parameter_validation.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(LAYER_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_object_tracker
LOCAL_SRC_FILES += $(LAYER_DIR)/layer-src/object_tracker/object_tracker.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(LAYER_DIR)/include \
                    $(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_threading
LOCAL_SRC_FILES += $(LAYER_DIR)/layer-src/threading/threading.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(LAYER_DIR)/include \
                    $(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_unique_objects
LOCAL_SRC_FILES += $(LAYER_DIR)/layer-src/unique_objects/unique_objects.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(LAYER_DIR)/include \
                    $(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_swapchain
LOCAL_SRC_FILES += $(LAYER_DIR)/layer-src/swapchain/swapchain.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(LAYER_DIR)/include \
                    $(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden
LOCAL_LDLIBS    := -llog
LOCAL_LDFLAGS   += -Wl,-Bsymbolic
LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_api_dump
LOCAL_SRC_FILES += $(LAYER_DIR)/layer-src/api_dump/api_dump.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(SRC_DIR)/layersvt \
                    $(LAYER_DIR)/include \
                    $(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_screenshot
LOCAL_SRC_FILES += $(LAYER_DIR)/layer-src/screenshot/screenshot.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/common/vk_layer_table.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(SRC_DIR)/layersvt \
                    $(LAYER_DIR)/include \
                    $(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

# Pull in prebuilt shaderc
include $(CLEAR_VARS)
LOCAL_MODULE := shaderc-prebuilt
LOCAL_SRC_FILES := $(ANDROID_DIR)/external/shaderc/android_test/obj/local/$(TARGET_ARCH_ABI)/libshaderc.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := glslang-prebuilt
LOCAL_SRC_FILES := $(ANDROID_DIR)/external/shaderc/android_test/obj/local/$(TARGET_ARCH_ABI)/libglslang.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := OGLCompiler-prebuilt
LOCAL_SRC_FILES := $(ANDROID_DIR)/external/shaderc/android_test/obj/local/$(TARGET_ARCH_ABI)/libOGLCompiler.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := OSDependent-prebuilt
LOCAL_SRC_FILES := $(ANDROID_DIR)/external/shaderc/android_test/obj/local/$(TARGET_ARCH_ABI)/libOSDependent.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := HLSL-prebuilt
LOCAL_SRC_FILES := $(ANDROID_DIR)/external/shaderc/android_test/obj/local/$(TARGET_ARCH_ABI)/libHLSL.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := shaderc_util-prebuilt
LOCAL_SRC_FILES := $(ANDROID_DIR)/external/shaderc/android_test/obj/local/$(TARGET_ARCH_ABI)/libshaderc_util.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SPIRV-prebuilt
LOCAL_SRC_FILES := $(ANDROID_DIR)/external/shaderc/android_test/obj/local/$(TARGET_ARCH_ABI)/libSPIRV.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SPIRV-Tools-prebuilt
LOCAL_SRC_FILES := $(ANDROID_DIR)/external/shaderc/android_test/obj/local/$(TARGET_ARCH_ABI)/libSPIRV-Tools.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SPIRV-Tools-opt-prebuilt
LOCAL_SRC_FILES := $(ANDROID_DIR)/external/shaderc/android_test/obj/local/$(TARGET_ARCH_ABI)/libSPIRV-Tools-opt.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayerValidationTests
LOCAL_SRC_FILES += $(SRC_DIR)/tests/layer_validation_tests.cpp \
                   $(SRC_DIR)/tests/vktestbinding.cpp \
                   $(SRC_DIR)/tests/vktestframeworkandroid.cpp \
                   $(SRC_DIR)/tests/vkrenderframework.cpp \
                   $(SRC_DIR)/common/vulkan_wrapper.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(SRC_DIR)/libs \
                    $(SRC_DIR)/common \
                    $(ANDROID_DIR)/external/shaderc/libshaderc/include

LOCAL_STATIC_LIBRARIES := googletest_main layer_utils
LOCAL_SHARED_LIBRARIES += shaderc-prebuilt glslang-prebuilt OGLCompiler-prebuilt OSDependent-prebuilt HLSL-prebuilt shaderc_util-prebuilt SPIRV-prebuilt SPIRV-Tools-prebuilt SPIRV-Tools-opt-prebuilt
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
LOCAL_C_INCLUDES += $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(SRC_DIR)/libs \
                    $(SRC_DIR)/common \
                    $(ANDROID_DIR)/external/shaderc/libshaderc/include

LOCAL_STATIC_LIBRARIES := googletest_main layer_utils
LOCAL_SHARED_LIBRARIES += shaderc-prebuilt glslang-prebuilt OGLCompiler-prebuilt OSDependent-prebuilt HLSL-prebuilt shaderc_util-prebuilt SPIRV-prebuilt SPIRV-Tools-prebuilt SPIRV-Tools-opt-prebuilt
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -fvisibility=hidden -DVALIDATION_APK --include=$(SRC_DIR)/common/vulkan_wrapper.h
LOCAL_WHOLE_STATIC_LIBRARIES += android_native_app_glue
LOCAL_LDLIBS := -llog -landroid
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VkLayer_vktrace_layer
LOCAL_SRC_FILES += $(LAYER_DIR)/include/vktrace_vk_vk.cpp
LOCAL_SRC_FILES += $(LAYER_DIR)/include/vk_struct_size_helper.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_trace_packet_utils.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_filelike.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_interconnect.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_platform.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_process.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_settings.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_tracelog.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_pageguard_memorycopy.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_layer/vktrace_lib_trace.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_layer/vktrace_vk_exts.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_layer/vktrace_lib_pagestatusarray.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_layer/vktrace_lib_pageguardmappedmemory.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_layer/vktrace_lib_pageguardcapture.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_layer/vktrace_lib_pageguard.cpp

LOCAL_C_INCLUDES += $(SRC_DIR)/vktrace/include \
		    $(SRC_DIR)/include \
                    $(SRC_DIR)/layers \
                    $(LAYER_DIR)/include \
                    $(SRC_DIR)/vktrace/src/vktrace_common \
                    $(SRC_DIR)/vktrace/src/vktrace_layer \
                    $(SRC_DIR)/loader \
                    $(SRC_DIR)/vktrace/src/vktrace_layer/vktrace_lib_pageguardmappedmemory.h \
                    $(SRC_DIR)/vktrace/src/vktrace_layer/vktrace_lib_pageguardcapture.h \
                    $(SRC_DIR)/vktrace/src/vktrace_layer/vktrace_lib_pageguard.h \
                    $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_pageguard_memorycopy.h
LOCAL_STATIC_LIBRARIES += layer_utils
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_CPPFLAGS += -DPLATFORM_LINUX=1
LOCAL_CPPFLAGS += -DPAGEGUARD_MEMCPY_USE_PPL_LIB
LOCAL_CFLAGS += -DPLATFORM_LINUX=1
LOCAL_CFLAGS += -DPLATFORM_POSIX=1
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := vkreplay
LOCAL_SRC_FILES += $(LAYER_DIR)/include/vk_struct_size_helper.c
LOCAL_SRC_FILES += $(LAYER_DIR)/include/vkreplay_vk_replay_gen.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_trace_packet_utils.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_filelike.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_interconnect.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_platform.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_process.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_settings.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_common/vktrace_tracelog.c
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_replay/vkreplay_factory.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_replay/vkreplay_main.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_replay/vkreplay_seq.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_extensions/vktracevulkan/vkreplay/vkreplay.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_extensions/vktracevulkan/vkreplay/vkreplay_settings.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_extensions/vktracevulkan/vkreplay/vkreplay_vkdisplay.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/vktrace/src/vktrace_extensions/vktracevulkan/vkreplay/vkreplay_vkreplay.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/common/vulkan_wrapper.cpp
LOCAL_C_INCLUDES += $(SRC_DIR)/vktrace/include \
                    $(SRC_DIR)/include \
                    $(SRC_DIR)/include/vulkan \
                    $(SRC_DIR)/layers \
                    $(LAYER_DIR)/include \
                    $(SRC_DIR)/vktrace/src/vktrace_common \
                    $(SRC_DIR)/vktrace/src/vktrace_layer \
                    $(SRC_DIR)/vktrace/src/vktrace_replay \
                    $(SRC_DIR)/vktrace/src/vktrace_extensions/vktracevulkan/vkreplay \
                    $(SRC_DIR)/loader
LOCAL_STATIC_LIBRARIES += layer_utils android_native_app_glue
LOCAL_SHARED_LIBRARIES += VkLayer_vktrace_layer
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -DAPI_LOWERCASE=\"vulkan\" --include=$(SRC_DIR)/common/vulkan_wrapper.h -fexceptions
LOCAL_CPPFLAGS += -DPLATFORM_LINUX=1
LOCAL_CFLAGS += -DPLATFORM_LINUX=1
LOCAL_CFLAGS += -DPLATFORM_POSIX=1
LOCAL_LDLIBS    := -llog -landroid
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,third_party/googletest)
