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
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libudev.h>

#include "icd-instance.h"
#include "icd-utils.h"
#include "icd-enumerate-drm.h"

static enum icd_drm_minor_type get_minor_type(struct udev_device *minor_dev)
{
    const char *minor;

    minor = udev_device_get_property_value(minor_dev, "MINOR");
    if (!minor)
        return ICD_DRM_MINOR_INVALID;

    switch (atoi(minor) >> 6) {
    case 0:
        return ICD_DRM_MINOR_LEGACY;
    case 2:
        return ICD_DRM_MINOR_RENDER;
    default:
        return ICD_DRM_MINOR_INVALID;
    }
}

static void get_pci_id(struct udev_device *pci_dev, int *vendor, int *devid)
{
    const char *pci_id;

    pci_id = udev_device_get_property_value(pci_dev, "PCI_ID");
    if (sscanf(pci_id, "%x:%x", vendor, devid) != 2) {
        *vendor = 0;
        *devid = 0;
    }
}

static struct icd_drm_device *find_dev(struct icd_drm_device *devices,
                                       const char *parent_syspath)
{
    struct icd_drm_device *dev = devices;

    while (dev) {
        if (!strcmp((const char *) dev->id, parent_syspath))
            break;
        dev = dev->next;
    }

    return dev;
}

static struct icd_drm_device *probe_syspath(const struct icd_instance *instance,
                                            struct icd_drm_device *devices,
                                            struct udev *udev, const char *syspath,
                                            int vendor_id_match)
{
    struct udev_device *minor, *parent;
    enum icd_drm_minor_type type;
    const char *parent_syspath;
    struct icd_drm_device *dev;
    int vendor, devid;

    minor = udev_device_new_from_syspath(udev, syspath);
    if (!minor)
        return devices;

    type = get_minor_type(minor);
    if (type == ICD_DRM_MINOR_INVALID) {
        udev_device_unref(minor);
        return devices;
    }

    parent = udev_device_get_parent(minor);
    if (!parent) {
        udev_device_unref(minor);
        return devices;
    }

    get_pci_id(parent, &vendor, &devid);
    if (vendor_id_match && vendor != vendor_id_match) {
        udev_device_unref(minor);
        return devices;
    }

    parent_syspath = udev_device_get_syspath(parent);

    dev = find_dev(devices, parent_syspath);
    if (dev) {
        assert(dev->devid == devid);

        assert(!dev->minors[type]);
        if (dev->minors[type])
            udev_device_unref((struct udev_device *) dev->minors[type]);

        dev->minors[type] = (void *) minor;

        return devices;
    } else {
        dev = icd_instance_alloc(instance, sizeof(*dev), sizeof(int),
                VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
        if (!dev)
            return devices;

        memset(dev, 0, sizeof(*dev));

        dev->id = (const void *) parent_syspath;
        dev->devid = devid;
        dev->minors[type] = (void *) minor;

        dev->next = devices;

        return dev;
    }
}

struct icd_drm_device *icd_drm_enumerate(const struct icd_instance *instance,
                                         int vendor_id)
{
    struct icd_drm_device *devices = NULL;
    struct udev *udev;
    struct udev_enumerate *e;
    struct udev_list_entry *entry;

    udev = udev_new();
    if (udev == NULL) {
        icd_instance_log(instance, VK_DEBUG_REPORT_ERROR_BIT_EXT,
                         0, VK_NULL_HANDLE,     /* obj_type, object */
                         0, 0,                  /* location, msg_code */
                         "failed to initialize udev context");

        return NULL;
    }

    e = udev_enumerate_new(udev);
    if (e == NULL) {
        icd_instance_log(instance, VK_DEBUG_REPORT_ERROR_BIT_EXT,
                         0, VK_NULL_HANDLE,     /* obj_type, object */
                         0, 0,                  /* location, msg_code */
                         "failed to initialize udev enumerate context");
        udev_unref(udev);

        return NULL;
    }

    /* we are interested in DRM minors */
    udev_enumerate_add_match_subsystem(e, "drm");
    udev_enumerate_add_match_property(e, "DEVTYPE", "drm_minor");
    udev_enumerate_scan_devices(e);

    udev_list_entry_foreach(entry, udev_enumerate_get_list_entry(e)) {
        devices = probe_syspath(instance, devices, udev,
                udev_list_entry_get_name(entry), vendor_id);
    }

    free(e);
    free(udev);
    return devices;
}

void icd_drm_release(const struct icd_instance *instance,
                     struct icd_drm_device *devices)
{
    struct icd_drm_device *dev = devices;

    while (dev) {
        struct icd_drm_device *next = dev->next;
        size_t i;

        for (i = 0; i < ARRAY_SIZE(dev->minors); i++)
            udev_device_unref((struct udev_device *) dev->minors[i]);

        icd_instance_free(instance, dev);
        dev = next;
    }
}

const char *icd_drm_get_devnode(struct icd_drm_device *dev,
                                enum icd_drm_minor_type minor)
{
    return (dev->minors[minor]) ?
        udev_device_get_devnode((struct udev_device *) dev->minors[minor]) :
        NULL;
}
