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

// Tests the swapchain and present plumbing against a fake device: a dispatch table of stubs that count every object
// created and destroyed, record submissions, and fail on request. No Vulkan driver is needed.

#include <gtest/gtest.h>

#include <cstdint>
#include <cstring>
#include <map>
#include <memory>
#include <chrono>
#include <initializer_list>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "monitor_overlay.h"
#include "monitor_present.h"

namespace {

struct FakeDevice;
FakeDevice* fake = nullptr;

template <typename T>
T MakeHandle(uint64_t value) {
    static_assert(sizeof(T) == sizeof(uint64_t), "non-dispatchable handles are 64 bits");
    T handle{};
    memcpy(static_cast<void*>(&handle), &value, sizeof(T));
    return handle;
}

template <typename T>
uint64_t HandleValue(T handle) {
    uint64_t value = 0;
    memcpy(&value, static_cast<const void*>(&handle), sizeof(value));
    return value;
}

struct Submission {
    std::vector<VkSemaphore> waits;
    std::vector<VkPipelineStageFlags> wait_stages;
    std::vector<VkSemaphore> signals;
    uint32_t command_buffer_count = 0;
    VkFence fence = VK_NULL_HANDLE;
};

struct FakeDevice {
    std::map<std::string, int> created;
    std::map<std::string, int> destroyed;
    std::map<std::string, int> attempts;
    std::map<std::string, int> fail_at;  // Fail the Nth create attempt (1-based) of a kind of object
    uint64_t next_handle = 0x1000;
    uint32_t image_count = 3;

    // Fences are signaled by a successful submit. Waiting on one that is unsignaled and was never submitted would hang.
    std::map<uint64_t, bool> fence_signaled;
    std::set<uint64_t> fences_waited;
    int hung_waits = 0;
    VkResult submit_result = VK_SUCCESS;
    std::vector<Submission> submissions;

    // vkQueuePresentKHR calls that reached the driver
    struct Present {
        std::vector<VkSemaphore> waits;
        std::vector<VkSwapchainKHR> swapchains;
    };
    std::vector<Present> presents;
    VkResult present_result = VK_SUCCESS;
    int draws = 0;

    // Command buffers are dispatchable, so they get real memory for the loader's dispatch pointer.
    std::vector<std::unique_ptr<uint64_t>> command_buffers;
    int loader_data_calls = 0;

    // Usage of each image view from a chained VkImageViewUsageCreateInfo, 0 if there was none
    std::vector<VkImageUsageFlags> view_usages;

    FakeDevice() { fake = this; }
    ~FakeDevice() { fake = nullptr; }
    FakeDevice(const FakeDevice&) = delete;
    FakeDevice& operator=(const FakeDevice&) = delete;
    FakeDevice(FakeDevice&&) = delete;
    FakeDevice& operator=(FakeDevice&&) = delete;

    int Live(const std::string& kind) { return created[kind] - destroyed[kind]; }

    // Counts an attempt to create `kind`, and says whether it should fail
    bool Fails(const std::string& kind) {
        const int attempt = ++attempts[kind];
        const auto it = fail_at.find(kind);
        return it != fail_at.end() && it->second == attempt;
    }

    template <typename T>
    VkResult Create(const std::string& kind, T* out) {
        if (Fails(kind)) {
            return VK_ERROR_OUT_OF_DEVICE_MEMORY;
        }
        ++created[kind];
        *out = MakeHandle<T>(next_handle++);
        return VK_SUCCESS;
    }

    template <typename T>
    void Destroy(const std::string& kind, T handle) {
        if (HandleValue(handle) != 0) {
            ++destroyed[kind];
        }
    }

    static VkuDeviceDispatchTable Dispatch();
};

#define STUB_CREATE(Name, Info, Type, kind)                                                                     \
    VKAPI_ATTR VkResult VKAPI_CALL Stub##Name(VkDevice, const Info*, const VkAllocationCallbacks*, Type* out) { \
        return fake->Create(kind, out);                                                                         \
    }
#define STUB_DESTROY(Name, Type, kind) \
    VKAPI_ATTR void VKAPI_CALL Stub##Name(VkDevice, Type handle, const VkAllocationCallbacks*) { fake->Destroy(kind, handle); }

STUB_DESTROY(DestroyImageView, VkImageView, "view")
STUB_CREATE(CreateFramebuffer, VkFramebufferCreateInfo, VkFramebuffer, "framebuffer")
STUB_DESTROY(DestroyFramebuffer, VkFramebuffer, "framebuffer")
STUB_CREATE(CreateSemaphore, VkSemaphoreCreateInfo, VkSemaphore, "semaphore")
STUB_DESTROY(DestroySemaphore, VkSemaphore, "semaphore")
STUB_CREATE(CreateRenderPass, VkRenderPassCreateInfo, VkRenderPass, "render pass")
STUB_DESTROY(DestroyRenderPass, VkRenderPass, "render pass")
STUB_CREATE(CreatePipelineLayout, VkPipelineLayoutCreateInfo, VkPipelineLayout, "pipeline layout")
STUB_DESTROY(DestroyPipelineLayout, VkPipelineLayout, "pipeline layout")
STUB_CREATE(CreateShaderModule, VkShaderModuleCreateInfo, VkShaderModule, "shader module")
STUB_DESTROY(DestroyShaderModule, VkShaderModule, "shader module")
STUB_DESTROY(DestroyPipeline, VkPipeline, "pipeline")
STUB_CREATE(CreateCommandPool, VkCommandPoolCreateInfo, VkCommandPool, "command pool")
STUB_DESTROY(DestroyCommandPool, VkCommandPool, "command pool")
STUB_DESTROY(DestroyFence, VkFence, "fence")

VKAPI_ATTR VkResult VKAPI_CALL StubCreateImageView(VkDevice /*device*/, const VkImageViewCreateInfo* info,
                                                   const VkAllocationCallbacks* /*pAllocator*/, VkImageView* out) {
    VkImageUsageFlags usage = 0;
    for (const auto* node = static_cast<const VkBaseInStructure*>(info->pNext); node; node = node->pNext) {
        if (node->sType == VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO) {
            usage = reinterpret_cast<const VkImageViewUsageCreateInfo*>(node)->usage;
        }
    }
    fake->view_usages.push_back(usage);
    return fake->Create("view", out);
}

