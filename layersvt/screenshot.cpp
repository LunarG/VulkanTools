/*
 * Copyright (C) 2015-2021 Valve Corporation
 * Copyright (C) 2015-2021 LunarG, Inc.
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
 * Author: Cody Northrop <cody@lunarg.com>
 * Author: David Pinedo <david@lunarg.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Tony Barbour <tony@lunarg.com>
 */
#include <vulkan/utility/vk_format_utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <mutex>
#include <fstream>
#include <thread>
#include <condition_variable>

#if defined(_WIN32) && !defined(NDEBUG)
#include <crtdbg.h>
#endif

using namespace std;

#include <vulkan/layer/vk_layer_settings.hpp>
#include <vulkan/vk_enum_string_helper.h>
#include "vk_layer_table.h"

#include "screenshot_parsing.h"

#ifdef ANDROID
#include <android/trace.h>
#include <android/log.h>
#include <sys/system_properties.h>
#endif

namespace screenshot {

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

std::mutex globalLock;

std::condition_variable screenshotQueuedCV;
std::condition_variable screenshotSavedCV;
std::thread screenshotWriterThread;
bool shutdownScreenshotThread = false;
bool screenshotThreadStarted = false;

VkuLayerSettingSet globalLayerSettingSet = VK_NULL_HANDLE;

// If true, do not capture screenshots. Allows to control the layer at runtime.
bool pauseCapture = false;
const char *kSettingPauseCapture = "pause";

enum class ColorSpaceFormat { UNDEFINED, UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB };

// unordered map: associates Vulkan dispatchable objects to a dispatch table
typedef struct {
    VkuDeviceDispatchTable *device_dispatch_table;
    PFN_vkSetDeviceLoaderData pfn_dev_init;
} DispatchMapStruct;
static unordered_map<VkDevice, DispatchMapStruct *> dispatchMap;

// unordered map: associates a swap chain with a device, image extent, format,
// and list of images
typedef struct {
    VkDevice device;
    VkExtent2D imageExtent;
    VkFormat format;
    VkImage *imageList;
} SwapchainMapStruct;
static unordered_map<VkSwapchainKHR, SwapchainMapStruct *> swapchainMap;

// unordered map: associates an image with a device, image extent, and format
typedef struct {
    VkDevice device;
    VkExtent2D imageExtent;
    VkFormat format;
} ImageMapStruct;
static unordered_map<VkImage, ImageMapStruct *> imageMap;

// unordered map: associates a device with per device info -
//   wsi capability
//   set of queues created for this device
//   queue to queueFamilyIndex map
//   physical device
typedef struct {
    bool wsi_enabled;
    set<VkQueue> queues;
    unordered_map<VkQueue, uint32_t> queueIndexMap;
    VkPhysicalDevice physicalDevice;
} DeviceMapStruct;
static unordered_map<VkDevice, DeviceMapStruct *> deviceMap;

// unordered map: associates a physical device with an instance
typedef struct {
    VkInstance instance;
} PhysDeviceMapStruct;
static unordered_map<VkPhysicalDevice, PhysDeviceMapStruct *> physDeviceMap;

class Settings {
   public:
    ColorSpaceFormat userColorSpaceFormat = ColorSpaceFormat::UNDEFINED;

    // Where to write screenshots
    std::string targetFolder;

    // Screenshot file extension. Supported PPM and PAM.
    enum class ScreenshotExtension { PPM, PAM };
    ScreenshotExtension screenshotExtension = ScreenshotExtension::PPM;

    // Result screenshot scale. ScreenshotSize = scalePercent * FrameBufferSize / 100;
    int scalePercent = 100;

    // How many screenshots can be buffered.
    int maxScreenshotQueueSize = 10;

    // If true and screenshots queue is full skips new screenshots.
    // If false and screenshots queue is full delays next frame present.
    bool allowToSkipFrames = false;

    // Is profiling enabled
    bool isProfilingEnabled = true;

    // Checks if frame needs to be captured
    bool isFrameToCapture(int frame) const;
    // Checks if frame is after end of capture range
    bool isFrameAfterEndOfCaptureRange(int frame) const;

    // Init settings
    void init(VkuLayerSettingSet layerSettingSet);

   private:
    // Parse comma-separated frame list string into the set
    void populate_frame_list(const char *vk_screenshot_frames);

   private:
    // Screenshots will be generated from screenShotFrameRange's startFrame to startFrame+count-1 with skipped Interval in between.
    FrameRange screenShotFrameRange = {false, 0, SCREEN_SHOT_FRAMES_UNLIMITED, SCREEN_SHOT_FRAMES_INTERVAL_DEFAULT};

    // set: list of frames to take screenshots without duplication.
    set<int> screenshotFrames;
};

void Settings::init(VkuLayerSettingSet layerSettingSet) {
    const char *kSettingsKeyFrames = "frames";
    const char *kSettingKeyFormat = "format";
    const char *kSettingKeyDir = "dir";
    const char *kSettingScale = "scale";
    const char *kSettingQueueSize = "queue";
    const char *kSettingAllowSkip = "skip";
    const char *kSettingProfile = "profile";
    const char *kSettingScreenshotExtension = "extension";

    if (vkuHasLayerSetting(layerSettingSet, kSettingScale)) {
        vkuGetLayerSettingValue(layerSettingSet, kSettingScale, scalePercent);
        assert(scalePercent > 0);
    }

    if (vkuHasLayerSetting(layerSettingSet, kSettingQueueSize)) {
        vkuGetLayerSettingValue(layerSettingSet, kSettingQueueSize, maxScreenshotQueueSize);
        assert(maxScreenshotQueueSize > 0);
    }

    if (vkuHasLayerSetting(layerSettingSet, kSettingAllowSkip)) {
        vkuGetLayerSettingValue(layerSettingSet, kSettingAllowSkip, allowToSkipFrames);
    }

    if (vkuHasLayerSetting(layerSettingSet, kSettingProfile)) {
        vkuGetLayerSettingValue(layerSettingSet, kSettingProfile, isProfilingEnabled);
    }

    if (vkuHasLayerSetting(layerSettingSet, kSettingScreenshotExtension)) {
        std::string value;
        vkuGetLayerSettingValue(layerSettingSet, kSettingScreenshotExtension, value);
        std::transform(value.begin(), value.end(), value.begin(), [](char c) { return std::toupper(c); });
        screenshotExtension = ScreenshotExtension::PPM;
        if (value == "PAM") {
            screenshotExtension = ScreenshotExtension::PAM;
        } else if (value != "PPM") {
#ifdef ANDROID
            __android_log_print(ANDROID_LOG_ERROR, "screenshot",
                                "Selected screenshot extension:%s\nIs NOT in the list:\nPAM, PPM"
                                "\nPPM will be used instead",
                                value.c_str());
#else
            fprintf(stderr,
                    "screenshot: Selected screenshot extension:%s\nIs NOT in the list:\nPAM, PPM"
                    "\nPPM will be used instead",
                    value.c_str());
#endif
        }
    }

    if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyFrames)) {
        std::string value;
        vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyFrames, value);
        populate_frame_list(value.c_str());
    }

    if (vkuHasLayerSetting(layerSettingSet, kSettingKeyFormat)) {
        std::string value;
        vkuGetLayerSettingValue(layerSettingSet, kSettingKeyFormat, value);

        if (value == "UNORM") {
            userColorSpaceFormat = ColorSpaceFormat::UNORM;
        } else if (value == "SRGB") {
            userColorSpaceFormat = ColorSpaceFormat::SRGB;
        } else if (value == "SNORM") {
            userColorSpaceFormat = ColorSpaceFormat::SNORM;
        } else if (value == "USCALED") {
            userColorSpaceFormat = ColorSpaceFormat::USCALED;
        } else if (value == "SSCALED") {
            userColorSpaceFormat = ColorSpaceFormat::SSCALED;
        } else if (value == "UINT") {
            userColorSpaceFormat = ColorSpaceFormat::UINT;
        } else if (value == "SINT") {
            userColorSpaceFormat = ColorSpaceFormat::SINT;
        } else if (value != "USE_SWAPCHAIN_COLORSPACE") {
#ifdef ANDROID
            __android_log_print(ANDROID_LOG_ERROR, "screenshot",
                                "Selected format:%s\nIs NOT in the list:\nUNORM, SNORM, USCALED, SSCALED, UINT, SINT, "
                                "SRGB\nSwapchain Colorspace will be used instead\n",
                                value.c_str());
#else
            fprintf(stderr,
                    "screenshot: Selected format:%s\nIs NOT in the list:\nUNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB\n"
                    "Swapchain Colorspace will be used instead\n",
                    value.c_str());
#endif
        }
    }

    if (vkuHasLayerSetting(layerSettingSet, kSettingKeyDir)) {
        vkuGetLayerSettingValue(layerSettingSet, kSettingKeyDir, targetFolder);
    }
#ifdef ANDROID
    if (targetFolder.empty()) {
        targetFolder = "/sdcard/Android";
    }
#endif
}

// Get maximum frame number of the frame range
// FrameRange* pFrameRange, the specified frame rang
// return:
//  maximum frame number of the frame range,
//  if it's unlimited range, the return will be SCREEN_SHOT_FRAMES_UNLIMITED
int getEndFrameOfRange(const FrameRange *pFrameRange) {
    int endFrameOfRange = SCREEN_SHOT_FRAMES_UNLIMITED;
    if (pFrameRange->count != SCREEN_SHOT_FRAMES_UNLIMITED) {
        endFrameOfRange = pFrameRange->startFrame + (pFrameRange->count - 1) * pFrameRange->interval;
    }
    return endFrameOfRange;
}

