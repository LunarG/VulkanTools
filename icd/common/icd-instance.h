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
 * Author: Chia-I Wu <olv@lunarg.com>
 *
 */

#ifndef ICD_INSTANCE_H
#define ICD_INSTANCE_H

#include "icd-utils.h"
#include "icd.h"

#ifdef __cplusplus
extern "C" {
#endif

struct icd_instance_logger {
    PFN_vkDebugReportCallbackEXT func;
    void *user_data;
    VkFlags flags;

    struct icd_instance_logger *next;
};

struct icd_instance {
    char *name;

    VkAllocationCallbacks alloc_cb;

    struct icd_instance_logger *loggers;
};

struct icd_instance *icd_instance_create(const VkApplicationInfo *app_info,
                                         const VkAllocationCallbacks *alloc_cb);
void icd_instance_destroy(struct icd_instance *instance);

static inline void *icd_instance_alloc(const struct icd_instance *instance,
                                       size_t size, size_t alignment,
                                       VkSystemAllocationScope scope)
{
    return instance->alloc_cb.pfnAllocation(instance->alloc_cb.pUserData,
            size, alignment, scope);
}

static inline void icd_instance_free(const struct icd_instance *instance,
                                     void *ptr)
{
    instance->alloc_cb.pfnFree(instance->alloc_cb.pUserData, ptr);
}

VkResult icd_instance_create_logger(struct icd_instance *instance,
        const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugReportCallbackEXT *msg_obj);

void icd_instance_destroy_logger(struct icd_instance *instance,
        const VkDebugReportCallbackEXT msg_obj, const VkAllocationCallbacks *pAllocator);

void icd_instance_log(const struct icd_instance *instance,
                      VkFlags msg_flags,
                      VkDebugReportObjectTypeEXT obj_type,
                      uint64_t src_object,
                      size_t location, int32_t msg_code,
                      const char *msg);

#ifdef __cplusplus
}
#endif

#endif /* ICD_INSTANCE_H */