VKAPI_ATTR VkResult VKAPI_CALL StubCreateGraphicsPipelines(VkDevice /*device*/, VkPipelineCache /*pipelineCache*/, uint32_t count,
                                                           const VkGraphicsPipelineCreateInfo* /*pCreateInfos*/,
                                                           const VkAllocationCallbacks* /*pAllocator*/, VkPipeline* out) {
    for (uint32_t i = 0; i < count; ++i) {
        const VkResult result = fake->Create("pipeline", &out[i]);
        if (result != VK_SUCCESS) {
            return result;
        }
    }
    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL StubGetSwapchainImagesKHR(VkDevice /*device*/, VkSwapchainKHR /*swapchain*/, uint32_t* count,
                                                         VkImage* images) {
    if (!images) {
        *count = fake->image_count;
        return VK_SUCCESS;
    }
    for (uint32_t i = 0; i < *count && i < fake->image_count; ++i) {
        images[i] = MakeHandle<VkImage>(0x100 + i);
    }
    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL StubCreateFence(VkDevice /*device*/, const VkFenceCreateInfo* info,
                                               const VkAllocationCallbacks* /*pAllocator*/, VkFence* out) {
    const VkResult result = fake->Create("fence", out);
    if (result == VK_SUCCESS) {
        fake->fence_signaled[HandleValue(*out)] = (info->flags & VK_FENCE_CREATE_SIGNALED_BIT) != 0;
    }
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL StubWaitForFences(VkDevice /*device*/, uint32_t count, const VkFence* fences, VkBool32 /*waitAll*/,
                                                 uint64_t /*timeout*/) {
    for (uint32_t i = 0; i < count; ++i) {
        fake->fences_waited.insert(HandleValue(fences[i]));
        if (!fake->fence_signaled[HandleValue(fences[i])]) {
            ++fake->hung_waits;
        }
    }
    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL StubResetFences(VkDevice /*device*/, uint32_t count, const VkFence* fences) {
    for (uint32_t i = 0; i < count; ++i) {
        fake->fence_signaled[HandleValue(fences[i])] = false;
    }
    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL StubAllocateCommandBuffers(VkDevice /*device*/, const VkCommandBufferAllocateInfo* info,
                                                          VkCommandBuffer* out) {
    for (uint32_t i = 0; i < info->commandBufferCount; ++i) {
        if (fake->Fails("command buffer")) {
            return VK_ERROR_OUT_OF_DEVICE_MEMORY;
        }
        fake->command_buffers.push_back(std::make_unique<uint64_t>(0));
        out[i] = reinterpret_cast<VkCommandBuffer>(fake->command_buffers.back().get());
        ++fake->created["command buffer"];
    }
    return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL StubFreeCommandBuffers(VkDevice /*device*/, VkCommandPool /*commandPool*/, uint32_t count,
                                                  const VkCommandBuffer* buffers) {
    for (uint32_t i = 0; i < count; ++i) {
        if (buffers[i]) {
            ++fake->destroyed["command buffer"];
        }
    }
}

VKAPI_ATTR VkResult VKAPI_CALL StubQueueSubmit(VkQueue /*queue*/, uint32_t count, const VkSubmitInfo* submits, VkFence fence) {
    if (fake->submit_result != VK_SUCCESS) {
        return fake->submit_result;
    }
    for (uint32_t i = 0; i < count; ++i) {
        Submission submission;
        submission.waits.assign(submits[i].pWaitSemaphores, submits[i].pWaitSemaphores + submits[i].waitSemaphoreCount);
        submission.wait_stages.assign(submits[i].pWaitDstStageMask, submits[i].pWaitDstStageMask + submits[i].waitSemaphoreCount);
        submission.signals.assign(submits[i].pSignalSemaphores, submits[i].pSignalSemaphores + submits[i].signalSemaphoreCount);
        submission.command_buffer_count = submits[i].commandBufferCount;
        submission.fence = fence;
        fake->submissions.push_back(submission);
    }
    if (fence) {
        fake->fence_signaled[HandleValue(fence)] = true;
    }
    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL StubBeginCommandBuffer(VkCommandBuffer /*commandBuffer*/,
                                                      const VkCommandBufferBeginInfo* /*pBeginInfo*/) {
    return VK_SUCCESS;
}
VKAPI_ATTR VkResult VKAPI_CALL StubEndCommandBuffer(VkCommandBuffer /*commandBuffer*/) { return VK_SUCCESS; }

VKAPI_ATTR void VKAPI_CALL StubCmdBeginRenderPass(VkCommandBuffer /*commandBuffer*/, const VkRenderPassBeginInfo* /*pBegin*/,
                                                  VkSubpassContents /*contents*/) {}
VKAPI_ATTR void VKAPI_CALL StubCmdEndRenderPass(VkCommandBuffer /*commandBuffer*/) {}
VKAPI_ATTR void VKAPI_CALL StubCmdSetViewport(VkCommandBuffer /*commandBuffer*/, uint32_t /*first*/, uint32_t /*count*/,
                                              const VkViewport* /*pViewports*/) {}
VKAPI_ATTR void VKAPI_CALL StubCmdSetScissor(VkCommandBuffer /*commandBuffer*/, uint32_t /*first*/, uint32_t /*count*/,
                                             const VkRect2D* /*pScissors*/) {}
VKAPI_ATTR void VKAPI_CALL StubCmdBindPipeline(VkCommandBuffer /*commandBuffer*/, VkPipelineBindPoint /*bindPoint*/,
                                               VkPipeline /*pipeline*/) {}
VKAPI_ATTR void VKAPI_CALL StubCmdPushConstants(VkCommandBuffer /*commandBuffer*/, VkPipelineLayout /*layout*/,
                                                VkShaderStageFlags /*stages*/, uint32_t /*offset*/, uint32_t /*size*/,
                                                const void* /*pValues*/) {}
VKAPI_ATTR void VKAPI_CALL StubCmdDraw(VkCommandBuffer /*commandBuffer*/, uint32_t /*vertexCount*/, uint32_t /*instanceCount*/,
                                       uint32_t /*firstVertex*/, uint32_t /*firstInstance*/) {
    ++fake->draws;
}

VKAPI_ATTR VkResult VKAPI_CALL StubQueuePresentKHR(VkQueue /*queue*/, const VkPresentInfoKHR* info) {
    FakeDevice::Present present;
    present.waits.assign(info->pWaitSemaphores, info->pWaitSemaphores + info->waitSemaphoreCount);
    present.swapchains.assign(info->pSwapchains, info->pSwapchains + info->swapchainCount);
    fake->presents.push_back(present);
    return fake->present_result;
}

VkuDeviceDispatchTable FakeDevice::Dispatch() {
    VkuDeviceDispatchTable dispatch = {};
    dispatch.CreateImageView = StubCreateImageView;
    dispatch.DestroyImageView = StubDestroyImageView;
    dispatch.CreateFramebuffer = StubCreateFramebuffer;
    dispatch.DestroyFramebuffer = StubDestroyFramebuffer;
    dispatch.CreateSemaphore = StubCreateSemaphore;
    dispatch.DestroySemaphore = StubDestroySemaphore;
    dispatch.CreateRenderPass = StubCreateRenderPass;
    dispatch.DestroyRenderPass = StubDestroyRenderPass;
    dispatch.CreatePipelineLayout = StubCreatePipelineLayout;
    dispatch.DestroyPipelineLayout = StubDestroyPipelineLayout;
    dispatch.CreateShaderModule = StubCreateShaderModule;
    dispatch.DestroyShaderModule = StubDestroyShaderModule;
    dispatch.CreateGraphicsPipelines = StubCreateGraphicsPipelines;
    dispatch.DestroyPipeline = StubDestroyPipeline;
    dispatch.GetSwapchainImagesKHR = StubGetSwapchainImagesKHR;
    dispatch.CreateCommandPool = StubCreateCommandPool;
    dispatch.DestroyCommandPool = StubDestroyCommandPool;
    dispatch.AllocateCommandBuffers = StubAllocateCommandBuffers;
    dispatch.FreeCommandBuffers = StubFreeCommandBuffers;
    dispatch.CreateFence = StubCreateFence;
    dispatch.DestroyFence = StubDestroyFence;
    dispatch.WaitForFences = StubWaitForFences;
    dispatch.ResetFences = StubResetFences;
    dispatch.QueueSubmit = StubQueueSubmit;
    dispatch.BeginCommandBuffer = StubBeginCommandBuffer;
    dispatch.EndCommandBuffer = StubEndCommandBuffer;
    dispatch.CmdBeginRenderPass = StubCmdBeginRenderPass;
    dispatch.CmdEndRenderPass = StubCmdEndRenderPass;
    dispatch.CmdSetViewport = StubCmdSetViewport;
    dispatch.CmdSetScissor = StubCmdSetScissor;
    dispatch.CmdBindPipeline = StubCmdBindPipeline;
    dispatch.CmdPushConstants = StubCmdPushConstants;
    dispatch.CmdDraw = StubCmdDraw;
    dispatch.QueuePresentKHR = StubQueuePresentKHR;
    return dispatch;
}

uint64_t device_object = 0;
uint64_t queue_object = 0;
const VkDevice kDevice = reinterpret_cast<VkDevice>(&device_object);
const VkQueue kQueue = reinterpret_cast<VkQueue>(&queue_object);

VkSwapchainCreateInfoKHR SwapchainInfo() {
    VkSwapchainCreateInfoKHR info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    info.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
    info.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    info.imageExtent = {1920, 1080};
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    return info;
}

// A blendable format, on a device without VkImageViewUsageCreateInfo
constexpr monitor::OverlaySupport kBlend = {true, false};

constexpr const char* kKinds[] = {"view",     "framebuffer",   "semaphore",    "render pass",    "pipeline layout",
                                  "pipeline", "shader module", "command pool", "command buffer", "fence"};

void ExpectNothingLive(FakeDevice& device) {
    for (const char* kind : kKinds) {
        EXPECT_EQ(device.Live(kind), 0) << kind;
    }
}

TEST(MonitorPresent, SwapchainSetupCreatesObjectsPerImageAndTeardownFreesThem) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::OverlayPipelines pipelines;
    monitor::SwapchainOverlay overlay;
    monitor::SetupSwapchainOverlay(kDevice, dispatch, MakeHandle<VkSwapchainKHR>(0x50),
                                   monitor::PatchSwapchainCreateInfo(SwapchainInfo(), monitor::LineFlags{}), kBlend, pipelines,
                                   overlay);

    EXPECT_TRUE(overlay.ok);
    EXPECT_EQ(overlay.images.size(), 3u);
    EXPECT_EQ(device.Live("view"), 3);
    EXPECT_EQ(device.Live("framebuffer"), 3);
    EXPECT_EQ(device.Live("semaphore"), 3);
    EXPECT_EQ(device.Live("pipeline"), 1);
    EXPECT_EQ(device.Live("shader module"), 0);

    monitor::DestroySwapchainOverlay(kDevice, dispatch, overlay);
    monitor::DestroyOverlayPipelines(kDevice, dispatch, pipelines);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, RecreatedSwapchainReusesTheOverlayPipeline) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::OverlayPipelines pipelines;
    VkSwapchainCreateInfoKHR info = SwapchainInfo();

    // A resize destroys the swapchain and creates a new one with the same format
    monitor::SwapchainOverlay old_overlay;
    monitor::SetupSwapchainOverlay(kDevice, dispatch, MakeHandle<VkSwapchainKHR>(0x50),
                                   monitor::PatchSwapchainCreateInfo(info, monitor::LineFlags{}), kBlend, pipelines, old_overlay);
    monitor::DestroySwapchainOverlay(kDevice, dispatch, old_overlay);
    monitor::SwapchainOverlay new_overlay;
    monitor::SetupSwapchainOverlay(kDevice, dispatch, MakeHandle<VkSwapchainKHR>(0x60),
                                   monitor::PatchSwapchainCreateInfo(info, monitor::LineFlags{}), kBlend, pipelines, new_overlay);
    EXPECT_TRUE(new_overlay.ok);
    EXPECT_EQ(device.created["pipeline"], 1);

    // Another format, or no blending, needs its own pipeline
    info.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
    monitor::SwapchainOverlay other_format;
    monitor::SetupSwapchainOverlay(kDevice, dispatch, MakeHandle<VkSwapchainKHR>(0x70),
                                   monitor::PatchSwapchainCreateInfo(info, monitor::LineFlags{}), kBlend, pipelines, other_format);
    monitor::SwapchainOverlay opaque;
    monitor::SetupSwapchainOverlay(kDevice, dispatch, MakeHandle<VkSwapchainKHR>(0x80),
                                   monitor::PatchSwapchainCreateInfo(info, monitor::LineFlags{}), {false, false}, pipelines,
                                   opaque);
    EXPECT_EQ(device.created["pipeline"], 3);

    for (monitor::SwapchainOverlay* overlay : {&new_overlay, &other_format, &opaque}) {
        monitor::DestroySwapchainOverlay(kDevice, dispatch, *overlay);
    }
    EXPECT_EQ(device.Live("pipeline"), 3);  // Kept for the next swapchain
    monitor::DestroyOverlayPipelines(kDevice, dispatch, pipelines);
    ExpectNothingLive(device);
}

// The view inherits the swapchain's usage otherwise, and usage like STORAGE can make it invalid for the view format.
TEST(MonitorPresent, ViewsAreLimitedToColorAttachmentUsageWhenTheDeviceAllowsIt) {
    for (const bool view_usage : {true, false}) {
        FakeDevice device;
        const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
        monitor::OverlayPipelines pipelines;
        VkSwapchainCreateInfoKHR info = SwapchainInfo();
        info.imageUsage |= VK_IMAGE_USAGE_STORAGE_BIT;
        monitor::SwapchainOverlay overlay;
        monitor::SetupSwapchainOverlay(kDevice, dispatch, MakeHandle<VkSwapchainKHR>(0x50),
                                       monitor::PatchSwapchainCreateInfo(info, monitor::LineFlags{}), {true, view_usage}, pipelines,
                                       overlay);

        ASSERT_EQ(device.view_usages.size(), 3u);
        for (const VkImageUsageFlags usage : device.view_usages) {
            EXPECT_EQ(usage, view_usage ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : 0u) << "view_usage " << view_usage;
        }
        monitor::DestroySwapchainOverlay(kDevice, dispatch, overlay);
        monitor::DestroyOverlayPipelines(kDevice, dispatch, pipelines);
        ExpectNothingLive(device);
    }
}

// The swapchain is created and its overlay set up from one decision, so the two can't disagree.
TEST(MonitorPresent, SetupFollowsTheDecisionTheSwapchainWasCreatedWith) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::OverlayPipelines pipelines;
    monitor::SwapchainPatch patch = monitor::PatchSwapchainCreateInfo(SwapchainInfo(), monitor::LineFlags{});
    patch.eligibility.ok = false;
    patch.eligibility.reason = "decided at create time";
    monitor::SwapchainOverlay overlay;
    monitor::SetupSwapchainOverlay(kDevice, dispatch, MakeHandle<VkSwapchainKHR>(0x50), patch, kBlend, pipelines, overlay);

    EXPECT_FALSE(overlay.ok);
    EXPECT_STREQ(overlay.reason, "decided at create time");
    EXPECT_EQ(device.created["view"], 0);
}

TEST(MonitorPresent, DeferredSwapchainCreatesViewsOnFirstPresentOfEachImage) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    VkSwapchainCreateInfoKHR info = SwapchainInfo();
    info.flags = VK_SWAPCHAIN_CREATE_DEFERRED_MEMORY_ALLOCATION_BIT_KHR;
    monitor::OverlayPipelines pipelines;
    monitor::SwapchainOverlay overlay;
    monitor::SetupSwapchainOverlay(kDevice, dispatch, MakeHandle<VkSwapchainKHR>(0x50),
                                   monitor::PatchSwapchainCreateInfo(info, monitor::LineFlags{}), kBlend, pipelines, overlay);

    EXPECT_TRUE(overlay.ok);
    EXPECT_EQ(device.Live("semaphore"), 3);
    EXPECT_EQ(device.Live("view"), 0);  // The images may have no memory yet

    EXPECT_TRUE(monitor::PrepareImage(kDevice, dispatch, overlay, 2));
    EXPECT_EQ(device.Live("view"), 1);
    EXPECT_TRUE(monitor::PrepareImage(kDevice, dispatch, overlay, 2));
    EXPECT_EQ(device.Live("view"), 1);
    EXPECT_TRUE(monitor::PrepareImage(kDevice, dispatch, overlay, 0));
    EXPECT_TRUE(monitor::PrepareImage(kDevice, dispatch, overlay, 1));
    EXPECT_EQ(device.Live("view"), 3);
    EXPECT_EQ(device.Live("framebuffer"), 3);
    EXPECT_FALSE(monitor::PrepareImage(kDevice, dispatch, overlay, 3));

    monitor::DestroySwapchainOverlay(kDevice, dispatch, overlay);
    monitor::DestroyOverlayPipelines(kDevice, dispatch, pipelines);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, FailedSwapchainSetupDisablesOverlayWithoutLeaking) {
    // Fail part way through the per image objects, or the one-off pipeline objects
    const std::pair<const char*, int> failures[] = {
        {"view", 2}, {"framebuffer", 2}, {"semaphore", 2}, {"render pass", 1}, {"pipeline", 1}};
    for (const auto& failure : failures) {
        const char* kind = failure.first;
        FakeDevice device;
        device.fail_at[kind] = failure.second;
        const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
        monitor::OverlayPipelines pipelines;
        monitor::SwapchainOverlay overlay;
        monitor::SetupSwapchainOverlay(kDevice, dispatch, MakeHandle<VkSwapchainKHR>(0x50),
                                       monitor::PatchSwapchainCreateInfo(SwapchainInfo(), monitor::LineFlags{}), kBlend, pipelines,
                                       overlay);

        EXPECT_FALSE(overlay.ok) << kind;
        EXPECT_NE(overlay.reason, nullptr) << kind;
        monitor::DestroySwapchainOverlay(kDevice, dispatch, overlay);
        monitor::DestroyOverlayPipelines(kDevice, dispatch, pipelines);
        ExpectNothingLive(device);
    }
}

TEST(MonitorPresent, IneligibleSwapchainCreatesNothing) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    VkSwapchainCreateInfoKHR info = SwapchainInfo();
    info.presentMode = VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;
    monitor::OverlayPipelines pipelines;
    monitor::SwapchainOverlay overlay;
    monitor::SetupSwapchainOverlay(kDevice, dispatch, MakeHandle<VkSwapchainKHR>(0x50),
                                   monitor::PatchSwapchainCreateInfo(info, monitor::LineFlags{}), kBlend, pipelines, overlay);

    EXPECT_FALSE(overlay.ok);
    EXPECT_NE(overlay.reason, nullptr);
    for (const char* kind : kKinds) {
        EXPECT_EQ(device.created[kind], 0) << kind;
    }
}

VKAPI_ATTR VkResult VKAPI_CALL SetLoaderData(VkDevice /*device*/, void* object) {
    ++fake->loader_data_calls;
    *static_cast<uint64_t*>(object) = 0x10ADE7;  // Where the loader keeps its dispatch pointer
    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL FailLoaderData(VkDevice /*device*/, void* /*object*/) {
    ++fake->loader_data_calls;
    return VK_ERROR_INITIALIZATION_FAILED;
}

constexpr uint32_t kSlots = 3;

std::vector<std::string> warnings;
void RecordWarning(const char* message) { warnings.emplace_back(message); }

// A device with one graphics queue family and kQueue in it, the way the layer sets it up at vkCreateDevice.
// Also forgets the warnings of earlier tests.
void SetUpGraphicsDevice(monitor::DeviceOverlay& state) {
    warnings.clear();
    state.device = kDevice;
    state.set_loader_data = SetLoaderData;
    state.queue_families = {{VK_QUEUE_GRAPHICS_BIT, 2, 0, {1, 1, 1}}};
    state.queue_family_index[kQueue] = 0;
}

// Adds a swapchain the way vkCreateSwapchainKHR does
monitor::SwapchainOverlay& AddSwapchain(const VkuDeviceDispatchTable& dispatch, monitor::DeviceOverlay& state,
                                        VkSwapchainKHR swapchain) {
    EXPECT_EQ(monitor::AddSwapchainOverlay(dispatch, state, swapchain,
                                           monitor::PatchSwapchainCreateInfo(SwapchainInfo(), monitor::LineFlags{}), true),
              nullptr);
    return *state.swapchains.at(swapchain);
}

// An app present of one image of each swapchain, after its rendering signaled `rendered`
struct AppPresentInfo {
    std::vector<VkSwapchainKHR> swapchains;
    std::vector<uint32_t> indices;
    VkSemaphore rendered = MakeHandle<VkSemaphore>(0xA1);
    VkPresentInfoKHR info = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};

    AppPresentInfo(std::vector<VkSwapchainKHR> presented, std::vector<uint32_t> image_indices)
        : swapchains(std::move(presented)), indices(std::move(image_indices)) {
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &rendered;
        info.swapchainCount = static_cast<uint32_t>(swapchains.size());
        info.pSwapchains = swapchains.data();
        info.pImageIndices = indices.data();
    }
    AppPresentInfo(const AppPresentInfo&) = delete;
    AppPresentInfo& operator=(const AppPresentInfo&) = delete;
    AppPresentInfo(AppPresentInfo&&) = delete;
    AppPresentInfo& operator=(AppPresentInfo&&) = delete;
    ~AppPresentInfo() = default;
};

VkResult PresentWithOverlay(const VkuDeviceDispatchTable& dispatch, monitor::DeviceOverlay& state, const VkPresentInfoKHR& info,
                            VkQueue queue = kQueue) {
    return monitor::PresentWithOverlay(dispatch, state, queue, info, monitor::MonitorSettings{}, 7,
                                       std::chrono::steady_clock::now(), RecordWarning);
}

TEST(MonitorPresent, PresentDrawsTheOverlayBetweenTheAppAndThePresent) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::DeviceOverlay state;
    SetUpGraphicsDevice(state);
    const monitor::SwapchainOverlay& overlay = AddSwapchain(dispatch, state, MakeHandle<VkSwapchainKHR>(0x50));

    // The second swapchain was never seen by the layer, so it's passed through without an overlay
    const AppPresentInfo app({MakeHandle<VkSwapchainKHR>(0x50), MakeHandle<VkSwapchainKHR>(0x99)}, {1, 0});
    ASSERT_EQ(PresentWithOverlay(dispatch, state, app.info), VK_SUCCESS);

    ASSERT_EQ(device.submissions.size(), 1u);
    EXPECT_EQ(device.submissions[0].waits, std::vector<VkSemaphore>{app.rendered});
    EXPECT_EQ(device.submissions[0].signals, std::vector<VkSemaphore>{overlay.images[1].present_wait});
    EXPECT_EQ(device.draws, 1);
    ASSERT_EQ(device.presents.size(), 1u);
    EXPECT_EQ(device.presents[0].waits, std::vector<VkSemaphore>{overlay.images[1].present_wait});
    EXPECT_EQ(device.presents[0].swapchains, app.swapchains);

    monitor::DestroyDeviceOverlay(dispatch, state);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, QueuesOfOneFamilyDoNotWaitForEachOther) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::DeviceOverlay state;
    SetUpGraphicsDevice(state);
    uint64_t second_queue_object = 0;
    auto* const second_queue = reinterpret_cast<VkQueue>(&second_queue_object);
    state.queue_family_index[second_queue] = 0;
    AddSwapchain(dispatch, state, MakeHandle<VkSwapchainKHR>(0x50));
    AddSwapchain(dispatch, state, MakeHandle<VkSwapchainKHR>(0x60));

    for (uint32_t i = 0; i < kSlots; ++i) {
        const AppPresentInfo app({MakeHandle<VkSwapchainKHR>(0x50)}, {i});
        ASSERT_EQ(PresentWithOverlay(dispatch, state, app.info), VK_SUCCESS);
    }
    const AppPresentInfo other({MakeHandle<VkSwapchainKHR>(0x60)}, {0});
    ASSERT_EQ(PresentWithOverlay(dispatch, state, other.info, second_queue), VK_SUCCESS);
    EXPECT_TRUE(device.fences_waited.empty());

    monitor::DestroyDeviceOverlay(dispatch, state);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, ForgettingASwapchainWaitsForItsOverlayOnEveryQueue) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::DeviceOverlay state;
    SetUpGraphicsDevice(state);
    const monitor::SwapchainOverlay& overlay = AddSwapchain(dispatch, state, MakeHandle<VkSwapchainKHR>(0x50));
    const VkSemaphore present_wait = overlay.images[0].present_wait;

    const AppPresentInfo app({MakeHandle<VkSwapchainKHR>(0x50)}, {0});
    ASSERT_EQ(PresentWithOverlay(dispatch, state, app.info), VK_SUCCESS);
    monitor::ForgetSwapchain(dispatch, state, MakeHandle<VkSwapchainKHR>(0x50));

    EXPECT_EQ(device.fences_waited.size(), 1u);
    EXPECT_EQ(device.hung_waits, 0);
    EXPECT_TRUE(state.swapchains.empty());
    EXPECT_EQ(device.Live("semaphore"), 0) << HandleValue(present_wait);

    monitor::DestroyDeviceOverlay(dispatch, state);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, PresentWithoutKnownSwapchainsGoesStraightThrough) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::DeviceOverlay state;
    SetUpGraphicsDevice(state);

    const AppPresentInfo app({MakeHandle<VkSwapchainKHR>(0x99)}, {0});
    ASSERT_EQ(PresentWithOverlay(dispatch, state, app.info), VK_SUCCESS);

    EXPECT_TRUE(device.submissions.empty());
    ASSERT_EQ(device.presents.size(), 1u);
    EXPECT_EQ(device.presents[0].waits, std::vector<VkSemaphore>{app.rendered});
    monitor::DestroyDeviceOverlay(dispatch, state);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, SlotRingGivesEveryCommandBufferLoaderDispatch) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::SlotRing ring;
    monitor::CreateSlotRing(kDevice, dispatch, 0, SetLoaderData, kSlots, ring);

    EXPECT_TRUE(ring.ok);
    EXPECT_EQ(device.Live("command buffer"), static_cast<int>(kSlots));
    EXPECT_EQ(device.loader_data_calls, static_cast<int>(kSlots));
    for (const monitor::SubmitSlot& slot : ring.slots) {
        EXPECT_EQ(*reinterpret_cast<uint64_t*>(slot.command_buffer), 0x10ADE7u);
    }

    monitor::DestroySlotRing(kDevice, dispatch, ring);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, SlotRingIsDisabledWithoutLoaderDispatch) {
    for (PFN_vkSetDeviceLoaderData set_loader_data : {static_cast<PFN_vkSetDeviceLoaderData>(nullptr), FailLoaderData}) {
        FakeDevice device;
        const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
        monitor::SlotRing ring;
        monitor::CreateSlotRing(kDevice, dispatch, 0, set_loader_data, kSlots, ring);

        EXPECT_FALSE(ring.ok);
        EXPECT_NE(ring.reason, nullptr);
        EXPECT_FALSE(ring.retry);  // The loader won't change, so there's no point trying again
        monitor::DestroySlotRing(kDevice, dispatch, ring);
        ExpectNothingLive(device);
    }
}

