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

#include "monitor_present.h"

#include <algorithm>
#include <cstdint>
#include <chrono>
#include <cstring>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "monitor_overlay.h"
#include "monitor_renderer.h"

namespace monitor {

void DestroyOverlayPipelines(VkDevice device, const VkuDeviceDispatchTable &dispatch, OverlayPipelines &pipelines) {
    const std::scoped_lock lock(pipelines.lock);
    for (auto &entry : pipelines.by_key) {
        DestroyOverlayPipeline(device, dispatch, entry.second);
    }
    pipelines.by_key.clear();
}

void SetupSwapchainOverlay(VkDevice device, const VkuDeviceDispatchTable &dispatch, VkSwapchainKHR swapchain,
                           const SwapchainPatch &patch, OverlaySupport support, OverlayPipelines &pipelines,
                           SwapchainOverlay &overlay) {
    const VkSwapchainCreateInfoKHR &create_info = patch.create_info;
    overlay = {};
    overlay.swapchain = swapchain;
    overlay.format = create_info.imageFormat;
    overlay.color_space = create_info.imageColorSpace;
    overlay.extent = create_info.imageExtent;
    overlay.transform = create_info.preTransform;
    overlay.blend = support.blend;
    overlay.view_usage = support.view_usage;

    overlay.deferred = patch.eligibility.deferred;
    if (!patch.eligibility.ok) {
        overlay.reason = patch.eligibility.reason;
        return;
    }

    uint32_t count = 0;
    std::vector<VkImage> images;
    if (dispatch.GetSwapchainImagesKHR(device, swapchain, &count, nullptr) == VK_SUCCESS) {
        images.resize(count);
        if (dispatch.GetSwapchainImagesKHR(device, swapchain, &count, images.data()) != VK_SUCCESS) {
            images.clear();
        }
    }
    if (images.empty()) {
        overlay.reason = "could not get the swapchain images";
        return;
    }

    {
        const std::scoped_lock lock(pipelines.lock);
        const auto key = std::make_pair(overlay.format, overlay.blend);
        auto cached = pipelines.by_key.find(key);
        if (cached == pipelines.by_key.end()) {
            OverlayPipeline pipeline;
            if (CreateOverlayPipeline(device, dispatch, overlay.format, overlay.blend, pipeline) != VK_SUCCESS) {
                overlay.reason = "could not create the overlay pipeline";
                return;
            }
            cached = pipelines.by_key.emplace(key, pipeline).first;
        }
        overlay.pipeline = cached->second;
    }

    overlay.images.resize(images.size());
    for (size_t i = 0; i < images.size(); ++i) {
        overlay.images[i].image = images[i];
        constexpr VkSemaphoreCreateInfo semaphore_info = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        if (dispatch.CreateSemaphore(device, &semaphore_info, nullptr, &overlay.images[i].present_wait) != VK_SUCCESS) {
            overlay.reason = "could not create semaphores";
            return;
        }
    }

    // Deferred images have no memory until acquired, so their views wait for the first present
    if (!overlay.deferred) {
        for (uint32_t i = 0; i < overlay.images.size(); ++i) {
            if (!PrepareImage(device, dispatch, overlay, i)) {
                overlay.reason = "could not create image views";
                return;
            }
        }
    }
    overlay.ok = true;
}

bool PrepareImage(VkDevice device, const VkuDeviceDispatchTable &dispatch, SwapchainOverlay &overlay, uint32_t index) {
    if (index >= overlay.images.size()) {
        return false;
    }
    SwapchainImage &image = overlay.images[index];
    if (image.created) {
        return true;
    }

    // Without this the view inherits every swapchain usage, and some (STORAGE with an sRGB view of a mutable format
    // swapchain) aren't valid for the view
    constexpr VkImageViewUsageCreateInfo usage_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO, nullptr,
                                                       VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT};
    VkImageViewCreateInfo view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    view_info.pNext = overlay.view_usage ? &usage_info : nullptr;
    view_info.image = image.image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = overlay.format;
    view_info.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    if (dispatch.CreateImageView(device, &view_info, nullptr, &image.view) != VK_SUCCESS) {
        image.view = VK_NULL_HANDLE;
        return false;
    }

    VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    framebuffer_info.renderPass = overlay.pipeline.render_pass;
    framebuffer_info.attachmentCount = 1;
    framebuffer_info.pAttachments = &image.view;
    framebuffer_info.width = overlay.extent.width;
    framebuffer_info.height = overlay.extent.height;
    framebuffer_info.layers = 1;
    if (dispatch.CreateFramebuffer(device, &framebuffer_info, nullptr, &image.framebuffer) != VK_SUCCESS) {
        // Leave nothing behind, since a deferred image is tried again on its next present
        dispatch.DestroyImageView(device, image.view, nullptr);
        image.view = VK_NULL_HANDLE;
        image.framebuffer = VK_NULL_HANDLE;
        return false;
    }

    image.created = true;
    return true;
}

void DestroySwapchainOverlay(VkDevice device, const VkuDeviceDispatchTable &dispatch, SwapchainOverlay &overlay) {
    for (const SwapchainImage &image : overlay.images) {
        if (image.framebuffer) {
            dispatch.DestroyFramebuffer(device, image.framebuffer, nullptr);
        }
        if (image.view) {
            dispatch.DestroyImageView(device, image.view, nullptr);
        }
        if (image.present_wait) {
            dispatch.DestroySemaphore(device, image.present_wait, nullptr);
        }
    }
    overlay = {};
}

void CreateSlotRing(VkDevice device, const VkuDeviceDispatchTable &dispatch, uint32_t queue_family,
                    PFN_vkSetDeviceLoaderData set_loader_data, uint32_t slot_count, SlotRing &ring) {
    ring = {};
    if (slot_count == 0) {
        ring.reason = "no overlay, no submission slots";
        return;
    }
    if (!set_loader_data) {
        ring.reason = "no overlay, the loader didn't provide vkSetDeviceLoaderData";
        return;
    }

    // Running out of memory may not last, so the objects made so far are freed and the next present tries again
    const auto out_of_memory = [&](const char *reason) {
        DestroySlotRing(device, dispatch, ring);
        ring.reason = reason;
        ring.retry = true;
    };

    VkCommandPoolCreateInfo pool_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = queue_family;
    if (dispatch.CreateCommandPool(device, &pool_info, nullptr, &ring.pool) != VK_SUCCESS) {
        ring.pool = VK_NULL_HANDLE;
        out_of_memory("no overlay on some frames, could not create a command pool");
        return;
    }

    ring.slots.resize(slot_count);
    for (SubmitSlot &slot : ring.slots) {
        VkCommandBufferAllocateInfo allocate_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
        allocate_info.commandPool = ring.pool;
        allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocate_info.commandBufferCount = 1;
        if (dispatch.AllocateCommandBuffers(device, &allocate_info, &slot.command_buffer) != VK_SUCCESS) {
            slot.command_buffer = VK_NULL_HANDLE;
            out_of_memory("no overlay on some frames, could not allocate command buffers");
            return;
        }
        if (set_loader_data(device, slot.command_buffer) != VK_SUCCESS) {
            ring.reason = "no overlay, vkSetDeviceLoaderData failed";
            return;
        }

        constexpr VkFenceCreateInfo fence_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
        if (dispatch.CreateFence(device, &fence_info, nullptr, &slot.fence) != VK_SUCCESS) {
            slot.fence = VK_NULL_HANDLE;
            out_of_memory("no overlay on some frames, could not create fences");
            return;
        }
    }
    ring.ok = true;
}

static void WaitForSlot(VkDevice device, const VkuDeviceDispatchTable &dispatch, SubmitSlot &slot) {
    if (slot.in_flight) {
        dispatch.WaitForFences(device, 1, &slot.fence, VK_TRUE, UINT64_MAX);
        slot.in_flight = false;
        slot.swapchains.clear();
    }
}

SubmitSlot &AcquireSlot(VkDevice device, const VkuDeviceDispatchTable &dispatch, SlotRing &ring) {
    SubmitSlot &slot = ring.slots[ring.next];
    ring.next = (ring.next + 1) % static_cast<uint32_t>(ring.slots.size());
    WaitForSlot(device, dispatch, slot);
    return slot;
}

