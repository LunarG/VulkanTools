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
 *
 */

#ifndef ICD_ENUMERATE_DRM_H
#define ICD_ENUMERATE_DRM_H

enum icd_drm_minor_type {
    ICD_DRM_MINOR_LEGACY,
    ICD_DRM_MINOR_RENDER,

    ICD_DRM_MINOR_COUNT,
    ICD_DRM_MINOR_INVALID,
};

struct icd_drm_device {
    const void *id;
    int devid;

    void *minors[ICD_DRM_MINOR_COUNT];

    struct icd_drm_device *next;
};

struct icd_instance;

struct icd_drm_device *icd_drm_enumerate(const struct icd_instance *instance,
                                         int vendor_id);
void icd_drm_release(const struct icd_instance *instance,
                     struct icd_drm_device *devices);

const char *icd_drm_get_devnode(struct icd_drm_device *dev,
                                enum icd_drm_minor_type minor);

#endif /* ICD_ENUMERATE_DRM_H */
