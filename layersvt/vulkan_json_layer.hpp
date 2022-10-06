/*
 * Copyright (c) 2021 The Khronos Group Inc.
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

#pragma once

#include <algorithm>
#include "vk_layer_table.h"
#include "openssl/md5.h"
namespace vk_json 
{

struct ObjectResCreateInfo {
    uint32_t pipelineCacheCreateInfoCount;
    const VkPipelineCacheCreateInfo* pPipelineCacheCreateInfos;
    uint32_t pipelinePoolSizeCount;
    //const VkPipelinePoolSize* pPipelinePoolSizes;
    uint32_t semaphoreRequestCount;
    uint32_t commandBufferRequestCount;
    uint32_t fenceRequestCount;
    uint32_t deviceMemoryRequestCount;
    uint32_t bufferRequestCount;
    uint32_t imageRequestCount;
    uint32_t eventRequestCount;
    uint32_t queryPoolRequestCount;
    uint32_t bufferViewRequestCount;
    uint32_t imageViewRequestCount;
    uint32_t layeredImageViewRequestCount;
    uint32_t pipelineCacheRequestCount;
    uint32_t pipelineLayoutRequestCount;
    uint32_t renderPassRequestCount;
    uint32_t graphicsPipelineRequestCount;
    uint32_t computePipelineRequestCount;
    uint32_t descriptorSetLayoutRequestCount;
    uint32_t samplerRequestCount;
    uint32_t descriptorPoolRequestCount;
    uint32_t descriptorSetRequestCount;
    uint32_t framebufferRequestCount;
    uint32_t commandPoolRequestCount;
    uint32_t samplerYcbcrConversionRequestCount;
    uint32_t surfaceRequestCount;
    uint32_t swapchainRequestCount;
    uint32_t displayModeRequestCount;
    uint32_t subpassDescriptionRequestCount;
    uint32_t attachmentDescriptionRequestCount;
    uint32_t descriptorSetLayoutBindingRequestCount;
    uint32_t descriptorSetLayoutBindingLimit;
    uint32_t maxImageViewMipLevels;
    uint32_t maxImageViewArrayLayers;
    uint32_t maxLayeredImageViewMipLevels;
    uint32_t maxOcclusionQueriesPerPool;
    uint32_t maxPipelineStatisticsQueriesPerPool;
    uint32_t maxTimestampQueriesPerPool;
    uint32_t maxImmutableSamplersPerDescriptorSetLayout;
};

class PipelineData {
   public:
    static constexpr int INDENT_RESET         = 0;
    static constexpr int INDENT_RENDERPASS    = 8;
    static constexpr int INDENT_DESCRIPTORSET = 16;
    static constexpr int INDENT_PIPELINE_LAYOUT = 8;
    bool m_usePrivateDataExtension{false};

    ObjectResCreateInfo objResInfo {};

    PipelineData() { m_pipelineCount = 0; }

    ~PipelineData() {
        for (int i = 0; i < m_jsonStringsList.size(); i++)
        {
            delete(m_jsonStringsList[i]);
        }
        for (int i = 0; i < m_uuidAllocList.size(); i++)
        {
            delete [] m_uuidAllocList[i];
        }
    }

    VkResult createAndSetPrivateData(VkDevice &device, VkObjectType objectType, uint64_t objectHandle, std::string *data)
    {
        VkResult result = VK_SUCCESS;
        if (m_privateDataSlot == nullptr) {
            // Populate VkPrivateDataSlotCreateInfo
            VkPrivateDataSlotCreateInfo pPrivateDataSlotCreateInfo;
            pPrivateDataSlotCreateInfo.sType = VK_STRUCTURE_TYPE_PRIVATE_DATA_SLOT_CREATE_INFO;
            pPrivateDataSlotCreateInfo.pNext = NULL;
            pPrivateDataSlotCreateInfo.flags = 0;
            if (m_usePrivateDataExtension) {
                result = device_dispatch_table(device)->CreatePrivateDataSlotEXT(device, &pPrivateDataSlotCreateInfo, NULL, &m_privateDataSlot);
            } else {
                result = device_dispatch_table(device)->CreatePrivateDataSlot(device, &pPrivateDataSlotCreateInfo, NULL, &m_privateDataSlot);
            }
            if (result != VK_SUCCESS) {
                return result;
            }
        }

        if (m_usePrivateDataExtension) {
            result = device_dispatch_table(device)->SetPrivateDataEXT(device, objectType, objectHandle, m_privateDataSlot, reinterpret_cast<uint64_t>(data));
        } else {
            result = device_dispatch_table(device)->SetPrivateData(device, objectType, objectHandle, m_privateDataSlot, reinterpret_cast<uint64_t>(data));
        }

        return result;
    }

    std::string *getPrivateData(VkDevice &device, VkObjectType objectType, uint64_t objectHandle)
    {
        uint64_t pData;
        if (m_usePrivateDataExtension) {
            device_dispatch_table(device)->GetPrivateDataEXT(device, objectType, objectHandle, m_privateDataSlot, &pData);
        } else {
            device_dispatch_table(device)->GetPrivateData(device, objectType, objectHandle, m_privateDataSlot, &pData);
        }

        std::string *createInfoString = reinterpret_cast<std::string *>(pData);
        return createInfoString;
    }

    void addLayout(VkDevice &device, VkDescriptorSetLayout l, const VkDescriptorSetLayoutCreateInfo* pCreateInfo)
    {
        clearString();

        INDENT(INDENT_DESCRIPTORSET);
        print_VkDescriptorSetLayoutCreateInfo(pCreateInfo, "DescriptorsetLayout", false);
        INDENT(-INDENT_DESCRIPTORSET);

        std::string *jsonString = new std::string(_string_stream.str());
        m_jsonStringsList.push_back(jsonString);

        /* Private Data Slot */
        VkResult result = createAndSetPrivateData(device, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, reinterpret_cast<uint64_t>(l), jsonString);
        assert(result == VK_SUCCESS);

        if (pCreateInfo->pBindings) {
            for (unsigned int i = 0; i < pCreateInfo->bindingCount; i++) {
                VkDescriptorSetLayoutBinding b = pCreateInfo->pBindings[i];
                for (unsigned int j = 0; j < b.descriptorCount; j++) {
                    if (b.pImmutableSamplers) {
                        std::stringstream ss;
                        ss << i << "_" << j;
                        std::string s(ss.str());
                        VkSampler sampler = (const_cast<VkSampler>(b.pImmutableSamplers[i]));
                        m_ImmutableSamplerBindingMap.insert(std::make_pair(sampler, s));
                    }
                }
            }
        }
    }

    void setRenderPass2(VkDevice &device, const VkRenderPassCreateInfo2* pCreateInfo, VkRenderPass* pRenderPass)
    {
        m_renderPass2Flag = true;
        m_renderPassFlag = false;
        clearString();
        
        INDENT(INDENT_RENDERPASS);
        {
            print_VkRenderPassCreateInfo2(pCreateInfo, "RenderPass2");

            std::string *jsonString = new std::string(_string_stream.str());
            m_jsonStringsList.push_back(jsonString);

            /* Private Data Slot */
            VkResult result = createAndSetPrivateData(device, VK_OBJECT_TYPE_RENDER_PASS, reinterpret_cast<uint64_t>(*pRenderPass), jsonString);
            assert(result == VK_SUCCESS);
        }
        INDENT(-INDENT_RENDERPASS);
    }

    void setRenderPass(VkDevice &device, const VkRenderPassCreateInfo* pCreateInfo, VkRenderPass* pRenderPass)
    {
        m_renderPassFlag = true;
        m_renderPass2Flag = false;
        clearString();
        
        INDENT(INDENT_RENDERPASS);
        {
            print_VkRenderPassCreateInfo(pCreateInfo, "RenderPass");

            std::string *jsonString = new std::string(_string_stream.str());
            m_jsonStringsList.push_back(jsonString);

            /* Private Data Slot */
            VkResult result = createAndSetPrivateData(device, VK_OBJECT_TYPE_RENDER_PASS, reinterpret_cast<uint64_t>(*pRenderPass), jsonString);
            assert(result == VK_SUCCESS);
        }
        INDENT(-INDENT_RENDERPASS);
    }

    void setPhysicalDeviceFeatures2(const VkPhysicalDeviceFeatures2* pFeatures) 
    {
        clearString();

        INDENT(INDENT_PIPELINE_LAYOUT)
        print_VkPhysicalDeviceFeatures2(pFeatures, "PhysicalDeviceFeatures");
        INDENT(-INDENT_PIPELINE_LAYOUT)
        m_PhysicalDeviceFeatures2 = _string_stream.str();
    }

    void setPipelineLayout(VkDevice &device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkPipelineLayout* pPipelineLayout)
    {
        clearString();

        INDENT(INDENT_PIPELINE_LAYOUT)
        print_VkPipelineLayoutCreateInfo(pCreateInfo, "PipelineLayout");
        INDENT(-INDENT_PIPELINE_LAYOUT)

        std::string *jsonString = new std::string(_string_stream.str());
        m_jsonStringsList.push_back(jsonString);

        // Record what all descriptorset layouts are used in this layout.
        DescLayoutVec layouts;
        for (unsigned int i = 0; i < pCreateInfo->setLayoutCount; i++) {
            layouts.push_back(pCreateInfo->pSetLayouts[i]);
        }
        m_pipelineLayoutDescSetMap.insert(std::make_pair(*pPipelineLayout, layouts));

        /* Private Data Slot */
        VkResult result = createAndSetPrivateData(device, VK_OBJECT_TYPE_PIPELINE_LAYOUT, reinterpret_cast<uint64_t>(*pPipelineLayout), jsonString);
        assert(result == VK_SUCCESS);
    }

    void setShaderModuleInfo(const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModule* m) 
    {
        if (pCreateInfo == nullptr || m == nullptr) {
            std::cout << "Error with shader module!" << std::endl;
            exit(-1);
        }
        ShaderInformation s(*m, pCreateInfo->codeSize, pCreateInfo->pCode);
        m_shaderInfoMap.insert(std::make_pair(*m, s));
    }

    void setSamplerInfo(const VkSamplerCreateInfo* pCreateInfo, VkSampler* pSampler)
    {
        if (pCreateInfo == nullptr || pSampler == nullptr) {
            std::cout << "Error with sampler!" << std::endl;
            exit(-1);
        }
        m_samplerInfoMap.insert(std::make_pair(*pSampler, *pCreateInfo));
    }

    void setDevice(const VkDeviceCreateInfo* pCreateInfo)
    {
        for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
            if (strncmp(pCreateInfo->ppEnabledExtensionNames[i], "VK_KHR_swapchain", strlen("VK_KHR_swapchain"))) {
                m_deviceExtensions.push_back(pCreateInfo->ppEnabledExtensionNames[i]);
            }
        }
    }

    VkResult setPipelineUUID(VkDevice &device, VkPipeline pPipeline, uint8_t uuid[VK_UUID_SIZE])
    {
        VkResult result = VK_SUCCESS;
        if (m_privateDataUuidSlot == nullptr) {
            // Populate VkPrivateDataSlotCreateInfo
            VkPrivateDataSlotCreateInfo pPrivateDataSlotCreateInfo;
            pPrivateDataSlotCreateInfo.sType = VK_STRUCTURE_TYPE_PRIVATE_DATA_SLOT_CREATE_INFO;
            pPrivateDataSlotCreateInfo.pNext = NULL;
            pPrivateDataSlotCreateInfo.flags = 0;
            result = device_dispatch_table(device)->CreatePrivateDataSlot(device, &pPrivateDataSlotCreateInfo, NULL, &m_privateDataUuidSlot);
            if (result != VK_SUCCESS) {
                return result;
            }
        }

        uint8_t* uuidMem = new uint8_t[VK_UUID_SIZE];
        for (int i=0; i < VK_UUID_SIZE; i++) {
            uuidMem[i] = uuid[i];
        }
        m_uuidAllocList.push_back(uuidMem);

        result = device_dispatch_table(device)->SetPrivateData(device, VK_OBJECT_TYPE_PIPELINE, reinterpret_cast<uint64_t>(pPipeline),
                                                               m_privateDataUuidSlot, reinterpret_cast<uint64_t>(uuidMem));

        return result;
    }

    void getPipelineUUID(VkDevice &device, VkPipeline pPipeline, uint8_t uuid[VK_UUID_SIZE])
    {
        uint64_t pData;
        device_dispatch_table(device)->GetPrivateData(device, VK_OBJECT_TYPE_PIPELINE, reinterpret_cast<uint64_t>(pPipeline),
                                                      m_privateDataUuidSlot, &pData);

        uint8_t* uuidMem = reinterpret_cast<uint8_t *>(pData);
        for (int i=0; i < VK_UUID_SIZE; i++) {
            uuid[i] = uuidMem[i];
        }
    }

    void createUUID(const std::string& hashString, uint8_t uuid[VK_UUID_SIZE])
    {
        static_assert(MD5_DIGEST_LENGTH == VK_UUID_SIZE, "VK_UUID_SIZE is not equal to required MD5 digest length");
        MD5((unsigned char*) hashString.c_str(), hashString.length(), uuid);
    }

    void dumpUUID(FILE* fp, uint8_t uuid[VK_UUID_SIZE])
    {
        for (uint8_t i = 0; i < VK_UUID_SIZE; i++) {

            for (int j = 0; j < vk_json::s_num_spaces; j++) vk_json::_string_stream << " ";
            vk_json::_string_stream << static_cast<int>(uuid[i]);

            if (i < (VK_UUID_SIZE - 1))
                vk_json::_string_stream << ",\n";
            else
                vk_json::_string_stream << "\n";
        }
    }

    void dumpComputePipeline(VkDevice &device, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, VkPipeline* pPipelines)
    {
        for (uint32_t cnt = 0; cnt < createInfoCount; cnt++)
        {
            FILE* fp = openFile();

            vk_json::_string_stream.str({});
            vk_json::_string_stream.clear();

            // Set Compute pipeline
            INDENT(INDENT_PIPELINE_LAYOUT)
            print_VkComputePipelineCreateInfo(pCreateInfos[cnt], "Compute");
            INDENT(-INDENT_PIPELINE_LAYOUT)
            std::string computePipelineCreateInfoString = _string_stream.str();

            clearString();

            // Start JSON file
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "{" << std::endl;
            vk_json::s_num_spaces += 4;

            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "\"ComputePipelineState\" :" << std::endl;

            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "{" << std::endl;
            vk_json::s_num_spaces += 4;

            // TODO: This is not supported right now, so leaving this empty.
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "\"YcbcrSamplers\" :" << std::endl;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "[" << std::endl;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "]," << std::endl;

            // Gather and dump Immutable Samplers.
            dumpImmutableSamplers();

            // Dump all descriptorset layouts associated with the pipeline layout.
            std::vector<std::string> pLayoutsHashStringList(m_pipelineLayoutDescSetMap[pCreateInfos[cnt].layout].size(), "");
            dumpDescriptorSets(device, pCreateInfos[cnt].layout, pLayoutsHashStringList);

            // Dump pipeline layout
            /* Get Private Data associated with VkPipelineLayout object */
            std::string *pipelineLayoutCreateInfoString = getPrivateData(device, VK_OBJECT_TYPE_PIPELINE_LAYOUT, reinterpret_cast<uint64_t>(pCreateInfos[cnt].layout));

            if (pipelineLayoutCreateInfoString && !pipelineLayoutCreateInfoString->empty()) {
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "\"PipelineLayout\" : " << std::endl;
                vk_json::_string_stream << *pipelineLayoutCreateInfoString;
            } else {
                std::cout << "Error: No pipeline layout information available. Exiting!\n";
                exit(-1);
            }

            // Dump ComputePipeline
            if (!computePipelineCreateInfoString.empty()) {
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "\"ComputePipeline\" : " << std::endl;
                vk_json::_string_stream << computePipelineCreateInfoString;
            } else {
                std::cout << "Error: No compute pipeline information available. Exiting!\n";
                exit(-1);
            }

            // Dump shader file names
            std::stringstream shaderCode = dumpShaders(1, &(pCreateInfos[cnt].stage));

            // Generate unique hash for each pipeline
            std::string hashString = "";
            for (auto& it : pLayoutsHashStringList) {
                hashString += it;
            }

            hashString += *pipelineLayoutCreateInfoString;
            hashString += computePipelineCreateInfoString;
            hashString += m_PhysicalDeviceFeatures2;
            hashString += m_ImmutableSamplers;

            // Use shader code also for hash.
            hashString += shaderCode.str();

            // Dump PhysicalDeviceFeatures2
            if (!m_PhysicalDeviceFeatures2.empty()) {
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "\"PhysicalDeviceFeatures\" :" << std::endl;
                vk_json::_string_stream << m_PhysicalDeviceFeatures2.c_str();
            }

            // close ComputePipelineState
            vk_json::s_num_spaces -= 4;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "}," << std::endl;

            // device extensions
            {
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "\"EnabledExtensions\" : " << std::endl;
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "[" << std::endl;
                vk_json::s_num_spaces += 4;

                for (unsigned int j = 0; j < m_deviceExtensions.size(); j++)
                    vk_json::print_char(m_deviceExtensions[j].data(), "", (j + 1) != m_deviceExtensions.size());

                vk_json::s_num_spaces -= 4;
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "]," << std::endl;
            }

            // create pipeline identifier
            uint8_t pipelineUUID[VK_UUID_SIZE]{};
            createUUID(hashString, pipelineUUID);
            setPipelineUUID(device, pPipelines[cnt], pipelineUUID);

            // Dump Pipeline Identifier
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "\"PipelineUUID\" : " << std::endl;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "[" << std::endl;
            vk_json::s_num_spaces += 4;

            dumpUUID(fp, pipelineUUID);

            vk_json::s_num_spaces -= 4;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "]" << std::endl;

            // End JSON file
            vk_json::s_num_spaces -= 4;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "}" << std::endl;

            fprintf(fp, "%s", vk_json::_string_stream.str().c_str());
            m_pipelineCount++;
        }
    }

    void dumpImmutableSamplers() 
    {
        for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
        vk_json::_string_stream << "\"ImmutableSamplers\" :" << std::endl;
        for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
        vk_json::_string_stream << "[" << std::endl;
        vk_json::s_num_spaces += 4;

        int sz = (int)m_ImmutableSamplerBindingMap.size();
        int count = 0;

        for (const auto& keyVal : m_ImmutableSamplerBindingMap) {
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "{" << std::endl;
            vk_json::s_num_spaces += 4;

            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "\"" << "\"pImmutableSamplers_" << m_ImmutableSamplerBindingMap[keyVal.first] << "\" : " << std::endl;

            VkSamplerCreateInfo s = m_samplerInfoMap[keyVal.first];
            print_VkSamplerCreateInfo(s, "ImmutableSamplers", (count + 1 != sz));

            vk_json::s_num_spaces -= 4;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";

            if (count + 1 != sz)
                vk_json::_string_stream << "}," << std::endl;
            else
                vk_json::_string_stream << "}" << std::endl;

            count++;
        }
        vk_json::s_num_spaces -= 4;
        for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
        vk_json::_string_stream << "]," << std::endl;
    }

    void dumpDescriptorSets(VkDevice &device, VkPipelineLayout layout, std::vector<std::string> &pLayoutsHashStringList)
    {
        int count = 0;
        DescLayoutVec layouts = m_pipelineLayoutDescSetMap[layout];

        for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
        vk_json::_string_stream << "\"DescriptorSetLayouts\" :" << std::endl;
        for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
        vk_json::_string_stream << "[" << std::endl;
        vk_json::s_num_spaces += 4;

        for (auto& it : layouts) {
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "{" << std::endl;
            vk_json::s_num_spaces += 4;

            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "\"" << "pSetLayouts_" << count << "\"" << ":\n";

            std::string *descriptorSetLayoutCreateInfoString = getPrivateData(device, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, reinterpret_cast<uint64_t>(it));
            pLayoutsHashStringList.push_back(*descriptorSetLayoutCreateInfoString);

            if (descriptorSetLayoutCreateInfoString && !descriptorSetLayoutCreateInfoString->empty()) {
                vk_json::_string_stream << *descriptorSetLayoutCreateInfoString;
            } else {
                std::cout << "Error: No descriptorSetLayout information available. Exiting!\n";
                exit(-1);
            }

            vk_json::s_num_spaces -= 4;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            if ((count + 1) < layouts.size())
                vk_json::_string_stream << "}," << std::endl;
            else
                vk_json::_string_stream << "}" << std::endl;
            count++;
        }
        vk_json::s_num_spaces -= 4;
        for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
        vk_json::_string_stream << "]," << std::endl;
    }

    std::stringstream dumpShaders(uint32_t stageCount, const VkPipelineShaderStageCreateInfo* pStages) 
    {
        std::string jsonShaderString = "";
        std::stringstream shaderCode;
        for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
        vk_json::_string_stream << "\"ShaderFileNames\" :" << std::endl;
        for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
        vk_json::_string_stream << "[" << std::endl;
        vk_json::s_num_spaces += 4;

        for (unsigned int i = 0; i < stageCount; i++) {
            std::string shaderPath = getenv("VK_JSON_FILE_PATH") ? getenv("VK_JSON_FILE_PATH") : "";
            std::string shaderName = "";
            std::stringstream s;
            s << "_pipeline_" << m_pipelineCount;

            switch (pStages[i].stage) {
                // Skipping RT and Mesh shaders for now.
                case VK_SHADER_STAGE_VERTEX_BIT:
                    shaderName += m_exeName + s.str() + std::string(".vert.spv");
                    jsonShaderString = "VK_SHADER_STAGE_VERTEX_BIT";
                    break;
                case VK_SHADER_STAGE_FRAGMENT_BIT:
                    shaderName += m_exeName + s.str() + std::string(".frag.spv");
                    jsonShaderString = "VK_SHADER_STAGE_FRAGMENT_BIT";
                    break;
                case VK_SHADER_STAGE_GEOMETRY_BIT:
                    shaderName += m_exeName + s.str() + std::string(".geom.spv");
                    jsonShaderString = "VK_SHADER_STAGE_GEOMETRY_BIT";
                    break;
                case VK_SHADER_STAGE_COMPUTE_BIT:
                    shaderName += m_exeName + s.str() + std::string(".compute.spv");
                    jsonShaderString = "ComputeShader";
                    break;
                case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
                    shaderName += m_exeName + s.str() + std::string(".tcs.spv");
                    jsonShaderString = "VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT";
                    break;
                case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
                    shaderName += m_exeName + s.str() + std::string(".tes.spv");
                    jsonShaderString = "VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT";
                    break;
                default:
                    assert(0);
            }

            FILE* shaderCodeFile = fopen((shaderPath + shaderName).c_str(), "wb");
            char* p = (char*)m_shaderInfoMap[pStages[i].module].m_code;
            for (int k = 0; k < m_shaderInfoMap[pStages[i].module].m_codeSize; k++) {
                shaderCode << *p;
                fprintf(shaderCodeFile, "%c", *p++);
            }

            fflush(shaderCodeFile);
            fclose(shaderCodeFile);

            // Dump shader file name
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "{" << std::endl;
            vk_json::s_num_spaces += 4;

            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "\"stage\" : \"" << VkShaderStageFlagBits_map[pStages[i].stage].c_str()
                                    << "\",\n";

            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "\"filename\" : \"" << shaderName.c_str() << "\"" << std::endl;

            vk_json::s_num_spaces -= 4;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "}" << ((i + 1 == stageCount) ? ("\n") : (",\n"));
        }
        vk_json::s_num_spaces -= 4;
        for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
        vk_json::_string_stream << ((m_PhysicalDeviceFeatures2.empty()) ? ("]") : ("],")) <<std::endl;

        return shaderCode;
    }

    // maintain the same order as CTS does in writeJSON_GraphicsPipeline_vkpccjson
    // so that we can generate uniform JSON output.
    void dumpGraphicsPipeline(VkDevice &device, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, VkPipeline* pPipelines)
    {
        for (uint32_t cnt = 0; cnt < createInfoCount; cnt++)
        {
            FILE* fp = openFile();

            vk_json::_string_stream.str({});
            vk_json::_string_stream.clear();

            // Set Graphics Pipeline
            INDENT(INDENT_PIPELINE_LAYOUT)
            print_VkGraphicsPipelineCreateInfo(pCreateInfos[cnt], "Graphics");
            INDENT(-INDENT_PIPELINE_LAYOUT)
            std::string graphicsPipelineCreateInfoString = _string_stream.str();

            clearString();

            // Start JSON file
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "{" << std::endl;
            vk_json::s_num_spaces += 4;

            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "\"GraphicsPipelineState\" :" << std::endl;

            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "{" << std::endl;
            vk_json::s_num_spaces += 4;

            // Dump RenderPass information.
            std::string *renderPassCreateInfoString = getPrivateData(device, VK_OBJECT_TYPE_RENDER_PASS, reinterpret_cast<uint64_t>(pCreateInfos[cnt].renderPass));
            if (renderPassCreateInfoString && !renderPassCreateInfoString->empty()) {
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                if (m_renderPassFlag) {
                    vk_json::_string_stream << "\"Renderpass\" : " << std::endl;
                }
                else if (m_renderPass2Flag) {
                    vk_json::_string_stream << "\"Renderpass2\" : " << std::endl;
                }
                vk_json::_string_stream << *renderPassCreateInfoString << std::endl;
            } else {
                std::cout << "Error: No renderpass information available. Exiting!\n";
                exit(-1);
            }
        
            // TODO: This is not supported right now, so leaving this empty.
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "\"YcbcrSamplers\" :" << std::endl;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "[" << std::endl;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "]," << std::endl;

            // Gather and dump Immutable Samplers.
            dumpImmutableSamplers();

            // Dump all descriptorset layouts associated with the pipeline layout.
            std::vector<std::string> pLayoutsHashStringList(m_pipelineLayoutDescSetMap[pCreateInfos[cnt].layout].size(), "");
            dumpDescriptorSets(device, pCreateInfos[cnt].layout, pLayoutsHashStringList);

            // Dump pipeline layout
            /* Get Private Data associated with VkPipelineLayout object */
            std::string *pipelineLayoutCreateInfoString = getPrivateData(device, VK_OBJECT_TYPE_PIPELINE_LAYOUT, reinterpret_cast<uint64_t>(pCreateInfos[cnt].layout));
            if (pipelineLayoutCreateInfoString && !pipelineLayoutCreateInfoString->empty()) {
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "\"PipelineLayout\" : " << std::endl;
                vk_json::_string_stream << *pipelineLayoutCreateInfoString;
            } else {
                std::cout << "Error: No pipeline layout information available. Exiting!\n";
                exit(-1);
            }

            // Dump GraphicsPipeline
            if (!graphicsPipelineCreateInfoString.empty()) {
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "\"GraphicsPipeline\" : " << std::endl;
                vk_json::_string_stream << graphicsPipelineCreateInfoString;
            } else {
                std::cout << "Error: No graphics pipeline information available. Exiting!\n";
                exit(-1);
            }

            // Dump shaders
            std::stringstream shaderCode = dumpShaders(pCreateInfos[cnt].stageCount, pCreateInfos[cnt].pStages);

            // Dump PhysicalDeviceFeatures2
            if (!m_PhysicalDeviceFeatures2.empty()) {
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "\"PhysicalDeviceFeatures\" :" << std::endl;
                vk_json::_string_stream << m_PhysicalDeviceFeatures2.c_str();
            }

            // close GraphicsPipelineState
            vk_json::s_num_spaces -= 4;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "}," << std::endl;

            // Generate unique hash for each pipeline
            std::string hashString = *renderPassCreateInfoString;
            for (auto& it : pLayoutsHashStringList) {
                hashString += it;
            }
            hashString += *pipelineLayoutCreateInfoString;
            hashString += graphicsPipelineCreateInfoString;

            // Use shader code also for hash.
            hashString += shaderCode.str();

            // device extensions
            {
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "\"EnabledExtensions\" : " << std::endl;
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "[" << std::endl;
                vk_json::s_num_spaces += 4;

                for (unsigned int j = 0; j < m_deviceExtensions.size(); j++)
                    vk_json::print_char(m_deviceExtensions[j].data(), "", (j + 1) != m_deviceExtensions.size());

                vk_json::s_num_spaces -= 4;
                for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
                vk_json::_string_stream << "]," << std::endl;
            }

            // create pipeline identifier
            uint8_t pipelineUUID[VK_UUID_SIZE]{};
            createUUID(hashString, pipelineUUID);
            setPipelineUUID(device, pPipelines[cnt], pipelineUUID);

            // Dump Pipeline Identifier
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "\"PipelineUUID\" : " << std::endl;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "[" << std::endl;
            vk_json::s_num_spaces += 4;

            dumpUUID(fp, pipelineUUID);

            vk_json::s_num_spaces -= 4;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "]" << std::endl;

            vk_json::s_num_spaces -= 4;
            for (int i = 0; i < vk_json::s_num_spaces; i++) vk_json::_string_stream << " ";
            vk_json::_string_stream << "}" << std::endl;

            fprintf(fp, "%s", vk_json::_string_stream.str().c_str());
            m_pipelineCount++;
        }

        dumpObjectReservationInfo();
    }

    void dumpObjectReservationInfo()
    {
        std::string filename = getenv("VK_JSON_FILE_PATH");
        filename += getProcessName();
        filename += "_objectResInfo.hpp";
        FILE* fp = fopen(filename.c_str(), "w");
        if (!fp) {
            std::cout << "Unable to create file!";
            exit(-1);
        }

        std::stringstream ss {};
        ss << "#ifndef " << getProcessName() << "_objectResInfo_HPP\n";
        ss << "#define " << getProcessName() << "_objectResInfo_HPP\n\n";
        ss << "static VkDeviceObjectReservationCreateInfo g_objectResCreateInfo {};\n";
        ss << "static void SetObjectResCreateInfo() \n {\n";
        ss << "\tg_objectResCreateInfo.sType                                      = VK_STRUCTURE_TYPE_DEVICE_OBJECT_RESERVATION_CREATE_INFO;\n";
        ss << "\tg_objectResCreateInfo.pNext                                      = nullptr;\n";
        ss << "\tg_objectResCreateInfo.commandBufferRequestCount                  = " << objResInfo.commandBufferRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.fenceRequestCount                          = " << objResInfo.fenceRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.deviceMemoryRequestCount                   = " << 1 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.bufferRequestCount                         = " << objResInfo.bufferRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.imageRequestCount                          = " << objResInfo.imageRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.eventRequestCount                          = " << objResInfo.eventRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.queryPoolRequestCount                      = " << objResInfo.queryPoolRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.bufferViewRequestCount                     = " << objResInfo.bufferViewRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.imageViewRequestCount                      = " << objResInfo.imageViewRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.layeredImageViewRequestCount               = " << objResInfo.layeredImageViewRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.pipelineCacheRequestCount                  = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.pipelineLayoutRequestCount                 = " << objResInfo.pipelineLayoutRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.renderPassRequestCount                     = " << objResInfo.renderPassRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.graphicsPipelineRequestCount               = " << objResInfo.graphicsPipelineRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.computePipelineRequestCount                = " << objResInfo.computePipelineRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.descriptorSetLayoutRequestCount            = " << objResInfo.descriptorSetLayoutRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.samplerRequestCount                        = " << objResInfo.samplerRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.descriptorPoolRequestCount                 = " << objResInfo.descriptorPoolRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.descriptorSetRequestCount                  = " << objResInfo.descriptorSetRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.framebufferRequestCount                    = " << objResInfo.framebufferRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.commandPoolRequestCount                    = " << objResInfo.commandPoolRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.surfaceRequestCount                        = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.swapchainRequestCount                      = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.displayModeRequestCount                    = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.subpassDescriptionRequestCount             = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.descriptorSetLayoutBindingRequestCount     = " << objResInfo.descriptorSetLayoutBindingRequestCount << ";\n";
        ss << "\tg_objectResCreateInfo.attachmentDescriptionRequestCount          = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.samplerYcbcrConversionRequestCount         = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.descriptorSetLayoutBindingLimit            = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.maxImageViewMipLevels                      = " << objResInfo.maxImageViewMipLevels << ";\n";
        ss << "\tg_objectResCreateInfo.maxImageViewArrayLayers                    = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.maxLayeredImageViewMipLevels               = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.maxOcclusionQueriesPerPool                 = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.maxPipelineStatisticsQueriesPerPool        = " << 0 << "; // TODO: To be filled\n";
        ss << "\tg_objectResCreateInfo.maxImmutableSamplersPerDescriptorSetLayout = " << 0 << "; // TODO: To be filled\n";
        ss << "\n }";
        ss << "\n#endif";

        fprintf(fp, "%s", ss.str().c_str());
        fflush(fp);
        fclose(fp);
    }

    void clearString() { _string_stream.str(std::string("")); }

   private:
    // TODO: Better way to do this?
    const char* getProcessName() 
    {
#ifdef _WIN32
        if (GetModuleFileName(0, m_exeName, MAX_NAME_SIZE) == 0) return "";

        // Some raw C code to get the .exe name on Windows.
        char* p = m_exeName + (strnlen(m_exeName, MAX_NAME_SIZE) - 1);
        int writePos = 0;

        while (*p-- != '.')
            ;
        while (*p != '\\') m_exeName[writePos++] = *p--;
        m_exeName[writePos] = 0;
        _strrev(m_exeName);
#endif

#ifdef __linux__
        char exeName[1024];
        int ret = readlink("/proc/self/exe", exeName, sizeof(exeName) - 1);
        if (ret == -1) {
            std::cout << "Error obtaining process name!" << std::endl;
            exit(1);
        }
        exeName[ret] = 0;

        char* p = exeName + (ret - 1);
        int writePos = 0;
        while (*p != '/') m_exeName[writePos++] = *p--;

        m_exeName[writePos] = 0;
        std::string tmp(m_exeName);
        std::reverse(tmp.begin(), tmp.end());
        strncpy(m_exeName, tmp.c_str(), sizeof(m_exeName) - 1);
#endif
        return m_exeName;
    }

    FILE* openFile() 
    {
        char* path = getenv("VK_JSON_FILE_PATH");
        std::string filename;
        if (path) {
            filename = path;
#ifdef _WIN32
            filename += "\\";
#else
            filename += "/";
#endif
        } else {
            std::cout << "Error with setting VK_JSON_FILE_PATH. Please set the environment variable.\n";
            exit(-1);
        }
        std::cout << "Dumping generated JSON into:  " << path << "\n";
        std::stringstream ss;
        ss << "pipeline_" << m_pipelineCount;
        filename += getProcessName();
        filename += "_";
        filename += ss.str();
        filename += std::string(".json");

        std::cout << "Writing into JSON: " << filename << "\n";
        FILE* fp = fopen(filename.c_str(), "w");
        if (!fp) {
            std::cout << "Unable to create file!";
            exit(-1);
        }

        return fp;
    }

    struct ShaderInformation {
        ShaderInformation() {}
        ~ShaderInformation() { free(m_code); }
        ShaderInformation(const ShaderInformation& s) {
            m_module = s.m_module;
            m_codeSize = s.m_codeSize;

            m_code = (uint32_t*)malloc(m_codeSize);
            memcpy(m_code, s.m_code, s.m_codeSize);
        }
        ShaderInformation(VkShaderModule m, size_t sz, const uint32_t* c) {
            m_module = m;
            m_codeSize = sz;

            m_code = (uint32_t*)malloc(sz);
            memcpy(m_code, c, sz);
        }

        VkShaderModule m_module;
        size_t m_codeSize;
        uint32_t* m_code;
    };

    typedef std::vector<VkDescriptorSetLayout> DescLayoutVec;

    std::vector<std::string *> m_jsonStringsList;
    VkPrivateDataSlot m_privateDataSlot{};
    std::vector<uint8_t *> m_uuidAllocList;
    VkPrivateDataSlot m_privateDataUuidSlot{};
    std::string m_GraphicsPipelineCreateInfo;
    std::string m_ComputePipelineCreateInfo;
    std::string m_PhysicalDeviceFeatures2;
    std::string m_ImmutableSamplers;
    std::map<VkPipelineLayout, DescLayoutVec> m_pipelineLayoutDescSetMap;
    std::map<VkShaderModule, ShaderInformation> m_shaderInfoMap;
    std::map<VkSampler, VkSamplerCreateInfo> m_samplerInfoMap;
    std::map<VkSampler, std::string> m_ImmutableSamplerBindingMap;
    std::vector<std::string> m_deviceExtensions;
    int m_pipelineCount;
    static const int MAX_NAME_SIZE = 255;
    char m_exeName[MAX_NAME_SIZE];
    bool m_renderPassFlag = false;
    bool m_renderPass2Flag = false;
};