// Running out of memory may not last, so the ring frees what it made and can be created again
TEST(MonitorPresent, SlotRingCanBeRetriedAfterRunningOutOfMemory) {
    const std::pair<const char*, int> failures[] = {{"command pool", 1}, {"command buffer", 2}, {"fence", 2}};
    for (const auto& failure : failures) {
        const char* kind = failure.first;
        FakeDevice device;
        device.fail_at[kind] = failure.second;
        const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
        monitor::SlotRing ring;
        monitor::CreateSlotRing(kDevice, dispatch, 0, SetLoaderData, kSlots, ring);

        EXPECT_FALSE(ring.ok) << kind;
        EXPECT_NE(ring.reason, nullptr) << kind;
        EXPECT_TRUE(ring.retry) << kind;
        ExpectNothingLive(device);

        monitor::CreateSlotRing(kDevice, dispatch, 0, SetLoaderData, kSlots, ring);
        EXPECT_TRUE(ring.ok) << kind;
        monitor::DestroySlotRing(kDevice, dispatch, ring);
        ExpectNothingLive(device);
    }
}

// Takes the next slot and submits it with no semaphores, as one present would.
VkResult Present(monitor::SlotRing& ring, const VkuDeviceDispatchTable& dispatch, VkSwapchainKHR swapchain) {
    monitor::SubmitSlot& slot = monitor::AcquireSlot(kDevice, dispatch, ring);
    return monitor::SubmitOverlay(kDevice, dispatch, kQueue, slot, {}, {}, {swapchain});
}

