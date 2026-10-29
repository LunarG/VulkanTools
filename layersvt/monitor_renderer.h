/*
 * Copyright (c) 2026 LunarG, Inc.
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
 */

#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/utility/vk_dispatch_table.h>

#include "monitor_overlay.h"

namespace monitor {

// Draws the overlay into a color attachment that is in VK_IMAGE_LAYOUT_PRESENT_SRC_KHR before and after.
struct OverlayPipeline {
    VkRenderPass render_pass = VK_NULL_HANDLE;
    VkPipelineLayout layout = VK_NULL_HANDLE;
    VkPipeline pipeline = VK_NULL_HANDLE;
};

// Without blending the panel is drawn opaque. Destroys anything it created if it fails.
[[nodiscard]] VkResult CreateOverlayPipeline(VkDevice device, const VkuDeviceDispatchTable &dispatch, VkFormat format, bool blend,
                                             OverlayPipeline &pipeline);

void DestroyOverlayPipeline(VkDevice device, const VkuDeviceDispatchTable &dispatch, OverlayPipeline &pipeline);

// Records the overlay render pass. `framebuffer` holds one image view compatible with the pipeline's render pass, and
// `panel` is OverlayLayout::image_rect, clipped to the image. Nothing is recorded if that leaves it empty.
void RecordOverlay(const VkuDeviceDispatchTable &dispatch, VkCommandBuffer command_buffer, const OverlayPipeline &pipeline,
                   VkFramebuffer framebuffer, VkExtent2D image_extent, const Rect &panel,
                   const OverlayPushConstants &push_constants);

}  // namespace monitor