static PipelineData s_pipe;

}  // namespace vk_json


// mark any features implemented in the layer as supported
void set_layer_supported_features(VkPhysicalDeviceFeatures2 *pFeatures)
{
    VkBaseOutStructure* pNext = reinterpret_cast<VkBaseOutStructure *>(pFeatures->pNext);

    while (pNext != nullptr)
    {
        if (pNext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_PROPERTIES_FEATURES_EXT)
        {
            VkPhysicalDevicePipelinePropertiesFeaturesEXT *features = reinterpret_cast<VkPhysicalDevicePipelinePropertiesFeaturesEXT *>(pNext);
            features->pipelinePropertiesIdentifier = VK_TRUE;
        }
        pNext = pNext->pNext;
    }

}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties)
{

    VkResult result = VK_SUCCESS;

    if (pPipelineProperties->sType == VK_STRUCTURE_TYPE_PIPELINE_PROPERTIES_IDENTIFIER_EXT)
    {
        VkPipelinePropertiesIdentifierEXT *props = reinterpret_cast<VkPipelinePropertiesIdentifierEXT *>(pPipelineProperties);

        vk_json::s_pipe.getPipelineUUID(device, pPipelineInfo->pipeline, props->pipelineIdentifier);
    }

    return result;
}

#if 0
VkDevice g_vkDevice;

void test(VkPipeline pipeline)
{
    VkPipelineInfoEXT pInfo{VK_STRUCTURE_TYPE_PIPELINE_INFO_EXT};
    pInfo.pipeline = pipeline;

    VkPipelinePropertiesIdentifierEXT pIdent{VK_STRUCTURE_TYPE_PIPELINE_PROPERTIES_IDENTIFIER_EXT};
    VkBaseOutStructure *pOut = reinterpret_cast<VkBaseOutStructure *>(&pIdent);

    vkGetPipelinePropertiesEXT(g_vkDevice, &pInfo, pOut);
}
#endif

