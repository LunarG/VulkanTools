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
 * Author: Tony Barbour <tony@LunarG.com>
 *
 */

#include "dev.h"
#include "shader.h"
#include "compiler/shader/compiler_interface.h"

static void shader_module_destroy(struct intel_obj *obj)
{
    struct intel_shader_module *sm = intel_shader_module_from_obj(obj);

    if (sm->vs)
        shader_destroy_ir(sm->vs);
    if (sm->tcs)
        shader_destroy_ir(sm->tcs);
    if (sm->tes)
        shader_destroy_ir(sm->tes);
    if (sm->gs)
        shader_destroy_ir(sm->gs);
    if (sm->fs)
        shader_destroy_ir(sm->fs);
    if (sm->cs)
        shader_destroy_ir(sm->cs);

    free(sm->code);
    sm->code = 0;

    intel_base_destroy(&sm->obj.base);
}

const struct intel_ir *intel_shader_module_get_ir(struct intel_shader_module *sm,
                                                  VkShaderStageFlagBits stage)
{
    struct intel_ir **ir;

    switch (stage) {
    case VK_SHADER_STAGE_VERTEX_BIT:
        ir = &sm->vs;
        break;
    case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
        ir = &sm->tcs;
        break;
    case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
        ir = &sm->tes;
        break;
    case VK_SHADER_STAGE_GEOMETRY_BIT:
        ir = &sm->gs;
        break;
    case VK_SHADER_STAGE_FRAGMENT_BIT:
        ir = &sm->fs;
        break;
    case VK_SHADER_STAGE_COMPUTE_BIT:
        ir = &sm->cs;
        break;
    default:
        assert(!"unsupported shader stage");
        return NULL;
        break;
    }

    shader_create_ir_with_lock(sm->gpu, sm->code, sm->code_size, stage, ir);

    return *ir;
}

static VkResult shader_module_create(struct intel_dev *dev,
                                     const VkShaderModuleCreateInfo *info,
                                     struct intel_shader_module **sm_ret)
{
    struct intel_shader_module *sm;

    sm = (struct intel_shader_module *) intel_base_create(&dev->base.handle,
            sizeof(*sm), dev->base.dbg, VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, info, 0);
    if (!sm)
        return VK_ERROR_OUT_OF_HOST_MEMORY;

    sm->gpu = dev->gpu;
    sm->code_size = info->codeSize;
    sm->code = malloc(info->codeSize);
    if (!sm->code) {
        shader_module_destroy(&sm->obj);
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    memcpy(sm->code, info->pCode, info->codeSize);
    sm->obj.destroy = shader_module_destroy;

    *sm_ret = sm;

    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateShaderModule(
    VkDevice                                    device,
    const VkShaderModuleCreateInfo*             pCreateInfo,
    const VkAllocationCallbacks*                     pAllocator,
    VkShaderModule*                             pShaderModule)
{
    struct intel_dev *dev = intel_dev(device);

    return shader_module_create(dev, pCreateInfo, (struct intel_shader_module **) pShaderModule);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyShaderModule(
    VkDevice                                device,
    VkShaderModule                          shaderModule,
    const VkAllocationCallbacks*                     pAllocator)

 {
    struct intel_obj *obj = intel_obj(shaderModule);

    obj->destroy(obj);
 }
