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
 * Author: Mark Lobodzinski <mark@lunarg.com>
 * Author: Tobin Ehlis <tobin@lunarg.com>
 *
 */

#ifndef GPU_H
#define GPU_H

#include "intel.h"
#include "extension_info.h"

#define INTEL_GPU_ASSERT(gpu, min_gen, max_gen)   \
       assert(intel_gpu_gen(gpu) >= INTEL_GEN(min_gen) && \
              intel_gpu_gen(gpu) <= INTEL_GEN(max_gen))

enum intel_gpu_engine_type {
    /* TODO BLT support */
    INTEL_GPU_ENGINE_3D,

    INTEL_GPU_ENGINE_COUNT
};

struct intel_instance;
struct intel_wsi_display;
struct intel_winsys;

/*
 * intel_gpu is the only object that does not inherit from intel_base.
 */
struct intel_gpu {
    struct intel_handle handle;

    struct intel_gpu *next;

    int devid;          /* PCI device ID */
    char *primary_node; /* path to the primary node */
    char *render_node;  /* path to the render node */
    int gen_opaque;     /* always read this with intel_gpu_gen() */
    int gt;

    VkDeviceSize max_batch_buffer_size;
    uint32_t batch_buffer_reloc_count;

    /*
     * The enabled hardware features could be limited by the kernel.  These
     * mutable fds allows us to talk to the kernel before the device is
     * created.
     */
    int primary_fd_internal;
    int render_fd_internal;

    struct intel_winsys *winsys;

    struct intel_wsi_display **displays;
    uint32_t display_count;
};

static inline struct intel_gpu *intel_gpu(VkPhysicalDevice gpu)
{
    return (struct intel_gpu *) gpu;
}

static inline int intel_gpu_gen(const struct intel_gpu *gpu)
{
#ifdef INTEL_GEN_SPECIALIZED
    return INTEL_GEN(INTEL_GEN_SPECIALIZED);
#else
    return gpu->gen_opaque;
#endif
}

VkResult intel_gpu_create(const struct intel_instance *instance, int devid,
                            const char *primary_node, const char *render_node,
                            struct intel_gpu **gpu_ret);
void intel_gpu_destroy(struct intel_gpu *gpu);


void intel_gpu_get_props(const struct intel_gpu *gpu,
                         VkPhysicalDeviceProperties *props);

void intel_gpu_get_sparse_properties(VkPhysicalDeviceSparseProperties *pProps);

void intel_gpu_get_limits(VkPhysicalDeviceLimits *pLimits);

void intel_gpu_get_queue_props(const struct intel_gpu *gpu,
                               enum intel_gpu_engine_type engine,
                               VkQueueFamilyProperties *props);
void intel_gpu_get_memory_props(const struct intel_gpu *gpu,
                                VkPhysicalDeviceMemoryProperties *props);

int intel_gpu_get_max_threads(const struct intel_gpu *gpu,
                              VkShaderStageFlagBits stage);

int intel_gpu_get_primary_fd(struct intel_gpu *gpu);

VkResult intel_gpu_init_winsys(struct intel_gpu *gpu);
void intel_gpu_cleanup_winsys(struct intel_gpu *gpu);

enum intel_phy_dev_ext_type intel_gpu_lookup_phy_dev_extension(const struct intel_gpu *gpu,
        const char *extensionName);

#endif /* GPU_H */