bool Settings::isFrameToCapture(int frame) const {
    if (screenShotFrameRange.valid) {
        if (frame < screenShotFrameRange.startFrame) return false;
        if ((frame - screenShotFrameRange.startFrame) % screenShotFrameRange.interval > 0) return false;
        int rangeLastFrame = getEndFrameOfRange(&screenShotFrameRange);
        if (rangeLastFrame != SCREEN_SHOT_FRAMES_UNLIMITED && frame > rangeLastFrame) return false;
        return true;
    }
    return screenshotFrames.find(frame) != screenshotFrames.end();
}

bool Settings::isFrameAfterEndOfCaptureRange(int frame) const {
    if (screenShotFrameRange.valid) {
        int rangeLastFrame = getEndFrameOfRange(&screenShotFrameRange);
        if (rangeLastFrame == SCREEN_SHOT_FRAMES_UNLIMITED) return false;
        return frame > rangeLastFrame;
    }
    return screenshotFrames.empty() || frame > *std::prev(screenshotFrames.end());
}

void Settings::populate_frame_list(const char *vk_screenshot_frames) {
    string spec(vk_screenshot_frames), word;
    size_t start = 0, comma = 0;

    if (!isOptionBelongToScreenShotRange(vk_screenshot_frames)) {
        while (start < spec.size()) {
            int frameToAdd;
            comma = spec.find(',', start);
            if (comma == string::npos)
                word = string(spec, start);
            else
                word = string(spec, start, comma - start);
            frameToAdd = atoi(word.c_str());
            // Add the frame number to set, but only do it if the word
            // started with a digit and if
            // it's not already in the list
            if (*(word.c_str()) >= '0' && *(word.c_str()) <= '9') {
                screenshotFrames.insert(frameToAdd);
            }
            if (comma == string::npos) break;
            start = comma + 1;
        }
    } else {
        int parsingStatus = initScreenShotFrameRange(vk_screenshot_frames, &screenShotFrameRange);
        if (parsingStatus != 0) {
#ifdef ANDROID
            __android_log_print(ANDROID_LOG_ERROR, "screenshot", "Range error\n");
#else
            fprintf(stderr, "screenshot: Range error\n");
#endif
        }
    }
}

Settings settings;

#ifdef ANDROID
class ATrace {
   public:
    ATrace(const char *block) {
        if (settings.isProfilingEnabled) ATrace_beginSection(block);
    }

    ~ATrace() {
        if (settings.isProfilingEnabled) ATrace_endSection();
    }
};

#define PROFILE(name) ATrace atrace_scope_##__LINE__(name);
#define PROFILE_COUNTER(name, value) \
    if (settings.isProfilingEnabled) ATrace_setCounter(name, value);
#else
#define PROFILE(name)
#define PROFILE_COUNTER(name, value)
#endif

