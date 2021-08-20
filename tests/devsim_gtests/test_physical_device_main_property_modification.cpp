/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors:
 * - Jeremy Kniager <jeremyk@lunarg.com>
 */

#include <iostream>

#include <gtest/gtest.h>
#include <vulkan/vulkan.h>

#include "devsim_test_helper.h"

TEST(test_physical_device_main_property_modification, single_file) {
    VkResult err = VK_SUCCESS;

    devsim_test::initLayerPath();

    devsim_test::VulkanInstanceBuilder inst_builder;

    std::vector<std::string> filepaths = {TEST_SOURCE_PATH "/devsim_test1_in.json"};
    devsim_test::setDevsimFilenames(filepaths);

    // std::string abs_filepath = devsim_test::getAbsolutePath("../../../tests/devsim_gtests/devsim_test1_in.json");
    // devsim_test::setEnvironmentSetting("VK_DEVSIM_FILENAME", abs_filepath.c_str());

    inst_builder.addLayer("VK_LAYER_LUNARG_device_simulation");

    err = inst_builder.makeInstance();
    ASSERT_EQ(err, VK_SUCCESS);

    VkInstance test_inst = inst_builder.getInstance();

    uint32_t gpu_count = 1;
    VkPhysicalDevice gpu = VK_NULL_HANDLE;
    vkEnumeratePhysicalDevices(test_inst, &gpu_count, &gpu);

    VkPhysicalDeviceProperties gpu_props{};
    vkGetPhysicalDeviceProperties(gpu, &gpu_props);

    VkPhysicalDeviceFeatures gpu_feats{};
    vkGetPhysicalDeviceFeatures(gpu, &gpu_feats);

    VkPhysicalDeviceMemoryProperties gpu_mem_props{};
    vkGetPhysicalDeviceMemoryProperties(gpu, &gpu_mem_props);

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_family_props(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, queue_family_props.data());

    // Properties
    EXPECT_EQ(gpu_props.apiVersion, 1);
    EXPECT_EQ(gpu_props.driverVersion, 2);
    EXPECT_EQ(gpu_props.vendorID, 0);
    EXPECT_EQ(gpu_props.deviceID, 0);
    EXPECT_EQ(gpu_props.deviceType, 0);
    EXPECT_STREQ(gpu_props.deviceName, "device_simulation_layer_test_1");
    EXPECT_EQ(gpu_props.pipelineCacheUUID[0], 1);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[1], 2);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[2], 3);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[3], 4);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[4], 5);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[5], 6);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[6], 7);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[7], 8);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[8], 9);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[9], 10);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[10], 11);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[11], 12);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[12], 13);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[13], 14);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[14], 15);
    EXPECT_EQ(gpu_props.pipelineCacheUUID[15], 16);

    // Limits
    EXPECT_EQ(gpu_props.limits.maxImageDimension1D, 900001000);
    EXPECT_EQ(gpu_props.limits.maxImageDimension2D, 900001001);
    EXPECT_EQ(gpu_props.limits.maxImageDimension3D, 900001002);
    EXPECT_EQ(gpu_props.limits.maxImageDimensionCube, 900001003);
    EXPECT_EQ(gpu_props.limits.maxImageArrayLayers, 900001004);
    EXPECT_EQ(gpu_props.limits.maxTexelBufferElements, 900001005);
    EXPECT_EQ(gpu_props.limits.maxUniformBufferRange, 900001006);
    EXPECT_EQ(gpu_props.limits.maxStorageBufferRange, 900001007);
    EXPECT_EQ(gpu_props.limits.maxPushConstantsSize, 900001008);
    EXPECT_EQ(gpu_props.limits.maxMemoryAllocationCount, 900001009);
    EXPECT_EQ(gpu_props.limits.maxSamplerAllocationCount, 900001010);
    EXPECT_EQ(gpu_props.limits.bufferImageGranularity, 80000001011);
    EXPECT_EQ(gpu_props.limits.sparseAddressSpaceSize, 80000001012);
    EXPECT_EQ(gpu_props.limits.maxBoundDescriptorSets, 900001013);
    EXPECT_EQ(gpu_props.limits.maxPerStageDescriptorSamplers, 900001014);
    EXPECT_EQ(gpu_props.limits.maxPerStageDescriptorUniformBuffers, 900001015);
    EXPECT_EQ(gpu_props.limits.maxPerStageDescriptorStorageBuffers, 900001016);
    EXPECT_EQ(gpu_props.limits.maxPerStageDescriptorSampledImages,	900001017);
    EXPECT_EQ(gpu_props.limits.maxPerStageDescriptorStorageImages, 900001018);
    EXPECT_EQ(gpu_props.limits.maxPerStageDescriptorInputAttachments, 900001019);
    EXPECT_EQ(gpu_props.limits.maxPerStageResources, 900001020);
    EXPECT_EQ(gpu_props.limits.maxDescriptorSetSamplers, 900001021);
    EXPECT_EQ(gpu_props.limits.maxDescriptorSetUniformBuffers, 900001022);
    EXPECT_EQ(gpu_props.limits.maxDescriptorSetUniformBuffersDynamic, 900001023);
    EXPECT_EQ(gpu_props.limits.maxDescriptorSetStorageBuffers, 900001024);
    EXPECT_EQ(gpu_props.limits.maxDescriptorSetStorageBuffersDynamic, 900001025);
    EXPECT_EQ(gpu_props.limits.maxDescriptorSetSampledImages, 900001026);
    EXPECT_EQ(gpu_props.limits.maxDescriptorSetStorageImages, 900001027);
    EXPECT_EQ(gpu_props.limits.maxDescriptorSetInputAttachments, 900001028);
    EXPECT_EQ(gpu_props.limits.maxVertexInputAttributes, 900001029);
    EXPECT_EQ(gpu_props.limits.maxVertexInputBindings, 900001030);
    EXPECT_EQ(gpu_props.limits.maxVertexInputAttributeOffset, 900001031);
    EXPECT_EQ(gpu_props.limits.maxVertexInputBindingStride, 900001032);
    EXPECT_EQ(gpu_props.limits.maxVertexOutputComponents, 900001033);
    EXPECT_EQ(gpu_props.limits.maxTessellationGenerationLevel, 900001034);
    EXPECT_EQ(gpu_props.limits.maxTessellationPatchSize, 900001035);
    EXPECT_EQ(gpu_props.limits.maxTessellationControlPerVertexInputComponents, 900001036);
    EXPECT_EQ(gpu_props.limits.maxTessellationControlPerVertexOutputComponents, 900001037);
    EXPECT_EQ(gpu_props.limits.maxTessellationControlPerPatchOutputComponents, 900001038);
    EXPECT_EQ(gpu_props.limits.maxTessellationControlTotalOutputComponents, 900001039);
    EXPECT_EQ(gpu_props.limits.maxTessellationEvaluationInputComponents, 900001040);
    EXPECT_EQ(gpu_props.limits.maxTessellationEvaluationOutputComponents, 900001041);
    EXPECT_EQ(gpu_props.limits.maxGeometryShaderInvocations, 900001042);
    EXPECT_EQ(gpu_props.limits.maxGeometryInputComponents, 900001043);
    EXPECT_EQ(gpu_props.limits.maxGeometryOutputComponents, 900001044);
    EXPECT_EQ(gpu_props.limits.maxGeometryOutputVertices, 900001045);
    EXPECT_EQ(gpu_props.limits.maxGeometryTotalOutputComponents, 900001046);
    EXPECT_EQ(gpu_props.limits.maxFragmentInputComponents, 900001047);
    EXPECT_EQ(gpu_props.limits.maxFragmentOutputAttachments, 900001048);
    EXPECT_EQ(gpu_props.limits.maxFragmentDualSrcAttachments, 900001049);
    EXPECT_EQ(gpu_props.limits.maxFragmentCombinedOutputResources, 900001050);
    EXPECT_EQ(gpu_props.limits.maxComputeSharedMemorySize, 900001051);
    EXPECT_EQ(gpu_props.limits.maxComputeWorkGroupCount[0], 900001052);
    EXPECT_EQ(gpu_props.limits.maxComputeWorkGroupCount[1], 900001053);
    EXPECT_EQ(gpu_props.limits.maxComputeWorkGroupCount[2], 900001054);
    EXPECT_EQ(gpu_props.limits.maxComputeWorkGroupInvocations, 900001055);
    EXPECT_EQ(gpu_props.limits.maxComputeWorkGroupSize[0], 900001056);
    EXPECT_EQ(gpu_props.limits.maxComputeWorkGroupSize[1], 900001057);
    EXPECT_EQ(gpu_props.limits.maxComputeWorkGroupSize[2], 900001058);
    EXPECT_EQ(gpu_props.limits.subPixelPrecisionBits, 900001059);
    EXPECT_EQ(gpu_props.limits.subTexelPrecisionBits, 900001060);
    EXPECT_EQ(gpu_props.limits.mipmapPrecisionBits, 900001061);
    EXPECT_EQ(gpu_props.limits.maxDrawIndexedIndexValue, 31062);
    EXPECT_EQ(gpu_props.limits.maxDrawIndirectCount, 31063);
    EXPECT_EQ(gpu_props.limits.maxSamplerLodBias, 900001064);
    EXPECT_EQ(gpu_props.limits.maxSamplerAnisotropy, 900001065);
    EXPECT_EQ(gpu_props.limits.maxViewports, 900001066);
    EXPECT_EQ(gpu_props.limits.maxViewportDimensions[0], 900001067);
    EXPECT_EQ(gpu_props.limits.maxViewportDimensions[1], 900001068);
    EXPECT_EQ(gpu_props.limits.viewportBoundsRange[0], -900001069);
    EXPECT_EQ(gpu_props.limits.viewportBoundsRange[1], 900001070);
    EXPECT_EQ(gpu_props.limits.viewportSubPixelBits, 900001071);
    EXPECT_EQ(gpu_props.limits.minMemoryMapAlignment, 900001072);
    EXPECT_EQ(gpu_props.limits.minTexelBufferOffsetAlignment, 80000001073);
    EXPECT_EQ(gpu_props.limits.minUniformBufferOffsetAlignment, 80000001074);
    EXPECT_EQ(gpu_props.limits.minStorageBufferOffsetAlignment, 80000001075);
    EXPECT_EQ(gpu_props.limits.minTexelOffset, -900001076);
    EXPECT_EQ(gpu_props.limits.maxTexelOffset, 900001077);
    EXPECT_EQ(gpu_props.limits.minTexelGatherOffset, -900001078);
    EXPECT_EQ(gpu_props.limits.maxTexelGatherOffset, 900001079);
    EXPECT_EQ(gpu_props.limits.minInterpolationOffset, 201.5);
    EXPECT_EQ(gpu_props.limits.maxInterpolationOffset, -202.5);
    EXPECT_EQ(gpu_props.limits.subPixelInterpolationOffsetBits, 900001080);
    EXPECT_EQ(gpu_props.limits.maxFramebufferWidth, 900001081);
    EXPECT_EQ(gpu_props.limits.maxFramebufferHeight, 900001082);
    EXPECT_EQ(gpu_props.limits.maxFramebufferLayers, 900001083);
    EXPECT_EQ(gpu_props.limits.framebufferColorSampleCounts, 900001084);
    EXPECT_EQ(gpu_props.limits.framebufferDepthSampleCounts, 900001085);
    EXPECT_EQ(gpu_props.limits.framebufferStencilSampleCounts, 900001086);
    EXPECT_EQ(gpu_props.limits.framebufferNoAttachmentsSampleCounts, 900001087);
    EXPECT_EQ(gpu_props.limits.maxColorAttachments, 900001088);
    EXPECT_EQ(gpu_props.limits.sampledImageColorSampleCounts, 900001089);
    EXPECT_EQ(gpu_props.limits.sampledImageIntegerSampleCounts, 900001090);
    EXPECT_EQ(gpu_props.limits.sampledImageDepthSampleCounts, 900001091);
    EXPECT_EQ(gpu_props.limits.sampledImageStencilSampleCounts, 900001092);
    EXPECT_EQ(gpu_props.limits.storageImageSampleCounts, 900001093);
    EXPECT_EQ(gpu_props.limits.maxSampleMaskWords, 900001094);
    EXPECT_EQ(gpu_props.limits.timestampComputeAndGraphics, 900001095);
    EXPECT_EQ(gpu_props.limits.timestampPeriod, 900001096);
    EXPECT_EQ(gpu_props.limits.maxClipDistances, 900001097);
    EXPECT_EQ(gpu_props.limits.maxCullDistances, 900001098);
    EXPECT_EQ(gpu_props.limits.maxCombinedClipAndCullDistances, 900001099);
    EXPECT_EQ(gpu_props.limits.discreteQueuePriorities, 900001100);
    EXPECT_EQ(gpu_props.limits.pointSizeRange[0], -203.5);
    EXPECT_EQ(gpu_props.limits.pointSizeRange[1], -204.5);
    EXPECT_EQ(gpu_props.limits.lineWidthRange[0], -205.5);
    EXPECT_EQ(gpu_props.limits.lineWidthRange[1], -206.5);
    EXPECT_EQ(gpu_props.limits.pointSizeGranularity, -207.5);
    EXPECT_EQ(gpu_props.limits.lineWidthGranularity, -208.5);
    EXPECT_EQ(gpu_props.limits.strictLines, 900001101);
    EXPECT_EQ(gpu_props.limits.standardSampleLocations, 900001102);
    EXPECT_EQ(gpu_props.limits.optimalBufferCopyOffsetAlignment, 8000001103);
    EXPECT_EQ(gpu_props.limits.optimalBufferCopyRowPitchAlignment, 80000001104);
    EXPECT_EQ(gpu_props.limits.nonCoherentAtomSize, 80000001105);

    // Sparse Properties
    EXPECT_EQ(gpu_props.sparseProperties.residencyStandard2DBlockShape, 900001106);
    EXPECT_EQ(gpu_props.sparseProperties.residencyStandard2DMultisampleBlockShape, 900001107);
    EXPECT_EQ(gpu_props.sparseProperties.residencyStandard3DBlockShape, 900001108);
    EXPECT_EQ(gpu_props.sparseProperties.residencyAlignedMipSize, 900001109);
    EXPECT_EQ(gpu_props.sparseProperties.residencyNonResidentStrict, 900001110);

    // Features
    EXPECT_EQ(gpu_feats.robustBufferAccess, 101);
    EXPECT_EQ(gpu_feats.fullDrawIndexUint32, 102);
    EXPECT_EQ(gpu_feats.imageCubeArray, 103);
    EXPECT_EQ(gpu_feats.independentBlend, 104);
    EXPECT_EQ(gpu_feats.geometryShader, 105);
    EXPECT_EQ(gpu_feats.tessellationShader, 106);
    EXPECT_EQ(gpu_feats.sampleRateShading, 107);
    EXPECT_EQ(gpu_feats.dualSrcBlend, 108);
    EXPECT_EQ(gpu_feats.logicOp, 109);
    EXPECT_EQ(gpu_feats.multiDrawIndirect, 110);
    EXPECT_EQ(gpu_feats.drawIndirectFirstInstance, 111);
    EXPECT_EQ(gpu_feats.depthClamp, 112);
    EXPECT_EQ(gpu_feats.depthBiasClamp, 113);
    EXPECT_EQ(gpu_feats.fillModeNonSolid, 114);
    EXPECT_EQ(gpu_feats.depthBounds, 115);
    EXPECT_EQ(gpu_feats.wideLines, 116);
    EXPECT_EQ(gpu_feats.largePoints, 117);
    EXPECT_EQ(gpu_feats.alphaToOne, 118);
    EXPECT_EQ(gpu_feats.multiViewport, 119);
    EXPECT_EQ(gpu_feats.samplerAnisotropy, 120);
    EXPECT_EQ(gpu_feats.textureCompressionETC2, 121);
    EXPECT_EQ(gpu_feats.textureCompressionASTC_LDR, 122);
    EXPECT_EQ(gpu_feats.textureCompressionBC, 123);
    EXPECT_EQ(gpu_feats.occlusionQueryPrecise, 124);
    EXPECT_EQ(gpu_feats.pipelineStatisticsQuery, 125);
    EXPECT_EQ(gpu_feats.vertexPipelineStoresAndAtomics, 126);
    EXPECT_EQ(gpu_feats.fragmentStoresAndAtomics, 127);
    EXPECT_EQ(gpu_feats.shaderTessellationAndGeometryPointSize, 128);
    EXPECT_EQ(gpu_feats.shaderImageGatherExtended, 129);
    EXPECT_EQ(gpu_feats.shaderStorageImageExtendedFormats, 130);
    EXPECT_EQ(gpu_feats.shaderStorageImageMultisample, 131);
    EXPECT_EQ(gpu_feats.shaderStorageImageReadWithoutFormat, 132);
    EXPECT_EQ(gpu_feats.shaderStorageImageWriteWithoutFormat, 133);
    EXPECT_EQ(gpu_feats.shaderUniformBufferArrayDynamicIndexing, 134);
    EXPECT_EQ(gpu_feats.shaderSampledImageArrayDynamicIndexing, 135);
    EXPECT_EQ(gpu_feats.shaderStorageBufferArrayDynamicIndexing, 136);
    EXPECT_EQ(gpu_feats.shaderStorageImageArrayDynamicIndexing, 137);
    EXPECT_EQ(gpu_feats.shaderClipDistance, 138);
    EXPECT_EQ(gpu_feats.shaderCullDistance, 139);
    EXPECT_EQ(gpu_feats.shaderFloat64, 140);
    EXPECT_EQ(gpu_feats.shaderInt64, 141);
    EXPECT_EQ(gpu_feats.shaderInt16, 142);
    EXPECT_EQ(gpu_feats.shaderResourceResidency, 143);
    EXPECT_EQ(gpu_feats.shaderResourceMinLod, 144);
    EXPECT_EQ(gpu_feats.sparseBinding, 145);
    EXPECT_EQ(gpu_feats.sparseResidencyBuffer, 146);
    EXPECT_EQ(gpu_feats.sparseResidencyImage2D, 147);
    EXPECT_EQ(gpu_feats.sparseResidencyImage3D, 148);
    EXPECT_EQ(gpu_feats.sparseResidency2Samples, 149);
    EXPECT_EQ(gpu_feats.sparseResidency4Samples, 150);
    EXPECT_EQ(gpu_feats.sparseResidency8Samples, 151);
    EXPECT_EQ(gpu_feats.sparseResidency16Samples, 152);
    EXPECT_EQ(gpu_feats.sparseResidencyAliased, 153);
    EXPECT_EQ(gpu_feats.variableMultisampleRate, 154);
    EXPECT_EQ(gpu_feats.inheritedQueries, 155);

    // Memory Properties
    // Heaps
    EXPECT_EQ(gpu_mem_props.memoryHeaps[0].size, 80000002001);

    // Queue Family Properties
    EXPECT_EQ(queue_family_props[0].minImageTransferGranularity.depth, 900004001);
    EXPECT_EQ(queue_family_props[0].minImageTransferGranularity.height, 900004002);
    EXPECT_EQ(queue_family_props[0].minImageTransferGranularity.width, 900004003);
    EXPECT_EQ(queue_family_props[0].queueCount, 900004004);
    EXPECT_EQ(queue_family_props[0].queueFlags, 900004005);
    EXPECT_EQ(queue_family_props[0].timestampValidBits, 900004006);

    EXPECT_EQ(queue_family_props[1].minImageTransferGranularity.depth, 900004007);
    EXPECT_EQ(queue_family_props[1].minImageTransferGranularity.height, 900004008);
    EXPECT_EQ(queue_family_props[1].minImageTransferGranularity.width, 900004009);
    EXPECT_EQ(queue_family_props[1].queueCount, 900004010);
    EXPECT_EQ(queue_family_props[1].queueFlags, 900004011);
    EXPECT_EQ(queue_family_props[1].timestampValidBits, 900004012);

    EXPECT_EQ(queue_family_props[2].minImageTransferGranularity.depth, 900004013);
    EXPECT_EQ(queue_family_props[2].minImageTransferGranularity.height, 900004014);
    EXPECT_EQ(queue_family_props[2].minImageTransferGranularity.width, 900004015);
    EXPECT_EQ(queue_family_props[2].queueCount, 900004016);
    EXPECT_EQ(queue_family_props[2].queueFlags, 900004017);
    EXPECT_EQ(queue_family_props[2].timestampValidBits, 900004018);
    
    vkDestroyInstance(test_inst, nullptr);
}