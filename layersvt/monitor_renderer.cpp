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

#include "monitor_renderer.h"

#include "monitor_overlay_shaders.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

#include "monitor_overlay.h"

namespace monitor {

// The image arrives in PRESENT_SRC_KHR after whatever the app last did to it, and goes back to PRESENT_SRC_KHR.
[[nodiscard]] static VkResult CreateRenderPass(VkDevice device, const VkuDeviceDispatchTable &dispatch, VkFormat format,
                                               VkRenderPass &render_pass) {
    VkAttachmentDescription attachment = {};
    attachment.format = format;
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    constexpr VkAttachmentReference color = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color;

    // ALL_COMMANDS also covers apps that write the image with transfers, or present without a semaphore on the same queue.
    std::array<VkSubpassDependency, 2> dependencies = {};
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[1].dstAccessMask = 0;

    VkRenderPassCreateInfo info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    info.attachmentCount = 1;
    info.pAttachments = &attachment;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = static_cast<uint32_t>(dependencies.size());
    info.pDependencies = dependencies.data();
    return dispatch.CreateRenderPass(device, &info, nullptr, &render_pass);
}

template <size_t N>
[[nodiscard]] static VkResult CreateShaderModule(VkDevice device, const VkuDeviceDispatchTable &dispatch,
                                                 const std::array<uint32_t, N> &code, VkShaderModule &module) {
    VkShaderModuleCreateInfo info = {VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    info.codeSize = code.size() * sizeof(uint32_t);
    info.pCode = code.data();
    return dispatch.CreateShaderModule(device, &info, nullptr, &module);
}

[[nodiscard]] static VkResult CreateGraphicsPipeline(VkDevice device, const VkuDeviceDispatchTable &dispatch, bool blend,
                                                     OverlayPipeline &pipeline) {
    VkShaderModule vert = VK_NULL_HANDLE;
    VkShaderModule frag = VK_NULL_HANDLE;
    VkResult result = CreateShaderModule(device, dispatch, kMonitorOverlayVertSpirv, vert);
    if (result == VK_SUCCESS) {
        result = CreateShaderModule(device, dispatch, kMonitorOverlayFragSpirv, frag);
    }

    if (result == VK_SUCCESS) {
        std::array<VkPipelineShaderStageCreateInfo, 2> stages = {};
        stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        stages[0].module = vert;
        stages[0].pName = "main";
        stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        stages[1].module = frag;
        stages[1].pName = "main";

        constexpr VkPipelineVertexInputStateCreateInfo vertex_input = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
        VkPipelineInputAssemblyStateCreateInfo input_assembly = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
        input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        VkPipelineViewportStateCreateInfo viewport = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
        viewport.viewportCount = 1;
        viewport.scissorCount = 1;
        VkPipelineRasterizationStateCreateInfo rasterization = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
        rasterization.polygonMode = VK_POLYGON_MODE_FILL;
        rasterization.cullMode = VK_CULL_MODE_NONE;
        rasterization.lineWidth = 1.0f;
        VkPipelineMultisampleStateCreateInfo multisample = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
        multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        // Alpha is never written, since it can control how the compositor blends the window.
        VkPipelineColorBlendAttachmentState attachment = {};
        attachment.blendEnable = blend ? VK_TRUE : VK_FALSE;
        attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        attachment.colorBlendOp = VK_BLEND_OP_ADD;
        attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        attachment.alphaBlendOp = VK_BLEND_OP_ADD;
        attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT;
        VkPipelineColorBlendStateCreateInfo color_blend = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
        color_blend.attachmentCount = 1;
        color_blend.pAttachments = &attachment;

        constexpr std::array dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dynamic = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
        dynamic.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
        dynamic.pDynamicStates = dynamic_states.data();

        VkGraphicsPipelineCreateInfo info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
        info.stageCount = static_cast<uint32_t>(stages.size());
        info.pStages = stages.data();
        info.pVertexInputState = &vertex_input;
        info.pInputAssemblyState = &input_assembly;
        info.pViewportState = &viewport;
        info.pRasterizationState = &rasterization;
        info.pMultisampleState = &multisample;
        info.pColorBlendState = &color_blend;
        info.pDynamicState = &dynamic;
        info.layout = pipeline.layout;
        info.renderPass = pipeline.render_pass;
        result = dispatch.CreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &info, nullptr, &pipeline.pipeline);
    }

    if (frag) {
        dispatch.DestroyShaderModule(device, frag, nullptr);
    }
    if (vert) {
        dispatch.DestroyShaderModule(device, vert, nullptr);
    }
    return result;
}

VkResult CreateOverlayPipeline(VkDevice device, const VkuDeviceDispatchTable &dispatch, VkFormat format, bool blend,
                               OverlayPipeline &pipeline) {
    pipeline = {};
    VkResult result = CreateRenderPass(device, dispatch, format, pipeline.render_pass);

    if (result == VK_SUCCESS) {
        constexpr VkPushConstantRange range = {VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                                               sizeof(OverlayPushConstants)};
        VkPipelineLayoutCreateInfo info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        info.pushConstantRangeCount = 1;
        info.pPushConstantRanges = &range;
        result = dispatch.CreatePipelineLayout(device, &info, nullptr, &pipeline.layout);
    }
    if (result == VK_SUCCESS) {
        result = CreateGraphicsPipeline(device, dispatch, blend, pipeline);
    }

    if (result != VK_SUCCESS) {
        DestroyOverlayPipeline(device, dispatch, pipeline);
    }
    return result;
}

void DestroyOverlayPipeline(VkDevice device, const VkuDeviceDispatchTable &dispatch, OverlayPipeline &pipeline) {
    if (pipeline.pipeline) {
        dispatch.DestroyPipeline(device, pipeline.pipeline, nullptr);
    }
    if (pipeline.layout) {
        dispatch.DestroyPipelineLayout(device, pipeline.layout, nullptr);
    }
    if (pipeline.render_pass) {
        dispatch.DestroyRenderPass(device, pipeline.render_pass, nullptr);
    }
    pipeline = {};
}

void RecordOverlay(const VkuDeviceDispatchTable &dispatch, VkCommandBuffer command_buffer, const OverlayPipeline &pipeline,
                   VkFramebuffer framebuffer, VkExtent2D image_extent, const Rect &panel,
                   const OverlayPushConstants &push_constants) {
    // The render area and scissor must stay inside the framebuffer.
    const int32_t x0 = std::max(panel[0], 0);
    const int32_t y0 = std::max(panel[1], 0);
    const int32_t x1 = std::min(panel[2], static_cast<int32_t>(image_extent.width));
    const int32_t y1 = std::min(panel[3], static_cast<int32_t>(image_extent.height));
    if (x1 <= x0 || y1 <= y0) {
        return;
    }

    // Only the panel is loaded and stored, which keeps the cost on tiled GPUs low.
    const VkRect2D area = {{x0, y0}, {static_cast<uint32_t>(x1 - x0), static_cast<uint32_t>(y1 - y0)}};
    VkRenderPassBeginInfo begin = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    begin.renderPass = pipeline.render_pass;
    begin.framebuffer = framebuffer;
    begin.renderArea = area;
    dispatch.CmdBeginRenderPass(command_buffer, &begin, VK_SUBPASS_CONTENTS_INLINE);

    const VkViewport viewport = {0.0f, 0.0f, static_cast<float>(image_extent.width), static_cast<float>(image_extent.height),
                                 0.0f, 1.0f};
    dispatch.CmdSetViewport(command_buffer, 0, 1, &viewport);
    dispatch.CmdSetScissor(command_buffer, 0, 1, &area);
    dispatch.CmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
    dispatch.CmdPushConstants(command_buffer, pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                              sizeof(push_constants), &push_constants);
    dispatch.CmdDraw(command_buffer, 4, 1, 0, 0);
    dispatch.CmdEndRenderPass(command_buffer);
}

}  // namespace monitor