static bool memory_type_from_properties(VkPhysicalDeviceMemoryProperties *memory_properties, uint32_t typeBits,
                                        VkFlags requirements_mask, uint32_t *typeIndex) {
    // Search memtypes to find first index with those properties
    for (uint32_t i = 0; i < 32; i++) {
        if ((typeBits & 1) == 1) {
            // Type is available, does it match user properties?
            if ((memory_properties->memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    // No memory types matched, return failure
    return false;
}

static DispatchMapStruct *get_dispatch_info(VkDevice dev) {
    auto it = dispatchMap.find(dev);
    if (it == dispatchMap.end())
        return NULL;
    else
        return it->second;
}

static DeviceMapStruct *get_device_info(VkDevice dev) {
    auto it = deviceMap.find(dev);
    if (it == deviceMap.end())
        return NULL;
    else
        return it->second;
}

static void init_screenshot(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator) {
    vkuCreateLayerSettingSet("VK_LAYER_LUNARG_screenshot", vkuFindLayerSettingsCreateInfo(pCreateInfo), pAllocator, nullptr,
                             &globalLayerSettingSet);

    settings.init(globalLayerSettingSet);
}

void screenshotWriterThreadFunc();

void startScreenshotThread() {
    if (screenshotThreadStarted || shutdownScreenshotThread) return;
    screenshotThreadStarted = true;
    screenshotWriterThread = std::thread(screenshotWriterThreadFunc);
}

void stopScreenshotThread() { shutdownScreenshotThread = true; }

void waitScreenshotThreadIsOver() {
    if (!screenshotThreadStarted) return;
    if (screenshotWriterThread.joinable()) {
        screenshotWriterThread.join();
    }
}

VkQueue getQueueForScreenshot(VkDevice device) {
    // Find a queue that we can use for taking a screenshot
    VkQueue queue = VK_NULL_HANDLE;
    uint32_t count;
    VkBool32 graphicsCapable = VK_FALSE;
    VkBool32 presentCapable = VK_FALSE;
    VkuInstanceDispatchTable *pInstanceTable;
    DeviceMapStruct *devMap = get_device_info(device);
    if (NULL == devMap) {
        assert(0);
        return queue;
    }

    pInstanceTable = instance_dispatch_table(physDeviceMap[devMap->physicalDevice]->instance);
    assert(pInstanceTable);
    pInstanceTable->GetPhysicalDeviceQueueFamilyProperties(devMap->physicalDevice, &count, NULL);

    std::vector<VkQueueFamilyProperties> queueProps(count);

#if defined(__ANDROID__)
    // On Android, all physical devices and queue families must be capable of presentation with any native window
    presentCapable = VK_TRUE;
#endif

    if (queueProps.size() > 0) {
        pInstanceTable->GetPhysicalDeviceQueueFamilyProperties(devMap->physicalDevice, &count, queueProps.data());

        // Iterate over all queues for this device, searching for a queue that is graphics and present capable
        for (auto it = deviceMap[device]->queues.begin(); it != deviceMap[device]->queues.end(); it++) {
            queue = *it;
            graphicsCapable = ((queueProps[deviceMap[device]->queueIndexMap[queue]].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0);
#if defined(_WIN32)
            presentCapable =
                instance_dispatch_table(devMap->physicalDevice)
                    ->GetPhysicalDeviceWin32PresentationSupportKHR(devMap->physicalDevice, deviceMap[device]->queueIndexMap[queue]);
#elif not defined(__ANDROID__)
            // Everything else not Windows or Android
            // TODO: Make a function call to get present support from vkGetPhysicalDeviceXlibPresentationSupportKHR,
            // vkGetPhysicalDeviceXcbPresentationSupportKHR, etc
            presentCapable = graphicsCapable;
#endif
            if (graphicsCapable && presentCapable) break;
        }
    }
    return queue;
}

// Writes image to a PAM file.
bool writePAM(const char *filename, const char *pixels, uint32_t width, uint32_t height, uint32_t numChannels, uint32_t rowPitch) {
    PROFILE("writePAM");
    std::ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file << "P7\n";
    file << "WIDTH " << width << "\n";
    file << "HEIGHT " << height << "\n";
    file << "DEPTH " << numChannels << "\n";
    file << "MAXVAL " << 255 << "\n";
    file << "TUPLTYPE " << (numChannels == 3 ? "RGB" : "RGB_ALPHA") << "\n";
    file << "ENDHDR\n";

    if (numChannels * width == rowPitch) {
        file.write(pixels, height * rowPitch);
    } else {
        for (uint32_t y = 0; y < height; y++) {
            file.write(pixels, numChannels * width);
            pixels += rowPitch;
        }
    }
    file.close();
    return true;
}

// Writes image to a PPM file.
bool writePPM(const char *filename, const char *pixels, uint32_t width, uint32_t height, uint32_t numChannels, uint32_t rowPitch) {
    PROFILE("writePPM");

    std::ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file << "P6\n";
    file << width << "\n";
    file << height << "\n";
    file << 255 << "\n";

    if (3 == numChannels) {
        for (uint32_t y = 0; y < height; y++) {
            file.write(pixels, 3 * width);
            pixels += rowPitch;
        }
    } else if (4 == numChannels) {
        std::vector<unsigned char> tempRowBuffer;
        tempRowBuffer.resize(3 * width + 1);
        for (uint32_t y = 0; y < height; y++) {
            const uint32_t *srcRow = reinterpret_cast<const uint32_t *>(pixels);
            unsigned char *destRow = tempRowBuffer.data();

            for (uint32_t x = 0; x < width; x++) {
                *reinterpret_cast<uint32_t *>(destRow) = *srcRow++;
                destRow += 3;
            }
            file.write(reinterpret_cast<const char *>(tempRowBuffer.data()), 3 * width);
            pixels += rowPitch;
        }
    }
    file.close();
    return true;
}

static VkFormat determineOutputFormat(VkFormat format, ColorSpaceFormat userColorSpaceFormat, uint32_t numChannels) {
    // Initial dest format is undefined as we will look for one
    VkFormat destformat = VK_FORMAT_UNDEFINED;

    // This variable set by readScreenShotFormatENV func during init
    if (userColorSpaceFormat != ColorSpaceFormat::UNDEFINED) {
        switch (userColorSpaceFormat) {
            case ColorSpaceFormat::UNORM:
                if (numChannels == 4)
                    destformat = VK_FORMAT_R8G8B8A8_UNORM;
                else
                    destformat = VK_FORMAT_R8G8B8_UNORM;
                break;
            case ColorSpaceFormat::SRGB:
                if (numChannels == 4)
                    destformat = VK_FORMAT_R8G8B8A8_SRGB;
                else
                    destformat = VK_FORMAT_R8G8B8_SRGB;
                break;
            case ColorSpaceFormat::SNORM:
                if (numChannels == 4)
                    destformat = VK_FORMAT_R8G8B8A8_SNORM;
                else
                    destformat = VK_FORMAT_R8G8B8_SNORM;
                break;
            case ColorSpaceFormat::USCALED:
                if (numChannels == 4)
                    destformat = VK_FORMAT_R8G8B8A8_USCALED;
                else
                    destformat = VK_FORMAT_R8G8B8_USCALED;
                break;
            case ColorSpaceFormat::SSCALED:
                if (numChannels == 4)
                    destformat = VK_FORMAT_R8G8B8A8_SSCALED;
                else
                    destformat = VK_FORMAT_R8G8B8_SSCALED;
                break;
            case ColorSpaceFormat::UINT:
                if (numChannels == 4)
                    destformat = VK_FORMAT_R8G8B8A8_UINT;
                else
                    destformat = VK_FORMAT_R8G8B8_UINT;
                break;
            case ColorSpaceFormat::SINT:
                if (numChannels == 4)
                    destformat = VK_FORMAT_R8G8B8A8_SINT;
                else
                    destformat = VK_FORMAT_R8G8B8_SINT;
                break;
            default:
                destformat = VK_FORMAT_UNDEFINED;
                break;
        }
    }

    // User did not require specific format so we use same colorspace with
    // swapchain format
    if (destformat == VK_FORMAT_UNDEFINED) {
        // Here we reserve swapchain color space only as RGBA swizzle will be later.
        //
        // One Potential optimization here would be: set destination to RGB all the
        // time instead RGBA. PPM does not support Alpha channel, so we can write
        // RGB one row by row but RGBA written one pixel at a time.
        // This requires BLIT operation to get involved but current drivers (mostly)
        // does not support BLIT operations on 3 Channel render targets.
        // So format conversion gets costly.
        if (numChannels == 4) {
            if (vkuFormatIsUNORM(format))
                destformat = VK_FORMAT_R8G8B8A8_UNORM;
            else if (vkuFormatIsSRGB(format))
                destformat = VK_FORMAT_R8G8B8A8_SRGB;
            else if (vkuFormatIsSNORM(format))
                destformat = VK_FORMAT_R8G8B8A8_SNORM;
            else if (vkuFormatIsUSCALED(format))
                destformat = VK_FORMAT_R8G8B8A8_USCALED;
            else if (vkuFormatIsSSCALED(format))
                destformat = VK_FORMAT_R8G8B8A8_SSCALED;
            else if (vkuFormatIsUINT(format))
                destformat = VK_FORMAT_R8G8B8A8_UINT;
            else if (vkuFormatIsSINT(format))
                destformat = VK_FORMAT_R8G8B8A8_SINT;
        } else {  // numChannels 3
            if (vkuFormatIsUNORM(format))
                destformat = VK_FORMAT_R8G8B8_UNORM;
            else if (vkuFormatIsSRGB(format))
                destformat = VK_FORMAT_R8G8B8_SRGB;
            else if (vkuFormatIsSNORM(format))
                destformat = VK_FORMAT_R8G8B8_SNORM;
            else if (vkuFormatIsUSCALED(format))
                destformat = VK_FORMAT_R8G8B8_USCALED;
            else if (vkuFormatIsSSCALED(format))
                destformat = VK_FORMAT_R8G8B8_SSCALED;
            else if (vkuFormatIsUINT(format))
                destformat = VK_FORMAT_R8G8B8_UINT;
            else if (vkuFormatIsSINT(format))
                destformat = VK_FORMAT_R8G8B8_SINT;
        }
    }

    // Still could not find the right format then we use UNORM
    if (destformat == VK_FORMAT_UNDEFINED) {
        static bool alreadyPrintFormatWarning = false;
        if (!alreadyPrintFormatWarning) {
#ifdef ANDROID
            __android_log_print(ANDROID_LOG_INFO, "screenshot",
                                "Swapchain format is not in the list:\nUNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB\n");
#else
            fprintf(stderr,
                    "screenshot: Swapchain format is not in the list:\nUNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB\n"
                    "UNORM colorspace will be used instead\n");
#endif
            alreadyPrintFormatWarning = true;
        }
        if (numChannels == 4)
            destformat = VK_FORMAT_R8G8B8A8_UNORM;
        else
            destformat = VK_FORMAT_R8G8B8_UNORM;
    }

    // From vulkan spec:
    //   VUID-vkCmdBlitImage-srcImage-00229
    //     If either of srcImage or dstImage was created with a signed integer VkFormat,
    //     the other must also have been created with a signed integer VkFormat
    //   VUID-vkCmdBlitImage-srcImage-00230
    //     If either of srcImage or dstImage was created with an unsigned integer VkFormat,
    //     the other must also have been created with an unsigned integer VkFormat
    // If the destination format is not compatible, set destintation format to source format and print a warning.
    // Yes, the expression in the if stmt is correct. It makes sure that the correct signed/unsigned formats
    // are used for destformat and format.
    if (vkuFormatIsSINT(format) || vkuFormatIsSINT(destformat) || vkuFormatIsUINT(format) || vkuFormatIsUINT(destformat)) {
        // Print a warning if we need to change destformat
        if (destformat != format) {
            // format might be BGRA instead of RGBA, so just choose the RGBA equivalent of format
            if (numChannels == 4) {
                if (vkuFormatIsUNORM(format))
                    destformat = VK_FORMAT_R8G8B8A8_UNORM;
                else if (vkuFormatIsSRGB(format))
                    destformat = VK_FORMAT_R8G8B8A8_SRGB;
                else if (vkuFormatIsSNORM(format))
                    destformat = VK_FORMAT_R8G8B8A8_SNORM;
                else if (vkuFormatIsUSCALED(format))
                    destformat = VK_FORMAT_R8G8B8A8_USCALED;
                else if (vkuFormatIsSSCALED(format))
                    destformat = VK_FORMAT_R8G8B8A8_SSCALED;
                else
                    // Conversion failed to find a suitable format, fallback on swapchain format
                    destformat = format;
            } else {  // numChannels 3
                if (vkuFormatIsUNORM(format))
                    destformat = VK_FORMAT_R8G8B8_UNORM;
                else if (vkuFormatIsSRGB(format))
                    destformat = VK_FORMAT_R8G8B8_SRGB;
                else if (vkuFormatIsSNORM(format))
                    destformat = VK_FORMAT_R8G8B8_SNORM;
                else if (vkuFormatIsUSCALED(format))
                    destformat = VK_FORMAT_R8G8B8_USCALED;
                else if (vkuFormatIsSSCALED(format))
                    destformat = VK_FORMAT_R8G8B8_SSCALED;
                else
                    // Conversion failed to find a suitable format, fallback on swapchain format
                    destformat = format;
            }

#ifdef ANDROID
            __android_log_print(ANDROID_LOG_INFO, "screenshot",
                                "Incompatible output format requested, changing output format to %s\n",
                                string_VkFormat(destformat));
#else
            fprintf(stderr, "screenshot: Incompatible output format requested, changing output format to %s\n",
                    string_VkFormat(destformat));
#endif
        }
    }
    return destformat;
}

// Contains data required for frame's screenshot
struct ScreenshotQueueData {
    uint32_t frameNumber;
    VkDevice device;
    VkImage image1;  // source image
    VkuDeviceDispatchTable *pTableDevice;
    uint32_t dstWidth;
    uint32_t dstHeight;
    int dstNumChannels;

    // Below is data to clean up in destructor
    VkImage image2;
    VkImage image3;
    VkDeviceMemory mem2;
    VkDeviceMemory mem3;
    VkCommandBuffer commandBuffer;
    VkCommandPool commandPool;
    VkSemaphore semaphore;
    VkFence fence;
    ~ScreenshotQueueData();
};

ScreenshotQueueData::~ScreenshotQueueData() {
    PROFILE("~ScreenshotQueueData");
    if (mem2) pTableDevice->FreeMemory(device, mem2, NULL);
    if (image2) pTableDevice->DestroyImage(device, image2, NULL);

    if (mem3) pTableDevice->FreeMemory(device, mem3, NULL);
    if (image3) pTableDevice->DestroyImage(device, image3, NULL);

    if (commandBuffer) pTableDevice->FreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    if (commandPool) pTableDevice->DestroyCommandPool(device, commandPool, NULL);
    if (semaphore) pTableDevice->DestroySemaphore(device, semaphore, NULL);
    if (fence) pTableDevice->DestroyFence(device, fence, NULL);
}

std::list<std::shared_ptr<ScreenshotQueueData>> screenshotsData;
std::unordered_map<VkImage, std::list<std::shared_ptr<ScreenshotQueueData>>> screenshotDataCache;

bool prepareScreenshotData(ScreenshotQueueData &data, VkImage image1) {
    PROFILE("screenshot.prepare");
    VkResult err;
    bool pass;

    // Bail immediately if we can't find the image.
    if (imageMap.empty() || imageMap.find(image1) == imageMap.end()) return false;

    // Collect object info from maps.  This info is generally recorded
    // by the other functions hooked in this layer.
    VkDevice device = imageMap[image1]->device;
    VkPhysicalDevice physicalDevice = deviceMap[device]->physicalDevice;
    VkInstance instance = physDeviceMap[physicalDevice]->instance;
    DispatchMapStruct *dispMap = get_dispatch_info(device);
    if (NULL == dispMap) {
        assert(0);
        return false;
    }
    VkQueue queue = getQueueForScreenshot(device);
    if (!queue) {
#ifdef ANDROID
        __android_log_print(ANDROID_LOG_ERROR, "screenshot", "Failure - capable queue not found\n");
#else
        fprintf(stderr, "screenshot: Could not find a capable queue\n");
#endif
        return false;
    }
    VkuDeviceDispatchTable *pTableDevice = dispMap->device_dispatch_table;
    VkuDeviceDispatchTable *pTableQueue =
        get_dispatch_info(static_cast<VkDevice>(static_cast<void *>(queue)))->device_dispatch_table;
    VkuInstanceDispatchTable *pInstanceTable;
    pInstanceTable = instance_dispatch_table(instance);

    // Gather incoming image info and check image format for compatibility with
    // the target format.
    // This function supports both 24-bit and 32-bit swapchain images.
    uint32_t const width = imageMap[image1]->imageExtent.width;
    uint32_t const height = imageMap[image1]->imageExtent.height;
    VkFormat const format = imageMap[image1]->format;
    uint32_t const numChannels = vkuFormatComponentCount(format);

    if ((3 != numChannels) && (4 != numChannels)) {
        assert(0);
        return false;
    }

    // userColorSpaceFormat set by readScreenShotFormatENV func during init
    VkFormat destformat = determineOutputFormat(format, settings.userColorSpaceFormat, numChannels);

    if ((vkuFormatCompatibilityClass(destformat) != vkuFormatCompatibilityClass(format))) {
        assert(0);
        return false;
    }

    data.image1 = image1;
    data.dstWidth = width * settings.scalePercent / 100;
    data.dstHeight = height * settings.scalePercent / 100;
    data.dstNumChannels = vkuFormatComponentCount(destformat);
    data.device = device;
    data.pTableDevice = pTableDevice;

    // General Approach
    //
    // The idea here is to copy/convert the swapchain image into another image
    // that can be mapped and read by the CPU to produce a PPM file.
    // The image must be untiled and converted to a specific format for easy
    // parsing.  The memory for the final image must be host-visible.
    // Note that in Vulkan, a BLIT operation must be used to perform a format
    // conversion.
    //
    // Devices vary in their ability to blit to/from linear and optimal tiling.
    // So we must query the device properties to get this information.
    //
    // If the device cannot BLIT to a LINEAR image, then the operation must be
    // done in two steps:
    // 1) BLIT the swapchain image (image1) to a temp image (image2) that is
    // created with TILING_OPTIMAL.
    // 2) COPY image2 to another temp image (image3) that is created with
    // TILING_LINEAR.
    // 3) Map image 3 and write the PPM file.
    //
    // If the device can BLIT to a LINEAR image, then:
    // 1) BLIT the swapchain image (image1) to a temp image (image2) that is
    // created with TILING_LINEAR.
    // 2) Map image 2 and write the PPM file.
    //
    // There seems to be no way to tell if the swapchain image (image1) is tiled
    // or not.  We therefore assume that the BLIT operation can always read from
    // both linear and optimal tiled (swapchain) images.
    // There is therefore no point in looking at the BLIT_SRC properties.
    //
    // There is also the optimization where the incoming and target formats are
    // the same.  In this case, just do a COPY.

    VkFormatProperties targetFormatProps;
    pInstanceTable->GetPhysicalDeviceFormatProperties(physicalDevice, destformat, &targetFormatProps);
    bool need2steps = false;
    bool copyOnly = false;
    if (destformat == format && width == data.dstWidth && height == data.dstHeight) {
        copyOnly = true;
    } else {
        bool const bltLinear = targetFormatProps.linearTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT;
        bool const bltOptimal = targetFormatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT;
        if (!bltLinear && !bltOptimal) {
            // Cannot blit to either target tiling type.  It should be pretty
            // unlikely to have a device that cannot blit to either type.
            // This should be quite rare. Punt.
#ifdef ANDROID
            __android_log_print(ANDROID_LOG_DEBUG, "screenshot", "Output format not supported, screen capture failed");
#else
            fprintf(stderr, "screenshot: Output format not supported, screen capture failed\n");
#endif
            return false;
        } else if (!bltLinear && bltOptimal) {
            // Cannot blit to a linear target but can blt to optimal, so copy
            // after blit is needed.
            need2steps = true;
        }
        // Else bltLinear is available and only 1 step is needed.
    }

    // Set up the image creation info for both the blit and copy images, in case
    // both are needed.
    VkImageCreateInfo imgCreateInfo2 = {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        NULL,
        0,
        VK_IMAGE_TYPE_2D,
        destformat,
        {data.dstWidth, data.dstHeight, 1},
        1,
        1,
        VK_SAMPLE_COUNT_1_BIT,
        VK_IMAGE_TILING_LINEAR,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        NULL,
        VK_IMAGE_LAYOUT_UNDEFINED,
    };
    VkImageCreateInfo imgCreateInfo3 = imgCreateInfo2;

    // If we need both images, set up image2 to be read/write and tiled.
    if (need2steps) {
        imgCreateInfo2.tiling = VK_IMAGE_TILING_OPTIMAL;
        imgCreateInfo2.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }

    VkMemoryAllocateInfo memAllocInfo = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, NULL,
        0,  // allocationSize, queried later
        0   // memoryTypeIndex, queried later
    };
    VkMemoryRequirements memRequirements;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    // Create image2 and allocate its memory.  It could be the intermediate or
    // final image.
    err = pTableDevice->CreateImage(device, &imgCreateInfo2, NULL, &data.image2);
    assert(!err);
    if (VK_SUCCESS != err) return false;
    pTableDevice->GetImageMemoryRequirements(device, data.image2, &memRequirements);
    memAllocInfo.allocationSize = memRequirements.size;
    pInstanceTable->GetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
    pass = memory_type_from_properties(&memoryProperties, memRequirements.memoryTypeBits,
                                       need2steps ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                                       &memAllocInfo.memoryTypeIndex);
    assert(pass);
    (void)pass;
    err = pTableDevice->AllocateMemory(device, &memAllocInfo, NULL, &data.mem2);
    assert(!err);
    if (VK_SUCCESS != err) return false;
    err = pTableQueue->BindImageMemory(device, data.image2, data.mem2, 0);
    assert(!err);
    if (VK_SUCCESS != err) return false;

    // Create image3 and allocate its memory, if needed.
    if (need2steps) {
        err = pTableDevice->CreateImage(device, &imgCreateInfo3, NULL, &data.image3);
        assert(!err);
        if (VK_SUCCESS != err) return false;
        pTableDevice->GetImageMemoryRequirements(device, data.image3, &memRequirements);
        memAllocInfo.allocationSize = memRequirements.size;
        pInstanceTable->GetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
        pass = memory_type_from_properties(&memoryProperties, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                                           &memAllocInfo.memoryTypeIndex);
        assert(pass);
        (void)pass;
        err = pTableDevice->AllocateMemory(device, &memAllocInfo, NULL, &data.mem3);
        assert(!err);
        if (VK_SUCCESS != err) return false;
        err = pTableQueue->BindImageMemory(device, data.image3, data.mem3, 0);
        assert(!err);
        if (VK_SUCCESS != err) return false;
    }

    // We want to create our own command pool to be sure we can use it from this thread
    VkCommandPoolCreateInfo cmd_pool_info = {};
    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_info.pNext = NULL;
    auto it = deviceMap[device]->queueIndexMap.find(queue);
    assert(it != deviceMap[device]->queueIndexMap.end());
    cmd_pool_info.queueFamilyIndex = it->second;
    cmd_pool_info.flags = 0;

    err = pTableDevice->CreateCommandPool(device, &cmd_pool_info, NULL, &data.commandPool);
    assert(!err);

    // Set up the command buffer.
    const VkCommandBufferAllocateInfo allocCommandBufferInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, NULL,
                                                                data.commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1};
    err = pTableDevice->AllocateCommandBuffers(device, &allocCommandBufferInfo, &data.commandBuffer);
    assert(!err);
    if (VK_SUCCESS != err) return false;

    VkDevice cmdBuf = static_cast<VkDevice>(static_cast<void *>(data.commandBuffer));
    if (deviceMap.find(cmdBuf) != deviceMap.end()) {
        // Remove element with key cmdBuf from deviceMap so we can replace it
        deviceMap.erase(cmdBuf);
    }
    dispatchMap.emplace(cmdBuf, dispMap);
    VkuDeviceDispatchTable *pTableCommandBuffer;
    pTableCommandBuffer = get_dispatch_info(cmdBuf)->device_dispatch_table;

    // We have just created a dispatchable object, but the dispatch table has
    // not been placed in the object yet.  When a "normal" application creates
    // a command buffer, the dispatch table is installed by the top-level api
    // binding (trampoline.c). But here, we have to do it ourselves.
    if (!dispMap->pfn_dev_init) {
        *((const void **)data.commandBuffer) = *(void **)device;
    } else {
        err = dispMap->pfn_dev_init(device, (void *)data.commandBuffer);
        assert(!err);
    }

    const VkCommandBufferBeginInfo commandBufferBeginInfo = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        NULL,
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    err = pTableCommandBuffer->BeginCommandBuffer(data.commandBuffer, &commandBufferBeginInfo);
    assert(!err);

    // This barrier is used to transition from/to present Layout
    VkImageMemoryBarrier presentMemoryBarrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                                                 NULL,
                                                 VK_ACCESS_MEMORY_WRITE_BIT,
                                                 VK_ACCESS_TRANSFER_READ_BIT,
                                                 VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                                 VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                 VK_QUEUE_FAMILY_IGNORED,
                                                 VK_QUEUE_FAMILY_IGNORED,
                                                 image1,
                                                 {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}};

    // This barrier is used to transition from a newly-created layout to a blt
    // or copy destination layout.
    VkImageMemoryBarrier destMemoryBarrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                                              NULL,
                                              0,
                                              VK_ACCESS_TRANSFER_WRITE_BIT,
                                              VK_IMAGE_LAYOUT_UNDEFINED,
                                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                              VK_QUEUE_FAMILY_IGNORED,
                                              VK_QUEUE_FAMILY_IGNORED,
                                              data.image2,
                                              {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}};

    // This barrier is used to transition a dest layout to general layout.
    VkImageMemoryBarrier generalMemoryBarrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                                                 NULL,
                                                 VK_ACCESS_TRANSFER_WRITE_BIT,
                                                 VK_ACCESS_MEMORY_READ_BIT,
                                                 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                 VK_IMAGE_LAYOUT_GENERAL,
                                                 VK_QUEUE_FAMILY_IGNORED,
                                                 VK_QUEUE_FAMILY_IGNORED,
                                                 data.image2,
                                                 {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}};

    VkPipelineStageFlags srcStages = VK_PIPELINE_STAGE_TRANSFER_BIT;
    VkPipelineStageFlags dstStages = VK_PIPELINE_STAGE_TRANSFER_BIT;

    // The source image needs to be transitioned from present to transfer
    // source.
    pTableCommandBuffer->CmdPipelineBarrier(data.commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, dstStages, 0, 0, NULL, 0,
                                            NULL, 1, &presentMemoryBarrier);

    // image2 needs to be transitioned from its undefined state to transfer
    // destination.
    pTableCommandBuffer->CmdPipelineBarrier(data.commandBuffer, srcStages, dstStages, 0, 0, NULL, 0, NULL, 1, &destMemoryBarrier);

    if (copyOnly) {
        const VkImageCopy imageCopyRegion = {
            {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1}, {0, 0, 0}, {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1}, {0, 0, 0}, {width, height, 1}};
        pTableCommandBuffer->CmdCopyImage(data.commandBuffer, image1, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, data.image2,
                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);
    } else {
        VkImageBlit imageBlitRegion = {};
        imageBlitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageBlitRegion.srcSubresource.baseArrayLayer = 0;
        imageBlitRegion.srcSubresource.layerCount = 1;
        imageBlitRegion.srcSubresource.mipLevel = 0;
        imageBlitRegion.srcOffsets[1].x = width;
        imageBlitRegion.srcOffsets[1].y = height;
        imageBlitRegion.srcOffsets[1].z = 1;
        imageBlitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageBlitRegion.dstSubresource.baseArrayLayer = 0;
        imageBlitRegion.dstSubresource.layerCount = 1;
        imageBlitRegion.dstSubresource.mipLevel = 0;
        imageBlitRegion.dstOffsets[1].x = data.dstWidth;
        imageBlitRegion.dstOffsets[1].y = data.dstHeight;
        imageBlitRegion.dstOffsets[1].z = 1;

        pTableCommandBuffer->CmdBlitImage(data.commandBuffer, image1, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, data.image2,
                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlitRegion, VK_FILTER_NEAREST);
        if (need2steps) {
            // image 3 needs to be transitioned from its undefined state to a
            // transfer destination.
            destMemoryBarrier.image = data.image3;
            pTableCommandBuffer->CmdPipelineBarrier(data.commandBuffer, srcStages, dstStages, 0, 0, NULL, 0, NULL, 1,
                                                    &destMemoryBarrier);

            // Transition image2 so that it can be read for the upcoming copy to
            // image 3.
            destMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            destMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            destMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            destMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            destMemoryBarrier.image = data.image2;
            pTableCommandBuffer->CmdPipelineBarrier(data.commandBuffer, srcStages, dstStages, 0, 0, NULL, 0, NULL, 1,
                                                    &destMemoryBarrier);

            // This step essentially untiles the image.
            const VkImageCopy imageCopyRegion = {{VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1},
                                                 {0, 0, 0},
                                                 {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1},
                                                 {0, 0, 0},
                                                 {data.dstWidth, data.dstHeight, 1}};
            pTableCommandBuffer->CmdCopyImage(data.commandBuffer, data.image2, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, data.image3,
                                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);
            generalMemoryBarrier.image = data.image3;
        }
    }

    // The destination needs to be transitioned from the optimal copy format to
    // the format we can read with the CPU.
    pTableCommandBuffer->CmdPipelineBarrier(data.commandBuffer, srcStages, dstStages, 0, 0, NULL, 0, NULL, 1,
                                            &generalMemoryBarrier);

    // Restore the swap chain image layout to what it was before.
    // This may not be strictly needed, but it is generally good to restore
    // things to original state.
    presentMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    presentMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    presentMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    presentMemoryBarrier.dstAccessMask = 0;
    pTableCommandBuffer->CmdPipelineBarrier(data.commandBuffer, srcStages, dstStages, 0, 0, NULL, 0, NULL, 1,
                                            &presentMemoryBarrier);

    err = pTableCommandBuffer->EndCommandBuffer(data.commandBuffer);
    assert(!err);

    VkSemaphoreCreateInfo semaphoreInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    if (pTableDevice->CreateSemaphore(device, &semaphoreInfo, nullptr, &data.semaphore) != VK_SUCCESS) {
        return false;
    }

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    if (pTableDevice->CreateFence(device, &fenceInfo, nullptr, &data.fence) != VK_SUCCESS) {
        return false;
    }

    return true;
}

