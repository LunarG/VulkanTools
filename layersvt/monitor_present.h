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

#include <chrono>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vk_layer.h>
#include <vulkan/utility/vk_dispatch_table.h>

#include "monitor_overlay.h"
#include "monitor_renderer.h"

namespace monitor {

// Per swapchain image objects. The semaphore is what vkQueuePresentKHR waits on for this image; reusing it per image
// index is safe because the image can only be acquired again after the previous present of it has started.
struct SwapchainImage {
    VkImage image = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkFramebuffer framebuffer = VK_NULL_HANDLE;
    VkSemaphore present_wait = VK_NULL_HANDLE;
    bool created = false;  // View and framebuffer exist
};

struct SwapchainOverlay {
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkColorSpaceKHR color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    VkExtent2D extent = {};
    VkSurfaceTransformFlagBitsKHR transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    bool deferred = false;  // Image memory is bound on first acquire, so views are made at the first present
    bool blend = true;
    bool view_usage = false;       // Views are limited to color attachment usage
    bool ok = false;               // The overlay can be drawn
    const char *reason = nullptr;  // Why not, when !ok
    OverlayPipeline pipeline;      // Owned by the device's OverlayPipelines
    std::vector<SwapchainImage> images;
    FrameStats stats;
};

// Overlay pipelines by swapchain format and blending. The swapchains of a device share them, so recreating a swapchain,
// as apps do on every resize, doesn't compile the shaders again. `lock` is held while a pipeline is created, so two
// swapchains of a new format don't both compile it; presents never take it.
struct OverlayPipelines {
    std::mutex lock;
    std::map<std::pair<VkFormat, bool>, OverlayPipeline> by_key;
};

// The device must be idle.
void DestroyOverlayPipelines(VkDevice device, const VkuDeviceDispatchTable &dispatch, OverlayPipelines &pipelines);

// What the device and the swapchain format let the overlay use
struct OverlaySupport {
    bool blend = true;        // The format supports blending
    bool view_usage = false;  // VkImageViewUsageCreateInfo can be chained (Vulkan 1.1 or VK_KHR_maintenance2)
};

// Sets up everything needed to draw on a swapchain already created from `patch`. On failure `overlay.ok` is false, the swapchain
// still works, and DestroySwapchainOverlay is still the way to clean up.
void SetupSwapchainOverlay(VkDevice device, const VkuDeviceDispatchTable &dispatch, VkSwapchainKHR swapchain,
                           const SwapchainPatch &patch, OverlaySupport support, OverlayPipelines &pipelines,
                           SwapchainOverlay &overlay);

// Makes sure the image view and framebuffer for an acquired image exist. Returns false if they couldn't be created.
[[nodiscard]] bool PrepareImage(VkDevice device, const VkuDeviceDispatchTable &dispatch, SwapchainOverlay &overlay, uint32_t index);

// The swapchain must not be in use by the GPU (see WaitForSwapchain).
void DestroySwapchainOverlay(VkDevice device, const VkuDeviceDispatchTable &dispatch, SwapchainOverlay &overlay);

// One overlay submission: a command buffer and the fence that says when it's done.
struct SubmitSlot {
    VkCommandBuffer command_buffer = VK_NULL_HANDLE;
    VkFence fence = VK_NULL_HANDLE;
    bool in_flight = false;                         // Submitted, so the fence will signal
    std::vector<VkSwapchainKHR> swapchains;         // Drawn on by that submission
    std::vector<VkPipelineStageFlags> wait_stages;  // Kept to reuse its memory
};

// Submission slots for one queue, used round robin. Externally synchronized (QueueOverlay::lock).
struct SlotRing {
    VkCommandPool pool = VK_NULL_HANDLE;
    std::vector<SubmitSlot> slots;
    uint32_t next = 0;
    bool ok = false;
    const char *reason = nullptr;  // Why there's no overlay, when !ok
    bool retry = false;            // It ran out of memory, so the next present tries again
};

// The layer makes its own command buffers, so the loader needs `set_loader_data` to give them a dispatch pointer.
// Without it the ring isn't usable.
void CreateSlotRing(VkDevice device, const VkuDeviceDispatchTable &dispatch, uint32_t queue_family,
                    PFN_vkSetDeviceLoaderData set_loader_data, uint32_t slot_count, SlotRing &ring);

// Returns the next slot once its previous submission is done, ready to record.
[[nodiscard]] SubmitSlot &AcquireSlot(VkDevice device, const VkuDeviceDispatchTable &dispatch, SlotRing &ring);

// Submits the slot's recorded command buffer. The app's semaphores are waited on before the overlay draws, and the
// signal semaphores are what the present then waits on. On failure the slot is left reusable.
[[nodiscard]] VkResult SubmitOverlay(VkDevice device, const VkuDeviceDispatchTable &dispatch, VkQueue queue, SubmitSlot &slot,
                                     const std::vector<VkSemaphore> &waits, const std::vector<VkSemaphore> &signals,
                                     const std::vector<VkSwapchainKHR> &swapchains);

// Waits until no submission is drawing on the swapchain.
void WaitForSwapchain(VkDevice device, const VkuDeviceDispatchTable &dispatch, SlotRing &ring, VkSwapchainKHR swapchain);

void DestroySlotRing(VkDevice device, const VkuDeviceDispatchTable &dispatch, SlotRing &ring);

// Storage for a VkPresentInfoKHR rewritten to wait on the overlay, and the parts of its pNext chain that were copied.
// The rewritten info points into this struct, so it must stay in place until the present is done. There is one per
// queue, which the app keeps from presenting twice at once, and reusing it keeps presents from allocating.
struct PresentRewrite {
    VkPresentInfoKHR info = {};
    std::vector<VkSemaphore> waits;
    VkPresentRegionsKHR present_regions = {};
    std::vector<VkPresentRegionKHR> regions;
    std::vector<std::vector<uint64_t>> copies;
    std::vector<const VkBaseInStructure *> prefix;  // Structs ahead of VkPresentRegionsKHR
    bool regions_unchanged = false;                 // VkPresentRegionsKHR was behind a struct that can't be copied
};

// The present waits on `waits` instead of the app's semaphores. Swapchains with an overlay in `overlays[i]` report their whole
// image as changed in VkPresentRegionsKHR, so incremental presents still update the overlay. The rest of the pNext
// chain is passed through.
[[nodiscard]] const VkPresentInfoKHR &RewritePresentInfo(const VkPresentInfoKHR &original, const std::vector<VkSemaphore> &waits,
                                                         const std::vector<SwapchainOverlay *> &overlays, PresentRewrite &rewrite);

// Undoes the overlay submit's semaphore operations after the present that followed it failed without being enqueued. It
// waits on `overlay_signals`, so they can be signaled again, and signals `app_waits`, which the overlay submit consumed, so
// the application's semaphores are as it left them.
[[nodiscard]] VkResult UndoOverlaySemaphores(const VkuDeviceDispatchTable &dispatch, VkQueue queue,
                                             const std::vector<VkSemaphore> &overlay_signals,
                                             const std::vector<VkSemaphore> &app_waits);

// Overlay state of one queue. The app already keeps presents to a queue from overlapping, so only the present in
// progress uses it. `lock` guards just the ring, for ForgetSwapchain, which waits for the submissions of every queue.
struct QueueOverlay {
    std::mutex lock;
    SlotRing ring;

