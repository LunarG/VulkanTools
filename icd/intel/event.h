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

#ifndef EVENT_H
#define EVENT_H

#include "intel.h"
#include "obj.h"

struct intel_dev;

struct intel_event {
    struct intel_obj obj;
};

static inline struct intel_event *intel_event(VkEvent event)
{
    return *(struct intel_event **) &event;
}

static inline struct intel_event *intel_event_from_obj(struct intel_obj *obj)
{
    return (struct intel_event *) obj;
}

VkResult intel_event_create(struct intel_dev *dev,
                              const VkEventCreateInfo *info,
                              struct intel_event **event_ret);
void intel_event_destroy(struct intel_event *event);

VkResult intel_event_set(struct intel_event *event);
VkResult intel_event_reset(struct intel_event *event);
VkResult intel_event_get_status(struct intel_event *event);

#endif /* EVENT_H */
