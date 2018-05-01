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

#include <string>
#include <sstream>

class PipelineCacheWarning : public layer_factory {
   public:
    // Constructor for interceptor
    PipelineCacheWarning() : layer_factory(this){};

    // Intercept graphics pipeline creation
    VkResult PreCallCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount,
                                            const VkGraphicsPipelineCreateInfo* pCreateInfos,
                                            const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
        if ((createInfoCount > 1) && (!pipelineCache)) {
            std::stringstream message;
            message << "Performance Warning:  This vkCreateGraphicsPipelines call is creating multiple pipelines but is not using "
                       "a pipeline cache, which may help with performance";
            PerformanceWarning(message.str());
        }
        return VK_SUCCESS;
    };

    // Intercept compute pipeline creation
    VkResult vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount,
                                      const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator,
                                      VkPipeline* pPipelines) {
        if ((createInfoCount > 1) && (!pipelineCache)) {
            std::stringstream message;
            message << "Performance Warning:  This vkCreateComputePipelines call is creating multiple pipelines but is not using a "
                       "pipeline cache, which may help with performance";
            PerformanceWarning(message.str());
        }
        return VK_SUCCESS;
    }
};

PipelineCacheWarning pipeline_cache_warning;
