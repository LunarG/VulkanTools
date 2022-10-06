#ifndef JSON_GEN_LAYER_TESTS_objectResInfo_HPP
#define JSON_GEN_LAYER_TESTS_objectResInfo_HPP

static VkDeviceObjectReservationCreateInfo g_objectResCreateInfo {};
static void SetObjectResCreateInfo() 
 {
	g_objectResCreateInfo.sType                                      = VK_STRUCTURE_TYPE_DEVICE_OBJECT_RESERVATION_CREATE_INFO;
	g_objectResCreateInfo.pNext                                      = nullptr;
	g_objectResCreateInfo.commandBufferRequestCount                  = 0;
	g_objectResCreateInfo.fenceRequestCount                          = 0;
	g_objectResCreateInfo.deviceMemoryRequestCount                   = 1; // TODO: To be filled
	g_objectResCreateInfo.bufferRequestCount                         = 0;
	g_objectResCreateInfo.imageRequestCount                          = 0;
	g_objectResCreateInfo.eventRequestCount                          = 0;
	g_objectResCreateInfo.queryPoolRequestCount                      = 0;
	g_objectResCreateInfo.bufferViewRequestCount                     = 0;
	g_objectResCreateInfo.imageViewRequestCount                      = 0;
	g_objectResCreateInfo.layeredImageViewRequestCount               = 0;
	g_objectResCreateInfo.pipelineCacheRequestCount                  = 0; // TODO: To be filled
	g_objectResCreateInfo.pipelineLayoutRequestCount                 = 1;
	g_objectResCreateInfo.renderPassRequestCount                     = 1;
	g_objectResCreateInfo.graphicsPipelineRequestCount               = 1;
	g_objectResCreateInfo.computePipelineRequestCount                = 0;
	g_objectResCreateInfo.descriptorSetLayoutRequestCount            = 0;
	g_objectResCreateInfo.samplerRequestCount                        = 0;
	g_objectResCreateInfo.descriptorPoolRequestCount                 = 0;
	g_objectResCreateInfo.descriptorSetRequestCount                  = 0;
	g_objectResCreateInfo.framebufferRequestCount                    = 0;
	g_objectResCreateInfo.commandPoolRequestCount                    = 0;
	g_objectResCreateInfo.surfaceRequestCount                        = 0; // TODO: To be filled
	g_objectResCreateInfo.swapchainRequestCount                      = 0; // TODO: To be filled
	g_objectResCreateInfo.displayModeRequestCount                    = 0; // TODO: To be filled
	g_objectResCreateInfo.subpassDescriptionRequestCount             = 0; // TODO: To be filled
	g_objectResCreateInfo.descriptorSetLayoutBindingRequestCount     = 0;
	g_objectResCreateInfo.attachmentDescriptionRequestCount          = 0; // TODO: To be filled
	g_objectResCreateInfo.samplerYcbcrConversionRequestCount         = 0; // TODO: To be filled
	g_objectResCreateInfo.descriptorSetLayoutBindingLimit            = 0; // TODO: To be filled
	g_objectResCreateInfo.maxImageViewMipLevels                      = 0;
	g_objectResCreateInfo.maxImageViewArrayLayers                    = 0; // TODO: To be filled
	g_objectResCreateInfo.maxLayeredImageViewMipLevels               = 0; // TODO: To be filled
	g_objectResCreateInfo.maxOcclusionQueriesPerPool                 = 0; // TODO: To be filled
	g_objectResCreateInfo.maxPipelineStatisticsQueriesPerPool        = 0; // TODO: To be filled
	g_objectResCreateInfo.maxImmutableSamplersPerDescriptorSetLayout = 0; // TODO: To be filled

 }
#endif