    // Used by the present in progress on this queue, and kept so presenting doesn't allocate once warmed up
    PresentRewrite rewrite;
    std::vector<SwapchainOverlay *> overlays;  // For each presented swapchain, null if it gets no overlay
    std::vector<std::string> lines;
    std::vector<VkSemaphore> waits;
    std::vector<VkSemaphore> signals;
    std::vector<VkSwapchainKHR> swapchains;
};

// Overlay state of one device. The plain fields are set at vkCreateDevice and only read after that. `lock` guards the
// maps; the swapchain and queue overlays in them are only used by presents to them, which the app already keeps from
// overlapping with other uses of the swapchain or queue.
struct DeviceOverlay {
    std::mutex lock;
    VkDevice device = VK_NULL_HANDLE;
    PFN_vkSetDeviceLoaderData set_loader_data = nullptr;
    bool view_usage = false;  // VkImageViewUsageCreateInfo can be used
    std::vector<VkQueueFamilyProperties> queue_families;
    std::unordered_map<VkQueue, uint32_t> queue_family_index;
    std::unordered_map<VkSwapchainKHR, std::unique_ptr<SwapchainOverlay>> swapchains;
    OverlayPipelines pipelines;
    std::unordered_map<VkQueue, std::unique_ptr<QueueOverlay>> queues;
};

// Stops tracking the swapchain and waits until no overlay submission draws on it. Call before the swapchain is destroyed.
// Returns its overlay, or null if it had none, for DestroySwapchainOverlay after the swapchain is gone: a present may
// still be waiting on the overlay's semaphores until then.
[[nodiscard]] std::unique_ptr<SwapchainOverlay> ForgetSwapchain(const VkuDeviceDispatchTable &dispatch, DeviceOverlay &state,
                                                                VkSwapchainKHR swapchain);

// Sets up the overlay of a newly created swapchain and starts tracking it. Returns why the overlay can't be drawn on it,
// or null. The device lock is only held to add it, so presents to other swapchains go on while it's set up.
[[nodiscard]] const char *AddSwapchainOverlay(const VkuDeviceDispatchTable &dispatch, DeviceOverlay &state,
                                              VkSwapchainKHR swapchain, const SwapchainPatch &patch, bool blend_supported);

// Destroys everything the overlay created on the device. The device must be idle.
void DestroyDeviceOverlay(const VkuDeviceDispatchTable &dispatch, DeviceOverlay &state);

// Tells the user why the overlay isn't drawn as asked. Called for every present it applies to; the layer shows each
// distinct message once.
using Warn = void (*)(const char *message);

// `warn` when the user turned on log_warnings, otherwise a Warn that drops the message
[[nodiscard]] Warn WarnIf(bool enabled, Warn warn);

// Presents with the overlay drawn on every swapchain that can have it, in one submit that waits for the app's work.
// The present then waits for that submit instead. Without the overlay, the app's present goes through unchanged.
// No lock is held while waiting for earlier overlay submissions or while the driver presents.
[[nodiscard]] VkResult PresentWithOverlay(const VkuDeviceDispatchTable &dispatch, DeviceOverlay &state, VkQueue queue,
                                          const VkPresentInfoKHR &present_info, const MonitorSettings &settings,
                                          uint64_t frame_number, std::chrono::steady_clock::time_point now, Warn warn);

}  // namespace monitor
