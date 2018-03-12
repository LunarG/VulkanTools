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

class ZeroCounts : public layer_factory {
   public:
    // Constructor for interceptor
    ZeroCounts() : layer_factory(this){};

    // Intercept CmdDraw calls and check instanceCount
    void PreCallCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,
                        uint32_t firstInstance) {
        if (instanceCount == 0) {
            Warning("Warning: You are calling vkCmdDraw with an instanceCount of Zero.");
        }
    };

    // Intercept CmdDrawIndexed calls and check instanceCount
    void PreCallCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                               int32_t vertexOffset, uint32_t firstInstance) {
        if (instanceCount == 0) {
            Warning("Warning: You are calling vkCmdDrawIndexed with an instanceCount of Zero.");
        }
    };

    // Intercept CmdDrawIndirect calls and check drawCount
    void PreCallCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount,
                                uint32_t stride) {
        if (drawCount == 0) {
            Warning("Warning: You are calling vkCmdDrawIndirect with an instanceCount of Zero.");
        }
    };

    // Intercept CmdDrawIndexedIndirect calls and check drawCount
    void PreCallCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount,
                                       uint32_t stride) {
        if (drawCount == 0) {
            Warning("Warning: You are calling vkCmdDrawIndexedIndirect with a drawCount of Zero.");
        }
    };

    // Intercept CmdDispatch call and check groupCounts
    void PreCallCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
        if ((groupCountX == 0) || (groupCountY == 0) || (groupCountZ == 0)) {
            std::stringstream message;
            message << "Warning: You are calling vkCmdDispatch while one or more groupCounts are zero ( groupCountX = "
                    << groupCountX << ", groupCountY = " << groupCountY << ", groupCountZ = " << groupCountZ << " ).";
            Warning(message.str());
        }
    };
};

ZeroCounts warn_for_zero_counts;
