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
 *
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "kmd/winsys.h"
#include "intel.h"
#include "gpu.h"
#include "obj.h"

#define INTEL_QUEUE_ATOMIC_COUNTER_COUNT 1024

struct intel_dev;
struct intel_fence;

struct intel_semaphore {
    struct intel_obj obj;

    int references;
};

static inline struct intel_semaphore *intel_semaphore(VkSemaphore semaphore)
{
    return *(struct intel_semaphore **) &semaphore;
}

static inline struct intel_semaphore *intel_semaphore_from_obj(struct intel_obj *obj)
{
    return (struct intel_semaphore *) obj;
}

void intel_semaphore_destroy(struct intel_semaphore *semaphore);

struct intel_queue {
    struct intel_base base;

    struct intel_dev *dev;
    enum intel_ring_type ring;

    struct intel_bo *atomic_bo;
    struct intel_bo *select_graphics_bo;
    struct intel_bo *select_compute_bo;

    int last_pipeline_select;

    struct intel_fence *fence;
};

static inline struct intel_queue *intel_queue(VkQueue queue)
{
    return (struct intel_queue *) queue;
}

VkResult intel_queue_create(struct intel_dev *dev,
                              enum intel_gpu_engine_type engine,
                              struct intel_queue **queue_ret);
void intel_queue_destroy(struct intel_queue *queue);

VkResult intel_queue_wait(struct intel_queue *queue, int64_t timeout);

VkResult intel_semaphore_create(struct intel_dev *dev,
                                const VkSemaphoreCreateInfo *info,
                                struct intel_semaphore **semaphore_ret);

#endif /* QUEUE_H */
