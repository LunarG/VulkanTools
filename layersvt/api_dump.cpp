/* THIS FILE IS GENERATED.  DO NOT EDIT. */

/*
 * Copyright (c) 2015-2016 Valve Corporation
 * Copyright (c) 2015-2016 LunarG, Inc.
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
 * Author: Tobin Ehlis <tobin@lunarg.com>
 * Author: Courtney Goeltzenleuchter <courtney@lunarg.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Mark Lobodzinski <mark@lunarg.com>
 * Author: Mike Stroyan <mike@lunarg.com>
 * Author: Tony Barbour <tony@LunarG.com>
 */

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #854
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>

#include "vk_loader_platform.h"
#include "vulkan/vk_layer.h"
#include "vk_api_dump_helper_cpp.h"
#include "vk_layer_table.h"
#include "vk_layer_extension_utils.h"
#include "vk_layer_config.h"
#include "vk_layer_utils.h"
#include <unordered_map>
#include "api_dump.h"

static std::ofstream fileStream;

#ifdef ANDROID
static std::string fileName = "/sdcard/Android/vk_apidump.txt";
#else
static std::string fileName = "vk_apidump.txt";
#endif

std::ostream* outputStream = NULL;
void ConfigureOutputStream(bool writeToFile, bool flushAfterWrite)
{
    if(writeToFile)
    {
        if (fileName == "stdout")
        {
            outputStream = &std::cout;
            (*outputStream) << std::endl << "api_dump output filename 'stdout' specified. Writing to STDOUT instead of a file." << std::endl << std::endl;
        } else {
            fileStream.open(fileName);
            if ((fileStream.rdstate() & fileStream.failbit) != 0) {
                outputStream = &std::cout;
                (*outputStream) << std::endl << "api_dump ERROR: Bad output filename specified: " << fileName << ". Writing to STDOUT instead" << std::endl << std::endl;
            }
            else
                outputStream = &fileStream;
        }
    }
    else
    {
        outputStream = &std::cout;
    }

    if(flushAfterWrite)
    {
        outputStream->sync_with_stdio(true);
    }
    else
    {
        outputStream->sync_with_stdio(false);
    }

}

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #913
static bool g_ApiDumpDetailed = true;
static uint64_t g_frameCounter = 0;

static LOADER_PLATFORM_THREAD_ONCE_DECLARATION(initOnce);

static int printLockInitialized = 0;
static loader_platform_thread_mutex printLock;

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #922
#define LAYER_EXT_ARRAY_SIZE 1
#define LAYER_DEV_EXT_ARRAY_SIZE 1
#define MAX_TID 513
static loader_platform_thread_id tidMapping[MAX_TID] = {0};
static uint32_t maxTID = 0;
// Map actual TID to an index value and return that index
//  This keeps TIDs in range from 0-MAX_TID and simplifies compares between runs
static uint32_t getTIDIndex() {
    loader_platform_thread_id tid = loader_platform_get_thread_id();
    for (uint32_t i = 0; i < maxTID; i++) {
        if (tid == tidMapping[i])
            return i;
    }
    // Don't yet have mapping, set it and return newly set index
    uint32_t retVal = (uint32_t) maxTID;
    tidMapping[maxTID++] = tid;
    assert(maxTID < MAX_TID);
    return retVal;
}


// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #947
#include "vk_dispatch_table_helper.h"
#include "vk_layer_config.h"

static void initapi_dump(void)
{
    using namespace StreamControl;
    using namespace std;

    char const*const logName = getLayerOption("lunarg_api_dump.log_filename");
    if(logName != NULL)
    {
        fileName = logName;
    }

    char const*const detailedStr = getLayerOption("lunarg_api_dump.detailed");
    if(detailedStr != NULL)
    {
        if(strcmp(detailedStr, "TRUE") == 0)
        {
            g_ApiDumpDetailed = true;
        }
        else if(strcmp(detailedStr, "FALSE") == 0)
        {
            g_ApiDumpDetailed = false;
        }
    }

    char const*const writeToFileStr = getLayerOption("lunarg_api_dump.file");
    bool writeToFile = false;
    if(writeToFileStr != NULL)
    {
        if(strcmp(writeToFileStr, "TRUE") == 0)
        {
            writeToFile = true;
        }
        else if(strcmp(writeToFileStr, "FALSE") == 0)
        {
            writeToFile = false;
        }
    }

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #989
    char const*const noAddrStr = getLayerOption("lunarg_api_dump.no_addr");
    if(noAddrStr != NULL)
    {
        if(strcmp(noAddrStr, "FALSE") == 0)
        {
            StreamControl::writeAddress = true;
        }
        else if(strcmp(noAddrStr, "TRUE") == 0)
        {
            StreamControl::writeAddress = false;
        }
    }

    char const*const flushAfterWriteStr = getLayerOption("lunarg_api_dump.flush");
    bool flushAfterWrite = false;
    if(flushAfterWriteStr != NULL)
    {
        if(strcmp(flushAfterWriteStr, "TRUE") == 0)
        {
            flushAfterWrite = true;
        }
        else if(strcmp(flushAfterWriteStr, "FALSE") == 0)
        {
            flushAfterWrite = false;
        }
    }

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1017
    ConfigureOutputStream(writeToFile, flushAfterWrite);

    if (!printLockInitialized)
    {
        // TODO/TBD: Need to delete this mutex sometime.  How???
        loader_platform_thread_create_mutex(&printLock);
        printLockInitialized = 1;
    }
}


VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    using namespace StreamControl;
    using namespace std;
    loader_platform_thread_once(&initOnce, initapi_dump);
    VkLayerInstanceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance) fpGetInstanceProcAddr(NULL, "vkCreateInstance");
    if (fpCreateInstance == NULL) {
        (*outputStream) << "t{" << getTIDIndex() << "} " << g_frameCounter << " vkCreateInstance FAILED TO FIND PROC ADDRESS" << endl;
        return VK_ERROR_INITIALIZATION_FAILED;
    }
    // Advance the link info for the next element on the chain
    assert(chain_info->u.pLayerInfo);
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);
    if (result == VK_SUCCESS) {
        initInstanceTable(*pInstance, fpGetInstanceProcAddr);
        createInstanceRegisterExtensions(pCreateInfo, *pInstance);
    }
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateInstance(pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pInstance ? "*" : "") << "pInstance = 0x" << nouppercase <<  hex << (pInstance ? HandleCast(*pInstance) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateInstance(pCreateInfo = address, pAllocator = address, pInstance = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkinstancecreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}


VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    dispatch_key key = get_dispatch_key(instance);
    VkLayerInstanceDispatchTable *pDisp  = instance_dispatch_table(instance);
    pDisp->DestroyInstance(instance, pAllocator);
    instanceExtMap.erase(pDisp);
    destroy_instance_dispatch_table(key);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyInstance(instance = 0x" << nouppercase <<  hex << HandleCast(instance) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyInstance(instance = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(instance)->EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkEnumeratePhysicalDevices(instance = 0x" << nouppercase <<  hex << HandleCast(instance) << dec << ", " << (pPhysicalDeviceCount ? "*" : "") << "pPhysicalDeviceCount = 0x" << nouppercase <<  hex << (pPhysicalDeviceCount ? *pPhysicalDeviceCount : 0) << dec << ", " << (pPhysicalDevices ? "*" : "") << "pPhysicalDevices = 0x" << nouppercase <<  hex << (pPhysicalDevices ? HandleCast(*pPhysicalDevices) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkEnumeratePhysicalDevices(instance = address, pPhysicalDeviceCount = address, pPhysicalDevices = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pPhysicalDevices) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < *pPhysicalDeviceCount; i++) {
                if (StreamControl::writeAddress == true) {
                    (*outputStream) << "   pPhysicalDevices[" << i << "] = 0x" << nouppercase << hex << HandleCast(pPhysicalDevices[i]) << dec << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
    using namespace StreamControl;
    using namespace std;
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceFeatures(physicalDevice, pFeatures);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceFeatures(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", pFeatures = 0x" << nouppercase <<  hex << HandleCast(pFeatures) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceFeatures(physicalDevice = address, pFeatures = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pFeatures) {
            string tmp_str = vk_print_vkphysicaldevicefeatures(pFeatures, "    ");
            (*outputStream) << "   pFeatures:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
{
    using namespace StreamControl;
    using namespace std;
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceFormatProperties(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", format = " << string_VkFormat(format) << ", pFormatProperties = 0x" << nouppercase <<  hex << HandleCast(pFormatProperties) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceFormatProperties(physicalDevice = address, format = " << string_VkFormat(format) << ", pFormatProperties = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pFormatProperties) {
            string tmp_str = vk_print_vkformatproperties(pFormatProperties, "    ");
            (*outputStream) << "   pFormatProperties:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceImageFormatProperties(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", format = " << string_VkFormat(format) << ", type = " << string_VkImageType(type) << ", tiling = " << string_VkImageTiling(tiling) << ", usage = 0x" << nouppercase <<  hex << usage << dec << ", flags = 0x" << nouppercase <<  hex << flags << dec << ", pImageFormatProperties = 0x" << nouppercase <<  hex << HandleCast(pImageFormatProperties) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceImageFormatProperties(physicalDevice = address, format = " << string_VkFormat(format) << ", type = " << string_VkImageType(type) << ", tiling = " << string_VkImageTiling(tiling) << ", usage = address, flags = address, pImageFormatProperties = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pImageFormatProperties) {
            string tmp_str = vk_print_vkimageformatproperties(pImageFormatProperties, "    ");
            (*outputStream) << "   pImageFormatProperties:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
    using namespace StreamControl;
    using namespace std;
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceProperties(physicalDevice, pProperties);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceProperties(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", pProperties = 0x" << nouppercase <<  hex << HandleCast(pProperties) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceProperties(physicalDevice = address, pProperties = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pProperties) {
            string tmp_str = vk_print_vkphysicaldeviceproperties(pProperties, "    ");
            (*outputStream) << "   pProperties:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties)
{
    using namespace StreamControl;
    using namespace std;
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", " << (pQueueFamilyPropertyCount ? "*" : "") << "pQueueFamilyPropertyCount = 0x" << nouppercase <<  hex << (pQueueFamilyPropertyCount ? *pQueueFamilyPropertyCount : 0) << dec << ", pQueueFamilyProperties = 0x" << nouppercase <<  hex << HandleCast(pQueueFamilyProperties) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice = address, pQueueFamilyPropertyCount = address, pQueueFamilyProperties = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pQueueFamilyProperties) {
            string tmp_str = vk_print_vkqueuefamilyproperties(pQueueFamilyProperties, "    ");
            (*outputStream) << "   pQueueFamilyProperties:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
    using namespace StreamControl;
    using namespace std;
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceMemoryProperties(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", pMemoryProperties = 0x" << nouppercase <<  hex << HandleCast(pMemoryProperties) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceMemoryProperties(physicalDevice = address, pMemoryProperties = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pMemoryProperties) {
            string tmp_str = vk_print_vkphysicaldevicememoryproperties(pMemoryProperties, "    ");
            (*outputStream) << "   pMemoryProperties:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1201


VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = explicit_CreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateDevice(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pDevice ? "*" : "") << "pDevice = 0x" << nouppercase <<  hex << (pDevice ? HandleCast(*pDevice) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateDevice(physicalDevice = address, pCreateInfo = address, pAllocator = address, pDevice = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkdevicecreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    dispatch_key key = get_dispatch_key(device);
    VkLayerDispatchTable *pDisp  = device_dispatch_table(device);
    pDisp->DestroyDevice(device, pAllocator);
    deviceExtMap.erase(pDisp);
    destroy_device_dispatch_table(key);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyDevice(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyDevice(device = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #300

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount,  VkExtensionProperties* pProperties)
{
    return util_GetExtensionProperties(0, NULL, pCount, pProperties);
}

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #327
static const VkLayerProperties globalLayerProps[] = {
    {
        "VK_LAYER_LUNARG_api_dump",
        VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), // specVersion
        VK_MAKE_VERSION(0, 1, 0), // implementationVersion
        "layer: api_dump",
    }
};

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #337

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t *pCount,  VkLayerProperties* pProperties)
{
    return util_GetLayerProperties(ARRAY_SIZE(globalLayerProps), globalLayerProps, pCount, pProperties);
}

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #351
static const VkLayerProperties deviceLayerProps[] = {
    {
        "VK_LAYER_LUNARG_api_dump",
        VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION),
        VK_MAKE_VERSION(0, 1, 0),
        "layer: api_dump",
    }
};
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount, VkLayerProperties* pProperties)
{
    return util_GetLayerProperties(ARRAY_SIZE(deviceLayerProps), deviceLayerProps, pCount, pProperties);
}


VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->GetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetDeviceQueue(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", queueFamilyIndex = 0x" << nouppercase <<  hex << queueFamilyIndex << dec << ", queueIndex = 0x" << nouppercase <<  hex << queueIndex << dec << ", " << (pQueue ? "*" : "") << "pQueue = 0x" << nouppercase <<  hex << (pQueue ? HandleCast(*pQueue) : 0) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetDeviceQueue(device = address, queueFamilyIndex = address, queueIndex = address, pQueue = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(queue)->QueueSubmit(queue, submitCount, pSubmits, fence);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkQueueSubmit(queue = 0x" << nouppercase <<  hex << HandleCast(queue) << dec << ", submitCount = 0x" << nouppercase <<  hex << submitCount << dec << ", pSubmits = 0x" << nouppercase <<  hex << HandleCast(pSubmits) << dec << ", fence = 0x" << nouppercase <<  hex << HandleCast(fence) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkQueueSubmit(queue = address, submitCount = address, pSubmits = address, fence = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pSubmits) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < submitCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vksubmitinfo(&pSubmits[i], "    ");
                    (*outputStream) << "   pSubmits[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkQueueWaitIdle(VkQueue queue)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(queue)->QueueWaitIdle(queue);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkQueueWaitIdle(queue = 0x" << nouppercase <<  hex << HandleCast(queue) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkQueueWaitIdle(queue = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkDeviceWaitIdle(VkDevice device)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->DeviceWaitIdle(device);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDeviceWaitIdle(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDeviceWaitIdle(device = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->AllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkAllocateMemory(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pAllocateInfo = 0x" << nouppercase <<  hex << HandleCast(pAllocateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pMemory ? "*" : "") << "pMemory = 0x" << nouppercase <<  hex << (pMemory ? HandleCast(*pMemory) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkAllocateMemory(device = address, pAllocateInfo = address, pAllocator = address, pMemory = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocateInfo) {
            string tmp_str = vk_print_vkmemoryallocateinfo(pAllocateInfo, "    ");
            (*outputStream) << "   pAllocateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->FreeMemory(device, memory, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkFreeMemory(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", memory = 0x" << nouppercase <<  hex << HandleCast(memory) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkFreeMemory(device = address, memory = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->MapMemory(device, memory, offset, size, flags, ppData);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkMapMemory(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", memory = 0x" << nouppercase <<  hex << HandleCast(memory) << dec << ", offset = 0x" << nouppercase <<  hex << offset << dec << ", size = 0x" << nouppercase <<  hex << size << dec << ", flags = 0x" << nouppercase <<  hex << flags << dec << ", " << (ppData ? "*" : "") << "ppData = 0x" << nouppercase <<  hex << (ppData ? HandleCast(*ppData) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkMapMemory(device = address, memory = address, offset = address, size = address, flags = address, ppData = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkUnmapMemory(VkDevice device, VkDeviceMemory memory)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->UnmapMemory(device, memory);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkUnmapMemory(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", memory = 0x" << nouppercase <<  hex << HandleCast(memory) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkUnmapMemory(device = address, memory = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkFlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->FlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkFlushMappedMemoryRanges(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", memoryRangeCount = 0x" << nouppercase <<  hex << memoryRangeCount << dec << ", pMemoryRanges = 0x" << nouppercase <<  hex << HandleCast(pMemoryRanges) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkFlushMappedMemoryRanges(device = address, memoryRangeCount = address, pMemoryRanges = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pMemoryRanges) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < memoryRangeCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkmappedmemoryrange(&pMemoryRanges[i], "    ");
                    (*outputStream) << "   pMemoryRanges[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->InvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkInvalidateMappedMemoryRanges(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", memoryRangeCount = 0x" << nouppercase <<  hex << memoryRangeCount << dec << ", pMemoryRanges = 0x" << nouppercase <<  hex << HandleCast(pMemoryRanges) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkInvalidateMappedMemoryRanges(device = address, memoryRangeCount = address, pMemoryRanges = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pMemoryRanges) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < memoryRangeCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkmappedmemoryrange(&pMemoryRanges[i], "    ");
                    (*outputStream) << "   pMemoryRanges[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->GetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetDeviceMemoryCommitment(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", memory = 0x" << nouppercase <<  hex << HandleCast(memory) << dec << ", " << (pCommittedMemoryInBytes ? "*" : "") << "pCommittedMemoryInBytes = 0x" << nouppercase <<  hex << (pCommittedMemoryInBytes ? *pCommittedMemoryInBytes : 0) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetDeviceMemoryCommitment(device = address, memory = address, pCommittedMemoryInBytes = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->BindBufferMemory(device, buffer, memory, memoryOffset);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkBindBufferMemory(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", buffer = 0x" << nouppercase <<  hex << HandleCast(buffer) << dec << ", memory = 0x" << nouppercase <<  hex << HandleCast(memory) << dec << ", memoryOffset = 0x" << nouppercase <<  hex << memoryOffset << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkBindBufferMemory(device = address, buffer = address, memory = address, memoryOffset = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->BindImageMemory(device, image, memory, memoryOffset);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkBindImageMemory(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", image = 0x" << nouppercase <<  hex << HandleCast(image) << dec << ", memory = 0x" << nouppercase <<  hex << HandleCast(memory) << dec << ", memoryOffset = 0x" << nouppercase <<  hex << memoryOffset << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkBindImageMemory(device = address, image = address, memory = address, memoryOffset = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetBufferMemoryRequirements(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", buffer = 0x" << nouppercase <<  hex << HandleCast(buffer) << dec << ", pMemoryRequirements = 0x" << nouppercase <<  hex << HandleCast(pMemoryRequirements) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetBufferMemoryRequirements(device = address, buffer = address, pMemoryRequirements = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pMemoryRequirements) {
            string tmp_str = vk_print_vkmemoryrequirements(pMemoryRequirements, "    ");
            (*outputStream) << "   pMemoryRequirements:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->GetImageMemoryRequirements(device, image, pMemoryRequirements);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetImageMemoryRequirements(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", image = 0x" << nouppercase <<  hex << HandleCast(image) << dec << ", pMemoryRequirements = 0x" << nouppercase <<  hex << HandleCast(pMemoryRequirements) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetImageMemoryRequirements(device = address, image = address, pMemoryRequirements = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pMemoryRequirements) {
            string tmp_str = vk_print_vkmemoryrequirements(pMemoryRequirements, "    ");
            (*outputStream) << "   pMemoryRequirements:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->GetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetImageSparseMemoryRequirements(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", image = 0x" << nouppercase <<  hex << HandleCast(image) << dec << ", " << (pSparseMemoryRequirementCount ? "*" : "") << "pSparseMemoryRequirementCount = 0x" << nouppercase <<  hex << (pSparseMemoryRequirementCount ? *pSparseMemoryRequirementCount : 0) << dec << ", pSparseMemoryRequirements = 0x" << nouppercase <<  hex << HandleCast(pSparseMemoryRequirements) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetImageSparseMemoryRequirements(device = address, image = address, pSparseMemoryRequirementCount = address, pSparseMemoryRequirements = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pSparseMemoryRequirements) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < *pSparseMemoryRequirementCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vksparseimagememoryrequirements(&pSparseMemoryRequirements[i], "    ");
                    (*outputStream) << "   pSparseMemoryRequirements[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties)
{
    using namespace StreamControl;
    using namespace std;
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", format = " << string_VkFormat(format) << ", type = " << string_VkImageType(type) << ", samples = " << string_VkSampleCountFlagBits(samples) << ", usage = 0x" << nouppercase <<  hex << usage << dec << ", tiling = " << string_VkImageTiling(tiling) << ", " << (pPropertyCount ? "*" : "") << "pPropertyCount = 0x" << nouppercase <<  hex << (pPropertyCount ? *pPropertyCount : 0) << dec << ", pProperties = 0x" << nouppercase <<  hex << HandleCast(pProperties) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice = address, format = " << string_VkFormat(format) << ", type = " << string_VkImageType(type) << ", samples = " << string_VkSampleCountFlagBits(samples) << ", usage = address, tiling = " << string_VkImageTiling(tiling) << ", pPropertyCount = address, pProperties = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pProperties) {
            string tmp_str = vk_print_vksparseimageformatproperties(pProperties, "    ");
            (*outputStream) << "   pProperties:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(queue)->QueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkQueueBindSparse(queue = 0x" << nouppercase <<  hex << HandleCast(queue) << dec << ", bindInfoCount = 0x" << nouppercase <<  hex << bindInfoCount << dec << ", pBindInfo = 0x" << nouppercase <<  hex << HandleCast(pBindInfo) << dec << ", fence = 0x" << nouppercase <<  hex << HandleCast(fence) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkQueueBindSparse(queue = address, bindInfoCount = address, pBindInfo = address, fence = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pBindInfo) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < bindInfoCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkbindsparseinfo(&pBindInfo[i], "    ");
                    (*outputStream) << "   pBindInfo[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateFence(device, pCreateInfo, pAllocator, pFence);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateFence(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pFence ? "*" : "") << "pFence = 0x" << nouppercase <<  hex << (pFence ? HandleCast(*pFence) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateFence(device = address, pCreateInfo = address, pAllocator = address, pFence = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkfencecreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyFence(device, fence, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyFence(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", fence = 0x" << nouppercase <<  hex << HandleCast(fence) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyFence(device = address, fence = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->ResetFences(device, fenceCount, pFences);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkResetFences(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", fenceCount = 0x" << nouppercase <<  hex << fenceCount << dec << ", " << (pFences ? "*" : "") << "pFences = 0x" << nouppercase <<  hex << (pFences ? HandleCast(*pFences) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkResetFences(device = address, fenceCount = address, pFences = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pFences) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < fenceCount; i++) {
                if (StreamControl::writeAddress == true) {
                    (*outputStream) << "   pFences[" << i << "] = 0x" << nouppercase << hex << HandleCast(pFences[i]) << dec << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceStatus(VkDevice device, VkFence fence)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->GetFenceStatus(device, fence);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetFenceStatus(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", fence = 0x" << nouppercase <<  hex << HandleCast(fence) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetFenceStatus(device = address, fence = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkWaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->WaitForFences(device, fenceCount, pFences, waitAll, timeout);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkWaitForFences(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", fenceCount = 0x" << nouppercase <<  hex << fenceCount << dec << ", " << (pFences ? "*" : "") << "pFences = 0x" << nouppercase <<  hex << (pFences ? HandleCast(*pFences) : 0) << dec << ", waitAll = " << waitAll << ", timeout = 0x" << nouppercase <<  hex << timeout << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkWaitForFences(device = address, fenceCount = address, pFences = address, waitAll = " << waitAll << ", timeout = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pFences) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < fenceCount; i++) {
                if (StreamControl::writeAddress == true) {
                    (*outputStream) << "   pFences[" << i << "] = 0x" << nouppercase << hex << HandleCast(pFences[i]) << dec << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateSemaphore(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pSemaphore ? "*" : "") << "pSemaphore = 0x" << nouppercase <<  hex << (pSemaphore ? HandleCast(*pSemaphore) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateSemaphore(device = address, pCreateInfo = address, pAllocator = address, pSemaphore = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vksemaphorecreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroySemaphore(device, semaphore, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroySemaphore(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", semaphore = 0x" << nouppercase <<  hex << HandleCast(semaphore) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroySemaphore(device = address, semaphore = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateEvent(device, pCreateInfo, pAllocator, pEvent);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateEvent(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pEvent ? "*" : "") << "pEvent = 0x" << nouppercase <<  hex << (pEvent ? HandleCast(*pEvent) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateEvent(device = address, pCreateInfo = address, pAllocator = address, pEvent = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkeventcreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyEvent(device, event, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyEvent(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", event = 0x" << nouppercase <<  hex << HandleCast(event) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyEvent(device = address, event = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetEventStatus(VkDevice device, VkEvent event)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->GetEventStatus(device, event);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetEventStatus(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", event = 0x" << nouppercase <<  hex << HandleCast(event) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetEventStatus(device = address, event = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkSetEvent(VkDevice device, VkEvent event)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->SetEvent(device, event);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkSetEvent(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", event = 0x" << nouppercase <<  hex << HandleCast(event) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkSetEvent(device = address, event = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkResetEvent(VkDevice device, VkEvent event)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->ResetEvent(device, event);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkResetEvent(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", event = 0x" << nouppercase <<  hex << HandleCast(event) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkResetEvent(device = address, event = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateQueryPool(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pQueryPool ? "*" : "") << "pQueryPool = 0x" << nouppercase <<  hex << (pQueryPool ? HandleCast(*pQueryPool) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateQueryPool(device = address, pCreateInfo = address, pAllocator = address, pQueryPool = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkquerypoolcreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyQueryPool(device, queryPool, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyQueryPool(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", queryPool = 0x" << nouppercase <<  hex << HandleCast(queryPool) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyQueryPool(device = address, queryPool = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->GetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetQueryPoolResults(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", queryPool = 0x" << nouppercase <<  hex << HandleCast(queryPool) << dec << ", firstQuery = 0x" << nouppercase <<  hex << firstQuery << dec << ", queryCount = 0x" << nouppercase <<  hex << queryCount << dec << ", dataSize = 0x" << nouppercase <<  hex << dataSize << dec << ", pData = 0x" << nouppercase <<  hex << HandleCast(pData) << dec << ", stride = 0x" << nouppercase <<  hex << stride << dec << ", flags = 0x" << nouppercase <<  hex << flags << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetQueryPoolResults(device = address, queryPool = address, firstQuery = address, queryCount = address, dataSize = address, pData = address, stride = address, flags = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateBuffer(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pBuffer ? "*" : "") << "pBuffer = 0x" << nouppercase <<  hex << (pBuffer ? HandleCast(*pBuffer) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateBuffer(device = address, pCreateInfo = address, pAllocator = address, pBuffer = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkbuffercreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyBuffer(device, buffer, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyBuffer(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", buffer = 0x" << nouppercase <<  hex << HandleCast(buffer) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyBuffer(device = address, buffer = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateBufferView(device, pCreateInfo, pAllocator, pView);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateBufferView(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pView ? "*" : "") << "pView = 0x" << nouppercase <<  hex << (pView ? HandleCast(*pView) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateBufferView(device = address, pCreateInfo = address, pAllocator = address, pView = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkbufferviewcreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyBufferView(device, bufferView, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyBufferView(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", bufferView = 0x" << nouppercase <<  hex << HandleCast(bufferView) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyBufferView(device = address, bufferView = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateImage(device, pCreateInfo, pAllocator, pImage);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateImage(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pImage ? "*" : "") << "pImage = 0x" << nouppercase <<  hex << (pImage ? HandleCast(*pImage) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateImage(device = address, pCreateInfo = address, pAllocator = address, pImage = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkimagecreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyImage(device, image, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyImage(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", image = 0x" << nouppercase <<  hex << HandleCast(image) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyImage(device = address, image = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->GetImageSubresourceLayout(device, image, pSubresource, pLayout);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetImageSubresourceLayout(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", image = 0x" << nouppercase <<  hex << HandleCast(image) << dec << ", pSubresource = 0x" << nouppercase <<  hex << HandleCast(pSubresource) << dec << ", pLayout = 0x" << nouppercase <<  hex << HandleCast(pLayout) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetImageSubresourceLayout(device = address, image = address, pSubresource = address, pLayout = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pSubresource) {
            string tmp_str = vk_print_vkimagesubresource(pSubresource, "    ");
            (*outputStream) << "   pSubresource:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pLayout) {
            string tmp_str = vk_print_vksubresourcelayout(pLayout, "    ");
            (*outputStream) << "   pLayout:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateImageView(device, pCreateInfo, pAllocator, pView);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateImageView(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pView ? "*" : "") << "pView = 0x" << nouppercase <<  hex << (pView ? HandleCast(*pView) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateImageView(device = address, pCreateInfo = address, pAllocator = address, pView = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkimageviewcreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyImageView(device, imageView, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyImageView(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", imageView = 0x" << nouppercase <<  hex << HandleCast(imageView) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyImageView(device = address, imageView = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateShaderModule(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pShaderModule ? "*" : "") << "pShaderModule = 0x" << nouppercase <<  hex << (pShaderModule ? HandleCast(*pShaderModule) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateShaderModule(device = address, pCreateInfo = address, pAllocator = address, pShaderModule = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkshadermodulecreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyShaderModule(device, shaderModule, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyShaderModule(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", shaderModule = 0x" << nouppercase <<  hex << HandleCast(shaderModule) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyShaderModule(device = address, shaderModule = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreatePipelineCache(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pPipelineCache ? "*" : "") << "pPipelineCache = 0x" << nouppercase <<  hex << (pPipelineCache ? HandleCast(*pPipelineCache) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreatePipelineCache(device = address, pCreateInfo = address, pAllocator = address, pPipelineCache = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkpipelinecachecreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyPipelineCache(device, pipelineCache, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyPipelineCache(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pipelineCache = 0x" << nouppercase <<  hex << HandleCast(pipelineCache) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyPipelineCache(device = address, pipelineCache = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->GetPipelineCacheData(device, pipelineCache, pDataSize, pData);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPipelineCacheData(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pipelineCache = 0x" << nouppercase <<  hex << HandleCast(pipelineCache) << dec << ", " << (pDataSize ? "*" : "") << "pDataSize = 0x" << nouppercase <<  hex << (pDataSize ? *pDataSize : 0) << dec << ", pData = 0x" << nouppercase <<  hex << HandleCast(pData) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPipelineCacheData(device = address, pipelineCache = address, pDataSize = address, pData = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkMergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->MergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkMergePipelineCaches(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", dstCache = 0x" << nouppercase <<  hex << HandleCast(dstCache) << dec << ", srcCacheCount = 0x" << nouppercase <<  hex << srcCacheCount << dec << ", " << (pSrcCaches ? "*" : "") << "pSrcCaches = 0x" << nouppercase <<  hex << (pSrcCaches ? HandleCast(*pSrcCaches) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkMergePipelineCaches(device = address, dstCache = address, srcCacheCount = address, pSrcCaches = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pSrcCaches) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < srcCacheCount; i++) {
                if (StreamControl::writeAddress == true) {
                    (*outputStream) << "   pSrcCaches[" << i << "] = 0x" << nouppercase << hex << HandleCast(pSrcCaches[i]) << dec << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateGraphicsPipelines(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pipelineCache = 0x" << nouppercase <<  hex << HandleCast(pipelineCache) << dec << ", createInfoCount = 0x" << nouppercase <<  hex << createInfoCount << dec << ", pCreateInfos = 0x" << nouppercase <<  hex << HandleCast(pCreateInfos) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pPipelines ? "*" : "") << "pPipelines = 0x" << nouppercase <<  hex << (pPipelines ? HandleCast(*pPipelines) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateGraphicsPipelines(device = address, pipelineCache = address, createInfoCount = address, pCreateInfos = address, pAllocator = address, pPipelines = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pCreateInfos) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < createInfoCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkgraphicspipelinecreateinfo(&pCreateInfos[i], "    ");
                    (*outputStream) << "   pCreateInfos[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateComputePipelines(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pipelineCache = 0x" << nouppercase <<  hex << HandleCast(pipelineCache) << dec << ", createInfoCount = 0x" << nouppercase <<  hex << createInfoCount << dec << ", pCreateInfos = 0x" << nouppercase <<  hex << HandleCast(pCreateInfos) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pPipelines ? "*" : "") << "pPipelines = 0x" << nouppercase <<  hex << (pPipelines ? HandleCast(*pPipelines) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateComputePipelines(device = address, pipelineCache = address, createInfoCount = address, pCreateInfos = address, pAllocator = address, pPipelines = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pCreateInfos) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < createInfoCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkcomputepipelinecreateinfo(&pCreateInfos[i], "    ");
                    (*outputStream) << "   pCreateInfos[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyPipeline(device, pipeline, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyPipeline(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pipeline = 0x" << nouppercase <<  hex << HandleCast(pipeline) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyPipeline(device = address, pipeline = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreatePipelineLayout(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pPipelineLayout ? "*" : "") << "pPipelineLayout = 0x" << nouppercase <<  hex << (pPipelineLayout ? HandleCast(*pPipelineLayout) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreatePipelineLayout(device = address, pCreateInfo = address, pAllocator = address, pPipelineLayout = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkpipelinelayoutcreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyPipelineLayout(device, pipelineLayout, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyPipelineLayout(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pipelineLayout = 0x" << nouppercase <<  hex << HandleCast(pipelineLayout) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyPipelineLayout(device = address, pipelineLayout = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateSampler(device, pCreateInfo, pAllocator, pSampler);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateSampler(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pSampler ? "*" : "") << "pSampler = 0x" << nouppercase <<  hex << (pSampler ? HandleCast(*pSampler) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateSampler(device = address, pCreateInfo = address, pAllocator = address, pSampler = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vksamplercreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroySampler(device, sampler, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroySampler(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", sampler = 0x" << nouppercase <<  hex << HandleCast(sampler) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroySampler(device = address, sampler = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateDescriptorSetLayout(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pSetLayout ? "*" : "") << "pSetLayout = 0x" << nouppercase <<  hex << (pSetLayout ? HandleCast(*pSetLayout) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateDescriptorSetLayout(device = address, pCreateInfo = address, pAllocator = address, pSetLayout = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkdescriptorsetlayoutcreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyDescriptorSetLayout(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", descriptorSetLayout = 0x" << nouppercase <<  hex << HandleCast(descriptorSetLayout) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyDescriptorSetLayout(device = address, descriptorSetLayout = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateDescriptorPool(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pDescriptorPool ? "*" : "") << "pDescriptorPool = 0x" << nouppercase <<  hex << (pDescriptorPool ? HandleCast(*pDescriptorPool) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateDescriptorPool(device = address, pCreateInfo = address, pAllocator = address, pDescriptorPool = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkdescriptorpoolcreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyDescriptorPool(device, descriptorPool, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyDescriptorPool(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", descriptorPool = 0x" << nouppercase <<  hex << HandleCast(descriptorPool) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyDescriptorPool(device = address, descriptorPool = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->ResetDescriptorPool(device, descriptorPool, flags);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkResetDescriptorPool(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", descriptorPool = 0x" << nouppercase <<  hex << HandleCast(descriptorPool) << dec << ", flags = 0x" << nouppercase <<  hex << flags << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkResetDescriptorPool(device = address, descriptorPool = address, flags = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->AllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkAllocateDescriptorSets(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pAllocateInfo = 0x" << nouppercase <<  hex << HandleCast(pAllocateInfo) << dec << ", " << (pDescriptorSets ? "*" : "") << "pDescriptorSets = 0x" << nouppercase <<  hex << (pDescriptorSets ? HandleCast(*pDescriptorSets) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkAllocateDescriptorSets(device = address, pAllocateInfo = address, pDescriptorSets = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocateInfo) {
            string tmp_str = vk_print_vkdescriptorsetallocateinfo(pAllocateInfo, "    ");
            (*outputStream) << "   pAllocateInfo:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkFreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->FreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkFreeDescriptorSets(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", descriptorPool = 0x" << nouppercase <<  hex << HandleCast(descriptorPool) << dec << ", descriptorSetCount = 0x" << nouppercase <<  hex << descriptorSetCount << dec << ", " << (pDescriptorSets ? "*" : "") << "pDescriptorSets = 0x" << nouppercase <<  hex << (pDescriptorSets ? HandleCast(*pDescriptorSets) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkFreeDescriptorSets(device = address, descriptorPool = address, descriptorSetCount = address, pDescriptorSets = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pDescriptorSets) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < descriptorSetCount; i++) {
                if (StreamControl::writeAddress == true) {
                    (*outputStream) << "   pDescriptorSets[" << i << "] = 0x" << nouppercase << hex << HandleCast(pDescriptorSets[i]) << dec << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->UpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkUpdateDescriptorSets(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", descriptorWriteCount = 0x" << nouppercase <<  hex << descriptorWriteCount << dec << ", pDescriptorWrites = 0x" << nouppercase <<  hex << HandleCast(pDescriptorWrites) << dec << ", descriptorCopyCount = 0x" << nouppercase <<  hex << descriptorCopyCount << dec << ", pDescriptorCopies = 0x" << nouppercase <<  hex << HandleCast(pDescriptorCopies) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkUpdateDescriptorSets(device = address, descriptorWriteCount = address, pDescriptorWrites = address, descriptorCopyCount = address, pDescriptorCopies = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pDescriptorWrites) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < descriptorWriteCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkwritedescriptorset(&pDescriptorWrites[i], "    ");
                    (*outputStream) << "   pDescriptorWrites[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pDescriptorCopies) {
            string tmp_str = vk_print_vkcopydescriptorset(pDescriptorCopies, "    ");
            (*outputStream) << "   pDescriptorCopies:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateFramebuffer(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pFramebuffer ? "*" : "") << "pFramebuffer = 0x" << nouppercase <<  hex << (pFramebuffer ? HandleCast(*pFramebuffer) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateFramebuffer(device = address, pCreateInfo = address, pAllocator = address, pFramebuffer = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkframebuffercreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyFramebuffer(device, framebuffer, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyFramebuffer(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", framebuffer = 0x" << nouppercase <<  hex << HandleCast(framebuffer) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyFramebuffer(device = address, framebuffer = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateRenderPass(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pRenderPass ? "*" : "") << "pRenderPass = 0x" << nouppercase <<  hex << (pRenderPass ? HandleCast(*pRenderPass) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateRenderPass(device = address, pCreateInfo = address, pAllocator = address, pRenderPass = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkrenderpasscreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyRenderPass(device, renderPass, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyRenderPass(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", renderPass = 0x" << nouppercase <<  hex << HandleCast(renderPass) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyRenderPass(device = address, renderPass = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->GetRenderAreaGranularity(device, renderPass, pGranularity);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetRenderAreaGranularity(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", renderPass = 0x" << nouppercase <<  hex << HandleCast(renderPass) << dec << ", pGranularity = 0x" << nouppercase <<  hex << HandleCast(pGranularity) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetRenderAreaGranularity(device = address, renderPass = address, pGranularity = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pGranularity) {
            string tmp_str = vk_print_vkextent2d(pGranularity, "    ");
            (*outputStream) << "   pGranularity:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateCommandPool(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pCommandPool ? "*" : "") << "pCommandPool = 0x" << nouppercase <<  hex << (pCommandPool ? HandleCast(*pCommandPool) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateCommandPool(device = address, pCreateInfo = address, pAllocator = address, pCommandPool = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkcommandpoolcreateinfo(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroyCommandPool(device, commandPool, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyCommandPool(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", commandPool = 0x" << nouppercase <<  hex << HandleCast(commandPool) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroyCommandPool(device = address, commandPool = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->ResetCommandPool(device, commandPool, flags);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkResetCommandPool(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", commandPool = 0x" << nouppercase <<  hex << HandleCast(commandPool) << dec << ", flags = 0x" << nouppercase <<  hex << flags << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkResetCommandPool(device = address, commandPool = address, flags = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->AllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkAllocateCommandBuffers(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pAllocateInfo = 0x" << nouppercase <<  hex << HandleCast(pAllocateInfo) << dec << ", " << (pCommandBuffers ? "*" : "") << "pCommandBuffers = 0x" << nouppercase <<  hex << (pCommandBuffers ? HandleCast(*pCommandBuffers) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkAllocateCommandBuffers(device = address, pAllocateInfo = address, pCommandBuffers = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocateInfo) {
            string tmp_str = vk_print_vkcommandbufferallocateinfo(pAllocateInfo, "    ");
            (*outputStream) << "   pAllocateInfo:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkFreeCommandBuffers(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", commandPool = 0x" << nouppercase <<  hex << HandleCast(commandPool) << dec << ", commandBufferCount = 0x" << nouppercase <<  hex << commandBufferCount << dec << ", " << (pCommandBuffers ? "*" : "") << "pCommandBuffers = 0x" << nouppercase <<  hex << (pCommandBuffers ? HandleCast(*pCommandBuffers) : 0) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkFreeCommandBuffers(device = address, commandPool = address, commandBufferCount = address, pCommandBuffers = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pCommandBuffers) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < commandBufferCount; i++) {
                if (StreamControl::writeAddress == true) {
                    (*outputStream) << "   pCommandBuffers[" << i << "] = 0x" << nouppercase << hex << HandleCast(pCommandBuffers[i]) << dec << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(commandBuffer)->BeginCommandBuffer(commandBuffer, pBeginInfo);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkBeginCommandBuffer(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", pBeginInfo = 0x" << nouppercase <<  hex << HandleCast(pBeginInfo) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkBeginCommandBuffer(commandBuffer = address, pBeginInfo = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pBeginInfo) {
            string tmp_str = vk_print_vkcommandbufferbegininfo(pBeginInfo, "    ");
            (*outputStream) << "   pBeginInfo:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEndCommandBuffer(VkCommandBuffer commandBuffer)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(commandBuffer)->EndCommandBuffer(commandBuffer);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkEndCommandBuffer(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkEndCommandBuffer(commandBuffer = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(commandBuffer)->ResetCommandBuffer(commandBuffer, flags);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkResetCommandBuffer(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", flags = 0x" << nouppercase <<  hex << flags << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkResetCommandBuffer(commandBuffer = address, flags = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBindPipeline(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", pipelineBindPoint = " << string_VkPipelineBindPoint(pipelineBindPoint) << ", pipeline = 0x" << nouppercase <<  hex << HandleCast(pipeline) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBindPipeline(commandBuffer = address, pipelineBindPoint = " << string_VkPipelineBindPoint(pipelineBindPoint) << ", pipeline = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetViewport(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", firstViewport = 0x" << nouppercase <<  hex << firstViewport << dec << ", viewportCount = 0x" << nouppercase <<  hex << viewportCount << dec << ", pViewports = 0x" << nouppercase <<  hex << HandleCast(pViewports) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetViewport(commandBuffer = address, firstViewport = address, viewportCount = address, pViewports = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pViewports) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < viewportCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkviewport(&pViewports[i], "    ");
                    (*outputStream) << "   pViewports[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetScissor(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", firstScissor = 0x" << nouppercase <<  hex << firstScissor << dec << ", scissorCount = 0x" << nouppercase <<  hex << scissorCount << dec << ", pScissors = 0x" << nouppercase <<  hex << HandleCast(pScissors) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetScissor(commandBuffer = address, firstScissor = address, scissorCount = address, pScissors = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pScissors) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < scissorCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkrect2d(&pScissors[i], "    ");
                    (*outputStream) << "   pScissors[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdSetLineWidth(commandBuffer, lineWidth);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetLineWidth(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", lineWidth = " << lineWidth << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetLineWidth(commandBuffer = address, lineWidth = " << lineWidth << ")\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetDepthBias(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", depthBiasConstantFactor = " << depthBiasConstantFactor << ", depthBiasClamp = " << depthBiasClamp << ", depthBiasSlopeFactor = " << depthBiasSlopeFactor << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetDepthBias(commandBuffer = address, depthBiasConstantFactor = " << depthBiasConstantFactor << ", depthBiasClamp = " << depthBiasClamp << ", depthBiasSlopeFactor = " << depthBiasSlopeFactor << ")\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4])
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdSetBlendConstants(commandBuffer, blendConstants);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetBlendConstants(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", blendConstants = " << "[" << blendConstants[0] << "," << blendConstants[1] << "," << blendConstants[2] << "," << blendConstants[3] << "]" << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetBlendConstants(commandBuffer = address, blendConstants = " << "[" << blendConstants[0] << "," << blendConstants[1] << "," << blendConstants[2] << "," << blendConstants[3] << "]" << ")\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetDepthBounds(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", minDepthBounds = " << minDepthBounds << ", maxDepthBounds = " << maxDepthBounds << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetDepthBounds(commandBuffer = address, minDepthBounds = " << minDepthBounds << ", maxDepthBounds = " << maxDepthBounds << ")\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetStencilCompareMask(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", faceMask = 0x" << nouppercase <<  hex << faceMask << dec << ", compareMask = 0x" << nouppercase <<  hex << compareMask << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetStencilCompareMask(commandBuffer = address, faceMask = address, compareMask = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetStencilWriteMask(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", faceMask = 0x" << nouppercase <<  hex << faceMask << dec << ", writeMask = 0x" << nouppercase <<  hex << writeMask << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetStencilWriteMask(commandBuffer = address, faceMask = address, writeMask = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdSetStencilReference(commandBuffer, faceMask, reference);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetStencilReference(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", faceMask = 0x" << nouppercase <<  hex << faceMask << dec << ", reference = 0x" << nouppercase <<  hex << reference << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetStencilReference(commandBuffer = address, faceMask = address, reference = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBindDescriptorSets(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", pipelineBindPoint = " << string_VkPipelineBindPoint(pipelineBindPoint) << ", layout = 0x" << nouppercase <<  hex << HandleCast(layout) << dec << ", firstSet = 0x" << nouppercase <<  hex << firstSet << dec << ", descriptorSetCount = 0x" << nouppercase <<  hex << descriptorSetCount << dec << ", " << (pDescriptorSets ? "*" : "") << "pDescriptorSets = 0x" << nouppercase <<  hex << (pDescriptorSets ? HandleCast(*pDescriptorSets) : 0) << dec << ", dynamicOffsetCount = 0x" << nouppercase <<  hex << dynamicOffsetCount << dec << ", " << (pDynamicOffsets ? "*" : "") << "pDynamicOffsets = 0x" << nouppercase <<  hex << (pDynamicOffsets ? *pDynamicOffsets : 0) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBindDescriptorSets(commandBuffer = address, pipelineBindPoint = " << string_VkPipelineBindPoint(pipelineBindPoint) << ", layout = address, firstSet = address, descriptorSetCount = address, pDescriptorSets = address, dynamicOffsetCount = address, pDynamicOffsets = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pDescriptorSets) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < descriptorSetCount; i++) {
                if (StreamControl::writeAddress == true) {
                    (*outputStream) << "   pDescriptorSets[" << i << "] = 0x" << nouppercase << hex << HandleCast(pDescriptorSets[i]) << dec << endl;
                }
            }
        }
        if (pDynamicOffsets) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < dynamicOffsetCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = string_convert_helper(pDynamicOffsets[i], "    ");
                    (*outputStream) << "   pDynamicOffsets[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBindIndexBuffer(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", buffer = 0x" << nouppercase <<  hex << HandleCast(buffer) << dec << ", offset = 0x" << nouppercase <<  hex << offset << dec << ", indexType = " << string_VkIndexType(indexType) << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBindIndexBuffer(commandBuffer = address, buffer = address, offset = address, indexType = " << string_VkIndexType(indexType) << ")\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBindVertexBuffers(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", firstBinding = 0x" << nouppercase <<  hex << firstBinding << dec << ", bindingCount = 0x" << nouppercase <<  hex << bindingCount << dec << ", " << (pBuffers ? "*" : "") << "pBuffers = 0x" << nouppercase <<  hex << (pBuffers ? HandleCast(*pBuffers) : 0) << dec << ", " << (pOffsets ? "*" : "") << "pOffsets = 0x" << nouppercase <<  hex << (pOffsets ? *pOffsets : 0) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBindVertexBuffers(commandBuffer = address, firstBinding = address, bindingCount = address, pBuffers = address, pOffsets = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDraw(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", vertexCount = 0x" << nouppercase <<  hex << vertexCount << dec << ", instanceCount = 0x" << nouppercase <<  hex << instanceCount << dec << ", firstVertex = 0x" << nouppercase <<  hex << firstVertex << dec << ", firstInstance = 0x" << nouppercase <<  hex << firstInstance << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDraw(commandBuffer = address, vertexCount = address, instanceCount = address, firstVertex = address, firstInstance = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDrawIndexed(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", indexCount = 0x" << nouppercase <<  hex << indexCount << dec << ", instanceCount = 0x" << nouppercase <<  hex << instanceCount << dec << ", firstIndex = 0x" << nouppercase <<  hex << firstIndex << dec << ", vertexOffset = 0x" << nouppercase <<  hex << vertexOffset << dec << ", firstInstance = 0x" << nouppercase <<  hex << firstInstance << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDrawIndexed(commandBuffer = address, indexCount = address, instanceCount = address, firstIndex = address, vertexOffset = address, firstInstance = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDrawIndirect(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", buffer = 0x" << nouppercase <<  hex << HandleCast(buffer) << dec << ", offset = 0x" << nouppercase <<  hex << offset << dec << ", drawCount = 0x" << nouppercase <<  hex << drawCount << dec << ", stride = 0x" << nouppercase <<  hex << stride << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDrawIndirect(commandBuffer = address, buffer = address, offset = address, drawCount = address, stride = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDrawIndexedIndirect(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", buffer = 0x" << nouppercase <<  hex << HandleCast(buffer) << dec << ", offset = 0x" << nouppercase <<  hex << offset << dec << ", drawCount = 0x" << nouppercase <<  hex << drawCount << dec << ", stride = 0x" << nouppercase <<  hex << stride << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDrawIndexedIndirect(commandBuffer = address, buffer = address, offset = address, drawCount = address, stride = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t z)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdDispatch(commandBuffer, x, y, z);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDispatch(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", x = 0x" << nouppercase <<  hex << x << dec << ", y = 0x" << nouppercase <<  hex << y << dec << ", z = 0x" << nouppercase <<  hex << z << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDispatch(commandBuffer = address, x = address, y = address, z = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdDispatchIndirect(commandBuffer, buffer, offset);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDispatchIndirect(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", buffer = 0x" << nouppercase <<  hex << HandleCast(buffer) << dec << ", offset = 0x" << nouppercase <<  hex << offset << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdDispatchIndirect(commandBuffer = address, buffer = address, offset = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdCopyBuffer(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", srcBuffer = 0x" << nouppercase <<  hex << HandleCast(srcBuffer) << dec << ", dstBuffer = 0x" << nouppercase <<  hex << HandleCast(dstBuffer) << dec << ", regionCount = 0x" << nouppercase <<  hex << regionCount << dec << ", pRegions = 0x" << nouppercase <<  hex << HandleCast(pRegions) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdCopyBuffer(commandBuffer = address, srcBuffer = address, dstBuffer = address, regionCount = address, pRegions = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pRegions) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < regionCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkbuffercopy(&pRegions[i], "    ");
                    (*outputStream) << "   pRegions[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdCopyImage(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", srcImage = 0x" << nouppercase <<  hex << HandleCast(srcImage) << dec << ", srcImageLayout = " << string_VkImageLayout(srcImageLayout) << ", dstImage = 0x" << nouppercase <<  hex << HandleCast(dstImage) << dec << ", dstImageLayout = " << string_VkImageLayout(dstImageLayout) << ", regionCount = 0x" << nouppercase <<  hex << regionCount << dec << ", pRegions = 0x" << nouppercase <<  hex << HandleCast(pRegions) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdCopyImage(commandBuffer = address, srcImage = address, srcImageLayout = " << string_VkImageLayout(srcImageLayout) << ", dstImage = address, dstImageLayout = " << string_VkImageLayout(dstImageLayout) << ", regionCount = address, pRegions = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pRegions) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < regionCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkimagecopy(&pRegions[i], "    ");
                    (*outputStream) << "   pRegions[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBlitImage(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", srcImage = 0x" << nouppercase <<  hex << HandleCast(srcImage) << dec << ", srcImageLayout = " << string_VkImageLayout(srcImageLayout) << ", dstImage = 0x" << nouppercase <<  hex << HandleCast(dstImage) << dec << ", dstImageLayout = " << string_VkImageLayout(dstImageLayout) << ", regionCount = 0x" << nouppercase <<  hex << regionCount << dec << ", pRegions = 0x" << nouppercase <<  hex << HandleCast(pRegions) << dec << ", filter = " << string_VkFilter(filter) << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBlitImage(commandBuffer = address, srcImage = address, srcImageLayout = " << string_VkImageLayout(srcImageLayout) << ", dstImage = address, dstImageLayout = " << string_VkImageLayout(dstImageLayout) << ", regionCount = address, pRegions = address, filter = " << string_VkFilter(filter) << ")\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pRegions) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < regionCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkimageblit(&pRegions[i], "    ");
                    (*outputStream) << "   pRegions[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdCopyBufferToImage(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", srcBuffer = 0x" << nouppercase <<  hex << HandleCast(srcBuffer) << dec << ", dstImage = 0x" << nouppercase <<  hex << HandleCast(dstImage) << dec << ", dstImageLayout = " << string_VkImageLayout(dstImageLayout) << ", regionCount = 0x" << nouppercase <<  hex << regionCount << dec << ", pRegions = 0x" << nouppercase <<  hex << HandleCast(pRegions) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdCopyBufferToImage(commandBuffer = address, srcBuffer = address, dstImage = address, dstImageLayout = " << string_VkImageLayout(dstImageLayout) << ", regionCount = address, pRegions = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pRegions) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < regionCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkbufferimagecopy(&pRegions[i], "    ");
                    (*outputStream) << "   pRegions[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdCopyImageToBuffer(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", srcImage = 0x" << nouppercase <<  hex << HandleCast(srcImage) << dec << ", srcImageLayout = " << string_VkImageLayout(srcImageLayout) << ", dstBuffer = 0x" << nouppercase <<  hex << HandleCast(dstBuffer) << dec << ", regionCount = 0x" << nouppercase <<  hex << regionCount << dec << ", pRegions = 0x" << nouppercase <<  hex << HandleCast(pRegions) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdCopyImageToBuffer(commandBuffer = address, srcImage = address, srcImageLayout = " << string_VkImageLayout(srcImageLayout) << ", dstBuffer = address, regionCount = address, pRegions = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pRegions) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < regionCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkbufferimagecopy(&pRegions[i], "    ");
                    (*outputStream) << "   pRegions[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdUpdateBuffer(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", dstBuffer = 0x" << nouppercase <<  hex << HandleCast(dstBuffer) << dec << ", dstOffset = 0x" << nouppercase <<  hex << dstOffset << dec << ", dataSize = 0x" << nouppercase <<  hex << dataSize << dec << ", pData = 0x" << nouppercase <<  hex << HandleCast(pData) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdUpdateBuffer(commandBuffer = address, dstBuffer = address, dstOffset = address, dataSize = address, pData = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdFillBuffer(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", dstBuffer = 0x" << nouppercase <<  hex << HandleCast(dstBuffer) << dec << ", dstOffset = 0x" << nouppercase <<  hex << dstOffset << dec << ", size = 0x" << nouppercase <<  hex << size << dec << ", data = 0x" << nouppercase <<  hex << data << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdFillBuffer(commandBuffer = address, dstBuffer = address, dstOffset = address, size = address, data = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdClearColorImage(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", image = 0x" << nouppercase <<  hex << HandleCast(image) << dec << ", imageLayout = " << string_VkImageLayout(imageLayout) << ", pColor = 0x" << nouppercase <<  hex << HandleCast(pColor) << dec << ", rangeCount = 0x" << nouppercase <<  hex << rangeCount << dec << ", pRanges = 0x" << nouppercase <<  hex << HandleCast(pRanges) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdClearColorImage(commandBuffer = address, image = address, imageLayout = " << string_VkImageLayout(imageLayout) << ", pColor = address, rangeCount = address, pRanges = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pColor) {
            string tmp_str = vk_print_vkclearcolorvalue(pColor, "    ");
            (*outputStream) << "   pColor:\n" << tmp_str << endl;
        }
        uint32_t i;
        if (pRanges) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < rangeCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkimagesubresourcerange(&pRanges[i], "    ");
                    (*outputStream) << "   pRanges[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdClearDepthStencilImage(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", image = 0x" << nouppercase <<  hex << HandleCast(image) << dec << ", imageLayout = " << string_VkImageLayout(imageLayout) << ", pDepthStencil = 0x" << nouppercase <<  hex << HandleCast(pDepthStencil) << dec << ", rangeCount = 0x" << nouppercase <<  hex << rangeCount << dec << ", pRanges = 0x" << nouppercase <<  hex << HandleCast(pRanges) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdClearDepthStencilImage(commandBuffer = address, image = address, imageLayout = " << string_VkImageLayout(imageLayout) << ", pDepthStencil = address, rangeCount = address, pRanges = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pDepthStencil) {
            string tmp_str = vk_print_vkcleardepthstencilvalue(pDepthStencil, "    ");
            (*outputStream) << "   pDepthStencil:\n" << tmp_str << endl;
        }
        uint32_t i;
        if (pRanges) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < rangeCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkimagesubresourcerange(&pRanges[i], "    ");
                    (*outputStream) << "   pRanges[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdClearAttachments(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", attachmentCount = 0x" << nouppercase <<  hex << attachmentCount << dec << ", pAttachments = 0x" << nouppercase <<  hex << HandleCast(pAttachments) << dec << ", rectCount = 0x" << nouppercase <<  hex << rectCount << dec << ", pRects = 0x" << nouppercase <<  hex << HandleCast(pRects) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdClearAttachments(commandBuffer = address, attachmentCount = address, pAttachments = address, rectCount = address, pRects = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pAttachments) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < attachmentCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkclearattachment(&pAttachments[i], "    ");
                    (*outputStream) << "   pAttachments[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
        if (pRects) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < rectCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkclearrect(&pRects[i], "    ");
                    (*outputStream) << "   pRects[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdResolveImage(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", srcImage = 0x" << nouppercase <<  hex << HandleCast(srcImage) << dec << ", srcImageLayout = " << string_VkImageLayout(srcImageLayout) << ", dstImage = 0x" << nouppercase <<  hex << HandleCast(dstImage) << dec << ", dstImageLayout = " << string_VkImageLayout(dstImageLayout) << ", regionCount = 0x" << nouppercase <<  hex << regionCount << dec << ", pRegions = 0x" << nouppercase <<  hex << HandleCast(pRegions) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdResolveImage(commandBuffer = address, srcImage = address, srcImageLayout = " << string_VkImageLayout(srcImageLayout) << ", dstImage = address, dstImageLayout = " << string_VkImageLayout(dstImageLayout) << ", regionCount = address, pRegions = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pRegions) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < regionCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkimageresolve(&pRegions[i], "    ");
                    (*outputStream) << "   pRegions[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdSetEvent(commandBuffer, event, stageMask);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetEvent(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", event = 0x" << nouppercase <<  hex << HandleCast(event) << dec << ", stageMask = 0x" << nouppercase <<  hex << stageMask << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdSetEvent(commandBuffer = address, event = address, stageMask = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdResetEvent(commandBuffer, event, stageMask);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdResetEvent(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", event = 0x" << nouppercase <<  hex << HandleCast(event) << dec << ", stageMask = 0x" << nouppercase <<  hex << stageMask << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdResetEvent(commandBuffer = address, event = address, stageMask = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdWaitEvents(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", eventCount = 0x" << nouppercase <<  hex << eventCount << dec << ", " << (pEvents ? "*" : "") << "pEvents = 0x" << nouppercase <<  hex << (pEvents ? HandleCast(*pEvents) : 0) << dec << ", srcStageMask = 0x" << nouppercase <<  hex << srcStageMask << dec << ", dstStageMask = 0x" << nouppercase <<  hex << dstStageMask << dec << ", memoryBarrierCount = 0x" << nouppercase <<  hex << memoryBarrierCount << dec << ", pMemoryBarriers = 0x" << nouppercase <<  hex << HandleCast(pMemoryBarriers) << dec << ", bufferMemoryBarrierCount = 0x" << nouppercase <<  hex << bufferMemoryBarrierCount << dec << ", pBufferMemoryBarriers = 0x" << nouppercase <<  hex << HandleCast(pBufferMemoryBarriers) << dec << ", imageMemoryBarrierCount = 0x" << nouppercase <<  hex << imageMemoryBarrierCount << dec << ", pImageMemoryBarriers = 0x" << nouppercase <<  hex << HandleCast(pImageMemoryBarriers) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdWaitEvents(commandBuffer = address, eventCount = address, pEvents = address, srcStageMask = address, dstStageMask = address, memoryBarrierCount = address, pMemoryBarriers = address, bufferMemoryBarrierCount = address, pBufferMemoryBarriers = address, imageMemoryBarrierCount = address, pImageMemoryBarriers = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pBufferMemoryBarriers) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < bufferMemoryBarrierCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkbuffermemorybarrier(&pBufferMemoryBarriers[i], "    ");
                    (*outputStream) << "   pBufferMemoryBarriers[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
        if (pEvents) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < eventCount; i++) {
                if (StreamControl::writeAddress == true) {
                    (*outputStream) << "   pEvents[" << i << "] = 0x" << nouppercase << hex << HandleCast(pEvents[i]) << dec << endl;
                }
            }
        }
        if (pImageMemoryBarriers) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < imageMemoryBarrierCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkimagememorybarrier(&pImageMemoryBarriers[i], "    ");
                    (*outputStream) << "   pImageMemoryBarriers[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
        if (pMemoryBarriers) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < memoryBarrierCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkmemorybarrier(&pMemoryBarriers[i], "    ");
                    (*outputStream) << "   pMemoryBarriers[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdPipelineBarrier(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", srcStageMask = 0x" << nouppercase <<  hex << srcStageMask << dec << ", dstStageMask = 0x" << nouppercase <<  hex << dstStageMask << dec << ", dependencyFlags = 0x" << nouppercase <<  hex << dependencyFlags << dec << ", memoryBarrierCount = 0x" << nouppercase <<  hex << memoryBarrierCount << dec << ", pMemoryBarriers = 0x" << nouppercase <<  hex << HandleCast(pMemoryBarriers) << dec << ", bufferMemoryBarrierCount = 0x" << nouppercase <<  hex << bufferMemoryBarrierCount << dec << ", pBufferMemoryBarriers = 0x" << nouppercase <<  hex << HandleCast(pBufferMemoryBarriers) << dec << ", imageMemoryBarrierCount = 0x" << nouppercase <<  hex << imageMemoryBarrierCount << dec << ", pImageMemoryBarriers = 0x" << nouppercase <<  hex << HandleCast(pImageMemoryBarriers) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdPipelineBarrier(commandBuffer = address, srcStageMask = address, dstStageMask = address, dependencyFlags = address, memoryBarrierCount = address, pMemoryBarriers = address, bufferMemoryBarrierCount = address, pBufferMemoryBarriers = address, imageMemoryBarrierCount = address, pImageMemoryBarriers = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pImageMemoryBarriers) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < imageMemoryBarrierCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkimagememorybarrier(&pImageMemoryBarriers[i], "    ");
                    (*outputStream) << "   pImageMemoryBarriers[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
        if (pMemoryBarriers) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < memoryBarrierCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkmemorybarrier(&pMemoryBarriers[i], "    ");
                    (*outputStream) << "   pMemoryBarriers[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
        if (pBufferMemoryBarriers) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < bufferMemoryBarrierCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vkbuffermemorybarrier(&pBufferMemoryBarriers[i], "    ");
                    (*outputStream) << "   pBufferMemoryBarriers[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdBeginQuery(commandBuffer, queryPool, query, flags);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBeginQuery(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", queryPool = 0x" << nouppercase <<  hex << HandleCast(queryPool) << dec << ", query = 0x" << nouppercase <<  hex << query << dec << ", flags = 0x" << nouppercase <<  hex << flags << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBeginQuery(commandBuffer = address, queryPool = address, query = address, flags = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdEndQuery(commandBuffer, queryPool, query);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdEndQuery(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", queryPool = 0x" << nouppercase <<  hex << HandleCast(queryPool) << dec << ", query = 0x" << nouppercase <<  hex << query << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdEndQuery(commandBuffer = address, queryPool = address, query = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdResetQueryPool(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", queryPool = 0x" << nouppercase <<  hex << HandleCast(queryPool) << dec << ", firstQuery = 0x" << nouppercase <<  hex << firstQuery << dec << ", queryCount = 0x" << nouppercase <<  hex << queryCount << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdResetQueryPool(commandBuffer = address, queryPool = address, firstQuery = address, queryCount = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdWriteTimestamp(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", pipelineStage = " << string_VkPipelineStageFlagBits(pipelineStage) << ", queryPool = 0x" << nouppercase <<  hex << HandleCast(queryPool) << dec << ", query = 0x" << nouppercase <<  hex << query << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdWriteTimestamp(commandBuffer = address, pipelineStage = " << string_VkPipelineStageFlagBits(pipelineStage) << ", queryPool = address, query = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdCopyQueryPoolResults(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", queryPool = 0x" << nouppercase <<  hex << HandleCast(queryPool) << dec << ", firstQuery = 0x" << nouppercase <<  hex << firstQuery << dec << ", queryCount = 0x" << nouppercase <<  hex << queryCount << dec << ", dstBuffer = 0x" << nouppercase <<  hex << HandleCast(dstBuffer) << dec << ", dstOffset = 0x" << nouppercase <<  hex << dstOffset << dec << ", stride = 0x" << nouppercase <<  hex << stride << dec << ", flags = 0x" << nouppercase <<  hex << flags << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdCopyQueryPoolResults(commandBuffer = address, queryPool = address, firstQuery = address, queryCount = address, dstBuffer = address, dstOffset = address, stride = address, flags = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdPushConstants(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", layout = 0x" << nouppercase <<  hex << HandleCast(layout) << dec << ", stageFlags = 0x" << nouppercase <<  hex << stageFlags << dec << ", offset = 0x" << nouppercase <<  hex << offset << dec << ", size = 0x" << nouppercase <<  hex << size << dec << ", pValues = 0x" << nouppercase <<  hex << HandleCast(pValues) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdPushConstants(commandBuffer = address, layout = address, stageFlags = address, offset = address, size = address, pValues = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBeginRenderPass(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", pRenderPassBegin = 0x" << nouppercase <<  hex << HandleCast(pRenderPassBegin) << dec << ", contents = " << string_VkSubpassContents(contents) << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdBeginRenderPass(commandBuffer = address, pRenderPassBegin = address, contents = " << string_VkSubpassContents(contents) << ")\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pRenderPassBegin) {
            string tmp_str = vk_print_vkrenderpassbegininfo(pRenderPassBegin, "    ");
            (*outputStream) << "   pRenderPassBegin:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdNextSubpass(commandBuffer, contents);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdNextSubpass(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", contents = " << string_VkSubpassContents(contents) << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdNextSubpass(commandBuffer = address, contents = " << string_VkSubpassContents(contents) << ")\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass(VkCommandBuffer commandBuffer)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdEndRenderPass(commandBuffer);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdEndRenderPass(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdEndRenderPass(commandBuffer = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(commandBuffer)->CmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdExecuteCommands(commandBuffer = 0x" << nouppercase <<  hex << HandleCast(commandBuffer) << dec << ", commandBufferCount = 0x" << nouppercase <<  hex << commandBufferCount << dec << ", " << (pCommandBuffers ? "*" : "") << "pCommandBuffers = 0x" << nouppercase <<  hex << (pCommandBuffers ? HandleCast(*pCommandBuffers) : 0) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCmdExecuteCommands(commandBuffer = address, commandBufferCount = address, pCommandBuffers = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pCommandBuffers) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < commandBufferCount; i++) {
                if (StreamControl::writeAddress == true) {
                    (*outputStream) << "   pCommandBuffers[" << i << "] = 0x" << nouppercase << hex << HandleCast(pCommandBuffers[i]) << dec << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    instance_dispatch_table(instance)->DestroySurfaceKHR(instance, surface, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroySurfaceKHR(instance = 0x" << nouppercase <<  hex << HandleCast(instance) << dec << ", surface = 0x" << nouppercase <<  hex << HandleCast(surface) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroySurfaceKHR(instance = address, surface = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", queueFamilyIndex = 0x" << nouppercase <<  hex << queueFamilyIndex << dec << ", surface = 0x" << nouppercase <<  hex << HandleCast(surface) << dec << ", pSupported = " << pSupported << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice = address, queueFamilyIndex = address, surface = address, pSupported = " << pSupported << ") = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", surface = 0x" << nouppercase <<  hex << HandleCast(surface) << dec << ", pSurfaceCapabilities = 0x" << nouppercase <<  hex << HandleCast(pSurfaceCapabilities) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice = address, surface = address, pSurfaceCapabilities = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pSurfaceCapabilities) {
            string tmp_str = vk_print_vksurfacecapabilitieskhr(pSurfaceCapabilities, "    ");
            (*outputStream) << "   pSurfaceCapabilities:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", surface = 0x" << nouppercase <<  hex << HandleCast(surface) << dec << ", " << (pSurfaceFormatCount ? "*" : "") << "pSurfaceFormatCount = 0x" << nouppercase <<  hex << (pSurfaceFormatCount ? *pSurfaceFormatCount : 0) << dec << ", pSurfaceFormats = 0x" << nouppercase <<  hex << HandleCast(pSurfaceFormats) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice = address, surface = address, pSurfaceFormatCount = address, pSurfaceFormats = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pSurfaceFormats) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < *pSurfaceFormatCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = vk_print_vksurfaceformatkhr(&pSurfaceFormats[i], "    ");
                    (*outputStream) << "   pSurfaceFormats[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", surface = 0x" << nouppercase <<  hex << HandleCast(surface) << dec << ", " << (pPresentModeCount ? "*" : "") << "pPresentModeCount = 0x" << nouppercase <<  hex << (pPresentModeCount ? *pPresentModeCount : 0) << dec << ", pPresentModes = 0x" << nouppercase <<  hex << HandleCast(pPresentModes) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice = address, surface = address, pPresentModeCount = address, pPresentModes = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pPresentModes) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < *pPresentModeCount; i++) {
                if (StreamControl::writeAddress == true) {
                    string tmp_str = string_convert_helper(pPresentModes[i], "    ");
                    (*outputStream) << "   pPresentModes[" << i << "]:\n" << tmp_str << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateSwapchainKHR(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", pSwapchain = 0x" << nouppercase <<  hex << HandleCast(pSwapchain) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateSwapchainKHR(device = address, pCreateInfo = address, pAllocator = address, pSwapchain = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkswapchaincreateinfokhr(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
    using namespace StreamControl;
    using namespace std;
    device_dispatch_table(device)->DestroySwapchainKHR(device, swapchain, pAllocator);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroySwapchainKHR(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", swapchain = 0x" << nouppercase <<  hex << HandleCast(swapchain) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkDestroySwapchainKHR(device = address, swapchain = address, pAllocator = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->GetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetSwapchainImagesKHR(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", swapchain = 0x" << nouppercase <<  hex << HandleCast(swapchain) << dec << ", " << (pSwapchainImageCount ? "*" : "") << "pSwapchainImageCount = 0x" << nouppercase <<  hex << (pSwapchainImageCount ? *pSwapchainImageCount : 0) << dec << ", pSwapchainImages = 0x" << nouppercase <<  hex << HandleCast(pSwapchainImages) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetSwapchainImagesKHR(device = address, swapchain = address, pSwapchainImageCount = address, pSwapchainImages = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pSwapchainImages) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < *pSwapchainImageCount; i++) {
                if (StreamControl::writeAddress == true) {
                    (*outputStream) << "   pSwapchainImages[" << i << "] = 0x" << nouppercase << hex << HandleCast(pSwapchainImages[i]) << dec << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->AcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkAcquireNextImageKHR(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", swapchain = 0x" << nouppercase <<  hex << HandleCast(swapchain) << dec << ", timeout = 0x" << nouppercase <<  hex << timeout << dec << ", semaphore = 0x" << nouppercase <<  hex << HandleCast(semaphore) << dec << ", fence = 0x" << nouppercase <<  hex << HandleCast(fence) << dec << ", " << (pImageIndex ? "*" : "") << "pImageIndex = 0x" << nouppercase <<  hex << (pImageIndex ? *pImageIndex : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkAcquireNextImageKHR(device = address, swapchain = address, timeout = address, semaphore = address, fence = address, pImageIndex = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
    using namespace StreamControl;
    using namespace std;
    VkLayerDispatchTable *pDisp  = device_dispatch_table(queue);
    VkResult result = pDisp->QueuePresentKHR(queue, pPresentInfo);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkQueuePresentKHR(queue = 0x" << nouppercase <<  hex << HandleCast(queue) << dec << ", pPresentInfo = 0x" << nouppercase <<  hex << HandleCast(pPresentInfo) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkQueuePresentKHR(queue = address, pPresentInfo = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pPresentInfo) {
            string tmp_str = vk_print_vkpresentinfokhr(pPresentInfo, "    ");
            (*outputStream) << "   pPresentInfo:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    ++g_frameCounter;
    return result;
}

#ifdef VK_USE_PLATFORM_XCB_KHR

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(instance)->CreateXcbSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateXcbSurfaceKHR(instance = 0x" << nouppercase <<  hex << HandleCast(instance) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pSurface ? "*" : "") << "pSurface = 0x" << nouppercase <<  hex << (pSurface ? HandleCast(*pSurface) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateXcbSurfaceKHR(instance = address, pCreateInfo = address, pAllocator = address, pSurface = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkxcbsurfacecreateinfokhr(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

#endif  // VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_XCB_KHR

VK_LAYER_EXPORT VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id)
{
    using namespace StreamControl;
    using namespace std;
    VkBool32 result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex, connection, visual_id);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", queueFamilyIndex = 0x" << nouppercase <<  hex << queueFamilyIndex << dec << ", connection = 0x" << nouppercase <<  hex << HandleCast(connection) << dec << ", visual_id = 0x" << nouppercase <<  hex << visual_id << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice = address, queueFamilyIndex = address, connection = address, visual_id = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

#endif  // VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_XLIB_KHR

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(instance)->CreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateXlibSurfaceKHR(instance = 0x" << nouppercase <<  hex << HandleCast(instance) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pSurface ? "*" : "") << "pSurface = 0x" << nouppercase <<  hex << (pSurface ? HandleCast(*pSurface) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateXlibSurfaceKHR(instance = address, pCreateInfo = address, pAllocator = address, pSurface = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkxlibsurfacecreateinfokhr(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

#endif  // VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_XLIB_KHR

VK_LAYER_EXPORT VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID)
{
    using namespace StreamControl;
    using namespace std;
    VkBool32 result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, dpy, visualID);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", queueFamilyIndex = 0x" << nouppercase <<  hex << queueFamilyIndex << dec << ", dpy = 0x" << nouppercase <<  hex << HandleCast(dpy) << dec << ", visualID = 0x" << nouppercase <<  hex << HandleCast(visualID) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice = address, queueFamilyIndex = address, dpy = address, visualID = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

#endif  // VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateWaylandSurfaceKHR(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(instance)->CreateWaylandSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateWaylandSurfaceKHR(instance = 0x" << nouppercase <<  hex << HandleCast(instance) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pSurface ? "*" : "") << "pSurface = 0x" << nouppercase <<  hex << (pSurface ? HandleCast(*pSurface) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateWaylandSurfaceKHR(instance = address, pCreateInfo = address, pAllocator = address, pSurface = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkwaylandsurfacecreateinfokhr(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

#endif  // VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

VK_LAYER_EXPORT VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display)
{
    using namespace StreamControl;
    using namespace std;
    VkBool32 result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, display);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", queueFamilyIndex = 0x" << nouppercase <<  hex << queueFamilyIndex << dec << ", display = 0x" << nouppercase <<  hex << HandleCast(display) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice = address, queueFamilyIndex = address, display = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

#endif  // VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateMirSurfaceKHR(VkInstance instance, const VkMirSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(instance)->CreateMirSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateMirSurfaceKHR(instance = 0x" << nouppercase <<  hex << HandleCast(instance) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pSurface ? "*" : "") << "pSurface = 0x" << nouppercase <<  hex << (pSurface ? HandleCast(*pSurface) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateMirSurfaceKHR(instance = address, pCreateInfo = address, pAllocator = address, pSurface = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkmirsurfacecreateinfokhr(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

#endif  // VK_USE_PLATFORM_MIR_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

VK_LAYER_EXPORT VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceMirPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection)
{
    using namespace StreamControl;
    using namespace std;
    VkBool32 result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceMirPresentationSupportKHR(physicalDevice, queueFamilyIndex, connection);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceMirPresentationSupportKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", queueFamilyIndex = 0x" << nouppercase <<  hex << queueFamilyIndex << dec << ", connection = 0x" << nouppercase <<  hex << HandleCast(connection) << dec << ")\n";
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceMirPresentationSupportKHR(physicalDevice = address, queueFamilyIndex = address, connection = address)\n";
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

#endif  // VK_USE_PLATFORM_MIR_KHR

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", " << (pPropertyCount ? "*" : "") << "pPropertyCount = 0x" << nouppercase <<  hex << (pPropertyCount ? *pPropertyCount : 0) << dec << ", pProperties = 0x" << nouppercase <<  hex << HandleCast(pProperties) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice = address, pPropertyCount = address, pProperties = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pProperties) {
            string tmp_str = vk_print_vkdisplaypropertieskhr(pProperties, "    ");
            (*outputStream) << "   pProperties:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", " << (pPropertyCount ? "*" : "") << "pPropertyCount = 0x" << nouppercase <<  hex << (pPropertyCount ? *pPropertyCount : 0) << dec << ", pProperties = 0x" << nouppercase <<  hex << HandleCast(pProperties) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice = address, pPropertyCount = address, pProperties = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pProperties) {
            string tmp_str = vk_print_vkdisplayplanepropertieskhr(pProperties, "    ");
            (*outputStream) << "   pProperties:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(physicalDevice)->GetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, pDisplayCount, pDisplays);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", planeIndex = 0x" << nouppercase <<  hex << planeIndex << dec << ", " << (pDisplayCount ? "*" : "") << "pDisplayCount = 0x" << nouppercase <<  hex << (pDisplayCount ? *pDisplayCount : 0) << dec << ", " << (pDisplays ? "*" : "") << "pDisplays = 0x" << nouppercase <<  hex << (pDisplays ? HandleCast(*pDisplays) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice = address, planeIndex = address, pDisplayCount = address, pDisplays = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
        uint32_t i;
        if (pDisplays) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1146
            for (i = 0; i < *pDisplayCount; i++) {
                if (StreamControl::writeAddress == true) {
                    (*outputStream) << "   pDisplays[" << i << "] = 0x" << nouppercase << hex << HandleCast(pDisplays[i]) << dec << endl;
                }
            }
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(physicalDevice)->GetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetDisplayModePropertiesKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", display = 0x" << nouppercase <<  hex << HandleCast(display) << dec << ", " << (pPropertyCount ? "*" : "") << "pPropertyCount = 0x" << nouppercase <<  hex << (pPropertyCount ? *pPropertyCount : 0) << dec << ", pProperties = 0x" << nouppercase <<  hex << HandleCast(pProperties) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetDisplayModePropertiesKHR(physicalDevice = address, display = address, pPropertyCount = address, pProperties = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pProperties) {
            string tmp_str = vk_print_vkdisplaymodepropertieskhr(pProperties, "    ");
            (*outputStream) << "   pProperties:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(physicalDevice)->CreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateDisplayModeKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", display = 0x" << nouppercase <<  hex << HandleCast(display) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pMode ? "*" : "") << "pMode = 0x" << nouppercase <<  hex << (pMode ? HandleCast(*pMode) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateDisplayModeKHR(physicalDevice = address, display = address, pCreateInfo = address, pAllocator = address, pMode = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkdisplaymodecreateinfokhr(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(physicalDevice)->GetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetDisplayPlaneCapabilitiesKHR(physicalDevice = 0x" << nouppercase <<  hex << HandleCast(physicalDevice) << dec << ", mode = 0x" << nouppercase <<  hex << HandleCast(mode) << dec << ", planeIndex = 0x" << nouppercase <<  hex << planeIndex << dec << ", pCapabilities = 0x" << nouppercase <<  hex << HandleCast(pCapabilities) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkGetDisplayPlaneCapabilitiesKHR(physicalDevice = address, mode = address, planeIndex = address, pCapabilities = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCapabilities) {
            string tmp_str = vk_print_vkdisplayplanecapabilitieskhr(pCapabilities, "    ");
            (*outputStream) << "   pCapabilities:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = instance_dispatch_table(instance)->CreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateDisplayPlaneSurfaceKHR(instance = 0x" << nouppercase <<  hex << HandleCast(instance) << dec << ", pCreateInfo = 0x" << nouppercase <<  hex << HandleCast(pCreateInfo) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pSurface ? "*" : "") << "pSurface = 0x" << nouppercase <<  hex << (pSurface ? HandleCast(*pSurface) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateDisplayPlaneSurfaceKHR(instance = address, pCreateInfo = address, pAllocator = address, pSurface = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfo) {
            string tmp_str = vk_print_vkdisplaysurfacecreateinfokhr(pCreateInfo, "    ");
            (*outputStream) << "   pCreateInfo:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains)
{
    using namespace StreamControl;
    using namespace std;
    VkResult result = device_dispatch_table(device)->CreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
    loader_platform_thread_lock_mutex(&printLock);
    // CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1047
    if (StreamControl::writeAddress == true) {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateSharedSwapchainsKHR(device = 0x" << nouppercase <<  hex << HandleCast(device) << dec << ", swapchainCount = 0x" << nouppercase <<  hex << swapchainCount << dec << ", pCreateInfos = 0x" << nouppercase <<  hex << HandleCast(pCreateInfos) << dec << ", pAllocator = 0x" << nouppercase <<  hex << HandleCast(pAllocator) << dec << ", " << (pSwapchains ? "*" : "") << "pSwapchains = 0x" << nouppercase <<  hex << (pSwapchains ? HandleCast(*pSwapchains) : 0) << dec << ") = " << string_VkResult((VkResult)result) << endl;
    }
    else {
        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vkCreateSharedSwapchainsKHR(device = address, swapchainCount = address, pCreateInfos = address, pAllocator = address, pSwapchains = address) = " << string_VkResult((VkResult)result) << endl;
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1107
    if (g_ApiDumpDetailed) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1114
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pCreateInfos) {
            string tmp_str = vk_print_vkswapchaincreateinfokhr(pCreateInfos, "    ");
            (*outputStream) << "   pCreateInfos:\n" << tmp_str << endl;
        }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #1123
        if (pAllocator) {
            string tmp_str = vk_print_vkallocationcallbacks(pAllocator, "    ");
            (*outputStream) << "   pAllocator:\n" << tmp_str << endl;
        }
    }
    loader_platform_thread_unlock_mutex(&printLock);
    return result;
}

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #409
static inline PFN_vkVoidFunction layer_intercept_proc(const char *name)
{
    if (!name || name[0] != 'v' || name[1] != 'k')
        return NULL;

    name += 2;
    if (!strcmp(name, "CreateInstance"))
        return (PFN_vkVoidFunction) vkCreateInstance;
    if (!strcmp(name, "DestroyInstance"))
        return (PFN_vkVoidFunction) vkDestroyInstance;
    if (!strcmp(name, "EnumeratePhysicalDevices"))
        return (PFN_vkVoidFunction) vkEnumeratePhysicalDevices;
    if (!strcmp(name, "GetPhysicalDeviceFeatures"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceFeatures;
    if (!strcmp(name, "GetPhysicalDeviceFormatProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceFormatProperties;
    if (!strcmp(name, "GetPhysicalDeviceImageFormatProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceImageFormatProperties;
    if (!strcmp(name, "GetPhysicalDeviceProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceProperties;
    if (!strcmp(name, "GetPhysicalDeviceQueueFamilyProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceQueueFamilyProperties;
    if (!strcmp(name, "GetPhysicalDeviceMemoryProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceMemoryProperties;
    if (!strcmp(name, "CreateDevice"))
        return (PFN_vkVoidFunction) vkCreateDevice;
    if (!strcmp(name, "DestroyDevice"))
        return (PFN_vkVoidFunction) vkDestroyDevice;
    if (!strcmp(name, "EnumerateInstanceExtensionProperties"))
        return (PFN_vkVoidFunction) vkEnumerateInstanceExtensionProperties;
    if (!strcmp(name, "EnumerateInstanceLayerProperties"))
        return (PFN_vkVoidFunction) vkEnumerateInstanceLayerProperties;
    if (!strcmp(name, "EnumerateDeviceLayerProperties"))
        return (PFN_vkVoidFunction) vkEnumerateDeviceLayerProperties;
    if (!strcmp(name, "GetDeviceQueue"))
        return (PFN_vkVoidFunction) vkGetDeviceQueue;
    if (!strcmp(name, "QueueSubmit"))
        return (PFN_vkVoidFunction) vkQueueSubmit;
    if (!strcmp(name, "QueueWaitIdle"))
        return (PFN_vkVoidFunction) vkQueueWaitIdle;
    if (!strcmp(name, "DeviceWaitIdle"))
        return (PFN_vkVoidFunction) vkDeviceWaitIdle;
    if (!strcmp(name, "AllocateMemory"))
        return (PFN_vkVoidFunction) vkAllocateMemory;
    if (!strcmp(name, "FreeMemory"))
        return (PFN_vkVoidFunction) vkFreeMemory;
    if (!strcmp(name, "MapMemory"))
        return (PFN_vkVoidFunction) vkMapMemory;
    if (!strcmp(name, "UnmapMemory"))
        return (PFN_vkVoidFunction) vkUnmapMemory;
    if (!strcmp(name, "FlushMappedMemoryRanges"))
        return (PFN_vkVoidFunction) vkFlushMappedMemoryRanges;
    if (!strcmp(name, "InvalidateMappedMemoryRanges"))
        return (PFN_vkVoidFunction) vkInvalidateMappedMemoryRanges;
    if (!strcmp(name, "GetDeviceMemoryCommitment"))
        return (PFN_vkVoidFunction) vkGetDeviceMemoryCommitment;
    if (!strcmp(name, "BindBufferMemory"))
        return (PFN_vkVoidFunction) vkBindBufferMemory;
    if (!strcmp(name, "BindImageMemory"))
        return (PFN_vkVoidFunction) vkBindImageMemory;
    if (!strcmp(name, "GetBufferMemoryRequirements"))
        return (PFN_vkVoidFunction) vkGetBufferMemoryRequirements;
    if (!strcmp(name, "GetImageMemoryRequirements"))
        return (PFN_vkVoidFunction) vkGetImageMemoryRequirements;
    if (!strcmp(name, "GetImageSparseMemoryRequirements"))
        return (PFN_vkVoidFunction) vkGetImageSparseMemoryRequirements;
    if (!strcmp(name, "GetPhysicalDeviceSparseImageFormatProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceSparseImageFormatProperties;
    if (!strcmp(name, "QueueBindSparse"))
        return (PFN_vkVoidFunction) vkQueueBindSparse;
    if (!strcmp(name, "CreateFence"))
        return (PFN_vkVoidFunction) vkCreateFence;
    if (!strcmp(name, "DestroyFence"))
        return (PFN_vkVoidFunction) vkDestroyFence;
    if (!strcmp(name, "ResetFences"))
        return (PFN_vkVoidFunction) vkResetFences;
    if (!strcmp(name, "GetFenceStatus"))
        return (PFN_vkVoidFunction) vkGetFenceStatus;
    if (!strcmp(name, "WaitForFences"))
        return (PFN_vkVoidFunction) vkWaitForFences;
    if (!strcmp(name, "CreateSemaphore"))
        return (PFN_vkVoidFunction) vkCreateSemaphore;
    if (!strcmp(name, "DestroySemaphore"))
        return (PFN_vkVoidFunction) vkDestroySemaphore;
    if (!strcmp(name, "CreateEvent"))
        return (PFN_vkVoidFunction) vkCreateEvent;
    if (!strcmp(name, "DestroyEvent"))
        return (PFN_vkVoidFunction) vkDestroyEvent;
    if (!strcmp(name, "GetEventStatus"))
        return (PFN_vkVoidFunction) vkGetEventStatus;
    if (!strcmp(name, "SetEvent"))
        return (PFN_vkVoidFunction) vkSetEvent;
    if (!strcmp(name, "ResetEvent"))
        return (PFN_vkVoidFunction) vkResetEvent;
    if (!strcmp(name, "CreateQueryPool"))
        return (PFN_vkVoidFunction) vkCreateQueryPool;
    if (!strcmp(name, "DestroyQueryPool"))
        return (PFN_vkVoidFunction) vkDestroyQueryPool;
    if (!strcmp(name, "GetQueryPoolResults"))
        return (PFN_vkVoidFunction) vkGetQueryPoolResults;
    if (!strcmp(name, "CreateBuffer"))
        return (PFN_vkVoidFunction) vkCreateBuffer;
    if (!strcmp(name, "DestroyBuffer"))
        return (PFN_vkVoidFunction) vkDestroyBuffer;
    if (!strcmp(name, "CreateBufferView"))
        return (PFN_vkVoidFunction) vkCreateBufferView;
    if (!strcmp(name, "DestroyBufferView"))
        return (PFN_vkVoidFunction) vkDestroyBufferView;
    if (!strcmp(name, "CreateImage"))
        return (PFN_vkVoidFunction) vkCreateImage;
    if (!strcmp(name, "DestroyImage"))
        return (PFN_vkVoidFunction) vkDestroyImage;
    if (!strcmp(name, "GetImageSubresourceLayout"))
        return (PFN_vkVoidFunction) vkGetImageSubresourceLayout;
    if (!strcmp(name, "CreateImageView"))
        return (PFN_vkVoidFunction) vkCreateImageView;
    if (!strcmp(name, "DestroyImageView"))
        return (PFN_vkVoidFunction) vkDestroyImageView;
    if (!strcmp(name, "CreateShaderModule"))
        return (PFN_vkVoidFunction) vkCreateShaderModule;
    if (!strcmp(name, "DestroyShaderModule"))
        return (PFN_vkVoidFunction) vkDestroyShaderModule;
    if (!strcmp(name, "CreatePipelineCache"))
        return (PFN_vkVoidFunction) vkCreatePipelineCache;
    if (!strcmp(name, "DestroyPipelineCache"))
        return (PFN_vkVoidFunction) vkDestroyPipelineCache;
    if (!strcmp(name, "GetPipelineCacheData"))
        return (PFN_vkVoidFunction) vkGetPipelineCacheData;
    if (!strcmp(name, "MergePipelineCaches"))
        return (PFN_vkVoidFunction) vkMergePipelineCaches;
    if (!strcmp(name, "CreateGraphicsPipelines"))
        return (PFN_vkVoidFunction) vkCreateGraphicsPipelines;
    if (!strcmp(name, "CreateComputePipelines"))
        return (PFN_vkVoidFunction) vkCreateComputePipelines;
    if (!strcmp(name, "DestroyPipeline"))
        return (PFN_vkVoidFunction) vkDestroyPipeline;
    if (!strcmp(name, "CreatePipelineLayout"))
        return (PFN_vkVoidFunction) vkCreatePipelineLayout;
    if (!strcmp(name, "DestroyPipelineLayout"))
        return (PFN_vkVoidFunction) vkDestroyPipelineLayout;
    if (!strcmp(name, "CreateSampler"))
        return (PFN_vkVoidFunction) vkCreateSampler;
    if (!strcmp(name, "DestroySampler"))
        return (PFN_vkVoidFunction) vkDestroySampler;
    if (!strcmp(name, "CreateDescriptorSetLayout"))
        return (PFN_vkVoidFunction) vkCreateDescriptorSetLayout;
    if (!strcmp(name, "DestroyDescriptorSetLayout"))
        return (PFN_vkVoidFunction) vkDestroyDescriptorSetLayout;
    if (!strcmp(name, "CreateDescriptorPool"))
        return (PFN_vkVoidFunction) vkCreateDescriptorPool;
    if (!strcmp(name, "DestroyDescriptorPool"))
        return (PFN_vkVoidFunction) vkDestroyDescriptorPool;
    if (!strcmp(name, "ResetDescriptorPool"))
        return (PFN_vkVoidFunction) vkResetDescriptorPool;
    if (!strcmp(name, "AllocateDescriptorSets"))
        return (PFN_vkVoidFunction) vkAllocateDescriptorSets;
    if (!strcmp(name, "FreeDescriptorSets"))
        return (PFN_vkVoidFunction) vkFreeDescriptorSets;
    if (!strcmp(name, "UpdateDescriptorSets"))
        return (PFN_vkVoidFunction) vkUpdateDescriptorSets;
    if (!strcmp(name, "CreateFramebuffer"))
        return (PFN_vkVoidFunction) vkCreateFramebuffer;
    if (!strcmp(name, "DestroyFramebuffer"))
        return (PFN_vkVoidFunction) vkDestroyFramebuffer;
    if (!strcmp(name, "CreateRenderPass"))
        return (PFN_vkVoidFunction) vkCreateRenderPass;
    if (!strcmp(name, "DestroyRenderPass"))
        return (PFN_vkVoidFunction) vkDestroyRenderPass;
    if (!strcmp(name, "GetRenderAreaGranularity"))
        return (PFN_vkVoidFunction) vkGetRenderAreaGranularity;
    if (!strcmp(name, "CreateCommandPool"))
        return (PFN_vkVoidFunction) vkCreateCommandPool;
    if (!strcmp(name, "DestroyCommandPool"))
        return (PFN_vkVoidFunction) vkDestroyCommandPool;
    if (!strcmp(name, "ResetCommandPool"))
        return (PFN_vkVoidFunction) vkResetCommandPool;
    if (!strcmp(name, "AllocateCommandBuffers"))
        return (PFN_vkVoidFunction) vkAllocateCommandBuffers;
    if (!strcmp(name, "FreeCommandBuffers"))
        return (PFN_vkVoidFunction) vkFreeCommandBuffers;
    if (!strcmp(name, "BeginCommandBuffer"))
        return (PFN_vkVoidFunction) vkBeginCommandBuffer;
    if (!strcmp(name, "EndCommandBuffer"))
        return (PFN_vkVoidFunction) vkEndCommandBuffer;
    if (!strcmp(name, "ResetCommandBuffer"))
        return (PFN_vkVoidFunction) vkResetCommandBuffer;
    if (!strcmp(name, "CmdBindPipeline"))
        return (PFN_vkVoidFunction) vkCmdBindPipeline;
    if (!strcmp(name, "CmdSetViewport"))
        return (PFN_vkVoidFunction) vkCmdSetViewport;
    if (!strcmp(name, "CmdSetScissor"))
        return (PFN_vkVoidFunction) vkCmdSetScissor;
    if (!strcmp(name, "CmdSetLineWidth"))
        return (PFN_vkVoidFunction) vkCmdSetLineWidth;
    if (!strcmp(name, "CmdSetDepthBias"))
        return (PFN_vkVoidFunction) vkCmdSetDepthBias;
    if (!strcmp(name, "CmdSetBlendConstants"))
        return (PFN_vkVoidFunction) vkCmdSetBlendConstants;
    if (!strcmp(name, "CmdSetDepthBounds"))
        return (PFN_vkVoidFunction) vkCmdSetDepthBounds;
    if (!strcmp(name, "CmdSetStencilCompareMask"))
        return (PFN_vkVoidFunction) vkCmdSetStencilCompareMask;
    if (!strcmp(name, "CmdSetStencilWriteMask"))
        return (PFN_vkVoidFunction) vkCmdSetStencilWriteMask;
    if (!strcmp(name, "CmdSetStencilReference"))
        return (PFN_vkVoidFunction) vkCmdSetStencilReference;
    if (!strcmp(name, "CmdBindDescriptorSets"))
        return (PFN_vkVoidFunction) vkCmdBindDescriptorSets;
    if (!strcmp(name, "CmdBindIndexBuffer"))
        return (PFN_vkVoidFunction) vkCmdBindIndexBuffer;
    if (!strcmp(name, "CmdBindVertexBuffers"))
        return (PFN_vkVoidFunction) vkCmdBindVertexBuffers;
    if (!strcmp(name, "CmdDraw"))
        return (PFN_vkVoidFunction) vkCmdDraw;
    if (!strcmp(name, "CmdDrawIndexed"))
        return (PFN_vkVoidFunction) vkCmdDrawIndexed;
    if (!strcmp(name, "CmdDrawIndirect"))
        return (PFN_vkVoidFunction) vkCmdDrawIndirect;
    if (!strcmp(name, "CmdDrawIndexedIndirect"))
        return (PFN_vkVoidFunction) vkCmdDrawIndexedIndirect;
    if (!strcmp(name, "CmdDispatch"))
        return (PFN_vkVoidFunction) vkCmdDispatch;
    if (!strcmp(name, "CmdDispatchIndirect"))
        return (PFN_vkVoidFunction) vkCmdDispatchIndirect;
    if (!strcmp(name, "CmdCopyBuffer"))
        return (PFN_vkVoidFunction) vkCmdCopyBuffer;
    if (!strcmp(name, "CmdCopyImage"))
        return (PFN_vkVoidFunction) vkCmdCopyImage;
    if (!strcmp(name, "CmdBlitImage"))
        return (PFN_vkVoidFunction) vkCmdBlitImage;
    if (!strcmp(name, "CmdCopyBufferToImage"))
        return (PFN_vkVoidFunction) vkCmdCopyBufferToImage;
    if (!strcmp(name, "CmdCopyImageToBuffer"))
        return (PFN_vkVoidFunction) vkCmdCopyImageToBuffer;
    if (!strcmp(name, "CmdUpdateBuffer"))
        return (PFN_vkVoidFunction) vkCmdUpdateBuffer;
    if (!strcmp(name, "CmdFillBuffer"))
        return (PFN_vkVoidFunction) vkCmdFillBuffer;
    if (!strcmp(name, "CmdClearColorImage"))
        return (PFN_vkVoidFunction) vkCmdClearColorImage;
    if (!strcmp(name, "CmdClearDepthStencilImage"))
        return (PFN_vkVoidFunction) vkCmdClearDepthStencilImage;
    if (!strcmp(name, "CmdClearAttachments"))
        return (PFN_vkVoidFunction) vkCmdClearAttachments;
    if (!strcmp(name, "CmdResolveImage"))
        return (PFN_vkVoidFunction) vkCmdResolveImage;
    if (!strcmp(name, "CmdSetEvent"))
        return (PFN_vkVoidFunction) vkCmdSetEvent;
    if (!strcmp(name, "CmdResetEvent"))
        return (PFN_vkVoidFunction) vkCmdResetEvent;
    if (!strcmp(name, "CmdWaitEvents"))
        return (PFN_vkVoidFunction) vkCmdWaitEvents;
    if (!strcmp(name, "CmdPipelineBarrier"))
        return (PFN_vkVoidFunction) vkCmdPipelineBarrier;
    if (!strcmp(name, "CmdBeginQuery"))
        return (PFN_vkVoidFunction) vkCmdBeginQuery;
    if (!strcmp(name, "CmdEndQuery"))
        return (PFN_vkVoidFunction) vkCmdEndQuery;
    if (!strcmp(name, "CmdResetQueryPool"))
        return (PFN_vkVoidFunction) vkCmdResetQueryPool;
    if (!strcmp(name, "CmdWriteTimestamp"))
        return (PFN_vkVoidFunction) vkCmdWriteTimestamp;
    if (!strcmp(name, "CmdCopyQueryPoolResults"))
        return (PFN_vkVoidFunction) vkCmdCopyQueryPoolResults;
    if (!strcmp(name, "CmdPushConstants"))
        return (PFN_vkVoidFunction) vkCmdPushConstants;
    if (!strcmp(name, "CmdBeginRenderPass"))
        return (PFN_vkVoidFunction) vkCmdBeginRenderPass;
    if (!strcmp(name, "CmdNextSubpass"))
        return (PFN_vkVoidFunction) vkCmdNextSubpass;
    if (!strcmp(name, "CmdEndRenderPass"))
        return (PFN_vkVoidFunction) vkCmdEndRenderPass;
    if (!strcmp(name, "CmdExecuteCommands"))
        return (PFN_vkVoidFunction) vkCmdExecuteCommands;

    return NULL;
}
static inline PFN_vkVoidFunction layer_intercept_instance_proc(const char *name)
{
    if (!name || name[0] != 'v' || name[1] != 'k')
        return NULL;

    name += 2;
    if (!strcmp(name, "DestroyInstance"))
        return (PFN_vkVoidFunction) vkDestroyInstance;
    if (!strcmp(name, "EnumeratePhysicalDevices"))
        return (PFN_vkVoidFunction) vkEnumeratePhysicalDevices;
    if (!strcmp(name, "GetPhysicalDeviceFeatures"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceFeatures;
    if (!strcmp(name, "GetPhysicalDeviceFormatProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceFormatProperties;
    if (!strcmp(name, "GetPhysicalDeviceImageFormatProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceImageFormatProperties;
    if (!strcmp(name, "GetPhysicalDeviceProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceProperties;
    if (!strcmp(name, "GetPhysicalDeviceQueueFamilyProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceQueueFamilyProperties;
    if (!strcmp(name, "GetPhysicalDeviceMemoryProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceMemoryProperties;
    if (!strcmp(name, "EnumerateInstanceExtensionProperties"))
        return (PFN_vkVoidFunction) vkEnumerateInstanceExtensionProperties;
    if (!strcmp(name, "EnumerateInstanceLayerProperties"))
        return (PFN_vkVoidFunction) vkEnumerateInstanceLayerProperties;
    if (!strcmp(name, "EnumerateDeviceLayerProperties"))
        return (PFN_vkVoidFunction) vkEnumerateDeviceLayerProperties;
    if (!strcmp(name, "GetPhysicalDeviceSparseImageFormatProperties"))
        return (PFN_vkVoidFunction) vkGetPhysicalDeviceSparseImageFormatProperties;

    return NULL;
}

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #543
VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* funcName)
{
    PFN_vkVoidFunction addr;


    loader_platform_thread_once(&initOnce, initapi_dump);

    if (!strcmp("vkGetDeviceProcAddr", funcName)) {
        return (PFN_vkVoidFunction) vkGetDeviceProcAddr;
    }

    addr = layer_intercept_proc(funcName);
    if (addr)
        return addr;
    if (device == VK_NULL_HANDLE) {
        return NULL;
    }


    VkLayerDispatchTable *pDisp =  device_dispatch_table(device);
    if (deviceExtMap.size() != 0 && deviceExtMap[pDisp].wsi_enabled)
    {
        if (!strcmp("vkCreateSwapchainKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkCreateSwapchainKHR);
        if (!strcmp("vkDestroySwapchainKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkDestroySwapchainKHR);
        if (!strcmp("vkGetSwapchainImagesKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkGetSwapchainImagesKHR);
        if (!strcmp("vkAcquireNextImageKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkAcquireNextImageKHR);
        if (!strcmp("vkQueuePresentKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkQueuePresentKHR);
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #581
    {
        if (pDisp->GetDeviceProcAddr == NULL)
            return NULL;
        return pDisp->GetDeviceProcAddr(device, funcName);
    }
}

// CODEGEN : file /home/lenny/Documents/VulkanTools/vk-vtlayer-generate.py line #588
VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* funcName)
{
    PFN_vkVoidFunction addr;
    if (!strcmp(funcName, "vkGetInstanceProcAddr"))
        return (PFN_vkVoidFunction) vkGetInstanceProcAddr;
    if (!strcmp(funcName, "vkCreateInstance"))
        return (PFN_vkVoidFunction) vkCreateInstance;
    if (!strcmp(funcName, "vkCreateDevice"))
        return (PFN_vkVoidFunction) vkCreateDevice;

    loader_platform_thread_once(&initOnce, initapi_dump);

    addr = layer_intercept_instance_proc(funcName);
    if (addr)
        return addr;
    if (instance == VK_NULL_HANDLE) {
        return NULL;
    }


    VkLayerInstanceDispatchTable* pTable = instance_dispatch_table(instance);

    if (instanceExtMap.size() != 0 && instanceExtMap[pTable].wsi_enabled)
    {
        if (!strcmp("vkDestroySurfaceKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkDestroySurfaceKHR);
        if (!strcmp("vkGetPhysicalDeviceSurfaceSupportKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfaceSupportKHR);
        if (!strcmp("vkGetPhysicalDeviceSurfaceCapabilitiesKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
        if (!strcmp("vkGetPhysicalDeviceSurfaceFormatsKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfaceFormatsKHR);
        if (!strcmp("vkGetPhysicalDeviceSurfacePresentModesKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfacePresentModesKHR);
#ifdef VK_USE_PLATFORM_XLIB_KHR
        if (!strcmp("vkCreateXlibSurfaceKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkCreateXlibSurfaceKHR);
#endif  // VK_USE_PLATFORM_XLIB_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
        if (!strcmp("vkGetPhysicalDeviceXlibPresentationSupportKHR", funcName))
            return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceXlibPresentationSupportKHR);
#endif  // VK_USE_PLATFORM_XLIB_KHR
    }

    if (pTable->GetInstanceProcAddr == NULL)
        return NULL;
    return pTable->GetInstanceProcAddr(instance, funcName);
}