// This function issues commands to copy/convert the swapchain image
// from whatever compatible format the swapchain image uses
// to a single format (VK_FORMAT_R8G8B8A8_UNORM) so that the converted
// result can be easily written to a PPM file.
//
// Error handling: If there is a problem, this function should silently
// fail without affecting the Present operation going on in the caller.
// The numerous debug asserts are to catch programming errors and are not
// expected to assert.  Recovery and clean up are implemented for image memory
// allocation failures.
// (TODO) It would be nice to pass any failure info to DebugReport or something.
//
// Returns true if successfull, false otherwise.
static bool queueScreenshot(ScreenshotQueueData &data, VkImage image1, const VkPresentInfoKHR *presentInfo) {
    PROFILE("screenshot.queue");
    if (data.image1 == VK_NULL_HANDLE) {
        if (!prepareScreenshotData(data, image1)) {
            assert(false);
            return false;
        }
    } else {
        if (data.pTableDevice->ResetFences(data.device, 1, &data.fence) != VK_SUCCESS) {
            assert(false);
            return false;
        }
    }

    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &data.commandBuffer;
    VkPipelineStageFlags layerWaitStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    submitInfo.pWaitDstStageMask = &layerWaitStage;
    submitInfo.pWaitSemaphores = presentInfo->pWaitSemaphores;
    submitInfo.waitSemaphoreCount = presentInfo->waitSemaphoreCount;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &data.semaphore;

    VkQueue queue = getQueueForScreenshot(data.device);
    if (!queue) {
#ifdef ANDROID
        __android_log_print(ANDROID_LOG_ERROR, "screenshot", "Failure - capable queue not found\n");
#else
        fprintf(stderr, "screenshot: Could not find a capable queue\n");
#endif
        return false;
    }

    VkuDeviceDispatchTable *pTableQueue =
        get_dispatch_info(static_cast<VkDevice>(static_cast<void *>(queue)))->device_dispatch_table;

    VkResult err = pTableQueue->QueueSubmit(queue, 1, &submitInfo, data.fence);
    assert(!err);

    return true;
}

