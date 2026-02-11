/* Copyright (c) 2015-2023 The Khronos Group Inc.
 * Copyright (c) 2015-2023 Valve Corporation
 * Copyright (c) 2015-2023 LunarG, Inc.
 * Copyright (C) 2015-2016 Google Inc.
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
 * Author: Lenny Komow <lenny@lunarg.com>
 * Author: Shannon McPherson <shannon@lunarg.com>
 * Author: David Pinedo <david@lunarg.com>
 * Author: Charles Giessen <charles@lunarg.com>
 */

// Implementation file for specifically implemented functions

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#include "generated/api_dump_dispatch.h"

extern "C" {

EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* pName) {
    PFN_vkVoidFunction instance_func = nullptr;
    instance_func = api_dump_known_instance_functions(instance, pName);
    if (instance_func) return instance_func;
    PFN_vkVoidFunction device_func = nullptr;

    device_func = api_dump_known_device_functions(NULL, pName);

    // Make sure that device functions queried through GIPA works
    if (device_func) return device_func;

    // Haven't created an instance yet, exit now since there is no instance_dispatch_table
    if (instance_dispatch_table(instance)->GetInstanceProcAddr == NULL) return nullptr;
    return instance_dispatch_table(instance)->GetInstanceProcAddr(instance, pName);
}

EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* pName) {
    PFN_vkVoidFunction device_func = nullptr;

    device_func = api_dump_known_device_functions(device, pName);

    if (device_func) return device_func;

    // Haven't created a device yet, exit now since there is no device_dispatch_table
    if (device_dispatch_table(device)->GetDeviceProcAddr == NULL) return nullptr;
    return device_dispatch_table(device)->GetDeviceProcAddr(device, pName);
}
}
