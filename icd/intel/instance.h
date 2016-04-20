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

#ifndef INSTANCE_H
#define INSTANCE_H

#include "intel.h"
#include "extension_info.h"

struct intel_gpu;

struct intel_instance {
    struct intel_handle handle;

    struct icd_instance *icd;

    struct intel_gpu *gpus;
    bool global_exts[INTEL_GLOBAL_EXT_COUNT];
};

static inline struct intel_instance *intel_instance(VkInstance instance)
{
    return (struct intel_instance *) instance;
}

enum intel_global_ext_type intel_gpu_lookup_global_extension(const char *extensionName);
#endif /* INSTANCE_H */