// Save an image to a PPM image file.
// Returns true if file is successfully written, false otherwise.
static bool writeScreenshot(ScreenshotQueueData &data) {
    PROFILE("screenshot.write");
    // Map the final image so that the CPU can read it.
    const VkImageSubresource sr = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0};
    VkSubresourceLayout srLayout;
    const char *pixels;
    if (data.image3 == VK_NULL_HANDLE) {
        data.pTableDevice->GetImageSubresourceLayout(data.device, data.image2, &sr, &srLayout);
        VkResult err = data.pTableDevice->MapMemory(data.device, data.mem2, 0, VK_WHOLE_SIZE, 0, (void **)&pixels);
        if (VK_SUCCESS != err) return false;
    } else {
        data.pTableDevice->GetImageSubresourceLayout(data.device, data.image3, &sr, &srLayout);
        VkResult err = data.pTableDevice->MapMemory(data.device, data.mem3, 0, VK_WHOLE_SIZE, 0, (void **)&pixels);
        if (VK_SUCCESS != err) return false;
    }

    pixels += srLayout.offset;

    string fileName;
    if (settings.targetFolder.empty()) {
        fileName = to_string(data.frameNumber);
    } else {
        fileName = settings.targetFolder;
        fileName += "/" + to_string(data.frameNumber);
    }

    bool writeResult;
    switch (settings.screenshotExtension) {
        case Settings::ScreenshotExtension::PPM:
            fileName += ".ppm";
            writeResult = writePPM(fileName.c_str(), pixels, data.dstWidth, data.dstHeight, data.dstNumChannels, srLayout.rowPitch);
            break;
        case Settings::ScreenshotExtension::PAM:
            fileName += ".pam";
            writeResult = writePAM(fileName.c_str(), pixels, data.dstWidth, data.dstHeight, data.dstNumChannels, srLayout.rowPitch);
            break;
    }

    if (data.image3 == VK_NULL_HANDLE) {
        data.pTableDevice->UnmapMemory(data.device, data.mem2);
    } else {
        data.pTableDevice->UnmapMemory(data.device, data.mem3);
    }

    if (!writeResult) {
#ifdef ANDROID
        __android_log_print(ANDROID_LOG_ERROR, "screenshot", "Failed to write image: %s", fileName.c_str());
#else
        fprintf(stderr, "screenshot: Failed to write image: %s\n", fileName.c_str());
#endif
        return false;
    }
