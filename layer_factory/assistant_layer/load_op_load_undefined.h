/*
 * Copyright (c) 2015-2017 Valve Corporation
 * Copyright (c) 2015-2017 LunarG, Inc.
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
 * Author: Mark Lobodzinski <mark@lunarg.com>
 */

#pragma once

#include "vector"
#include "string"
#include <algorithm>

class LoadAndUndefined : public layer_factory {
   public:
    // Constructor for interceptor
    LoadAndUndefined() : layer_factory(this){};

    // Intercept CreateRenderPassCalls calls and check LoadOp and Layout
    VkResult PreCallCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo *pCreateInfo,
                                     const VkAllocationCallbacks *pAllocator, VkRenderPass *pRenderPass) {
        for (uint32_t i = 0; i < pCreateInfo->attachmentCount; ++i) {
            VkFormat format = pCreateInfo->pAttachments[i].format;
            if (pCreateInfo->pAttachments[i].initialLayout == VK_IMAGE_LAYOUT_UNDEFINED) {
                if ((FormatIsColor(format) || FormatHasDepth(format)) &&
                    pCreateInfo->pAttachments[i].loadOp == VK_ATTACHMENT_LOAD_OP_LOAD) {
                    Warning(
                        "Render pass has an attachment with loadOp == VK_ATTACHMENT_LOAD_OP_LOAD and "
                        "initialLayout == VK_IMAGE_LAYOUT_UNDEFINED.  This is probably not what you "
                        "intended.  Consider using VK_ATTACHMENT_LOAD_OP_DONT_CARE instead if the "
                        "image truely is undefined at the start of the render pass.");
                }
                if (FormatHasStencil(format) && pCreateInfo->pAttachments[i].stencilLoadOp == VK_ATTACHMENT_LOAD_OP_LOAD) {
                    Warning(
                        "Render pass has an attachment with stencilLoadOp == VK_ATTACHMENT_LOAD_OP_LOAD "
                        "and initialLayout == VK_IMAGE_LAYOUT_UNDEFINED.  This is probably not what you "
                        "intended.  Consider using VK_ATTACHMENT_LOAD_OP_DONT_CARE instead if the "
                        "image truely is undefined at the start of the render pass.");
                }
            }
        }
        return VK_SUCCESS;
    };
};

LoadAndUndefined load_op_load_and_layout_undefined;