TEST(MonitorPresent, SlotRingOnlyWaitsForSubmittedWork) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::SlotRing ring;
    monitor::CreateSlotRing(kDevice, dispatch, 0, SetLoaderData, kSlots, ring);
    auto* const swapchain = MakeHandle<VkSwapchainKHR>(0x50);

    // The first trip around the ring has nothing to wait for
    for (uint32_t i = 0; i < kSlots; ++i) {
        ASSERT_EQ(Present(ring, dispatch, swapchain), VK_SUCCESS);
    }
    EXPECT_TRUE(device.fences_waited.empty());

    // Then each slot waits for its previous submission
    ASSERT_EQ(Present(ring, dispatch, swapchain), VK_SUCCESS);
    EXPECT_EQ(device.fences_waited.size(), 1u);
    EXPECT_EQ(device.hung_waits, 0);

    monitor::DestroySlotRing(kDevice, dispatch, ring);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, FailedSubmitDoesNotLeaveAFenceToWaitOnForever) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::SlotRing ring;
    monitor::CreateSlotRing(kDevice, dispatch, 0, SetLoaderData, kSlots, ring);
    auto* const swapchain = MakeHandle<VkSwapchainKHR>(0x50);

    for (uint32_t i = 0; i < kSlots; ++i) {
        ASSERT_EQ(Present(ring, dispatch, swapchain), VK_SUCCESS);
    }
    device.submit_result = VK_ERROR_OUT_OF_DEVICE_MEMORY;
    EXPECT_EQ(Present(ring, dispatch, swapchain), VK_ERROR_OUT_OF_DEVICE_MEMORY);
    device.submit_result = VK_SUCCESS;

    for (uint32_t i = 0; i < kSlots + 1; ++i) {
        ASSERT_EQ(Present(ring, dispatch, swapchain), VK_SUCCESS);
    }
    EXPECT_EQ(device.hung_waits, 0);

    monitor::DestroySlotRing(kDevice, dispatch, ring);
    EXPECT_EQ(device.hung_waits, 0);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, WaitForSwapchainOnlyWaitsForItsOwnSubmissions) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::SlotRing ring;
    monitor::CreateSlotRing(kDevice, dispatch, 0, SetLoaderData, kSlots, ring);
    auto* const first = MakeHandle<VkSwapchainKHR>(0x50);
    auto* const second = MakeHandle<VkSwapchainKHR>(0x60);

    ASSERT_EQ(Present(ring, dispatch, first), VK_SUCCESS);
    ASSERT_EQ(Present(ring, dispatch, second), VK_SUCCESS);
    monitor::WaitForSwapchain(kDevice, dispatch, ring, first);

    EXPECT_EQ(device.fences_waited.size(), 1u);
    EXPECT_EQ(device.fences_waited.count(HandleValue(ring.slots[0].fence)), 1u);
    EXPECT_EQ(device.hung_waits, 0);

    monitor::DestroySlotRing(kDevice, dispatch, ring);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, SubmitWaitsForTheAppAndSignalsThePresent) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::SlotRing ring;
    monitor::CreateSlotRing(kDevice, dispatch, 0, SetLoaderData, kSlots, ring);
    const std::vector<VkSemaphore> app = {MakeHandle<VkSemaphore>(0xA1), MakeHandle<VkSemaphore>(0xA2)};
    const std::vector<VkSemaphore> ours = {MakeHandle<VkSemaphore>(0xB1)};

    monitor::SubmitSlot& slot = monitor::AcquireSlot(kDevice, dispatch, ring);
    ASSERT_EQ(monitor::SubmitOverlay(kDevice, dispatch, kQueue, slot, app, ours, {MakeHandle<VkSwapchainKHR>(0x50)}), VK_SUCCESS);

    ASSERT_EQ(device.submissions.size(), 1u);
    const Submission& submission = device.submissions[0];
    EXPECT_EQ(submission.waits, app);
    EXPECT_EQ(submission.wait_stages, std::vector<VkPipelineStageFlags>(2, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT));
    EXPECT_EQ(submission.signals, ours);
    EXPECT_EQ(submission.command_buffer_count, 1u);
    EXPECT_EQ(HandleValue(submission.fence), HandleValue(slot.fence));

    monitor::DestroySlotRing(kDevice, dispatch, ring);
    ExpectNothingLive(device);
}