#ifdef ANDROID
    __android_log_print(ANDROID_LOG_INFO, "screenshot", "Saved image: %s", fileName.c_str());
#else
    printf("screenshot: Saved image: %s \n", fileName.c_str());
    fflush(stdout);
#endif
    return true;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateInstance(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator,
                                              VkInstance *pInstance) {
#if defined(_WIN32) && defined(_CRTDBG_MODE_FILE)
#if !defined(NDEBUG)
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif
    // Avoid "Abort, Retry, Ignore" dialog boxes
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

    VkLayerInstanceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    assert(fpGetInstanceProcAddr);
    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance)fpGetInstanceProcAddr(VK_NULL_HANDLE, "vkCreateInstance");
    if (fpCreateInstance == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Advance the link info for the next element on the chain
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);
    if (result != VK_SUCCESS) return result;

    initInstanceTable(*pInstance, fpGetInstanceProcAddr);

    init_screenshot(pCreateInfo, pAllocator);

    return result;
}

// TODO hook DestroyInstance to cleanup

static void createDeviceRegisterExtensions(const VkDeviceCreateInfo *pCreateInfo, VkDevice device) {
    uint32_t i;
    DispatchMapStruct *dispMap = get_dispatch_info(device);
    DeviceMapStruct *devMap = get_device_info(device);
    VkuDeviceDispatchTable *pDisp = dispMap->device_dispatch_table;
    PFN_vkGetDeviceProcAddr gpa = pDisp->GetDeviceProcAddr;
    pDisp->CreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)gpa(device, "vkCreateSwapchainKHR");
    pDisp->GetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)gpa(device, "vkGetSwapchainImagesKHR");
    pDisp->AcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)gpa(device, "vkAcquireNextImageKHR");
    pDisp->QueuePresentKHR = (PFN_vkQueuePresentKHR)gpa(device, "vkQueuePresentKHR");
    devMap->wsi_enabled = false;
    for (i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
        if (strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) devMap->wsi_enabled = true;
    }
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDevice(VkPhysicalDevice gpu, const VkDeviceCreateInfo *pCreateInfo,
                                            const VkAllocationCallbacks *pAllocator, VkDevice *pDevice) {
    VkLayerDeviceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    VkInstance instance = physDeviceMap[gpu]->instance;
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice)fpGetInstanceProcAddr(instance, "vkCreateDevice");
    if (fpCreateDevice == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Advance the link info for the next element on the chain
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    VkResult result = fpCreateDevice(gpu, pCreateInfo, pAllocator, pDevice);
    if (result != VK_SUCCESS) {
        return result;
    }

    assert(deviceMap.find(*pDevice) == deviceMap.end());
    DeviceMapStruct *deviceMapElem = new DeviceMapStruct;
    deviceMap[*pDevice] = deviceMapElem;
    assert(dispatchMap.find(*pDevice) == dispatchMap.end());
    DispatchMapStruct *dispatchMapElem = new DispatchMapStruct;
    dispatchMap[*pDevice] = dispatchMapElem;

    // Setup device dispatch table
    dispatchMapElem->device_dispatch_table = new VkuDeviceDispatchTable;
    vkuInitDeviceDispatchTable(*pDevice, dispatchMapElem->device_dispatch_table, fpGetDeviceProcAddr);

    createDeviceRegisterExtensions(pCreateInfo, *pDevice);
    // Create a mapping from a device to a physicalDevice
    deviceMapElem->physicalDevice = gpu;

    // store the loader callback for initializing created dispatchable objects
    chain_info = get_chain_info(pCreateInfo, VK_LOADER_DATA_CALLBACK);
    if (chain_info) {
        dispatchMapElem->pfn_dev_init = chain_info->u.pfnSetDeviceLoaderData;
    } else {
        dispatchMapElem->pfn_dev_init = NULL;
    }
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL EnumeratePhysicalDevices(VkInstance instance, uint32_t *pPhysicalDeviceCount,
                                                        VkPhysicalDevice *pPhysicalDevices) {
    VkResult result;

    VkuInstanceDispatchTable *pTable = instance_dispatch_table(instance);
    result = pTable->EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
    if (result == VK_SUCCESS && *pPhysicalDeviceCount > 0 && pPhysicalDevices) {
        for (uint32_t i = 0; i < *pPhysicalDeviceCount; i++) {
            // Create a mapping from a physicalDevice to an instance
            if (physDeviceMap[pPhysicalDevices[i]] == NULL) {
                PhysDeviceMapStruct *physDeviceMapElem = new PhysDeviceMapStruct;
                physDeviceMap[pPhysicalDevices[i]] = physDeviceMapElem;
            }
            physDeviceMap[pPhysicalDevices[i]]->instance = instance;
        }
    }
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL EnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t *pPhysicalDeviceGroupCount,
                                                             VkPhysicalDeviceGroupProperties *pPhysicalDeviceGroupProperties) {
    VkResult result;
    VkuInstanceDispatchTable *pTable = instance_dispatch_table(instance);
    result = pTable->EnumeratePhysicalDeviceGroups(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
    if (result == VK_SUCCESS && *pPhysicalDeviceGroupCount > 0 && pPhysicalDeviceGroupProperties) {
        for (uint32_t i = 0; i < *pPhysicalDeviceGroupCount; i++) {
            for (uint32_t j = 0; j < pPhysicalDeviceGroupProperties[i].physicalDeviceCount; j++) {
                // Create a mapping from each physicalDevice to an instance
                if (physDeviceMap[pPhysicalDeviceGroupProperties[i].physicalDevices[j]] == NULL) {
                    PhysDeviceMapStruct *physDeviceMapElem = new PhysDeviceMapStruct;
                    physDeviceMap[pPhysicalDeviceGroupProperties[i].physicalDevices[j]] = physDeviceMapElem;
                }
                physDeviceMap[pPhysicalDeviceGroupProperties[i].physicalDevices[j]]->instance = instance;
            }
        }
    }
    return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL DestroyDevice(VkDevice device, const VkAllocationCallbacks *pAllocator) {
    DispatchMapStruct *dispMap = get_dispatch_info(device);
    DeviceMapStruct *devMap = get_device_info(device);
    assert(dispMap);
    assert(devMap);
    VkuDeviceDispatchTable *pDisp = dispMap->device_dispatch_table;
    pDisp->DestroyDevice(device, pAllocator);

    std::lock_guard<std::mutex> lg(globalLock);
    delete pDisp;
    delete dispMap;
    delete devMap;

    deviceMap.erase(device);
}

VKAPI_ATTR void VKAPI_CALL GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue *pQueue) {
    DispatchMapStruct *dispMap = get_dispatch_info(device);
    assert(dispMap);
    VkuDeviceDispatchTable *pDisp = dispMap->device_dispatch_table;
    pDisp->GetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);

    // Save the device queue in a map if we are taking screenshots.
    std::lock_guard<std::mutex> lg(globalLock);

    // Add this queue to deviceMap[device].queues, and queueFamilyIndex to deviceMap[device].queueIndexMap
    if (deviceMap.find(device) != deviceMap.end()) {
        deviceMap[device]->queues.emplace(*pQueue);

        if (deviceMap[device]->queueIndexMap.find(*pQueue) != deviceMap[device]->queueIndexMap.end())
            deviceMap[device]->queueIndexMap.erase(*pQueue);
        deviceMap[device]->queueIndexMap.emplace(*pQueue, queueFamilyIndex);
    }

    // queues are dispatchable objects.
    // Create dispatchMap entry with this queue as its key.
    // Copy the device dispatch table to the new dispatch table.
    VkDevice que = static_cast<VkDevice>(static_cast<void *>(*pQueue));
    if (dispatchMap.find(que) != dispatchMap.end()) dispatchMap.erase(que);
    dispatchMap.emplace(que, dispMap);
}

VKAPI_ATTR void VKAPI_CALL GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2 *pQueueInfo, VkQueue *pQueue) {
    if (pQueueInfo) GetDeviceQueue(device, pQueueInfo->queueFamilyIndex, pQueueInfo->queueIndex, pQueue);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo,
                                                  const VkAllocationCallbacks *pAllocator, VkSwapchainKHR *pSwapchain) {
    DispatchMapStruct *dispMap = get_dispatch_info(device);
    assert(dispMap);
    VkuDeviceDispatchTable *pDisp = dispMap->device_dispatch_table;

    // This layer does an image copy later on, and the copy command expects the
    // transfer src bit to be on.
    VkSwapchainCreateInfoKHR myCreateInfo = *pCreateInfo;
    myCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    VkResult result = pDisp->CreateSwapchainKHR(device, &myCreateInfo, pAllocator, pSwapchain);

    // Save the swapchain in a map of we are taking screenshots.
    std::lock_guard<std::mutex> lg(globalLock);
    if (settings.isFrameAfterEndOfCaptureRange(0)) {
        // No screenshots in the list to take
        return result;
    }

    if (result == VK_SUCCESS) {
        // Create a mapping for a swapchain to a device, image extent, and
        // format
        SwapchainMapStruct *swapchainMapElem = new SwapchainMapStruct;
        swapchainMapElem->device = device;
        swapchainMapElem->imageExtent = pCreateInfo->imageExtent;
        swapchainMapElem->format = pCreateInfo->imageFormat;
        // If there's a (destroyed) swapchain with the same handle, remove it from the swapchainMap
        if (swapchainMap.find(*pSwapchain) != swapchainMap.end()) {
            delete swapchainMap[*pSwapchain];
            swapchainMap.erase(*pSwapchain);
        }
        swapchainMap.insert(make_pair(*pSwapchain, swapchainMapElem));

        // Create a mapping for the swapchain object into the dispatch table
        // TODO is this needed? screenshot_device_table_map.emplace((void
        // *)pSwapchain, pTable);
    }

    return result;
}

