/*
 * Copyright (c) 2016-2019 Advanced Micro Devices, Inc. All rights reserved.
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
#include "vktrace_lib_trim.h"
#include "vktrace_lib_helpers.h"
#include "vktrace_trace_packet_utils.h"
#include "vktrace_vk_vk_packets.h"
#include "vktrace_vk_packet_id.h"
#include "vk_struct_size_helper.h"
#include "vulkan/vulkan.h"

// defined in vktrace_lib_trace.cpp
extern layer_device_data *mdd(void *object);
extern layer_instance_data *mid(void *object);

bool g_trimEnabled = false;
bool g_trimIsPreTrim = false;
bool g_trimIsInTrim = false;
bool g_trimIsPostTrim = false;
uint64_t g_trimFrameCounter = 0;
uint64_t g_trimStartFrame = 0;
uint64_t g_trimEndFrame = UINT64_MAX;
uint64_t g_trimMaxBatchCmdCount = UINT64_MAX;
bool g_trimAlreadyFinished = false;
bool g_trimPostProcess = false;

bool g_TraceLockEnabled = false;

std::mutex g_trimImageHandling_Mutex;

static std::mutex g_Mutex_CommandBufferPipelineMap;

namespace trim {
// Tracks the existence of objects from the very beginning of the application
static StateTracker s_trimGlobalStateTracker;

// A snapshot of the GlobalStateTracker taken at the start of the trim frames.
static StateTracker s_trimStateTrackerSnapshot;

// Maximum length of the VKTRACE_TRIM_TRIGGER environment variable
static const int MAX_TRIM_TRIGGER_OPTION_STRING_LENGTH = 32;

static const int MAX_TRIM_TRIGGER_TYPE_STRING_LENGTH = 16;
static const int TRACE_TRIGGER_STRING_LENGTH = MAX_TRIM_TRIGGER_OPTION_STRING_LENGTH + MAX_TRIM_TRIGGER_TYPE_STRING_LENGTH;

VKTRACE_CRITICAL_SECTION trimRecordedPacketLock;
VKTRACE_CRITICAL_SECTION trimStateTrackerLock;
VKTRACE_CRITICAL_SECTION trimCommandBufferPacketLock;

//=========================================================================
// Information necessary to create the staged buffer and memory for DEVICE_LOCAL
// buffers.
// This holds all the necessary structures so that we can fill them in once when
// we make
// the call during trim snapshot, and then reuse them when generating the calls
// to recreate
// the buffer into the trace file.
//=========================================================================
struct StagingInfo {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkBufferCreateInfo bufferCreateInfo = {};
    VkMemoryRequirements bufferMemoryRequirements = {};

    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkMemoryAllocateInfo memoryAllocationInfo = {};

    // Region for copying buffers
    VkBufferCopy copyRegion = {};

    // Per-miplevel region for copying images
    std::vector<VkBufferImageCopy> imageCopyRegions;

    VkCommandPool commandPool = VK_NULL_HANDLE;
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    VkQueue queue = VK_NULL_HANDLE;
};

//=========================================================================
// Map to associate trace-time buffer with a staged buffer
//=========================================================================
static std::unordered_map<VkBuffer, StagingInfo> s_bufferToStagedInfoMap;

//=========================================================================
// Map to associate trace-time image with a staged buffer
//=========================================================================
static std::unordered_map<VkImage, StagingInfo> s_imageToStagedInfoMap;

//=========================================================================
// Map to associate trace-time image with its SubResourceSizes
//=========================================================================
static std::unordered_map<VkImage, std::vector<VkDeviceSize>> s_imageToSubResourceSizes;

//=========================================================================
// Associates a Device to a trim-specific CommandPool
//=========================================================================
static std::unordered_map<VkDevice, std::unordered_map<uint32_t, VkCommandPool>> s_deviceToCommandPoolMap;

//=========================================================================
// Typically an application will have one VkAllocationCallbacks struct and
// will pass in that same address as needed, so we'll keep a map to correlate
// the supplied address to the AllocationCallbacks object.
//=========================================================================
static std::unordered_map<const void *, VkAllocationCallbacks> s_trimAllocatorMap;

//=========================================================================
// Associates a Device to a trim-specific CommandBuffer
//=========================================================================
static std::unordered_map<VkDevice, VkCommandBuffer> s_deviceToCommandBufferMap;

//=========================================================================
VkDeviceSize calculateImageSubResourceSize(VkDevice device, VkImageCreateInfo imageCreateInfo,
                                           const VkAllocationCallbacks *pAllocator, uint32_t subresourceIndex) {
    VkDeviceSize imageSubResourceSize = 0;
    VkImage image;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.extent.depth = std::max(static_cast<uint32_t>(1), imageCreateInfo.extent.depth >> subresourceIndex);
    imageCreateInfo.extent.height = std::max(static_cast<uint32_t>(1), imageCreateInfo.extent.height >> subresourceIndex);
    imageCreateInfo.extent.width = std::max(static_cast<uint32_t>(1), imageCreateInfo.extent.width >> subresourceIndex);

    VkResult result = mdd(device)->devTable.CreateImage(device, &imageCreateInfo, NULL, &image);
    assert(result == VK_SUCCESS);

    if (result == VK_SUCCESS) {
        VkMemoryRequirements memoryRequirements;

        mdd(device)->devTable.GetImageMemoryRequirements(device, image, &memoryRequirements);
        imageSubResourceSize = memoryRequirements.size;
    }
    mdd(device)->devTable.DestroyImage(device, image, pAllocator);
    return imageSubResourceSize;
}

bool calculateImageAllSubResourceSize(VkDevice device, VkImageCreateInfo imageCreateInfo, const VkAllocationCallbacks *pAllocator,
                                      std::vector<VkDeviceSize> &subResourceSizes) {
    uint32_t wholeSize = 0;
    subResourceSizes.push_back(0);
    for (uint32_t i = 0; i < imageCreateInfo.mipLevels; i++) {
        subResourceSizes.push_back(calculateImageSubResourceSize(device, imageCreateInfo, pAllocator, i));
        wholeSize += subResourceSizes[i + 1];
    }
    subResourceSizes[0] = wholeSize;
    return true;
}

void addImageSubResourceSizes(VkImage image, std::vector<VkDeviceSize> subResourceSizes) {
    std::lock_guard<std::mutex> guard(g_trimImageHandling_Mutex);
    s_imageToSubResourceSizes[image] = subResourceSizes;
}

bool getImageSubResourceSizes(VkImage image, std::vector<VkDeviceSize> *pSubresourceSizes) {
    std::lock_guard<std::mutex> guard(g_trimImageHandling_Mutex);
    bool getImageSubResourceSizesResult = false;
    auto it = s_imageToSubResourceSizes.find(image);
    if (it != s_imageToSubResourceSizes.end()) {
        if (pSubresourceSizes != nullptr) {
            *pSubresourceSizes = it->second;
        }
        getImageSubResourceSizesResult = true;
    }
    return getImageSubResourceSizesResult;
}

void deleteImageSubResourceSizes(VkImage image) {
    std::lock_guard<std::mutex> guard(g_trimImageHandling_Mutex);
    auto it = s_imageToSubResourceSizes.find(image);
    if (it != s_imageToSubResourceSizes.end()) {
        s_imageToSubResourceSizes.erase(it);
    }
}

VkDeviceSize getImageSize(VkImage image) {
    std::vector<VkDeviceSize> subResourceSizes;
    getImageSubResourceSizes(image, &subResourceSizes);
    // Note: the image to subsresource map is created for
    // VK_IMAGE_TILING_OPTIMAL image difference fix, the vector
    // might be empty for other image tiling
    if (subResourceSizes.size() > 0) {
        return subResourceSizes[0];
    } else {
        return 0;
    }
}

VkDeviceSize getImageSubResourceOffset(VkImage image, uint32_t mipLevel) {
    VkDeviceSize subResourceOffset = 0;
    std::vector<VkDeviceSize> subResourceSizes;
    getImageSubResourceSizes(image, &subResourceSizes);
    for (uint32_t i = 0; i < mipLevel; i++) {
        subResourceOffset += subResourceSizes[i + 1];
    }
    return subResourceOffset;
}
//=========================================================================
// Start trimming
//=========================================================================
void start() {
    g_trimIsPreTrim = false;
    g_trimIsInTrim = true;
    snapshot_state_tracker();

    if (!g_trimPostProcess) {
        // This will write packets to recreate all objects
        write_all_referenced_object_calls();
    }
}

//=========================================================================
// Stop trimming
//=========================================================================
void stop() {
    g_trimIsInTrim = false;
    g_trimIsPostTrim = true;

    if (g_trimPostProcess) {
        // This will write packets to recreate all objects
        write_all_referenced_object_calls();
    }

    // write packets to destroy all created objects
    write_destroy_packets();

    // clean up
    s_trimStateTrackerSnapshot.clear();

    g_trimAlreadyFinished = true;
}

//=========================================================================
void AddImageTransition(VkCommandBuffer commandBuffer, ImageTransition transition) {
    s_trimGlobalStateTracker.AddImageTransition(commandBuffer, transition);
}

//=========================================================================
std::list<ImageTransition> GetImageTransitions(VkCommandBuffer commandBuffer) {
    return s_trimGlobalStateTracker.m_cmdBufferToImageTransitionsMap[commandBuffer];
}

//=========================================================================
void ClearImageTransitions(VkCommandBuffer commandBuffer) { s_trimGlobalStateTracker.ClearImageTransitions(commandBuffer); }

//=========================================================================
void AddBufferTransition(VkCommandBuffer commandBuffer, BufferTransition transition) {
    s_trimGlobalStateTracker.AddBufferTransition(commandBuffer, transition);
}

//=========================================================================
std::list<BufferTransition> GetBufferTransitions(VkCommandBuffer commandBuffer) {
    return s_trimGlobalStateTracker.m_cmdBufferToBufferTransitionsMap[commandBuffer];
}

//=========================================================================
void ClearBufferTransitions(VkCommandBuffer commandBuffer) { s_trimGlobalStateTracker.ClearBufferTransitions(commandBuffer); }

//=========================================================================
// Returns true if specified trigger enabled; false otherwise
//=========================================================================
bool is_trim_trigger_enabled(enum enum_trim_trigger triggerType) {
    // only one type of trigger is allowed at same time
    return getTraceTriggerOptionString(triggerType) != nullptr;
}

//=========================================================================
// Get user defined hotkey string.
// The user defined hotkey string can be one of F1-F12,TAB,CONTROL
// return:
//  char* value pointer to user defined hotkey string.
//=========================================================================
char *get_hotkey_string() {
    static bool firstTimeRunning = true;
    static char trim_trigger_hotkey[MAX_TRIM_TRIGGER_OPTION_STRING_LENGTH] = "";

    if (firstTimeRunning) {
        firstTimeRunning = false;
        const char *trimHotKeyOption = getTraceTriggerOptionString(enum_trim_trigger::hotKey);

        // the hotkey trigger option string could be <keyname> or
        // <keyname>-<frameCount>, we seperate the two case by search
        // "-" from the string.
        const char *trimHotKeyFrames = strstr(trimHotKeyOption, "-");
        if (trimHotKeyFrames) {
            // if the trigger option string include "-", we get the hotkey by
            // only picking the part string before "-";
            size_t len = trimHotKeyFrames - trimHotKeyOption;
            strncat(trim_trigger_hotkey, trimHotKeyOption, len);
        } else {
            strcpy(trim_trigger_hotkey, trimHotKeyOption);
        }
    }
    return trim_trigger_hotkey;
}

#if defined(PLATFORM_LINUX)
#if defined(ANDROID)

// TODO
enum_key_state key_state_platform_specific(char *pHotkeyString) {
    enum_key_state KeyState = enum_key_state::Released;
    return KeyState;
}

#else

static xcb_connection_t *keyboardConnection = nullptr;

//=========================================================================
// On Linux platform, xcb calls need Connection which is connected to target
// server, because hotkey process is supposed to insert into target application,
// so we need to capture the connection that target app use. the function is
// used to insert into Vulkan call to capture and save the connection.
//=========================================================================
void set_keyboard_connection(xcb_connection_t *pConnection) { keyboardConnection = pConnection; }

//=========================================================================
// Get connection which is used by target application.
//=========================================================================
xcb_connection_t *get_keyboard_connection() { return keyboardConnection; }

//=========================================================================
// Get specified key's real time state: it's released or pressed now.
// the function is platform specific.
//=========================================================================
enum_key_state GetAsyncKeyState(int keyCode) {
    enum_key_state keyState = enum_key_state::Released;
    xcb_connection_t *connection = get_keyboard_connection();
    xcb_key_symbols_t *hotKeySymbols = xcb_key_symbols_alloc(connection);
    if (hotKeySymbols) {
        xcb_keycode_t *keyLoc = xcb_key_symbols_get_keycode(hotKeySymbols, keyCode);
        if (keyLoc) {
            xcb_query_keymap_cookie_t cookie = xcb_query_keymap(connection);
            xcb_query_keymap_reply_t *keysBitMap = xcb_query_keymap_reply(connection, cookie, NULL);
            if ((keysBitMap->keys[(*keyLoc / 8)] & (1 << (*keyLoc % 8))) != 0) {
                keyState = enum_key_state::Pressed;
            }
            free(keysBitMap);
            free(keyLoc);
        }
        xcb_key_symbols_free(hotKeySymbols);
    }

    return keyState;
}

//=========================================================================
// Get specified key's real time state: it's released or pressed now.
// the function is "semi"cross-platform: the interface is cross-platform, the
// implementation is platform specific.
// Input:
//      char *pHotkeyString, the string of target key
// Output:
//      return specified key's real time state: it's released or pressed now.
//=========================================================================
enum_key_state key_state_platform_specific(char *pHotkeyString) {
    enum_key_state KeyState = enum_key_state::Released;
    static std::unordered_map<std::string, int> KeyCodeMap = {{"F1", XK_F1},
                                                              {"F2", XK_F2},
                                                              {"F3", XK_F3},
                                                              {"F4", XK_F4},
                                                              {"F5", XK_F5},
                                                              {"F6", XK_F6},
                                                              {"F7", XK_F7},
                                                              {"F8", XK_F8},
                                                              {"F9", XK_F9},
                                                              {"F10", XK_F10},
                                                              {"F11", XK_F11},
                                                              {"F12", XK_F12},
                                                              {"Tab", XK_Tab},
                                                              {"ControlLeft", XK_Control_L},
                                                              {"ControlRight", XK_Control_R}};

    std::string targetKeyString(pHotkeyString);
    std::unordered_map<std::string, int>::iterator iteratorKeyCode = KeyCodeMap.find(targetKeyString);
    if (iteratorKeyCode != KeyCodeMap.end()) {
        int keyCode = iteratorKeyCode->second;
        KeyState = GetAsyncKeyState(keyCode);
    }
    return KeyState;
}
#endif  // ANDROID
#elif defined(WIN32)

//=========================================================================
// Get specified key's real time state: it's released or pressed now.
// the function is "semi"cross-platform: the interface is cross-platform, the
// implementation is platform specific.
// Input:
//      char *pHotkeyString, the string of target key, the input cannot be
//      nullptr.
// Output:
//      return specified key's real time state: it's released or pressed now.
//=========================================================================
enum_key_state key_state_platform_specific(char *pHotkeyString) {
    enum_key_state KeyState = enum_key_state::Released;
    static std::unordered_map<std::string, int> KeyCodeMap = {
        {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3},   {"F4", VK_F4},   {"F5", VK_F5},   {"F6", VK_F6},   {"F7", VK_F7},
        {"F8", VK_F8}, {"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12}, {"TAB", VK_TAB}, {"CONTROL", VK_CONTROL}};

    std::string targetKeyString(pHotkeyString);
    std::unordered_map<std::string, int>::iterator iteratorKeyCode = KeyCodeMap.find(targetKeyString);
    if (iteratorKeyCode != KeyCodeMap.end()) {
        int keyCode = iteratorKeyCode->second;
        if (GetAsyncKeyState(keyCode) != 0) {  // the key is down or was down
                                               // after the previous call to
                                               // GetAsyncKeyState
            KeyState = enum_key_state::Pressed;
        }
    }
    return KeyState;
}
#endif

//=========================================================================
// Get specified key's real time state, if the input is null pointer, return key
// released.
//=========================================================================
enum_key_state key_state(char *pHotkeyString) {
    enum_key_state KeyState = enum_key_state::Released;
    if (pHotkeyString) {
        KeyState = key_state_platform_specific(pHotkeyString);
    }
    return KeyState;
}

//=========================================================================
// Refresh hotkey current state and detect if it's triggered or not.
//=========================================================================
bool is_hotkey_trim_triggered() {
    bool hotkeyTriggered = false;
    static enum_key_state currentHotKeyState = enum_key_state::Released;

    static char *trimHotkeyString = get_hotkey_string();

    enum_key_state hotKeyState = key_state(trimHotkeyString);

    if (hotKeyState != currentHotKeyState) {
        switch (hotKeyState) {
            case enum_key_state::Pressed:
                hotkeyTriggered = true;
                break;
            case enum_key_state::Released:
                break;
        }
    }
    currentHotKeyState = hotKeyState;
    return hotkeyTriggered;
}

//=========================================================================
char *getTraceTriggerOptionString(enum enum_trim_trigger triggerType) {
    static const char TRIM_TRIGGER_HOTKEY_TYPE_STRING[] = "hotkey";
    static const char TRIM_TRIGGER_FRAMES_TYPE_STRING[] = "frames";
    static const char TRIM_TRIGGER_FRAMES_DEFAULT_HOTKEY_STRING[] = "F12";

    static bool firstTimeRunning = true;
    static enum enum_trim_trigger trimTriggerType = enum_trim_trigger::none;
    static char trim_trigger_option[MAX_TRIM_TRIGGER_OPTION_STRING_LENGTH] = "";

    char typeString[MAX_TRIM_TRIGGER_TYPE_STRING_LENGTH];

    if (firstTimeRunning) {
        firstTimeRunning = false;
        const char *trim_trigger_string = vktrace_get_global_var(VKTRACE_TRIM_TRIGGER_ENV);
        if (trim_trigger_string) {
            assert(strlen(trim_trigger_string) < TRACE_TRIGGER_STRING_LENGTH);

            // only one type of trigger is allowed at same time
            if (sscanf(trim_trigger_string, "%[^-]-%s", typeString, trim_trigger_option) == 2) {
                if (strcmp(typeString, TRIM_TRIGGER_HOTKEY_TYPE_STRING) == 0) {
                    trimTriggerType = enum_trim_trigger::hotKey;
                } else {
                    if (strcmp(typeString, TRIM_TRIGGER_FRAMES_TYPE_STRING) == 0) {
                        trimTriggerType = enum_trim_trigger::frameCounter;
                    }
                }
            } else {
                if (strcmp(trim_trigger_string, TRIM_TRIGGER_HOTKEY_TYPE_STRING) == 0) {
                    // default hotkey
                    trimTriggerType = enum_trim_trigger::hotKey;
                    strcpy(trim_trigger_option, TRIM_TRIGGER_FRAMES_DEFAULT_HOTKEY_STRING);
                }
            }
        }
    }
    return (trimTriggerType != enum_trim_trigger::none) && (trimTriggerType == triggerType) ? trim_trigger_option : nullptr;
}

//=========================================================================
void getTrimPreProcessAndTraceLockOptions() {
    static bool firstTimeRun = true;
    if (firstTimeRun) {
        firstTimeRun = false;
        const char *env_trimPostProcess = vktrace_get_global_var(VKTRACE_TRIM_POST_PROCESS_ENV);
        if (env_trimPostProcess) {
            int envvalue;
            if (sscanf(env_trimPostProcess, "%d", &envvalue) == 1) {
                if (envvalue == 1) {
                    g_trimPostProcess = true;
                } else if (envvalue == 0) {
                    g_trimPostProcess = false;
                } else {
                    // Other values
                    g_trimPostProcess = false;
                }
            }
        }

        const char *lock_guard_alway_enabled_env = vktrace_get_global_var(VKTRACE_ENABLE_TRACE_LOCK_ENV);
        if (lock_guard_alway_enabled_env) {
            int lock_guard_value;
            if (sscanf(lock_guard_alway_enabled_env, "%d", &lock_guard_value) == 1) {
                if (1 == lock_guard_value) {
                    g_TraceLockEnabled = true;
                } else {
                    g_TraceLockEnabled = false;
                }
            }
        }
    }
}

//=========================================================================
void getTrimMaxBatchCmdCountOption() {
    // Set default trim maximum commands batched count by:
    // checking for physical devices maxMemoryAllocationCount minus the memory object count
    // which are already allocated.
    // This number will be used to determine the maximum batched commands per command buffer
    // For a safe range, by default this g_trimMaxBatchCmdCount is set to
    // smaller max batch count (divide by 100 factor).
    for (auto phyDeviceItr = s_trimStateTrackerSnapshot.createdPhysicalDevices.begin();
         phyDeviceItr != s_trimStateTrackerSnapshot.createdPhysicalDevices.end(); phyDeviceItr++) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDevice physicalDevice = phyDeviceItr->first;
        mid(physicalDevice)->instTable.GetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        uint32_t memoryAllocationCount = deviceProperties.limits.maxMemoryAllocationCount;
        if (memoryAllocationCount < g_trimMaxBatchCmdCount) {
            g_trimMaxBatchCmdCount = memoryAllocationCount;
        }
    }
    g_trimMaxBatchCmdCount -= s_trimStateTrackerSnapshot.createdDeviceMemorys.size();
    uint64_t maxAllowBatchCmdCount = g_trimMaxBatchCmdCount;
    g_trimMaxBatchCmdCount /= 100;

    // Get trim maximum commands batched count set by the option if there is any.
    const char *trimMaxCmdBatchSizeStr = vktrace_get_global_var(VKTRACE_TRIM_MAX_COMMAND_BATCH_SIZE_ENV);
    if (trimMaxCmdBatchSizeStr) {
        uint64_t trimMaxCmdBatchSzValue = 0;
        if (sscanf(trimMaxCmdBatchSizeStr, "%" PRIu64, &trimMaxCmdBatchSzValue) == 1) {
            if (trimMaxCmdBatchSzValue > maxAllowBatchCmdCount) {
                g_trimMaxBatchCmdCount = maxAllowBatchCmdCount;
            } else if (trimMaxCmdBatchSzValue > 0 && trimMaxCmdBatchSzValue < maxAllowBatchCmdCount) {
                g_trimMaxBatchCmdCount = trimMaxCmdBatchSzValue;
            }
        }
    }
}

//=========================================================================
void initialize() {
    getTrimPreProcessAndTraceLockOptions();
    const char *trimFrames = getTraceTriggerOptionString(enum_trim_trigger::frameCounter);
    if (trimFrames != nullptr) {
        uint32_t numFrames = 0;
        if (sscanf(trimFrames, "%" PRIu64 ",%" PRIu32, &g_trimStartFrame, &numFrames) == 2) {
            g_trimEndFrame = g_trimStartFrame + numFrames;
        } else {
            int U_ASSERT_ONLY matches = sscanf(trimFrames, "%" PRIu64 "-%" PRIu64, &g_trimStartFrame, &g_trimEndFrame);
            assert(matches == 2);
        }

        // make sure the start/end frames are in expected order.
        if (g_trimStartFrame <= g_trimEndFrame) {
            g_trimEnabled = true;
            g_trimIsPreTrim = (g_trimStartFrame > 0);
            g_trimIsInTrim = (g_trimStartFrame == 0);
        }
    }
    if ((!g_trimEnabled) && (trim::is_trim_trigger_enabled(trim::enum_trim_trigger::hotKey))) {

        // There are two types of hotkey trigger, their command line option string
        // are hotkey-<keyname> and hotkey-<keyname>-<frameCount>, the latter one
        // means capture the next <frameCount> frames after pressing the hotkey
        // once.
        //
        // Here we first get the hotkey option string, it's supposed to be <keyname>
        // or <keyname>-<frameCount>, then we search "-" from the string. if include
        // "-", it should not belong to the case of only <keyname> and  we need to
        // handle hotkey-<keyname>-<frameCount>.
        //
        // the way of handling is we get frameCount here through parsing and save it
        // to g_trimEndFrame, then when user press hotkey later, the frame counter at
        // that time will be set to g_trimStartFrame and g_trimEndFrame will be added
        // g_trimStartFrame.
        //
        // the following process is to get frameCount through parsing and save it to
        // g_trimEndFrame. only valid value can be set to g_trimEndFrame, if the
        // parsing meet any error, the g_trimEndFrame will keep it initial value
        // UINT64_MAX which means the trim capture still stop by hotkey.
        const char *trimHotKeyOption = getTraceTriggerOptionString(enum_trim_trigger::hotKey);
        const char *trimHotKeyFrames = strstr(trimHotKeyOption,"-");
        if (trimHotKeyFrames)
        { //trimHotKeyOption include "-", we continue the parsing to get frameCount

            uint32_t numFrames = 0;
            trimHotKeyFrames++;//ignore the "-" symbol
            if (!strstr(trimHotKeyFrames, "-"))
            {
                if (sscanf(trimHotKeyFrames, "%" PRIu32, &numFrames) == 1)
                {
                    //the frame number after hotkey press should not be 0 or negtive.
                    if (numFrames > 0)
                    {
                        g_trimEndFrame = static_cast<uint64_t>(numFrames);
                    }
                }
            }
        }
        g_trimEnabled = true;
        g_trimIsPreTrim = true;
        g_trimIsInTrim = false;
    }

    if (g_trimEnabled) {
        vktrace_create_critical_section(&trimStateTrackerLock);
        vktrace_create_critical_section(&trimRecordedPacketLock);
        vktrace_create_critical_section(&trimCommandBufferPacketLock);
        vktrace_create_critical_section(&trimTransitionMapLock);
    }
}

//=========================================================================
void deinitialize() {
    s_trimStateTrackerSnapshot.clear();
    s_trimGlobalStateTracker.clear();

    vktrace_delete_critical_section(&trimRecordedPacketLock);
    vktrace_delete_critical_section(&trimStateTrackerLock);
    vktrace_delete_critical_section(&trimCommandBufferPacketLock);
    vktrace_delete_critical_section(&trimTransitionMapLock);
}

//=========================================================================
void add_Allocator(const VkAllocationCallbacks *pAllocator) {
    if (pAllocator != NULL) {
        if (s_trimAllocatorMap.find(pAllocator) == s_trimAllocatorMap.end()) {
            // need to add this allocator address
            s_trimAllocatorMap[pAllocator] = *pAllocator;
        }
    }
}

//=========================================================================
VkAllocationCallbacks *get_Allocator(const VkAllocationCallbacks *pAllocator) {
    if (pAllocator == NULL) {
        return NULL;
    }

    std::unordered_map<const void *, VkAllocationCallbacks>::iterator iter = s_trimAllocatorMap.find(pAllocator);
    assert(iter != s_trimAllocatorMap.end());
    VkAllocationCallbacks *pStoredAllocator = &(iter->second);
    return pStoredAllocator;
}

//=========================================================================
bool IsMemoryDeviceOnly(VkDeviceMemory memory) {
    ObjectInfo *pInfo = get_DeviceMemory_objectInfo(memory);
    assert(pInfo != NULL);
    if (pInfo == NULL) return false;

    // Is device only if it has VK_MEMORY_PROPERTY_DEVICE_LOCAL and not
    // VK_MEMORY_PROPERTY_HOST_VISIBLE
    bool isDeviceOnly =
        (pInfo->ObjectInfo.DeviceMemory.propertyFlags &
         (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    return isDeviceOnly;
}

//=========================================================================
VkMemoryPropertyFlags LookUpMemoryProperties(VkDevice device, uint32_t memoryTypeIndex) {
    assert(memoryTypeIndex < VK_MAX_MEMORY_TYPES);
    if (memoryTypeIndex >= VK_MAX_MEMORY_TYPES) return false;

    ObjectInfo *pDeviceInfo = get_Device_objectInfo(device);
    assert(pDeviceInfo != NULL);
    if (pDeviceInfo == NULL) return false;

    VkPhysicalDevice physicalDevice = pDeviceInfo->belongsToPhysicalDevice;
    assert(physicalDevice != VK_NULL_HANDLE);
    if (physicalDevice == VK_NULL_HANDLE) return false;

    ObjectInfo *pInfo = get_PhysicalDevice_objectInfo(physicalDevice);
    assert(pInfo != NULL);
    if (pInfo == NULL) return false;

    if (memoryTypeIndex >= pInfo->ObjectInfo.PhysicalDevice.physicalDeviceMemoryProperties.memoryTypeCount) return false;

    return pInfo->ObjectInfo.PhysicalDevice.physicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags;
}

//=========================================================================
uint32_t FindMemoryTypeIndex(VkDevice device, uint32_t memoryTypeBits, VkMemoryPropertyFlags propertyFlags) {
    VkPhysicalDevice physicalDevice = get_Device_objectInfo(device)->belongsToPhysicalDevice;
    assert(physicalDevice != VK_NULL_HANDLE);
    if (physicalDevice == VK_NULL_HANDLE) return 0;

    ObjectInfo *pInfo = get_PhysicalDevice_objectInfo(physicalDevice);
    assert(pInfo != NULL);
    for (uint32_t i = 0; i < pInfo->ObjectInfo.PhysicalDevice.physicalDeviceMemoryProperties.memoryTypeCount; i++) {
        if ((memoryTypeBits & (1 << i)) &&
            ((pInfo->ObjectInfo.PhysicalDevice.physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & propertyFlags) ==
             propertyFlags))
            return i;
    }
    // default to 0 if a more specific match is not found
    return 0;
}

//=========================================================================
VkImageAspectFlags getImageAspectFromFormat(VkFormat format) {
    VkImageAspectFlags aspectMask;
    switch (format) {
        case VK_FORMAT_D16_UNORM:
        case VK_FORMAT_D32_SFLOAT:
        case VK_FORMAT_X8_D24_UNORM_PACK32:
            aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            break;
        case VK_FORMAT_S8_UINT:
            aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
            break;
        case VK_FORMAT_D16_UNORM_S8_UINT:
        case VK_FORMAT_D24_UNORM_S8_UINT:
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
            break;
        default:
            aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            break;
    }

    return aspectMask;
}

//=========================================================================
// Find existing trim-specific CommandPool from the Device, or
// create a new one.
//=========================================================================
VkCommandPool getCommandPoolFromDevice(VkDevice device, uint32_t queueFamilyIndex = 0) {
    assert(device != VK_NULL_HANDLE);

    if (queueFamilyIndex == VK_QUEUE_FAMILY_IGNORED) {
        queueFamilyIndex = 0;
    }

    auto deviceIter = s_deviceToCommandPoolMap.find(device);
    if (deviceIter == s_deviceToCommandPoolMap.end()) {
        // need to add device to map
        std::unordered_map<uint32_t, VkCommandPool> queueIndexToCommandPoolMap;
        s_deviceToCommandPoolMap[device] = queueIndexToCommandPoolMap;

        deviceIter = s_deviceToCommandPoolMap.find(device);
    }

    auto queueFamilyIter = deviceIter->second.find(queueFamilyIndex);
    if (queueFamilyIter == deviceIter->second.end()) {
        // create a new command pool on the device
        VkCommandPoolCreateInfo cmdPoolCreateInfo;
        cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cmdPoolCreateInfo.pNext = NULL;
        cmdPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
        cmdPoolCreateInfo.flags = 0;

        VkCommandPool commandPool = VK_NULL_HANDLE;
        VkResult result = mdd(device)->devTable.CreateCommandPool(device, &cmdPoolCreateInfo, NULL, &commandPool);
        assert(result == VK_SUCCESS);
        if (result == VK_SUCCESS) {
            s_deviceToCommandPoolMap[device][queueFamilyIndex] = commandPool;
        }

        queueFamilyIter = deviceIter->second.find(queueFamilyIndex);
    }

    return queueFamilyIter->second;
}

//=========================================================================
// Find existing trim-specific CommandBuffer from the Device, or
// create a new one.
//=========================================================================
VkCommandBuffer getCommandBufferFromDevice(VkDevice device, VkCommandPool commandPool = VK_NULL_HANDLE,
                                           uint32_t queueFamilyIndex = 0) {
    assert(device != VK_NULL_HANDLE);

    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    if (s_deviceToCommandBufferMap.find(device) == s_deviceToCommandBufferMap.end()) {
        if (commandPool == VK_NULL_HANDLE) {
            commandPool = getCommandPoolFromDevice(device, queueFamilyIndex);
        }

        // allocate a new command buffer on the device
        VkCommandBufferAllocateInfo allocateInfo;
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.pNext = NULL;
        allocateInfo.commandPool = commandPool;
        allocateInfo.level = (VkCommandBufferLevel)VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = 1;

        VkResult result = mdd(device)->devTable.AllocateCommandBuffers(device, &allocateInfo, &commandBuffer);
        assert(result == VK_SUCCESS);
        if (result == VK_SUCCESS) {
            s_deviceToCommandBufferMap[device] = commandBuffer;
        }

        // Because this commandBuffer was not allocated through the loader's
        // trampoile function, we need to assign the dispatch table here
        *(void **)commandBuffer = *(void **)device;
    } else {
        commandBuffer = s_deviceToCommandBufferMap[device];

        // by Doc, "Executable command buffers can be submitted, reset, or
        // recorded to another command buffer.", the command buffer here
        // will be reused to record command, reset is needed to make it
        // come back to initial state. Directly using without reset is
        // not supported by Doc, for the target title, vkBeginCommandBuffer
        // return fail can be found on specific hardware and driver.
        mdd(device)->devTable.ResetCommandBuffer(commandBuffer, 0);
    }

    return commandBuffer;
}

//=========================================================================
StagingInfo createStagingBuffer(VkDevice device, VkCommandPool commandPool, VkCommandBuffer commandBuffer,
                                uint32_t queueFamilyIndex, VkDeviceSize size) {
    StagingInfo stagingInfo = {};

    stagingInfo.commandPool = commandPool;
    stagingInfo.commandBuffer = commandBuffer;

    VkQueue queue = trim::get_DeviceQueue(device, queueFamilyIndex, 0);
    stagingInfo.queue = queue;

    stagingInfo.bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    stagingInfo.bufferCreateInfo.pNext = NULL;
    stagingInfo.bufferCreateInfo.flags = 0;
    stagingInfo.bufferCreateInfo.size = size;
    stagingInfo.bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    stagingInfo.bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    stagingInfo.bufferCreateInfo.queueFamilyIndexCount = 0;
    stagingInfo.bufferCreateInfo.pQueueFamilyIndices = NULL;

    mdd(device)->devTable.CreateBuffer(device, &stagingInfo.bufferCreateInfo, NULL, &stagingInfo.buffer);

    mdd(device)->devTable.GetBufferMemoryRequirements(device, stagingInfo.buffer, &stagingInfo.bufferMemoryRequirements);

    stagingInfo.memoryAllocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    stagingInfo.memoryAllocationInfo.pNext = NULL;
    stagingInfo.memoryAllocationInfo.allocationSize = stagingInfo.bufferMemoryRequirements.size;
    stagingInfo.memoryAllocationInfo.memoryTypeIndex =
        FindMemoryTypeIndex(device, stagingInfo.bufferMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    mdd(device)->devTable.AllocateMemory(device, &stagingInfo.memoryAllocationInfo, nullptr, &stagingInfo.memory);

    mdd(device)->devTable.BindBufferMemory(device, stagingInfo.buffer, stagingInfo.memory, 0);

    return stagingInfo;
}

//=========================================================================
bool generateCreateStagingBuffer(VkDevice device, StagingInfo stagingInfo) {
    // check for valid usage of memory allocation:
    // validate memory type index and allocation size before create memory allocation.
    ObjectInfo *device_object_info = get_Device_objectInfo(device);
    VkPhysicalDevice physical_device = device_object_info->belongsToPhysicalDevice;
    VkPhysicalDeviceMemoryProperties memory_properties;
    mid(physical_device)->instTable.GetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

    uint32_t memory_type_index = stagingInfo.memoryAllocationInfo.memoryTypeIndex;
    VkDeviceSize allocation_size = stagingInfo.memoryAllocationInfo.allocationSize;
    if (memory_type_index > memory_properties.memoryTypeCount) {
        return false;
    }

    if (allocation_size > memory_properties.memoryHeaps[memory_type_index].size) {
        return false;
    }

    // generate create buffer and memory allocation packets
    vktrace_trace_packet_header *pHeader =
        generate::vkCreateBuffer(false, device, &stagingInfo.bufferCreateInfo, NULL, &stagingInfo.buffer);
    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
    vktrace_delete_trace_packet(&pHeader);

    pHeader = generate::vkGetBufferMemoryRequirements(false, device, stagingInfo.buffer, &stagingInfo.bufferMemoryRequirements);
    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
    vktrace_delete_trace_packet(&pHeader);

    pHeader = generate::vkAllocateMemory(false, device, &stagingInfo.memoryAllocationInfo, NULL, &stagingInfo.memory);
    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
    vktrace_delete_trace_packet(&pHeader);

    // bind staging buffer to staging memory
    pHeader = generate::vkBindBufferMemory(false, device, stagingInfo.buffer, stagingInfo.memory, 0);
    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
    vktrace_delete_trace_packet(&pHeader);

    return true;
}

//=========================================================================
void generateDestroyStagingBuffer(VkDevice device, StagingInfo stagingInfo) {
    // delete staging buffer
    vktrace_trace_packet_header *pHeader = generate::vkDestroyBuffer(false, device, stagingInfo.buffer, NULL);
    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
    vktrace_delete_trace_packet(&pHeader);

    // free memory
    pHeader = generate::vkFreeMemory(false, device, stagingInfo.memory, NULL);
    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
    vktrace_delete_trace_packet(&pHeader);
}

//=========================================================================
void transitionImage(VkDevice device, VkCommandBuffer commandBuffer, VkImage image, VkAccessFlags srcAccessMask,
                     VkAccessFlags dstAccessMask, uint32_t queueFamilyIndex, VkImageLayout oldLayout, VkImageLayout newLayout,
                     VkImageAspectFlags aspectMask, uint32_t layerCount, uint32_t mipLevels) {
    // Create a pipeline barrier to make it host readable
    VkImageMemoryBarrier imageMemoryBarrier;
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = NULL;
    imageMemoryBarrier.srcAccessMask = srcAccessMask;
    imageMemoryBarrier.dstAccessMask = dstAccessMask;
    imageMemoryBarrier.oldLayout = oldLayout;
    imageMemoryBarrier.newLayout = newLayout;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
    imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
    imageMemoryBarrier.subresourceRange.layerCount = layerCount;
    imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
    imageMemoryBarrier.subresourceRange.levelCount = mipLevels;
    imageMemoryBarrier.srcQueueFamilyIndex = queueFamilyIndex;
    imageMemoryBarrier.dstQueueFamilyIndex = queueFamilyIndex;

    mdd(device)->devTable.CmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0,
                                             0, NULL, 0, NULL, 1, &imageMemoryBarrier);
};

//=========================================================================
void generateTransitionImage(VkDevice device, VkCommandBuffer commandBuffer, VkImage image, VkAccessFlags srcAccessMask,
                             VkAccessFlags dstAccessMask, uint32_t queueFamilyIndex, VkImageLayout oldLayout,
                             VkImageLayout newLayout, VkImageAspectFlags aspectMask, uint32_t layerCount, uint32_t mipLevels) {
    // Create a pipeline barrier to make it host readable
    VkImageMemoryBarrier imageMemoryBarrier;
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = NULL;
    imageMemoryBarrier.srcAccessMask = srcAccessMask;
    imageMemoryBarrier.dstAccessMask = dstAccessMask;
    imageMemoryBarrier.oldLayout = oldLayout;
    imageMemoryBarrier.newLayout = newLayout;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
    imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
    imageMemoryBarrier.subresourceRange.layerCount = layerCount;
    imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
    imageMemoryBarrier.subresourceRange.levelCount = mipLevels;
    imageMemoryBarrier.srcQueueFamilyIndex = queueFamilyIndex;
    imageMemoryBarrier.dstQueueFamilyIndex = queueFamilyIndex;

    vktrace_trace_packet_header *pHeader =
        generate::vkCmdPipelineBarrier(false, commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                       0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier);
    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
    vktrace_delete_trace_packet(&pHeader);
};

//=========================================================================
void transitionBuffer(VkDevice device, VkCommandBuffer commandBuffer, VkBuffer buffer, VkAccessFlags srcAccessMask,
                      VkAccessFlags dstAccessMask, VkDeviceSize offset, VkDeviceSize size, bool largeScopeBarrier = false) {
    // Create a pipeline barrier to make it host readable
    VkBufferMemoryBarrier bufferMemoryBarrier;
    bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    bufferMemoryBarrier.pNext = NULL;
    bufferMemoryBarrier.srcAccessMask = srcAccessMask;
    bufferMemoryBarrier.dstAccessMask = dstAccessMask;
    bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferMemoryBarrier.buffer = buffer;
    bufferMemoryBarrier.offset = offset;
    bufferMemoryBarrier.size = size;
    if (largeScopeBarrier) {
        // when trim copy buffer to a staging buffer, that buffer might be used by
        // target app with any usage at that time, so we need make sure memory
        // synchronization on that buffer for any action.
        // so here expand the barrier scope to VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM.
        mdd(device)->devTable.CmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM,
                                                 VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM, 0, 0, NULL, 1, &bufferMemoryBarrier, 0,
                                                 NULL);
    } else {
        mdd(device)->devTable.CmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                                 VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 1, &bufferMemoryBarrier, 0, NULL);
    }
};

//=========================================================================
void generateTransitionBuffer(VkDevice device, VkCommandBuffer commandBuffer, VkBuffer buffer, VkAccessFlags srcAccessMask,
                              VkAccessFlags dstAccessMask, VkDeviceSize offset, VkDeviceSize size) {
    // Create a pipeline barrier to make it host readable
    VkBufferMemoryBarrier bufferMemoryBarrier;
    bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    bufferMemoryBarrier.pNext = NULL;
    bufferMemoryBarrier.srcAccessMask = srcAccessMask;
    bufferMemoryBarrier.dstAccessMask = dstAccessMask;
    bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferMemoryBarrier.buffer = buffer;
    bufferMemoryBarrier.offset = offset;
    bufferMemoryBarrier.size = size;

    vktrace_trace_packet_header *pHeader =
        generate::vkCmdPipelineBarrier(false, commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                       0, 0, NULL, 1, &bufferMemoryBarrier, 0, NULL);
    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
    vktrace_delete_trace_packet(&pHeader);
};

//=========================================================================
// \param isMappedOffset If the buffer is currently mapped by the application,
// this is the offset that was used during that call to vkMapMemory.
// \param pIsMappedAddress If the buffer is currently mapped by the application,
// this is the address of the buffer that was returned by vkMapMemory.
//=========================================================================
void generateMapUnmap(bool makeCalls, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size,
                      VkDeviceSize isMappedOffset, void *pIsMappedAddress, vktrace_trace_packet_header **ppMapMemoryPacket,
                      vktrace_trace_packet_header **ppUnmapMemoryPacket) {
    assert(ppMapMemoryPacket != NULL);
    assert(ppUnmapMemoryPacket != NULL);

    uint32_t flags = 0;

    vktrace_trace_packet_header *pMapMemory =
        generate::vkMapMemory(makeCalls, device, memory, offset, size, flags, &pIsMappedAddress);

    if (pIsMappedAddress == NULL) {
        // if the mapped address is still NULL, that means the map failed for
        // some reason.
        // Since it failed, we'll just delete the packet and won't create an
        // unmap for it.
        vktrace_delete_trace_packet(&pMapMemory);
    } else {
        *ppMapMemoryPacket = trim::copy_packet(pMapMemory);
        vktrace_delete_trace_packet(&pMapMemory);

        // By creating the packet for UnmapMemory, we'll be adding the pData
        // buffer to it, which inherently copies it.

        void *bufferAddress = NULL;
        if (makeCalls) {
            // No need to adjust the pointer when the memory is actually mapped in generate::vkMapMemory() because the
            // pIsMappedAddress is re-populated to point to the beginning of the image memory.
            bufferAddress = pIsMappedAddress;
        } else {
            // Need to adjust the pointer to ensure it points to the beginning of
            // the image memory, which may NOT be
            // the same as the offset of the mapped address.
            bufferAddress = (BYTE *)pIsMappedAddress + (offset - isMappedOffset);
        }

        // Actually unmap the memory if it wasn't already mapped by the
        // application
        vktrace_trace_packet_header *pUnmapMemory = generate::vkUnmapMemory(makeCalls, size, bufferAddress, device, memory);
        *ppUnmapMemoryPacket = trim::copy_packet(pUnmapMemory);
        vktrace_delete_trace_packet(&pUnmapMemory);
    }
}

//=============================================================================
// Use this to snapshot the global state tracker at the start of the trim
// frames.
//=============================================================================
void snapshot_state_tracker() {
    // TODO: split this function into multiple functions.
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimStateTrackerSnapshot = s_trimGlobalStateTracker;

    getTrimMaxBatchCmdCountOption();

    //
    // Copying all the buffers is a length process, it include the following
    // sub-processes:
    //
    // for (any image in all tracked images)
    // {
    //    1a) Transition the image into host - readable state.
    //    2a) Map, copy, unmap the image.
    //    3a) Transition the images back to their previous state.
    // }
    //
    // for (any buffer in all tracked buffers)
    // {
    //    1b) Transition the buffers into host - readable state.
    //    2b) Map, copy, unmap the buffer.
    //    3b) Transition the buffer back to their previous state.
    // }
    //
    // 4) Destroy the command pools, command buffers, and fences.
    // Note: command pools, command buffers maps generated will be used
    // in generating packets later then only destroyed.
    //
    // Please note: the above sub-process order arrangement include some
    // consideration about driver limitation:
    //
    // Some driver has limitation on the max GPU memory allocations. For
    // some title with heavily sub-allocation behavior, the staging memory
    // allocations needed by trim will be a large number, the following
    // sub-process order minimize the active GPU memory allocations needed
    // by trim at same time, and avoid the allocations (needed by trim and
    // by the title itself) beyond driver limitation. Otherwise, it cause
    // some title hang problem due to fail to allocate memory.

    // a) Dump all images in batches, the process include the following sub-process:
    //    1a) Transition the image into host - readable state.
    //    2a) Map, copy, unmap the image.
    //    3a) Transition the image back to their previous state.
    //
    uint32_t batchIterCount = 0;
    auto imageIter = s_trimStateTrackerSnapshot.createdImages.begin();
    while (imageIter != s_trimStateTrackerSnapshot.createdImages.end()) {
        auto currentBatchImgIter = imageIter;
        batchIterCount = 0;

        // Running the following sub-process in batch:
        // Begin the command buffer for the image and
        // 1a) Transition image into host-readable state.
        // End the command buffer for the image and queue submit
        // Wait for the queue idle
        // 2a) Map, copy, unmap the image.
        for (; imageIter != s_trimStateTrackerSnapshot.createdImages.end() && batchIterCount < g_trimMaxBatchCmdCount;
             imageIter++, batchIterCount++) {
            VkDevice device = imageIter->second.belongsToDevice;
            VkImage image = imageIter->first;

            if ((imageIter->second.ObjectInfo.Image.memorySize != 0) && (device != VK_NULL_HANDLE)) {
                // If the memorysize is zero, it mean the image is not bound to any
                // memory so far, it might be just created when starting to trim.
                // for such case, what we need to do is recreating the image in
                // playback without copy its content to host side, it doesn't
                // has any content now and the title might set its content after
                // the trim starting. So skip the following process.
                // Some target title belong to such case, the following process
                // cause the title running crash during tracing because
                // the following part of loop suppose the image is bound to
                // memory so memorysize is not zero.
                // If device is VK_NULL_HANDLE, this is likely a swapchain image
                // which we haven't associated a device to, just skip over it.

                // 1a) Transition the image into host-readable state.

                uint32_t queueFamilyIndex = imageIter->second.ObjectInfo.Image.queueFamilyIndex;

                if (imageIter->second.ObjectInfo.Image.sharingMode == VK_SHARING_MODE_CONCURRENT) {
                    queueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                }

                VkCommandPool commandPool = getCommandPoolFromDevice(device, queueFamilyIndex);
                VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device, commandPool);

                // Begin the command buffer
                VkCommandBufferBeginInfo commandBufferBeginInfo;
                commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                commandBufferBeginInfo.pNext = NULL;
                commandBufferBeginInfo.pInheritanceInfo = NULL;
                commandBufferBeginInfo.flags = 0;
                VkResult result = mdd(device)->devTable.BeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
                assert(result == VK_SUCCESS);
                if (result != VK_SUCCESS) continue;

                if (imageIter->second.ObjectInfo.Image.needsStagingBuffer) {
                    StagingInfo stagingInfo = createStagingBuffer(
                        device, commandPool, commandBuffer, (queueFamilyIndex == VK_QUEUE_FAMILY_IGNORED) ? 0 : queueFamilyIndex,
                        std::max(getImageSize(image), imageIter->second.ObjectInfo.Image.memorySize));

                    // From Docs: srcImage must have a sample count equal to
                    // VK_SAMPLE_COUNT_1_BIT
                    // From Docs: srcImage must have been created with
                    // VK_IMAGE_USAGE_TRANSFER_SRC_BIT usage flag

                    // Copy from device_local image to host_visible buffer
                    bool callGetImageSubresourceLayoutApi = (false == getImageSubResourceSizes(image, nullptr));
                    VkImageAspectFlags aspectMask = imageIter->second.ObjectInfo.Image.aspectMask;
                    if (aspectMask == (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT)) {
                        stagingInfo.imageCopyRegions.reserve(2);

                        // First depth, then stencil
                        VkImageSubresource sub;
                        sub.arrayLayer = 0;
                        sub.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
                        sub.mipLevel = 0;
                        {
                            VkSubresourceLayout layout;
                            if (callGetImageSubresourceLayoutApi) {
                                mdd(device)->devTable.GetImageSubresourceLayout(device, image, &sub, &layout);
                            } else {
                                layout.offset = getImageSubResourceOffset(image, 0);
                            }

                            VkBufferImageCopy copyRegion = {};

                            copyRegion.bufferRowLength = 0;
                            copyRegion.bufferImageHeight = 0;
                            // On some platform, originally set to layout.rowPitch and layout.arrayPitch
                            // cause write outside of staging buffer memory size and hang at following
                            // queue submission in other frames after finish trim starting process when
                            // trim some titles.
                            //
                            // Here we set bufferRowLength and bufferImageHeight to 0 make the image
                            // copy to be tightly packed according to the imageExtent, the change fix
                            // the above problem.
                            //
                            // Although bufferRowLength,bufferImageHeight can be set to greater than
                            // the width and height member of imageExtent, but because we allocate memory
                            // for the staging buffer by image memory size and here we copy whole image,
                            // so greater than imageExtent take a risk that the copy beyond the staging
                            // buffer memory size.

                            copyRegion.bufferOffset = layout.offset;
                            copyRegion.imageExtent.depth = imageIter->second.ObjectInfo.Image.extent.depth;
                            copyRegion.imageExtent.width = imageIter->second.ObjectInfo.Image.extent.width;
                            copyRegion.imageExtent.height = imageIter->second.ObjectInfo.Image.extent.height;
                            copyRegion.imageOffset.x = 0;
                            copyRegion.imageOffset.y = 0;
                            copyRegion.imageOffset.z = 0;
                            copyRegion.imageSubresource.aspectMask = sub.aspectMask;
                            copyRegion.imageSubresource.baseArrayLayer = 0;
                            copyRegion.imageSubresource.layerCount = imageIter->second.ObjectInfo.Image.arrayLayers;
                            copyRegion.imageSubresource.mipLevel = 0;

                            stagingInfo.imageCopyRegions.push_back(copyRegion);
                        }

                        sub.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
                        {
                            VkSubresourceLayout layout;
                            if (callGetImageSubresourceLayoutApi) {
                                mdd(device)->devTable.GetImageSubresourceLayout(device, image, &sub, &layout);
                            } else {
                                layout.offset = getImageSubResourceOffset(image, 0);
                            }

                            VkBufferImageCopy copyRegion;

                            copyRegion.bufferRowLength = 0;
                            copyRegion.bufferImageHeight = 0;
                            // set bufferRowLength and bufferImageHeight to 0 make the image
                            // copy to be tightly packed according to the imageExtent.

                            copyRegion.bufferOffset = layout.offset;
                            copyRegion.imageExtent.depth = imageIter->second.ObjectInfo.Image.extent.depth;
                            copyRegion.imageExtent.width = imageIter->second.ObjectInfo.Image.extent.width;
                            copyRegion.imageExtent.height = imageIter->second.ObjectInfo.Image.extent.height;
                            copyRegion.imageOffset.x = 0;
                            copyRegion.imageOffset.y = 0;
                            copyRegion.imageOffset.z = 0;
                            copyRegion.imageSubresource.aspectMask = sub.aspectMask;
                            copyRegion.imageSubresource.baseArrayLayer = 0;
                            copyRegion.imageSubresource.layerCount = imageIter->second.ObjectInfo.Image.arrayLayers;
                            copyRegion.imageSubresource.mipLevel = 0;

                            stagingInfo.imageCopyRegions.push_back(copyRegion);
                        }
                    } else {
                        VkImageSubresource sub;
                        sub.arrayLayer = 0;
                        sub.aspectMask = aspectMask;
                        sub.mipLevel = 0;

                        // need to make a VkBufferImageCopy for each mip level
                        stagingInfo.imageCopyRegions.reserve(imageIter->second.ObjectInfo.Image.mipLevels);
                        for (uint32_t i = 0; i < imageIter->second.ObjectInfo.Image.mipLevels; i++) {
                            VkSubresourceLayout lay;
                            sub.mipLevel = i;
                            if (callGetImageSubresourceLayoutApi) {
                                mdd(device)->devTable.GetImageSubresourceLayout(device, image, &sub, &lay);
                            } else {
                                lay.offset = getImageSubResourceOffset(image, i);
                            }

                            VkBufferImageCopy copyRegion;
                            copyRegion.bufferRowLength = 0;    //< tightly packed texels
                            copyRegion.bufferImageHeight = 0;  //< tightly packed texels
                            copyRegion.bufferOffset = lay.offset;

                            if (imageIter->second.ObjectInfo.Image.imageType == VK_IMAGE_TYPE_3D) {
                                copyRegion.imageExtent.depth = imageIter->second.ObjectInfo.Image.extent.depth >> i;
                            } else {
                                copyRegion.imageExtent.depth = 1;
                            }

                            copyRegion.imageExtent.width = (imageIter->second.ObjectInfo.Image.extent.width >> i);

                            if (imageIter->second.ObjectInfo.Image.imageType != VK_IMAGE_TYPE_1D) {
                                copyRegion.imageExtent.height = (imageIter->second.ObjectInfo.Image.extent.height >> i);
                            } else {
                                copyRegion.imageExtent.height = 1;
                            }

                            copyRegion.imageOffset.x = 0;
                            copyRegion.imageOffset.y = 0;
                            copyRegion.imageOffset.z = 0;
                            copyRegion.imageSubresource.aspectMask = aspectMask;
                            copyRegion.imageSubresource.baseArrayLayer = 0;
                            copyRegion.imageSubresource.layerCount = imageIter->second.ObjectInfo.Image.arrayLayers;
                            copyRegion.imageSubresource.mipLevel = i;

                            stagingInfo.imageCopyRegions.push_back(copyRegion);
                        }
                    }

                    // From docs: srcImageLayout must specify the layout of the image
                    // subresources of srcImage specified in pRegions at the time this
                    // command is executed on a VkDevice
                    // From docs: srcImageLayout must be either of
                    // VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL or VK_IMAGE_LAYOUT_GENERAL
                    VkImageLayout srcImageLayout = imageIter->second.ObjectInfo.Image.mostRecentLayout;

                    // Transition the image so that it's in an optimal transfer source
                    // layout.
                    transitionImage(device, commandBuffer, image, imageIter->second.ObjectInfo.Image.accessFlags,
                                    imageIter->second.ObjectInfo.Image.accessFlags, queueFamilyIndex, srcImageLayout,
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, aspectMask,
                                    imageIter->second.ObjectInfo.Image.arrayLayers, imageIter->second.ObjectInfo.Image.mipLevels);

                    mdd(device)->devTable.CmdCopyImageToBuffer(
                        commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, stagingInfo.buffer,
                        static_cast<uint32_t>(stagingInfo.imageCopyRegions.size()), stagingInfo.imageCopyRegions.data());

                    // save the staging info for later
                    s_imageToStagedInfoMap[image] = stagingInfo;

                    // now that the image data is in a host-readable buffer
                    // transition image back to it's previous layout
                    transitionImage(device, commandBuffer, image, imageIter->second.ObjectInfo.Image.accessFlags,
                                    imageIter->second.ObjectInfo.Image.accessFlags, queueFamilyIndex,
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, srcImageLayout, aspectMask,
                                    imageIter->second.ObjectInfo.Image.arrayLayers, imageIter->second.ObjectInfo.Image.mipLevels);
                } else {
                    // Create a pipeline barrier to make it host readable
                    transitionImage(device, commandBuffer, image, imageIter->second.ObjectInfo.Image.accessFlags,
                                    VK_ACCESS_HOST_READ_BIT, queueFamilyIndex, imageIter->second.ObjectInfo.Image.mostRecentLayout,
                                    imageIter->second.ObjectInfo.Image.mostRecentLayout,
                                    imageIter->second.ObjectInfo.Image.aspectMask, imageIter->second.ObjectInfo.Image.arrayLayers,
                                    imageIter->second.ObjectInfo.Image.mipLevels);
                }

                // End the CommandBuffer
                mdd(device)->devTable.EndCommandBuffer(commandBuffer);

                // Submit the command buffer
                VkSubmitInfo submitInfo;
                submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submitInfo.pNext = NULL;
                submitInfo.waitSemaphoreCount = 0;
                submitInfo.pWaitSemaphores = NULL;
                submitInfo.pWaitDstStageMask = NULL;
                submitInfo.commandBufferCount = 1;
                submitInfo.pCommandBuffers = &commandBuffer;
                submitInfo.signalSemaphoreCount = 0;
                submitInfo.pSignalSemaphores = NULL;

                // Submit the queue
                VkQueue queue = trim::get_DeviceQueue(device, queueFamilyIndex, 0);
                mdd(device)->devTable.QueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

                // Wait for the queue idle
                VkResult waitResult = mdd(device)->devTable.QueueWaitIdle(queue);
                assert(waitResult == VK_SUCCESS);
                if (waitResult != VK_SUCCESS) continue;

                // 2a) Map, copy, unmap the image
                VkDeviceMemory memory = imageIter->second.ObjectInfo.Image.memory;
                VkDeviceSize offset = imageIter->second.ObjectInfo.Image.memoryOffset;
                VkDeviceSize size = imageIter->second.ObjectInfo.Image.memorySize;

                if (imageIter->second.ObjectInfo.Image.needsStagingBuffer) {
                    // Note that the staged memory object won't be in the state tracker,
                    // so we want to swap out the buffer and memory
                    // that will be mapped / unmapped.
                    StagingInfo staged = s_imageToStagedInfoMap[image];
                    memory = staged.memory;
                    offset = 0;

                    void *mappedAddress = NULL;

                    if (size != 0) {
                        generateMapUnmap(true, device, memory, offset, size, 0, mappedAddress,
                                         &imageIter->second.ObjectInfo.Image.pMapMemoryPacket,
                                         &imageIter->second.ObjectInfo.Image.pUnmapMemoryPacket);
                    }
                } else {
                    auto memoryIter = s_trimStateTrackerSnapshot.createdDeviceMemorys.find(memory);

                    if (memoryIter != s_trimStateTrackerSnapshot.createdDeviceMemorys.end()) {
                        void *mappedAddress = memoryIter->second.ObjectInfo.DeviceMemory.mappedAddress;
                        VkDeviceSize mappedOffset = memoryIter->second.ObjectInfo.DeviceMemory.mappedOffset;
                        VkDeviceSize mappedSize = memoryIter->second.ObjectInfo.DeviceMemory.mappedSize;

                        if (size != 0) {
                            // actually map the memory if it was not already mapped.
                            bool bAlreadyMapped = (mappedAddress != NULL);
                            if (bAlreadyMapped) {
                                // I imagine there could be a scenario where the
                                // application has persistently
                                // mapped PART of the memory, which may not contain the
                                // image that we're trying to copy right now.
                                // In that case, there will be errors due to this code.
                                // We know the range of memory that is mapped
                                // so we should be able to confirm whether or not we get
                                // into this situation.
                                bAlreadyMapped = (offset >= mappedOffset && (offset + size) <= (mappedOffset + mappedSize));
                            }

                            generateMapUnmap(!bAlreadyMapped, device, memory, offset, size, 0, mappedAddress,
                                             &imageIter->second.ObjectInfo.Image.pMapMemoryPacket,
                                             &imageIter->second.ObjectInfo.Image.pUnmapMemoryPacket);
                        }
                    }
                }
            }
        }

        // Running the following sub-process in batch:
        // 3a) Transition the image back to the previous state.
        // Begin the command buffer for the image
        // Destroy buffers and free memories for staging images and restore non-staging images to previous state
        // End the command buffer and queue submit then wait for idle
        imageIter = currentBatchImgIter;
        for (uint32_t itrCount = 0; imageIter != s_trimStateTrackerSnapshot.createdImages.end() && itrCount < batchIterCount;
             itrCount++, imageIter++) {
            VkDevice device = imageIter->second.belongsToDevice;
            VkImage image = imageIter->first;

            if ((imageIter->second.ObjectInfo.Image.memorySize != 0) && (device != VK_NULL_HANDLE)) {
                // If the memorysize is zero, it mean the image is not bound to any
                // memory so far, it might be just created when starting to trim.
                // for such case, what we need to do is recreating the image in
                // playback without copy its content to host side, it doesn't
                // has any content now and the title might set its content after
                // the trim starting. So skip the following process.
                // Some target title belong to such case, the following process
                // cause the title running crash during tracing because
                // the following part of loop suppose the image is bound to
                // memory so memorysize is not zero.
                // If device is VK_NULL_HANDLE, this is likely a swapchain image
                // which we haven't associated a device to, just skip over it.

                uint32_t queueFamilyIndex = imageIter->second.ObjectInfo.Image.queueFamilyIndex;

                if (imageIter->second.ObjectInfo.Image.sharingMode == VK_SHARING_MODE_CONCURRENT) {
                    queueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                }

                VkCommandPool commandPool = getCommandPoolFromDevice(device, queueFamilyIndex);
                VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device, commandPool);

                // Begin the command buffer
                VkCommandBufferBeginInfo commandBufferBeginInfo;
                commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                commandBufferBeginInfo.pNext = NULL;
                commandBufferBeginInfo.pInheritanceInfo = NULL;
                commandBufferBeginInfo.flags = 0;
                VkResult result = mdd(device)->devTable.BeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
                assert(result == VK_SUCCESS);
                if (result != VK_SUCCESS) continue;

                // only need to restore the images that did NOT need a staging buffer
                if (imageIter->second.ObjectInfo.Image.needsStagingBuffer) {
                    // delete the staging objects
                    StagingInfo staged = s_imageToStagedInfoMap[image];
                    mdd(device)->devTable.DestroyBuffer(device, staged.buffer, NULL);
                    mdd(device)->devTable.FreeMemory(device, staged.memory, NULL);
                } else {
                    transitionImage(
                        device, commandBuffer, image, VK_ACCESS_HOST_READ_BIT, imageIter->second.ObjectInfo.Image.accessFlags,
                        queueFamilyIndex, imageIter->second.ObjectInfo.Image.mostRecentLayout,
                        imageIter->second.ObjectInfo.Image.mostRecentLayout, imageIter->second.ObjectInfo.Image.aspectMask,
                        imageIter->second.ObjectInfo.Image.arrayLayers, imageIter->second.ObjectInfo.Image.mipLevels);
                }

                // End the command buffer
                mdd(device)->devTable.EndCommandBuffer(commandBuffer);

                // Submit the command buffer
                VkSubmitInfo submitInfo;
                submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submitInfo.pNext = NULL;
                submitInfo.waitSemaphoreCount = 0;
                submitInfo.pWaitSemaphores = NULL;
                submitInfo.pWaitDstStageMask = NULL;
                submitInfo.commandBufferCount = 1;
                submitInfo.pCommandBuffers = &commandBuffer;
                submitInfo.signalSemaphoreCount = 0;
                submitInfo.pSignalSemaphores = NULL;

                // Submit the queue
                VkQueue queue = trim::get_DeviceQueue(device, queueFamilyIndex, 0);
                mdd(device)->devTable.QueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

                // Wait for the queue idle
                VkResult waitResult = mdd(device)->devTable.QueueWaitIdle(queue);
                assert(waitResult == VK_SUCCESS);
                if (waitResult != VK_SUCCESS) continue;
            }
        }
    }

    // b) Dump all buffers in batches, the process include the following sub-process:
    //    1b) Transition the buffer into host - readable state.
    //    2b) Map, copy, unmap the buffer.
    //    3b) Transition the buffer back to their previous state.
    batchIterCount = 0;
    auto bufferIter = s_trimStateTrackerSnapshot.createdBuffers.begin();
    while (bufferIter != s_trimStateTrackerSnapshot.createdBuffers.end()) {
        auto currentBatchBufIter = bufferIter;
        batchIterCount = 0;

        // Running the following sub-process in batch:
        // Begin the command buffer for the buffer and
        // 1a) Transition buffer into host-readable state.
        // End the command buffer for the buffer and queue submit
        // Wait for the queue idle
        // 2a) Map, copy, unmap the buffer.
        for (; bufferIter != s_trimStateTrackerSnapshot.createdBuffers.end() && batchIterCount < g_trimMaxBatchCmdCount;
             bufferIter++, batchIterCount++) {
            VkDevice device = bufferIter->second.belongsToDevice;
            VkBuffer buffer = static_cast<VkBuffer>(bufferIter->first);

            if ((bufferIter->second.ObjectInfo.Buffer.pBindBufferMemoryPacket != nullptr) &&
                (bufferIter->second.ObjectInfo.Buffer.size != 0)) {
                // Similiar with image handling, skip the following process
                // if the buffer is not bound to any memory.

                // 1b) Transition the buffer into host-readable state.

                uint32_t queueFamilyIndex = bufferIter->second.ObjectInfo.Buffer.queueFamilyIndex;

                VkCommandPool commandPool = getCommandPoolFromDevice(device, queueFamilyIndex);
                VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device, commandPool);

                // Begin the command buffer
                VkCommandBufferBeginInfo commandBufferBeginInfo;
                commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                commandBufferBeginInfo.pNext = NULL;
                commandBufferBeginInfo.pInheritanceInfo = NULL;
                commandBufferBeginInfo.flags = 0;
                VkResult result = mdd(device)->devTable.BeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
                assert(result == VK_SUCCESS);
                if (result != VK_SUCCESS) continue;

                // If the buffer needs a staging buffer, it's because it's on
                // DEVICE_LOCAL memory that is not HOST_VISIBLE.
                // So we have to create another buffer and memory that IS HOST_VISIBLE
                // so that we can copy the data
                // from the DEVICE_LOCAL memory into HOST_VISIBLE memory, then map /
                // unmap the HOST_VISIBLE memory object.
                // The staging info is kept so that we can generate similar calls in the
                // trace file in order to recreate
                // the DEVICE_LOCAL buffer.
                if (bufferIter->second.ObjectInfo.Buffer.needsStagingBuffer) {
                    StagingInfo stagingInfo = createStagingBuffer(device, commandPool, commandBuffer, queueFamilyIndex,
                                                                  bufferIter->second.ObjectInfo.Buffer.size);

                    // Copy from device_local buffer to host_visible buffer
                    stagingInfo.copyRegion.srcOffset = 0;
                    stagingInfo.copyRegion.dstOffset = 0;
                    stagingInfo.copyRegion.size = bufferIter->second.ObjectInfo.Buffer.size;

                    transitionBuffer(device, commandBuffer, buffer, VK_ACCESS_FLAG_BITS_MAX_ENUM, VK_ACCESS_TRANSFER_READ_BIT, 0,
                                     bufferIter->second.ObjectInfo.Buffer.size, true);
                    mdd(device)->devTable.CmdCopyBuffer(commandBuffer, buffer, stagingInfo.buffer, 1, &stagingInfo.copyRegion);
                    transitionBuffer(device, commandBuffer, buffer, VK_ACCESS_TRANSFER_READ_BIT,
                                     bufferIter->second.ObjectInfo.Buffer.accessFlags, 0, bufferIter->second.ObjectInfo.Buffer.size,
                                     true);

                    // save the staging info for later
                    s_bufferToStagedInfoMap[buffer] = stagingInfo;
                } else {
                    transitionBuffer(device, commandBuffer, buffer, bufferIter->second.ObjectInfo.Buffer.accessFlags,
                                     VK_ACCESS_HOST_READ_BIT, 0, bufferIter->second.ObjectInfo.Buffer.size);
                }

                // TODO: LESS THAN IDEAL TO END & SUBMIT these super small command buffers
                // End the CommandBuffer
                mdd(device)->devTable.EndCommandBuffer(commandBuffer);

                // now submit the command buffer
                VkSubmitInfo submitInfo;
                submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submitInfo.pNext = NULL;
                submitInfo.waitSemaphoreCount = 0;
                submitInfo.pWaitSemaphores = NULL;
                submitInfo.pWaitDstStageMask = NULL;
                submitInfo.commandBufferCount = 1;
                submitInfo.pCommandBuffers = &commandBuffer;
                submitInfo.signalSemaphoreCount = 0;
                submitInfo.pSignalSemaphores = NULL;

                // Submit the queue and wait for it to complete
                VkQueue queue = trim::get_DeviceQueue(device, queueFamilyIndex, 0);

                mdd(device)->devTable.QueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
                VkResult waitResult = mdd(device)->devTable.QueueWaitIdle(queue);
                assert(waitResult == VK_SUCCESS);
                if (waitResult != VK_SUCCESS) continue;

                // 2b) Map, copy, unmap the buffer.

                device = bufferIter->second.belongsToDevice;
                buffer = bufferIter->first;

                VkDeviceMemory memory = bufferIter->second.ObjectInfo.Buffer.memory;
                VkDeviceSize offset = bufferIter->second.ObjectInfo.Buffer.memoryOffset;
                VkDeviceSize size = bufferIter->second.ObjectInfo.Buffer.size;

                void *mappedAddress = NULL;
                VkDeviceSize mappedOffset = 0;
                VkDeviceSize mappedSize = 0;

                if (bufferIter->second.ObjectInfo.Buffer.needsStagingBuffer) {
                    // Note that the staged memory object won't be in the state tracker,
                    // so we want to swap out the buffer and memory
                    // that will be mapped / unmapped.
                    StagingInfo staged = s_bufferToStagedInfoMap[buffer];
                    buffer = staged.buffer;
                    memory = staged.memory;
                    offset = 0;
                } else {
                    auto memoryIter = s_trimStateTrackerSnapshot.createdDeviceMemorys.find(memory);
                    assert(memoryIter != s_trimStateTrackerSnapshot.createdDeviceMemorys.end());
                    if (memoryIter != s_trimStateTrackerSnapshot.createdDeviceMemorys.end()) {
                        mappedAddress = memoryIter->second.ObjectInfo.DeviceMemory.mappedAddress;
                        mappedOffset = memoryIter->second.ObjectInfo.DeviceMemory.mappedOffset;
                        mappedSize = memoryIter->second.ObjectInfo.DeviceMemory.mappedSize;
                    }
                }

                if (size != 0) {
                    // actually map the memory if it was not already mapped.
                    bool bAlreadyMapped = (mappedAddress != NULL);
                    if (bAlreadyMapped) {
                        // I imagine there could be a scenario where the application has
                        // persistently
                        // mapped PART of the memory, which may not contain the image
                        // that we're trying to copy right now.
                        // In that case, there will be errors due to this code. We know
                        // the range of memory that is mapped
                        // so we should be able to confirm whether or not we get into
                        // this situation.
                        bAlreadyMapped = (offset >= mappedOffset && (offset + size) <= (mappedOffset + mappedSize));
                    }

                    generateMapUnmap(!bAlreadyMapped, device, memory, offset, size, 0, mappedAddress,
                                     &bufferIter->second.ObjectInfo.Buffer.pMapMemoryPacket,
                                     &bufferIter->second.ObjectInfo.Buffer.pUnmapMemoryPacket);
                }
            }
        }

        // Running the following sub-process in batch:
        // 3a) Transition the buffer back to the previous state.
        // Begin the command buffer for the buffer
        // Destroy buffers and free memories for staging buffers and restore non-staging buffers to previous state
        // End the command buffer and queue submit then wait for idle
        bufferIter = currentBatchBufIter;
        for (uint32_t itrCount = 0; bufferIter != s_trimStateTrackerSnapshot.createdBuffers.end() && itrCount < batchIterCount;
             itrCount++, bufferIter++) {
            // 3b) Transition the buffer back to their previous state.

            VkDevice device = bufferIter->second.belongsToDevice;
            VkBuffer buffer = static_cast<VkBuffer>(bufferIter->first);

            uint32_t queueFamilyIndex = bufferIter->second.ObjectInfo.Buffer.queueFamilyIndex;

            VkCommandPool commandPool = getCommandPoolFromDevice(device, queueFamilyIndex);
            VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device, commandPool);

            // Begin the command buffer
            VkCommandBufferBeginInfo commandBufferBeginInfo;
            commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            commandBufferBeginInfo.pNext = NULL;
            commandBufferBeginInfo.pInheritanceInfo = NULL;
            commandBufferBeginInfo.flags = 0;
            VkResult result = mdd(device)->devTable.BeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
            assert(result == VK_SUCCESS);
            if (result != VK_SUCCESS) continue;

            if (bufferIter->second.ObjectInfo.Buffer.needsStagingBuffer) {
                // if this buffer had a staging buffer, then we only need to do
                // delete the staging objects
                StagingInfo staged = s_bufferToStagedInfoMap[buffer];
                mdd(device)->devTable.DestroyBuffer(device, staged.buffer, NULL);
                mdd(device)->devTable.FreeMemory(device, staged.memory, NULL);
            } else {
                transitionBuffer(device, commandBuffer, buffer, VK_ACCESS_HOST_READ_BIT,
                                 bufferIter->second.ObjectInfo.Buffer.accessFlags, 0, bufferIter->second.ObjectInfo.Buffer.size);
            }

            // End the CommandBuffer
            mdd(device)->devTable.EndCommandBuffer(commandBuffer);

            // now submit the command buffer
            VkSubmitInfo submitInfo;
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.pNext = NULL;
            submitInfo.waitSemaphoreCount = 0;
            submitInfo.pWaitSemaphores = NULL;
            submitInfo.pWaitDstStageMask = NULL;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;
            submitInfo.signalSemaphoreCount = 0;
            submitInfo.pSignalSemaphores = NULL;

            // Submit the queue
            VkQueue queue = trim::get_DeviceQueue(device, queueFamilyIndex, 0);
            mdd(device)->devTable.QueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

            // Wait for queue to complete
            VkResult waitResult = mdd(device)->devTable.QueueWaitIdle(queue);
            assert(waitResult == VK_SUCCESS);
            if (waitResult != VK_SUCCESS) continue;
        }
    }

    // 4) Destroy the command pools / command buffers and fences
    for (auto deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin();
         deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++) {
        VkDevice device = reinterpret_cast<VkDevice>(deviceIter->first);
        VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device);

        mdd(device)->devTable.ResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

        // command pools
        VkCommandPool commandPool = getCommandPoolFromDevice(device);
        mdd(device)->devTable.ResetCommandPool(device, commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
        mdd(device)->devTable.DestroyCommandPool(device, commandPool, NULL);
    }

    // Now: generate a vkMapMemory to recreate the persistently mapped buffers
    for (auto iter = s_trimStateTrackerSnapshot.createdDeviceMemorys.begin();
         iter != s_trimStateTrackerSnapshot.createdDeviceMemorys.end(); iter++) {
        // if the application still has this memory mapped, then we need to make
        // sure the trim trace file leaves it mapped, so let's generate one more
        // call to vkMapBuffer.
        bool bCurrentlyMapped = (iter->second.ObjectInfo.DeviceMemory.mappedAddress != NULL);
        if (bCurrentlyMapped) {
            VkDevice device = iter->second.belongsToDevice;
            VkDeviceMemory deviceMemory = iter->first;
            VkDeviceSize offset = 0;
            VkDeviceSize size = iter->second.ObjectInfo.DeviceMemory.size;
            VkMemoryMapFlags flags = 0;
            void *pData = iter->second.ObjectInfo.DeviceMemory.mappedAddress;

            if (size != 0) {
                vktrace_trace_packet_header *pPersistentlyMapMemory =
                    generate::vkMapMemory(false, device, deviceMemory, offset, size, flags, &pData);
                iter->second.ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket = trim::copy_packet(pPersistentlyMapMemory);
                vktrace_delete_trace_packet(&pPersistentlyMapMemory);
            }
        }
    }

    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
#if TRIM_USE_ORDERED_IMAGE_CREATION
void add_Image_call(vktrace_trace_packet_header *pHeader) {
    if (pHeader != NULL) {
        vktrace_enter_critical_section(&trimStateTrackerLock);
        s_trimGlobalStateTracker.add_Image_call(pHeader);
        vktrace_leave_critical_section(&trimStateTrackerLock);
    }
}
#endif  // TRIM_USE_ORDERED_IMAGE_CREATION

void add_InTrim_call(vktrace_trace_packet_header *pHeader) {
    if (pHeader != NULL) {
        vktrace_enter_critical_section(&trimStateTrackerLock);
        s_trimGlobalStateTracker.add_InTrim_call(pHeader);
        vktrace_leave_critical_section(&trimStateTrackerLock);
    }
}

//=========================================================================
ObjectInfo &add_Instance_object(VkInstance var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_Instance(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_Instance_object(VkInstance var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_Instance(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_Instance_objectInfo(VkInstance var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdInstances.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdInstances.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_PhysicalDevice_object(VkPhysicalDevice var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_PhysicalDevice(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_PhysicalDevice_object(VkPhysicalDevice var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_PhysicalDevice(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_PhysicalDevice_objectInfo(VkPhysicalDevice var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdPhysicalDevices.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdPhysicalDevices.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
VkQueue get_DeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex) {
    auto deviceIter = s_trimStateTrackerSnapshot.createdDevices.find(device);
    if (deviceIter == s_trimStateTrackerSnapshot.createdDevices.end()) {
        // Invalid device
        return VK_NULL_HANDLE;
    }

    if (queueFamilyIndex == VK_QUEUE_FAMILY_IGNORED) {
        queueFamilyIndex = 0;
    }

    if (queueFamilyIndex >= deviceIter->second.ObjectInfo.Device.queueFamilyCount) {
        // Invalid queueFamilyIndex
        return VK_NULL_HANDLE;
    }

    if (queueIndex >= deviceIter->second.ObjectInfo.Device.pQueueFamilies[queueFamilyIndex].count) {
        // Invalid queueIndex
        return VK_NULL_HANDLE;
    }

    return deviceIter->second.ObjectInfo.Device.pQueueFamilies[queueFamilyIndex].queues[queueIndex];
}

//=========================================================================
ObjectInfo &add_Device_object(VkDevice var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_Device(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
// The method can be used to query all specific type vulkan objects belong
// to a device. The query result will be packed into an input vector and
// return to the caller.
//
// \param result_objects, the input vector which is used to store the query
// result.
//
// \param object_map, the input map object which include trim track info of
// all vulkan objects of the specific type, the method will check every one
// item in this map to find out which object is created on the device.
//
// \param device, the input vulkan device, the method will query all specific
// type objects belong to this device.
//
// \tparam object_class, this is the class type that the method query all
// objects of this type, it can be QueryPool, Event, Fence and all trim
// tracked vulkan classes.
//=========================================================================
template <class object_class>
void get_device_objects(VkDevice device, const std::unordered_map<object_class, ObjectInfo> &object_map,
                        std::vector<object_class> &result_objects) {
    for (auto info = object_map.begin(); info != object_map.end(); ++info) {
        if (info->second.belongsToDevice == device) {
            result_objects.push_back(info->first);
        }
    }
}

//=========================================================================
// The method can be used to delete all vulkan objects that currently belong
// to the device. For every trim tracked objects belong to the device, the
// method delete all these objects one type by one type. it first delete
// all QueryPool objects, then Event, Fence, Semaphore......CommandPool.
//
// For every class type, it first query all objects of this class for the
// device, then delete all of them by the query result.
//
// \param device, this is the device that this method will delete all
// assossiate objects based on it.
//=========================================================================
void delete_objects_for_destroy_device(VkDevice device) {
    size_t delete_objects_number = 0;
    // QueryPool
    std::vector<VkQueryPool> QueryPoolsToRemove;
    get_device_objects<VkQueryPool>(device, s_trimGlobalStateTracker.createdQueryPools, QueryPoolsToRemove);
    for (size_t i = 0; i < QueryPoolsToRemove.size(); i++) {
        trim::remove_QueryPool_object(QueryPoolsToRemove[i]);
    }
    delete_objects_number += QueryPoolsToRemove.size();

    // Event
    std::vector<VkEvent> EventsToRemove;
    get_device_objects<VkEvent>(device, s_trimGlobalStateTracker.createdEvents, EventsToRemove);
    for (size_t i = 0; i < EventsToRemove.size(); i++) {
        trim::remove_Event_object(EventsToRemove[i]);
    }
    delete_objects_number += EventsToRemove.size();

    // Fence
    std::vector<VkFence> FencesToRemove;
    get_device_objects<VkFence>(device, s_trimGlobalStateTracker.createdFences, FencesToRemove);
    for (size_t i = 0; i < FencesToRemove.size(); i++) {
        trim::remove_Fence_object(FencesToRemove[i]);
    }
    delete_objects_number += FencesToRemove.size();

    // Semaphore
    std::vector<VkSemaphore> SemaphoresToRemove;
    get_device_objects<VkSemaphore>(device, s_trimGlobalStateTracker.createdSemaphores, SemaphoresToRemove);
    for (size_t i = 0; i < SemaphoresToRemove.size(); i++) {
        trim::remove_Semaphore_object(SemaphoresToRemove[i]);
    }
    delete_objects_number += SemaphoresToRemove.size();

    // Framebuffer
    std::vector<VkFramebuffer> FramebuffersToRemove;
    get_device_objects<VkFramebuffer>(device, s_trimGlobalStateTracker.createdFramebuffers, FramebuffersToRemove);
    for (size_t i = 0; i < FramebuffersToRemove.size(); i++) {
        trim::remove_Framebuffer_object(FramebuffersToRemove[i]);
    }
    delete_objects_number += FramebuffersToRemove.size();

    // DescriptorSet
    std::vector<VkDescriptorSet> DescriptorSetsToRemove;
    get_device_objects<VkDescriptorSet>(device, s_trimGlobalStateTracker.createdDescriptorSets, DescriptorSetsToRemove);
    for (size_t i = 0; i < DescriptorSetsToRemove.size(); i++) {
        trim::remove_DescriptorSet_object(DescriptorSetsToRemove[i]);
    }
    delete_objects_number += DescriptorSetsToRemove.size();

    // DescriptorPool
    std::vector<VkDescriptorPool> DescriptorPoolsToRemove;
    get_device_objects<VkDescriptorPool>(device, s_trimGlobalStateTracker.createdDescriptorPools, DescriptorPoolsToRemove);
    for (size_t i = 0; i < DescriptorPoolsToRemove.size(); i++) {
        trim::remove_DescriptorPool_object(DescriptorPoolsToRemove[i]);
    }
    delete_objects_number += DescriptorPoolsToRemove.size();

    // Pipeline
    std::vector<VkPipeline> PipelinesToRemove;
    get_device_objects<VkPipeline>(device, s_trimGlobalStateTracker.createdPipelines, PipelinesToRemove);
    for (size_t i = 0; i < PipelinesToRemove.size(); i++) {
        trim::remove_Pipeline_object(PipelinesToRemove[i]);
    }
    delete_objects_number += PipelinesToRemove.size();

    // PipelineCache
    std::vector<VkPipelineCache> PipelineCachesToRemove;
    get_device_objects<VkPipelineCache>(device, s_trimGlobalStateTracker.createdPipelineCaches, PipelineCachesToRemove);
    for (size_t i = 0; i < PipelineCachesToRemove.size(); i++) {
        trim::remove_PipelineCache_object(PipelineCachesToRemove[i]);
    }
    delete_objects_number += PipelineCachesToRemove.size();

    // ShaderModule
    std::vector<VkShaderModule> ShaderModulesToRemove;
    get_device_objects<VkShaderModule>(device, s_trimGlobalStateTracker.createdShaderModules, ShaderModulesToRemove);
    for (size_t i = 0; i < ShaderModulesToRemove.size(); i++) {
        trim::remove_ShaderModule_object(ShaderModulesToRemove[i]);
    }
    delete_objects_number += ShaderModulesToRemove.size();

    // RenderPass
    std::vector<VkRenderPass> RenderPasssToRemove;
    get_device_objects<VkRenderPass>(device, s_trimGlobalStateTracker.createdRenderPasss, RenderPasssToRemove);
    for (size_t i = 0; i < RenderPasssToRemove.size(); i++) {
        trim::remove_RenderPass_object(RenderPasssToRemove[i]);
    }
    delete_objects_number += RenderPasssToRemove.size();

    // PipelineLayout
    std::vector<VkPipelineLayout> PipelineLayoutsToRemove;
    get_device_objects<VkPipelineLayout>(device, s_trimGlobalStateTracker.createdPipelineLayouts, PipelineLayoutsToRemove);
    for (size_t i = 0; i < PipelineLayoutsToRemove.size(); i++) {
        trim::remove_PipelineLayout_object(PipelineLayoutsToRemove[i]);
    }
    delete_objects_number += PipelineLayoutsToRemove.size();

    // DescriptorSetLayout
    std::vector<VkDescriptorSetLayout> DescriptorSetLayoutsToRemove;
    get_device_objects<VkDescriptorSetLayout>(device, s_trimGlobalStateTracker.createdDescriptorSetLayouts,
                                              DescriptorSetLayoutsToRemove);
    for (size_t i = 0; i < DescriptorSetLayoutsToRemove.size(); i++) {
        trim::remove_DescriptorSetLayout_object(DescriptorSetLayoutsToRemove[i]);
    }
    delete_objects_number += DescriptorSetLayoutsToRemove.size();

    // Sampler
    std::vector<VkSampler> SamplersToRemove;
    get_device_objects<VkSampler>(device, s_trimGlobalStateTracker.createdSamplers, SamplersToRemove);
    for (size_t i = 0; i < SamplersToRemove.size(); i++) {
        trim::remove_Sampler_object(SamplersToRemove[i]);
    }
    delete_objects_number += SamplersToRemove.size();

    // Buffer
    std::vector<VkBuffer> BuffersToRemove;
    get_device_objects<VkBuffer>(device, s_trimGlobalStateTracker.createdBuffers, BuffersToRemove);
    for (size_t i = 0; i < BuffersToRemove.size(); i++) {
        trim::remove_Buffer_object(BuffersToRemove[i]);
    }
    delete_objects_number += BuffersToRemove.size();

    // BufferView
    std::vector<VkBufferView> BufferViewsToRemove;
    get_device_objects<VkBufferView>(device, s_trimGlobalStateTracker.createdBufferViews, BufferViewsToRemove);
    for (size_t i = 0; i < BufferViewsToRemove.size(); i++) {
        trim::remove_BufferView_object(BufferViewsToRemove[i]);
    }
    delete_objects_number += BufferViewsToRemove.size();

    // Image
    std::vector<VkImage> ImagesToRemove;
    get_device_objects<VkImage>(device, s_trimGlobalStateTracker.createdImages, ImagesToRemove);
    for (size_t i = 0; i < ImagesToRemove.size(); i++) {
        trim::remove_Image_object(ImagesToRemove[i]);
    }
    delete_objects_number += ImagesToRemove.size();

    // ImageView
    std::vector<VkImageView> ImageViewsToRemove;
    get_device_objects<VkImageView>(device, s_trimGlobalStateTracker.createdImageViews, ImageViewsToRemove);
    for (size_t i = 0; i < ImageViewsToRemove.size(); i++) {
        trim::remove_ImageView_object(ImageViewsToRemove[i]);
    }
    delete_objects_number += ImageViewsToRemove.size();

    // DeviceMemory
    std::vector<VkDeviceMemory> DeviceMemorysToRemove;
    get_device_objects<VkDeviceMemory>(device, s_trimGlobalStateTracker.createdDeviceMemorys, DeviceMemorysToRemove);
    for (size_t i = 0; i < DeviceMemorysToRemove.size(); i++) {
        trim::remove_DeviceMemory_object(DeviceMemorysToRemove[i]);
    }
    delete_objects_number += DeviceMemorysToRemove.size();

    // SwapchainKHR
    std::vector<VkSwapchainKHR> SwapchainKHRsToRemove;
    get_device_objects<VkSwapchainKHR>(device, s_trimGlobalStateTracker.createdSwapchainKHRs, SwapchainKHRsToRemove);
    for (size_t i = 0; i < SwapchainKHRsToRemove.size(); i++) {
        trim::remove_SwapchainKHR_object(SwapchainKHRsToRemove[i]);
    }
    delete_objects_number += SwapchainKHRsToRemove.size();

    // CommandBuffer
    std::vector<VkCommandBuffer> CommandBuffersToRemove;
    get_device_objects<VkCommandBuffer>(device, s_trimGlobalStateTracker.createdCommandBuffers, CommandBuffersToRemove);
    for (size_t i = 0; i < CommandBuffersToRemove.size(); i++) {
        trim::remove_CommandBuffer_object(CommandBuffersToRemove[i]);
    }
    delete_objects_number += CommandBuffersToRemove.size();

    // CommandPool
    std::vector<VkCommandPool> CommandPoolsToRemove;
    get_device_objects<VkCommandPool>(device, s_trimGlobalStateTracker.createdCommandPools, CommandPoolsToRemove);
    for (size_t i = 0; i < CommandPoolsToRemove.size(); i++) {
        trim::remove_CommandPool_object(CommandPoolsToRemove[i]);
    }
    delete_objects_number += CommandPoolsToRemove.size();

    if (delete_objects_number != 0) {
        vktrace_LogWarning("Device destroyed but %d child objects were not destroyed.", delete_objects_number);
    }
}

//=========================================================================
void remove_Device_object(VkDevice var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);

    std::vector<VkQueue> queuesToRemove;
    for (auto info = s_trimGlobalStateTracker.createdQueues.begin(); info != s_trimGlobalStateTracker.createdQueues.end(); ++info) {
        if (info->second.belongsToDevice == var) {
            queuesToRemove.push_back(reinterpret_cast<VkQueue>(info->first));
        }
    }

    for (size_t i = 0; i < queuesToRemove.size(); i++) {
        trim::remove_Queue_object(queuesToRemove[i]);
    }

    if (g_trimIsInTrim) {
        // If vktrace is in the middle of a trim, then we want to also generate calls to destroy any objects that have not yet been
        // destroyed by the app.
        add_destroy_device_object_packets(var);
    } else {
        // If vktrace is not in the middle of a trim, that means it's in
        // pre-trim, and the target app destroy the device before we start
        // to trim (hot-key not pressed until now or it haven't reach
        // the start-frame), the device object will not be used later, so
        // we should not include related creation call for the device
        // in final trimmed trace file. not only this device, any objects
        // if created base on the device should be also excluded in final
        // trimmed trace file. We should remove all these associate
        // objects from trim track system to avoid generating any creation
        // call for them.
        //
        // If a target app destroys a device, it should already have
        // destroyed all objects based on the device. But some apps destroy
        // device without first destroying objects based on the device.
        //
        // Such behavior causes a problem if we only delete these associated
        // objects in corresponding destroy call, because target app
        // doesn't call these before destroy the device, we end up
        // generating all associate objects creation. When playback the
        // trimmed trace file, vkreplay will fail to remap the device when
        // it create those associate objects and output errors. So here
        // we delete all associate objects of the device from trim track
        // system to avoid any such creation call be included in final
        // trimmed trace file.
        delete_objects_for_destroy_device(var);
    }

    s_trimGlobalStateTracker.remove_Device(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_Device_objectInfo(VkDevice var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdDevices.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdDevices.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_SurfaceKHR_object(VkSurfaceKHR var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_SurfaceKHR(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_SurfaceKHR_object(VkSurfaceKHR var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_SurfaceKHR(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_SurfaceKHR_objectInfo(VkSurfaceKHR var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdSurfaceKHRs.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdSurfaceKHRs.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_Queue_object(VkQueue var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_Queue(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_Queue_object(const VkQueue var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_Queue(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_Queue_objectInfo(VkQueue var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdQueues.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdQueues.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_SwapchainKHR_object(VkSwapchainKHR var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_SwapchainKHR(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_SwapchainKHR_object(const VkSwapchainKHR var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_SwapchainKHR(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_SwapchainKHR_objectInfo(VkSwapchainKHR var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdSwapchainKHRs.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdSwapchainKHRs.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_CommandPool_object(VkCommandPool var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_CommandPool(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_CommandPool_object(const VkCommandPool var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_CommandPool(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_CommandPool_objectInfo(VkCommandPool var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdCommandPools.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdCommandPools.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_CommandBuffer_object(VkCommandBuffer var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_CommandBuffer(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_CommandBuffer_object(const VkCommandBuffer var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_CommandBuffer(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_CommandBuffer_objectInfo(VkCommandBuffer var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdCommandBuffers.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdCommandBuffers.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_DeviceMemory_object(VkDeviceMemory var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_DeviceMemory(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_DeviceMemory_object(const VkDeviceMemory var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_DeviceMemory(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_DeviceMemory_objectInfo(VkDeviceMemory var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdDeviceMemorys.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdDeviceMemorys.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_ImageView_object(VkImageView var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_ImageView(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_ImageView_object(const VkImageView var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_ImageView(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_ImageView_objectInfo(VkImageView var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdImageViews.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdImageViews.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_Image_object(VkImage var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_Image(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_Image_object(const VkImage var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_Image(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_Image_objectInfo(VkImage var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdImages.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdImages.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_BufferView_object(VkBufferView var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_BufferView(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_BufferView_object(const VkBufferView var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_BufferView(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_BufferView_objectInfo(VkBufferView var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdBufferViews.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdBufferViews.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_Buffer_object(VkBuffer var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_Buffer(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_Buffer_object(const VkBuffer var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_Buffer(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_Buffer_objectInfo(VkBuffer var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdBuffers.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdBuffers.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_Sampler_object(VkSampler var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_Sampler(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_Sampler_object(const VkSampler var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_Sampler(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_Sampler_objectInfo(VkSampler var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdSamplers.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdSamplers.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_DescriptorSetLayout_object(VkDescriptorSetLayout var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_DescriptorSetLayout(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_DescriptorSetLayout_object(VkDescriptorSetLayout var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_DescriptorSetLayout(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_DescriptorSetLayout_objectInfo(VkDescriptorSetLayout var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdDescriptorSetLayouts.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdDescriptorSetLayouts.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_DescriptorUpdateTemplate_object(VkDescriptorUpdateTemplate var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_DescriptorUpdateTemplate(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_DescriptorUpdateTemplate_object(VkDescriptorUpdateTemplate var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_DescriptorUpdateTemplate(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_DescriptorUpdateTemplate_objectInfo(VkDescriptorUpdateTemplate var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdDescriptorUpdateTemplates.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdDescriptorUpdateTemplates.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_PipelineLayout_object(VkPipelineLayout var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_PipelineLayout(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_PipelineLayout_object(const VkPipelineLayout var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_PipelineLayout(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_PipelineLayout_objectInfo(VkPipelineLayout var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdPipelineLayouts.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdPipelineLayouts.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_RenderPass_object(VkRenderPass var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_RenderPass(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_RenderPass_object(const VkRenderPass var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_RenderPass(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_RenderPass_objectInfo(VkRenderPass var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdRenderPasss.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdRenderPasss.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_ShaderModule_object(VkShaderModule var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_ShaderModule(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_ShaderModule_object(const VkShaderModule var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_ShaderModule(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_ShaderModule_objectInfo(VkShaderModule var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdShaderModules.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdShaderModules.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_PipelineCache_object(VkPipelineCache var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_PipelineCache(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

void remove_PipelineCache_object(const VkPipelineCache var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_PipelineCache(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_PipelineCache_objectInfo(VkPipelineCache var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdPipelineCaches.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdPipelineCaches.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_DescriptorPool_object(VkDescriptorPool var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_DescriptorPool(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_DescriptorPool_object(const VkDescriptorPool var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_DescriptorPool(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_DescriptorPool_objectInfo(VkDescriptorPool var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdDescriptorPools.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdDescriptorPools.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_Pipeline_object(VkPipeline var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_Pipeline(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_Pipeline_object(const VkPipeline var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_Pipeline(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_Pipeline_objectInfo(VkPipeline var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdPipelines.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdPipelines.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
// target title will bind pipeline to commandBuffer, the function record the
// binding relation to the pipeline track info.
void add_CommandBuffer_to_binding_Pipeline(VkCommandBuffer commandBuffer, VkPipeline pipeLine) {
    std::lock_guard<std::mutex> lock(g_Mutex_CommandBufferPipelineMap);

    auto pCommandBuffers = s_trimGlobalStateTracker.get_BoundCommandBuffers(pipeLine);
    if (pCommandBuffers != nullptr) {
        pCommandBuffers->insert(commandBuffer);
    }
}

//=========================================================================
// The function first clear all record calls in any commandbuffer if the
// command buffer use the pipeline, also clear ImageTransitions and
// BufferTransitions in the command buffer track info.
// The function is used when target title destroy the pipeline.
void clear_CommandBuffer_calls_by_binding_Pipeline(VkPipeline pipeLine) {
    std::lock_guard<std::mutex> lock(g_Mutex_CommandBufferPipelineMap);

    auto pCommandBuffers = s_trimGlobalStateTracker.get_BoundCommandBuffers(pipeLine);
    if (pCommandBuffers != nullptr) {
        for (auto pCommandBuffer = pCommandBuffers->begin(); pCommandBuffer != pCommandBuffers->end(); pCommandBuffer++) {
            remove_CommandBuffer_calls(*pCommandBuffer);
            ClearImageTransitions(*pCommandBuffer);
            ClearBufferTransitions(*pCommandBuffer);

            // remove all pipelines from the command buffer track info because
            // we already clear all its recorded calls.
            auto pPipelines = s_trimGlobalStateTracker.get_BindingPipelines(*pCommandBuffer);
            if (pPipelines != nullptr) {
                pPipelines->clear();
            }
        }
    }
}

//=========================================================================
// The function record the binding relation to the command buffer
// track info.
// The function is used when target title bind pipeline to commandBuffer.
void add_binding_Pipeline_to_CommandBuffer(VkCommandBuffer commandBuffer, VkPipeline pipeLine) {
    std::lock_guard<std::mutex> lock(g_Mutex_CommandBufferPipelineMap);

    auto pPipelines = s_trimGlobalStateTracker.get_BindingPipelines(commandBuffer);
    if (pPipelines != nullptr) {
        pPipelines->insert(pipeLine);
    }
}

//=========================================================================
// For every pipeline used by the command buffer, the function delete
// the command buffer from that pipeline, then delete all pipelines from the
// command buffer track info.
// The function is used when target title call vkResetCommandBuffer,
// vkFreeCommandBuffer or vkBeginCommandBuffer. so we need to remove its
// registration from all used pipelines:
void clear_binding_Pipelines_from_CommandBuffer(VkCommandBuffer commandBuffer) {
    std::lock_guard<std::mutex> lock(g_Mutex_CommandBufferPipelineMap);

    auto pBindingPipelines = s_trimGlobalStateTracker.get_BindingPipelines(commandBuffer);
    if (pBindingPipelines != nullptr) {
        for (auto pPipeline = pBindingPipelines->begin(); pPipeline != pBindingPipelines->end(); pPipeline++) {
            auto pCommandBuffers = s_trimGlobalStateTracker.get_BoundCommandBuffers(*pPipeline);
            if (pCommandBuffers != nullptr) {
                pCommandBuffers->erase(commandBuffer);
            }
        }

        // Clear all pipelines used by the command buffer from its track info.
        pBindingPipelines->clear();
    }
}

//=========================================================================
ObjectInfo &add_Semaphore_object(VkSemaphore var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_Semaphore(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_Semaphore_object(const VkSemaphore var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_Semaphore(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_Semaphore_objectInfo(VkSemaphore var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdSemaphores.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdSemaphores.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_Fence_object(VkFence var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_Fence(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_Fence_object(const VkFence var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_Fence(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_Fence_objectInfo(VkFence var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdFences.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdFences.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_Framebuffer_object(VkFramebuffer var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_Framebuffer(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_Framebuffer_object(const VkFramebuffer var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_Framebuffer(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_Framebuffer_objectInfo(VkFramebuffer var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdFramebuffers.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdFramebuffers.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_Event_object(VkEvent var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_Event(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_Event_object(const VkEvent var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_Event(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_Event_objectInfo(VkEvent var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdEvents.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdEvents.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_QueryPool_object(VkQueryPool var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_QueryPool(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_QueryPool_object(const VkQueryPool var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_QueryPool(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_QueryPool_objectInfo(VkQueryPool var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdQueryPools.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdQueryPools.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================
ObjectInfo &add_DescriptorSet_object(VkDescriptorSet var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    ObjectInfo &info = s_trimGlobalStateTracker.add_DescriptorSet(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return info;
}

//=========================================================================
void remove_DescriptorSet_object(const VkDescriptorSet var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.remove_DescriptorSet(var);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
ObjectInfo *get_DescriptorSet_objectInfo(VkDescriptorSet var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimGlobalStateTracker.createdDescriptorSets.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != s_trimGlobalStateTracker.createdDescriptorSets.end()) {
        pResult = &(iter->second);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return pResult;
}

//=========================================================================

#define TRIM_MARK_OBJECT_REFERENCE(type)                                   \
    void mark_##type##_reference(Vk##type var) {                           \
        vktrace_enter_critical_section(&trimStateTrackerLock);             \
        auto iter = s_trimStateTrackerSnapshot.created##type##s.find(var); \
        if (iter != s_trimStateTrackerSnapshot.created##type##s.end()) {   \
            ObjectInfo *info = &iter->second;                              \
            if (info != nullptr && !(info->bReferencedInTrimChecked)) {    \
                info->bReferencedInTrimChecked = true;                     \
                info->bReferencedInTrim = true;                            \
            }                                                              \
        }                                                                  \
        vktrace_leave_critical_section(&trimStateTrackerLock);             \
    }

#define TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(type)            \
    void mark_##type##_reference(Vk##type var) {                           \
        vktrace_enter_critical_section(&trimStateTrackerLock);             \
        auto iter = s_trimStateTrackerSnapshot.created##type##s.find(var); \
        if (iter != s_trimStateTrackerSnapshot.created##type##s.end()) {   \
            ObjectInfo *info = &iter->second;                              \
            if (info != nullptr && !(info->bReferencedInTrimChecked)) {    \
                info->bReferencedInTrimChecked = true;                     \
                info->bReferencedInTrim = true;                            \
                mark_Device_reference((VkDevice)info->belongsToDevice);    \
            }                                                              \
        }                                                                  \
        vktrace_leave_critical_section(&trimStateTrackerLock);             \
    }

#define TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICEMEMORY_DEPENDENCY(type)      \
    void mark_##type##_reference(Vk##type var) {                           \
        vktrace_enter_critical_section(&trimStateTrackerLock);             \
        auto iter = s_trimStateTrackerSnapshot.created##type##s.find(var); \
        if (iter != s_trimStateTrackerSnapshot.created##type##s.end()) {   \
            ObjectInfo *info = &iter->second;                              \
            if (info != nullptr && !(info->bReferencedInTrimChecked)) {    \
                info->bReferencedInTrimChecked = true;                     \
                info->bReferencedInTrim = true;                            \
                mark_DeviceMemory_reference(info->ObjectInfo.type.memory); \
                mark_Device_reference((VkDevice)info->belongsToDevice);    \
            }                                                              \
        }                                                                  \
        vktrace_leave_critical_section(&trimStateTrackerLock);             \
    }

TRIM_MARK_OBJECT_REFERENCE(Instance);
TRIM_MARK_OBJECT_REFERENCE(PhysicalDevice);
TRIM_MARK_OBJECT_REFERENCE(Device);

TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(CommandPool)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(DescriptorPool)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(RenderPass)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(PipelineCache)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Queue)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Semaphore)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(DeviceMemory)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Fence)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(SwapchainKHR)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Event)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(QueryPool)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(ShaderModule)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(PipelineLayout)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Sampler)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(DescriptorSetLayout)

TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICEMEMORY_DEPENDENCY(Image)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICEMEMORY_DEPENDENCY(Buffer)

void mark_CommandBuffer_reference(VkCommandBuffer var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimStateTrackerSnapshot.createdCommandBuffers.find(var);
    if (iter != s_trimStateTrackerSnapshot.createdCommandBuffers.end()) {
        ObjectInfo *info = &iter->second;
        if (info != nullptr && !(info->bReferencedInTrimChecked)) {
            info->bReferencedInTrimChecked = true;
            if (s_trimGlobalStateTracker.m_cmdBufferPackets.find(var) != s_trimGlobalStateTracker.m_cmdBufferPackets.end()) {
                info->bReferencedInTrim = true;
            }
            mark_CommandPool_reference(info->ObjectInfo.CommandBuffer.commandPool);
        }
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

void mark_Pipeline_reference(VkPipeline var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimStateTrackerSnapshot.createdPipelines.find(var);
    if (iter != s_trimStateTrackerSnapshot.createdPipelines.end()) {
        ObjectInfo *info = &iter->second;
        if (info != nullptr && !(info->bReferencedInTrimChecked)) {
            info->bReferencedInTrimChecked = true;
            info->bReferencedInTrim = true;
            mark_PipelineCache_reference(info->ObjectInfo.Pipeline.pipelineCache);
            mark_Device_reference(info->belongsToDevice);
        }
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

void mark_DescriptorSet_reference(VkDescriptorSet var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimStateTrackerSnapshot.createdDescriptorSets.find(var);
    if (iter != s_trimStateTrackerSnapshot.createdDescriptorSets.end()) {
        ObjectInfo *info = &iter->second;
        if (info != nullptr && !(info->bReferencedInTrimChecked)) {
            info->bReferencedInTrimChecked = true;
            info->bReferencedInTrim = true;
            mark_DescriptorPool_reference(info->ObjectInfo.DescriptorSet.descriptorPool);
        }
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

void mark_ImageView_reference(VkImageView var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimStateTrackerSnapshot.createdImageViews.find(var);
    if (iter != s_trimStateTrackerSnapshot.createdImageViews.end()) {
        ObjectInfo *info = &iter->second;
        if (info != nullptr && !(info->bReferencedInTrimChecked)) {
            info->bReferencedInTrimChecked = true;
            info->bReferencedInTrim = true;
            mark_Image_reference(info->ObjectInfo.ImageView.image);
        }
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

void mark_BufferView_reference(VkBufferView var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimStateTrackerSnapshot.createdBufferViews.find(var);
    if (iter != s_trimStateTrackerSnapshot.createdBufferViews.end()) {
        ObjectInfo *info = &iter->second;
        if (info != nullptr && !(info->bReferencedInTrimChecked)) {
            info->bReferencedInTrimChecked = true;
            info->bReferencedInTrim = true;
            mark_Buffer_reference(info->ObjectInfo.BufferView.buffer);
        }
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

void mark_Framebuffer_reference(VkFramebuffer var) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    auto iter = s_trimStateTrackerSnapshot.createdFramebuffers.find(var);
    if (iter != s_trimStateTrackerSnapshot.createdFramebuffers.end()) {
        ObjectInfo *info = &iter->second;
        if (info != nullptr && !(info->bReferencedInTrimChecked)) {
            info->bReferencedInTrimChecked = true;
            info->bReferencedInTrim = true;
            for (uint32_t i = 0; i < info->ObjectInfo.Framebuffer.attachmentCount; i++) {
                mark_ImageView_reference(info->ObjectInfo.Framebuffer.pAttachments[i]);
            }
            mark_Device_reference(info->belongsToDevice);
        }
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
// Recreate Objects for trim
//=========================================================================

// Check if it's a valid descriptor at specified location of descriptor
// info array in pDescriptorWrites.
// In the function, for the specified descriptor, according to the
// descriptor type, we check all related objects still be tracked or
// not to decide if it is still a valid object.
//
// Note: we check all needed objects, for example, if the descriptor type
// is VK_DESCRIPTOR_TYPE_SAMPLER, we check both sampler and imageview.
bool isValidDescriptorIndex(const VkWriteDescriptorSet *pDescriptorWrites, uint32_t index) {
    bool isValidDescriptor = true;
    if (index < pDescriptorWrites->descriptorCount) {
        switch (pDescriptorWrites->descriptorType) {
            case VK_DESCRIPTOR_TYPE_SAMPLER:
            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
            case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
            case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
            case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: {
                if (pDescriptorWrites->pImageInfo != nullptr) {
                    if ((pDescriptorWrites->descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER) ||
                        (pDescriptorWrites->descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)) {
                        if (nullptr == trim::get_Sampler_objectInfo(pDescriptorWrites->pImageInfo[index].sampler)) {
                            isValidDescriptor = false;
                        }
                    }
                    if ((isValidDescriptor) && ((pDescriptorWrites->descriptorType == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE) ||
                                                (pDescriptorWrites->descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE) ||
                                                (pDescriptorWrites->descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) ||
                                                (pDescriptorWrites->descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT))) {
                        if (nullptr == trim::get_ImageView_objectInfo(pDescriptorWrites->pImageInfo[index].imageView)) {
                            isValidDescriptor = false;
                        }
                    }
                } else {
                    assert(false);
                    isValidDescriptor = false;
                    vktrace_LogWarning(
                        "The descriptorType does not match when trim generate vkUpdateDescriptorSets call for update the bindings "
                        "of the DescriptorSet.");
                }
            } break;
            case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
            case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: {
                if (pDescriptorWrites->pTexelBufferView != nullptr) {
                    if (nullptr == trim::get_BufferView_objectInfo(pDescriptorWrites->pTexelBufferView[index])) {
                        isValidDescriptor = false;
                    }
                } else {
                    assert(false);
                    isValidDescriptor = false;
                    vktrace_LogWarning(
                        "The descriptorType does not match when trim generate vkUpdateDescriptorSets call for update the bindings "
                        "of the DescriptorSet.");
                }
            } break;
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: {
                if (pDescriptorWrites->pBufferInfo != nullptr) {
                    if (nullptr == trim::get_Buffer_objectInfo(pDescriptorWrites->pBufferInfo[index].buffer)) {
                        isValidDescriptor = false;
                    }
                } else {
                    assert(false);
                    isValidDescriptor = false;
                    vktrace_LogWarning(
                        "The descriptorType does not match when trim generate vkUpdateDescriptorSets call for update the bindings "
                        "of the DescriptorSet.");
                }
            } break;
            default:
                assert(false);
                break;
        }
    } else {
        assert(false);
    }
    return isValidDescriptor;
}

// Copy descriptor of location src to the location of dst. the location src,
// dst is the index of the descriptor array: pImageInfo, pBufferInfo or
// pTexelBufferView according to descriptorType.
//
// The function only copy related element from descriptorType specified array.
// If src and dst descriptorType mismatch, a warning meesage will be given to
// trace log.
void copyDescriptorByIndex(const VkWriteDescriptorSet *pDescriptorWrites, uint32_t dst, uint32_t src) {
    if ((dst < pDescriptorWrites->descriptorCount) && (src < pDescriptorWrites->descriptorCount)) {
        switch (pDescriptorWrites->descriptorType) {
            case VK_DESCRIPTOR_TYPE_SAMPLER:
            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
            case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
            case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
            case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: {
                if (pDescriptorWrites->pImageInfo != nullptr) {
                    memcpy(const_cast<VkDescriptorImageInfo *>(pDescriptorWrites->pImageInfo + dst),
                           const_cast<VkDescriptorImageInfo *>(pDescriptorWrites->pImageInfo + src), sizeof(VkDescriptorImageInfo));
                } else {
                    assert(false);
                    vktrace_LogWarning(
                        "The descriptorType does not match when trim generate vkUpdateDescriptorSets call for update the bindings "
                        "of the DescriptorSet.");
                }
            } break;
            case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
            case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: {
                if (pDescriptorWrites->pTexelBufferView != nullptr) {
                    memcpy(const_cast<VkBufferView *>(pDescriptorWrites->pTexelBufferView + dst),
                           const_cast<VkBufferView *>(pDescriptorWrites->pTexelBufferView + src), sizeof(VkBufferView));
                } else {
                    assert(false);
                    vktrace_LogWarning(
                        "The descriptorType does not match when trim generate vkUpdateDescriptorSets call for update the bindings "
                        "of the DescriptorSet.");
                }
            } break;
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: {
                if (pDescriptorWrites->pBufferInfo != nullptr) {
                    memcpy(const_cast<VkDescriptorBufferInfo *>(pDescriptorWrites->pBufferInfo + dst),
                           const_cast<VkDescriptorBufferInfo *>(pDescriptorWrites->pBufferInfo + src),
                           sizeof(VkDescriptorBufferInfo));
                } else {
                    assert(false);
                    vktrace_LogWarning(
                        "The descriptorType does not match when trim generate vkUpdateDescriptorSets call for update the bindings "
                        "of the DescriptorSet.");
                }
            } break;
            default:
                break;
        }
    } else {
        assert(false);
    }
}

// In array of pDescriptorWrites, find the first valid descriptor info and
// return its index in the array.
//
// If all are invalid, return -1;
int32_t getValidDescriptorIndexFromWriteDescriptorSet(const VkWriteDescriptorSet *pDescriptorWrites) {
    int32_t ValidDescriptorIndex = -1;
    for (uint32_t i = 0; i < pDescriptorWrites->descriptorCount; i++) {
        if (isValidDescriptorIndex(pDescriptorWrites, i)) {
            ValidDescriptorIndex = i;
            break;
        }
    }
    return ValidDescriptorIndex;
}

// Remove all invalid descriptors from one VkWriteDescriptorSet and replace
// with valid one.
// on the following conditions, the function return true:
//     1> it removed all invalid descriptors and replace with valid one.
//     2> no descriptors found in this VkWriteDescriptorSet.
// if the VkWriteDescriptorSet only include invalid descriptors, the
// function return false.
bool UpdateInvalidDescriptors(const VkWriteDescriptorSet *pDescriptorWrites) {
    bool removeInvalidDescriptorsFlag = false;

    // search and find the first valid one in the VkWriteDescriptorSet, we'll
    // use it to replace any invalid one in the descriptor info array of
    // the VkWriteDescriptorSet.
    //
    // note: we use this way to remove all invalid descriptors. for any
    // invalid descriptor in a binding, we can assume its location is
    // unused by target title at that time, so we can set it to any
    // valid one.
    //
    // note: By Doc, the descriptor at a binding location can be invalid
    // (just get vkAllocateDescriptorSets or corresponding object has been
    // destroied) but any descriptor info in update call must be valid.
    uint32_t validDescriptorIndex = getValidDescriptorIndexFromWriteDescriptorSet(pDescriptorWrites);
    if ((validDescriptorIndex != -1) || (pDescriptorWrites->descriptorCount == 0)) {
        // if we can remove all invalid descriptor info or pDescriptorWrites
        // don't have any descriptors, the function return true.
        removeInvalidDescriptorsFlag = true;

        for (int i = 0; i < pDescriptorWrites->descriptorCount; i++) {
            if (!isValidDescriptorIndex(pDescriptorWrites, i)) {
                copyDescriptorByIndex(pDescriptorWrites, i, validDescriptorIndex);
            }
        }
    } else {
        // all descriptors in this pDescriptorWrites is invalid, we should remove it;
        VkWriteDescriptorSet *pDescriptorWritesToChange = const_cast<VkWriteDescriptorSet *>(pDescriptorWrites);
        pDescriptorWritesToChange->descriptorCount = 0;
    }
    return removeInvalidDescriptorsFlag;
}

// remove all invalid descriptors for every element from the
// VkWriteDescriptorSet array of pDescriptorWrites.
// please note: the way we remove it is to replace it with a
// valid one.
void UpdateInvalidDescriptors(uint32_t descriptorWriteCount, const VkWriteDescriptorSet *pDescriptorWrites) {
    for (uint32_t i = 0; i < descriptorWriteCount; i++) {
        if (!UpdateInvalidDescriptors(&pDescriptorWrites[i])) {
            assert(false);
        }
    }
}

void recreate_instances(StateTracker &stateTracker) {
    for (auto iterator = stateTracker.seqInstances.begin(); iterator != stateTracker.seqInstances.end(); ++iterator) {
        auto obj = stateTracker.createdInstances.find(*iterator);
        if (obj == stateTracker.createdInstances.end()) {
            continue;
        }
        vktrace_write_trace_packet(obj->second.ObjectInfo.Instance.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Instance.pCreatePacket));

        if (obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket != NULL) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket,
                                       vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket));
        }

        if (obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket != NULL) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket,
                                       vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket));
        }
    }
}

void recreate_physical_device_mem_queue(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdPhysicalDevices.begin(); obj != stateTracker.createdPhysicalDevices.end(); obj++) {
        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDevicePropertiesPacket != nullptr) {
            // Generate GetPhysicalDeviceProperties Packet. It's needed by portability
            // process in vkAllocateMemory during playback.
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDevicePropertiesPacket,
                                       vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDevicePropertiesPacket));
        }
        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceProperties2KHRPacket != nullptr) {
            // Generate GetPhysicalDeviceProperties2KHR Packet. It's needed by portability
            // process in vkAllocateMemory during playback.
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceProperties2KHRPacket,
                                       vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceProperties2KHRPacket));
        }

        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket != nullptr) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket,
                                       vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket));
        }

        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesCountPacket != nullptr) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesCountPacket,
                                       vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(
                &(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesCountPacket));
        }

        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesPacket != nullptr) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesPacket,
                                       vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(
                &(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesPacket));
        }

        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyProperties2KHRCountPacket != nullptr) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyProperties2KHRCountPacket,
                                       vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(
                &(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyProperties2KHRCountPacket));
        }

        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyProperties2KHRPacket != nullptr) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyProperties2KHRPacket,
                                       vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(
                &(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyProperties2KHRPacket));
        }
    }
}

void recreate_surfaces(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdSurfaceKHRs.begin(); obj != stateTracker.createdSurfaceKHRs.end(); obj++) {
        vktrace_write_trace_packet(obj->second.ObjectInfo.SurfaceKHR.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.SurfaceKHR.pCreatePacket));

        VkSurfaceKHR surface = obj->first;

        for (auto physicalDeviceInfo = stateTracker.createdPhysicalDevices.begin();
             physicalDeviceInfo != stateTracker.createdPhysicalDevices.end(); physicalDeviceInfo++) {
            if (physicalDeviceInfo->second.belongsToInstance == obj->second.belongsToInstance) {
                VkPhysicalDevice physicalDevice = physicalDeviceInfo->first;

                uint32_t presentModesCount = 0;
                VkPresentModeKHR *pPresentModes;
                vktrace_trace_packet_header *pSurfacePresentModesCountHeader =
                    generate::vkGetPhysicalDeviceSurfacePresentModesKHR(true, physicalDevice, surface, &presentModesCount, NULL);
                vktrace_write_trace_packet(pSurfacePresentModesCountHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&(pSurfacePresentModesCountHeader));

                if (presentModesCount > 0) {
                    pPresentModes = VKTRACE_NEW_ARRAY(VkPresentModeKHR, presentModesCount);

                    vktrace_trace_packet_header *pSurfacePresentModeHeader = generate::vkGetPhysicalDeviceSurfacePresentModesKHR(
                        true, physicalDevice, surface, &presentModesCount, pPresentModes);
                    vktrace_write_trace_packet(pSurfacePresentModeHeader, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&(pSurfacePresentModeHeader));
                    VKTRACE_DELETE(pPresentModes);
                }

                uint32_t surfaceFormatCount = 0;
                VkSurfaceFormatKHR *pSurfaceFormats;
                vktrace_trace_packet_header *pSurfaceFormatsCountHeader =
                    generate::vkGetPhysicalDeviceSurfaceFormatsKHR(true, physicalDevice, surface, &surfaceFormatCount, NULL);
                vktrace_write_trace_packet(pSurfaceFormatsCountHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pSurfaceFormatsCountHeader);

                if (surfaceFormatCount > 0) {
                    pSurfaceFormats = VKTRACE_NEW_ARRAY(VkSurfaceFormatKHR, surfaceFormatCount);

                    vktrace_trace_packet_header *pSurfaceFormatsHeader = generate::vkGetPhysicalDeviceSurfaceFormatsKHR(
                        true, physicalDevice, surface, &surfaceFormatCount, pSurfaceFormats);
                    vktrace_write_trace_packet(pSurfaceFormatsHeader, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pSurfaceFormatsHeader);
                    VKTRACE_DELETE(pSurfaceFormats);
                }

                VkSurfaceCapabilitiesKHR surfaceCapabilities;
                vktrace_trace_packet_header *pSurfaceCapabilitiesHeader =
                    generate::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(true, physicalDevice, surface, &surfaceCapabilities);
                vktrace_write_trace_packet(pSurfaceCapabilitiesHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pSurfaceCapabilitiesHeader);

                for (uint32_t queueFamilyIndex = 0;
                     queueFamilyIndex < physicalDeviceInfo->second.ObjectInfo.PhysicalDevice.queueFamilyCount; queueFamilyIndex++) {
                    VkBool32 supported;
                    vktrace_trace_packet_header *pHeader =
                        generate::vkGetPhysicalDeviceSurfaceSupportKHR(true, physicalDevice, queueFamilyIndex, surface, &supported);
                    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pHeader);
                }
            }
        }
    }
}

void recreate_devices(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdDevices.begin(); obj != stateTracker.createdDevices.end(); obj++) {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Device.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Device.pCreatePacket));
    }
}

void recreate_queues(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdQueues.begin(); obj != stateTracker.createdQueues.end(); obj++) {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Queue.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Queue.pCreatePacket));
    }
}

void recreate_command_pools(StateTracker &stateTracker) {
    for (auto poolObj = stateTracker.createdCommandPools.begin(); poolObj != stateTracker.createdCommandPools.end(); poolObj++) {
        if (g_trimPostProcess && !poolObj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_CommandPool_object((VkCommandPool)poolObj->first);
            continue;
        }
        vktrace_write_trace_packet(poolObj->second.ObjectInfo.CommandPool.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(poolObj->second.ObjectInfo.CommandPool.pCreatePacket));

        // Now allocate command buffers that were allocated on this pool
        for (int32_t level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; level <= VK_COMMAND_BUFFER_LEVEL_SECONDARY; level++) {
            VkCommandBufferAllocateInfo allocateInfo;
            allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocateInfo.pNext = NULL;
            allocateInfo.commandPool = (VkCommandPool)poolObj->first;
            allocateInfo.level = (VkCommandBufferLevel)level;
            allocateInfo.commandBufferCount = poolObj->second.ObjectInfo.CommandPool.numCommandBuffersAllocated[level];
            if (allocateInfo.commandBufferCount > 0) {
                VkCommandBuffer *pCommandBuffers = new VkCommandBuffer[allocateInfo.commandBufferCount];
                uint32_t index = 0;
                for (auto cbIter = stateTracker.createdCommandBuffers.begin(); cbIter != stateTracker.createdCommandBuffers.end();
                     cbIter++) {
                    if (cbIter->second.ObjectInfo.CommandBuffer.commandPool == (VkCommandPool)poolObj->first &&
                        cbIter->second.ObjectInfo.CommandBuffer.level == level) {
                        pCommandBuffers[index] = (VkCommandBuffer)cbIter->first;
                        index++;
                    }
                }

                vktrace_trace_packet_header *pHeader =
                    generate::vkAllocateCommandBuffers(false, poolObj->second.belongsToDevice, &allocateInfo, pCommandBuffers);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&(pHeader));

                delete[] pCommandBuffers;
            }
        }
    }
}

void recreate_device_mem(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdDeviceMemorys.begin(); obj != stateTracker.createdDeviceMemorys.end(); obj++) {
        // AllocateMemory
        vktrace_write_trace_packet(obj->second.ObjectInfo.DeviceMemory.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.DeviceMemory.pCreatePacket));
    }
}

void create_commandbuffers() {
    // Create command pool per device per queueFamilyIndex and command buffer per pool
    // Then begin all command buffers, ready to record batching resource (images) copies
    for (auto deviceItr = s_deviceToCommandPoolMap.begin(); deviceItr != s_deviceToCommandPoolMap.end(); deviceItr++) {
        VkDevice device = deviceItr->first;
        std::unordered_map<uint32_t, VkCommandPool> queueTypeToCommandPoolMap = deviceItr->second;
        for (auto queueItr = queueTypeToCommandPoolMap.begin(); queueItr != queueTypeToCommandPoolMap.end(); queueItr++) {
            uint32_t queueFamilyIndex = queueItr->first;
            VkCommandPool commandPool = queueItr->second;

            // create command pool packets
            const VkCommandPoolCreateInfo cmdPoolCreateInfo = {
                VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, NULL,
                VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queueFamilyIndex};

            vktrace_trace_packet_header *pCreateCommandPoolPacket =
                generate::vkCreateCommandPool(false, device, &cmdPoolCreateInfo, NULL, &commandPool);
            vktrace_write_trace_packet(pCreateCommandPoolPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pCreateCommandPoolPacket);

            // create command buffer packets
            VkCommandBufferAllocateInfo commandBufferAllocateInfo;
            commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            commandBufferAllocateInfo.pNext = NULL;
            commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            commandBufferAllocateInfo.commandBufferCount = 1;
            commandBufferAllocateInfo.commandPool = commandPool;

            VkCommandBuffer commandBuffer = s_deviceToCommandBufferMap[device];

            vktrace_trace_packet_header *pHeader =
                generate::vkAllocateCommandBuffers(false, device, &commandBufferAllocateInfo, &commandBuffer);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }
}

void begin_commandbuffers() {
    // begin all command buffers
    for (auto deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin();
         deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++) {
        VkDevice device = reinterpret_cast<VkDevice>(deviceIter->first);
        VkCommandBuffer commandBuffer = s_deviceToCommandBufferMap[device];

        VkCommandBufferBeginInfo commandBufferBeginInfo;
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = NULL;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        commandBufferBeginInfo.pInheritanceInfo = NULL;

        vktrace_trace_packet_header *pHeader = generate::vkBeginCommandBuffer(false, commandBuffer, &commandBufferBeginInfo);
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);
    }
}

void record_created_images_commands(StateTracker &stateTracker, uint32_t &imgIteratorCount) {
    // record commands in batch (limited by g_trimMaxBatchCmdCount)
    uint32_t batchItrCount = 0;
    for (auto obj = std::next(stateTracker.createdImages.begin(), imgIteratorCount);
         obj != stateTracker.createdImages.end() && batchItrCount < g_trimMaxBatchCmdCount;
         obj++, batchItrCount++, imgIteratorCount++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_Image_object((VkImage)obj->first);
            continue;
        }
        VkImage image = obj->first;
        VkDevice device = obj->second.belongsToDevice;

        if (obj->second.ObjectInfo.Image.mostRecentLayout == VK_IMAGE_LAYOUT_UNDEFINED) {
            // If the current layout is UNDEFINED, that means the app hasn't
            // used it yet, or it doesn't care what is currently
            // in the image and so it will be discarded when the app uses it
            // next. As such, there's no point in us trying to
            // recreate this image.
            continue;
        }

        if (obj->second.ObjectInfo.Image.memorySize != 0) {
            // If the image is not bound to any memory, skip the following
            // process because it base on the assumption of memory binding.

            uint32_t queueFamilyIndex = obj->second.ObjectInfo.Image.queueFamilyIndex;
            if (obj->second.ObjectInfo.Image.sharingMode == VK_SHARING_MODE_CONCURRENT) {
                queueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            }

            if (obj->second.ObjectInfo.Image.needsStagingBuffer) {
                // make a staging buffer and copy the data into the image (similar
                // to what we do for buffers)
                StagingInfo stagingInfo = s_imageToStagedInfoMap[image];
                stagingInfo.bufferCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

                // generate packets needed to create a staging buffer
                if (generateCreateStagingBuffer(device, stagingInfo) == false) {
                    break;
                }

                // here's where we map / unmap to insert data into the buffer
                {
                    // write map / unmap packets so the memory contents gets set on
                    // replay
                    if (obj->second.ObjectInfo.Image.pMapMemoryPacket != NULL) {
                        vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pMapMemoryPacket, vktrace_trace_get_trace_file());
                        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Image.pMapMemoryPacket));
                    }

                    if (obj->second.ObjectInfo.Image.pUnmapMemoryPacket != NULL) {
                        vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pUnmapMemoryPacket, vktrace_trace_get_trace_file());
                        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Image.pUnmapMemoryPacket));
                    }
                }

                // start batching resource copies
                // Transition image to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
                generateTransitionImage(device, stagingInfo.commandBuffer, image, 0, VK_ACCESS_TRANSFER_WRITE_BIT, queueFamilyIndex,
                                        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                        obj->second.ObjectInfo.Image.aspectMask, obj->second.ObjectInfo.Image.arrayLayers,
                                        obj->second.ObjectInfo.Image.mipLevels);

                // issue call to copy buffer
                vktrace_trace_packet_header *pHeader = generate::vkCmdCopyBufferToImage(
                    false, stagingInfo.commandBuffer, stagingInfo.buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    static_cast<uint32_t>(stagingInfo.imageCopyRegions.size()), stagingInfo.imageCopyRegions.data());
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);

                // transition image to final layout
                generateTransitionImage(device, stagingInfo.commandBuffer, image, VK_ACCESS_TRANSFER_WRITE_BIT,
                                        obj->second.ObjectInfo.Image.accessFlags, queueFamilyIndex,
                                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, obj->second.ObjectInfo.Image.mostRecentLayout,
                                        obj->second.ObjectInfo.Image.aspectMask, obj->second.ObjectInfo.Image.arrayLayers,
                                        obj->second.ObjectInfo.Image.mipLevels);
                // end batching resource copies
            } else {
                VkImageLayout initialLayout = obj->second.ObjectInfo.Image.initialLayout;
                VkImageLayout desiredLayout = obj->second.ObjectInfo.Image.mostRecentLayout;

                // Need to make sure images have the correct VkImageLayout.
                if (obj->second.ObjectInfo.Image.bIsSwapchainImage == false) {
                    uint32_t mipLevels = obj->second.ObjectInfo.Image.mipLevels;
                    uint32_t arrayLayers = obj->second.ObjectInfo.Image.arrayLayers;
                    VkFormat format = obj->second.ObjectInfo.Image.format;
                    uint32_t srcAccessMask = (initialLayout == VK_IMAGE_LAYOUT_PREINITIALIZED) ? VK_ACCESS_HOST_WRITE_BIT : 0;
                    VkImageAspectFlags aspectMask = getImageAspectFromFormat(format);

                    uint32_t srcQueueFamilyIndex = queueFamilyIndex;
                    uint32_t dstQueueFamilyIndex = queueFamilyIndex;

                    // Make VkImageMemoryBarrier structs to change the image's
                    // layout
                    VkImageMemoryBarrier imageMemoryBarrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                                                               NULL,
                                                               srcAccessMask,
                                                               0,  // dstAccessMask, determined below
                                                               initialLayout,
                                                               desiredLayout,
                                                               srcQueueFamilyIndex,
                                                               dstQueueFamilyIndex,
                                                               image,
                                                               {aspectMask, 0, mipLevels, 0, arrayLayers}};

                    if (desiredLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
                        // Make sure anything that was copying from this image has completed
                        imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                    }

                    if (desiredLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
                        imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                    }

                    if (desiredLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
                        imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                    }

                    if (desiredLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
                        // Make sure any Copy or CPU writes to image are flushed
                        imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
                    }

                    VkImageMemoryBarrier *pmemory_barrier = &imageMemoryBarrier;

                    VkPipelineStageFlags src_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                    VkPipelineStageFlags dest_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

                    // Use VkCmdPipelineBarrier to transition the images
                    VkCommandBuffer commandBuffer = s_deviceToCommandBufferMap[device];
                    vktrace_trace_packet_header *pCmdPipelineBarrierPacket = generate::vkCmdPipelineBarrier(
                        false, commandBuffer, src_stages, dest_stages, 0, 0, NULL, 0, NULL, 1, pmemory_barrier);
                    vktrace_write_trace_packet(pCmdPipelineBarrierPacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pCmdPipelineBarrierPacket);
                }
            }
        }
    }
}

void record_created_buffers_commands(StateTracker &stateTracker, uint32_t &bufIteratorCount) {
    // record commands in batch (limited by g_trimMaxBatchCmdCount)
    uint32_t batchItrCount = 0;
    for (auto obj = std::next(stateTracker.createdBuffers.begin(), bufIteratorCount);
         obj != stateTracker.createdBuffers.end() && batchItrCount < g_trimMaxBatchCmdCount;
         obj++, batchItrCount++, bufIteratorCount++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_Buffer_object((VkBuffer)obj->first);
            continue;
        }
        VkBuffer buffer = (VkBuffer)obj->first;
        VkDevice device = obj->second.belongsToDevice;

        // CreateBuffer
        assert(obj->second.ObjectInfo.Buffer.pCreatePacket != NULL);
        if (obj->second.ObjectInfo.Buffer.pCreatePacket != NULL) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pCreatePacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Buffer.pCreatePacket));
        }

        if ((obj->second.ObjectInfo.Buffer.pBindBufferMemoryPacket != nullptr) && (obj->second.ObjectInfo.Buffer.size != 0)) {
            // If the buffer is not bound to memory, it might be just created
            // when starting to trim, so the following process should be
            // skipped on the above condition.

            // BindBufferMemory
            if (obj->second.ObjectInfo.Buffer.pBindBufferMemoryPacket != NULL) {
                vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pBindBufferMemoryPacket, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Buffer.pBindBufferMemoryPacket));
            }

            if (obj->second.ObjectInfo.Buffer.needsStagingBuffer) {
                StagingInfo stagingInfo = s_bufferToStagedInfoMap[buffer];
                stagingInfo.bufferCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

                // Generate packets to create the staging buffer
                if (generateCreateStagingBuffer(device, stagingInfo) == false) {
                    break;
                }

                // here's where we map / unmap to insert data into the buffer
                {
                    // write map / unmap packets so the memory contents gets set on
                    // replay
                    if (obj->second.ObjectInfo.Buffer.pMapMemoryPacket != NULL) {
                        vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pMapMemoryPacket, vktrace_trace_get_trace_file());
                        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Buffer.pMapMemoryPacket));
                    }

                    if (obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket != NULL) {
                        vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket,
                                                   vktrace_trace_get_trace_file());
                        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket));
                    }
                }

                // Transition Buffer to be writeable
                generateTransitionBuffer(device, stagingInfo.commandBuffer, buffer, 0, VK_ACCESS_TRANSFER_WRITE_BIT, 0,
                                         obj->second.ObjectInfo.Buffer.size);

                // issue call to copy buffer
                stagingInfo.copyRegion.dstOffset = 0;
                stagingInfo.copyRegion.srcOffset = 0;
                vktrace_trace_packet_header *pHeader = generate::vkCmdCopyBuffer(
                    false, stagingInfo.commandBuffer, stagingInfo.buffer, buffer, 1, &stagingInfo.copyRegion);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);

                // transition buffer to final access mask
                generateTransitionBuffer(device, stagingInfo.commandBuffer, buffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                                         obj->second.ObjectInfo.Buffer.accessFlags, 0, obj->second.ObjectInfo.Buffer.size);

            } else {
                // write map / unmap packets so the memory contents gets set on
                // replay
                if (obj->second.ObjectInfo.Buffer.pMapMemoryPacket != NULL) {
                    vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pMapMemoryPacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Buffer.pMapMemoryPacket));
                }

                if (obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket != NULL) {
                    vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket));
                }
            }
        }
    }
}

void submit_commandbuffers() {
    // End command buffer and submit queues
    for (auto deviceItr = s_deviceToCommandPoolMap.begin(); deviceItr != s_deviceToCommandPoolMap.end(); deviceItr++) {
        VkDevice device = deviceItr->first;
        std::unordered_map<uint32_t, VkCommandPool> queueTypeToCommandPoolMap = deviceItr->second;
        for (auto queueItr = queueTypeToCommandPoolMap.begin(); queueItr != queueTypeToCommandPoolMap.end(); queueItr++) {
            uint32_t queueFamilyIndex = queueItr->first;
            VkCommandBuffer commandBuffer = s_deviceToCommandBufferMap[device];

            vktrace_trace_packet_header *pEndCommandBufferPacket = generate::vkEndCommandBuffer(false, commandBuffer);
            vktrace_write_trace_packet(pEndCommandBufferPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pEndCommandBufferPacket);

            // Queue submit the command buffer
            if (commandBuffer != VK_NULL_HANDLE) {
                // retrieve device queue from queueFamilyIndex
                VkQueue trimQueue = VK_NULL_HANDLE;
                uint32_t queueIndex = 0;  // just using the first queue
                                          // available, we don't yet verify if
                                          // this even exists, just assuming.
                trimQueue = trim::get_DeviceQueue(device, queueFamilyIndex, queueIndex);

                // submit queue
                if (trimQueue != VK_NULL_HANDLE) {
                    VkSubmitInfo submitInfo = {VK_STRUCTURE_TYPE_SUBMIT_INFO, NULL, 0, NULL, NULL, 1, &commandBuffer, 0, NULL};
                    vktrace_trace_packet_header *pQueueSubmitPacket =
                        generate::vkQueueSubmit(false, trimQueue, 1, &submitInfo, VK_NULL_HANDLE);
                    vktrace_write_trace_packet(pQueueSubmitPacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pQueueSubmitPacket);
                }
            }
        }
    }
}

void wait_for_queues() {
    for (auto deviceItr = s_deviceToCommandPoolMap.begin(); deviceItr != s_deviceToCommandPoolMap.end(); deviceItr++) {
        VkDevice device = deviceItr->first;
        std::unordered_map<uint32_t, VkCommandPool> queueTypeToCommandPoolMap = deviceItr->second;
        for (auto queueItr = queueTypeToCommandPoolMap.begin(); queueItr != queueTypeToCommandPoolMap.end(); queueItr++) {
            uint32_t queueFamilyIndex = queueItr->first;
            VkCommandBuffer commandBuffer = s_deviceToCommandBufferMap[device];

            // Queue submit the command buffer
            if (commandBuffer != VK_NULL_HANDLE) {
                // retrieve device queue from queueFamilyIndex
                VkQueue trimQueue = VK_NULL_HANDLE;
                uint32_t queueIndex = 0;  // just using the first queue
                                          // available, we don't yet verify if
                                          // this even exists, just assuming.
                trimQueue = trim::get_DeviceQueue(device, queueFamilyIndex, queueIndex);

                if (trimQueue != VK_NULL_HANDLE) {
                    // wait for queue to finish
                    vktrace_trace_packet_header *pQueueWaitIdlePacket = generate::vkQueueWaitIdle(false, trimQueue);
                    vktrace_write_trace_packet(pQueueWaitIdlePacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pQueueWaitIdlePacket);
                }
            }
        }
    }
}

void destroy_stg_img_resource(StateTracker &stateTracker, uint32_t &destroyImgIteratorCount, uint32_t &imgIteratorCount) {
    // Destroy staging buffers and memory
    for (auto obj = std::next(stateTracker.createdImages.begin(), destroyImgIteratorCount);
         obj != stateTracker.createdImages.end() && destroyImgIteratorCount < imgIteratorCount; obj++, destroyImgIteratorCount++) {
        VkImage image = obj->first;
        VkDevice device = obj->second.belongsToDevice;

        if (obj->second.ObjectInfo.Image.mostRecentLayout == VK_IMAGE_LAYOUT_UNDEFINED) {
            // If the current layout is UNDEFINED, that means the app hasn't
            // used it yet, or it doesn't care what is currently
            // in the image and so it will be discarded when the app uses it
            // next. As such, there's no point in us trying to
            // recreate this image.
            continue;
        }

        if (obj->second.ObjectInfo.Image.memorySize != 0) {
            // If the image is not bound to any memory, skip the following
            // process because it base on the assumption of memory binding.

            uint32_t queueFamilyIndex = obj->second.ObjectInfo.Image.queueFamilyIndex;
            if (obj->second.ObjectInfo.Image.sharingMode == VK_SHARING_MODE_CONCURRENT) {
                queueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            }

            if (obj->second.ObjectInfo.Image.needsStagingBuffer) {
                // retrieve staging info from map
                StagingInfo stagingInfo = s_imageToStagedInfoMap[image];

                // delete staging buffer
                generateDestroyStagingBuffer(device, stagingInfo);
            }
        }
    }
}

void destroy_stg_buf_resource(StateTracker &stateTracker, uint32_t &destroyBufIteratorCount, uint32_t &bufIteratorCount) {
    // Destroy staging buffers and memory
    for (auto obj = std::next(stateTracker.createdBuffers.begin(), destroyBufIteratorCount);
         obj != stateTracker.createdBuffers.end() && destroyBufIteratorCount < bufIteratorCount; obj++, destroyBufIteratorCount++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_Buffer_object((VkBuffer)obj->first);
            continue;
        }
        VkBuffer buffer = (VkBuffer)obj->first;
        VkDevice device = obj->second.belongsToDevice;

        if (0 != obj->second.ObjectInfo.Buffer.size) {
            if (obj->second.ObjectInfo.Buffer.needsStagingBuffer) {
                // retrieve staging info from map
                StagingInfo stagingInfo = s_bufferToStagedInfoMap[buffer];
                // delete staging buffer
                generateDestroyStagingBuffer(device, stagingInfo);
            }
        }
    }
}

void destroy_commandbuffers() {
    // Delete command buffers and command pools
    for (auto deviceItr = s_deviceToCommandPoolMap.begin(); deviceItr != s_deviceToCommandPoolMap.end(); deviceItr++) {
        VkDevice device = deviceItr->first;
        std::unordered_map<uint32_t, VkCommandPool> queueTypeToCommandPoolMap = deviceItr->second;
        for (auto queueItr = queueTypeToCommandPoolMap.begin(); queueItr != queueTypeToCommandPoolMap.end(); queueItr++) {
            VkCommandPool commandPool = queueItr->second;

            // reset or free command buffer
            vktrace_trace_packet_header *pResetCommandPoolPacket =
                generate::vkResetCommandPool(false, device, commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
            vktrace_write_trace_packet(pResetCommandPoolPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pResetCommandPoolPacket);

            // delete command pool
            vktrace_trace_packet_header *pDestroyCommandPoolPacket =
                generate::vkDestroyCommandPool(false, device, commandPool, NULL);
            vktrace_write_trace_packet(pDestroyCommandPoolPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pDestroyCommandPoolPacket);
        }
    }
}

void destroy_commandbuffers_map() {
    // Destroy the command pools / command buffers and fences maps
    for (auto deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin();
         deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++) {
        VkDevice device = reinterpret_cast<VkDevice>(deviceIter->first);
        // update command buffers map
        s_deviceToCommandBufferMap.erase(device);
        // update command pools map
        s_deviceToCommandPoolMap.erase(device);
    }
}

void recreate_images(StateTracker &stateTracker) {
    // First fill in the memory that the image will be associated with
    for (auto obj = stateTracker.createdImages.begin(); obj != stateTracker.createdImages.end(); obj++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            continue;
        }
        // only update the memory for images that don't need a staging buffer
        if (obj->second.ObjectInfo.Image.needsStagingBuffer == false) {
            // write map / unmap packets so the memory contents gets set on
            // replay
            if (obj->second.ObjectInfo.Image.pMapMemoryPacket != NULL) {
                vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pMapMemoryPacket, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Image.pMapMemoryPacket));
            }

            if (obj->second.ObjectInfo.Image.pUnmapMemoryPacket != NULL) {
                vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pUnmapMemoryPacket, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Image.pUnmapMemoryPacket));
            }
        }
    }

#if TRIM_USE_ORDERED_IMAGE_CREATION
    for (auto iter = stateTracker.m_image_calls.begin(); iter != stateTracker.m_image_calls.end(); ++iter) {
        vktrace_write_trace_packet(*iter, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(*iter));
    }
#endif  // TRIM_USE_ORDERED_IMAGE_CREATION

    // The location of following code block which is used to recreate
    // Swapchain must be put after ordered image creation if
    // TRIM_USE_ORDERED_IMAGE_CREATION is enabled. the reason:
    // Let's consider the following calls during capture a title:
    //
    //      vkCreateImage --->the handle value of created image is A
    //      ......
    //      vkDestroyImage ----> destroy A
    //      ......
    //      vkGetSwapchainImagesKHR ----> get an array B of swapchain images
    //
    // for some titles on specific hardware/driver, we can found
    // sometimes A is same value with one element of B. It caused crash
    // problem for trimmed trace file playback if we keep Swapchain recreation
    // before ordered image creation, that's because trim generate following
    // calls:
    //
    //      vkGetSwapchainImagesKHR ----> get an array B of swapchain images
    //      ......
    //      vkCreateImage --->created image handle value is A
    //      ......
    //      vkDestroyImage ----> destroy A
    //
    // During playback, swapchain images will be first put in map, and then
    // A will be put in map, then map item will be deleted when meet
    // vkDestroyImage A, after here, any call which need remap A will get
    // error, but compared with original title, the remap should return that
    // swapchain image.

    // SwapchainKHR
    for (auto iterator = stateTracker.seqSwapchainKHRs.begin(); iterator != stateTracker.seqSwapchainKHRs.end(); ++iterator) {
        auto obj = stateTracker.createdSwapchainKHRs.find(*iterator);
        if (obj == stateTracker.createdSwapchainKHRs.end()) {
            continue;
        }
        vktrace_write_trace_packet(obj->second.ObjectInfo.SwapchainKHR.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.SwapchainKHR.pCreatePacket));

        vktrace_write_trace_packet(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImageCountPacket,
                                   vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImageCountPacket));

        vktrace_write_trace_packet(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImagesPacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImagesPacket));
    }

    for (auto obj = stateTracker.createdImages.begin(); obj != stateTracker.createdImages.end(); obj++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            continue;
        }
#if !TRIM_USE_ORDERED_IMAGE_CREATION
        // CreateImage
        if (obj->second.ObjectInfo.Image.pCreatePacket != NULL) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pCreatePacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Image.pCreatePacket));
        }

        // GetImageMemoryRequirements
        if (obj->second.ObjectInfo.Image.pGetImageMemoryRequirementsPacket != NULL) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pGetImageMemoryRequirementsPacket,
                                       vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Image.pGetImageMemoryRequirementsPacket));
        }
#endif  //! TRIM_USE_ORDERED_IMAGE_CREATION

        // BindImageMemory
        if (obj->second.ObjectInfo.Image.pBindImageMemoryPacket != NULL) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pBindImageMemoryPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Image.pBindImageMemoryPacket));
        }
    }

    vktrace_LogDebug("Recreating Images.");

    // This current approach is:
    // create a command pool on each device only once.
    // create a command buffer in each pool.
    // then batch commands for images:
    // for images need staging buffers,
    // map/unmap to insert data into buffers then
    // transition image to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    // and batch(record) calls to vkCmdCopyBufferToImage
    // for images that do not need staging buffers,
    // batch(record) all the VkImageMemoryBarrier structs into a
    // single call to VkCmdPipelineBarrier (per-device).
    // end the command buffers once
    // queue submit them once
    // and delete them once

    uint32_t imgIteratorCount = 0;
    uint32_t destroyImgIteratorCount = 0;

    // Create command pool per device per queueFamilyIndex and command buffer per pool
    create_commandbuffers();

    while (imgIteratorCount < stateTracker.createdImages.size()) {
        // Begin all command buffers created earlier
        begin_commandbuffers();

        // Batch record commands for created images
        record_created_images_commands(stateTracker, imgIteratorCount);

        // End command buffers and queue submit them
        submit_commandbuffers();

        // Wait for queue idle
        wait_for_queues();

        // Destroy staging buffers and memories for the staging images
        destroy_stg_img_resource(stateTracker, destroyImgIteratorCount, imgIteratorCount);
    }

    // Delete command buffers and command pools created earlier per device per queue
    destroy_commandbuffers();

    vktrace_LogDebug("Recreating Images (Done).");
}

void recreate_imageviews(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdImageViews.begin(); obj != stateTracker.createdImageViews.end(); obj++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_ImageView_object((VkImageView)obj->first);
            continue;
        }
        vktrace_write_trace_packet(obj->second.ObjectInfo.ImageView.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.ImageView.pCreatePacket));
    }
}

void recreate_buffers(StateTracker &stateTracker) {
    vktrace_LogDebug("Recreating Buffers.");

    uint32_t bufIteratorCount = 0;
    uint32_t destroyBufIteratorCount = 0;

    // Create command pool per device per queueFamilyIndex and command buffer per pool
    create_commandbuffers();

    while (bufIteratorCount < stateTracker.createdBuffers.size()) {
        // Begin all command buffers created earlier
        begin_commandbuffers();

        // Batch record commands for created buffers
        record_created_buffers_commands(stateTracker, bufIteratorCount);

        // End command buffers and queue submit them
        submit_commandbuffers();

        // Wait for queue idle
        wait_for_queues();

        // Destroy staging buffers and memories for the staging buffers
        destroy_stg_buf_resource(stateTracker, destroyBufIteratorCount, bufIteratorCount);
    }

    // Delete command buffers and command pools created earlier per device per queue
    destroy_commandbuffers();

    // Destroy the command pools / command buffers and fences maps created earlier in snapshot
    destroy_commandbuffers_map();

    vktrace_LogDebug("Recreating Buffers (Done).");
}

void recreate_device_persistently_map_mem(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdDeviceMemorys.begin(); obj != stateTracker.createdDeviceMemorys.end(); obj++) {
        if (obj->second.ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket != NULL) {
            vktrace_write_trace_packet(obj->second.ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket,
                                       vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket));
        }
    }
}

void recreate_bufferviews(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdBufferViews.begin(); obj != stateTracker.createdBufferViews.end(); obj++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_BufferView_object((VkBufferView)obj->first);
            continue;
        }
        vktrace_write_trace_packet(obj->second.ObjectInfo.BufferView.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.BufferView.pCreatePacket));
    }
}

void recreate_samplers(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdSamplers.begin(); obj != stateTracker.createdSamplers.end(); obj++) {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Sampler.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Sampler.pCreatePacket));
    }
}

void recreate_descriptor_set_layout(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdDescriptorSetLayouts.begin(); obj != stateTracker.createdDescriptorSetLayouts.end();
         obj++) {
        vktrace_write_trace_packet(obj->second.ObjectInfo.DescriptorSetLayout.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.DescriptorSetLayout.pCreatePacket));
    }
}

void recreate_pipeline_layout(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdPipelineLayouts.begin(); obj != stateTracker.createdPipelineLayouts.end(); obj++) {
        vktrace_write_trace_packet(obj->second.ObjectInfo.PipelineLayout.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.PipelineLayout.pCreatePacket));
    }
}

void recreate_renderpass(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdRenderPasss.begin(); obj != stateTracker.createdRenderPasss.end(); obj++) {
        vktrace_write_trace_packet(obj->second.ObjectInfo.RenderPass.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.RenderPass.pCreatePacket));
    }
}

void recreate_shader_modules(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdShaderModules.begin(); obj != stateTracker.createdShaderModules.end(); obj++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_ShaderModule_object((VkShaderModule)obj->first);
            continue;
        }
        VkShaderModule shaderModule = static_cast<VkShaderModule>(obj->first);
        vktrace_trace_packet_header *pHeader =
            generate::vkCreateShaderModule(false, obj->second.belongsToDevice, &obj->second.ObjectInfo.ShaderModule.createInfo,
                                           obj->second.ObjectInfo.ShaderModule.pAllocator, &shaderModule);
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);
    }
}

void recreate_pipeline_caches(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdPipelineCaches.begin(); obj != stateTracker.createdPipelineCaches.end(); obj++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_PipelineCache_object((VkPipelineCache)obj->first);
            continue;
        }
        vktrace_write_trace_packet(obj->second.ObjectInfo.PipelineCache.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.PipelineCache.pCreatePacket));
    }
}

void recreate_pipelines(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdPipelines.begin(); obj != stateTracker.createdPipelines.end(); obj++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_Pipeline_object((VkPipeline)obj->first);
            continue;
        }
        VkPipeline pipeline = obj->first;
        VkDevice device = obj->second.belongsToDevice;
        VkPipelineCache pipelineCache = obj->second.ObjectInfo.Pipeline.pipelineCache;
        vktrace_trace_packet_header *pHeader = nullptr;

        // Create necessary shader modules
        for (uint32_t moduleIndex = 0; moduleIndex < obj->second.ObjectInfo.Pipeline.shaderModuleCreateInfoCount; moduleIndex++) {
            VkShaderModule module = (obj->second.ObjectInfo.Pipeline.isGraphicsPipeline)
                                        ? obj->second.ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pStages[moduleIndex].module
                                        : obj->second.ObjectInfo.Pipeline.computePipelineCreateInfo.stage.module;

            auto shaderModuleObj = stateTracker.createdShaderModules.find(module);
            if (shaderModuleObj == stateTracker.createdShaderModules.end() ||
                (g_trimPostProcess && !shaderModuleObj->second.bReferencedInTrim)) {
                // the shader module does not yet exist, so create it specifically for this pipeline
                vktrace_trace_packet_header *pCreateShaderModule = generate::vkCreateShaderModule(
                    false, device, &obj->second.ObjectInfo.Pipeline.pShaderModuleCreateInfos[moduleIndex], nullptr, &module);
                vktrace_write_trace_packet(pCreateShaderModule, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pCreateShaderModule);
            }
        }

        // Create appropriate pipeline object
        if (obj->second.ObjectInfo.Pipeline.isGraphicsPipeline) {
            // Sometimes the original RenderPass is deleted, and a new one is
            // created that has the same handle, even if it has different
            // properties (rendertargets, etc). I call each of these a different
            // "version" of the RenderPass. If the Pipeline wasn't deleted
            // when the RenderPass was deleted, then we may have pipelines that
            // were created based on an older "version" of the RenderPass.
            VkRenderPass originalRenderPass = obj->second.ObjectInfo.Pipeline.graphicsPipelineCreateInfo.renderPass;
            uint32_t thisRenderPassVersion = obj->second.ObjectInfo.Pipeline.renderPassVersion;
            uint32_t latestVersion = stateTracker.get_RenderPassVersion(originalRenderPass);

            trim::ObjectInfo *pRenderPass = stateTracker.get_RenderPass(originalRenderPass);
            if (thisRenderPassVersion < latestVersion || pRenderPass == nullptr) {
                // Actually recreate the old RenderPass to get a new handle to
                // supply to the pipeline creation call
                VkRenderPassCreateInfo *pRPCreateInfo =
                    stateTracker.get_RenderPassCreateInfo(originalRenderPass, thisRenderPassVersion);
                vktrace_trace_packet_header *pCreateRenderPass = trim::generate::vkCreateRenderPass(
                    true, device, pRPCreateInfo, nullptr, &obj->second.ObjectInfo.Pipeline.graphicsPipelineCreateInfo.renderPass);
                vktrace_write_trace_packet(pCreateRenderPass, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pCreateRenderPass);
            }

            pHeader = trim::generate::vkCreateGraphicsPipelines(
                false, device, pipelineCache, 1, &obj->second.ObjectInfo.Pipeline.graphicsPipelineCreateInfo, nullptr, &pipeline);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            if (thisRenderPassVersion < latestVersion || pRenderPass == nullptr) {
                vktrace_trace_packet_header *pDestroyRenderPass = generate::vkDestroyRenderPass(
                    true, device, obj->second.ObjectInfo.Pipeline.graphicsPipelineCreateInfo.renderPass, nullptr);
                vktrace_write_trace_packet(pDestroyRenderPass, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pDestroyRenderPass);
            }
        } else {
            pHeader = trim::generate::vkCreateComputePipelines(
                false, device, pipelineCache, 1, &obj->second.ObjectInfo.Pipeline.computePipelineCreateInfo, nullptr, &pipeline);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // Destroy ShaderModule objects
        for (uint32_t moduleIndex = 0; moduleIndex < obj->second.ObjectInfo.Pipeline.shaderModuleCreateInfoCount; moduleIndex++) {
            VkShaderModule module = (obj->second.ObjectInfo.Pipeline.isGraphicsPipeline)
                                        ? obj->second.ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pStages[moduleIndex].module
                                        : obj->second.ObjectInfo.Pipeline.computePipelineCreateInfo.stage.module;

            auto shaderModuleObj = stateTracker.createdShaderModules.find(module);
            if (shaderModuleObj == stateTracker.createdShaderModules.end() ||
                (g_trimPostProcess && !shaderModuleObj->second.bReferencedInTrim)) {
                // the shader module did not previously exist, so delete it.
                vktrace_trace_packet_header *pDestroyShaderModule = generate::vkDestroyShaderModule(false, device, module, nullptr);
                vktrace_write_trace_packet(pDestroyShaderModule, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pDestroyShaderModule);
            }
        }
    }
}

void recreate_descriptor_pools(StateTracker &stateTracker) {
    for (auto poolObj = stateTracker.createdDescriptorPools.begin(); poolObj != stateTracker.createdDescriptorPools.end();
         poolObj++) {
        if (g_trimPostProcess && !poolObj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_DescriptorPool_object((VkDescriptorPool)poolObj->first);
            continue;
        }
        // write the createDescriptorPool packet
        vktrace_write_trace_packet(poolObj->second.ObjectInfo.DescriptorPool.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(poolObj->second.ObjectInfo.DescriptorPool.pCreatePacket));

        if (poolObj->second.ObjectInfo.DescriptorPool.numSets > 0) {
            // now allocate all DescriptorSets that are part of this pool
            uint64_t vktraceStartTime = vktrace_get_time();
            VkDescriptorSetAllocateInfo allocateInfo;
            allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocateInfo.pNext = NULL;
            allocateInfo.descriptorPool = poolObj->first;

            VkDescriptorSetLayout *pSetLayouts = new VkDescriptorSetLayout[poolObj->second.ObjectInfo.DescriptorPool.numSets];
            allocateInfo.pSetLayouts = pSetLayouts;
            VkDescriptorSet *pDescriptorSets = new VkDescriptorSet[poolObj->second.ObjectInfo.DescriptorPool.numSets];
            uint32_t setIndex = 0;

            for (auto setObj = stateTracker.createdDescriptorSets.begin(); setObj != stateTracker.createdDescriptorSets.end();
                 setObj++) {
                // get descriptorSetLayouts and DescriptorSets specific to this
                // pool
                if (g_trimPostProcess && !setObj->second.bReferencedInTrim) {
                    // TODO: Find a way to not remove from s_trimGlobalStateTracker.
                    trim::remove_DescriptorSet_object((VkDescriptorSet)setObj->first);
                    continue;
                }
                if (setObj->second.ObjectInfo.DescriptorSet.descriptorPool == allocateInfo.descriptorPool &&
                    setIndex < poolObj->second.ObjectInfo.DescriptorPool.numSets) {
                    pSetLayouts[setIndex] = setObj->second.ObjectInfo.DescriptorSet.layout;
                    pDescriptorSets[setIndex] = setObj->first;
                    setIndex++;
                }
            }

            // update descriptorSetCount to ensure it is correct
            allocateInfo.descriptorSetCount = setIndex;

            VkDevice device = poolObj->second.belongsToDevice;

            vktrace_trace_packet_header *pHeader =
                generate::vkAllocateDescriptorSets(false, device, &allocateInfo, pDescriptorSets);
            pHeader->vktrace_begin_time = vktraceStartTime;
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(pHeader));

            delete[] pSetLayouts;
            delete[] pDescriptorSets;
        }
    }
}

void recreate_descriptor_sets(StateTracker &stateTracker) {
    // Update DescriptorSets
    // needs to be done per-Device
    for (auto deviceObj = stateTracker.createdDevices.begin(); deviceObj != stateTracker.createdDevices.end(); deviceObj++) {
        for (auto setObj = stateTracker.createdDescriptorSets.begin(); setObj != stateTracker.createdDescriptorSets.end();
             setObj++) {
            if (g_trimPostProcess && !setObj->second.bReferencedInTrim) {
                // TODO: Find a way to not remove from s_trimGlobalStateTracker.
                trim::remove_DescriptorSet_object((VkDescriptorSet)setObj->first);
                continue;
            }
            if (setObj->second.belongsToDevice == (VkDevice)deviceObj->first) {
                // when trim track vkAllocateDescriptorSets, it create numBindings
                // WriteDescriptorSets and CopyDescriptorSets to record the binding
                // change, here we will use these recorded data of changed binding
                // to generate vkUpdateDescriptorSets packet for playback to
                // update/restore the descriptorSets state when starting trim.
                // descriptorWriteCount and descriptorCopyCount all <= numBindings.
                // they are used to indicate so far how many bindings of this
                // descriptorset has been updated, if we start to trim at a location
                // close to title's beginning, that's very possible not all bindings
                // has been updated.
                uint32_t descriptorWriteCount = setObj->second.ObjectInfo.DescriptorSet.writeDescriptorCount;
                uint32_t descriptorCopyCount = setObj->second.ObjectInfo.DescriptorSet.copyDescriptorCount;
                if (descriptorWriteCount == 0 && descriptorCopyCount == 0) {
                    continue;
                }
                VkWriteDescriptorSet *pDescriptorWrites = setObj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets;
                VkCopyDescriptorSet *pDescriptorCopies = setObj->second.ObjectInfo.DescriptorSet.pCopyDescriptorSets;

                // In trim, we track target application descriptorset state when it
                // call vkUpdateDescriptorSets to update descriptorset. By this way
                // , trim maintain the every binding states of the descriptorset.
                // when start to trim (press hotkey or reach frame range), trim
                // dump the current descriptorset state to trace file.
                //
                // Because the tracking time and trim starting time is different,
                // it's possible that some descriptors arenot valid anymore at trim
                // starting time although valid at tracking time.
                //
                // For the target title in XCAP-759, some binding has thousands of
                // descriptors to be updated, and at trim starting time, lots of them
                // are already invalid and located sparsly.
                //
                // During playback, these invalid descriptors cause vkreplay skip
                // related vkUpdateDescriptorSets call, and if we let vkreplay ignore
                // and do those callS, it cause crash inside some driver.
                //
                // By Doc, descriptors must be valid in vkUpdateDescriptorSets call,
                // so here, before we generate the update call, we check every
                // descriptor in pDescriptorWrites and remove all invalid descriptors.
                std::vector<uint32_t> descriptorCountsBackup(descriptorWriteCount);
                for (int i = 0; i < descriptorWriteCount; i++) {
                    descriptorCountsBackup[i] = pDescriptorWrites[i].descriptorCount;
                }
                UpdateInvalidDescriptors(descriptorWriteCount, pDescriptorWrites);
                vktrace_trace_packet_header *pHeader =
                    generate::vkUpdateDescriptorSets(false, setObj->second.belongsToDevice, descriptorWriteCount, pDescriptorWrites,
                                                     descriptorCopyCount, pDescriptorCopies);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);
                for (int i = 0; i < descriptorWriteCount; i++) {
                    pDescriptorWrites[i].descriptorCount = descriptorCountsBackup[i];
                }
            }
        }
    }
}

void recreate_framebuffers(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdFramebuffers.begin(); obj != stateTracker.createdFramebuffers.end(); obj++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_Framebuffer_object((VkFramebuffer)obj->first);
            continue;
        }
        vktrace_write_trace_packet(obj->second.ObjectInfo.Framebuffer.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Framebuffer.pCreatePacket));
    }
}

void recreate_semaphores(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdSemaphores.begin(); obj != stateTracker.createdSemaphores.end(); obj++) {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Semaphore.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Semaphore.pCreatePacket));
    }
}

void recreate_fences(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdFences.begin(); obj != stateTracker.createdFences.end(); obj++) {
        VkDevice device = obj->second.belongsToDevice;
        VkFence fence = obj->first;
        VkAllocationCallbacks *pAllocator = get_Allocator(obj->second.ObjectInfo.Fence.pAllocator);

        VkFenceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = (obj->second.ObjectInfo.Fence.signaled) ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

        vktrace_trace_packet_header *pCreateFence = generate::vkCreateFence(false, device, &createInfo, pAllocator, &fence);
        vktrace_write_trace_packet(pCreateFence, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(pCreateFence));
    }
}

void recreate_events(StateTracker &stateTracker) {
    VkResult result = VK_EVENT_RESET;
    vktrace_trace_packet_header *pheader = nullptr;
    for (auto obj = stateTracker.createdEvents.begin(); obj != stateTracker.createdEvents.end(); obj++) {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Event.pCreatePacket, vktrace_trace_get_trace_file());
        auto eventobject = obj->second.vkObject;
        result = mdd(reinterpret_cast<void *>(obj->second.belongsToDevice))
                     ->devTable.GetEventStatus(obj->second.belongsToDevice, FromHandleId<VkEvent>(eventobject));
        switch (result) {
            case VK_EVENT_SET:
                pheader = generate::vkSetEvent(false, obj->second.belongsToDevice, FromHandleId<VkEvent>(eventobject));
                break;
            default:
                break;
        }
        if (pheader != nullptr) {
            vktrace_write_trace_packet(pheader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet_no_lock(&(pheader));
        }
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.Event.pCreatePacket));
    }
}

void recreate_query_pools(StateTracker &stateTracker) {
    for (auto obj = stateTracker.createdQueryPools.begin(); obj != stateTracker.createdQueryPools.end(); obj++) {
        if (g_trimPostProcess && !obj->second.bReferencedInTrim) {
            // TODO: Find a way to not remove from s_trimGlobalStateTracker.
            trim::remove_QueryPool_object((VkQueryPool)obj->first);
            continue;
        }
        vktrace_write_trace_packet(obj->second.ObjectInfo.QueryPool.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.QueryPool.pCreatePacket));

        VkCommandBuffer commandBuffer = obj->second.ObjectInfo.QueryPool.commandBuffer;

        if (commandBuffer != VK_NULL_HANDLE) {
            VkQueryPool queryPool = obj->first;

            VkCommandBufferBeginInfo beginInfo;
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.pNext = nullptr;
            beginInfo.pInheritanceInfo = nullptr;
            beginInfo.flags = 0;
            vktrace_trace_packet_header *pBeginCB = generate::vkBeginCommandBuffer(false, commandBuffer, &beginInfo);
            vktrace_write_trace_packet(pBeginCB, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pBeginCB);

            vktrace_trace_packet_header *pResetPacket =
                generate::vkCmdResetQueryPool(false, commandBuffer, queryPool, 0, obj->second.ObjectInfo.QueryPool.size);
            vktrace_write_trace_packet(pResetPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pResetPacket);

            // Go through each query and start / stop if needed.
            for (uint32_t i = 0; i < obj->second.ObjectInfo.QueryPool.size; i++) {
                if (obj->second.ObjectInfo.QueryPool.pResultsAvailable[i]) {
                    if (obj->second.ObjectInfo.QueryPool.queryType == VK_QUERY_TYPE_TIMESTAMP) {
                        // Since were faking the query result anyway, simply use
                        // the TOP_OF_PIPE as we aren't actually drawing
                        // anything.
                        vktrace_trace_packet_header *pWriteTimestamp =
                            generate::vkCmdWriteTimestamp(false, commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, queryPool, i);
                        vktrace_write_trace_packet(pWriteTimestamp, vktrace_trace_get_trace_file());
                        vktrace_delete_trace_packet(&pWriteTimestamp);
                    } else {
                        // This query needs to be begin-ended to make a
                        // queryable result.
                        // Note that by doing this, the initial results will be
                        // incorrect,
                        // so the first frame of the replay may be incorrect.
                        VkQueryControlFlags flags = 0;
                        vktrace_trace_packet_header *pBeginQuery =
                            generate::vkCmdBeginQuery(false, commandBuffer, queryPool, i, flags);
                        vktrace_write_trace_packet(pBeginQuery, vktrace_trace_get_trace_file());
                        vktrace_delete_trace_packet(&pBeginQuery);

                        vktrace_trace_packet_header *pEndQuery = generate::vkCmdEndQuery(false, commandBuffer, queryPool, i);
                        vktrace_write_trace_packet(pEndQuery, vktrace_trace_get_trace_file());
                        vktrace_delete_trace_packet(&pEndQuery);
                    }
                }
            }

            vktrace_trace_packet_header *pEndCB = generate::vkEndCommandBuffer(false, commandBuffer);
            vktrace_write_trace_packet(pEndCB, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pEndCB);

            ObjectInfo *cbInfo = s_trimStateTrackerSnapshot.get_CommandBuffer(commandBuffer);
            VkQueue queue = cbInfo->ObjectInfo.CommandBuffer.submitQueue;

            VkSubmitInfo submitInfo;
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.pNext = NULL;
            submitInfo.waitSemaphoreCount = 0;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;
            submitInfo.pSignalSemaphores = NULL;
            submitInfo.signalSemaphoreCount = 0;
            submitInfo.pWaitDstStageMask = NULL;
            submitInfo.pWaitSemaphores = NULL;

            vktrace_trace_packet_header *pQueueSubmit = generate::vkQueueSubmit(false, queue, 1, &submitInfo, VK_NULL_HANDLE);
            vktrace_write_trace_packet(pQueueSubmit, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pQueueSubmit);

            vktrace_trace_packet_header *pQueueWait = generate::vkQueueWaitIdle(false, queue);
            vktrace_write_trace_packet(pQueueWait, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pQueueWait);
        }
    }
}

void recreate_allocated_command_buffers(StateTracker &stateTracker) {
    // write out the packets to recreate the command buffers that were allocated
    vktrace_enter_critical_section(&trimCommandBufferPacketLock);
    // Secondary command buffers should be replayed before primary command buffers.
    // 1. Go through secondary command buffers
    for (auto cmdBuffer = stateTracker.createdCommandBuffers.begin(); cmdBuffer != stateTracker.createdCommandBuffers.end();
         ++cmdBuffer) {
        if (g_trimPostProcess && !cmdBuffer->second.bReferencedInTrim) {
            continue;
        }
        if (cmdBuffer->second.ObjectInfo.CommandBuffer.level == VK_COMMAND_BUFFER_LEVEL_SECONDARY) {
            std::list<vktrace_trace_packet_header *> &packets = stateTracker.m_cmdBufferPackets[(VkCommandBuffer)cmdBuffer->first];

            for (std::list<vktrace_trace_packet_header *>::iterator packet = packets.begin(); packet != packets.end(); ++packet) {
                vktrace_trace_packet_header *pHeader = *packet;
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet_no_lock(&pHeader);
            }
            packets.clear();
        }
    }
    // 2. Go through primary command buffers
    for (auto cmdBuffer = stateTracker.createdCommandBuffers.begin(); cmdBuffer != stateTracker.createdCommandBuffers.end();
         ++cmdBuffer) {
        if (g_trimPostProcess && !cmdBuffer->second.bReferencedInTrim) {
            continue;
        }
        if (cmdBuffer->second.ObjectInfo.CommandBuffer.level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
            std::list<vktrace_trace_packet_header *> &packets = stateTracker.m_cmdBufferPackets[(VkCommandBuffer)cmdBuffer->first];

            for (std::list<vktrace_trace_packet_header *>::iterator packet = packets.begin(); packet != packets.end(); ++packet) {
                vktrace_trace_packet_header *pHeader = *packet;
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet_no_lock(&pHeader);
            }
            packets.clear();
        }
    }
    vktrace_leave_critical_section(&trimCommandBufferPacketLock);
}

void recreate_signalled_semaphores(StateTracker &stateTracker) {
    // Collect semaphores that need signaling
    size_t maxSemaphores = stateTracker.createdSemaphores.size();
    uint32_t signalSemaphoreCount = 0;
    VkSemaphore *pSignalSemaphores = VKTRACE_NEW_ARRAY(VkSemaphore, maxSemaphores);
    for (auto obj = stateTracker.createdSemaphores.begin(); obj != stateTracker.createdSemaphores.end(); obj++) {
        VkQueue queue = obj->second.ObjectInfo.Semaphore.signaledOnQueue;
        if (queue != VK_NULL_HANDLE) {
            VkSemaphore semaphore = obj->first;
            pSignalSemaphores[signalSemaphoreCount++] = semaphore;

            VkSubmitInfo submit_info;
            submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submit_info.pNext = NULL;
            submit_info.waitSemaphoreCount = 0;
            submit_info.pWaitSemaphores = NULL;
            submit_info.pWaitDstStageMask = NULL;
            submit_info.commandBufferCount = 0;
            submit_info.pCommandBuffers = NULL;
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores = &semaphore;

            vktrace_trace_packet_header *pHeader = generate::vkQueueSubmit(false, queue, 1, &submit_info, VK_NULL_HANDLE);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        } else {
            // The semaphore is not signaled by queue related calls like
            // vkQueueSubmit and vkQueueBindSparse, here we continue
            // to check if it is signaled by swapchain related calls like
            // vkAcquireNextImageKHR.
            if (obj->second.ObjectInfo.Semaphore.signaledOnSwapChain != VK_NULL_HANDLE) {
                // The Semaphore is signaled by swapchain related calls
                // so here we need to signal the recreated semaphore.

                VkSemaphore semaphore = obj->first;

                VkQueue queue = trim::get_DeviceQueue(obj->second.belongsToDevice, 0, 0);

                // generate a queue submit to signal the semaphore
                VkSubmitInfo submit_info;
                submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submit_info.pNext = NULL;
                submit_info.waitSemaphoreCount = 0;
                submit_info.pWaitSemaphores = NULL;
                submit_info.pWaitDstStageMask = NULL;
                submit_info.commandBufferCount = 0;
                submit_info.pCommandBuffers = NULL;
                submit_info.signalSemaphoreCount = 1;
                submit_info.pSignalSemaphores = &semaphore;

                vktrace_trace_packet_header *pHeader = generate::vkQueueSubmit(false, queue, 1, &submit_info, VK_NULL_HANDLE);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);

                // generate vkWaitQueueIdle() to make sure the semaphore is signaled.
                vktrace_trace_packet_header *pQueueWaitIdlePacket = generate::vkQueueWaitIdle(false, queue);
                vktrace_write_trace_packet(pQueueWaitIdlePacket, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pQueueWaitIdlePacket);
            }
        }
    }

    VKTRACE_DELETE(pSignalSemaphores);
}

void recreate_descriptor_update_templates(StateTracker &stateTracker) {
    // DescriptorUpdateTemplate
    for (auto obj = stateTracker.createdDescriptorUpdateTemplates.begin();
         obj != stateTracker.createdDescriptorUpdateTemplates.end(); obj++) {
        vktrace_write_trace_packet(obj->second.ObjectInfo.DescriptorUpdateTemplate.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(obj->second.ObjectInfo.DescriptorUpdateTemplate.pCreatePacket));
    }
}

void recreate_in_trim_objects() {
    // Recreate in-trim objects
    vktrace_enter_critical_section(&trimStateTrackerLock);
    for (auto iter = s_trimGlobalStateTracker.m_inTrim_calls.begin(); iter != s_trimGlobalStateTracker.m_inTrim_calls.end();
         ++iter) {
        vktrace_write_trace_packet(*iter, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet_no_lock(&(*iter));
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
// Recreate all objects
//=========================================================================
void write_all_referenced_object_calls() {
    if (g_trimPostProcess) {
        vktrace_LogDebug("vktrace marking references before recreating objects for trim.");

        // Mark reference for objects being used by referenced command buffer.
        for (auto obj = s_trimStateTrackerSnapshot.createdCommandBuffers.begin();
             obj != s_trimStateTrackerSnapshot.createdCommandBuffers.end(); ++obj) {
            if (obj->second.bReferencedInTrim) {
                std::list<vktrace_trace_packet_header *> &packets =
                    s_trimStateTrackerSnapshot.m_cmdBufferPackets[(VkCommandBuffer)obj->first];
                for (std::list<vktrace_trace_packet_header *>::iterator packet = packets.begin(); packet != packets.end();
                     ++packet) {
                    vktrace_trace_packet_header *pHeader = *packet;
                    switch (pHeader->packet_id) {
                        case VKTRACE_TPI_VK_vkCmdBindPipeline: {
                            packet_vkCmdBindPipeline *pPacket = (packet_vkCmdBindPipeline *)(pHeader->pBody);
                            if (pPacket->pipeline != VK_NULL_HANDLE) {
                                trim::mark_Pipeline_reference(pPacket->pipeline);
                            }
                        } break;
                        case VKTRACE_TPI_VK_vkCmdBindDescriptorSets: {
                            packet_vkCmdBindDescriptorSets *pPacket = (packet_vkCmdBindDescriptorSets *)(pHeader->pBody);
                            const VkDescriptorSet *pDescriptorSets =
                                (const VkDescriptorSet *)vktrace_trace_packet_interpret_buffer_pointer(
                                    pHeader, (intptr_t)pPacket->pDescriptorSets);
                            if (pPacket->descriptorSetCount != 0 && pDescriptorSets != NULL) {
                                for (uint32_t i = 0; i < pPacket->descriptorSetCount; i++) {
                                    trim::mark_DescriptorSet_reference(pDescriptorSets[i]);
                                }
                            }
                        } break;
                        case VKTRACE_TPI_VK_vkBeginCommandBuffer: {
                            if (obj->second.ObjectInfo.CommandBuffer.level == VK_COMMAND_BUFFER_LEVEL_SECONDARY) {
                                packet_vkBeginCommandBuffer *pPacket = (packet_vkBeginCommandBuffer *)(pHeader->pBody);
                                const VkCommandBufferBeginInfo *pBeginInfo =
                                    (const VkCommandBufferBeginInfo *)vktrace_trace_packet_interpret_buffer_pointer(
                                        pHeader, (intptr_t)pPacket->pBeginInfo);
                                if (pBeginInfo != NULL && pBeginInfo->pInheritanceInfo) {
                                    VkCommandBufferInheritanceInfo *pInheritanceInfo =
                                        (VkCommandBufferInheritanceInfo *)vktrace_trace_packet_interpret_buffer_pointer(
                                            pHeader, (intptr_t)pPacket->pBeginInfo->pInheritanceInfo);
                                    if (pInheritanceInfo) {
                                        trim::mark_Framebuffer_reference(pInheritanceInfo->framebuffer);
                                    }
                                }
                            }
                        } break;
                        case VKTRACE_TPI_VK_vkCmdBeginRenderPass: {
                            packet_vkCmdBeginRenderPass *pPacket = (packet_vkCmdBeginRenderPass *)(pHeader->pBody);
                            const VkRenderPassBeginInfo *pRenderPassBegin =
                                (const VkRenderPassBeginInfo *)vktrace_trace_packet_interpret_buffer_pointer(
                                    pHeader, (intptr_t)pPacket->pRenderPassBegin);
                            if (pRenderPassBegin != NULL) {
                                trim::mark_Framebuffer_reference(pRenderPassBegin->framebuffer);
                            }
                        } break;
                        case VKTRACE_TPI_VK_vkCmdBeginQuery: {
                            packet_vkCmdBeginQuery *pPacket = (packet_vkCmdBeginQuery *)(pHeader->pBody);
                            trim::mark_QueryPool_reference(pPacket->queryPool);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdEndQuery: {
                            packet_vkCmdEndQuery *pPacket = (packet_vkCmdEndQuery *)(pHeader->pBody);
                            trim::mark_QueryPool_reference(pPacket->queryPool);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdResetQueryPool: {
                            packet_vkCmdResetQueryPool *pPacket = (packet_vkCmdResetQueryPool *)(pHeader->pBody);
                            trim::mark_QueryPool_reference(pPacket->queryPool);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdWriteTimestamp: {
                            packet_vkCmdWriteTimestamp *pPacket = (packet_vkCmdWriteTimestamp *)(pHeader->pBody);
                            trim::mark_QueryPool_reference(pPacket->queryPool);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdCopyQueryPoolResults: {
                            packet_vkCmdCopyQueryPoolResults *pPacket = (packet_vkCmdCopyQueryPoolResults *)(pHeader->pBody);
                            trim::mark_QueryPool_reference(pPacket->queryPool);
                            trim::mark_Buffer_reference(pPacket->dstBuffer);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdWaitEvents: {
                            packet_vkCmdWaitEvents *pPacket = (packet_vkCmdWaitEvents *)(pHeader->pBody);
                            const VkBufferMemoryBarrier *pBufferMemoryBarriers =
                                (const VkBufferMemoryBarrier *)vktrace_trace_packet_interpret_buffer_pointer(
                                    pHeader, (intptr_t)pPacket->pBufferMemoryBarriers);
                            if (pBufferMemoryBarriers != NULL) {
                                for (uint32_t i = 0; i < pPacket->bufferMemoryBarrierCount; i++) {
                                    trim::mark_Buffer_reference(pBufferMemoryBarriers[i].buffer);
                                }
                            }
                            const VkImageMemoryBarrier *pImageMemoryBarriers =
                                (const VkImageMemoryBarrier *)vktrace_trace_packet_interpret_buffer_pointer(
                                    pHeader, (intptr_t)pPacket->pImageMemoryBarriers);
                            if (pImageMemoryBarriers != NULL) {
                                for (uint32_t i = 0; i < pPacket->imageMemoryBarrierCount; i++) {
                                    trim::mark_Image_reference(pImageMemoryBarriers[i].image);
                                }
                            }
                        } break;
                        case VKTRACE_TPI_VK_vkCmdPipelineBarrier: {
                            packet_vkCmdPipelineBarrier *pPacket = (packet_vkCmdPipelineBarrier *)(pHeader->pBody);
                            const VkBufferMemoryBarrier *pBufferMemoryBarriers =
                                (const VkBufferMemoryBarrier *)vktrace_trace_packet_interpret_buffer_pointer(
                                    pHeader, (intptr_t)pPacket->pBufferMemoryBarriers);
                            if (pBufferMemoryBarriers != NULL) {
                                for (uint32_t i = 0; i < pPacket->bufferMemoryBarrierCount; i++) {
                                    trim::mark_Buffer_reference(pBufferMemoryBarriers[i].buffer);
                                }
                            }
                            const VkImageMemoryBarrier *pImageMemoryBarriers =
                                (const VkImageMemoryBarrier *)vktrace_trace_packet_interpret_buffer_pointer(
                                    pHeader, (intptr_t)pPacket->pImageMemoryBarriers);
                            if (pImageMemoryBarriers != NULL) {
                                for (uint32_t i = 0; i < pPacket->imageMemoryBarrierCount; i++) {
                                    trim::mark_Image_reference(pImageMemoryBarriers[i].image);
                                }
                            }
                        } break;
                        case VKTRACE_TPI_VK_vkCmdBindIndexBuffer: {
                            packet_vkCmdBindIndexBuffer *pPacket = (packet_vkCmdBindIndexBuffer *)(pHeader->pBody);
                            trim::mark_Buffer_reference(pPacket->buffer);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdBindVertexBuffers: {
                            packet_vkCmdBindVertexBuffers *pPacket = (packet_vkCmdBindVertexBuffers *)(pHeader->pBody);
                            const VkBuffer *pBuffers = (const VkBuffer *)vktrace_trace_packet_interpret_buffer_pointer(
                                pHeader, (intptr_t)pPacket->pBuffers);
                            if (pBuffers != NULL) {
                                for (uint32_t i = 0; i < pPacket->bindingCount; i++) {
                                    trim::mark_Buffer_reference(pBuffers[i]);
                                }
                            }
                        } break;
                        case VKTRACE_TPI_VK_vkCmdDrawIndirect: {
                            packet_vkCmdDrawIndirect *pPacket = (packet_vkCmdDrawIndirect *)(pHeader->pBody);
                            trim::mark_Buffer_reference(pPacket->buffer);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdDrawIndexedIndirect: {
                            packet_vkCmdDrawIndexedIndirect *pPacket = (packet_vkCmdDrawIndexedIndirect *)(pHeader->pBody);
                            trim::mark_Buffer_reference(pPacket->buffer);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdDispatchIndirect: {
                            packet_vkCmdDispatchIndirect *pPacket = (packet_vkCmdDispatchIndirect *)(pHeader->pBody);
                            trim::mark_Buffer_reference(pPacket->buffer);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdCopyBuffer: {
                            packet_vkCmdCopyBuffer *pPacket = (packet_vkCmdCopyBuffer *)(pHeader->pBody);
                            trim::mark_Buffer_reference(pPacket->srcBuffer);
                            trim::mark_Buffer_reference(pPacket->dstBuffer);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdCopyBufferToImage: {
                            packet_vkCmdCopyBufferToImage *pPacket = (packet_vkCmdCopyBufferToImage *)(pHeader->pBody);
                            trim::mark_Buffer_reference(pPacket->srcBuffer);
                            trim::mark_Image_reference(pPacket->dstImage);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdCopyImageToBuffer: {
                            packet_vkCmdCopyImageToBuffer *pPacket = (packet_vkCmdCopyImageToBuffer *)(pHeader->pBody);
                            trim::mark_Buffer_reference(pPacket->dstBuffer);
                            trim::mark_Image_reference(pPacket->srcImage);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdUpdateBuffer: {
                            packet_vkCmdUpdateBuffer *pPacket = (packet_vkCmdUpdateBuffer *)(pHeader->pBody);
                            trim::mark_Buffer_reference(pPacket->dstBuffer);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdFillBuffer: {
                            packet_vkCmdFillBuffer *pPacket = (packet_vkCmdFillBuffer *)(pHeader->pBody);
                            trim::mark_Buffer_reference(pPacket->dstBuffer);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdPushDescriptorSetKHR: {
                            packet_vkCmdPushDescriptorSetKHR *pPacket = (packet_vkCmdPushDescriptorSetKHR *)(pHeader->pBody);
                            const VkWriteDescriptorSet *pDescriptorWrites =
                                (const VkWriteDescriptorSet *)vktrace_trace_packet_interpret_buffer_pointer(
                                    pHeader, (intptr_t)pPacket->pDescriptorWrites);
                            for (uint32_t i = 0; i < pPacket->descriptorWriteCount; i++) {
                                switch (pDescriptorWrites[i].descriptorType) {
                                    case VK_DESCRIPTOR_TYPE_SAMPLER:
                                    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                                    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                                    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                                    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: {
                                        VkDescriptorImageInfo *pImageInfo =
                                            (VkDescriptorImageInfo *)vktrace_trace_packet_interpret_buffer_pointer(
                                                pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pImageInfo);
                                        for (uint32_t j = 0; j < pDescriptorWrites[i].descriptorCount; j++) {
                                            trim::mark_ImageView_reference(pImageInfo[j].imageView);
                                        }
                                    } break;
                                    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                                    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                                    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
                                    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: {
                                        VkDescriptorBufferInfo *pBufferInfo =
                                            (VkDescriptorBufferInfo *)vktrace_trace_packet_interpret_buffer_pointer(
                                                pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pBufferInfo);
                                        for (uint32_t j = 0; j < pDescriptorWrites[i].descriptorCount; j++) {
                                            trim::mark_Buffer_reference(pBufferInfo[j].buffer);
                                        }
                                    } break;
                                    case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
                                    case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: {
                                        VkBufferView *pTexelBufferView =
                                            (VkBufferView *)vktrace_trace_packet_interpret_buffer_pointer(
                                                pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pTexelBufferView);
                                        for (uint32_t j = 0; j < pDescriptorWrites[i].descriptorCount; j++) {
                                            trim::mark_BufferView_reference(pTexelBufferView[j]);
                                        }
                                    } break;
                                    default:
                                        break;
                                }
                            }
                        } break;
                        case VKTRACE_TPI_VK_vkCmdCopyImage: {
                            packet_vkCmdCopyImage *pPacket = (packet_vkCmdCopyImage *)(pHeader->pBody);
                            trim::mark_Image_reference(pPacket->srcImage);
                            trim::mark_Image_reference(pPacket->dstImage);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdBlitImage: {
                            packet_vkCmdBlitImage *pPacket = (packet_vkCmdBlitImage *)(pHeader->pBody);
                            trim::mark_Image_reference(pPacket->srcImage);
                            trim::mark_Image_reference(pPacket->dstImage);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdClearColorImage: {
                            packet_vkCmdClearColorImage *pPacket = (packet_vkCmdClearColorImage *)(pHeader->pBody);
                            trim::mark_Image_reference(pPacket->image);
                        } break;
                        case VKTRACE_TPI_VK_vkCmdClearDepthStencilImage: {
                            packet_vkCmdClearDepthStencilImage *pPacket = (packet_vkCmdClearDepthStencilImage *)(pHeader->pBody);
                            trim::mark_Image_reference(pPacket->image);
                        } break;
                        default:
                            break;
                    }
                }
            }
        }

        // Mark reference for objects (buffer, buffer view) used by update descriptor sets
        for (auto deviceObj = s_trimStateTrackerSnapshot.createdDevices.begin();
             deviceObj != s_trimStateTrackerSnapshot.createdDevices.end(); deviceObj++) {
            for (auto setObj = s_trimStateTrackerSnapshot.createdDescriptorSets.begin();
                 setObj != s_trimStateTrackerSnapshot.createdDescriptorSets.end(); setObj++) {
                if (setObj->second.bReferencedInTrim) {
                    if (setObj->second.belongsToDevice == (VkDevice)deviceObj->first) {
                        uint32_t descriptorWriteCount = setObj->second.ObjectInfo.DescriptorSet.writeDescriptorCount;
                        if (descriptorWriteCount > 0) {
                            VkWriteDescriptorSet *pDescriptorWrites = setObj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets;
                            for (uint32_t i = 0; i < descriptorWriteCount; i++) {
                                trim::mark_DescriptorSet_reference(pDescriptorWrites[i].dstSet);
                                for (uint32_t j = 0; j < pDescriptorWrites[i].descriptorCount; j++) {
                                    switch (pDescriptorWrites[i].descriptorType) {
                                        case VK_DESCRIPTOR_TYPE_SAMPLER:
                                        case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                                        case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                                        case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                                        case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: {
                                            trim::mark_ImageView_reference(pDescriptorWrites[i].pImageInfo[j].imageView);
                                        } break;
                                        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                                        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                                        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
                                        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: {
                                            trim::mark_Buffer_reference(pDescriptorWrites[i].pBufferInfo[j].buffer);
                                        } break;
                                        case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
                                        case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: {
                                            trim::mark_BufferView_reference(pDescriptorWrites[i].pTexelBufferView[j]);
                                        } break;
                                        default:
                                            break;
                                    }
                                }
                            }
                        }
                        uint32_t descriptorCopyCount = setObj->second.ObjectInfo.DescriptorSet.copyDescriptorCount;
                        if (descriptorCopyCount > 0) {
                            VkCopyDescriptorSet *pDescriptorCopies = setObj->second.ObjectInfo.DescriptorSet.pCopyDescriptorSets;
                            for (uint32_t i = 0; i < descriptorCopyCount; i++) {
                                trim::mark_DescriptorSet_reference(pDescriptorCopies[i].srcSet);
                                trim::mark_DescriptorSet_reference(pDescriptorCopies[i].dstSet);
                            }
                        }
                    }
                }
            }
        }

        // Mark reference for objects related to swapchain images
        for (auto obj = s_trimStateTrackerSnapshot.createdFramebuffers.begin();
             obj != s_trimStateTrackerSnapshot.createdFramebuffers.end(); obj++) {
            for (uint32_t i = 0; i < obj->second.ObjectInfo.Framebuffer.attachmentCount; i++) {
                trim::ObjectInfo *pImageViewObjectInfo =
                    trim::get_ImageView_objectInfo(obj->second.ObjectInfo.Framebuffer.pAttachments[i]);
                if (pImageViewObjectInfo) {
                    trim::ObjectInfo *pImageObjectInfo =
                        trim::get_Image_objectInfo(pImageViewObjectInfo->ObjectInfo.ImageView.image);
                    if (pImageObjectInfo && pImageObjectInfo->ObjectInfo.Image.bIsSwapchainImage) {
                        trim::mark_Framebuffer_reference((VkFramebuffer)obj->first);
                    }
                }
            }
        }
    }

    vktrace_LogDebug("vktrace recreating objects for trim.");

    vktrace_enter_critical_section(&trimStateTrackerLock);
    // write the referenced objects from the snapshot
    StateTracker &stateTracker = s_trimStateTrackerSnapshot;
    vktrace_leave_critical_section(&trimStateTrackerLock);

    // Instances (& PhysicalDevices)
    recreate_instances(stateTracker);

    // PhysicalDevice memory and queue family properties
    recreate_physical_device_mem_queue(stateTracker);

    // SurfaceKHR and surface properties
    recreate_surfaces(stateTracker);

    // Devices
    recreate_devices(stateTracker);

    // Queue
    recreate_queues(stateTracker);

    // CommandPool
    recreate_command_pools(stateTracker);

    // DeviceMemory
    recreate_device_mem(stateTracker);

    // Image
    recreate_images(stateTracker);

    // ImageView
    recreate_imageviews(stateTracker);

    // Buffer
    recreate_buffers(stateTracker);

    // DeviceMemory (Persistently Map Memory)
    recreate_device_persistently_map_mem(stateTracker);

    // BufferView
    recreate_bufferviews(stateTracker);

    // Sampler
    recreate_samplers(stateTracker);

    // DescriptorSetLayout
    recreate_descriptor_set_layout(stateTracker);

    // PipelineLayout
    recreate_pipeline_layout(stateTracker);

    // RenderPass
    recreate_renderpass(stateTracker);

    // ShaderModule
    recreate_shader_modules(stateTracker);

    // PipelineCache
    recreate_pipeline_caches(stateTracker);

    // Pipeline
    recreate_pipelines(stateTracker);

    // DescriptorPool
    recreate_descriptor_pools(stateTracker);

    // DescriptorSets
    recreate_descriptor_sets(stateTracker);

    // Framebuffer
    recreate_framebuffers(stateTracker);

    // Semaphore
    recreate_semaphores(stateTracker);

    // Fence
    recreate_fences(stateTracker);

    // Event
    recreate_events(stateTracker);

    // QueryPool
    recreate_query_pools(stateTracker);

    // Command Buffers
    recreate_allocated_command_buffers(stateTracker);

    // Semaphores that need signalling
    recreate_signalled_semaphores(stateTracker);

    // DescriptorUpdateTemplate
    recreate_descriptor_update_templates(stateTracker);

    // Recreate in-trim objects
    recreate_in_trim_objects();

    vktrace_LogDebug("vktrace done recreating objects for trim.");
}

//=========================================================================
// Object tracking
//=========================================================================
void add_RenderPassCreateInfo(VkRenderPass renderPass, const VkRenderPassCreateInfo *pCreateInfo) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimGlobalStateTracker.add_RenderPassCreateInfo(renderPass, pCreateInfo);
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//=========================================================================
uint32_t get_RenderPassVersion(VkRenderPass renderPass) {
    uint32_t version = 0;
    vktrace_enter_critical_section(&trimStateTrackerLock);
    version = s_trimGlobalStateTracker.get_RenderPassVersion(renderPass);
    vktrace_leave_critical_section(&trimStateTrackerLock);
    return version;
}

//=========================================================================
void add_CommandBuffer_call(VkCommandBuffer commandBuffer, vktrace_trace_packet_header *pHeader) {
    if (pHeader != NULL) {
        vktrace_enter_critical_section(&trimCommandBufferPacketLock);
        s_trimGlobalStateTracker.add_CommandBuffer_call(commandBuffer, pHeader);
        vktrace_leave_critical_section(&trimCommandBufferPacketLock);
    }
}

//=========================================================================
void remove_CommandBuffer_calls(VkCommandBuffer commandBuffer) {
    vktrace_enter_critical_section(&trimCommandBufferPacketLock);
    s_trimGlobalStateTracker.remove_CommandBuffer_calls(commandBuffer);
    if (g_trimIsInTrim) {
        // Set command buffer reference as checked to not re-create the
        // useless command buffer calls before trim if the same command
        // buffer is re-used in trim.
        mark_CommandBuffer_reference(commandBuffer);
    }
    vktrace_leave_critical_section(&trimCommandBufferPacketLock);
}

//=========================================================================
void reset_DescriptorPool(VkDescriptorPool descriptorPool) {
    vktrace_enter_critical_section(&trimStateTrackerLock);
    for (auto dsIter = s_trimGlobalStateTracker.createdDescriptorSets.begin();
         dsIter != s_trimGlobalStateTracker.createdDescriptorSets.end();) {
        if (dsIter->second.ObjectInfo.DescriptorSet.descriptorPool == descriptorPool) {
            VkDescriptorSet setToRemove = (VkDescriptorSet)dsIter->first;
            dsIter++;
            s_trimGlobalStateTracker.remove_DescriptorSet(setToRemove);
        } else {
            dsIter++;
        }
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);
}

//===============================================
// Packet Recording for frames of interest
//===============================================
void write_packet(vktrace_trace_packet_header *pHeader) {
    if (g_trimPostProcess && g_trimIsInTrim) {
        add_InTrim_call(copy_packet(pHeader));
    } else {
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
    }
    vktrace_delete_trace_packet(&pHeader);
}

//=============================================================================
// Generate packets to destroy all objects on the specified device and add them to the recorded packets list.
// Ideally, the app would have destroyed all the objects before destroying the device, so this shouldn't
// necessarily add many packets to the list of recorded packets (except for ShaderModules, which we
// specifically skip their destruction).
//=============================================================================
void add_destroy_device_object_packets(VkDevice device) {
    // QueryPool
    for (auto obj = s_trimGlobalStateTracker.createdQueryPools.begin(); obj != s_trimGlobalStateTracker.createdQueryPools.end();
         obj++) {
        if (obj->second.belongsToDevice == device) {
            VkQueryPool queryPool = obj->first;
            VkAllocationCallbacks *pAllocator = get_Allocator(obj->second.ObjectInfo.QueryPool.pAllocator);

            vktrace_trace_packet_header *pHeader =
                generate::vkDestroyQueryPool(false, obj->second.belongsToDevice, queryPool, pAllocator);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // Event
    for (auto obj = s_trimGlobalStateTracker.createdEvents.begin(); obj != s_trimGlobalStateTracker.createdEvents.end(); obj++) {
        if (obj->second.belongsToDevice == device) {
            VkEvent event = obj->first;
            VkAllocationCallbacks *pAllocator = get_Allocator(obj->second.ObjectInfo.Event.pAllocator);

            vktrace_trace_packet_header *pHeader = generate::vkDestroyEvent(false, obj->second.belongsToDevice, event, pAllocator);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // Fence
    for (auto obj = s_trimGlobalStateTracker.createdFences.begin(); obj != s_trimGlobalStateTracker.createdFences.end(); obj++) {
        if (obj->second.belongsToDevice == device) {
            VkFence fence = obj->first;
            VkAllocationCallbacks *pAllocator = get_Allocator(obj->second.ObjectInfo.Fence.pAllocator);

            vktrace_trace_packet_header *pHeader = generate::vkDestroyFence(false, obj->second.belongsToDevice, fence, pAllocator);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // Semaphore
    for (auto obj = s_trimGlobalStateTracker.createdSemaphores.begin(); obj != s_trimGlobalStateTracker.createdSemaphores.end();
         obj++) {
        if (obj->second.belongsToDevice == device) {
            VkSemaphore semaphore = obj->first;
            VkAllocationCallbacks *pAllocator = get_Allocator(obj->second.ObjectInfo.Semaphore.pAllocator);

            vktrace_trace_packet_header *pHeader =
                generate::vkDestroySemaphore(false, obj->second.belongsToDevice, semaphore, pAllocator);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // Framebuffer
    for (auto obj = s_trimGlobalStateTracker.createdFramebuffers.begin(); obj != s_trimGlobalStateTracker.createdFramebuffers.end();
         obj++) {
        if (obj->second.belongsToDevice == device) {
            VkFramebuffer framebuffer = obj->first;
            VkAllocationCallbacks *pAllocator = get_Allocator(obj->second.ObjectInfo.Framebuffer.pAllocator);

            vktrace_trace_packet_header *pHeader =
                generate::vkDestroyFramebuffer(false, obj->second.belongsToDevice, framebuffer, pAllocator);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // DescriptorPool
    for (auto obj = s_trimGlobalStateTracker.createdDescriptorPools.begin();
         obj != s_trimGlobalStateTracker.createdDescriptorPools.end(); obj++) {
        if (obj->second.belongsToDevice == device) {
            // Free the associated DescriptorSets
            VkDescriptorPool descriptorPool = obj->first;

            // We can always call ResetDescriptorPool, but can only use
            // FreeDescriptorSets if the pool was created with
            // VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT.
            {
                vktrace_trace_packet_header *pHeader =
                    generate::vkResetDescriptorPool(false, obj->second.belongsToDevice, descriptorPool, 0);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);
            }

            // Now destroy the DescriptorPool
            {
                vktrace_trace_packet_header *pHeader =
                    generate::vkDestroyDescriptorPool(false, obj->second.belongsToDevice, descriptorPool,
                                                      get_Allocator(obj->second.ObjectInfo.DescriptorPool.pAllocator));
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);
            }
        }
    }

    // Pipeline
    for (auto obj = s_trimGlobalStateTracker.createdPipelines.begin(); obj != s_trimGlobalStateTracker.createdPipelines.end();
         obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader =
                generate::vkDestroyPipeline(false, obj->second.belongsToDevice, (VkPipeline)obj->first,
                                            get_Allocator(obj->second.ObjectInfo.Pipeline.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // PipelineCache
    for (auto obj = s_trimGlobalStateTracker.createdPipelineCaches.begin();
         obj != s_trimGlobalStateTracker.createdPipelineCaches.end(); obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader =
                generate::vkDestroyPipelineCache(false, obj->second.belongsToDevice, (VkPipelineCache)obj->first,
                                                 get_Allocator(obj->second.ObjectInfo.PipelineCache.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // ShaderModule
    for (auto obj = s_trimGlobalStateTracker.createdShaderModules.begin();
         obj != s_trimGlobalStateTracker.createdShaderModules.end(); obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader =
                generate::vkDestroyShaderModule(false, obj->second.belongsToDevice, (VkShaderModule)obj->first,
                                                get_Allocator(obj->second.ObjectInfo.ShaderModule.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // RenderPass
    for (auto obj = s_trimGlobalStateTracker.createdRenderPasss.begin(); obj != s_trimGlobalStateTracker.createdRenderPasss.end();
         obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader =
                generate::vkDestroyRenderPass(false, obj->second.belongsToDevice, (VkRenderPass)obj->first,
                                              get_Allocator(obj->second.ObjectInfo.RenderPass.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // PipelineLayout
    for (auto obj = s_trimGlobalStateTracker.createdPipelineLayouts.begin();
         obj != s_trimGlobalStateTracker.createdPipelineLayouts.end(); obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader =
                generate::vkDestroyPipelineLayout(false, obj->second.belongsToDevice, (VkPipelineLayout)obj->first,
                                                  get_Allocator(obj->second.ObjectInfo.PipelineLayout.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // DescriptorSetLayout
    for (auto obj = s_trimGlobalStateTracker.createdDescriptorSetLayouts.begin();
         obj != s_trimGlobalStateTracker.createdDescriptorSetLayouts.end(); obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader =
                generate::vkDestroyDescriptorSetLayout(false, obj->second.belongsToDevice, (VkDescriptorSetLayout)obj->first,
                                                       get_Allocator(obj->second.ObjectInfo.DescriptorSetLayout.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // Sampler
    for (auto obj = s_trimGlobalStateTracker.createdSamplers.begin(); obj != s_trimGlobalStateTracker.createdSamplers.end();
         obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader =
                generate::vkDestroySampler(false, obj->second.belongsToDevice, (VkSampler)obj->first,
                                           get_Allocator(obj->second.ObjectInfo.Sampler.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // Buffer
    for (auto obj = s_trimGlobalStateTracker.createdBuffers.begin(); obj != s_trimGlobalStateTracker.createdBuffers.end(); obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader = generate::vkDestroyBuffer(
                false, obj->second.belongsToDevice, (VkBuffer)obj->first, get_Allocator(obj->second.ObjectInfo.Buffer.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // BufferView
    for (auto obj = s_trimGlobalStateTracker.createdBufferViews.begin(); obj != s_trimGlobalStateTracker.createdBufferViews.end();
         obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader =
                generate::vkDestroyBufferView(false, obj->second.belongsToDevice, (VkBufferView)obj->first,
                                              get_Allocator(obj->second.ObjectInfo.BufferView.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // Image
    for (auto obj = s_trimGlobalStateTracker.createdImages.begin(); obj != s_trimGlobalStateTracker.createdImages.end(); obj++) {
        if (obj->second.belongsToDevice == device) {
            if (obj->second.ObjectInfo.Image.bIsSwapchainImage == false) {
                vktrace_trace_packet_header *pHeader =
                    generate::vkDestroyImage(false, obj->second.belongsToDevice, (VkImage)obj->first,
                                             get_Allocator(obj->second.ObjectInfo.Image.pAllocator));
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);
            }
        }
    }

    // ImageView
    for (auto obj = s_trimGlobalStateTracker.createdImageViews.begin(); obj != s_trimGlobalStateTracker.createdImageViews.end();
         obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader =
                generate::vkDestroyImageView(false, obj->second.belongsToDevice, (VkImageView)obj->first,
                                             get_Allocator(obj->second.ObjectInfo.ImageView.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // DeviceMemory
    for (auto obj = s_trimGlobalStateTracker.createdDeviceMemorys.begin();
         obj != s_trimGlobalStateTracker.createdDeviceMemorys.end(); obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader =
                generate::vkFreeMemory(false, obj->second.belongsToDevice, (VkDeviceMemory)obj->first,
                                       get_Allocator(obj->second.ObjectInfo.DeviceMemory.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // SwapchainKHR
    for (auto obj = s_trimGlobalStateTracker.createdSwapchainKHRs.begin();
         obj != s_trimGlobalStateTracker.createdSwapchainKHRs.end(); obj++) {
        if (obj->second.belongsToDevice == device) {
            vktrace_trace_packet_header *pHeader =
                generate::vkDestroySwapchainKHR(false, obj->second.belongsToDevice, (VkSwapchainKHR)obj->first,
                                                get_Allocator(obj->second.ObjectInfo.SwapchainKHR.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // CommandPool
    for (auto obj = s_trimGlobalStateTracker.createdCommandPools.begin(); obj != s_trimGlobalStateTracker.createdCommandPools.end();
         obj++) {
        if (obj->second.belongsToDevice == device) {
            // free the command buffers
            for (int32_t level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; level < VK_COMMAND_BUFFER_LEVEL_SECONDARY; level++) {
                uint32_t commandBufferCount = obj->second.ObjectInfo.CommandPool.numCommandBuffersAllocated[level];
                if (commandBufferCount > 0) {
                    uint64_t vktrace_begin_time = vktrace_get_time();

                    VkCommandBuffer *pCommandBuffers = new VkCommandBuffer[commandBufferCount];
                    uint32_t index = 0;
                    for (auto cbIter = s_trimGlobalStateTracker.createdCommandBuffers.begin();
                         cbIter != s_trimGlobalStateTracker.createdCommandBuffers.end(); cbIter++) {
                        if (cbIter->second.ObjectInfo.CommandBuffer.commandPool == (VkCommandPool)obj->first &&
                            cbIter->second.ObjectInfo.CommandBuffer.level == level) {
                            pCommandBuffers[index] = (VkCommandBuffer)cbIter->first;
                            index++;
                        }
                    }

                    vktrace_trace_packet_header *pHeader = generate::vkFreeCommandBuffers(
                        false, obj->second.belongsToDevice, (VkCommandPool)obj->first, commandBufferCount, pCommandBuffers);
                    pHeader->vktrace_begin_time = vktrace_begin_time;
                    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pHeader);

                    delete[] pCommandBuffers;
                }
            }

            // Destroy the CommandPool
            vktrace_trace_packet_header *pHeader =
                generate::vkDestroyCommandPool(false, obj->second.belongsToDevice, (VkCommandPool)obj->first,
                                               get_Allocator(obj->second.ObjectInfo.CommandPool.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }
}
//=============================================================================

//===============================================
// Write packets to destroy all created created objects.
// Remember that we want to destroy them roughly in the opposite
// order they were created, so that means the Instance is the last
// object to destroy!
//===============================================
void write_destroy_packets() {
    vktrace_LogDebug("vktrace destroying objects after trim.");

    vktrace_enter_critical_section(&trimStateTrackerLock);
    // Make sure all queues have completed before trying to delete anything
    for (auto obj = s_trimGlobalStateTracker.createdQueues.begin(); obj != s_trimGlobalStateTracker.createdQueues.end(); obj++) {
        VkQueue queue = obj->first;
        vktrace_trace_packet_header *pHeader = generate::vkQueueWaitIdle(false, queue);
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);
    }

    // Device
    // This will also write packets to destroy any remaining objects on the device
    for (auto obj = s_trimGlobalStateTracker.createdDevices.begin(); obj != s_trimGlobalStateTracker.createdDevices.end(); obj++) {
        add_destroy_device_object_packets((VkDevice)obj->first);
        vktrace_trace_packet_header *pHeader =
            generate::vkDestroyDevice(false, (VkDevice)obj->first, get_Allocator(obj->second.ObjectInfo.Device.pAllocator));
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);
    }

    // SurfaceKHR
    for (auto obj = s_trimGlobalStateTracker.createdSurfaceKHRs.begin(); obj != s_trimGlobalStateTracker.createdSurfaceKHRs.end();
         obj++) {
        vktrace_trace_packet_header *pHeader =
            generate::vkDestroySurfaceKHR(false, obj->second.belongsToInstance, (VkSurfaceKHR)obj->first,
                                          get_Allocator(obj->second.ObjectInfo.SurfaceKHR.pAllocator));
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);
    }

    // Instance
    for (auto obj = s_trimGlobalStateTracker.createdInstances.begin(); obj != s_trimGlobalStateTracker.createdInstances.end();
         obj++) {
        vktrace_trace_packet_header *pHeader =
            generate::vkDestroyInstance(false, (VkInstance)obj->first, get_Allocator(obj->second.ObjectInfo.Instance.pAllocator));
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);
    }
    vktrace_leave_critical_section(&trimStateTrackerLock);

    vktrace_LogDebug("vktrace done destroying objects after trim.");
}

}  // namespace trim