// The overlays RewritePresentInfo gets: one per presented swapchain, null where none is drawn
std::vector<monitor::SwapchainOverlay*> Drawn(std::initializer_list<bool> drawn) {
    static monitor::SwapchainOverlay overlay;
    std::vector<monitor::SwapchainOverlay*> overlays;
    for (const bool is_drawn : drawn) {
        overlays.push_back(is_drawn ? &overlay : nullptr);
    }
    return overlays;
}

struct AppPresent {
    VkSwapchainKHR swapchains[2] = {MakeHandle<VkSwapchainKHR>(0x50), MakeHandle<VkSwapchainKHR>(0x60)};
    uint32_t indices[2] = {1, 2};
    VkResult results[2] = {};
    VkSemaphore waits[1] = {MakeHandle<VkSemaphore>(0xA1)};
    VkRectLayerKHR rects[2] = {{{0, 0}, {16, 16}, 0}, {{32, 32}, {8, 8}, 0}};
    VkPresentRegionKHR regions[2] = {{1, &rects[0]}, {1, &rects[1]}};
    VkPresentRegionsKHR present_regions = {VK_STRUCTURE_TYPE_PRESENT_REGIONS_KHR, nullptr, 2, regions};
    VkPresentInfoKHR info = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};

    AppPresent() {
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = waits;
        info.swapchainCount = 2;
        info.pSwapchains = swapchains;
        info.pImageIndices = indices;
        info.pResults = results;
    }
};