VKAPI_ATTR void DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks *pAllocator) {
    {
        PROFILE("screenshot.finish");
        stopScreenshotThread();
        {
            std::unique_lock<std::mutex> lock(globalLock);
            screenshotQueuedCV.notify_one();
        }
        waitScreenshotThreadIsOver();
    }

    DispatchMapStruct *dispMap = get_dispatch_info(device);
    assert(dispMap);
    VkuDeviceDispatchTable *pDisp = dispMap->device_dispatch_table;
    pDisp->DestroySwapchainKHR(device, swapchain, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t *pCount,
                                                     VkImage *pSwapchainImages) {
    DispatchMapStruct *dispMap = get_dispatch_info(device);
    assert(dispMap);
    VkuDeviceDispatchTable *pDisp = dispMap->device_dispatch_table;
    VkResult result = pDisp->GetSwapchainImagesKHR(device, swapchain, pCount, pSwapchainImages);
    if (result != VK_SUCCESS) return result;

    // Save the swapchain images in a map if we are taking screenshots
    std::lock_guard<std::mutex> lg(globalLock);
    if (settings.isFrameAfterEndOfCaptureRange(0)) {
        // No screenshots in the list to take
        return result;
    }

    PROFILE("screenshot.init");
    if (pSwapchainImages && !swapchainMap.empty() && swapchainMap.find(swapchain) != swapchainMap.end()) {
        unsigned i;

        for (i = 0; i < *pCount; i++) {
            // Create a mapping for an image to a device, image extent, and
            // format
            if (imageMap[pSwapchainImages[i]] == NULL) {
                ImageMapStruct *imageMapElem = new ImageMapStruct;
                imageMap[pSwapchainImages[i]] = imageMapElem;
            }
            imageMap[pSwapchainImages[i]]->device = swapchainMap[swapchain]->device;
            imageMap[pSwapchainImages[i]]->imageExtent = swapchainMap[swapchain]->imageExtent;
            imageMap[pSwapchainImages[i]]->format = swapchainMap[swapchain]->format;

            std::shared_ptr<ScreenshotQueueData> data = std::make_shared<ScreenshotQueueData>();
            if (prepareScreenshotData(*data, pSwapchainImages[i])) {
                screenshotDataCache[pSwapchainImages[i]].push_back(data);
            }
        }

        // Add list of images to swapchain to image map
        SwapchainMapStruct *swapchainMapElem = swapchainMap[swapchain];
        if (i >= 1 && swapchainMapElem) {
            VkImage *imageList = new VkImage[i];
            swapchainMapElem->imageList = imageList;
            for (unsigned j = 0; j < i; j++) {
                swapchainMapElem->imageList[j] = pSwapchainImages[j];
            }
        }
    }
    return result;
}

void screenshotWriterThreadFunc() {
    while (true) {
        std::shared_ptr<ScreenshotQueueData> dataToSave;
        {
            PROFILE("Waiting for CPU")
            std::unique_lock<std::mutex> lock(globalLock);
            if (screenshotsData.empty()) {
                screenshotQueuedCV.wait(lock, [] { return !screenshotsData.empty() || shutdownScreenshotThread; });
            }
            if (shutdownScreenshotThread && screenshotsData.empty()) break;
            if (screenshotsData.empty()) continue;
            dataToSave = screenshotsData.front();
            screenshotsData.pop_front();
            PROFILE_COUNTER("screenshot.QueueSize", screenshotsData.size());
        }

        while (true) {
            VkResult fenceWaitResult;
            {
                PROFILE("Waiting for GPU")
                fenceWaitResult =
                    dataToSave->pTableDevice->WaitForFences(dataToSave->device, 1, &dataToSave->fence, VK_TRUE, UINT64_MAX);
            }

            if (fenceWaitResult == VK_SUCCESS) {
                writeScreenshot(*dataToSave);
            } else if (fenceWaitResult == VK_TIMEOUT) {
                continue;
            }
            break;
        }
        if (settings.isFrameAfterEndOfCaptureRange(dataToSave->frameNumber + 1)) {
#ifdef ANDROID
            __android_log_print(ANDROID_LOG_INFO, "screenshot", "No more frames to capture");
#else
            printf("screenshot: No more frames to capture\n");
            fflush(stdout);
#endif
            break;
        }
        {
            std::lock_guard<std::mutex> lock(globalLock);
            screenshotDataCache[dataToSave->image1].emplace_back(dataToSave);
            screenshotSavedCV.notify_one();
        }
    }
#ifdef ANDROID
    __android_log_print(ANDROID_LOG_INFO, "screenshot", "Resources cleanup");
#else
    printf("screenshot: Resources cleanup\n");
    fflush(stdout);
#endif
    {
        std::lock_guard<std::mutex> lock(globalLock);
        // Free all our maps since we are done with them.
        for (auto swapchainIter = swapchainMap.begin(); swapchainIter != swapchainMap.end(); swapchainIter++) {
            SwapchainMapStruct *swapchainMapElem = swapchainIter->second;
            delete swapchainMapElem;
        }
        for (auto imageIter = imageMap.begin(); imageIter != imageMap.end(); imageIter++) {
            ImageMapStruct *imageMapElem = imageIter->second;
            delete imageMapElem;
        }
        for (auto physDeviceIter = physDeviceMap.begin(); physDeviceIter != physDeviceMap.end(); physDeviceIter++) {
            PhysDeviceMapStruct *physDeviceMapElem = physDeviceIter->second;
            delete physDeviceMapElem;
        }
        swapchainMap.clear();
        imageMap.clear();
        physDeviceMap.clear();
        screenshotDataCache.clear();
    }

#ifdef ANDROID
    __android_log_print(ANDROID_LOG_INFO, "screenshot", "Images thread is over");
#else
    printf("screenshot: Images thread is over\n");
    fflush(stdout);
#endif
}

VKAPI_ATTR VkResult VKAPI_CALL QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo) {
    VkPresentInfoKHR presentInfo = *pPresentInfo;
    static int frameNumber = 0;
    if (vkuHasLayerSetting(globalLayerSettingSet, kSettingPauseCapture)) {
        vkuGetLayerSettingValue(globalLayerSettingSet, kSettingPauseCapture, pauseCapture);
    }
    if (!pauseCapture && settings.isFrameToCapture(frameNumber)) {
        // If there are 0 swapchains, skip taking the snapshot
        if (pPresentInfo && pPresentInfo->swapchainCount > 0) {
            std::unique_lock<std::mutex> lock(globalLock);
            while (!settings.allowToSkipFrames) {
                if (screenshotsData.size() < settings.maxScreenshotQueueSize) break;
                PROFILE("screenshot.wait");
                screenshotSavedCV.wait(lock, [] { return screenshotsData.size() < settings.maxScreenshotQueueSize; });
            }
            // We'll dump only one image: the first
            VkSwapchainKHR swapchain = pPresentInfo->pSwapchains[0];
            if (settings.allowToSkipFrames && screenshotsData.size() >= settings.maxScreenshotQueueSize) {
                static int skippedFrames = 0;
                PROFILE_COUNTER("screenshot.SkippedFrames", ++skippedFrames);
            } else {
                std::shared_ptr<ScreenshotQueueData> data;
                VkImage image = swapchainMap[swapchain]->imageList[pPresentInfo->pImageIndices[0]];
                if (!screenshotDataCache[image].empty()) {
                    data = screenshotDataCache[image].back();
                    screenshotDataCache[image].pop_back();
                } else {
                    data = std::make_shared<ScreenshotQueueData>();
                }
                if (queueScreenshot(*data, image, &presentInfo)) {
                    presentInfo.pWaitSemaphores = &data->semaphore;
                    presentInfo.waitSemaphoreCount = 1;
                    data->frameNumber = frameNumber;
                    screenshotsData.emplace_back(data);
                    PROFILE_COUNTER("screenshot.QueueSize", screenshotsData.size());
                    startScreenshotThread();
#ifdef ANDROID
                    __android_log_print(ANDROID_LOG_INFO, "screenshot", "Queued screeshot for frame: %d", frameNumber);
#else
                    printf("screenshot: Queued screeshot for frame: %d\n", frameNumber);
                    fflush(stdout);
#endif
                    screenshotQueuedCV.notify_one();
                }
            }
        } else {
#ifdef ANDROID
            __android_log_print(ANDROID_LOG_ERROR, "screenshot", "Failure - no swapchain specified\n");
#else
            fprintf(stderr, "screenshot: Failure - no swapchain specified\n");
#endif
        }
    }
    frameNumber++;
    DispatchMapStruct *dispMap = get_dispatch_info((VkDevice)queue);
    VkuDeviceDispatchTable *pDisp = dispMap->device_dispatch_table;
    assert(dispMap);
    VkResult result = pDisp->QueuePresentKHR(queue, &presentInfo);
    return result;
}