VkResult SubmitOverlay(VkDevice device, const VkuDeviceDispatchTable &dispatch, VkQueue queue, SubmitSlot &slot,
                       const std::vector<VkSemaphore> &waits, const std::vector<VkSemaphore> &signals,
                       const std::vector<VkSwapchainKHR> &swapchains) {
    slot.wait_stages.assign(waits.size(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    VkSubmitInfo submit = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit.waitSemaphoreCount = static_cast<uint32_t>(waits.size());
    submit.pWaitSemaphores = waits.data();
    submit.pWaitDstStageMask = slot.wait_stages.data();
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &slot.command_buffer;
    submit.signalSemaphoreCount = static_cast<uint32_t>(signals.size());
    submit.pSignalSemaphores = signals.data();

    // The fence is only reset when a submit will signal it again. A failed submit leaves it reset, but not in flight.
    VkResult result = dispatch.ResetFences(device, 1, &slot.fence);
    if (result != VK_SUCCESS) {
        return result;
    }
    result = dispatch.QueueSubmit(queue, 1, &submit, slot.fence);
    if (result == VK_SUCCESS) {
        slot.in_flight = true;
        slot.swapchains = swapchains;
    }
    return result;
}

void WaitForSwapchain(VkDevice device, const VkuDeviceDispatchTable &dispatch, SlotRing &ring, VkSwapchainKHR swapchain) {
    for (SubmitSlot &slot : ring.slots) {
        if (std::find(slot.swapchains.begin(), slot.swapchains.end(), swapchain) != slot.swapchains.end()) {
            WaitForSlot(device, dispatch, slot);
        }
    }
}

void DestroySlotRing(VkDevice device, const VkuDeviceDispatchTable &dispatch, SlotRing &ring) {
    for (SubmitSlot &slot : ring.slots) {
        WaitForSlot(device, dispatch, slot);
        if (slot.fence) {
            dispatch.DestroyFence(device, slot.fence, nullptr);
        }
        if (slot.command_buffer) {
            dispatch.FreeCommandBuffers(device, ring.pool, 1, &slot.command_buffer);
        }
    }
    if (ring.pool) {
        dispatch.DestroyCommandPool(device, ring.pool, nullptr);
    }
    ring = {};
}

// Sizes of the VkPresentInfoKHR extension structs that can come before VkPresentRegionsKHR. To change the regions,
// every struct ahead of them in the chain has to be copied, and only structs of known size can be.
[[nodiscard]] static size_t PresentStructSize(VkStructureType type) {
    switch (type) {
        case VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR:
            return sizeof(VkDisplayPresentInfoKHR);
        case VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_INFO_KHR:
            return sizeof(VkDeviceGroupPresentInfoKHR);
        case VK_STRUCTURE_TYPE_PRESENT_TIMES_INFO_GOOGLE:
            return sizeof(VkPresentTimesInfoGOOGLE);
        case VK_STRUCTURE_TYPE_PRESENT_ID_KHR:
            return sizeof(VkPresentIdKHR);
        case VK_STRUCTURE_TYPE_PRESENT_ID_2_KHR:
            return sizeof(VkPresentId2KHR);
        case VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_FENCE_INFO_KHR:
            return sizeof(VkSwapchainPresentFenceInfoKHR);
        case VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_MODE_INFO_KHR:
            return sizeof(VkSwapchainPresentModeInfoKHR);
        case VK_STRUCTURE_TYPE_FRAME_BOUNDARY_EXT:
            return sizeof(VkFrameBoundaryEXT);
        default:
            return 0;
    }
}

// Rebuilds the start of the chain with a copy of VkPresentRegionsKHR in which overlay swapchains have no rectangles,
// meaning the whole image changed. Returns the original chain if there are no regions or they can't be reached.
[[nodiscard]] static const void *RewritePresentRegions(const void *chain, const std::vector<SwapchainOverlay *> &overlays,
                                                       PresentRewrite &rewrite) {
    std::vector<const VkBaseInStructure *> &prefix = rewrite.prefix;
    prefix.clear();
    const auto *node = static_cast<const VkBaseInStructure *>(chain);
    for (; node && node->sType != VK_STRUCTURE_TYPE_PRESENT_REGIONS_KHR; node = node->pNext) {
        prefix.push_back(node);
    }
    if (!node) {
        return chain;
    }
    for (const VkBaseInStructure *before : prefix) {
        if (PresentStructSize(before->sType) == 0) {
            rewrite.regions_unchanged = true;
            return chain;
        }
    }

    rewrite.present_regions = *reinterpret_cast<const VkPresentRegionsKHR *>(node);
    if (rewrite.present_regions.pRegions) {
        rewrite.regions.assign(rewrite.present_regions.pRegions,
                               rewrite.present_regions.pRegions + rewrite.present_regions.swapchainCount);
        for (size_t i = 0; i < rewrite.regions.size() && i < overlays.size(); ++i) {
            if (overlays[i]) {
                rewrite.regions[i].rectangleCount = 0;
                rewrite.regions[i].pRectangles = nullptr;
            }
        }
        rewrite.present_regions.pRegions = rewrite.regions.data();
    }

    // Copy the structs ahead of the regions, back to front, each pointing at the next copy
    const void *next = &rewrite.present_regions;
    rewrite.copies.resize(prefix.size());
    for (size_t i = prefix.size(); i-- > 0;) {
        const size_t size = PresentStructSize(prefix[i]->sType);
        std::vector<uint64_t> &copy = rewrite.copies[i];
        copy.resize((size + sizeof(uint64_t) - 1) / sizeof(uint64_t));
        memcpy(copy.data(), prefix[i], size);
        reinterpret_cast<VkBaseInStructure *>(copy.data())->pNext = static_cast<const VkBaseInStructure *>(next);
        next = copy.data();
    }
    return next;
}

const VkPresentInfoKHR &RewritePresentInfo(const VkPresentInfoKHR &original, const std::vector<VkSemaphore> &waits,
                                           const std::vector<SwapchainOverlay *> &overlays, PresentRewrite &rewrite) {
    // The vectors keep their capacity, so presenting doesn't allocate once warmed up
    rewrite.waits.assign(waits.begin(), waits.end());
    rewrite.regions.clear();
    rewrite.regions_unchanged = false;
    rewrite.info = original;
    rewrite.info.waitSemaphoreCount = static_cast<uint32_t>(rewrite.waits.size());
    rewrite.info.pWaitSemaphores = rewrite.waits.data();
    rewrite.info.pNext = RewritePresentRegions(original.pNext, overlays, rewrite);
    return rewrite.info;
}

VkResult UndoOverlaySemaphores(const VkuDeviceDispatchTable &dispatch, VkQueue queue,
                               const std::vector<VkSemaphore> &overlay_signals, const std::vector<VkSemaphore> &app_waits) {
    if (overlay_signals.empty() && app_waits.empty()) {
        return VK_SUCCESS;
    }
    const std::vector<VkPipelineStageFlags> stages(overlay_signals.size(), VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
    VkSubmitInfo submit = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit.waitSemaphoreCount = static_cast<uint32_t>(overlay_signals.size());
    submit.pWaitSemaphores = overlay_signals.data();
    submit.pWaitDstStageMask = stages.data();
    submit.signalSemaphoreCount = static_cast<uint32_t>(app_waits.size());
    submit.pSignalSemaphores = app_waits.data();
    return dispatch.QueueSubmit(queue, 1, &submit, VK_NULL_HANDLE);
}

Warn WarnIf(bool enabled, Warn warn) {
    if (enabled) {
        return warn;
    }
    return [](const char * /*message*/) {};
}

std::unique_ptr<SwapchainOverlay> ForgetSwapchain(const VkuDeviceDispatchTable &dispatch, DeviceOverlay &state,
                                                  VkSwapchainKHR swapchain) {
    // The waits happen without the device lock, so presents to other swapchains aren't held up. Queue overlays live as
    // long as the device, and the app can't present this swapchain while destroying it.
    std::unique_ptr<SwapchainOverlay> overlay;
    std::vector<QueueOverlay *> queues;
    {
        const std::scoped_lock lock(state.lock);
        auto it = state.swapchains.find(swapchain);
        if (it == state.swapchains.end()) {
            return nullptr;
        }
        overlay = std::move(it->second);
        state.swapchains.erase(it);
        for (auto &queue : state.queues) {
            queues.push_back(queue.second.get());
        }
    }
    for (QueueOverlay *queue : queues) {
        const std::scoped_lock queue_lock(queue->lock);
        WaitForSwapchain(state.device, dispatch, queue->ring, swapchain);
    }
    return overlay;
}

const char *AddSwapchainOverlay(const VkuDeviceDispatchTable &dispatch, DeviceOverlay &state, VkSwapchainKHR swapchain,
                                const SwapchainPatch &patch, bool blend_supported) {
    auto overlay = std::make_unique<SwapchainOverlay>();
    SetupSwapchainOverlay(state.device, dispatch, swapchain, patch, {blend_supported, state.view_usage}, state.pipelines, *overlay);
    const char *reason = overlay->ok ? nullptr : overlay->reason;

    const std::scoped_lock lock(state.lock);
    state.swapchains[swapchain] = std::move(overlay);
    return reason;
}

void DestroyDeviceOverlay(const VkuDeviceDispatchTable &dispatch, DeviceOverlay &state) {
    const std::scoped_lock lock(state.lock);
    for (auto &swapchain : state.swapchains) {
        DestroySwapchainOverlay(state.device, dispatch, *swapchain.second);
    }
    state.swapchains.clear();
    for (auto &queue : state.queues) {
        DestroySlotRing(state.device, dispatch, queue.second->ring);
    }
    state.queues.clear();
    DestroyOverlayPipelines(state.device, dispatch, state.pipelines);
}

namespace {
// What the device lock part of a present found
struct PresentTargets {
    QueueOverlay *queue = nullptr;  // Null if the layer doesn't know the queue
    uint32_t family = 0;
    VkQueueFlags queue_flags = 0;
    bool any_overlay = false;   // Some swapchain is ready for the overlay
    bool any_eligible = false;  // Some swapchain could have the overlay, whatever the queue
};
}  // namespace

// Finds the queue's state, records statistics for every presented swapchain, and lists the overlays to draw in the
// queue's state. Swapchains being presented can't be destroyed meanwhile, so the overlays stay valid after the device
// lock is released.
[[nodiscard]] static PresentTargets FindPresentTargets(const VkuDeviceDispatchTable &dispatch, DeviceOverlay &state, VkQueue queue,
                                                       const VkPresentInfoKHR &present_info, const MonitorSettings &settings,
                                                       std::chrono::steady_clock::time_point now) {
    PresentTargets targets;
    const std::scoped_lock lock(state.lock);
    const auto found = state.queue_family_index.find(queue);
    if (found != state.queue_family_index.end() && found->second < state.queue_families.size()) {
        targets.family = found->second;
        targets.queue_flags = state.queue_families[targets.family].queueFlags;
        std::unique_ptr<QueueOverlay> &entry = state.queues[queue];
        if (!entry) {
            entry = std::make_unique<QueueOverlay>();
        }
        targets.queue = entry.get();
        targets.queue->overlays.assign(present_info.swapchainCount, nullptr);
    }

    for (uint32_t i = 0; i < present_info.swapchainCount; ++i) {
        auto it = state.swapchains.find(present_info.pSwapchains[i]);
        if (it == state.swapchains.end()) {
            continue;
        }
        RecordPresent(it->second->stats, now, settings.update_interval);
        targets.any_eligible |= it->second->ok;
        if (targets.queue && it->second->ok && PrepareImage(state.device, dispatch, *it->second, present_info.pImageIndices[i])) {
            targets.queue->overlays[i] = it->second.get();
            targets.any_overlay = true;
        }
    }
    return targets;
}

// Records the overlay of every swapchain in the queue's list, and what the submit then signals and draws on.
static void RecordOverlays(const VkuDeviceDispatchTable &dispatch, QueueOverlay &queue_state, VkCommandBuffer command_buffer,
                           const VkPresentInfoKHR &present_info, const MonitorSettings &settings, uint64_t frame_number,
                           Warn warn) {
    queue_state.signals.clear();
    queue_state.swapchains.clear();
    for (uint32_t i = 0; i < present_info.swapchainCount; ++i) {
        const SwapchainOverlay *overlay = queue_state.overlays[i];
        if (!overlay) {
            continue;
        }
        const SwapchainImage &image = overlay->images[present_info.pImageIndices[i]];

        FormatOverlayLines(overlay->stats.snapshot, frame_number, settings.lines, queue_state.lines);
        const OverlayLayout layout =
            ComputeOverlayLayout(overlay->extent, settings.position, settings.scale_percent, overlay->transform, queue_state.lines);
        const WhiteLevel white = WhiteForColorSpace(overlay->color_space);
        if (layout.transform_warning) {
            warn("mirrored swapchain transforms are drawn without the mirror");
        }
        if (!white.known) {
            warn("unknown swapchain color space, the overlay may be too bright or too dark");
        }

        const OverlayPushConstants push_constants =
            PackOverlayPushConstants(layout, queue_state.lines, white.value, !overlay->blend);
        RecordOverlay(dispatch, command_buffer, overlay->pipeline, image.framebuffer, overlay->extent, layout.image_rect,
                      push_constants);
        queue_state.signals.push_back(image.present_wait);
        queue_state.swapchains.push_back(overlay->swapchain);
    }
}

VkResult PresentWithOverlay(const VkuDeviceDispatchTable &dispatch, DeviceOverlay &state, VkQueue queue,
                            const VkPresentInfoKHR &present_info, const MonitorSettings &settings, uint64_t frame_number,
                            std::chrono::steady_clock::time_point now, Warn warn) {
    const PresentTargets targets = FindPresentTargets(dispatch, state, queue, present_info, settings, now);
    QueueOverlay *queue_state = targets.queue;

    const bool any_line = AnyLineVisible(settings.lines);
    if (!queue_state) {
        if (targets.any_eligible && any_line) {
            warn("no overlay when presenting from a queue the layer didn't see from vkGetDeviceQueue or vkGetDeviceQueue2");
        }
        return dispatch.QueuePresentKHR(queue, &present_info);
    }
    if (!ShouldDrawOverlay(targets.queue_flags, targets.any_overlay, any_line)) {
        if (targets.any_overlay && any_line && ((targets.queue_flags & VK_QUEUE_GRAPHICS_BIT) == 0u)) {
            warn("no overlay when presenting from a queue without graphics");
        }
        return dispatch.QueuePresentKHR(queue, &present_info);
    }

    std::unique_lock<std::mutex> queue_lock(queue_state->lock);
    const auto present_without_overlay = [&](const char *why) {
        queue_lock.unlock();
        warn(why);
        return dispatch.QueuePresentKHR(queue, &present_info);
    };

    SlotRing &ring = queue_state->ring;
    if (!ring.ok && (!ring.reason || ring.retry)) {
        CreateSlotRing(state.device, dispatch, targets.family, state.set_loader_data, 3, ring);
    }
    if (!ring.ok) {
        return present_without_overlay(ring.reason ? ring.reason : "no overlay, could not create command buffers");
    }

    SubmitSlot &slot = AcquireSlot(state.device, dispatch, ring);
    VkCommandBufferBeginInfo begin = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    if (dispatch.BeginCommandBuffer(slot.command_buffer, &begin) != VK_SUCCESS) {
        return present_without_overlay("out of memory, some frames are presented without the overlay");
    }

    RecordOverlays(dispatch, *queue_state, slot.command_buffer, present_info, settings, frame_number, warn);
    if (dispatch.EndCommandBuffer(slot.command_buffer) != VK_SUCCESS) {
        return present_without_overlay("out of memory, some frames are presented without the overlay");
    }

    queue_state->waits.assign(present_info.pWaitSemaphores, present_info.pWaitSemaphores + present_info.waitSemaphoreCount);
    const std::vector<VkSemaphore> &signals = queue_state->signals;
    VkResult result = SubmitOverlay(state.device, dispatch, queue, slot, queue_state->waits, signals, queue_state->swapchains);
    if (result != VK_SUCCESS) {
        // A failed submit leaves the app's semaphores alone, so the frame is presented as if the layer weren't there. Even
        // after a device loss, the driver's present reports the error and fills in pResults for the application.
        const bool out_of_memory = result == VK_ERROR_OUT_OF_HOST_MEMORY || result == VK_ERROR_OUT_OF_DEVICE_MEMORY;
        return present_without_overlay(out_of_memory ? "out of memory, some frames are presented without the overlay"
                                                     : "the overlay submit failed, some frames are presented without the overlay");
    }

    // The rewritten info lives in the queue's state, which only the next present to this queue changes
    const VkPresentInfoKHR &rewritten = RewritePresentInfo(present_info, signals, queue_state->overlays, queue_state->rewrite);
    const bool regions_unchanged = queue_state->rewrite.regions_unchanged;
    queue_lock.unlock();
    if (regions_unchanged) {
        warn("VkPresentRegionsKHR is behind a struct the layer doesn't know, so the overlay may not update");
    }
    result = dispatch.QueuePresentKHR(queue, &rewritten);
    if (result == VK_ERROR_OUT_OF_HOST_MEMORY || result == VK_ERROR_OUT_OF_DEVICE_MEMORY) {
        // The spec says a present failing this way leaves its semaphores untouched, but the overlay submit already waited
        // on the application's. Only this present uses the queue's state, since the application synchronizes the queue.
        // The present's error is the one the application needs, so a failure here isn't reported.
        (void)UndoOverlaySemaphores(dispatch, queue, signals, queue_state->waits);
    }
    return result;
}

}  // namespace monitor