const VkPresentRegionsKHR* FindRegions(const void* chain) {
    for (const auto* node = static_cast<const VkBaseInStructure*>(chain); node; node = node->pNext) {
        if (node->sType == VK_STRUCTURE_TYPE_PRESENT_REGIONS_KHR) {
            return reinterpret_cast<const VkPresentRegionsKHR*>(node);
        }
    }
    return nullptr;
}

TEST(MonitorPresent, PresentWaitsOnTheOverlayInsteadOfTheApp) {
    const AppPresent app;
    const std::vector<VkSemaphore> ours = {MakeHandle<VkSemaphore>(0xB1), MakeHandle<VkSemaphore>(0xB2)};
    monitor::PresentRewrite rewrite;
    const VkPresentInfoKHR& info = monitor::RewritePresentInfo(app.info, ours, Drawn({true, true}), rewrite);

    ASSERT_EQ(info.waitSemaphoreCount, 2u);
    EXPECT_EQ(HandleValue(info.pWaitSemaphores[0]), 0xB1u);
    EXPECT_EQ(HandleValue(info.pWaitSemaphores[1]), 0xB2u);
    EXPECT_EQ(info.swapchainCount, 2u);
    EXPECT_EQ(info.pSwapchains, app.swapchains);
    EXPECT_EQ(info.pImageIndices, app.indices);
    EXPECT_EQ(info.pResults, app.results);
    EXPECT_EQ(info.pNext, nullptr);
    EXPECT_EQ(app.info.pWaitSemaphores, app.waits);  // The app's struct is untouched
}

TEST(MonitorPresent, PresentRegionsCoverTheWholeImageWhenTheOverlayIsDrawn) {
    AppPresent app;
    VkPresentIdKHR present_id = {VK_STRUCTURE_TYPE_PRESENT_ID_KHR, nullptr, 2, nullptr};
    VkDisplayPresentInfoKHR display = {VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR, &app.present_regions};
    app.present_regions.pNext = &present_id;
    app.info.pNext = &display;

    monitor::PresentRewrite rewrite;
    const VkPresentInfoKHR& info =
        monitor::RewritePresentInfo(app.info, {MakeHandle<VkSemaphore>(0xB1)}, Drawn({false, true}), rewrite);

    // The display struct before the regions is still there, and so is the present ID after them
    const auto* first = static_cast<const VkBaseInStructure*>(info.pNext);
    ASSERT_NE(first, nullptr);
    EXPECT_EQ(first->sType, VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR);
    const VkPresentRegionsKHR* regions = FindRegions(info.pNext);
    ASSERT_NE(regions, nullptr);
    EXPECT_NE(regions, &app.present_regions);
    EXPECT_EQ(regions->pNext, &present_id);

    // Only the swapchain with the overlay reports the whole image as changed
    ASSERT_EQ(regions->swapchainCount, 2u);
    EXPECT_EQ(regions->pRegions[0].rectangleCount, 1u);
    EXPECT_EQ(regions->pRegions[0].pRectangles, &app.rects[0]);
    EXPECT_EQ(regions->pRegions[1].rectangleCount, 0u);
    EXPECT_FALSE(rewrite.regions_unchanged);

    // The app's structs are untouched
    EXPECT_EQ(app.regions[1].rectangleCount, 1u);
    EXPECT_EQ(display.pNext, &app.present_regions);
}

struct UnknownStruct {
    VkStructureType sType = VK_STRUCTURE_TYPE_MAX_ENUM;
    const void* pNext = nullptr;
};