static const VkLayerProperties global_layer = {
    "VK_LAYER_LUNARG_screenshot",  // layerName
    VK_MAKE_VERSION(1, 0, 68),     // specVersion (clamped to final 1.0 spec version)
    1,                             // implementationVersion
    "Layer: screenshot",           // description
};

VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceLayerProperties(uint32_t *pCount, VkLayerProperties *pProperties) {
    return util_GetLayerProperties(1, &global_layer, pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount,
                                                              VkLayerProperties *pProperties) {
    return util_GetLayerProperties(1, &global_layer, pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount,
                                                                    VkExtensionProperties *pProperties) {
    if (pLayerName && !strcmp(pLayerName, global_layer.layerName)) return util_GetExtensionProperties(0, NULL, pCount, pProperties);

    return VK_ERROR_LAYER_NOT_PRESENT;
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char *pLayerName,
                                                                  uint32_t *pCount, VkExtensionProperties *pProperties) {
    if (pLayerName && !strcmp(pLayerName, global_layer.layerName)) return util_GetExtensionProperties(0, NULL, pCount, pProperties);

    assert(physicalDevice);

    VkuInstanceDispatchTable *pTable = instance_dispatch_table(physicalDevice);
    return pTable->EnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t *pToolCount,
                                                                  VkPhysicalDeviceToolPropertiesEXT *pToolProperties) {
    static const VkPhysicalDeviceToolPropertiesEXT screenshot_layer_tool_props = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TOOL_PROPERTIES_EXT,
        nullptr,
        "Screenshot Layer",
        "1",
        VK_TOOL_PURPOSE_PROFILING_BIT_EXT | VK_TOOL_PURPOSE_TRACING_BIT_EXT | VK_TOOL_PURPOSE_ADDITIONAL_FEATURES_BIT_EXT,
        "The VK_LAYER_LUNARG_screenshot layer records frames to image files.",
        "VK_LAYER_LUNARG_screenshot"};

    auto original_pToolProperties = pToolProperties;
    if (pToolProperties != nullptr) {
        *pToolProperties = screenshot_layer_tool_props;
        pToolProperties = ((*pToolCount > 1) ? &pToolProperties[1] : nullptr);
        (*pToolCount)--;
    }

    VkuInstanceDispatchTable *pInstanceTable = instance_dispatch_table(physicalDevice);
    VkResult result = pInstanceTable->GetPhysicalDeviceToolPropertiesEXT(physicalDevice, pToolCount, pToolProperties);

    if (original_pToolProperties != nullptr) {
        pToolProperties = original_pToolProperties;
    }

    (*pToolCount)++;

    return result;
}

static PFN_vkVoidFunction intercept_core_instance_command(const char *name);

static PFN_vkVoidFunction intercept_core_device_command(const char *name);

static PFN_vkVoidFunction intercept_khr_swapchain_command(const char *name, VkDevice dev);

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetDeviceProcAddr(VkDevice dev, const char *funcName) {
    PFN_vkVoidFunction proc = intercept_core_device_command(funcName);
    if (proc) return proc;

    if (dev == NULL) {
        return NULL;
    }

    proc = intercept_khr_swapchain_command(funcName, dev);
    if (proc) return proc;

    DispatchMapStruct *dispMap = get_dispatch_info(dev);
    assert(dispMap);
    VkuDeviceDispatchTable *pDisp = dispMap->device_dispatch_table;

    if (pDisp->GetDeviceProcAddr == NULL) return NULL;
    return pDisp->GetDeviceProcAddr(dev, funcName);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetInstanceProcAddr(VkInstance instance, const char *funcName) {
    PFN_vkVoidFunction proc = intercept_core_instance_command(funcName);
    if (proc) return proc;

    assert(instance);

    proc = intercept_core_device_command(funcName);
    if (!proc) proc = intercept_khr_swapchain_command(funcName, VK_NULL_HANDLE);
    if (proc) return proc;

    VkuInstanceDispatchTable *pTable = instance_dispatch_table(instance);
    if (pTable->GetInstanceProcAddr == NULL) return NULL;
    return pTable->GetInstanceProcAddr(instance, funcName);
}

static PFN_vkVoidFunction intercept_core_instance_command(const char *name) {
    static const struct {
        const char *name;
        PFN_vkVoidFunction proc;
    } core_instance_commands[] = {
        {"vkGetInstanceProcAddr", reinterpret_cast<PFN_vkVoidFunction>(GetInstanceProcAddr)},
        {"vkCreateInstance", reinterpret_cast<PFN_vkVoidFunction>(CreateInstance)},
        {"vkCreateDevice", reinterpret_cast<PFN_vkVoidFunction>(CreateDevice)},
        {"vkEnumeratePhysicalDevices", reinterpret_cast<PFN_vkVoidFunction>(EnumeratePhysicalDevices)},
        {"vkEnumeratePhysicalDeviceGroups", reinterpret_cast<PFN_vkVoidFunction>(EnumeratePhysicalDeviceGroups)},
        {"vkEnumerateInstanceLayerProperties", reinterpret_cast<PFN_vkVoidFunction>(EnumerateInstanceLayerProperties)},
        {"vkEnumerateDeviceLayerProperties", reinterpret_cast<PFN_vkVoidFunction>(EnumerateDeviceLayerProperties)},
        {"vkEnumerateInstanceExtensionProperties", reinterpret_cast<PFN_vkVoidFunction>(EnumerateInstanceExtensionProperties)},
        {"vkEnumerateDeviceExtensionProperties", reinterpret_cast<PFN_vkVoidFunction>(EnumerateDeviceExtensionProperties)},
        {"vkGetPhysicalDeviceToolPropertiesEXT", reinterpret_cast<PFN_vkVoidFunction>(GetPhysicalDeviceToolPropertiesEXT)}};

    for (size_t i = 0; i < ARRAY_SIZE(core_instance_commands); i++) {
        if (!strcmp(core_instance_commands[i].name, name)) return core_instance_commands[i].proc;
    }

    return nullptr;
}

static PFN_vkVoidFunction intercept_core_device_command(const char *name) {
    static const struct {
        const char *name;
        PFN_vkVoidFunction proc;
    } core_device_commands[] = {
        {"vkGetDeviceProcAddr", reinterpret_cast<PFN_vkVoidFunction>(GetDeviceProcAddr)},
        {"vkGetDeviceQueue", reinterpret_cast<PFN_vkVoidFunction>(GetDeviceQueue)},
        {"vkGetDeviceQueue2", reinterpret_cast<PFN_vkVoidFunction>(GetDeviceQueue2)},
        {"vkDestroyDevice", reinterpret_cast<PFN_vkVoidFunction>(DestroyDevice)},
    };

    for (size_t i = 0; i < ARRAY_SIZE(core_device_commands); i++) {
        if (!strcmp(core_device_commands[i].name, name)) return core_device_commands[i].proc;
    }

    return nullptr;
}

static PFN_vkVoidFunction intercept_khr_swapchain_command(const char *name, VkDevice dev) {
    static const struct {
        const char *name;
        PFN_vkVoidFunction proc;
    } khr_swapchain_commands[] = {
        {"vkCreateSwapchainKHR", reinterpret_cast<PFN_vkVoidFunction>(CreateSwapchainKHR)},
        {"vkDestroySwapchainKHR", reinterpret_cast<PFN_vkVoidFunction>(DestroySwapchainKHR)},
        {"vkGetSwapchainImagesKHR", reinterpret_cast<PFN_vkVoidFunction>(GetSwapchainImagesKHR)},
        {"vkQueuePresentKHR", reinterpret_cast<PFN_vkVoidFunction>(QueuePresentKHR)},
    };

    if (dev) {
        DeviceMapStruct *devMap = get_device_info(dev);
        if (!devMap->wsi_enabled) return nullptr;
    }

    for (size_t i = 0; i < ARRAY_SIZE(khr_swapchain_commands); i++) {
        if (!strcmp(khr_swapchain_commands[i].name, name)) return khr_swapchain_commands[i].proc;
    }

    return nullptr;
}

}  // namespace screenshot

#if defined(__GNUC__) && __GNUC__ >= 4
#define EXPORT_FUNCTION __attribute__((visibility("default")))
#elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590)
#define EXPORT_FUNCTION __attribute__((visibility("default")))
#else
#define EXPORT_FUNCTION
#endif

// loader-layer interface v0, just wrappers since there is only a layer

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t *pCount,
                                                                                  VkLayerProperties *pProperties) {
    return screenshot::EnumerateInstanceLayerProperties(pCount, pProperties);
}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount,
                                                                                VkLayerProperties *pProperties) {
    // the layer command handles VK_NULL_HANDLE just fine internally
    assert(physicalDevice == VK_NULL_HANDLE);
    return screenshot::EnumerateDeviceLayerProperties(VK_NULL_HANDLE, pCount, pProperties);
}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount,
                                                                                      VkExtensionProperties *pProperties) {
    return screenshot::EnumerateInstanceExtensionProperties(pLayerName, pCount, pProperties);
}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice,
                                                                                    const char *pLayerName, uint32_t *pCount,
                                                                                    VkExtensionProperties *pProperties) {
    // the layer command handles VK_NULL_HANDLE just fine internally
    assert(physicalDevice == VK_NULL_HANDLE);
    return screenshot::EnumerateDeviceExtensionProperties(VK_NULL_HANDLE, pLayerName, pCount, pProperties);
}

EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice dev, const char *funcName) {
    return screenshot::GetDeviceProcAddr(dev, funcName);
}

EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char *funcName) {
    return screenshot::GetInstanceProcAddr(instance, funcName);
}
