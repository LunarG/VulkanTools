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
 * Author: Chris Forbes <chrisf@ijw.co.nz>
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 *
 */

#ifndef SHADER_H
#define SHADER_H

#include "intel.h"
#include "obj.h"

struct intel_ir;

struct intel_shader_module {
    struct intel_obj obj;

    const struct intel_gpu *gpu;
    /* content is just a copy of the SPIRV image */
    uint32_t code_size;
    void *code;

    /* simple cache */
    struct intel_ir *vs;
    struct intel_ir *tcs;
    struct intel_ir *tes;
    struct intel_ir *gs;
    struct intel_ir *fs;
    struct intel_ir *cs;
};

static inline struct intel_shader_module *intel_shader_module(VkShaderModule shaderModule)
{
    return *(struct intel_shader_module **) &shaderModule;
}

static inline struct intel_shader_module *intel_shader_module_from_obj(struct intel_obj *obj)
{
    return (struct intel_shader_module *)obj;
}

const struct intel_ir *intel_shader_module_get_ir(struct intel_shader_module *sm,
                                                  VkShaderStageFlagBits stage);

#endif /* SHADER_H */