TEST(MonitorPresent, PresentRegionsAreLeftAloneBehindAnUnknownStruct) {
    AppPresent app;
    UnknownStruct unknown = {static_cast<VkStructureType>(0x7FFF0001), &app.present_regions};
    app.info.pNext = &unknown;

    monitor::PresentRewrite rewrite;
    const VkPresentInfoKHR& info =
        monitor::RewritePresentInfo(app.info, {MakeHandle<VkSemaphore>(0xB1)}, Drawn({true, true}), rewrite);

    EXPECT_EQ(info.pNext, &unknown);
    EXPECT_TRUE(rewrite.regions_unchanged);
}

TEST(MonitorPresent, PresentRewriteCanBeReused) {
    AppPresent with_regions;
    with_regions.info.pNext = &with_regions.present_regions;
    AppPresent without_regions;
    UnknownStruct unknown = {static_cast<VkStructureType>(0x7FFF0001), &without_regions.present_regions};
    without_regions.info.pNext = &unknown;

    monitor::PresentRewrite rewrite;
    (void)monitor::RewritePresentInfo(without_regions.info, {MakeHandle<VkSemaphore>(0xB1)}, Drawn({true, true}), rewrite);
    EXPECT_TRUE(rewrite.regions_unchanged);

    const VkPresentInfoKHR& info =
        monitor::RewritePresentInfo(with_regions.info, {MakeHandle<VkSemaphore>(0xC1)}, Drawn({true, false}), rewrite);
    EXPECT_FALSE(rewrite.regions_unchanged);
    ASSERT_EQ(info.waitSemaphoreCount, 1u);
    EXPECT_EQ(HandleValue(info.pWaitSemaphores[0]), 0xC1u);
    const VkPresentRegionsKHR* regions = FindRegions(info.pNext);
    ASSERT_NE(regions, nullptr);
    EXPECT_EQ(regions->pRegions[0].rectangleCount, 0u);
    EXPECT_EQ(regions->pRegions[1].rectangleCount, 1u);

    const AppPresent no_chain;
    const VkPresentInfoKHR& plain = monitor::RewritePresentInfo(no_chain.info, {}, Drawn({true, true}), rewrite);
    EXPECT_EQ(plain.pNext, nullptr);
    EXPECT_EQ(plain.waitSemaphoreCount, 0u);
}

TEST(MonitorPresent, RetryingAFailedImageDoesNotLeak) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    VkSwapchainCreateInfoKHR info = SwapchainInfo();
    info.flags = VK_SWAPCHAIN_CREATE_DEFERRED_MEMORY_ALLOCATION_BIT_KHR;
    monitor::OverlayPipelines pipelines;
    monitor::SwapchainOverlay overlay;
    monitor::SetupSwapchainOverlay(kDevice, dispatch, MakeHandle<VkSwapchainKHR>(0x50),
                                   monitor::PatchSwapchainCreateInfo(info, monitor::LineFlags{}), kBlend, pipelines, overlay);

    device.fail_at["framebuffer"] = 1;
    EXPECT_FALSE(monitor::PrepareImage(kDevice, dispatch, overlay, 0));
    EXPECT_TRUE(monitor::PrepareImage(kDevice, dispatch, overlay, 0));
    EXPECT_EQ(device.Live("view"), 1);

    monitor::DestroySwapchainOverlay(kDevice, dispatch, overlay);
    monitor::DestroyOverlayPipelines(kDevice, dispatch, pipelines);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, SlotRingNeedsAtLeastOneSlot) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::SlotRing ring;
    monitor::CreateSlotRing(kDevice, dispatch, 0, SetLoaderData, 0, ring);
    EXPECT_FALSE(ring.ok);
    monitor::DestroySlotRing(kDevice, dispatch, ring);
    ExpectNothingLive(device);
}

VKAPI_ATTR VkResult VKAPI_CALL FailResetFences(VkDevice /*device*/, uint32_t /*fenceCount*/, const VkFence* /*pFences*/) {
    return VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

TEST(MonitorPresent, SubmitIsSkippedIfTheFenceCantBeReset) {
    FakeDevice device;
    VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::SlotRing ring;
    monitor::CreateSlotRing(kDevice, dispatch, 0, SetLoaderData, kSlots, ring);

    dispatch.ResetFences = FailResetFences;
    EXPECT_EQ(Present(ring, dispatch, MakeHandle<VkSwapchainKHR>(0x50)), VK_ERROR_OUT_OF_DEVICE_MEMORY);
    EXPECT_TRUE(device.submissions.empty());
    EXPECT_FALSE(ring.slots[0].in_flight);

    monitor::DestroySlotRing(kDevice, dispatch, ring);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, UndoWithNoSemaphoresSubmitsNothing) {
    const FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    EXPECT_EQ(monitor::UndoOverlaySemaphores(dispatch, kQueue, {}, {}), VK_SUCCESS);
    EXPECT_TRUE(device.submissions.empty());
}

TEST(MonitorPresent, UndoUnsignalsTheOverlaySemaphoresAndSignalsTheAppsAgain) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    const std::vector<VkSemaphore> ours = {MakeHandle<VkSemaphore>(0xB1), MakeHandle<VkSemaphore>(0xB2)};
    const std::vector<VkSemaphore> apps = {MakeHandle<VkSemaphore>(0xA1)};
    EXPECT_EQ(monitor::UndoOverlaySemaphores(dispatch, kQueue, ours, apps), VK_SUCCESS);

    ASSERT_EQ(device.submissions.size(), 1u);
    EXPECT_EQ(device.submissions[0].waits, ours);
    EXPECT_EQ(device.submissions[0].wait_stages, std::vector<VkPipelineStageFlags>(2, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT));
    EXPECT_EQ(device.submissions[0].command_buffer_count, 0u);
    EXPECT_EQ(device.submissions[0].signals, apps);
}

// A present that fails with an out of memory error must leave the app's semaphores as they were, but the overlay submit
// already waited on them.
TEST(MonitorPresent, PresentOutOfMemoryGivesTheAppItsSemaphoresBack) {
    for (const VkResult error : {VK_ERROR_OUT_OF_HOST_MEMORY, VK_ERROR_OUT_OF_DEVICE_MEMORY}) {
        FakeDevice device;
        const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
        monitor::DeviceOverlay state;
        SetUpGraphicsDevice(state);
        const monitor::SwapchainOverlay& overlay = AddSwapchain(dispatch, state, MakeHandle<VkSwapchainKHR>(0x50));

        device.present_result = error;
        const AppPresentInfo app({MakeHandle<VkSwapchainKHR>(0x50)}, {1});
        EXPECT_EQ(PresentWithOverlay(dispatch, state, app.info), error);

        ASSERT_EQ(device.submissions.size(), 2u) << error;
        const std::vector<VkSemaphore> ours = {overlay.images[1].present_wait};
        EXPECT_EQ(device.submissions[1].waits, ours) << error;
        EXPECT_EQ(device.submissions[1].signals, std::vector<VkSemaphore>{app.rendered}) << error;
        EXPECT_EQ(device.submissions[1].command_buffer_count, 0u) << error;

        monitor::DestroyDeviceOverlay(dispatch, state);
        ExpectNothingLive(device);
    }
}

