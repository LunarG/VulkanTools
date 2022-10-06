/*
 * Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <vulkan/vulkan.h>
#include <vulkan/vk_layer.h>

#include <assert.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

static const char* jsonGenLayerName = "VK_LAYER_KHRONOS_json_gen";
std::string SHADERS_PATH = "";

static VkInstance       globalInstance;
static VkRenderPass     globalRenderPass;
static VkPipelineLayout globalGraphicsPipelineLayout;
static VkPipelineLayout globalComputePipelineLayout;
static VkPipeline       globalGraphicsPipeline;
static VkPipeline       globalComputePipeline;
static VkDevice         globalDev = VK_NULL_HANDLE;
static VkQueue          globalQueue = VK_NULL_HANDLE;
static std::vector<VkPhysicalDevice>    globalPhysdevs;
static VkPhysicalDeviceProperties       globalDeviceProperties;
static VkPhysicalDeviceMemoryProperties globalDeviceMemoryProperties;
static VkPipelineShaderStageCreateInfo  globalShaderStageInfo[2] = { { }, { } };
static VkFormat globalColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
static int      globalWidth  = 1920;
static int      globalHeight = 1080;
int globalUUIDGraphicsPipeline[] = {
    74,
    141,
    64,
    185,
    174,
    95,
    22,
    195,
    64,
    240,
    45,
    12,
    137,
    74,
    92,
    40
};
int globalUUIDComputePipeline[] = {
    0,
    92,
    99,
    197,
    34,
    32,
    36,
    136,
    145,
    57,
    35,
    207,
    203,
    7,
    104,
    136
};

#define LOG(fn) if (result == VK_SUCCESS) std::cout << fn << " OK" << std::endl;      \
                else                      std::cout << fn << " ERROR!!" << std::endl; \
                assert(result == VK_SUCCESS);

VkResult checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (uint32_t i = 0; i < layerCount; i++) {
        if (!strncmp(availableLayers[i].layerName, jsonGenLayerName, strlen(jsonGenLayerName))) {
            return VK_SUCCESS;
        }
    }

    return VK_ERROR_INITIALIZATION_FAILED;
}

VkResult createInstance()
{
    VkResult result = VK_SUCCESS;

    result = checkValidationLayerSupport();
    LOG("checkValidationLayerSupport");

    //Create a Vulkan instance
    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo         = NULL;
    instanceCreateInfo.enabledExtensionCount    = 0;
    instanceCreateInfo.ppEnabledExtensionNames  = NULL;

    VkLayerInstanceLink layerLink = {};
    layerLink.pNext = nullptr;
    layerLink.pfnNextGetInstanceProcAddr = nullptr;
    layerLink.pfnNextGetPhysicalDeviceProcAddr = nullptr;

    // enable json_gen layer for instance
    VkLayerInstanceCreateInfo layerInfo = {};
    layerInfo.sType = VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO;
    layerInfo.function = VK_LAYER_LINK_INFO;
    layerInfo.u.pLayerInfo = &layerLink;

    instanceCreateInfo.pNext = &layerInfo;
    instanceCreateInfo.enabledLayerCount = 1;
    instanceCreateInfo.ppEnabledLayerNames = &jsonGenLayerName;

    result = vkCreateInstance(&instanceCreateInfo, NULL, &globalInstance);
    LOG("vkCreateInstance");

    return result;
}

VkResult selectPhysicalDevice()
{
    VkResult result = VK_SUCCESS;

    uint32_t physdevCount = 0;
    result = vkEnumeratePhysicalDevices(globalInstance, &physdevCount, nullptr);
    assert(result == VK_SUCCESS);
    assert(physdevCount > 0);

    globalPhysdevs.resize(physdevCount);
    result = vkEnumeratePhysicalDevices(globalInstance, &physdevCount, globalPhysdevs.data());
    assert(result == VK_SUCCESS);

    vkGetPhysicalDeviceProperties(globalPhysdevs[0], &globalDeviceProperties);
    
    return result;
}

VkResult createLogicalDevice()
{
    VkResult result = VK_SUCCESS;

    float priority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo = {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        NULL,
        0, // flags
        0, // queueFamilyIndex
        1, // queueCount
        &priority // pQueuePriorities
    };

    VkDeviceCreateInfo deviceInfo = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        NULL,
        0,
        1,
        &queueInfo,
        0, NULL,
        0, nullptr,
        NULL
    };

    // enable json_gen layer for device
    VkLayerDeviceLink layerDevInfo = {};

    VkLayerDeviceCreateInfo devLayerCreateInfo = {};
    devLayerCreateInfo.sType =  VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO;
    devLayerCreateInfo.pNext = &layerDevInfo;
    devLayerCreateInfo.function = VK_LAYER_LINK_INFO;
    devLayerCreateInfo.u.pLayerInfo = &layerDevInfo;

    deviceInfo.pNext = &devLayerCreateInfo;
    deviceInfo.enabledLayerCount = 1;
    deviceInfo.ppEnabledLayerNames = &jsonGenLayerName;

    result = vkCreateDevice(globalPhysdevs[0], &deviceInfo, NULL, &globalDev);
    LOG("vkCreateDevice");

    vkGetPhysicalDeviceMemoryProperties(globalPhysdevs[0], &globalDeviceMemoryProperties);
    vkGetDeviceQueue(globalDev, 0, 0, &globalQueue);

    return result;
}

static VkShaderModule loadSPIRVShader(std::string filename)
{
    size_t shaderSize;
    char* shaderCode = NULL;

    std::ifstream file(filename, std::ios::binary | std::ios::in | std::ios::ate);

    if (file.is_open()) {
        shaderSize = file.tellg();
        file.seekg(0, std::ios::beg);
        // Copy file contents into a buffer
        shaderCode = new char[shaderSize];
        file.read(shaderCode, shaderSize);
        file.close();
        assert(shaderSize > 0);
    }

    if (shaderCode) {
        // Create a new shader module that will be used for pipeline creation
        VkShaderModuleCreateInfo moduleCreateInfo{};
        moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleCreateInfo.codeSize = shaderSize;
        moduleCreateInfo.pCode = (uint32_t*)shaderCode;

        VkShaderModule shaderModule;
        VkResult result = (vkCreateShaderModule(globalDev, &moduleCreateInfo, NULL, &shaderModule));
        assert(result == VK_SUCCESS);

        delete[] shaderCode;
        return shaderModule;
    } else {
        std::cerr << "Error: Could not open shader file \"" << filename << "\"" << std::endl;
        return VK_NULL_HANDLE;
    }
}

void createGraphicsShaderStageInfo()
{
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.pName = "main";
    vertShaderStageInfo.module = loadSPIRVShader(SHADERS_PATH + "\\vert.spv");

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.pName = "main";
    fragShaderStageInfo.module = loadSPIRVShader(SHADERS_PATH + "\\frag.spv");

    globalShaderStageInfo[0] = vertShaderStageInfo;
    globalShaderStageInfo[1] = fragShaderStageInfo;
}

VkResult createRenderPass()
{
    VkResult result = VK_SUCCESS;

    VkAttachmentDescription colorAttachments[1] = {};

    // Color attachment
    colorAttachments[0].format           = globalColorFormat;
    colorAttachments[0].samples          = VK_SAMPLE_COUNT_1_BIT;
    colorAttachments[0].loadOp           = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachments[0].storeOp          = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachments[0].stencilLoadOp    = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachments[0].stencilStoreOp   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachments[0].initialLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachments[0].finalLayout      = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment       = 0;
    colorAttachmentRef.layout           = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags                   = 0;
    subpass.inputAttachmentCount    = 0;
    subpass.pInputAttachments       = NULL;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &colorAttachmentRef;
    subpass.pResolveAttachments     = NULL;
    subpass.pDepthStencilAttachment = NULL;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments    = NULL;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext            = NULL;
    renderPassInfo.attachmentCount  = 1;
    renderPassInfo.pAttachments     = colorAttachments;
    renderPassInfo.subpassCount     = 1;
    renderPassInfo.pSubpasses       = &subpass;
    renderPassInfo.dependencyCount  = 0;
    renderPassInfo.pDependencies    = NULL;

    result = vkCreateRenderPass(globalDev, &renderPassInfo, NULL, &globalRenderPass);
    LOG("vkCreateRenderPass")

    return result;
}

VkResult createGraphicsPipeline()
{
    VkResult result;

    VkPipelineLayoutCreateInfo layoutCreateInfo = {};
    layoutCreateInfo.sType                     = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    result = vkCreatePipelineLayout(globalDev, &layoutCreateInfo, nullptr, &globalGraphicsPipelineLayout);
    LOG("vkCreatePipelineLayout - Graphics Pipeline");

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType                   = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.renderPass              = globalRenderPass;
    pipelineCreateInfo.layout                  = globalGraphicsPipelineLayout;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    inputAssemblyState.sType                   = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.topology                = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineCreateInfo.pInputAssemblyState     = &inputAssemblyState;

    // Viewport state
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount                = 1;
    viewportState.scissorCount                 = 1;
    pipelineCreateInfo.pViewportState          = &viewportState;

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    std::vector<VkDynamicState> dynamicStateEnables;
    dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
    dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);
    dynamicState.sType                         = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pDynamicStates                = dynamicStateEnables.data();
    dynamicState.dynamicStateCount             = static_cast<uint32_t>(dynamicStateEnables.size());
    pipelineCreateInfo.pDynamicState           = &dynamicState;


    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    multisampleState.sType                     = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.pSampleMask               = NULL;
    multisampleState.rasterizationSamples      = VK_SAMPLE_COUNT_1_BIT;
    pipelineCreateInfo.pMultisampleState       = &multisampleState;

    // Rasterization state
    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    rasterizationState.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.polygonMode             = VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode                = VK_CULL_MODE_NONE;
    rasterizationState.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationState.depthClampEnable        = VK_FALSE;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.depthBiasEnable         = VK_FALSE;
    rasterizationState.lineWidth               = 1.0f;
    pipelineCreateInfo.pRasterizationState     = &rasterizationState;

    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    VkPipelineColorBlendAttachmentState blendAttachmentState[1] = {};
    blendAttachmentState[0].colorWriteMask     = 0xf;
    blendAttachmentState[0].blendEnable        = VK_FALSE;
    colorBlendState.attachmentCount            = 1;
    colorBlendState.pAttachments               = blendAttachmentState;
    pipelineCreateInfo.pColorBlendState        = &colorBlendState;

    pipelineCreateInfo.stageCount              = 2;
    pipelineCreateInfo.pStages                 = globalShaderStageInfo;

    pipelineCreateInfo.pNext = nullptr;
    result = vkCreateGraphicsPipelines(globalDev, VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, &globalGraphicsPipeline);
    LOG("vkCreateGraphicsPipelines");

    return result;
}

VkResult createComputePipeline()
{
    VkResult result;
    
    VkPipelineLayoutCreateInfo layoutCreateInfo = {};
    layoutCreateInfo.sType                     = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    result = vkCreatePipelineLayout(globalDev, &layoutCreateInfo, nullptr, &globalComputePipelineLayout);
    LOG("vkCreatePipelineLayout - Compute Pipeline");

    VkComputePipelineCreateInfo computePipelineCreateInfo = {};
    computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    computePipelineCreateInfo.layout = globalComputePipelineLayout;

    VkPipelineShaderStageCreateInfo shaderStage = {};
    shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStage.module = loadSPIRVShader(SHADERS_PATH + "\\headless.comp.spv");
    shaderStage.pName = "main";

    computePipelineCreateInfo.stage = shaderStage;

    result = vkCreateComputePipelines(globalDev, VK_NULL_HANDLE, 1, &computePipelineCreateInfo, NULL, &globalComputePipeline);
    LOG("vkCreateComputePipelines");

    return result;
}

bool compareUUID(uint8_t* uuidGraphics, uint8_t* uuidCompute)
{
    for (int i = 0; i < VK_UUID_SIZE; i++) {
        if (static_cast<int>(uuidGraphics[i]) != globalUUIDGraphicsPipeline[i]) {
            std::cerr << "Obtained Graphics Pipeline UUID:" << std::endl;
            for (int i = 0; i < VK_UUID_SIZE; i++) {
                std::cout << static_cast<int>(uuidGraphics[i]) << std::endl;
            }
            std::cerr << "Expected Graphics Pipeline UUID:" << std::endl;
            for (int i = 0; i < VK_UUID_SIZE; i++) {
                std::cout << static_cast<int>(globalUUIDGraphicsPipeline[i]) << std::endl;
            }
            std::cerr << "Error: Obtained UUID does not match with the expected globalUUIDGraphicsPipeline" << std::endl;
            return false;
        }
    }

    for (int i = 0; i < VK_UUID_SIZE; i++) {
        if (static_cast<int>(uuidCompute[i]) != globalUUIDComputePipeline[i]) {
            std::cerr << "Obtained Compute Pipeline UUID:" << std::endl;
            for (int i = 0; i < VK_UUID_SIZE; i++) {
                std::cout << static_cast<int>(uuidCompute[i]) << std::endl;
            }
            std::cerr << "Expected Compute Pipeline UUID:" << std::endl;
            for (int i = 0; i < VK_UUID_SIZE; i++) {
                std::cout << static_cast<int>(globalUUIDComputePipeline[i]) << std::endl;
            }
            std::cerr << "Error: Obtained UUID does not match with the expected globalUUIDComputePipeline" << std::endl;
            return false;
        }
    }

    return true;
}

bool checkPipelinePropertiesEXT()
{
    PFN_vkGetPipelinePropertiesEXT fpGetPipelinePropertiesEXT = reinterpret_cast<PFN_vkGetPipelinePropertiesEXT>(vkGetDeviceProcAddr(globalDev, "vkGetPipelinePropertiesEXT"));

    VkPipelineInfoEXT pInfoGraphics{VK_STRUCTURE_TYPE_PIPELINE_INFO_EXT};
    pInfoGraphics.pipeline = globalGraphicsPipeline;
    VkPipelinePropertiesIdentifierEXT pIdentGraphics{VK_STRUCTURE_TYPE_PIPELINE_PROPERTIES_IDENTIFIER_EXT};
    VkBaseOutStructure *pOutGraphics = reinterpret_cast<VkBaseOutStructure *>(&pIdentGraphics);
    fpGetPipelinePropertiesEXT(globalDev, &pInfoGraphics, pOutGraphics);
    uint8_t *uuidGraphics = pIdentGraphics.pipelineIdentifier;

    VkPipelineInfoEXT pInfoCompute{VK_STRUCTURE_TYPE_PIPELINE_INFO_EXT};
    pInfoCompute.pipeline = globalComputePipeline;
    VkPipelinePropertiesIdentifierEXT pIdentCompute{VK_STRUCTURE_TYPE_PIPELINE_PROPERTIES_IDENTIFIER_EXT};
    VkBaseOutStructure *pOutCompute = reinterpret_cast<VkBaseOutStructure *>(&pIdentCompute);
    fpGetPipelinePropertiesEXT(globalDev, &pInfoCompute, pOutCompute);
    uint8_t *uuidCompute = pIdentCompute.pipelineIdentifier;

    if (!compareUUID(uuidGraphics, uuidCompute)) {
        std::cerr << "Error: vkGetPipelinePropertiesEXT failed to produce correct UUID" << std::endl;
        return false;
    }

    std::cout<<"vkGetPipelinePropertiesEXT OK"<<std::endl;
    return true;
}

static VkResult cleanup()
{
    vkDestroyRenderPass(globalDev, globalRenderPass, nullptr);
    vkDestroyPipelineLayout(globalDev, globalGraphicsPipelineLayout, nullptr);
    vkDestroyPipelineLayout(globalDev, globalComputePipelineLayout, nullptr);
    vkDestroyPipeline(globalDev, globalGraphicsPipeline, nullptr);
    vkDestroyPipeline(globalDev, globalComputePipeline, nullptr);
    vkDestroyShaderModule(globalDev, globalShaderStageInfo[0].module, nullptr);
    vkDestroyShaderModule(globalDev, globalShaderStageInfo[1].module, nullptr);
    vkDestroyDevice(globalDev, NULL);
    vkDestroyInstance(globalInstance, NULL);

    return VK_SUCCESS;
}

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <option(s)> SOURCES"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-j,--jsonSpv\t\tSpecify the path to the JSON files and SPIRV Shaders directory"
              << std::endl;
}

static bool parseCommandLineArgs(int argc, char** argv)
{
    if (argc < 3) {
        show_usage(argv[0]);
        return false;
    }
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 0;
        } else if ((arg == "-s") || (arg == "--shaders")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                SHADERS_PATH = argv[i+1]; // Increment 'i' so we don't get the argument as the next argv[i].
                i++;
            } else { // There was no argument to the destination option.
                std::cerr << "--shaders option requires one argument." << std::endl;
                return false;
            }  
        } else {
            show_usage(argv[0]);
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv)
{
    VkResult result = VK_SUCCESS;

    bool status = parseCommandLineArgs(argc, argv);
    if (!status)
        return 0;

    result = createInstance();

    result = selectPhysicalDevice();

    result = createLogicalDevice();

    createGraphicsShaderStageInfo();

    result = createRenderPass();

    result = createGraphicsPipeline();

    result = createComputePipeline();

    assert(checkPipelinePropertiesEXT() == true);

    result = cleanup();
    assert(result == VK_SUCCESS);

    return 0;
}