/*
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Chia-I Wu <olvaffe@gmail.com>
 * Author: Chia-I Wu <olv@lunarg.com>
 * Author: Jeremy Hayes <jeremy@lunarg.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 *
 */

#ifndef INTEL_H
#define INTEL_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <vulkan/vulkan.h>
#include <vulkan/vk_icd.h>

#include "icd.h"
#include "icd-spv.h"
#include "icd-format.h"
#include "icd-instance.h"
#include "icd-utils.h"

#define INTEL_API_VERSION VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION)
#define INTEL_DRIVER_VERSION 0

#define INTEL_GEN(gen) ((int) ((gen) * 100))

#define INTEL_MAX_VERTEX_BINDING_COUNT 33
#define INTEL_MAX_VERTEX_ELEMENT_COUNT (INTEL_MAX_VERTEX_BINDING_COUNT + 1)
#define INTEL_MAX_RENDER_TARGETS 8
#define INTEL_MAX_VIEWPORTS 16

#define INTEL_MEMORY_PROPERTY_ALL (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |\
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
#define INTEL_MEMORY_HEAP_COUNT   1
#define INTEL_MEMORY_HEAP_SIZE    (2u << 30)
#define INTEL_MEMORY_TYPE_COUNT   1

enum intel_debug_flags {
    INTEL_DEBUG_BATCH       = 1 << 0,

    INTEL_DEBUG_NOHW        = 1 << 20,
    INTEL_DEBUG_NOCACHE     = 1 << 21,
    INTEL_DEBUG_NOHIZ       = 1 << 22,
    INTEL_DEBUG_HANG        = 1 << 23,
};

struct intel_instance;

struct intel_handle {
    /* the loader expects a "void *" at the beginning */
    void *loader_data;

    uint32_t magic;
    const struct intel_instance *instance;
};

extern int intel_debug;

static const uint32_t intel_handle_magic = 0x494e544c;

static inline void intel_handle_init(struct intel_handle *handle,
                                     VkDebugReportObjectTypeEXT type,
                                     const struct intel_instance *instance)
{
    set_loader_magic_value((void *)handle);

    handle->magic = intel_handle_magic + type;
    handle->instance = instance;
}

/**
 * Return true if \p handle is a valid intel_handle.  This assumes the first
 * sizeof(intel_handle) bytes are readable, and they does not happen to have
 * our magic values.
 */
static inline bool intel_handle_validate(const void *handle)
{
//    const uint32_t handle_type =
//        ((const struct intel_handle *) handle)->magic - intel_handle_magic;

    /* TODO: this does not work for extensions, needs adjusting */
//    return (handle_type <= VK_DEBUG_REPORT_OBJECT_TYPE_END_RANGE);
    return true;
}

/**
 * Return true if \p handle is a valid intel_handle of \p type.
 *
 * \see intel_handle_validate().
 */
static inline bool intel_handle_validate_type(const void *handle,
                                              VkDebugReportObjectTypeEXT type)
{
    const uint32_t handle_type =
        ((const struct intel_handle *) handle)->magic - intel_handle_magic;

    return (handle_type == (uint32_t) type);
}

void *intel_alloc(const void *handle,
                  size_t size, size_t alignment,
                  VkSystemAllocationScope scope);

void intel_free(const void *handle, void *ptr);

void intel_logv(const void *handle,
                VkFlags msg_flags,
                VkDebugReportObjectTypeEXT obj_type, uint64_t src_object,
                size_t location, int32_t msg_code,
                const char *format, va_list ap);

static inline void intel_log(const void *handle,
                             VkFlags msg_flags,
                             VkDebugReportObjectTypeEXT obj_type, uint64_t src_object,
                             size_t location, int32_t msg_code,
                             const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    intel_logv(handle, msg_flags, obj_type, src_object,
               location, msg_code, format, ap);
    va_end(ap);
}

#endif /* INTEL_H */