// A queue whose command pool couldn't be created the first time gets the overlay on a later present
TEST(MonitorPresent, PresentRetriesTheQueueAfterRunningOutOfMemory) {
    FakeDevice device;
    device.fail_at["command pool"] = 1;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::DeviceOverlay state;
    SetUpGraphicsDevice(state);
    AddSwapchain(dispatch, state, MakeHandle<VkSwapchainKHR>(0x50));

    const AppPresentInfo app({MakeHandle<VkSwapchainKHR>(0x50)}, {1});
    EXPECT_EQ(PresentWithOverlay(dispatch, state, app.info), VK_SUCCESS);
    EXPECT_TRUE(device.submissions.empty());
    EXPECT_EQ(PresentWithOverlay(dispatch, state, app.info), VK_SUCCESS);
    EXPECT_EQ(device.submissions.size(), 1u);
    EXPECT_EQ(device.draws, 1);

    monitor::DestroyDeviceOverlay(dispatch, state);
    ExpectNothingLive(device);
}

// Other errors mean the present was enqueued, so its waits happened and there is nothing to undo
TEST(MonitorPresent, PresentOutOfDateUndoesNothing) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::DeviceOverlay state;
    SetUpGraphicsDevice(state);
    AddSwapchain(dispatch, state, MakeHandle<VkSwapchainKHR>(0x50));

    device.present_result = VK_ERROR_OUT_OF_DATE_KHR;
    const AppPresentInfo app({MakeHandle<VkSwapchainKHR>(0x50)}, {1});
    EXPECT_EQ(PresentWithOverlay(dispatch, state, app.info), VK_ERROR_OUT_OF_DATE_KHR);
    EXPECT_EQ(device.submissions.size(), 1u);

    monitor::DestroyDeviceOverlay(dispatch, state);
    ExpectNothingLive(device);
}

// The present only updates the app's dirty rectangles then, so the overlay may not update; say so once.
TEST(MonitorPresent, PresentWarnsWhenPresentRegionsCantCoverTheOverlay) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::DeviceOverlay state;
    SetUpGraphicsDevice(state);
    AddSwapchain(dispatch, state, MakeHandle<VkSwapchainKHR>(0x50));

    AppPresentInfo app({MakeHandle<VkSwapchainKHR>(0x50)}, {0});
    constexpr VkRectLayerKHR rect = {{0, 0}, {16, 16}, 0};
    const VkPresentRegionKHR region = {1, &rect};
    VkPresentRegionsKHR regions = {VK_STRUCTURE_TYPE_PRESENT_REGIONS_KHR, nullptr, 1, &region};
    UnknownStruct unknown = {static_cast<VkStructureType>(0x7FFF0001), &regions};
    app.info.pNext = &unknown;
    ASSERT_EQ(PresentWithOverlay(dispatch, state, app.info), VK_SUCCESS);
    ASSERT_EQ(warnings.size(), 1u);
    EXPECT_NE(warnings[0].find("VkPresentRegionsKHR"), std::string::npos) << warnings[0];

    // Presents the layer can rewrite don't warn
    warnings.clear();
    app.info.pNext = &regions;
    ASSERT_EQ(PresentWithOverlay(dispatch, state, app.info), VK_SUCCESS);
    EXPECT_TRUE(warnings.empty());

    monitor::DestroyDeviceOverlay(dispatch, state);
    ExpectNothingLive(device);
}

// A queue the layer never saw retrieved isn't one without graphics, so the message has to say which it is.
TEST(MonitorPresent, PresentFromAnUnknownQueueSaysSo) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::DeviceOverlay state;
    SetUpGraphicsDevice(state);
    AddSwapchain(dispatch, state, MakeHandle<VkSwapchainKHR>(0x50));
    uint64_t unknown_queue_object = 0;
    auto* const unknown_queue = reinterpret_cast<VkQueue>(&unknown_queue_object);

    const AppPresentInfo app({MakeHandle<VkSwapchainKHR>(0x50)}, {0});
    ASSERT_EQ(PresentWithOverlay(dispatch, state, app.info, unknown_queue), VK_SUCCESS);
    EXPECT_TRUE(device.submissions.empty());
    ASSERT_EQ(device.presents.size(), 1u);
    ASSERT_EQ(warnings.size(), 1u);
    EXPECT_NE(warnings[0].find("queue"), std::string::npos) << warnings[0];
    EXPECT_EQ(warnings[0].find("graphics"), std::string::npos) << warnings[0];

    monitor::DestroyDeviceOverlay(dispatch, state);
    ExpectNothingLive(device);
}

TEST(MonitorPresent, PresentFromAQueueWithoutGraphicsSaysSo) {
    FakeDevice device;
    const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
    monitor::DeviceOverlay state;
    SetUpGraphicsDevice(state);
    state.queue_families.push_back({VK_QUEUE_COMPUTE_BIT, 1, 0, {1, 1, 1}});
    uint64_t compute_queue_object = 0;
    auto* const compute_queue = reinterpret_cast<VkQueue>(&compute_queue_object);
    state.queue_family_index[compute_queue] = 1;
    AddSwapchain(dispatch, state, MakeHandle<VkSwapchainKHR>(0x50));

    const AppPresentInfo app({MakeHandle<VkSwapchainKHR>(0x50)}, {0});
    ASSERT_EQ(PresentWithOverlay(dispatch, state, app.info, compute_queue), VK_SUCCESS);
    EXPECT_TRUE(device.submissions.empty());
    ASSERT_EQ(warnings.size(), 1u);
    EXPECT_NE(warnings[0].find("without graphics"), std::string::npos) << warnings[0];

    monitor::DestroyDeviceOverlay(dispatch, state);
    ExpectNothingLive(device);
}

// Whether views can be limited to color attachment usage is decided once per device
TEST(MonitorPresent, SwapchainsUseTheDevicesImageViewUsageSupport) {
    for (const bool view_usage : {true, false}) {
        FakeDevice device;
        const VkuDeviceDispatchTable dispatch = FakeDevice::Dispatch();
        monitor::DeviceOverlay state;
        SetUpGraphicsDevice(state);
        state.view_usage = view_usage;
        VkSwapchainCreateInfoKHR info = SwapchainInfo();
        info.imageUsage |= VK_IMAGE_USAGE_STORAGE_BIT;
        EXPECT_EQ(monitor::AddSwapchainOverlay(dispatch, state, MakeHandle<VkSwapchainKHR>(0x50),
                                               monitor::PatchSwapchainCreateInfo(info, monitor::LineFlags{}), true),
                  nullptr);

        ASSERT_EQ(device.view_usages.size(), 3u);
        for (const VkImageUsageFlags usage : device.view_usages) {
            EXPECT_EQ(usage, view_usage ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : 0u) << "view_usage " << view_usage;
        }
        monitor::DestroyDeviceOverlay(dispatch, state);
        ExpectNothingLive(device);
    }
}

// The layer shows each warning once
TEST(MonitorPresent, EachMessageIsShownOnce) {
    monitor::ShownMessages shown;
    EXPECT_TRUE(monitor::FirstTimeShown(shown, "mirrored swapchain transforms are drawn without the mirror"));
    EXPECT_FALSE(monitor::FirstTimeShown(shown, "mirrored swapchain transforms are drawn without the mirror"));
    EXPECT_TRUE(monitor::FirstTimeShown(shown, "unknown swapchain color space, the overlay may be too bright or too dark"));
}

// Warnings are only passed on when the user turned on log_warnings
TEST(MonitorPresent, WarningsOnlyReachTheUserWhenAsked) {
    warnings.clear();
    monitor::WarnIf(false, RecordWarning)("dropped");
    EXPECT_TRUE(warnings.empty());
    monitor::WarnIf(true, RecordWarning)("shown");
    EXPECT_EQ(warnings, std::vector<std::string>{"shown"});
}

}  // namespace
