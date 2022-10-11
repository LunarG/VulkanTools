#ifndef _VULKAN_JSON_DATA_HPP
#define _VULKAN_JSON_DATA_HPP

/*
 * Copyright (c) 2021 The Khronos Group Inc.
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
 *//*!
 * \file
 * \brief Defines JSON generators for Vulkan structures
 */


/********************************************************************************************/
/** This code is generated. To make changes, please modify the scripts or the relevant xml **/
/********************************************************************************************/

#include <iostream>
#include <map>
#include <bitset>
#include <functional>
#include <sstream>
#include <cassert>
#include <cmath>
#ifndef VULKAN_JSON_CTS
    #include <vulkan/vulkan.h>
#endif

#ifdef _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#endif
	#define VC_EXTRALEAN
	#define NOMINMAX
	#include <windows.h>
#endif

namespace vk_json {

static thread_local int s_num_spaces    = 0;
#if defined(USE_THREAD_LOCAL_WAR)
// Workaround (off by default) for certain platforms that have a thread_local libc bug
std::stringstream & TLSGetStrStream();
#define _string_stream  TLSGetStrStream()
#else
static thread_local std::stringstream _string_stream;
#endif

static void dumpPNextChain(const void* pNext);

// By default, redirect to std::cout. Can stream it to a stringstream if needed.
//#define   _OUT std::cout
#define _OUT _string_stream

// Helper utility to do indentation in the generated json file.
#define PRINT_SPACE for (int k = 0; k < s_num_spaces; k++) _OUT << " ";

#define INDENT(sz) s_num_spaces += (sz);

#define PRINT_VAL(c) PRINT_SPACE \
    if (s != "") {\
        _OUT << "\"" << s << "\"" << " : " << o << (c ? "," : "") << std::endl; \
    } else {\
        _OUT << o << (c ? "," : "") << std::endl; \
    }

#define PRINT_STR(c) PRINT_SPACE \
    if (s != "") {\
        _OUT << "\"" << s << "\"" << " : " << "\"" << o << "\"" << (c ? "," : "") << std::endl; \
    } else {\
        _OUT << "\"" << o << "\"" << (c ? "," : "") << std::endl; \
    }

// To make sure the generated data is consistent across platforms,
// we typecast to 32-bit and dump the data.
// The value is not expected to exceed the range.
static void print_size_t(const size_t* o, const std::string& s, bool commaNeeded=true)
{
    PRINT_SPACE
    _OUT << "\"" << s << "\"" << " : " << static_cast<uint32_t>(*o) << (commaNeeded ? "," : "") << std::endl;\
}
static void print_size_t(size_t o, const std::string& s, bool commaNeeded=true)
{
    PRINT_SPACE
    _OUT << "\"" << s << "\"" << " : " << static_cast<uint32_t>(o) << (commaNeeded ? "," : "") << std::endl;\
}

static void print_int32_t(int32_t o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_int32_t(const int32_t * o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_uint32_t(uint32_t o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_uint32_t(const uint32_t * o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_uint8_t(uint8_t o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_uint8_t(const uint8_t * o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_uint64_t(uint64_t o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_uint64_t(const uint64_t * o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_float(float o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_float(const float * o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_int(int o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_int(const int * o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_double(double o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_double(const double * o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_int64_t(int64_t o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_int64_t(const int64_t * o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_uint16_t(uint16_t o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_uint16_t(const uint16_t * o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_char(const char * const* o, const std::string& s, bool commaNeeded=true)
{
    PRINT_STR(commaNeeded)
}

static void print_char(char o, const std::string& s, bool commaNeeded=true)
{
    PRINT_VAL(commaNeeded)
}

static void print_char(const char * o, const std::string& s, bool commaNeeded=true)
{
    PRINT_STR(commaNeeded)
}


// Base 64 formater class from executor/xeTestLogWriter.cpp

class Base64Formatter
{
public:
	const uint8_t*	data;
	int				numBytes;

	Base64Formatter(const uint8_t* data_, int numBytes_) : data(data_), numBytes(numBytes_) {}
};

std::ostream& operator<< (std::ostream& str, const Base64Formatter& fmt)
{
	static const char s_base64Table[64] =
	{
		'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m',
		'n','o','p','q','r','s','t','u','v','w','x','y','z',
		'0','1','2','3','4','5','6','7','8','9','+','/'
	};

	const uint8_t*	data = fmt.data;
	int				numBytes = fmt.numBytes;
	int				srcNdx = 0;

	assert(data && (numBytes > 0));

	/* Loop all input chars. */
	while (srcNdx < numBytes)
	{
        #undef min
		int		numRead = std::min(3, numBytes - srcNdx);
		uint8_t	s0 = data[srcNdx];
		uint8_t	s1 = (numRead >= 2) ? data[srcNdx + 1] : 0;
		uint8_t	s2 = (numRead >= 3) ? data[srcNdx + 2] : 0;
		char	d[4];

		srcNdx += numRead;

		d[0] = s_base64Table[s0 >> 2];
		d[1] = s_base64Table[((s0 & 0x3) << 4) | (s1 >> 4)];
		d[2] = s_base64Table[((s1 & 0xF) << 2) | (s2 >> 6)];
		d[3] = s_base64Table[s2 & 0x3F];

		if (numRead < 3) d[3] = '=';
		if (numRead < 2) d[2] = '=';

		/* Write data. */
		str.write(&d[0], sizeof(d));
	}

	return str;
}

inline Base64Formatter toBase64(const uint8_t* bytes, int numBytes) {return Base64Formatter(bytes, numBytes); }

static void print_void_data(const void * o, int oSize, const std::string& s, bool commaNeeded=true)
{
	if (o != NULL && oSize != 0)
	{
		PRINT_SPACE _OUT << "\"" << s << "\"" << " : " << "\"" << toBase64((uint8_t*)o, oSize) << "\"" << (commaNeeded ? "," : "") << std::endl;
	}
	else
	{
		PRINT_SPACE _OUT << "\"" << s << "\"" << " : " << "\"NULL\"" << (commaNeeded ? "," : "") << std::endl;
	}
}

static void print_VkBool32(VkBool32 obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "\"" << str << "\"" << " : " << "\"" << ((obj == 0) ? ("VK_FALSE") : ("VK_TRUE")) << "\"" << (commaNeeded ? "," : "") << std::endl;
}
static void print_VkBool32(const VkBool32 * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "\"" << str << "\"" << " : " << "\"" << ((obj == 0) ? ("VK_FALSE") : ("VK_TRUE")) << "\"" << (commaNeeded ? "," : "") << std::endl;
}

static void print_VkDeviceAddress(VkDeviceAddress obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "\"" << str << "\"" << " : " << "\"" << obj << "\"" << (commaNeeded ? "," : "") << std::endl;
}
static void print_VkDeviceAddress(const VkDeviceAddress * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "\"" << str << "\"" << " : " << "\"" << obj << "\"" << (commaNeeded ? "," : "") << std::endl;
}

static void print_VkDeviceSize(VkDeviceSize obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "\"" << str << "\"" << " : " << "\"" << obj << "\"" << (commaNeeded ? "," : "") << std::endl;
}
static void print_VkDeviceSize(const VkDeviceSize * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "\"" << str << "\"" << " : " << "\"" << obj << "\"" << (commaNeeded ? "," : "") << std::endl;
}

static void print_VkFlags(VkFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "\"" << str << "\"" << " : " << "\"" << obj << "\"" << (commaNeeded ? "," : "") << std::endl;
}
static void print_VkFlags(const VkFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "\"" << str << "\"" << " : " << "\"" << obj << "\"" << (commaNeeded ? "," : "") << std::endl;
}

static void print_VkSampleMask(VkSampleMask obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "\"" << str << "\"" << " : " << "\"" << obj << "\"" << (commaNeeded ? "," : "") << std::endl;
}
static void print_VkSampleMask(const VkSampleMask * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "\"" << str << "\"" << " : " << "\"" << obj << "\"" << (commaNeeded ? "," : "") << std::endl;
}

static void print_VkBuffer(VkBuffer obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkBuffer(const VkBuffer * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkImage(VkImage obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkImage(const VkImage * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkInstance(VkInstance obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkInstance(const VkInstance * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkPhysicalDevice(VkPhysicalDevice obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkPhysicalDevice(const VkPhysicalDevice * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkDevice(VkDevice obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkDevice(const VkDevice * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkQueue(VkQueue obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkQueue(const VkQueue * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkSemaphore(VkSemaphore obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkSemaphore(const VkSemaphore * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkCommandBuffer(VkCommandBuffer obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkCommandBuffer(const VkCommandBuffer * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkFence(VkFence obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkFence(const VkFence * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkDeviceMemory(VkDeviceMemory obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkDeviceMemory(const VkDeviceMemory * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkEvent(VkEvent obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkEvent(const VkEvent * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkQueryPool(VkQueryPool obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkQueryPool(const VkQueryPool * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkBufferView(VkBufferView obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkBufferView(const VkBufferView * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkImageView(VkImageView obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkImageView(const VkImageView * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkShaderModule(VkShaderModule obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkShaderModule(const VkShaderModule * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkPipelineCache(VkPipelineCache obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkPipelineCache(const VkPipelineCache * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkPipelineLayout(VkPipelineLayout obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkPipelineLayout(const VkPipelineLayout * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkPipeline(VkPipeline obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkPipeline(const VkPipeline * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkRenderPass(VkRenderPass obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkRenderPass(const VkRenderPass * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkDescriptorSetLayout(VkDescriptorSetLayout obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkDescriptorSetLayout(const VkDescriptorSetLayout * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkSampler(VkSampler obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkSampler(const VkSampler * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkDescriptorSet(VkDescriptorSet obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkDescriptorSet(const VkDescriptorSet * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkDescriptorPool(VkDescriptorPool obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkDescriptorPool(const VkDescriptorPool * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkFramebuffer(VkFramebuffer obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkFramebuffer(const VkFramebuffer * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkCommandPool(VkCommandPool obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkCommandPool(const VkCommandPool * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkResult_map = {
    std::make_pair(0, "VK_SUCCESS"),
    std::make_pair(1, "VK_NOT_READY"),
    std::make_pair(2, "VK_TIMEOUT"),
    std::make_pair(3, "VK_EVENT_SET"),
    std::make_pair(4, "VK_EVENT_RESET"),
    std::make_pair(5, "VK_INCOMPLETE"),
    std::make_pair(-1, "VK_ERROR_OUT_OF_HOST_MEMORY"),
    std::make_pair(-2, "VK_ERROR_OUT_OF_DEVICE_MEMORY"),
    std::make_pair(-3, "VK_ERROR_INITIALIZATION_FAILED"),
    std::make_pair(-4, "VK_ERROR_DEVICE_LOST"),
    std::make_pair(-5, "VK_ERROR_MEMORY_MAP_FAILED"),
    std::make_pair(-6, "VK_ERROR_LAYER_NOT_PRESENT"),
    std::make_pair(-7, "VK_ERROR_EXTENSION_NOT_PRESENT"),
    std::make_pair(-8, "VK_ERROR_FEATURE_NOT_PRESENT"),
    std::make_pair(-9, "VK_ERROR_INCOMPATIBLE_DRIVER"),
    std::make_pair(-10, "VK_ERROR_TOO_MANY_OBJECTS"),
    std::make_pair(-11, "VK_ERROR_FORMAT_NOT_SUPPORTED"),
    std::make_pair(-12, "VK_ERROR_FRAGMENTED_POOL"),
    std::make_pair(-13, "VK_ERROR_UNKNOWN"),
    std::make_pair(1000069000, "VK_ERROR_OUT_OF_POOL_MEMORY"),
    std::make_pair(1000072003, "VK_ERROR_INVALID_EXTERNAL_HANDLE"),
    std::make_pair(1000161000, "VK_ERROR_FRAGMENTATION"),
    std::make_pair(1000257000, "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS"),
    std::make_pair(1000000000, "VK_ERROR_SURFACE_LOST_KHR"),
    std::make_pair(1000000001, "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR"),
    std::make_pair(1000001003, "VK_SUBOPTIMAL_KHR"),
    std::make_pair(1000001004, "VK_ERROR_OUT_OF_DATE_KHR"),
    std::make_pair(1000003001, "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR"),
    std::make_pair(1000011001, "VK_ERROR_VALIDATION_FAILED_EXT"),
    std::make_pair(1000012000, "VK_ERROR_INVALID_SHADER_NV"),
    std::make_pair(1000158000, "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT"),
    std::make_pair(1000174001, "VK_ERROR_NOT_PERMITTED_EXT"),
    std::make_pair(1000255000, "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT"),
    std::make_pair(1000268000, "VK_THREAD_IDLE_KHR"),
    std::make_pair(1000268001, "VK_THREAD_DONE_KHR"),
    std::make_pair(1000268002, "VK_OPERATION_DEFERRED_KHR"),
    std::make_pair(1000268003, "VK_OPERATION_NOT_DEFERRED_KHR"),
    std::make_pair(1000297000, "VK_PIPELINE_COMPILE_REQUIRED_EXT"),
};
static void print_VkResult(VkResult obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkResult_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkResult_map[obj] << "\"" << std::endl;
}
static void print_VkResult(const VkResult * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkResult_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkResult_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkStructureType_map = {
    std::make_pair(0, "VK_STRUCTURE_TYPE_APPLICATION_INFO"),
    std::make_pair(1, "VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO"),
    std::make_pair(2, "VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO"),
    std::make_pair(3, "VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO"),
    std::make_pair(4, "VK_STRUCTURE_TYPE_SUBMIT_INFO"),
    std::make_pair(5, "VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO"),
    std::make_pair(6, "VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE"),
    std::make_pair(7, "VK_STRUCTURE_TYPE_BIND_SPARSE_INFO"),
    std::make_pair(8, "VK_STRUCTURE_TYPE_FENCE_CREATE_INFO"),
    std::make_pair(9, "VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO"),
    std::make_pair(10, "VK_STRUCTURE_TYPE_EVENT_CREATE_INFO"),
    std::make_pair(11, "VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO"),
    std::make_pair(12, "VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO"),
    std::make_pair(13, "VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO"),
    std::make_pair(14, "VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO"),
    std::make_pair(15, "VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO"),
    std::make_pair(16, "VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO"),
    std::make_pair(17, "VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO"),
    std::make_pair(18, "VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO"),
    std::make_pair(19, "VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO"),
    std::make_pair(20, "VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO"),
    std::make_pair(21, "VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO"),
    std::make_pair(22, "VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO"),
    std::make_pair(23, "VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO"),
    std::make_pair(24, "VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO"),
    std::make_pair(25, "VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO"),
    std::make_pair(26, "VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO"),
    std::make_pair(27, "VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO"),
    std::make_pair(28, "VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO"),
    std::make_pair(29, "VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO"),
    std::make_pair(30, "VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO"),
    std::make_pair(31, "VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO"),
    std::make_pair(32, "VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO"),
    std::make_pair(33, "VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO"),
    std::make_pair(34, "VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO"),
    std::make_pair(35, "VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET"),
    std::make_pair(36, "VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET"),
    std::make_pair(37, "VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO"),
    std::make_pair(38, "VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO"),
    std::make_pair(39, "VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO"),
    std::make_pair(40, "VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO"),
    std::make_pair(41, "VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO"),
    std::make_pair(42, "VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO"),
    std::make_pair(43, "VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO"),
    std::make_pair(44, "VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER"),
    std::make_pair(45, "VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER"),
    std::make_pair(46, "VK_STRUCTURE_TYPE_MEMORY_BARRIER"),
    std::make_pair(47, "VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO"),
    std::make_pair(48, "VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO"),
    std::make_pair(1000094000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES"),
    std::make_pair(1000157000, "VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO"),
    std::make_pair(1000157001, "VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO"),
    std::make_pair(1000083000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES"),
    std::make_pair(1000127000, "VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS"),
    std::make_pair(1000127001, "VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO"),
    std::make_pair(1000060000, "VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO"),
    std::make_pair(1000060003, "VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO"),
    std::make_pair(1000060004, "VK_STRUCTURE_TYPE_DEVICE_GROUP_COMMAND_BUFFER_BEGIN_INFO"),
    std::make_pair(1000060005, "VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO"),
    std::make_pair(1000060006, "VK_STRUCTURE_TYPE_DEVICE_GROUP_BIND_SPARSE_INFO"),
    std::make_pair(1000060013, "VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_DEVICE_GROUP_INFO"),
    std::make_pair(1000060014, "VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_DEVICE_GROUP_INFO"),
    std::make_pair(1000070000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GROUP_PROPERTIES"),
    std::make_pair(1000070001, "VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO"),
    std::make_pair(1000146000, "VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2"),
    std::make_pair(1000146001, "VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2"),
    std::make_pair(1000146002, "VK_STRUCTURE_TYPE_IMAGE_SPARSE_MEMORY_REQUIREMENTS_INFO_2"),
    std::make_pair(1000146003, "VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2"),
    std::make_pair(1000146004, "VK_STRUCTURE_TYPE_SPARSE_IMAGE_MEMORY_REQUIREMENTS_2"),
    std::make_pair(1000059000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2"),
    std::make_pair(1000059001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2"),
    std::make_pair(1000059002, "VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2"),
    std::make_pair(1000059003, "VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2"),
    std::make_pair(1000059004, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2"),
    std::make_pair(1000059005, "VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2"),
    std::make_pair(1000059006, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2"),
    std::make_pair(1000059007, "VK_STRUCTURE_TYPE_SPARSE_IMAGE_FORMAT_PROPERTIES_2"),
    std::make_pair(1000059008, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SPARSE_IMAGE_FORMAT_INFO_2"),
    std::make_pair(1000117000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES"),
    std::make_pair(1000117001, "VK_STRUCTURE_TYPE_RENDER_PASS_INPUT_ATTACHMENT_ASPECT_CREATE_INFO"),
    std::make_pair(1000117002, "VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO"),
    std::make_pair(1000117003, "VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_DOMAIN_ORIGIN_STATE_CREATE_INFO"),
    std::make_pair(1000053000, "VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO"),
    std::make_pair(1000053001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES"),
    std::make_pair(1000053002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES"),
    std::make_pair(1000120000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES"),
    std::make_pair(1000145000, "VK_STRUCTURE_TYPE_PROTECTED_SUBMIT_INFO"),
    std::make_pair(1000145001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_FEATURES"),
    std::make_pair(1000145002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_PROPERTIES"),
    std::make_pair(1000145003, "VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2"),
    std::make_pair(1000156000, "VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_CREATE_INFO"),
    std::make_pair(1000156001, "VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_INFO"),
    std::make_pair(1000156002, "VK_STRUCTURE_TYPE_BIND_IMAGE_PLANE_MEMORY_INFO"),
    std::make_pair(1000156003, "VK_STRUCTURE_TYPE_IMAGE_PLANE_MEMORY_REQUIREMENTS_INFO"),
    std::make_pair(1000156004, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES"),
    std::make_pair(1000156005, "VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_IMAGE_FORMAT_PROPERTIES"),
    std::make_pair(1000085000, "VK_STRUCTURE_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_CREATE_INFO"),
    std::make_pair(1000071000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_IMAGE_FORMAT_INFO"),
    std::make_pair(1000071001, "VK_STRUCTURE_TYPE_EXTERNAL_IMAGE_FORMAT_PROPERTIES"),
    std::make_pair(1000071002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_BUFFER_INFO"),
    std::make_pair(1000071003, "VK_STRUCTURE_TYPE_EXTERNAL_BUFFER_PROPERTIES"),
    std::make_pair(1000071004, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES"),
    std::make_pair(1000072000, "VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO"),
    std::make_pair(1000072001, "VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO"),
    std::make_pair(1000072002, "VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO"),
    std::make_pair(1000112000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_FENCE_INFO"),
    std::make_pair(1000112001, "VK_STRUCTURE_TYPE_EXTERNAL_FENCE_PROPERTIES"),
    std::make_pair(1000113000, "VK_STRUCTURE_TYPE_EXPORT_FENCE_CREATE_INFO"),
    std::make_pair(1000077000, "VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO"),
    std::make_pair(1000076000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SEMAPHORE_INFO"),
    std::make_pair(1000076001, "VK_STRUCTURE_TYPE_EXTERNAL_SEMAPHORE_PROPERTIES"),
    std::make_pair(1000168000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES"),
    std::make_pair(1000168001, "VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_SUPPORT"),
    std::make_pair(1000063000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES"),
    std::make_pair(49, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES"),
    std::make_pair(50, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES"),
    std::make_pair(51, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES"),
    std::make_pair(52, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES"),
    std::make_pair(1000147000, "VK_STRUCTURE_TYPE_IMAGE_FORMAT_LIST_CREATE_INFO"),
    std::make_pair(1000109000, "VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2"),
    std::make_pair(1000109001, "VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2"),
    std::make_pair(1000109002, "VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2"),
    std::make_pair(1000109003, "VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2"),
    std::make_pair(1000109004, "VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2"),
    std::make_pair(1000109005, "VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO"),
    std::make_pair(1000109006, "VK_STRUCTURE_TYPE_SUBPASS_END_INFO"),
    std::make_pair(1000177000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES"),
    std::make_pair(1000196000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES"),
    std::make_pair(1000180000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES"),
    std::make_pair(1000082000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES"),
    std::make_pair(1000197000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES"),
    std::make_pair(1000161000, "VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO"),
    std::make_pair(1000161001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES"),
    std::make_pair(1000161002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES"),
    std::make_pair(1000161003, "VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO"),
    std::make_pair(1000161004, "VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_LAYOUT_SUPPORT"),
    std::make_pair(1000199000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES"),
    std::make_pair(1000199001, "VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_DEPTH_STENCIL_RESOLVE"),
    std::make_pair(1000221000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES"),
    std::make_pair(1000246000, "VK_STRUCTURE_TYPE_IMAGE_STENCIL_USAGE_CREATE_INFO"),
    std::make_pair(1000130000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_FILTER_MINMAX_PROPERTIES"),
    std::make_pair(1000130001, "VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO"),
    std::make_pair(1000211000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES"),
    std::make_pair(1000108000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES"),
    std::make_pair(1000108001, "VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO"),
    std::make_pair(1000108002, "VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO"),
    std::make_pair(1000108003, "VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO"),
    std::make_pair(1000253000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES"),
    std::make_pair(1000175000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES"),
    std::make_pair(1000241000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES"),
    std::make_pair(1000241001, "VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_STENCIL_LAYOUT"),
    std::make_pair(1000241002, "VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_STENCIL_LAYOUT"),
    std::make_pair(1000261000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES"),
    std::make_pair(1000207000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES"),
    std::make_pair(1000207001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES"),
    std::make_pair(1000207002, "VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO"),
    std::make_pair(1000207003, "VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO"),
    std::make_pair(1000207004, "VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO"),
    std::make_pair(1000207005, "VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO"),
    std::make_pair(1000257000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES"),
    std::make_pair(1000244001, "VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO"),
    std::make_pair(1000257002, "VK_STRUCTURE_TYPE_BUFFER_OPAQUE_CAPTURE_ADDRESS_CREATE_INFO"),
    std::make_pair(1000257003, "VK_STRUCTURE_TYPE_MEMORY_OPAQUE_CAPTURE_ADDRESS_ALLOCATE_INFO"),
    std::make_pair(1000257004, "VK_STRUCTURE_TYPE_DEVICE_MEMORY_OPAQUE_CAPTURE_ADDRESS_INFO"),
    std::make_pair(1000001000, "VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR"),
    std::make_pair(1000001001, "VK_STRUCTURE_TYPE_PRESENT_INFO_KHR"),
    std::make_pair(1000060007, "VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_CAPABILITIES_KHR"),
    std::make_pair(1000060008, "VK_STRUCTURE_TYPE_IMAGE_SWAPCHAIN_CREATE_INFO_KHR"),
    std::make_pair(1000060009, "VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_SWAPCHAIN_INFO_KHR"),
    std::make_pair(1000060010, "VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR"),
    std::make_pair(1000060011, "VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_INFO_KHR"),
    std::make_pair(1000060012, "VK_STRUCTURE_TYPE_DEVICE_GROUP_SWAPCHAIN_CREATE_INFO_KHR"),
    std::make_pair(1000002000, "VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR"),
    std::make_pair(1000002001, "VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR"),
    std::make_pair(1000003000, "VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR"),
    std::make_pair(1000004000, "VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR"),
    std::make_pair(1000005000, "VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR"),
    std::make_pair(1000006000, "VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR"),
    std::make_pair(1000008000, "VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR"),
    std::make_pair(1000009000, "VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR"),
    std::make_pair(1000010000, "VK_STRUCTURE_TYPE_NATIVE_BUFFER_ANDROID"),
    std::make_pair(1000010001, "VK_STRUCTURE_TYPE_SWAPCHAIN_IMAGE_CREATE_INFO_ANDROID"),
    std::make_pair(1000010002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENTATION_PROPERTIES_ANDROID"),
    std::make_pair(1000011000, "VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT"),
    std::make_pair(1000018000, "VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_RASTERIZATION_ORDER_AMD"),
    std::make_pair(1000022000, "VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT"),
    std::make_pair(1000022001, "VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT"),
    std::make_pair(1000022002, "VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT"),
    std::make_pair(1000023000, "VK_STRUCTURE_TYPE_VIDEO_PROFILE_KHR"),
    std::make_pair(1000023001, "VK_STRUCTURE_TYPE_VIDEO_CAPABILITIES_KHR"),
    std::make_pair(1000023002, "VK_STRUCTURE_TYPE_VIDEO_PICTURE_RESOURCE_KHR"),
    std::make_pair(1000023003, "VK_STRUCTURE_TYPE_VIDEO_GET_MEMORY_PROPERTIES_KHR"),
    std::make_pair(1000023004, "VK_STRUCTURE_TYPE_VIDEO_BIND_MEMORY_KHR"),
    std::make_pair(1000023005, "VK_STRUCTURE_TYPE_VIDEO_SESSION_CREATE_INFO_KHR"),
    std::make_pair(1000023006, "VK_STRUCTURE_TYPE_VIDEO_SESSION_PARAMETERS_CREATE_INFO_KHR"),
    std::make_pair(1000023007, "VK_STRUCTURE_TYPE_VIDEO_SESSION_PARAMETERS_UPDATE_INFO_KHR"),
    std::make_pair(1000023008, "VK_STRUCTURE_TYPE_VIDEO_BEGIN_CODING_INFO_KHR"),
    std::make_pair(1000023009, "VK_STRUCTURE_TYPE_VIDEO_END_CODING_INFO_KHR"),
    std::make_pair(1000023010, "VK_STRUCTURE_TYPE_VIDEO_CODING_CONTROL_INFO_KHR"),
    std::make_pair(1000023011, "VK_STRUCTURE_TYPE_VIDEO_REFERENCE_SLOT_KHR"),
    std::make_pair(1000023012, "VK_STRUCTURE_TYPE_VIDEO_QUEUE_FAMILY_PROPERTIES_2_KHR"),
    std::make_pair(1000023013, "VK_STRUCTURE_TYPE_VIDEO_PROFILES_KHR"),
    std::make_pair(1000023014, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_FORMAT_INFO_KHR"),
    std::make_pair(1000023015, "VK_STRUCTURE_TYPE_VIDEO_FORMAT_PROPERTIES_KHR"),
    std::make_pair(1000024000, "VK_STRUCTURE_TYPE_VIDEO_DECODE_INFO_KHR"),
    std::make_pair(1000026000, "VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_IMAGE_CREATE_INFO_NV"),
    std::make_pair(1000026001, "VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_BUFFER_CREATE_INFO_NV"),
    std::make_pair(1000026002, "VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_MEMORY_ALLOCATE_INFO_NV"),
    std::make_pair(1000028000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT"),
    std::make_pair(1000028001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT"),
    std::make_pair(1000028002, "VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_STREAM_CREATE_INFO_EXT"),
    std::make_pair(1000029000, "VK_STRUCTURE_TYPE_CU_MODULE_CREATE_INFO_NVX"),
    std::make_pair(1000029001, "VK_STRUCTURE_TYPE_CU_FUNCTION_CREATE_INFO_NVX"),
    std::make_pair(1000029002, "VK_STRUCTURE_TYPE_CU_LAUNCH_INFO_NVX"),
    std::make_pair(1000030000, "VK_STRUCTURE_TYPE_IMAGE_VIEW_HANDLE_INFO_NVX"),
    std::make_pair(1000030001, "VK_STRUCTURE_TYPE_IMAGE_VIEW_ADDRESS_PROPERTIES_NVX"),
    std::make_pair(1000038000, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_CAPABILITIES_EXT"),
    std::make_pair(1000038001, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_SESSION_CREATE_INFO_EXT"),
    std::make_pair(1000038002, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_SESSION_PARAMETERS_CREATE_INFO_EXT"),
    std::make_pair(1000038003, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_SESSION_PARAMETERS_ADD_INFO_EXT"),
    std::make_pair(1000038004, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_VCL_FRAME_INFO_EXT"),
    std::make_pair(1000038005, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_DPB_SLOT_INFO_EXT"),
    std::make_pair(1000038006, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_NALU_SLICE_EXT"),
    std::make_pair(1000038007, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_EMIT_PICTURE_PARAMETERS_EXT"),
    std::make_pair(1000038008, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_PROFILE_EXT"),
    std::make_pair(1000038009, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_RATE_CONTROL_INFO_EXT"),
    std::make_pair(1000038010, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_RATE_CONTROL_LAYER_INFO_EXT"),
    std::make_pair(1000039000, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_CAPABILITIES_EXT"),
    std::make_pair(1000039001, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_SESSION_CREATE_INFO_EXT"),
    std::make_pair(1000039002, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_SESSION_PARAMETERS_CREATE_INFO_EXT"),
    std::make_pair(1000039003, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_SESSION_PARAMETERS_ADD_INFO_EXT"),
    std::make_pair(1000039004, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_VCL_FRAME_INFO_EXT"),
    std::make_pair(1000039005, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_DPB_SLOT_INFO_EXT"),
    std::make_pair(1000039006, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_NALU_SLICE_EXT"),
    std::make_pair(1000039007, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_EMIT_PICTURE_PARAMETERS_EXT"),
    std::make_pair(1000039008, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_PROFILE_EXT"),
    std::make_pair(1000039009, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_REFERENCE_LISTS_EXT"),
    std::make_pair(1000039010, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_RATE_CONTROL_INFO_EXT"),
    std::make_pair(1000039011, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_RATE_CONTROL_LAYER_INFO_EXT"),
    std::make_pair(1000040000, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_CAPABILITIES_EXT"),
    std::make_pair(1000040001, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_SESSION_CREATE_INFO_EXT"),
    std::make_pair(1000040002, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_PICTURE_INFO_EXT"),
    std::make_pair(1000040003, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_MVC_EXT"),
    std::make_pair(1000040004, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_PROFILE_EXT"),
    std::make_pair(1000040005, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_SESSION_PARAMETERS_CREATE_INFO_EXT"),
    std::make_pair(1000040006, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_SESSION_PARAMETERS_ADD_INFO_EXT"),
    std::make_pair(1000040007, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_DPB_SLOT_INFO_EXT"),
    std::make_pair(1000041000, "VK_STRUCTURE_TYPE_TEXTURE_LOD_GATHER_FORMAT_PROPERTIES_AMD"),
    std::make_pair(1000044000, "VK_STRUCTURE_TYPE_RENDERING_INFO_KHR"),
    std::make_pair(1000044001, "VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR"),
    std::make_pair(1000044002, "VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR"),
    std::make_pair(1000044003, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR"),
    std::make_pair(1000044004, "VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_RENDERING_INFO_KHR"),
    std::make_pair(1000044006, "VK_STRUCTURE_TYPE_RENDERING_FRAGMENT_SHADING_RATE_ATTACHMENT_INFO_KHR"),
    std::make_pair(1000044007, "VK_STRUCTURE_TYPE_RENDERING_FRAGMENT_DENSITY_MAP_ATTACHMENT_INFO_EXT"),
    std::make_pair(1000044008, "VK_STRUCTURE_TYPE_ATTACHMENT_SAMPLE_COUNT_INFO_AMD"),
    std::make_pair(1000044009, "VK_STRUCTURE_TYPE_MULTIVIEW_PER_VIEW_ATTRIBUTES_INFO_NVX"),
    std::make_pair(1000049000, "VK_STRUCTURE_TYPE_STREAM_DESCRIPTOR_SURFACE_CREATE_INFO_GGP"),
    std::make_pair(1000050000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CORNER_SAMPLED_IMAGE_FEATURES_NV"),
    std::make_pair(1000051000, "VK_STRUCTURE_TYPE_PRIVATE_VENDOR_INFO_RESERVED_OFFSET_0_NV"),
    std::make_pair(1000056000, "VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO_NV"),
    std::make_pair(1000056001, "VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO_NV"),
    std::make_pair(1000057000, "VK_STRUCTURE_TYPE_IMPORT_MEMORY_WIN32_HANDLE_INFO_NV"),
    std::make_pair(1000057001, "VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_NV"),
    std::make_pair(1000058000, "VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_NV"),
    std::make_pair(1000060007, "VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_CAPABILITIES_KHR"),
    std::make_pair(1000060008, "VK_STRUCTURE_TYPE_IMAGE_SWAPCHAIN_CREATE_INFO_KHR"),
    std::make_pair(1000060009, "VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_SWAPCHAIN_INFO_KHR"),
    std::make_pair(1000060010, "VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR"),
    std::make_pair(1000060011, "VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_INFO_KHR"),
    std::make_pair(1000060012, "VK_STRUCTURE_TYPE_DEVICE_GROUP_SWAPCHAIN_CREATE_INFO_KHR"),
    std::make_pair(1000061000, "VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT"),
    std::make_pair(1000062000, "VK_STRUCTURE_TYPE_VI_SURFACE_CREATE_INFO_NN"),
    std::make_pair(1000066000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXTURE_COMPRESSION_ASTC_HDR_FEATURES_EXT"),
    std::make_pair(1000067000, "VK_STRUCTURE_TYPE_IMAGE_VIEW_ASTC_DECODE_MODE_EXT"),
    std::make_pair(1000067001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ASTC_DECODE_FEATURES_EXT"),
    std::make_pair(1000073000, "VK_STRUCTURE_TYPE_IMPORT_MEMORY_WIN32_HANDLE_INFO_KHR"),
    std::make_pair(1000073001, "VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_KHR"),
    std::make_pair(1000073002, "VK_STRUCTURE_TYPE_MEMORY_WIN32_HANDLE_PROPERTIES_KHR"),
    std::make_pair(1000073003, "VK_STRUCTURE_TYPE_MEMORY_GET_WIN32_HANDLE_INFO_KHR"),
    std::make_pair(1000074000, "VK_STRUCTURE_TYPE_IMPORT_MEMORY_FD_INFO_KHR"),
    std::make_pair(1000074001, "VK_STRUCTURE_TYPE_MEMORY_FD_PROPERTIES_KHR"),
    std::make_pair(1000074002, "VK_STRUCTURE_TYPE_MEMORY_GET_FD_INFO_KHR"),
    std::make_pair(1000075000, "VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_KHR"),
    std::make_pair(1000078000, "VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR"),
    std::make_pair(1000078001, "VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR"),
    std::make_pair(1000078002, "VK_STRUCTURE_TYPE_D3D12_FENCE_SUBMIT_INFO_KHR"),
    std::make_pair(1000078003, "VK_STRUCTURE_TYPE_SEMAPHORE_GET_WIN32_HANDLE_INFO_KHR"),
    std::make_pair(1000079000, "VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_FD_INFO_KHR"),
    std::make_pair(1000079001, "VK_STRUCTURE_TYPE_SEMAPHORE_GET_FD_INFO_KHR"),
    std::make_pair(1000080000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR"),
    std::make_pair(1000081000, "VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_CONDITIONAL_RENDERING_INFO_EXT"),
    std::make_pair(1000081001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT"),
    std::make_pair(1000081002, "VK_STRUCTURE_TYPE_CONDITIONAL_RENDERING_BEGIN_INFO_EXT"),
    std::make_pair(1000084000, "VK_STRUCTURE_TYPE_PRESENT_REGIONS_KHR"),
    std::make_pair(1000087000, "VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_W_SCALING_STATE_CREATE_INFO_NV"),
    std::make_pair(1000090000, "VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_EXT"),
    std::make_pair(1000091000, "VK_STRUCTURE_TYPE_DISPLAY_POWER_INFO_EXT"),
    std::make_pair(1000091001, "VK_STRUCTURE_TYPE_DEVICE_EVENT_INFO_EXT"),
    std::make_pair(1000091002, "VK_STRUCTURE_TYPE_DISPLAY_EVENT_INFO_EXT"),
    std::make_pair(1000091003, "VK_STRUCTURE_TYPE_SWAPCHAIN_COUNTER_CREATE_INFO_EXT"),
    std::make_pair(1000092000, "VK_STRUCTURE_TYPE_PRESENT_TIMES_INFO_GOOGLE"),
    std::make_pair(1000097000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PER_VIEW_ATTRIBUTES_PROPERTIES_NVX"),
    std::make_pair(1000098000, "VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_SWIZZLE_STATE_CREATE_INFO_NV"),
    std::make_pair(1000099000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISCARD_RECTANGLE_PROPERTIES_EXT"),
    std::make_pair(1000099001, "VK_STRUCTURE_TYPE_PIPELINE_DISCARD_RECTANGLE_STATE_CREATE_INFO_EXT"),
    std::make_pair(1000101000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT"),
    std::make_pair(1000101001, "VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT"),
    std::make_pair(1000102000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT"),
    std::make_pair(1000102001, "VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_DEPTH_CLIP_STATE_CREATE_INFO_EXT"),
    std::make_pair(1000105000, "VK_STRUCTURE_TYPE_HDR_METADATA_EXT"),
    std::make_pair(1000111000, "VK_STRUCTURE_TYPE_SHARED_PRESENT_SURFACE_CAPABILITIES_KHR"),
    std::make_pair(1000114000, "VK_STRUCTURE_TYPE_IMPORT_FENCE_WIN32_HANDLE_INFO_KHR"),
    std::make_pair(1000114001, "VK_STRUCTURE_TYPE_EXPORT_FENCE_WIN32_HANDLE_INFO_KHR"),
    std::make_pair(1000114002, "VK_STRUCTURE_TYPE_FENCE_GET_WIN32_HANDLE_INFO_KHR"),
    std::make_pair(1000115000, "VK_STRUCTURE_TYPE_IMPORT_FENCE_FD_INFO_KHR"),
    std::make_pair(1000115001, "VK_STRUCTURE_TYPE_FENCE_GET_FD_INFO_KHR"),
    std::make_pair(1000116000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR"),
    std::make_pair(1000116001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_PROPERTIES_KHR"),
    std::make_pair(1000116002, "VK_STRUCTURE_TYPE_QUERY_POOL_PERFORMANCE_CREATE_INFO_KHR"),
    std::make_pair(1000116003, "VK_STRUCTURE_TYPE_PERFORMANCE_QUERY_SUBMIT_INFO_KHR"),
    std::make_pair(1000116004, "VK_STRUCTURE_TYPE_ACQUIRE_PROFILING_LOCK_INFO_KHR"),
    std::make_pair(1000116005, "VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_KHR"),
    std::make_pair(1000116006, "VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_DESCRIPTION_KHR"),
    std::make_pair(1000116007, "VK_STRUCTURE_TYPE_PERFORMANCE_QUERY_RESERVATION_INFO_KHR"),
    std::make_pair(1000119000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR"),
    std::make_pair(1000119001, "VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR"),
    std::make_pair(1000119002, "VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR"),
    std::make_pair(1000121000, "VK_STRUCTURE_TYPE_DISPLAY_PROPERTIES_2_KHR"),
    std::make_pair(1000121001, "VK_STRUCTURE_TYPE_DISPLAY_PLANE_PROPERTIES_2_KHR"),
    std::make_pair(1000121002, "VK_STRUCTURE_TYPE_DISPLAY_MODE_PROPERTIES_2_KHR"),
    std::make_pair(1000121003, "VK_STRUCTURE_TYPE_DISPLAY_PLANE_INFO_2_KHR"),
    std::make_pair(1000121004, "VK_STRUCTURE_TYPE_DISPLAY_PLANE_CAPABILITIES_2_KHR"),
    std::make_pair(1000122000, "VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK"),
    std::make_pair(1000123000, "VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK"),
    std::make_pair(1000128000, "VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT"),
    std::make_pair(1000128001, "VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT"),
    std::make_pair(1000128002, "VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT"),
    std::make_pair(1000128003, "VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT"),
    std::make_pair(1000128004, "VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT"),
    std::make_pair(1000129000, "VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_USAGE_ANDROID"),
    std::make_pair(1000129001, "VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_PROPERTIES_ANDROID"),
    std::make_pair(1000129002, "VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_FORMAT_PROPERTIES_ANDROID"),
    std::make_pair(1000129003, "VK_STRUCTURE_TYPE_IMPORT_ANDROID_HARDWARE_BUFFER_INFO_ANDROID"),
    std::make_pair(1000129004, "VK_STRUCTURE_TYPE_MEMORY_GET_ANDROID_HARDWARE_BUFFER_INFO_ANDROID"),
    std::make_pair(1000129005, "VK_STRUCTURE_TYPE_EXTERNAL_FORMAT_ANDROID"),
    std::make_pair(1000129006, "VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_FORMAT_PROPERTIES_2_ANDROID"),
    std::make_pair(1000138000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT"),
    std::make_pair(1000138001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT"),
    std::make_pair(1000138002, "VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT"),
    std::make_pair(1000138003, "VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_INLINE_UNIFORM_BLOCK_CREATE_INFO_EXT"),
    std::make_pair(1000143000, "VK_STRUCTURE_TYPE_SAMPLE_LOCATIONS_INFO_EXT"),
    std::make_pair(1000143001, "VK_STRUCTURE_TYPE_RENDER_PASS_SAMPLE_LOCATIONS_BEGIN_INFO_EXT"),
    std::make_pair(1000143002, "VK_STRUCTURE_TYPE_PIPELINE_SAMPLE_LOCATIONS_STATE_CREATE_INFO_EXT"),
    std::make_pair(1000143003, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLE_LOCATIONS_PROPERTIES_EXT"),
    std::make_pair(1000143004, "VK_STRUCTURE_TYPE_MULTISAMPLE_PROPERTIES_EXT"),
    std::make_pair(1000148000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT"),
    std::make_pair(1000148001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT"),
    std::make_pair(1000148002, "VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_ADVANCED_STATE_CREATE_INFO_EXT"),
    std::make_pair(1000149000, "VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_TO_COLOR_STATE_CREATE_INFO_NV"),
    std::make_pair(1000150007, "VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR"),
    std::make_pair(1000150000, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR"),
    std::make_pair(1000150002, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR"),
    std::make_pair(1000150003, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR"),
    std::make_pair(1000150004, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR"),
    std::make_pair(1000150005, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR"),
    std::make_pair(1000150006, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR"),
    std::make_pair(1000150009, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_VERSION_INFO_KHR"),
    std::make_pair(1000150010, "VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR"),
    std::make_pair(1000150011, "VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR"),
    std::make_pair(1000150012, "VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR"),
    std::make_pair(1000150013, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR"),
    std::make_pair(1000150014, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR"),
    std::make_pair(1000150017, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR"),
    std::make_pair(1000150020, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR"),
    std::make_pair(1000347000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR"),
    std::make_pair(1000347001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR"),
    std::make_pair(1000150015, "VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR"),
    std::make_pair(1000150016, "VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR"),
    std::make_pair(1000150018, "VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_INTERFACE_CREATE_INFO_KHR"),
    std::make_pair(1000348013, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR"),
    std::make_pair(1000152000, "VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_MODULATION_STATE_CREATE_INFO_NV"),
    std::make_pair(1000154000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_FEATURES_NV"),
    std::make_pair(1000154001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_PROPERTIES_NV"),
    std::make_pair(1000158000, "VK_STRUCTURE_TYPE_DRM_FORMAT_MODIFIER_PROPERTIES_LIST_EXT"),
    std::make_pair(1000158002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_DRM_FORMAT_MODIFIER_INFO_EXT"),
    std::make_pair(1000158003, "VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_LIST_CREATE_INFO_EXT"),
    std::make_pair(1000158004, "VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_EXPLICIT_CREATE_INFO_EXT"),
    std::make_pair(1000158005, "VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_PROPERTIES_EXT"),
    std::make_pair(1000158006, "VK_STRUCTURE_TYPE_DRM_FORMAT_MODIFIER_PROPERTIES_LIST_2_EXT"),
    std::make_pair(1000160000, "VK_STRUCTURE_TYPE_VALIDATION_CACHE_CREATE_INFO_EXT"),
    std::make_pair(1000160001, "VK_STRUCTURE_TYPE_SHADER_MODULE_VALIDATION_CACHE_CREATE_INFO_EXT"),
    std::make_pair(1000163000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR"),
    std::make_pair(1000163001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_KHR"),
    std::make_pair(1000164000, "VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_SHADING_RATE_IMAGE_STATE_CREATE_INFO_NV"),
    std::make_pair(1000164001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_FEATURES_NV"),
    std::make_pair(1000164002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_PROPERTIES_NV"),
    std::make_pair(1000164005, "VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_COARSE_SAMPLE_ORDER_STATE_CREATE_INFO_NV"),
    std::make_pair(1000165000, "VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV"),
    std::make_pair(1000165001, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NV"),
    std::make_pair(1000165003, "VK_STRUCTURE_TYPE_GEOMETRY_NV"),
    std::make_pair(1000165004, "VK_STRUCTURE_TYPE_GEOMETRY_TRIANGLES_NV"),
    std::make_pair(1000165005, "VK_STRUCTURE_TYPE_GEOMETRY_AABB_NV"),
    std::make_pair(1000165006, "VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NV"),
    std::make_pair(1000165007, "VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV"),
    std::make_pair(1000165008, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NV"),
    std::make_pair(1000165009, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV"),
    std::make_pair(1000165011, "VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV"),
    std::make_pair(1000165012, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_INFO_NV"),
    std::make_pair(1000166000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_REPRESENTATIVE_FRAGMENT_TEST_FEATURES_NV"),
    std::make_pair(1000166001, "VK_STRUCTURE_TYPE_PIPELINE_REPRESENTATIVE_FRAGMENT_TEST_STATE_CREATE_INFO_NV"),
    std::make_pair(1000170000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_IMAGE_FORMAT_INFO_EXT"),
    std::make_pair(1000170001, "VK_STRUCTURE_TYPE_FILTER_CUBIC_IMAGE_VIEW_IMAGE_FORMAT_PROPERTIES_EXT"),
    std::make_pair(1000174000, "VK_STRUCTURE_TYPE_DEVICE_QUEUE_GLOBAL_PRIORITY_CREATE_INFO_EXT"),
    std::make_pair(1000178000, "VK_STRUCTURE_TYPE_IMPORT_MEMORY_HOST_POINTER_INFO_EXT"),
    std::make_pair(1000178001, "VK_STRUCTURE_TYPE_MEMORY_HOST_POINTER_PROPERTIES_EXT"),
    std::make_pair(1000178002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT"),
    std::make_pair(1000181000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR"),
    std::make_pair(1000183000, "VK_STRUCTURE_TYPE_PIPELINE_COMPILER_CONTROL_CREATE_INFO_AMD"),
    std::make_pair(1000184000, "VK_STRUCTURE_TYPE_CALIBRATED_TIMESTAMP_INFO_EXT"),
    std::make_pair(1000185000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_AMD"),
    std::make_pair(1000187000, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_CAPABILITIES_EXT"),
    std::make_pair(1000187001, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_SESSION_CREATE_INFO_EXT"),
    std::make_pair(1000187002, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_SESSION_PARAMETERS_CREATE_INFO_EXT"),
    std::make_pair(1000187003, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_SESSION_PARAMETERS_ADD_INFO_EXT"),
    std::make_pair(1000187004, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_PROFILE_EXT"),
    std::make_pair(1000187005, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_PICTURE_INFO_EXT"),
    std::make_pair(1000187006, "VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_DPB_SLOT_INFO_EXT"),
    std::make_pair(1000189000, "VK_STRUCTURE_TYPE_DEVICE_MEMORY_OVERALLOCATION_CREATE_INFO_AMD"),
    std::make_pair(1000190000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT"),
    std::make_pair(1000190001, "VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT"),
    std::make_pair(1000190002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT"),
    std::make_pair(1000191000, "VK_STRUCTURE_TYPE_PRESENT_FRAME_TOKEN_GGP"),
    std::make_pair(1000192000, "VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT"),
    std::make_pair(1000201000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_NV"),
    std::make_pair(1000202000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV"),
    std::make_pair(1000202001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_NV"),
    std::make_pair(1000203000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_FEATURES_NV"),
    std::make_pair(1000204000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_FOOTPRINT_FEATURES_NV"),
    std::make_pair(1000205000, "VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_EXCLUSIVE_SCISSOR_STATE_CREATE_INFO_NV"),
    std::make_pair(1000205002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXCLUSIVE_SCISSOR_FEATURES_NV"),
    std::make_pair(1000206000, "VK_STRUCTURE_TYPE_CHECKPOINT_DATA_NV"),
    std::make_pair(1000206001, "VK_STRUCTURE_TYPE_QUEUE_FAMILY_CHECKPOINT_PROPERTIES_NV"),
    std::make_pair(1000209000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_FUNCTIONS_2_FEATURES_INTEL"),
    std::make_pair(1000210000, "VK_STRUCTURE_TYPE_QUERY_POOL_PERFORMANCE_QUERY_CREATE_INFO_INTEL"),
    std::make_pair(1000210001, "VK_STRUCTURE_TYPE_INITIALIZE_PERFORMANCE_API_INFO_INTEL"),
    std::make_pair(1000210002, "VK_STRUCTURE_TYPE_PERFORMANCE_MARKER_INFO_INTEL"),
    std::make_pair(1000210003, "VK_STRUCTURE_TYPE_PERFORMANCE_STREAM_MARKER_INFO_INTEL"),
    std::make_pair(1000210004, "VK_STRUCTURE_TYPE_PERFORMANCE_OVERRIDE_INFO_INTEL"),
    std::make_pair(1000210005, "VK_STRUCTURE_TYPE_PERFORMANCE_CONFIGURATION_ACQUIRE_INFO_INTEL"),
    std::make_pair(1000212000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT"),
    std::make_pair(1000213000, "VK_STRUCTURE_TYPE_DISPLAY_NATIVE_HDR_SURFACE_CAPABILITIES_AMD"),
    std::make_pair(1000213001, "VK_STRUCTURE_TYPE_SWAPCHAIN_DISPLAY_NATIVE_HDR_CREATE_INFO_AMD"),
    std::make_pair(1000214000, "VK_STRUCTURE_TYPE_IMAGEPIPE_SURFACE_CREATE_INFO_FUCHSIA"),
    std::make_pair(1000215000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TERMINATE_INVOCATION_FEATURES_KHR"),
    std::make_pair(1000217000, "VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT"),
    std::make_pair(1000218000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_FEATURES_EXT"),
    std::make_pair(1000218001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_PROPERTIES_EXT"),
    std::make_pair(1000218002, "VK_STRUCTURE_TYPE_RENDER_PASS_FRAGMENT_DENSITY_MAP_CREATE_INFO_EXT"),
    std::make_pair(1000225000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT"),
    std::make_pair(1000225001, "VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_REQUIRED_SUBGROUP_SIZE_CREATE_INFO_EXT"),
    std::make_pair(1000225002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES_EXT"),
    std::make_pair(1000226000, "VK_STRUCTURE_TYPE_FRAGMENT_SHADING_RATE_ATTACHMENT_INFO_KHR"),
    std::make_pair(1000226001, "VK_STRUCTURE_TYPE_PIPELINE_FRAGMENT_SHADING_RATE_STATE_CREATE_INFO_KHR"),
    std::make_pair(1000226002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_PROPERTIES_KHR"),
    std::make_pair(1000226003, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR"),
    std::make_pair(1000226004, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_KHR"),
    std::make_pair(1000227000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_2_AMD"),
    std::make_pair(1000229000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COHERENT_MEMORY_FEATURES_AMD"),
    std::make_pair(1000234000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT"),
    std::make_pair(1000237000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT"),
    std::make_pair(1000238000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT"),
    std::make_pair(1000238001, "VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT"),
    std::make_pair(1000239000, "VK_STRUCTURE_TYPE_SURFACE_PROTECTED_CAPABILITIES_KHR"),
    std::make_pair(1000240000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEDICATED_ALLOCATION_IMAGE_ALIASING_FEATURES_NV"),
    std::make_pair(1000244000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_EXT"),
    std::make_pair(1000244002, "VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_CREATE_INFO_EXT"),
    std::make_pair(1000245000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TOOL_PROPERTIES_EXT"),
    std::make_pair(1000247000, "VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT"),
    std::make_pair(1000248000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_WAIT_FEATURES_KHR"),
    std::make_pair(1000249000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_FEATURES_NV"),
    std::make_pair(1000249001, "VK_STRUCTURE_TYPE_COOPERATIVE_MATRIX_PROPERTIES_NV"),
    std::make_pair(1000249002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_PROPERTIES_NV"),
    std::make_pair(1000250000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COVERAGE_REDUCTION_MODE_FEATURES_NV"),
    std::make_pair(1000250001, "VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_REDUCTION_STATE_CREATE_INFO_NV"),
    std::make_pair(1000250002, "VK_STRUCTURE_TYPE_FRAMEBUFFER_MIXED_SAMPLES_COMBINATION_NV"),
    std::make_pair(1000251000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT"),
    std::make_pair(1000252000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_IMAGE_ARRAYS_FEATURES_EXT"),
    std::make_pair(1000254000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROVOKING_VERTEX_FEATURES_EXT"),
    std::make_pair(1000254001, "VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_PROVOKING_VERTEX_STATE_CREATE_INFO_EXT"),
    std::make_pair(1000254002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROVOKING_VERTEX_PROPERTIES_EXT"),
    std::make_pair(1000255000, "VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT"),
    std::make_pair(1000255002, "VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_FULL_SCREEN_EXCLUSIVE_EXT"),
    std::make_pair(1000255001, "VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT"),
    std::make_pair(1000256000, "VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT"),
    std::make_pair(1000259000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_FEATURES_EXT"),
    std::make_pair(1000259001, "VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_LINE_STATE_CREATE_INFO_EXT"),
    std::make_pair(1000259002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_PROPERTIES_EXT"),
    std::make_pair(1000260000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_FEATURES_EXT"),
    std::make_pair(1000265000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES_EXT"),
    std::make_pair(1000267000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT"),
    std::make_pair(1000269000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_EXECUTABLE_PROPERTIES_FEATURES_KHR"),
    std::make_pair(1000269001, "VK_STRUCTURE_TYPE_PIPELINE_INFO_KHR"),
    std::make_pair(1000269002, "VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_PROPERTIES_KHR"),
    std::make_pair(1000269003, "VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INFO_KHR"),
    std::make_pair(1000269004, "VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_STATISTIC_KHR"),
    std::make_pair(1000269005, "VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INTERNAL_REPRESENTATION_KHR"),
    std::make_pair(1000273000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_2_FEATURES_EXT"),
    std::make_pair(1000276000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES_EXT"),
    std::make_pair(1000277000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_PROPERTIES_NV"),
    std::make_pair(1000277001, "VK_STRUCTURE_TYPE_GRAPHICS_SHADER_GROUP_CREATE_INFO_NV"),
    std::make_pair(1000277002, "VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_SHADER_GROUPS_CREATE_INFO_NV"),
    std::make_pair(1000277003, "VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV"),
    std::make_pair(1000277004, "VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_CREATE_INFO_NV"),
    std::make_pair(1000277005, "VK_STRUCTURE_TYPE_GENERATED_COMMANDS_INFO_NV"),
    std::make_pair(1000277006, "VK_STRUCTURE_TYPE_GENERATED_COMMANDS_MEMORY_REQUIREMENTS_INFO_NV"),
    std::make_pair(1000277007, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_FEATURES_NV"),
    std::make_pair(1000278000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INHERITED_VIEWPORT_SCISSOR_FEATURES_NV"),
    std::make_pair(1000278001, "VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_VIEWPORT_SCISSOR_INFO_NV"),
    std::make_pair(1000280000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_FEATURES_KHR"),
    std::make_pair(1000280001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_PROPERTIES_KHR"),
    std::make_pair(1000281000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT"),
    std::make_pair(1000281001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT"),
    std::make_pair(1000282000, "VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_RENDER_PASS_TRANSFORM_INFO_QCOM"),
    std::make_pair(1000282001, "VK_STRUCTURE_TYPE_RENDER_PASS_TRANSFORM_BEGIN_INFO_QCOM"),
    std::make_pair(1000284000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_MEMORY_REPORT_FEATURES_EXT"),
    std::make_pair(1000284001, "VK_STRUCTURE_TYPE_DEVICE_DEVICE_MEMORY_REPORT_CREATE_INFO_EXT"),
    std::make_pair(1000284002, "VK_STRUCTURE_TYPE_DEVICE_MEMORY_REPORT_CALLBACK_DATA_EXT"),
    std::make_pair(1000286000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT"),
    std::make_pair(1000286001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT"),
    std::make_pair(1000287000, "VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT"),
    std::make_pair(1000287001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT"),
    std::make_pair(1000287002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT"),
    std::make_pair(1000290000, "VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR"),
    std::make_pair(1000294000, "VK_STRUCTURE_TYPE_PRESENT_ID_KHR"),
    std::make_pair(1000294001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_ID_FEATURES_KHR"),
    std::make_pair(1000295000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIVATE_DATA_FEATURES_EXT"),
    std::make_pair(1000295001, "VK_STRUCTURE_TYPE_DEVICE_PRIVATE_DATA_CREATE_INFO_EXT"),
    std::make_pair(1000295002, "VK_STRUCTURE_TYPE_PRIVATE_DATA_SLOT_CREATE_INFO_EXT"),
    std::make_pair(1000297000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_CREATION_CACHE_CONTROL_FEATURES_EXT"),
    std::make_pair(1000299000, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_INFO_KHR"),
    std::make_pair(1000299001, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_RATE_CONTROL_INFO_KHR"),
    std::make_pair(1000299002, "VK_STRUCTURE_TYPE_VIDEO_ENCODE_RATE_CONTROL_LAYER_INFO_KHR"),
    std::make_pair(1000300000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DIAGNOSTICS_CONFIG_FEATURES_NV"),
    std::make_pair(1000300001, "VK_STRUCTURE_TYPE_DEVICE_DIAGNOSTICS_CONFIG_CREATE_INFO_NV"),
    std::make_pair(1000308000, "VK_STRUCTURE_TYPE_REFRESH_OBJECT_LIST_KHR"),
    std::make_pair(1000309000, "VK_STRUCTURE_TYPE_RESERVED_QCOM"),
    std::make_pair(1000314000, "VK_STRUCTURE_TYPE_MEMORY_BARRIER_2_KHR"),
    std::make_pair(1000314001, "VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2_KHR"),
    std::make_pair(1000314002, "VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR"),
    std::make_pair(1000314003, "VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR"),
    std::make_pair(1000314004, "VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR"),
    std::make_pair(1000314005, "VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR"),
    std::make_pair(1000314006, "VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR"),
    std::make_pair(1000314007, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR"),
    std::make_pair(1000314008, "VK_STRUCTURE_TYPE_QUEUE_FAMILY_CHECKPOINT_PROPERTIES_2_NV"),
    std::make_pair(1000314009, "VK_STRUCTURE_TYPE_CHECKPOINT_DATA_2_NV"),
    std::make_pair(1000323000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_FEATURES_KHR"),
    std::make_pair(1000325000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ZERO_INITIALIZE_WORKGROUP_MEMORY_FEATURES_KHR"),
    std::make_pair(1000326000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_PROPERTIES_NV"),
    std::make_pair(1000326001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_FEATURES_NV"),
    std::make_pair(1000326002, "VK_STRUCTURE_TYPE_PIPELINE_FRAGMENT_SHADING_RATE_ENUM_STATE_CREATE_INFO_NV"),
    std::make_pair(1000327000, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_MOTION_TRIANGLES_DATA_NV"),
    std::make_pair(1000327001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_MOTION_BLUR_FEATURES_NV"),
    std::make_pair(1000327002, "VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MOTION_INFO_NV"),
    std::make_pair(1000330000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_2_PLANE_444_FORMATS_FEATURES_EXT"),
    std::make_pair(1000332000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_FEATURES_EXT"),
    std::make_pair(1000332001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_PROPERTIES_EXT"),
    std::make_pair(1000333000, "VK_STRUCTURE_TYPE_COPY_COMMAND_TRANSFORM_INFO_QCOM"),
    std::make_pair(1000335000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES_EXT"),
    std::make_pair(1000336000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_FEATURES_KHR"),
    std::make_pair(1000337000, "VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2_KHR"),
    std::make_pair(1000337001, "VK_STRUCTURE_TYPE_COPY_IMAGE_INFO_2_KHR"),
    std::make_pair(1000337002, "VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2_KHR"),
    std::make_pair(1000337003, "VK_STRUCTURE_TYPE_COPY_IMAGE_TO_BUFFER_INFO_2_KHR"),
    std::make_pair(1000337004, "VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2_KHR"),
    std::make_pair(1000337005, "VK_STRUCTURE_TYPE_RESOLVE_IMAGE_INFO_2_KHR"),
    std::make_pair(1000337006, "VK_STRUCTURE_TYPE_BUFFER_COPY_2_KHR"),
    std::make_pair(1000337007, "VK_STRUCTURE_TYPE_IMAGE_COPY_2_KHR"),
    std::make_pair(1000337008, "VK_STRUCTURE_TYPE_IMAGE_BLIT_2_KHR"),
    std::make_pair(1000337009, "VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2_KHR"),
    std::make_pair(1000337010, "VK_STRUCTURE_TYPE_IMAGE_RESOLVE_2_KHR"),
    std::make_pair(1000340000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_4444_FORMATS_FEATURES_EXT"),
    std::make_pair(1000342000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_FEATURES_ARM"),
    std::make_pair(1000344000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RGBA10X6_FORMATS_FEATURES_EXT"),
    std::make_pair(1000346000, "VK_STRUCTURE_TYPE_DIRECTFB_SURFACE_CREATE_INFO_EXT"),
    std::make_pair(1000351000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_VALVE"),
    std::make_pair(1000351002, "VK_STRUCTURE_TYPE_MUTABLE_DESCRIPTOR_TYPE_CREATE_INFO_VALVE"),
    std::make_pair(1000352000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT"),
    std::make_pair(1000352001, "VK_STRUCTURE_TYPE_VERTEX_INPUT_BINDING_DESCRIPTION_2_EXT"),
    std::make_pair(1000352002, "VK_STRUCTURE_TYPE_VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_2_EXT"),
    std::make_pair(1000353000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRM_PROPERTIES_EXT"),
    std::make_pair(1000355000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_CONTROL_FEATURES_EXT"),
    std::make_pair(1000355001, "VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_DEPTH_CLIP_CONTROL_CREATE_INFO_EXT"),
    std::make_pair(1000356000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIMITIVE_TOPOLOGY_LIST_RESTART_FEATURES_EXT"),
    std::make_pair(1000360000, "VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3_KHR"),
    std::make_pair(1000364000, "VK_STRUCTURE_TYPE_IMPORT_MEMORY_ZIRCON_HANDLE_INFO_FUCHSIA"),
    std::make_pair(1000364001, "VK_STRUCTURE_TYPE_MEMORY_ZIRCON_HANDLE_PROPERTIES_FUCHSIA"),
    std::make_pair(1000364002, "VK_STRUCTURE_TYPE_MEMORY_GET_ZIRCON_HANDLE_INFO_FUCHSIA"),
    std::make_pair(1000365000, "VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_ZIRCON_HANDLE_INFO_FUCHSIA"),
    std::make_pair(1000365001, "VK_STRUCTURE_TYPE_SEMAPHORE_GET_ZIRCON_HANDLE_INFO_FUCHSIA"),
    std::make_pair(1000366000, "VK_STRUCTURE_TYPE_BUFFER_COLLECTION_CREATE_INFO_FUCHSIA"),
    std::make_pair(1000366001, "VK_STRUCTURE_TYPE_IMPORT_MEMORY_BUFFER_COLLECTION_FUCHSIA"),
    std::make_pair(1000366002, "VK_STRUCTURE_TYPE_BUFFER_COLLECTION_IMAGE_CREATE_INFO_FUCHSIA"),
    std::make_pair(1000366003, "VK_STRUCTURE_TYPE_BUFFER_COLLECTION_PROPERTIES_FUCHSIA"),
    std::make_pair(1000366004, "VK_STRUCTURE_TYPE_BUFFER_CONSTRAINTS_INFO_FUCHSIA"),
    std::make_pair(1000366005, "VK_STRUCTURE_TYPE_BUFFER_COLLECTION_BUFFER_CREATE_INFO_FUCHSIA"),
    std::make_pair(1000366006, "VK_STRUCTURE_TYPE_IMAGE_CONSTRAINTS_INFO_FUCHSIA"),
    std::make_pair(1000366007, "VK_STRUCTURE_TYPE_IMAGE_FORMAT_CONSTRAINTS_INFO_FUCHSIA"),
    std::make_pair(1000366008, "VK_STRUCTURE_TYPE_SYSMEM_COLOR_SPACE_FUCHSIA"),
    std::make_pair(1000366009, "VK_STRUCTURE_TYPE_BUFFER_COLLECTION_CONSTRAINTS_INFO_FUCHSIA"),
    std::make_pair(1000369000, "VK_STRUCTURE_TYPE_SUBPASS_SHADING_PIPELINE_CREATE_INFO_HUAWEI"),
    std::make_pair(1000369001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_SHADING_FEATURES_HUAWEI"),
    std::make_pair(1000369002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_SHADING_PROPERTIES_HUAWEI"),
    std::make_pair(1000370000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INVOCATION_MASK_FEATURES_HUAWEI"),
    std::make_pair(1000371000, "VK_STRUCTURE_TYPE_MEMORY_GET_REMOTE_ADDRESS_INFO_NV"),
    std::make_pair(1000371001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_RDMA_FEATURES_NV"),
    std::make_pair(1000373000, "VK_STRUCTURE_TYPE_IMPORT_FENCE_SCI_SYNC_INFO_NV"),
    std::make_pair(1000373001, "VK_STRUCTURE_TYPE_EXPORT_FENCE_SCI_SYNC_INFO_NV"),
    std::make_pair(1000373002, "VK_STRUCTURE_TYPE_FENCE_GET_SCI_SYNC_INFO_NV"),
    std::make_pair(1000373003, "VK_STRUCTURE_TYPE_SCI_SYNC_ATTRIBUTES_INFO_NV"),
    std::make_pair(1000373004, "VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_SCI_SYNC_INFO_NV"),
    std::make_pair(1000373005, "VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_SCI_SYNC_INFO_NV"),
    std::make_pair(1000373006, "VK_STRUCTURE_TYPE_SEMAPHORE_GET_SCI_SYNC_INFO_NV"),
    std::make_pair(1000373007, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SCI_SYNC_FEATURES_NV"),
    std::make_pair(1000374000, "VK_STRUCTURE_TYPE_IMPORT_MEMORY_SCI_BUF_INFO_NV"),
    std::make_pair(1000374001, "VK_STRUCTURE_TYPE_EXPORT_MEMORY_SCI_BUF_INFO_NV"),
    std::make_pair(1000374002, "VK_STRUCTURE_TYPE_MEMORY_GET_SCI_BUF_INFO_NV"),
    std::make_pair(1000374003, "VK_STRUCTURE_TYPE_MEMORY_SCI_BUF_PROPERTIES_NV"),
    std::make_pair(1000374004, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SCI_BUF_FEATURES_NV"),
    std::make_pair(1000377000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT"),
    std::make_pair(1000378000, "VK_STRUCTURE_TYPE_SCREEN_SURFACE_CREATE_INFO_QNX"),
    std::make_pair(1000381000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COLOR_WRITE_ENABLE_FEATURES_EXT"),
    std::make_pair(1000381001, "VK_STRUCTURE_TYPE_PIPELINE_COLOR_WRITE_CREATE_INFO_EXT"),
    std::make_pair(1000388000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GLOBAL_PRIORITY_QUERY_FEATURES_EXT"),
    std::make_pair(1000388001, "VK_STRUCTURE_TYPE_QUEUE_FAMILY_GLOBAL_PRIORITY_PROPERTIES_EXT"),
    std::make_pair(1000391000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_MIN_LOD_FEATURES_EXT"),
    std::make_pair(1000391001, "VK_STRUCTURE_TYPE_IMAGE_VIEW_MIN_LOD_CREATE_INFO_EXT"),
    std::make_pair(1000392000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTI_DRAW_FEATURES_EXT"),
    std::make_pair(1000392001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTI_DRAW_PROPERTIES_EXT"),
    std::make_pair(1000411000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BORDER_COLOR_SWIZZLE_FEATURES_EXT"),
    std::make_pair(1000411001, "VK_STRUCTURE_TYPE_SAMPLER_BORDER_COLOR_COMPONENT_MAPPING_CREATE_INFO_EXT"),
    std::make_pair(1000412000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PAGEABLE_DEVICE_LOCAL_MEMORY_FEATURES_EXT"),
    std::make_pair(1000413000, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES_KHR"),
    std::make_pair(1000413001, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES_KHR"),
    std::make_pair(1000413002, "VK_STRUCTURE_TYPE_DEVICE_BUFFER_MEMORY_REQUIREMENTS_KHR"),
    std::make_pair(1000413003, "VK_STRUCTURE_TYPE_DEVICE_IMAGE_MEMORY_REQUIREMENTS_KHR"),
    std::make_pair(1000435000, "VK_STRUCTURE_TYPE_APPLICATION_PARAMETERS_EXT"),
    std::make_pair(1000489000, "VK_STRUCTURE_TYPE_SEMAPHORE_SCI_SYNC_POOL_CREATE_INFO_NV"),
    std::make_pair(1000489001, "VK_STRUCTURE_TYPE_SEMAPHORE_SCI_SYNC_CREATE_INFO_NV"),
    std::make_pair(1000489002, "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SCI_SYNC_2_FEATURES_NV"),
    std::make_pair(1000373000, "VK_STRUCTURE_TYPE_IMPORT_FENCE_SCI_SYNC_INFO_NV"),
    std::make_pair(1000373001, "VK_STRUCTURE_TYPE_EXPORT_FENCE_SCI_SYNC_INFO_NV"),
    std::make_pair(1000373002, "VK_STRUCTURE_TYPE_FENCE_GET_SCI_SYNC_INFO_NV"),
    std::make_pair(1000373003, "VK_STRUCTURE_TYPE_SCI_SYNC_ATTRIBUTES_INFO_NV"),
    std::make_pair(1000489003, "VK_STRUCTURE_TYPE_DEVICE_SEMAPHORE_SCI_SYNC_POOL_RESERVATION_CREATE_INFO_NV"),
};
static void print_VkStructureType(VkStructureType obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkStructureType_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkStructureType_map[obj] << "\"" << std::endl;
}
static void print_VkStructureType(const VkStructureType * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkStructureType_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkStructureType_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkAccessFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_ACCESS_INDIRECT_COMMAND_READ_BIT"),
    std::make_pair(1ULL << 1, "VK_ACCESS_INDEX_READ_BIT"),
    std::make_pair(1ULL << 2, "VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT"),
    std::make_pair(1ULL << 3, "VK_ACCESS_UNIFORM_READ_BIT"),
    std::make_pair(1ULL << 4, "VK_ACCESS_INPUT_ATTACHMENT_READ_BIT"),
    std::make_pair(1ULL << 5, "VK_ACCESS_SHADER_READ_BIT"),
    std::make_pair(1ULL << 6, "VK_ACCESS_SHADER_WRITE_BIT"),
    std::make_pair(1ULL << 7, "VK_ACCESS_COLOR_ATTACHMENT_READ_BIT"),
    std::make_pair(1ULL << 8, "VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT"),
    std::make_pair(1ULL << 9, "VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT"),
    std::make_pair(1ULL << 10, "VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT"),
    std::make_pair(1ULL << 11, "VK_ACCESS_TRANSFER_READ_BIT"),
    std::make_pair(1ULL << 12, "VK_ACCESS_TRANSFER_WRITE_BIT"),
    std::make_pair(1ULL << 13, "VK_ACCESS_HOST_READ_BIT"),
    std::make_pair(1ULL << 14, "VK_ACCESS_HOST_WRITE_BIT"),
    std::make_pair(1ULL << 15, "VK_ACCESS_MEMORY_READ_BIT"),
    std::make_pair(1ULL << 16, "VK_ACCESS_MEMORY_WRITE_BIT"),
    std::make_pair(1ULL << 25, "VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT"),
    std::make_pair(1ULL << 26, "VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT"),
    std::make_pair(1ULL << 27, "VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT"),
    std::make_pair(1ULL << 20, "VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT"),
    std::make_pair(1ULL << 19, "VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT"),
    std::make_pair(1ULL << 21, "VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR"),
    std::make_pair(1ULL << 22, "VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR"),
    std::make_pair(1ULL << 24, "VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT"),
    std::make_pair(1ULL << 23, "VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR"),
    std::make_pair(1ULL << 17, "VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV"),
    std::make_pair(1ULL << 18, "VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV"),
    std::make_pair(0, "VK_ACCESS_NONE_KHR"),
};
static void print_VkAccessFlagBits(VkAccessFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkAccessFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkAccessFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkAccessFlagBits(const VkAccessFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkAccessFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkAccessFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkImageLayout_map = {
    std::make_pair(0, "VK_IMAGE_LAYOUT_UNDEFINED"),
    std::make_pair(1, "VK_IMAGE_LAYOUT_GENERAL"),
    std::make_pair(2, "VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL"),
    std::make_pair(3, "VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL"),
    std::make_pair(4, "VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL"),
    std::make_pair(5, "VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL"),
    std::make_pair(6, "VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL"),
    std::make_pair(7, "VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL"),
    std::make_pair(8, "VK_IMAGE_LAYOUT_PREINITIALIZED"),
    std::make_pair(1000117000, "VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL"),
    std::make_pair(1000117001, "VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL"),
    std::make_pair(1000241000, "VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL"),
    std::make_pair(1000241001, "VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL"),
    std::make_pair(1000241002, "VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL"),
    std::make_pair(1000241003, "VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL"),
    std::make_pair(1000001002, "VK_IMAGE_LAYOUT_PRESENT_SRC_KHR"),
    std::make_pair(1000024000, "VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR"),
    std::make_pair(1000024001, "VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR"),
    std::make_pair(1000024002, "VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR"),
    std::make_pair(1000111000, "VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR"),
    std::make_pair(1000218000, "VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT"),
    std::make_pair(1000164003, "VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR"),
    std::make_pair(1000299000, "VK_IMAGE_LAYOUT_VIDEO_ENCODE_DST_KHR"),
    std::make_pair(1000299001, "VK_IMAGE_LAYOUT_VIDEO_ENCODE_SRC_KHR"),
    std::make_pair(1000299002, "VK_IMAGE_LAYOUT_VIDEO_ENCODE_DPB_KHR"),
    std::make_pair(1000314000, "VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR"),
    std::make_pair(1000314001, "VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR"),
};
static void print_VkImageLayout(VkImageLayout obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageLayout_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageLayout_map[obj] << "\"" << std::endl;
}
static void print_VkImageLayout(const VkImageLayout * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageLayout_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageLayout_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkImageAspectFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_IMAGE_ASPECT_COLOR_BIT"),
    std::make_pair(1ULL << 1, "VK_IMAGE_ASPECT_DEPTH_BIT"),
    std::make_pair(1ULL << 2, "VK_IMAGE_ASPECT_STENCIL_BIT"),
    std::make_pair(1ULL << 3, "VK_IMAGE_ASPECT_METADATA_BIT"),
    std::make_pair(1ULL << 4, "VK_IMAGE_ASPECT_PLANE_0_BIT"),
    std::make_pair(1ULL << 5, "VK_IMAGE_ASPECT_PLANE_1_BIT"),
    std::make_pair(1ULL << 6, "VK_IMAGE_ASPECT_PLANE_2_BIT"),
    std::make_pair(1ULL << 7, "VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT"),
    std::make_pair(1ULL << 8, "VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT"),
    std::make_pair(1ULL << 9, "VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT"),
    std::make_pair(1ULL << 10, "VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT"),
    std::make_pair(0, "VK_IMAGE_ASPECT_NONE_KHR"),
};
static void print_VkImageAspectFlagBits(VkImageAspectFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageAspectFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageAspectFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkImageAspectFlagBits(const VkImageAspectFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageAspectFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageAspectFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkObjectType_map = {
    std::make_pair(0, "VK_OBJECT_TYPE_UNKNOWN"),
    std::make_pair(1, "VK_OBJECT_TYPE_INSTANCE"),
    std::make_pair(2, "VK_OBJECT_TYPE_PHYSICAL_DEVICE"),
    std::make_pair(3, "VK_OBJECT_TYPE_DEVICE"),
    std::make_pair(4, "VK_OBJECT_TYPE_QUEUE"),
    std::make_pair(5, "VK_OBJECT_TYPE_SEMAPHORE"),
    std::make_pair(6, "VK_OBJECT_TYPE_COMMAND_BUFFER"),
    std::make_pair(7, "VK_OBJECT_TYPE_FENCE"),
    std::make_pair(8, "VK_OBJECT_TYPE_DEVICE_MEMORY"),
    std::make_pair(9, "VK_OBJECT_TYPE_BUFFER"),
    std::make_pair(10, "VK_OBJECT_TYPE_IMAGE"),
    std::make_pair(11, "VK_OBJECT_TYPE_EVENT"),
    std::make_pair(12, "VK_OBJECT_TYPE_QUERY_POOL"),
    std::make_pair(13, "VK_OBJECT_TYPE_BUFFER_VIEW"),
    std::make_pair(14, "VK_OBJECT_TYPE_IMAGE_VIEW"),
    std::make_pair(15, "VK_OBJECT_TYPE_SHADER_MODULE"),
    std::make_pair(16, "VK_OBJECT_TYPE_PIPELINE_CACHE"),
    std::make_pair(17, "VK_OBJECT_TYPE_PIPELINE_LAYOUT"),
    std::make_pair(18, "VK_OBJECT_TYPE_RENDER_PASS"),
    std::make_pair(19, "VK_OBJECT_TYPE_PIPELINE"),
    std::make_pair(20, "VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT"),
    std::make_pair(21, "VK_OBJECT_TYPE_SAMPLER"),
    std::make_pair(22, "VK_OBJECT_TYPE_DESCRIPTOR_POOL"),
    std::make_pair(23, "VK_OBJECT_TYPE_DESCRIPTOR_SET"),
    std::make_pair(24, "VK_OBJECT_TYPE_FRAMEBUFFER"),
    std::make_pair(25, "VK_OBJECT_TYPE_COMMAND_POOL"),
    std::make_pair(1000156000, "VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION"),
    std::make_pair(1000085000, "VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE"),
    std::make_pair(1000000000, "VK_OBJECT_TYPE_SURFACE_KHR"),
    std::make_pair(1000001000, "VK_OBJECT_TYPE_SWAPCHAIN_KHR"),
    std::make_pair(1000002000, "VK_OBJECT_TYPE_DISPLAY_KHR"),
    std::make_pair(1000002001, "VK_OBJECT_TYPE_DISPLAY_MODE_KHR"),
    std::make_pair(1000011000, "VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT"),
    std::make_pair(1000023000, "VK_OBJECT_TYPE_VIDEO_SESSION_KHR"),
    std::make_pair(1000023001, "VK_OBJECT_TYPE_VIDEO_SESSION_PARAMETERS_KHR"),
    std::make_pair(1000029000, "VK_OBJECT_TYPE_CU_MODULE_NVX"),
    std::make_pair(1000029001, "VK_OBJECT_TYPE_CU_FUNCTION_NVX"),
    std::make_pair(1000128000, "VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT"),
    std::make_pair(1000150000, "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR"),
    std::make_pair(1000160000, "VK_OBJECT_TYPE_VALIDATION_CACHE_EXT"),
    std::make_pair(1000165000, "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV"),
    std::make_pair(1000210000, "VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL"),
    std::make_pair(1000268000, "VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR"),
    std::make_pair(1000277000, "VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV"),
    std::make_pair(1000295000, "VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT"),
    std::make_pair(1000366000, "VK_OBJECT_TYPE_BUFFER_COLLECTION_FUCHSIA"),
    std::make_pair(1000489000, "VK_OBJECT_TYPE_SEMAPHORE_SCI_SYNC_POOL_NV"),
};
static void print_VkObjectType(VkObjectType obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkObjectType_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkObjectType_map[obj] << "\"" << std::endl;
}
static void print_VkObjectType(const VkObjectType * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkObjectType_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkObjectType_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPipelineCacheHeaderVersion_map = {
    std::make_pair(1, "VK_PIPELINE_CACHE_HEADER_VERSION_ONE"),
};
static void print_VkPipelineCacheHeaderVersion(VkPipelineCacheHeaderVersion obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineCacheHeaderVersion_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineCacheHeaderVersion_map[obj] << "\"" << std::endl;
}
static void print_VkPipelineCacheHeaderVersion(const VkPipelineCacheHeaderVersion * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineCacheHeaderVersion_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineCacheHeaderVersion_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkVendorId_map = {
    std::make_pair(0x10001, "VK_VENDOR_ID_VIV"),
    std::make_pair(0x10002, "VK_VENDOR_ID_VSI"),
    std::make_pair(0x10003, "VK_VENDOR_ID_KAZAN"),
    std::make_pair(0x10004, "VK_VENDOR_ID_CODEPLAY"),
    std::make_pair(0x10005, "VK_VENDOR_ID_MESA"),
    std::make_pair(0x10006, "VK_VENDOR_ID_POCL"),
};
static void print_VkVendorId(VkVendorId obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkVendorId_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkVendorId_map[obj] << "\"" << std::endl;
}
static void print_VkVendorId(const VkVendorId * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkVendorId_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkVendorId_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSystemAllocationScope_map = {
    std::make_pair(0, "VK_SYSTEM_ALLOCATION_SCOPE_COMMAND"),
    std::make_pair(1, "VK_SYSTEM_ALLOCATION_SCOPE_OBJECT"),
    std::make_pair(2, "VK_SYSTEM_ALLOCATION_SCOPE_CACHE"),
    std::make_pair(3, "VK_SYSTEM_ALLOCATION_SCOPE_DEVICE"),
    std::make_pair(4, "VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE"),
};
static void print_VkSystemAllocationScope(VkSystemAllocationScope obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSystemAllocationScope_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSystemAllocationScope_map[obj] << "\"" << std::endl;
}
static void print_VkSystemAllocationScope(const VkSystemAllocationScope * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSystemAllocationScope_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSystemAllocationScope_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkInternalAllocationType_map = {
    std::make_pair(0, "VK_INTERNAL_ALLOCATION_TYPE_EXECUTABLE"),
};
static void print_VkInternalAllocationType(VkInternalAllocationType obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkInternalAllocationType_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkInternalAllocationType_map[obj] << "\"" << std::endl;
}
static void print_VkInternalAllocationType(const VkInternalAllocationType * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkInternalAllocationType_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkInternalAllocationType_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkFormat_map = {
    std::make_pair(0, "VK_FORMAT_UNDEFINED"),
    std::make_pair(1, "VK_FORMAT_R4G4_UNORM_PACK8"),
    std::make_pair(2, "VK_FORMAT_R4G4B4A4_UNORM_PACK16"),
    std::make_pair(3, "VK_FORMAT_B4G4R4A4_UNORM_PACK16"),
    std::make_pair(4, "VK_FORMAT_R5G6B5_UNORM_PACK16"),
    std::make_pair(5, "VK_FORMAT_B5G6R5_UNORM_PACK16"),
    std::make_pair(6, "VK_FORMAT_R5G5B5A1_UNORM_PACK16"),
    std::make_pair(7, "VK_FORMAT_B5G5R5A1_UNORM_PACK16"),
    std::make_pair(8, "VK_FORMAT_A1R5G5B5_UNORM_PACK16"),
    std::make_pair(9, "VK_FORMAT_R8_UNORM"),
    std::make_pair(10, "VK_FORMAT_R8_SNORM"),
    std::make_pair(11, "VK_FORMAT_R8_USCALED"),
    std::make_pair(12, "VK_FORMAT_R8_SSCALED"),
    std::make_pair(13, "VK_FORMAT_R8_UINT"),
    std::make_pair(14, "VK_FORMAT_R8_SINT"),
    std::make_pair(15, "VK_FORMAT_R8_SRGB"),
    std::make_pair(16, "VK_FORMAT_R8G8_UNORM"),
    std::make_pair(17, "VK_FORMAT_R8G8_SNORM"),
    std::make_pair(18, "VK_FORMAT_R8G8_USCALED"),
    std::make_pair(19, "VK_FORMAT_R8G8_SSCALED"),
    std::make_pair(20, "VK_FORMAT_R8G8_UINT"),
    std::make_pair(21, "VK_FORMAT_R8G8_SINT"),
    std::make_pair(22, "VK_FORMAT_R8G8_SRGB"),
    std::make_pair(23, "VK_FORMAT_R8G8B8_UNORM"),
    std::make_pair(24, "VK_FORMAT_R8G8B8_SNORM"),
    std::make_pair(25, "VK_FORMAT_R8G8B8_USCALED"),
    std::make_pair(26, "VK_FORMAT_R8G8B8_SSCALED"),
    std::make_pair(27, "VK_FORMAT_R8G8B8_UINT"),
    std::make_pair(28, "VK_FORMAT_R8G8B8_SINT"),
    std::make_pair(29, "VK_FORMAT_R8G8B8_SRGB"),
    std::make_pair(30, "VK_FORMAT_B8G8R8_UNORM"),
    std::make_pair(31, "VK_FORMAT_B8G8R8_SNORM"),
    std::make_pair(32, "VK_FORMAT_B8G8R8_USCALED"),
    std::make_pair(33, "VK_FORMAT_B8G8R8_SSCALED"),
    std::make_pair(34, "VK_FORMAT_B8G8R8_UINT"),
    std::make_pair(35, "VK_FORMAT_B8G8R8_SINT"),
    std::make_pair(36, "VK_FORMAT_B8G8R8_SRGB"),
    std::make_pair(37, "VK_FORMAT_R8G8B8A8_UNORM"),
    std::make_pair(38, "VK_FORMAT_R8G8B8A8_SNORM"),
    std::make_pair(39, "VK_FORMAT_R8G8B8A8_USCALED"),
    std::make_pair(40, "VK_FORMAT_R8G8B8A8_SSCALED"),
    std::make_pair(41, "VK_FORMAT_R8G8B8A8_UINT"),
    std::make_pair(42, "VK_FORMAT_R8G8B8A8_SINT"),
    std::make_pair(43, "VK_FORMAT_R8G8B8A8_SRGB"),
    std::make_pair(44, "VK_FORMAT_B8G8R8A8_UNORM"),
    std::make_pair(45, "VK_FORMAT_B8G8R8A8_SNORM"),
    std::make_pair(46, "VK_FORMAT_B8G8R8A8_USCALED"),
    std::make_pair(47, "VK_FORMAT_B8G8R8A8_SSCALED"),
    std::make_pair(48, "VK_FORMAT_B8G8R8A8_UINT"),
    std::make_pair(49, "VK_FORMAT_B8G8R8A8_SINT"),
    std::make_pair(50, "VK_FORMAT_B8G8R8A8_SRGB"),
    std::make_pair(51, "VK_FORMAT_A8B8G8R8_UNORM_PACK32"),
    std::make_pair(52, "VK_FORMAT_A8B8G8R8_SNORM_PACK32"),
    std::make_pair(53, "VK_FORMAT_A8B8G8R8_USCALED_PACK32"),
    std::make_pair(54, "VK_FORMAT_A8B8G8R8_SSCALED_PACK32"),
    std::make_pair(55, "VK_FORMAT_A8B8G8R8_UINT_PACK32"),
    std::make_pair(56, "VK_FORMAT_A8B8G8R8_SINT_PACK32"),
    std::make_pair(57, "VK_FORMAT_A8B8G8R8_SRGB_PACK32"),
    std::make_pair(58, "VK_FORMAT_A2R10G10B10_UNORM_PACK32"),
    std::make_pair(59, "VK_FORMAT_A2R10G10B10_SNORM_PACK32"),
    std::make_pair(60, "VK_FORMAT_A2R10G10B10_USCALED_PACK32"),
    std::make_pair(61, "VK_FORMAT_A2R10G10B10_SSCALED_PACK32"),
    std::make_pair(62, "VK_FORMAT_A2R10G10B10_UINT_PACK32"),
    std::make_pair(63, "VK_FORMAT_A2R10G10B10_SINT_PACK32"),
    std::make_pair(64, "VK_FORMAT_A2B10G10R10_UNORM_PACK32"),
    std::make_pair(65, "VK_FORMAT_A2B10G10R10_SNORM_PACK32"),
    std::make_pair(66, "VK_FORMAT_A2B10G10R10_USCALED_PACK32"),
    std::make_pair(67, "VK_FORMAT_A2B10G10R10_SSCALED_PACK32"),
    std::make_pair(68, "VK_FORMAT_A2B10G10R10_UINT_PACK32"),
    std::make_pair(69, "VK_FORMAT_A2B10G10R10_SINT_PACK32"),
    std::make_pair(70, "VK_FORMAT_R16_UNORM"),
    std::make_pair(71, "VK_FORMAT_R16_SNORM"),
    std::make_pair(72, "VK_FORMAT_R16_USCALED"),
    std::make_pair(73, "VK_FORMAT_R16_SSCALED"),
    std::make_pair(74, "VK_FORMAT_R16_UINT"),
    std::make_pair(75, "VK_FORMAT_R16_SINT"),
    std::make_pair(76, "VK_FORMAT_R16_SFLOAT"),
    std::make_pair(77, "VK_FORMAT_R16G16_UNORM"),
    std::make_pair(78, "VK_FORMAT_R16G16_SNORM"),
    std::make_pair(79, "VK_FORMAT_R16G16_USCALED"),
    std::make_pair(80, "VK_FORMAT_R16G16_SSCALED"),
    std::make_pair(81, "VK_FORMAT_R16G16_UINT"),
    std::make_pair(82, "VK_FORMAT_R16G16_SINT"),
    std::make_pair(83, "VK_FORMAT_R16G16_SFLOAT"),
    std::make_pair(84, "VK_FORMAT_R16G16B16_UNORM"),
    std::make_pair(85, "VK_FORMAT_R16G16B16_SNORM"),
    std::make_pair(86, "VK_FORMAT_R16G16B16_USCALED"),
    std::make_pair(87, "VK_FORMAT_R16G16B16_SSCALED"),
    std::make_pair(88, "VK_FORMAT_R16G16B16_UINT"),
    std::make_pair(89, "VK_FORMAT_R16G16B16_SINT"),
    std::make_pair(90, "VK_FORMAT_R16G16B16_SFLOAT"),
    std::make_pair(91, "VK_FORMAT_R16G16B16A16_UNORM"),
    std::make_pair(92, "VK_FORMAT_R16G16B16A16_SNORM"),
    std::make_pair(93, "VK_FORMAT_R16G16B16A16_USCALED"),
    std::make_pair(94, "VK_FORMAT_R16G16B16A16_SSCALED"),
    std::make_pair(95, "VK_FORMAT_R16G16B16A16_UINT"),
    std::make_pair(96, "VK_FORMAT_R16G16B16A16_SINT"),
    std::make_pair(97, "VK_FORMAT_R16G16B16A16_SFLOAT"),
    std::make_pair(98, "VK_FORMAT_R32_UINT"),
    std::make_pair(99, "VK_FORMAT_R32_SINT"),
    std::make_pair(100, "VK_FORMAT_R32_SFLOAT"),
    std::make_pair(101, "VK_FORMAT_R32G32_UINT"),
    std::make_pair(102, "VK_FORMAT_R32G32_SINT"),
    std::make_pair(103, "VK_FORMAT_R32G32_SFLOAT"),
    std::make_pair(104, "VK_FORMAT_R32G32B32_UINT"),
    std::make_pair(105, "VK_FORMAT_R32G32B32_SINT"),
    std::make_pair(106, "VK_FORMAT_R32G32B32_SFLOAT"),
    std::make_pair(107, "VK_FORMAT_R32G32B32A32_UINT"),
    std::make_pair(108, "VK_FORMAT_R32G32B32A32_SINT"),
    std::make_pair(109, "VK_FORMAT_R32G32B32A32_SFLOAT"),
    std::make_pair(110, "VK_FORMAT_R64_UINT"),
    std::make_pair(111, "VK_FORMAT_R64_SINT"),
    std::make_pair(112, "VK_FORMAT_R64_SFLOAT"),
    std::make_pair(113, "VK_FORMAT_R64G64_UINT"),
    std::make_pair(114, "VK_FORMAT_R64G64_SINT"),
    std::make_pair(115, "VK_FORMAT_R64G64_SFLOAT"),
    std::make_pair(116, "VK_FORMAT_R64G64B64_UINT"),
    std::make_pair(117, "VK_FORMAT_R64G64B64_SINT"),
    std::make_pair(118, "VK_FORMAT_R64G64B64_SFLOAT"),
    std::make_pair(119, "VK_FORMAT_R64G64B64A64_UINT"),
    std::make_pair(120, "VK_FORMAT_R64G64B64A64_SINT"),
    std::make_pair(121, "VK_FORMAT_R64G64B64A64_SFLOAT"),
    std::make_pair(122, "VK_FORMAT_B10G11R11_UFLOAT_PACK32"),
    std::make_pair(123, "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32"),
    std::make_pair(124, "VK_FORMAT_D16_UNORM"),
    std::make_pair(125, "VK_FORMAT_X8_D24_UNORM_PACK32"),
    std::make_pair(126, "VK_FORMAT_D32_SFLOAT"),
    std::make_pair(127, "VK_FORMAT_S8_UINT"),
    std::make_pair(128, "VK_FORMAT_D16_UNORM_S8_UINT"),
    std::make_pair(129, "VK_FORMAT_D24_UNORM_S8_UINT"),
    std::make_pair(130, "VK_FORMAT_D32_SFLOAT_S8_UINT"),
    std::make_pair(131, "VK_FORMAT_BC1_RGB_UNORM_BLOCK"),
    std::make_pair(132, "VK_FORMAT_BC1_RGB_SRGB_BLOCK"),
    std::make_pair(133, "VK_FORMAT_BC1_RGBA_UNORM_BLOCK"),
    std::make_pair(134, "VK_FORMAT_BC1_RGBA_SRGB_BLOCK"),
    std::make_pair(135, "VK_FORMAT_BC2_UNORM_BLOCK"),
    std::make_pair(136, "VK_FORMAT_BC2_SRGB_BLOCK"),
    std::make_pair(137, "VK_FORMAT_BC3_UNORM_BLOCK"),
    std::make_pair(138, "VK_FORMAT_BC3_SRGB_BLOCK"),
    std::make_pair(139, "VK_FORMAT_BC4_UNORM_BLOCK"),
    std::make_pair(140, "VK_FORMAT_BC4_SNORM_BLOCK"),
    std::make_pair(141, "VK_FORMAT_BC5_UNORM_BLOCK"),
    std::make_pair(142, "VK_FORMAT_BC5_SNORM_BLOCK"),
    std::make_pair(143, "VK_FORMAT_BC6H_UFLOAT_BLOCK"),
    std::make_pair(144, "VK_FORMAT_BC6H_SFLOAT_BLOCK"),
    std::make_pair(145, "VK_FORMAT_BC7_UNORM_BLOCK"),
    std::make_pair(146, "VK_FORMAT_BC7_SRGB_BLOCK"),
    std::make_pair(147, "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK"),
    std::make_pair(148, "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK"),
    std::make_pair(149, "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK"),
    std::make_pair(150, "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK"),
    std::make_pair(151, "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK"),
    std::make_pair(152, "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK"),
    std::make_pair(153, "VK_FORMAT_EAC_R11_UNORM_BLOCK"),
    std::make_pair(154, "VK_FORMAT_EAC_R11_SNORM_BLOCK"),
    std::make_pair(155, "VK_FORMAT_EAC_R11G11_UNORM_BLOCK"),
    std::make_pair(156, "VK_FORMAT_EAC_R11G11_SNORM_BLOCK"),
    std::make_pair(157, "VK_FORMAT_ASTC_4x4_UNORM_BLOCK"),
    std::make_pair(158, "VK_FORMAT_ASTC_4x4_SRGB_BLOCK"),
    std::make_pair(159, "VK_FORMAT_ASTC_5x4_UNORM_BLOCK"),
    std::make_pair(160, "VK_FORMAT_ASTC_5x4_SRGB_BLOCK"),
    std::make_pair(161, "VK_FORMAT_ASTC_5x5_UNORM_BLOCK"),
    std::make_pair(162, "VK_FORMAT_ASTC_5x5_SRGB_BLOCK"),
    std::make_pair(163, "VK_FORMAT_ASTC_6x5_UNORM_BLOCK"),
    std::make_pair(164, "VK_FORMAT_ASTC_6x5_SRGB_BLOCK"),
    std::make_pair(165, "VK_FORMAT_ASTC_6x6_UNORM_BLOCK"),
    std::make_pair(166, "VK_FORMAT_ASTC_6x6_SRGB_BLOCK"),
    std::make_pair(167, "VK_FORMAT_ASTC_8x5_UNORM_BLOCK"),
    std::make_pair(168, "VK_FORMAT_ASTC_8x5_SRGB_BLOCK"),
    std::make_pair(169, "VK_FORMAT_ASTC_8x6_UNORM_BLOCK"),
    std::make_pair(170, "VK_FORMAT_ASTC_8x6_SRGB_BLOCK"),
    std::make_pair(171, "VK_FORMAT_ASTC_8x8_UNORM_BLOCK"),
    std::make_pair(172, "VK_FORMAT_ASTC_8x8_SRGB_BLOCK"),
    std::make_pair(173, "VK_FORMAT_ASTC_10x5_UNORM_BLOCK"),
    std::make_pair(174, "VK_FORMAT_ASTC_10x5_SRGB_BLOCK"),
    std::make_pair(175, "VK_FORMAT_ASTC_10x6_UNORM_BLOCK"),
    std::make_pair(176, "VK_FORMAT_ASTC_10x6_SRGB_BLOCK"),
    std::make_pair(177, "VK_FORMAT_ASTC_10x8_UNORM_BLOCK"),
    std::make_pair(178, "VK_FORMAT_ASTC_10x8_SRGB_BLOCK"),
    std::make_pair(179, "VK_FORMAT_ASTC_10x10_UNORM_BLOCK"),
    std::make_pair(180, "VK_FORMAT_ASTC_10x10_SRGB_BLOCK"),
    std::make_pair(181, "VK_FORMAT_ASTC_12x10_UNORM_BLOCK"),
    std::make_pair(182, "VK_FORMAT_ASTC_12x10_SRGB_BLOCK"),
    std::make_pair(183, "VK_FORMAT_ASTC_12x12_UNORM_BLOCK"),
    std::make_pair(184, "VK_FORMAT_ASTC_12x12_SRGB_BLOCK"),
    std::make_pair(1000156000, "VK_FORMAT_G8B8G8R8_422_UNORM"),
    std::make_pair(1000156001, "VK_FORMAT_B8G8R8G8_422_UNORM"),
    std::make_pair(1000156002, "VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM"),
    std::make_pair(1000156003, "VK_FORMAT_G8_B8R8_2PLANE_420_UNORM"),
    std::make_pair(1000156004, "VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM"),
    std::make_pair(1000156005, "VK_FORMAT_G8_B8R8_2PLANE_422_UNORM"),
    std::make_pair(1000156006, "VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM"),
    std::make_pair(1000156007, "VK_FORMAT_R10X6_UNORM_PACK16"),
    std::make_pair(1000156008, "VK_FORMAT_R10X6G10X6_UNORM_2PACK16"),
    std::make_pair(1000156009, "VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16"),
    std::make_pair(1000156010, "VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16"),
    std::make_pair(1000156011, "VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16"),
    std::make_pair(1000156012, "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16"),
    std::make_pair(1000156013, "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16"),
    std::make_pair(1000156014, "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16"),
    std::make_pair(1000156015, "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16"),
    std::make_pair(1000156016, "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16"),
    std::make_pair(1000156017, "VK_FORMAT_R12X4_UNORM_PACK16"),
    std::make_pair(1000156018, "VK_FORMAT_R12X4G12X4_UNORM_2PACK16"),
    std::make_pair(1000156019, "VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16"),
    std::make_pair(1000156020, "VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16"),
    std::make_pair(1000156021, "VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16"),
    std::make_pair(1000156022, "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16"),
    std::make_pair(1000156023, "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16"),
    std::make_pair(1000156024, "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16"),
    std::make_pair(1000156025, "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16"),
    std::make_pair(1000156026, "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16"),
    std::make_pair(1000156027, "VK_FORMAT_G16B16G16R16_422_UNORM"),
    std::make_pair(1000156028, "VK_FORMAT_B16G16R16G16_422_UNORM"),
    std::make_pair(1000156029, "VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM"),
    std::make_pair(1000156030, "VK_FORMAT_G16_B16R16_2PLANE_420_UNORM"),
    std::make_pair(1000156031, "VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM"),
    std::make_pair(1000156032, "VK_FORMAT_G16_B16R16_2PLANE_422_UNORM"),
    std::make_pair(1000156033, "VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM"),
    std::make_pair(1000054000, "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG"),
    std::make_pair(1000054001, "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG"),
    std::make_pair(1000054002, "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG"),
    std::make_pair(1000054003, "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG"),
    std::make_pair(1000054004, "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG"),
    std::make_pair(1000054005, "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG"),
    std::make_pair(1000054006, "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG"),
    std::make_pair(1000054007, "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG"),
    std::make_pair(1000066000, "VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066001, "VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066002, "VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066003, "VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066004, "VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066005, "VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066006, "VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066007, "VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066008, "VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066009, "VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066010, "VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066011, "VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066012, "VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000066013, "VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000288000, "VK_FORMAT_ASTC_3x3x3_UNORM_BLOCK_EXT"),
    std::make_pair(1000288001, "VK_FORMAT_ASTC_3x3x3_SRGB_BLOCK_EXT"),
    std::make_pair(1000288002, "VK_FORMAT_ASTC_3x3x3_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000288003, "VK_FORMAT_ASTC_4x3x3_UNORM_BLOCK_EXT"),
    std::make_pair(1000288004, "VK_FORMAT_ASTC_4x3x3_SRGB_BLOCK_EXT"),
    std::make_pair(1000288005, "VK_FORMAT_ASTC_4x3x3_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000288006, "VK_FORMAT_ASTC_4x4x3_UNORM_BLOCK_EXT"),
    std::make_pair(1000288007, "VK_FORMAT_ASTC_4x4x3_SRGB_BLOCK_EXT"),
    std::make_pair(1000288008, "VK_FORMAT_ASTC_4x4x3_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000288009, "VK_FORMAT_ASTC_4x4x4_UNORM_BLOCK_EXT"),
    std::make_pair(1000288010, "VK_FORMAT_ASTC_4x4x4_SRGB_BLOCK_EXT"),
    std::make_pair(1000288011, "VK_FORMAT_ASTC_4x4x4_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000288012, "VK_FORMAT_ASTC_5x4x4_UNORM_BLOCK_EXT"),
    std::make_pair(1000288013, "VK_FORMAT_ASTC_5x4x4_SRGB_BLOCK_EXT"),
    std::make_pair(1000288014, "VK_FORMAT_ASTC_5x4x4_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000288015, "VK_FORMAT_ASTC_5x5x4_UNORM_BLOCK_EXT"),
    std::make_pair(1000288016, "VK_FORMAT_ASTC_5x5x4_SRGB_BLOCK_EXT"),
    std::make_pair(1000288017, "VK_FORMAT_ASTC_5x5x4_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000288018, "VK_FORMAT_ASTC_5x5x5_UNORM_BLOCK_EXT"),
    std::make_pair(1000288019, "VK_FORMAT_ASTC_5x5x5_SRGB_BLOCK_EXT"),
    std::make_pair(1000288020, "VK_FORMAT_ASTC_5x5x5_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000288021, "VK_FORMAT_ASTC_6x5x5_UNORM_BLOCK_EXT"),
    std::make_pair(1000288022, "VK_FORMAT_ASTC_6x5x5_SRGB_BLOCK_EXT"),
    std::make_pair(1000288023, "VK_FORMAT_ASTC_6x5x5_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000288024, "VK_FORMAT_ASTC_6x6x5_UNORM_BLOCK_EXT"),
    std::make_pair(1000288025, "VK_FORMAT_ASTC_6x6x5_SRGB_BLOCK_EXT"),
    std::make_pair(1000288026, "VK_FORMAT_ASTC_6x6x5_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000288027, "VK_FORMAT_ASTC_6x6x6_UNORM_BLOCK_EXT"),
    std::make_pair(1000288028, "VK_FORMAT_ASTC_6x6x6_SRGB_BLOCK_EXT"),
    std::make_pair(1000288029, "VK_FORMAT_ASTC_6x6x6_SFLOAT_BLOCK_EXT"),
    std::make_pair(1000330000, "VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT"),
    std::make_pair(1000330001, "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT"),
    std::make_pair(1000330002, "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT"),
    std::make_pair(1000330003, "VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT"),
    std::make_pair(1000340000, "VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT"),
    std::make_pair(1000340001, "VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT"),
};
static void print_VkFormat(VkFormat obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFormat_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFormat_map[obj] << "\"" << std::endl;
}
static void print_VkFormat(const VkFormat * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFormat_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFormat_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkFormatFeatureFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT"),
    std::make_pair(1ULL << 1, "VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT"),
    std::make_pair(1ULL << 2, "VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT"),
    std::make_pair(1ULL << 3, "VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT"),
    std::make_pair(1ULL << 4, "VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT"),
    std::make_pair(1ULL << 5, "VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT"),
    std::make_pair(1ULL << 6, "VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT"),
    std::make_pair(1ULL << 7, "VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT"),
    std::make_pair(1ULL << 8, "VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT"),
    std::make_pair(1ULL << 9, "VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT"),
    std::make_pair(1ULL << 10, "VK_FORMAT_FEATURE_BLIT_SRC_BIT"),
    std::make_pair(1ULL << 11, "VK_FORMAT_FEATURE_BLIT_DST_BIT"),
    std::make_pair(1ULL << 12, "VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT"),
    std::make_pair(1ULL << 14, "VK_FORMAT_FEATURE_TRANSFER_SRC_BIT"),
    std::make_pair(1ULL << 15, "VK_FORMAT_FEATURE_TRANSFER_DST_BIT"),
    std::make_pair(1ULL << 17, "VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT"),
    std::make_pair(1ULL << 18, "VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT"),
    std::make_pair(1ULL << 19, "VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT"),
    std::make_pair(1ULL << 20, "VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT"),
    std::make_pair(1ULL << 21, "VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT"),
    std::make_pair(1ULL << 22, "VK_FORMAT_FEATURE_DISJOINT_BIT"),
    std::make_pair(1ULL << 23, "VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT"),
    std::make_pair(1ULL << 16, "VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT"),
    std::make_pair(1ULL << 13, "VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG"),
    std::make_pair(1ULL << 25, "VK_FORMAT_FEATURE_VIDEO_DECODE_OUTPUT_BIT_KHR"),
    std::make_pair(1ULL << 26, "VK_FORMAT_FEATURE_VIDEO_DECODE_DPB_BIT_KHR"),
    std::make_pair(1ULL << 29, "VK_FORMAT_FEATURE_ACCELERATION_STRUCTURE_VERTEX_BUFFER_BIT_KHR"),
    std::make_pair(1ULL << 24, "VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT"),
    std::make_pair(1ULL << 30, "VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR"),
    std::make_pair(1ULL << 27, "VK_FORMAT_FEATURE_VIDEO_ENCODE_INPUT_BIT_KHR"),
    std::make_pair(1ULL << 28, "VK_FORMAT_FEATURE_VIDEO_ENCODE_DPB_BIT_KHR"),
};
static void print_VkFormatFeatureFlagBits(VkFormatFeatureFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFormatFeatureFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFormatFeatureFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkFormatFeatureFlagBits(const VkFormatFeatureFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFormatFeatureFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFormatFeatureFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkImageCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_IMAGE_CREATE_SPARSE_BINDING_BIT"),
    std::make_pair(1ULL << 1, "VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT"),
    std::make_pair(1ULL << 2, "VK_IMAGE_CREATE_SPARSE_ALIASED_BIT"),
    std::make_pair(1ULL << 3, "VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT"),
    std::make_pair(1ULL << 4, "VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT"),
    std::make_pair(1ULL << 10, "VK_IMAGE_CREATE_ALIAS_BIT"),
    std::make_pair(1ULL << 6, "VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT"),
    std::make_pair(1ULL << 5, "VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT"),
    std::make_pair(1ULL << 7, "VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT"),
    std::make_pair(1ULL << 8, "VK_IMAGE_CREATE_EXTENDED_USAGE_BIT"),
    std::make_pair(1ULL << 11, "VK_IMAGE_CREATE_PROTECTED_BIT"),
    std::make_pair(1ULL << 9, "VK_IMAGE_CREATE_DISJOINT_BIT"),
    std::make_pair(1ULL << 13, "VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV"),
    std::make_pair(1ULL << 12, "VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT"),
    std::make_pair(1ULL << 14, "VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT"),
    std::make_pair(1ULL << 16, "VK_IMAGE_CREATE_RESERVED_16_BIT_AMD"),
    std::make_pair(1ULL << 17, "VK_IMAGE_CREATE_RESERVED_394_BIT_EXT"),
    std::make_pair(1ULL << 15, "VK_IMAGE_CREATE_RESERVED_426_BIT_QCOM"),
};
static void print_VkImageCreateFlagBits(VkImageCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkImageCreateFlagBits(const VkImageCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSampleCountFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_SAMPLE_COUNT_1_BIT"),
    std::make_pair(1ULL << 1, "VK_SAMPLE_COUNT_2_BIT"),
    std::make_pair(1ULL << 2, "VK_SAMPLE_COUNT_4_BIT"),
    std::make_pair(1ULL << 3, "VK_SAMPLE_COUNT_8_BIT"),
    std::make_pair(1ULL << 4, "VK_SAMPLE_COUNT_16_BIT"),
    std::make_pair(1ULL << 5, "VK_SAMPLE_COUNT_32_BIT"),
    std::make_pair(1ULL << 6, "VK_SAMPLE_COUNT_64_BIT"),
};
static void print_VkSampleCountFlagBits(VkSampleCountFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSampleCountFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSampleCountFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkSampleCountFlagBits(const VkSampleCountFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSampleCountFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSampleCountFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkImageTiling_map = {
    std::make_pair(0, "VK_IMAGE_TILING_OPTIMAL"),
    std::make_pair(1, "VK_IMAGE_TILING_LINEAR"),
    std::make_pair(1000158000, "VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT"),
};
static void print_VkImageTiling(VkImageTiling obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageTiling_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageTiling_map[obj] << "\"" << std::endl;
}
static void print_VkImageTiling(const VkImageTiling * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageTiling_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageTiling_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkImageType_map = {
    std::make_pair(0, "VK_IMAGE_TYPE_1D"),
    std::make_pair(1, "VK_IMAGE_TYPE_2D"),
    std::make_pair(2, "VK_IMAGE_TYPE_3D"),
};
static void print_VkImageType(VkImageType obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageType_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageType_map[obj] << "\"" << std::endl;
}
static void print_VkImageType(const VkImageType * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageType_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageType_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkImageUsageFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_IMAGE_USAGE_TRANSFER_SRC_BIT"),
    std::make_pair(1ULL << 1, "VK_IMAGE_USAGE_TRANSFER_DST_BIT"),
    std::make_pair(1ULL << 2, "VK_IMAGE_USAGE_SAMPLED_BIT"),
    std::make_pair(1ULL << 3, "VK_IMAGE_USAGE_STORAGE_BIT"),
    std::make_pair(1ULL << 4, "VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT"),
    std::make_pair(1ULL << 5, "VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT"),
    std::make_pair(1ULL << 6, "VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT"),
    std::make_pair(1ULL << 7, "VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT"),
    std::make_pair(1ULL << 10, "VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR"),
    std::make_pair(1ULL << 11, "VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR"),
    std::make_pair(1ULL << 12, "VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR"),
    std::make_pair(1ULL << 16, "VK_IMAGE_USAGE_RESERVED_16_BIT_QCOM"),
    std::make_pair(1ULL << 17, "VK_IMAGE_USAGE_RESERVED_17_BIT_QCOM"),
    std::make_pair(1ULL << 9, "VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT"),
    std::make_pair(1ULL << 8, "VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR"),
    std::make_pair(1ULL << 13, "VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR"),
    std::make_pair(1ULL << 14, "VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR"),
    std::make_pair(1ULL << 15, "VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR"),
    std::make_pair(1ULL << 19, "VK_IMAGE_USAGE_RESERVED_19_BIT_EXT"),
    std::make_pair(1ULL << 18, "VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI"),
};
static void print_VkImageUsageFlagBits(VkImageUsageFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageUsageFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageUsageFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkImageUsageFlagBits(const VkImageUsageFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageUsageFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageUsageFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkMemoryHeapFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_MEMORY_HEAP_DEVICE_LOCAL_BIT"),
    std::make_pair(1ULL << 1, "VK_MEMORY_HEAP_MULTI_INSTANCE_BIT"),
};
static void print_VkMemoryHeapFlagBits(VkMemoryHeapFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkMemoryHeapFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkMemoryHeapFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkMemoryHeapFlagBits(const VkMemoryHeapFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkMemoryHeapFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkMemoryHeapFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkMemoryPropertyFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT"),
    std::make_pair(1ULL << 1, "VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT"),
    std::make_pair(1ULL << 2, "VK_MEMORY_PROPERTY_HOST_COHERENT_BIT"),
    std::make_pair(1ULL << 3, "VK_MEMORY_PROPERTY_HOST_CACHED_BIT"),
    std::make_pair(1ULL << 4, "VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT"),
    std::make_pair(1ULL << 5, "VK_MEMORY_PROPERTY_PROTECTED_BIT"),
    std::make_pair(1ULL << 6, "VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD"),
    std::make_pair(1ULL << 7, "VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD"),
    std::make_pair(1ULL << 8, "VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV"),
};
static void print_VkMemoryPropertyFlagBits(VkMemoryPropertyFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkMemoryPropertyFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkMemoryPropertyFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkMemoryPropertyFlagBits(const VkMemoryPropertyFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkMemoryPropertyFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkMemoryPropertyFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPhysicalDeviceType_map = {
    std::make_pair(0, "VK_PHYSICAL_DEVICE_TYPE_OTHER"),
    std::make_pair(1, "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU"),
    std::make_pair(2, "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU"),
    std::make_pair(3, "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU"),
    std::make_pair(4, "VK_PHYSICAL_DEVICE_TYPE_CPU"),
};
static void print_VkPhysicalDeviceType(VkPhysicalDeviceType obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPhysicalDeviceType_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPhysicalDeviceType_map[obj] << "\"" << std::endl;
}
static void print_VkPhysicalDeviceType(const VkPhysicalDeviceType * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPhysicalDeviceType_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPhysicalDeviceType_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkQueueFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_QUEUE_GRAPHICS_BIT"),
    std::make_pair(1ULL << 1, "VK_QUEUE_COMPUTE_BIT"),
    std::make_pair(1ULL << 2, "VK_QUEUE_TRANSFER_BIT"),
    std::make_pair(1ULL << 3, "VK_QUEUE_SPARSE_BINDING_BIT"),
    std::make_pair(1ULL << 4, "VK_QUEUE_PROTECTED_BIT"),
    std::make_pair(1ULL << 5, "VK_QUEUE_VIDEO_DECODE_BIT_KHR"),
    std::make_pair(1ULL << 6, "VK_QUEUE_VIDEO_ENCODE_BIT_KHR"),
};
static void print_VkQueueFlagBits(VkQueueFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkQueueFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkQueueFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkQueueFlagBits(const VkQueueFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkQueueFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkQueueFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkDeviceQueueCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT"),
};
static void print_VkDeviceQueueCreateFlagBits(VkDeviceQueueCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDeviceQueueCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDeviceQueueCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkDeviceQueueCreateFlagBits(const VkDeviceQueueCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDeviceQueueCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDeviceQueueCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPipelineStageFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT"),
    std::make_pair(1ULL << 1, "VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT"),
    std::make_pair(1ULL << 2, "VK_PIPELINE_STAGE_VERTEX_INPUT_BIT"),
    std::make_pair(1ULL << 3, "VK_PIPELINE_STAGE_VERTEX_SHADER_BIT"),
    std::make_pair(1ULL << 4, "VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT"),
    std::make_pair(1ULL << 5, "VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT"),
    std::make_pair(1ULL << 6, "VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT"),
    std::make_pair(1ULL << 7, "VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT"),
    std::make_pair(1ULL << 8, "VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT"),
    std::make_pair(1ULL << 9, "VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT"),
    std::make_pair(1ULL << 10, "VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT"),
    std::make_pair(1ULL << 11, "VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT"),
    std::make_pair(1ULL << 12, "VK_PIPELINE_STAGE_TRANSFER_BIT"),
    std::make_pair(1ULL << 13, "VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT"),
    std::make_pair(1ULL << 14, "VK_PIPELINE_STAGE_HOST_BIT"),
    std::make_pair(1ULL << 15, "VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT"),
    std::make_pair(1ULL << 16, "VK_PIPELINE_STAGE_ALL_COMMANDS_BIT"),
    std::make_pair(1ULL << 24, "VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT"),
    std::make_pair(1ULL << 18, "VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT"),
    std::make_pair(1ULL << 25, "VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR"),
    std::make_pair(1ULL << 21, "VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR"),
    std::make_pair(1ULL << 19, "VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV"),
    std::make_pair(1ULL << 20, "VK_PIPELINE_STAGE_MESH_SHADER_BIT_NV"),
    std::make_pair(1ULL << 23, "VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT"),
    std::make_pair(1ULL << 22, "VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR"),
    std::make_pair(1ULL << 17, "VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV"),
    std::make_pair(0, "VK_PIPELINE_STAGE_NONE_KHR"),
};
static void print_VkPipelineStageFlagBits(VkPipelineStageFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineStageFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineStageFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkPipelineStageFlagBits(const VkPipelineStageFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineStageFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineStageFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSparseMemoryBindFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_SPARSE_MEMORY_BIND_METADATA_BIT"),
};
static void print_VkSparseMemoryBindFlagBits(VkSparseMemoryBindFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSparseMemoryBindFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSparseMemoryBindFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkSparseMemoryBindFlagBits(const VkSparseMemoryBindFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSparseMemoryBindFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSparseMemoryBindFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSparseImageFormatFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_SPARSE_IMAGE_FORMAT_SINGLE_MIPTAIL_BIT"),
    std::make_pair(1ULL << 1, "VK_SPARSE_IMAGE_FORMAT_ALIGNED_MIP_SIZE_BIT"),
    std::make_pair(1ULL << 2, "VK_SPARSE_IMAGE_FORMAT_NONSTANDARD_BLOCK_SIZE_BIT"),
};
static void print_VkSparseImageFormatFlagBits(VkSparseImageFormatFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSparseImageFormatFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSparseImageFormatFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkSparseImageFormatFlagBits(const VkSparseImageFormatFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSparseImageFormatFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSparseImageFormatFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkFenceCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_FENCE_CREATE_SIGNALED_BIT"),
};
static void print_VkFenceCreateFlagBits(VkFenceCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFenceCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFenceCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkFenceCreateFlagBits(const VkFenceCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFenceCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFenceCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkEventCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_EVENT_CREATE_DEVICE_ONLY_BIT_KHR"),
};
static void print_VkEventCreateFlagBits(VkEventCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkEventCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkEventCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkEventCreateFlagBits(const VkEventCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkEventCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkEventCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkQueryPipelineStatisticFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT"),
    std::make_pair(1ULL << 1, "VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT"),
    std::make_pair(1ULL << 2, "VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT"),
    std::make_pair(1ULL << 3, "VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT"),
    std::make_pair(1ULL << 4, "VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT"),
    std::make_pair(1ULL << 5, "VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT"),
    std::make_pair(1ULL << 6, "VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT"),
    std::make_pair(1ULL << 7, "VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT"),
    std::make_pair(1ULL << 8, "VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT"),
    std::make_pair(1ULL << 9, "VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT"),
    std::make_pair(1ULL << 10, "VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT"),
};
static void print_VkQueryPipelineStatisticFlagBits(VkQueryPipelineStatisticFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkQueryPipelineStatisticFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkQueryPipelineStatisticFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkQueryPipelineStatisticFlagBits(const VkQueryPipelineStatisticFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkQueryPipelineStatisticFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkQueryPipelineStatisticFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkQueryType_map = {
    std::make_pair(0, "VK_QUERY_TYPE_OCCLUSION"),
    std::make_pair(1, "VK_QUERY_TYPE_PIPELINE_STATISTICS"),
    std::make_pair(2, "VK_QUERY_TYPE_TIMESTAMP"),
    std::make_pair(1000023000, "VK_QUERY_TYPE_RESULT_STATUS_ONLY_KHR"),
    std::make_pair(1000028004, "VK_QUERY_TYPE_TRANSFORM_FEEDBACK_STREAM_EXT"),
    std::make_pair(1000116000, "VK_QUERY_TYPE_PERFORMANCE_QUERY_KHR"),
    std::make_pair(1000150000, "VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR"),
    std::make_pair(1000150001, "VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_SIZE_KHR"),
    std::make_pair(1000165000, "VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_NV"),
    std::make_pair(1000210000, "VK_QUERY_TYPE_PERFORMANCE_QUERY_INTEL"),
    std::make_pair(1000299000, "VK_QUERY_TYPE_VIDEO_ENCODE_BITSTREAM_BUFFER_RANGE_KHR"),
};
static void print_VkQueryType(VkQueryType obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkQueryType_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkQueryType_map[obj] << "\"" << std::endl;
}
static void print_VkQueryType(const VkQueryType * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkQueryType_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkQueryType_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkQueryResultFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_QUERY_RESULT_64_BIT"),
    std::make_pair(1ULL << 1, "VK_QUERY_RESULT_WAIT_BIT"),
    std::make_pair(1ULL << 2, "VK_QUERY_RESULT_WITH_AVAILABILITY_BIT"),
    std::make_pair(1ULL << 3, "VK_QUERY_RESULT_PARTIAL_BIT"),
    std::make_pair(1ULL << 4, "VK_QUERY_RESULT_WITH_STATUS_BIT_KHR"),
};
static void print_VkQueryResultFlagBits(VkQueryResultFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkQueryResultFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkQueryResultFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkQueryResultFlagBits(const VkQueryResultFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkQueryResultFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkQueryResultFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkBufferCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_BUFFER_CREATE_SPARSE_BINDING_BIT"),
    std::make_pair(1ULL << 1, "VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT"),
    std::make_pair(1ULL << 2, "VK_BUFFER_CREATE_SPARSE_ALIASED_BIT"),
    std::make_pair(1ULL << 3, "VK_BUFFER_CREATE_PROTECTED_BIT"),
    std::make_pair(1ULL << 4, "VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT"),
    std::make_pair(1ULL << 5, "VK_BUFFER_CREATE_RESERVED_5_BIT_AMD"),
};
static void print_VkBufferCreateFlagBits(VkBufferCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkBufferCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkBufferCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkBufferCreateFlagBits(const VkBufferCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkBufferCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkBufferCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkBufferUsageFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_BUFFER_USAGE_TRANSFER_SRC_BIT"),
    std::make_pair(1ULL << 1, "VK_BUFFER_USAGE_TRANSFER_DST_BIT"),
    std::make_pair(1ULL << 2, "VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT"),
    std::make_pair(1ULL << 3, "VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT"),
    std::make_pair(1ULL << 4, "VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT"),
    std::make_pair(1ULL << 5, "VK_BUFFER_USAGE_STORAGE_BUFFER_BIT"),
    std::make_pair(1ULL << 6, "VK_BUFFER_USAGE_INDEX_BUFFER_BIT"),
    std::make_pair(1ULL << 7, "VK_BUFFER_USAGE_VERTEX_BUFFER_BIT"),
    std::make_pair(1ULL << 8, "VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT"),
    std::make_pair(1ULL << 17, "VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT"),
    std::make_pair(1ULL << 13, "VK_BUFFER_USAGE_VIDEO_DECODE_SRC_BIT_KHR"),
    std::make_pair(1ULL << 14, "VK_BUFFER_USAGE_VIDEO_DECODE_DST_BIT_KHR"),
    std::make_pair(1ULL << 11, "VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT"),
    std::make_pair(1ULL << 12, "VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT"),
    std::make_pair(1ULL << 9, "VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT"),
    std::make_pair(1ULL << 19, "VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR"),
    std::make_pair(1ULL << 20, "VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR"),
    std::make_pair(1ULL << 10, "VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR"),
    std::make_pair(1ULL << 18, "VK_BUFFER_USAGE_RESERVED_18_BIT_QCOM"),
    std::make_pair(1ULL << 15, "VK_BUFFER_USAGE_VIDEO_ENCODE_DST_BIT_KHR"),
    std::make_pair(1ULL << 16, "VK_BUFFER_USAGE_VIDEO_ENCODE_SRC_BIT_KHR"),
    std::make_pair(1ULL << 21, "VK_BUFFER_USAGE_RESERVED_21_BIT_AMD"),
    std::make_pair(1ULL << 22, "VK_BUFFER_USAGE_RESERVED_22_BIT_AMD"),
};
static void print_VkBufferUsageFlagBits(VkBufferUsageFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkBufferUsageFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkBufferUsageFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkBufferUsageFlagBits(const VkBufferUsageFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkBufferUsageFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkBufferUsageFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSharingMode_map = {
    std::make_pair(0, "VK_SHARING_MODE_EXCLUSIVE"),
    std::make_pair(1, "VK_SHARING_MODE_CONCURRENT"),
};
static void print_VkSharingMode(VkSharingMode obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSharingMode_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSharingMode_map[obj] << "\"" << std::endl;
}
static void print_VkSharingMode(const VkSharingMode * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSharingMode_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSharingMode_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkComponentSwizzle_map = {
    std::make_pair(0, "VK_COMPONENT_SWIZZLE_IDENTITY"),
    std::make_pair(1, "VK_COMPONENT_SWIZZLE_ZERO"),
    std::make_pair(2, "VK_COMPONENT_SWIZZLE_ONE"),
    std::make_pair(3, "VK_COMPONENT_SWIZZLE_R"),
    std::make_pair(4, "VK_COMPONENT_SWIZZLE_G"),
    std::make_pair(5, "VK_COMPONENT_SWIZZLE_B"),
    std::make_pair(6, "VK_COMPONENT_SWIZZLE_A"),
};
static void print_VkComponentSwizzle(VkComponentSwizzle obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkComponentSwizzle_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkComponentSwizzle_map[obj] << "\"" << std::endl;
}
static void print_VkComponentSwizzle(const VkComponentSwizzle * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkComponentSwizzle_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkComponentSwizzle_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkImageViewCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT"),
    std::make_pair(1ULL << 1, "VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DEFERRED_BIT_EXT"),
};
static void print_VkImageViewCreateFlagBits(VkImageViewCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageViewCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageViewCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkImageViewCreateFlagBits(const VkImageViewCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageViewCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageViewCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkImageViewType_map = {
    std::make_pair(0, "VK_IMAGE_VIEW_TYPE_1D"),
    std::make_pair(1, "VK_IMAGE_VIEW_TYPE_2D"),
    std::make_pair(2, "VK_IMAGE_VIEW_TYPE_3D"),
    std::make_pair(3, "VK_IMAGE_VIEW_TYPE_CUBE"),
    std::make_pair(4, "VK_IMAGE_VIEW_TYPE_1D_ARRAY"),
    std::make_pair(5, "VK_IMAGE_VIEW_TYPE_2D_ARRAY"),
    std::make_pair(6, "VK_IMAGE_VIEW_TYPE_CUBE_ARRAY"),
};
static void print_VkImageViewType(VkImageViewType obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageViewType_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageViewType_map[obj] << "\"" << std::endl;
}
static void print_VkImageViewType(const VkImageViewType * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkImageViewType_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkImageViewType_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPipelineCacheCreateFlagBits_map = {
    std::make_pair(1ULL << 1, "VK_PIPELINE_CACHE_CREATE_RESERVED_1_BIT_EXT"),
    std::make_pair(1ULL << 0, "VK_PIPELINE_CACHE_CREATE_EXTERNALLY_SYNCHRONIZED_BIT_EXT"),
};
static void print_VkPipelineCacheCreateFlagBits(VkPipelineCacheCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineCacheCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineCacheCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkPipelineCacheCreateFlagBits(const VkPipelineCacheCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineCacheCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineCacheCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkBlendFactor_map = {
    std::make_pair(0, "VK_BLEND_FACTOR_ZERO"),
    std::make_pair(1, "VK_BLEND_FACTOR_ONE"),
    std::make_pair(2, "VK_BLEND_FACTOR_SRC_COLOR"),
    std::make_pair(3, "VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR"),
    std::make_pair(4, "VK_BLEND_FACTOR_DST_COLOR"),
    std::make_pair(5, "VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR"),
    std::make_pair(6, "VK_BLEND_FACTOR_SRC_ALPHA"),
    std::make_pair(7, "VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA"),
    std::make_pair(8, "VK_BLEND_FACTOR_DST_ALPHA"),
    std::make_pair(9, "VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA"),
    std::make_pair(10, "VK_BLEND_FACTOR_CONSTANT_COLOR"),
    std::make_pair(11, "VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR"),
    std::make_pair(12, "VK_BLEND_FACTOR_CONSTANT_ALPHA"),
    std::make_pair(13, "VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA"),
    std::make_pair(14, "VK_BLEND_FACTOR_SRC_ALPHA_SATURATE"),
    std::make_pair(15, "VK_BLEND_FACTOR_SRC1_COLOR"),
    std::make_pair(16, "VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR"),
    std::make_pair(17, "VK_BLEND_FACTOR_SRC1_ALPHA"),
    std::make_pair(18, "VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA"),
};
static void print_VkBlendFactor(VkBlendFactor obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkBlendFactor_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkBlendFactor_map[obj] << "\"" << std::endl;
}
static void print_VkBlendFactor(const VkBlendFactor * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkBlendFactor_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkBlendFactor_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkBlendOp_map = {
    std::make_pair(0, "VK_BLEND_OP_ADD"),
    std::make_pair(1, "VK_BLEND_OP_SUBTRACT"),
    std::make_pair(2, "VK_BLEND_OP_REVERSE_SUBTRACT"),
    std::make_pair(3, "VK_BLEND_OP_MIN"),
    std::make_pair(4, "VK_BLEND_OP_MAX"),
    std::make_pair(1000148000, "VK_BLEND_OP_ZERO_EXT"),
    std::make_pair(1000148001, "VK_BLEND_OP_SRC_EXT"),
    std::make_pair(1000148002, "VK_BLEND_OP_DST_EXT"),
    std::make_pair(1000148003, "VK_BLEND_OP_SRC_OVER_EXT"),
    std::make_pair(1000148004, "VK_BLEND_OP_DST_OVER_EXT"),
    std::make_pair(1000148005, "VK_BLEND_OP_SRC_IN_EXT"),
    std::make_pair(1000148006, "VK_BLEND_OP_DST_IN_EXT"),
    std::make_pair(1000148007, "VK_BLEND_OP_SRC_OUT_EXT"),
    std::make_pair(1000148008, "VK_BLEND_OP_DST_OUT_EXT"),
    std::make_pair(1000148009, "VK_BLEND_OP_SRC_ATOP_EXT"),
    std::make_pair(1000148010, "VK_BLEND_OP_DST_ATOP_EXT"),
    std::make_pair(1000148011, "VK_BLEND_OP_XOR_EXT"),
    std::make_pair(1000148012, "VK_BLEND_OP_MULTIPLY_EXT"),
    std::make_pair(1000148013, "VK_BLEND_OP_SCREEN_EXT"),
    std::make_pair(1000148014, "VK_BLEND_OP_OVERLAY_EXT"),
    std::make_pair(1000148015, "VK_BLEND_OP_DARKEN_EXT"),
    std::make_pair(1000148016, "VK_BLEND_OP_LIGHTEN_EXT"),
    std::make_pair(1000148017, "VK_BLEND_OP_COLORDODGE_EXT"),
    std::make_pair(1000148018, "VK_BLEND_OP_COLORBURN_EXT"),
    std::make_pair(1000148019, "VK_BLEND_OP_HARDLIGHT_EXT"),
    std::make_pair(1000148020, "VK_BLEND_OP_SOFTLIGHT_EXT"),
    std::make_pair(1000148021, "VK_BLEND_OP_DIFFERENCE_EXT"),
    std::make_pair(1000148022, "VK_BLEND_OP_EXCLUSION_EXT"),
    std::make_pair(1000148023, "VK_BLEND_OP_INVERT_EXT"),
    std::make_pair(1000148024, "VK_BLEND_OP_INVERT_RGB_EXT"),
    std::make_pair(1000148025, "VK_BLEND_OP_LINEARDODGE_EXT"),
    std::make_pair(1000148026, "VK_BLEND_OP_LINEARBURN_EXT"),
    std::make_pair(1000148027, "VK_BLEND_OP_VIVIDLIGHT_EXT"),
    std::make_pair(1000148028, "VK_BLEND_OP_LINEARLIGHT_EXT"),
    std::make_pair(1000148029, "VK_BLEND_OP_PINLIGHT_EXT"),
    std::make_pair(1000148030, "VK_BLEND_OP_HARDMIX_EXT"),
    std::make_pair(1000148031, "VK_BLEND_OP_HSL_HUE_EXT"),
    std::make_pair(1000148032, "VK_BLEND_OP_HSL_SATURATION_EXT"),
    std::make_pair(1000148033, "VK_BLEND_OP_HSL_COLOR_EXT"),
    std::make_pair(1000148034, "VK_BLEND_OP_HSL_LUMINOSITY_EXT"),
    std::make_pair(1000148035, "VK_BLEND_OP_PLUS_EXT"),
    std::make_pair(1000148036, "VK_BLEND_OP_PLUS_CLAMPED_EXT"),
    std::make_pair(1000148037, "VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT"),
    std::make_pair(1000148038, "VK_BLEND_OP_PLUS_DARKER_EXT"),
    std::make_pair(1000148039, "VK_BLEND_OP_MINUS_EXT"),
    std::make_pair(1000148040, "VK_BLEND_OP_MINUS_CLAMPED_EXT"),
    std::make_pair(1000148041, "VK_BLEND_OP_CONTRAST_EXT"),
    std::make_pair(1000148042, "VK_BLEND_OP_INVERT_OVG_EXT"),
    std::make_pair(1000148043, "VK_BLEND_OP_RED_EXT"),
    std::make_pair(1000148044, "VK_BLEND_OP_GREEN_EXT"),
    std::make_pair(1000148045, "VK_BLEND_OP_BLUE_EXT"),
};
static void print_VkBlendOp(VkBlendOp obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkBlendOp_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkBlendOp_map[obj] << "\"" << std::endl;
}
static void print_VkBlendOp(const VkBlendOp * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkBlendOp_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkBlendOp_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkColorComponentFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_COLOR_COMPONENT_R_BIT"),
    std::make_pair(1ULL << 1, "VK_COLOR_COMPONENT_G_BIT"),
    std::make_pair(1ULL << 2, "VK_COLOR_COMPONENT_B_BIT"),
    std::make_pair(1ULL << 3, "VK_COLOR_COMPONENT_A_BIT"),
};
static void print_VkColorComponentFlagBits(VkColorComponentFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkColorComponentFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkColorComponentFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkColorComponentFlagBits(const VkColorComponentFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkColorComponentFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkColorComponentFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkCompareOp_map = {
    std::make_pair(0, "VK_COMPARE_OP_NEVER"),
    std::make_pair(1, "VK_COMPARE_OP_LESS"),
    std::make_pair(2, "VK_COMPARE_OP_EQUAL"),
    std::make_pair(3, "VK_COMPARE_OP_LESS_OR_EQUAL"),
    std::make_pair(4, "VK_COMPARE_OP_GREATER"),
    std::make_pair(5, "VK_COMPARE_OP_NOT_EQUAL"),
    std::make_pair(6, "VK_COMPARE_OP_GREATER_OR_EQUAL"),
    std::make_pair(7, "VK_COMPARE_OP_ALWAYS"),
};
static void print_VkCompareOp(VkCompareOp obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCompareOp_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCompareOp_map[obj] << "\"" << std::endl;
}
static void print_VkCompareOp(const VkCompareOp * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCompareOp_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCompareOp_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPipelineCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT"),
    std::make_pair(1ULL << 1, "VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT"),
    std::make_pair(1ULL << 2, "VK_PIPELINE_CREATE_DERIVATIVE_BIT"),
    std::make_pair(1ULL << 3, "VK_PIPELINE_CREATE_VIEW_INDEX_FROM_DEVICE_INDEX_BIT"),
    std::make_pair(1ULL << 4, "VK_PIPELINE_CREATE_DISPATCH_BASE_BIT"),
    std::make_pair(1ULL << 21, "VK_PIPELINE_CREATE_RENDERING_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR"),
    std::make_pair(1ULL << 22, "VK_PIPELINE_CREATE_RENDERING_FRAGMENT_DENSITY_MAP_ATTACHMENT_BIT_EXT"),
    std::make_pair(1ULL << 14, "VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_ANY_HIT_SHADERS_BIT_KHR"),
    std::make_pair(1ULL << 15, "VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_CLOSEST_HIT_SHADERS_BIT_KHR"),
    std::make_pair(1ULL << 16, "VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_MISS_SHADERS_BIT_KHR"),
    std::make_pair(1ULL << 17, "VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_INTERSECTION_SHADERS_BIT_KHR"),
    std::make_pair(1ULL << 12, "VK_PIPELINE_CREATE_RAY_TRACING_SKIP_TRIANGLES_BIT_KHR"),
    std::make_pair(1ULL << 13, "VK_PIPELINE_CREATE_RAY_TRACING_SKIP_AABBS_BIT_KHR"),
    std::make_pair(1ULL << 19, "VK_PIPELINE_CREATE_RAY_TRACING_SHADER_GROUP_HANDLE_CAPTURE_REPLAY_BIT_KHR"),
    std::make_pair(1ULL << 5, "VK_PIPELINE_CREATE_DEFER_COMPILE_BIT_NV"),
    std::make_pair(1ULL << 6, "VK_PIPELINE_CREATE_CAPTURE_STATISTICS_BIT_KHR"),
    std::make_pair(1ULL << 7, "VK_PIPELINE_CREATE_CAPTURE_INTERNAL_REPRESENTATIONS_BIT_KHR"),
    std::make_pair(1ULL << 18, "VK_PIPELINE_CREATE_INDIRECT_BINDABLE_BIT_NV"),
    std::make_pair(1ULL << 11, "VK_PIPELINE_CREATE_LIBRARY_BIT_KHR"),
    std::make_pair(1ULL << 8, "VK_PIPELINE_CREATE_FAIL_ON_PIPELINE_COMPILE_REQUIRED_BIT_EXT"),
    std::make_pair(1ULL << 9, "VK_PIPELINE_CREATE_EARLY_RETURN_ON_FAILURE_BIT_EXT"),
    std::make_pair(1ULL << 23, "VK_PIPELINE_CREATE_RESERVED_23_BIT_AMD"),
    std::make_pair(1ULL << 10, "VK_PIPELINE_CREATE_RESERVED_10_BIT_AMD"),
    std::make_pair(1ULL << 20, "VK_PIPELINE_CREATE_RAY_TRACING_ALLOW_MOTION_BIT_NV"),
};
static void print_VkPipelineCreateFlagBits(VkPipelineCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkPipelineCreateFlagBits(const VkPipelineCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPipelineShaderStageCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT_EXT"),
    std::make_pair(1ULL << 1, "VK_PIPELINE_SHADER_STAGE_CREATE_REQUIRE_FULL_SUBGROUPS_BIT_EXT"),
    std::make_pair(1ULL << 3, "VK_PIPELINE_SHADER_STAGE_CREATE_RESERVED_3_BIT_KHR"),
};
static void print_VkPipelineShaderStageCreateFlagBits(VkPipelineShaderStageCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineShaderStageCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineShaderStageCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkPipelineShaderStageCreateFlagBits(const VkPipelineShaderStageCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineShaderStageCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineShaderStageCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkShaderStageFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_SHADER_STAGE_VERTEX_BIT"),
    std::make_pair(1ULL << 1, "VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT"),
    std::make_pair(1ULL << 2, "VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT"),
    std::make_pair(1ULL << 3, "VK_SHADER_STAGE_GEOMETRY_BIT"),
    std::make_pair(1ULL << 4, "VK_SHADER_STAGE_FRAGMENT_BIT"),
    std::make_pair(1ULL << 5, "VK_SHADER_STAGE_COMPUTE_BIT"),
    std::make_pair(0x0000001F, "VK_SHADER_STAGE_ALL_GRAPHICS"),
    std::make_pair(0x7FFFFFFF, "VK_SHADER_STAGE_ALL"),
    std::make_pair(1ULL << 8, "VK_SHADER_STAGE_RAYGEN_BIT_KHR"),
    std::make_pair(1ULL << 9, "VK_SHADER_STAGE_ANY_HIT_BIT_KHR"),
    std::make_pair(1ULL << 10, "VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR"),
    std::make_pair(1ULL << 11, "VK_SHADER_STAGE_MISS_BIT_KHR"),
    std::make_pair(1ULL << 12, "VK_SHADER_STAGE_INTERSECTION_BIT_KHR"),
    std::make_pair(1ULL << 13, "VK_SHADER_STAGE_CALLABLE_BIT_KHR"),
    std::make_pair(1ULL << 6, "VK_SHADER_STAGE_TASK_BIT_NV"),
    std::make_pair(1ULL << 7, "VK_SHADER_STAGE_MESH_BIT_NV"),
    std::make_pair(1ULL << 14, "VK_SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI"),
};
static void print_VkShaderStageFlagBits(VkShaderStageFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkShaderStageFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkShaderStageFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkShaderStageFlagBits(const VkShaderStageFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkShaderStageFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkShaderStageFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkCullModeFlagBits_map = {
    std::make_pair(0, "VK_CULL_MODE_NONE"),
    std::make_pair(1ULL << 0, "VK_CULL_MODE_FRONT_BIT"),
    std::make_pair(1ULL << 1, "VK_CULL_MODE_BACK_BIT"),
    std::make_pair(0x00000003, "VK_CULL_MODE_FRONT_AND_BACK"),
};
static void print_VkCullModeFlagBits(VkCullModeFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCullModeFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCullModeFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkCullModeFlagBits(const VkCullModeFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCullModeFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCullModeFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkDynamicState_map = {
    std::make_pair(0, "VK_DYNAMIC_STATE_VIEWPORT"),
    std::make_pair(1, "VK_DYNAMIC_STATE_SCISSOR"),
    std::make_pair(2, "VK_DYNAMIC_STATE_LINE_WIDTH"),
    std::make_pair(3, "VK_DYNAMIC_STATE_DEPTH_BIAS"),
    std::make_pair(4, "VK_DYNAMIC_STATE_BLEND_CONSTANTS"),
    std::make_pair(5, "VK_DYNAMIC_STATE_DEPTH_BOUNDS"),
    std::make_pair(6, "VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK"),
    std::make_pair(7, "VK_DYNAMIC_STATE_STENCIL_WRITE_MASK"),
    std::make_pair(8, "VK_DYNAMIC_STATE_STENCIL_REFERENCE"),
    std::make_pair(1000087000, "VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_NV"),
    std::make_pair(1000099000, "VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT"),
    std::make_pair(1000143000, "VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT"),
    std::make_pair(1000347000, "VK_DYNAMIC_STATE_RAY_TRACING_PIPELINE_STACK_SIZE_KHR"),
    std::make_pair(1000164004, "VK_DYNAMIC_STATE_VIEWPORT_SHADING_RATE_PALETTE_NV"),
    std::make_pair(1000164006, "VK_DYNAMIC_STATE_VIEWPORT_COARSE_SAMPLE_ORDER_NV"),
    std::make_pair(1000205001, "VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_NV"),
    std::make_pair(1000226000, "VK_DYNAMIC_STATE_FRAGMENT_SHADING_RATE_KHR"),
    std::make_pair(1000259000, "VK_DYNAMIC_STATE_LINE_STIPPLE_EXT"),
    std::make_pair(1000267000, "VK_DYNAMIC_STATE_CULL_MODE_EXT"),
    std::make_pair(1000267001, "VK_DYNAMIC_STATE_FRONT_FACE_EXT"),
    std::make_pair(1000267002, "VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY_EXT"),
    std::make_pair(1000267003, "VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT_EXT"),
    std::make_pair(1000267004, "VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT_EXT"),
    std::make_pair(1000267005, "VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE_EXT"),
    std::make_pair(1000267006, "VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE_EXT"),
    std::make_pair(1000267007, "VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE_EXT"),
    std::make_pair(1000267008, "VK_DYNAMIC_STATE_DEPTH_COMPARE_OP_EXT"),
    std::make_pair(1000267009, "VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE_EXT"),
    std::make_pair(1000267010, "VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE_EXT"),
    std::make_pair(1000267011, "VK_DYNAMIC_STATE_STENCIL_OP_EXT"),
    std::make_pair(1000352000, "VK_DYNAMIC_STATE_VERTEX_INPUT_EXT"),
    std::make_pair(1000377000, "VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT"),
    std::make_pair(1000377001, "VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE_EXT"),
    std::make_pair(1000377002, "VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE_EXT"),
    std::make_pair(1000377003, "VK_DYNAMIC_STATE_LOGIC_OP_EXT"),
    std::make_pair(1000377004, "VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE_EXT"),
    std::make_pair(1000381000, "VK_DYNAMIC_STATE_COLOR_WRITE_ENABLE_EXT"),
};
static void print_VkDynamicState(VkDynamicState obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDynamicState_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDynamicState_map[obj] << "\"" << std::endl;
}
static void print_VkDynamicState(const VkDynamicState * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDynamicState_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDynamicState_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkFrontFace_map = {
    std::make_pair(0, "VK_FRONT_FACE_COUNTER_CLOCKWISE"),
    std::make_pair(1, "VK_FRONT_FACE_CLOCKWISE"),
};
static void print_VkFrontFace(VkFrontFace obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFrontFace_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFrontFace_map[obj] << "\"" << std::endl;
}
static void print_VkFrontFace(const VkFrontFace * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFrontFace_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFrontFace_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkVertexInputRate_map = {
    std::make_pair(0, "VK_VERTEX_INPUT_RATE_VERTEX"),
    std::make_pair(1, "VK_VERTEX_INPUT_RATE_INSTANCE"),
};
static void print_VkVertexInputRate(VkVertexInputRate obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkVertexInputRate_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkVertexInputRate_map[obj] << "\"" << std::endl;
}
static void print_VkVertexInputRate(const VkVertexInputRate * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkVertexInputRate_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkVertexInputRate_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPrimitiveTopology_map = {
    std::make_pair(0, "VK_PRIMITIVE_TOPOLOGY_POINT_LIST"),
    std::make_pair(1, "VK_PRIMITIVE_TOPOLOGY_LINE_LIST"),
    std::make_pair(2, "VK_PRIMITIVE_TOPOLOGY_LINE_STRIP"),
    std::make_pair(3, "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST"),
    std::make_pair(4, "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP"),
    std::make_pair(5, "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN"),
    std::make_pair(6, "VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY"),
    std::make_pair(7, "VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY"),
    std::make_pair(8, "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY"),
    std::make_pair(9, "VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY"),
    std::make_pair(10, "VK_PRIMITIVE_TOPOLOGY_PATCH_LIST"),
};
static void print_VkPrimitiveTopology(VkPrimitiveTopology obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPrimitiveTopology_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPrimitiveTopology_map[obj] << "\"" << std::endl;
}
static void print_VkPrimitiveTopology(const VkPrimitiveTopology * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPrimitiveTopology_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPrimitiveTopology_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPolygonMode_map = {
    std::make_pair(0, "VK_POLYGON_MODE_FILL"),
    std::make_pair(1, "VK_POLYGON_MODE_LINE"),
    std::make_pair(2, "VK_POLYGON_MODE_POINT"),
    std::make_pair(1000153000, "VK_POLYGON_MODE_FILL_RECTANGLE_NV"),
};
static void print_VkPolygonMode(VkPolygonMode obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPolygonMode_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPolygonMode_map[obj] << "\"" << std::endl;
}
static void print_VkPolygonMode(const VkPolygonMode * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPolygonMode_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPolygonMode_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPipelineDepthStencilStateCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_PIPELINE_DEPTH_STENCIL_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS_BIT_ARM"),
    std::make_pair(1ULL << 1, "VK_PIPELINE_DEPTH_STENCIL_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS_BIT_ARM"),
};
static void print_VkPipelineDepthStencilStateCreateFlagBits(VkPipelineDepthStencilStateCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineDepthStencilStateCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineDepthStencilStateCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkPipelineDepthStencilStateCreateFlagBits(const VkPipelineDepthStencilStateCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineDepthStencilStateCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineDepthStencilStateCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkStencilOp_map = {
    std::make_pair(0, "VK_STENCIL_OP_KEEP"),
    std::make_pair(1, "VK_STENCIL_OP_ZERO"),
    std::make_pair(2, "VK_STENCIL_OP_REPLACE"),
    std::make_pair(3, "VK_STENCIL_OP_INCREMENT_AND_CLAMP"),
    std::make_pair(4, "VK_STENCIL_OP_DECREMENT_AND_CLAMP"),
    std::make_pair(5, "VK_STENCIL_OP_INVERT"),
    std::make_pair(6, "VK_STENCIL_OP_INCREMENT_AND_WRAP"),
    std::make_pair(7, "VK_STENCIL_OP_DECREMENT_AND_WRAP"),
};
static void print_VkStencilOp(VkStencilOp obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkStencilOp_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkStencilOp_map[obj] << "\"" << std::endl;
}
static void print_VkStencilOp(const VkStencilOp * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkStencilOp_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkStencilOp_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPipelineColorBlendStateCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_PIPELINE_COLOR_BLEND_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_BIT_ARM"),
};
static void print_VkPipelineColorBlendStateCreateFlagBits(VkPipelineColorBlendStateCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineColorBlendStateCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineColorBlendStateCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkPipelineColorBlendStateCreateFlagBits(const VkPipelineColorBlendStateCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineColorBlendStateCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineColorBlendStateCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkLogicOp_map = {
    std::make_pair(0, "VK_LOGIC_OP_CLEAR"),
    std::make_pair(1, "VK_LOGIC_OP_AND"),
    std::make_pair(2, "VK_LOGIC_OP_AND_REVERSE"),
    std::make_pair(3, "VK_LOGIC_OP_COPY"),
    std::make_pair(4, "VK_LOGIC_OP_AND_INVERTED"),
    std::make_pair(5, "VK_LOGIC_OP_NO_OP"),
    std::make_pair(6, "VK_LOGIC_OP_XOR"),
    std::make_pair(7, "VK_LOGIC_OP_OR"),
    std::make_pair(8, "VK_LOGIC_OP_NOR"),
    std::make_pair(9, "VK_LOGIC_OP_EQUIVALENT"),
    std::make_pair(10, "VK_LOGIC_OP_INVERT"),
    std::make_pair(11, "VK_LOGIC_OP_OR_REVERSE"),
    std::make_pair(12, "VK_LOGIC_OP_COPY_INVERTED"),
    std::make_pair(13, "VK_LOGIC_OP_OR_INVERTED"),
    std::make_pair(14, "VK_LOGIC_OP_NAND"),
    std::make_pair(15, "VK_LOGIC_OP_SET"),
};
static void print_VkLogicOp(VkLogicOp obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkLogicOp_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkLogicOp_map[obj] << "\"" << std::endl;
}
static void print_VkLogicOp(const VkLogicOp * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkLogicOp_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkLogicOp_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkBorderColor_map = {
    std::make_pair(0, "VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK"),
    std::make_pair(1, "VK_BORDER_COLOR_INT_TRANSPARENT_BLACK"),
    std::make_pair(2, "VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK"),
    std::make_pair(3, "VK_BORDER_COLOR_INT_OPAQUE_BLACK"),
    std::make_pair(4, "VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE"),
    std::make_pair(5, "VK_BORDER_COLOR_INT_OPAQUE_WHITE"),
    std::make_pair(1000287003, "VK_BORDER_COLOR_FLOAT_CUSTOM_EXT"),
    std::make_pair(1000287004, "VK_BORDER_COLOR_INT_CUSTOM_EXT"),
};
static void print_VkBorderColor(VkBorderColor obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkBorderColor_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkBorderColor_map[obj] << "\"" << std::endl;
}
static void print_VkBorderColor(const VkBorderColor * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkBorderColor_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkBorderColor_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkFilter_map = {
    std::make_pair(0, "VK_FILTER_NEAREST"),
    std::make_pair(1, "VK_FILTER_LINEAR"),
    std::make_pair(1000015000, "VK_FILTER_CUBIC_IMG"),
};
static void print_VkFilter(VkFilter obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFilter_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFilter_map[obj] << "\"" << std::endl;
}
static void print_VkFilter(const VkFilter * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFilter_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFilter_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSamplerAddressMode_map = {
    std::make_pair(0, "VK_SAMPLER_ADDRESS_MODE_REPEAT"),
    std::make_pair(1, "VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT"),
    std::make_pair(2, "VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE"),
    std::make_pair(3, "VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER"),
    std::make_pair(4, "VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE"),
    std::make_pair(4, "VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE"),
};
static void print_VkSamplerAddressMode(VkSamplerAddressMode obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerAddressMode_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerAddressMode_map[obj] << "\"" << std::endl;
}
static void print_VkSamplerAddressMode(const VkSamplerAddressMode * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerAddressMode_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerAddressMode_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSamplerCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_SAMPLER_CREATE_SUBSAMPLED_BIT_EXT"),
    std::make_pair(1ULL << 1, "VK_SAMPLER_CREATE_SUBSAMPLED_COARSE_RECONSTRUCTION_BIT_EXT"),
    std::make_pair(1ULL << 3, "VK_SAMPLER_CREATE_RESERVED_3_BIT_AMD"),
    std::make_pair(1ULL << 2, "VK_SAMPLER_CREATE_RESERVED_2_BIT_EXT"),
};
static void print_VkSamplerCreateFlagBits(VkSamplerCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkSamplerCreateFlagBits(const VkSamplerCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSamplerMipmapMode_map = {
    std::make_pair(0, "VK_SAMPLER_MIPMAP_MODE_NEAREST"),
    std::make_pair(1, "VK_SAMPLER_MIPMAP_MODE_LINEAR"),
};
static void print_VkSamplerMipmapMode(VkSamplerMipmapMode obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerMipmapMode_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerMipmapMode_map[obj] << "\"" << std::endl;
}
static void print_VkSamplerMipmapMode(const VkSamplerMipmapMode * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerMipmapMode_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerMipmapMode_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkDescriptorPoolCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT"),
    std::make_pair(1ULL << 1, "VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT"),
    std::make_pair(1ULL << 2, "VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_VALVE"),
};
static void print_VkDescriptorPoolCreateFlagBits(VkDescriptorPoolCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDescriptorPoolCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDescriptorPoolCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkDescriptorPoolCreateFlagBits(const VkDescriptorPoolCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDescriptorPoolCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDescriptorPoolCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkDescriptorType_map = {
    std::make_pair(0, "VK_DESCRIPTOR_TYPE_SAMPLER"),
    std::make_pair(1, "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER"),
    std::make_pair(2, "VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE"),
    std::make_pair(3, "VK_DESCRIPTOR_TYPE_STORAGE_IMAGE"),
    std::make_pair(4, "VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER"),
    std::make_pair(5, "VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER"),
    std::make_pair(6, "VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER"),
    std::make_pair(7, "VK_DESCRIPTOR_TYPE_STORAGE_BUFFER"),
    std::make_pair(8, "VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC"),
    std::make_pair(9, "VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC"),
    std::make_pair(10, "VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT"),
    std::make_pair(1000138000, "VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT"),
    std::make_pair(1000150000, "VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR"),
    std::make_pair(1000165000, "VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV"),
    std::make_pair(1000351000, "VK_DESCRIPTOR_TYPE_MUTABLE_VALVE"),
};
static void print_VkDescriptorType(VkDescriptorType obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDescriptorType_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDescriptorType_map[obj] << "\"" << std::endl;
}
static void print_VkDescriptorType(const VkDescriptorType * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDescriptorType_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDescriptorType_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkDescriptorSetLayoutCreateFlagBits_map = {
    std::make_pair(1ULL << 1, "VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT"),
    std::make_pair(1ULL << 0, "VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR"),
    std::make_pair(1ULL << 4, "VK_DESCRIPTOR_SET_LAYOUT_CREATE_RESERVED_4_BIT_AMD"),
    std::make_pair(1ULL << 3, "VK_DESCRIPTOR_SET_LAYOUT_CREATE_RESERVED_3_BIT_AMD"),
    std::make_pair(1ULL << 2, "VK_DESCRIPTOR_SET_LAYOUT_CREATE_HOST_ONLY_POOL_BIT_VALVE"),
};
static void print_VkDescriptorSetLayoutCreateFlagBits(VkDescriptorSetLayoutCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDescriptorSetLayoutCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDescriptorSetLayoutCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkDescriptorSetLayoutCreateFlagBits(const VkDescriptorSetLayoutCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDescriptorSetLayoutCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDescriptorSetLayoutCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkAttachmentDescriptionFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT"),
};
static void print_VkAttachmentDescriptionFlagBits(VkAttachmentDescriptionFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkAttachmentDescriptionFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkAttachmentDescriptionFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkAttachmentDescriptionFlagBits(const VkAttachmentDescriptionFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkAttachmentDescriptionFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkAttachmentDescriptionFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkAttachmentLoadOp_map = {
    std::make_pair(0, "VK_ATTACHMENT_LOAD_OP_LOAD"),
    std::make_pair(1, "VK_ATTACHMENT_LOAD_OP_CLEAR"),
    std::make_pair(2, "VK_ATTACHMENT_LOAD_OP_DONT_CARE"),
    std::make_pair(1000400000, "VK_ATTACHMENT_LOAD_OP_NONE_EXT"),
};
static void print_VkAttachmentLoadOp(VkAttachmentLoadOp obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkAttachmentLoadOp_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkAttachmentLoadOp_map[obj] << "\"" << std::endl;
}
static void print_VkAttachmentLoadOp(const VkAttachmentLoadOp * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkAttachmentLoadOp_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkAttachmentLoadOp_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkAttachmentStoreOp_map = {
    std::make_pair(0, "VK_ATTACHMENT_STORE_OP_STORE"),
    std::make_pair(1, "VK_ATTACHMENT_STORE_OP_DONT_CARE"),
    std::make_pair(1000301000, "VK_ATTACHMENT_STORE_OP_NONE_KHR"),
};
static void print_VkAttachmentStoreOp(VkAttachmentStoreOp obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkAttachmentStoreOp_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkAttachmentStoreOp_map[obj] << "\"" << std::endl;
}
static void print_VkAttachmentStoreOp(const VkAttachmentStoreOp * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkAttachmentStoreOp_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkAttachmentStoreOp_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkDependencyFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_DEPENDENCY_BY_REGION_BIT"),
    std::make_pair(1ULL << 2, "VK_DEPENDENCY_DEVICE_GROUP_BIT"),
    std::make_pair(1ULL << 1, "VK_DEPENDENCY_VIEW_LOCAL_BIT"),
};
static void print_VkDependencyFlagBits(VkDependencyFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDependencyFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDependencyFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkDependencyFlagBits(const VkDependencyFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDependencyFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDependencyFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkFramebufferCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT"),
};
static void print_VkFramebufferCreateFlagBits(VkFramebufferCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFramebufferCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFramebufferCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkFramebufferCreateFlagBits(const VkFramebufferCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFramebufferCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFramebufferCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPipelineBindPoint_map = {
    std::make_pair(0, "VK_PIPELINE_BIND_POINT_GRAPHICS"),
    std::make_pair(1, "VK_PIPELINE_BIND_POINT_COMPUTE"),
    std::make_pair(1000165000, "VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR"),
    std::make_pair(1000369003, "VK_PIPELINE_BIND_POINT_SUBPASS_SHADING_HUAWEI"),
};
static void print_VkPipelineBindPoint(VkPipelineBindPoint obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineBindPoint_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineBindPoint_map[obj] << "\"" << std::endl;
}
static void print_VkPipelineBindPoint(const VkPipelineBindPoint * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPipelineBindPoint_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPipelineBindPoint_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkRenderPassCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_RENDER_PASS_CREATE_RESERVED_0_BIT_KHR"),
    std::make_pair(1ULL << 1, "VK_RENDER_PASS_CREATE_TRANSFORM_BIT_QCOM"),
};
static void print_VkRenderPassCreateFlagBits(VkRenderPassCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkRenderPassCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkRenderPassCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkRenderPassCreateFlagBits(const VkRenderPassCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkRenderPassCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkRenderPassCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSubpassDescriptionFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_SUBPASS_DESCRIPTION_PER_VIEW_ATTRIBUTES_BIT_NVX"),
    std::make_pair(1ULL << 1, "VK_SUBPASS_DESCRIPTION_PER_VIEW_POSITION_X_ONLY_BIT_NVX"),
    std::make_pair(1ULL << 2, "VK_SUBPASS_DESCRIPTION_FRAGMENT_REGION_BIT_QCOM"),
    std::make_pair(1ULL << 3, "VK_SUBPASS_DESCRIPTION_SHADER_RESOLVE_BIT_QCOM"),
    std::make_pair(1ULL << 4, "VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS_BIT_ARM"),
    std::make_pair(1ULL << 5, "VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS_BIT_ARM"),
    std::make_pair(1ULL << 6, "VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS_BIT_ARM"),
};
static void print_VkSubpassDescriptionFlagBits(VkSubpassDescriptionFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSubpassDescriptionFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSubpassDescriptionFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkSubpassDescriptionFlagBits(const VkSubpassDescriptionFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSubpassDescriptionFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSubpassDescriptionFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkCommandPoolCreateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_COMMAND_POOL_CREATE_TRANSIENT_BIT"),
    std::make_pair(1ULL << 1, "VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT"),
    std::make_pair(1ULL << 2, "VK_COMMAND_POOL_CREATE_PROTECTED_BIT"),
};
static void print_VkCommandPoolCreateFlagBits(VkCommandPoolCreateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCommandPoolCreateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCommandPoolCreateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkCommandPoolCreateFlagBits(const VkCommandPoolCreateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCommandPoolCreateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCommandPoolCreateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkCommandPoolResetFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT"),
    std::make_pair(1ULL << 1, "VK_COMMAND_POOL_RESET_RESERVED_1_BIT_COREAVI"),
};
static void print_VkCommandPoolResetFlagBits(VkCommandPoolResetFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCommandPoolResetFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCommandPoolResetFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkCommandPoolResetFlagBits(const VkCommandPoolResetFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCommandPoolResetFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCommandPoolResetFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkCommandBufferLevel_map = {
    std::make_pair(0, "VK_COMMAND_BUFFER_LEVEL_PRIMARY"),
    std::make_pair(1, "VK_COMMAND_BUFFER_LEVEL_SECONDARY"),
};
static void print_VkCommandBufferLevel(VkCommandBufferLevel obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCommandBufferLevel_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCommandBufferLevel_map[obj] << "\"" << std::endl;
}
static void print_VkCommandBufferLevel(const VkCommandBufferLevel * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCommandBufferLevel_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCommandBufferLevel_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkCommandBufferUsageFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT"),
    std::make_pair(1ULL << 1, "VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT"),
    std::make_pair(1ULL << 2, "VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT"),
};
static void print_VkCommandBufferUsageFlagBits(VkCommandBufferUsageFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCommandBufferUsageFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCommandBufferUsageFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkCommandBufferUsageFlagBits(const VkCommandBufferUsageFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCommandBufferUsageFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCommandBufferUsageFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkQueryControlFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_QUERY_CONTROL_PRECISE_BIT"),
};
static void print_VkQueryControlFlagBits(VkQueryControlFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkQueryControlFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkQueryControlFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkQueryControlFlagBits(const VkQueryControlFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkQueryControlFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkQueryControlFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkCommandBufferResetFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT"),
};
static void print_VkCommandBufferResetFlagBits(VkCommandBufferResetFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCommandBufferResetFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCommandBufferResetFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkCommandBufferResetFlagBits(const VkCommandBufferResetFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkCommandBufferResetFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkCommandBufferResetFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkIndexType_map = {
    std::make_pair(0, "VK_INDEX_TYPE_UINT16"),
    std::make_pair(1, "VK_INDEX_TYPE_UINT32"),
    std::make_pair(1000165000, "VK_INDEX_TYPE_NONE_KHR"),
    std::make_pair(1000265000, "VK_INDEX_TYPE_UINT8_EXT"),
};
static void print_VkIndexType(VkIndexType obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkIndexType_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkIndexType_map[obj] << "\"" << std::endl;
}
static void print_VkIndexType(const VkIndexType * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkIndexType_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkIndexType_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkStencilFaceFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_STENCIL_FACE_FRONT_BIT"),
    std::make_pair(1ULL << 1, "VK_STENCIL_FACE_BACK_BIT"),
    std::make_pair(0x00000003, "VK_STENCIL_FACE_FRONT_AND_BACK"),
};
static void print_VkStencilFaceFlagBits(VkStencilFaceFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkStencilFaceFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkStencilFaceFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkStencilFaceFlagBits(const VkStencilFaceFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkStencilFaceFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkStencilFaceFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSubpassContents_map = {
    std::make_pair(0, "VK_SUBPASS_CONTENTS_INLINE"),
    std::make_pair(1, "VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS"),
};
static void print_VkSubpassContents(VkSubpassContents obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSubpassContents_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSubpassContents_map[obj] << "\"" << std::endl;
}
static void print_VkSubpassContents(const VkSubpassContents * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSubpassContents_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSubpassContents_map[*obj] << "\"" << std::endl;
}

static void print_VkAccessFlags(VkAccessFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkAccessFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkAccessFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkAccessFlags(const VkAccessFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkAccessFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkAccessFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkImageAspectFlags(VkImageAspectFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkImageAspectFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkImageAspectFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkImageAspectFlags(const VkImageAspectFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkImageAspectFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkImageAspectFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkFormatFeatureFlags(VkFormatFeatureFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkFormatFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkFormatFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkFormatFeatureFlags(const VkFormatFeatureFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkFormatFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkFormatFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkImageCreateFlags(VkImageCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkImageCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkImageCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkImageCreateFlags(const VkImageCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkImageCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkImageCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkSampleCountFlags(VkSampleCountFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSampleCountFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSampleCountFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkSampleCountFlags(const VkSampleCountFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSampleCountFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSampleCountFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkImageUsageFlags(VkImageUsageFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkImageUsageFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkImageUsageFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkImageUsageFlags(const VkImageUsageFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkImageUsageFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkImageUsageFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkInstanceCreateFlags(VkInstanceCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkInstanceCreateFlags(const VkInstanceCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkMemoryHeapFlags(VkMemoryHeapFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkMemoryHeapFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkMemoryHeapFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkMemoryHeapFlags(const VkMemoryHeapFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkMemoryHeapFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkMemoryHeapFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkMemoryPropertyFlags(VkMemoryPropertyFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkMemoryPropertyFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkMemoryPropertyFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkMemoryPropertyFlags(const VkMemoryPropertyFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkMemoryPropertyFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkMemoryPropertyFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkQueueFlags(VkQueueFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkQueueFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkQueueFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkQueueFlags(const VkQueueFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkQueueFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkQueueFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkDeviceCreateFlags(VkDeviceCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkDeviceCreateFlags(const VkDeviceCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkDeviceQueueCreateFlags(VkDeviceQueueCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkDeviceQueueCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkDeviceQueueCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkDeviceQueueCreateFlags(const VkDeviceQueueCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkDeviceQueueCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkDeviceQueueCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkPipelineStageFlags(VkPipelineStageFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineStageFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineStageFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkPipelineStageFlags(const VkPipelineStageFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineStageFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineStageFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkMemoryMapFlags(VkMemoryMapFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkMemoryMapFlags(const VkMemoryMapFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkSparseMemoryBindFlags(VkSparseMemoryBindFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSparseMemoryBindFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSparseMemoryBindFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkSparseMemoryBindFlags(const VkSparseMemoryBindFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSparseMemoryBindFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSparseMemoryBindFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkSparseImageFormatFlags(VkSparseImageFormatFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSparseImageFormatFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSparseImageFormatFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkSparseImageFormatFlags(const VkSparseImageFormatFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSparseImageFormatFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSparseImageFormatFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkFenceCreateFlags(VkFenceCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkFenceCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkFenceCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkFenceCreateFlags(const VkFenceCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkFenceCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkFenceCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkSemaphoreCreateFlags(VkSemaphoreCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkSemaphoreCreateFlags(const VkSemaphoreCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkEventCreateFlags(VkEventCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkEventCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkEventCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkEventCreateFlags(const VkEventCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkEventCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkEventCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkQueryPipelineStatisticFlags(VkQueryPipelineStatisticFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkQueryPipelineStatisticFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkQueryPipelineStatisticFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkQueryPipelineStatisticFlags(const VkQueryPipelineStatisticFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkQueryPipelineStatisticFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkQueryPipelineStatisticFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkQueryPoolCreateFlags(VkQueryPoolCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkQueryPoolCreateFlags(const VkQueryPoolCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkQueryResultFlags(VkQueryResultFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkQueryResultFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkQueryResultFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkQueryResultFlags(const VkQueryResultFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkQueryResultFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkQueryResultFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkBufferCreateFlags(VkBufferCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkBufferCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkBufferCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkBufferCreateFlags(const VkBufferCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkBufferCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkBufferCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkBufferUsageFlags(VkBufferUsageFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkBufferUsageFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkBufferUsageFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkBufferUsageFlags(const VkBufferUsageFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkBufferUsageFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkBufferUsageFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkBufferViewCreateFlags(VkBufferViewCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkBufferViewCreateFlags(const VkBufferViewCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkImageViewCreateFlags(VkImageViewCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkImageViewCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkImageViewCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkImageViewCreateFlags(const VkImageViewCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkImageViewCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkImageViewCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkShaderModuleCreateFlags(VkShaderModuleCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkShaderModuleCreateFlags(const VkShaderModuleCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkPipelineCacheCreateFlags(VkPipelineCacheCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineCacheCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineCacheCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkPipelineCacheCreateFlags(const VkPipelineCacheCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineCacheCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineCacheCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkColorComponentFlags(VkColorComponentFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkColorComponentFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkColorComponentFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkColorComponentFlags(const VkColorComponentFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkColorComponentFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkColorComponentFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkPipelineCreateFlags(VkPipelineCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkPipelineCreateFlags(const VkPipelineCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkPipelineShaderStageCreateFlags(VkPipelineShaderStageCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineShaderStageCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineShaderStageCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkPipelineShaderStageCreateFlags(const VkPipelineShaderStageCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineShaderStageCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineShaderStageCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkCullModeFlags(VkCullModeFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkCullModeFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkCullModeFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkCullModeFlags(const VkCullModeFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkCullModeFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkCullModeFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkPipelineVertexInputStateCreateFlags(VkPipelineVertexInputStateCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkPipelineVertexInputStateCreateFlags(const VkPipelineVertexInputStateCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkPipelineInputAssemblyStateCreateFlags(VkPipelineInputAssemblyStateCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkPipelineInputAssemblyStateCreateFlags(const VkPipelineInputAssemblyStateCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkPipelineTessellationStateCreateFlags(VkPipelineTessellationStateCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkPipelineTessellationStateCreateFlags(const VkPipelineTessellationStateCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkPipelineViewportStateCreateFlags(VkPipelineViewportStateCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkPipelineViewportStateCreateFlags(const VkPipelineViewportStateCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkPipelineRasterizationStateCreateFlags(VkPipelineRasterizationStateCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkPipelineRasterizationStateCreateFlags(const VkPipelineRasterizationStateCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkPipelineMultisampleStateCreateFlags(VkPipelineMultisampleStateCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkPipelineMultisampleStateCreateFlags(const VkPipelineMultisampleStateCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkPipelineDepthStencilStateCreateFlags(VkPipelineDepthStencilStateCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineDepthStencilStateCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineDepthStencilStateCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkPipelineDepthStencilStateCreateFlags(const VkPipelineDepthStencilStateCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineDepthStencilStateCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineDepthStencilStateCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkPipelineColorBlendStateCreateFlags(VkPipelineColorBlendStateCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineColorBlendStateCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineColorBlendStateCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkPipelineColorBlendStateCreateFlags(const VkPipelineColorBlendStateCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPipelineColorBlendStateCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPipelineColorBlendStateCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkPipelineDynamicStateCreateFlags(VkPipelineDynamicStateCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkPipelineDynamicStateCreateFlags(const VkPipelineDynamicStateCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkPipelineLayoutCreateFlags(VkPipelineLayoutCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkPipelineLayoutCreateFlags(const VkPipelineLayoutCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkShaderStageFlags(VkShaderStageFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkShaderStageFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkShaderStageFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkShaderStageFlags(const VkShaderStageFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkShaderStageFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkShaderStageFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkSamplerCreateFlags(VkSamplerCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSamplerCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSamplerCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkSamplerCreateFlags(const VkSamplerCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSamplerCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSamplerCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkDescriptorPoolCreateFlags(VkDescriptorPoolCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkDescriptorPoolCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkDescriptorPoolCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkDescriptorPoolCreateFlags(const VkDescriptorPoolCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkDescriptorPoolCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkDescriptorPoolCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkDescriptorPoolResetFlags(VkDescriptorPoolResetFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkDescriptorPoolResetFlags(const VkDescriptorPoolResetFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkDescriptorSetLayoutCreateFlags(VkDescriptorSetLayoutCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkDescriptorSetLayoutCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkDescriptorSetLayoutCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkDescriptorSetLayoutCreateFlags(const VkDescriptorSetLayoutCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkDescriptorSetLayoutCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkDescriptorSetLayoutCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkAttachmentDescriptionFlags(VkAttachmentDescriptionFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkAttachmentDescriptionFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkAttachmentDescriptionFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkAttachmentDescriptionFlags(const VkAttachmentDescriptionFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkAttachmentDescriptionFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkAttachmentDescriptionFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkDependencyFlags(VkDependencyFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkDependencyFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkDependencyFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkDependencyFlags(const VkDependencyFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkDependencyFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkDependencyFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkFramebufferCreateFlags(VkFramebufferCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkFramebufferCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkFramebufferCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkFramebufferCreateFlags(const VkFramebufferCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkFramebufferCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkFramebufferCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkRenderPassCreateFlags(VkRenderPassCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkRenderPassCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkRenderPassCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkRenderPassCreateFlags(const VkRenderPassCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkRenderPassCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkRenderPassCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkSubpassDescriptionFlags(VkSubpassDescriptionFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSubpassDescriptionFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSubpassDescriptionFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkSubpassDescriptionFlags(const VkSubpassDescriptionFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSubpassDescriptionFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSubpassDescriptionFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkCommandPoolCreateFlags(VkCommandPoolCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkCommandPoolCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkCommandPoolCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkCommandPoolCreateFlags(const VkCommandPoolCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkCommandPoolCreateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkCommandPoolCreateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkCommandPoolResetFlags(VkCommandPoolResetFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkCommandPoolResetFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkCommandPoolResetFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkCommandPoolResetFlags(const VkCommandPoolResetFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkCommandPoolResetFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkCommandPoolResetFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkCommandBufferUsageFlags(VkCommandBufferUsageFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkCommandBufferUsageFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkCommandBufferUsageFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkCommandBufferUsageFlags(const VkCommandBufferUsageFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkCommandBufferUsageFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkCommandBufferUsageFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkQueryControlFlags(VkQueryControlFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkQueryControlFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkQueryControlFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkQueryControlFlags(const VkQueryControlFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkQueryControlFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkQueryControlFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkCommandBufferResetFlags(VkCommandBufferResetFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkCommandBufferResetFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkCommandBufferResetFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkCommandBufferResetFlags(const VkCommandBufferResetFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkCommandBufferResetFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkCommandBufferResetFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkStencilFaceFlags(VkStencilFaceFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkStencilFaceFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkStencilFaceFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkStencilFaceFlags(const VkStencilFaceFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkStencilFaceFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkStencilFaceFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkExtent2D(VkExtent2D obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.width, "width", 1);

     print_uint32_t(obj.height, "height", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExtent2D(const VkExtent2D * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->width, "width", 1);

     print_uint32_t(obj->height, "height", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExtent3D(VkExtent3D obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.width, "width", 1);

     print_uint32_t(obj.height, "height", 1);

     print_uint32_t(obj.depth, "depth", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExtent3D(const VkExtent3D * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->width, "width", 1);

     print_uint32_t(obj->height, "height", 1);

     print_uint32_t(obj->depth, "depth", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkOffset2D(VkOffset2D obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_int32_t(obj.x, "x", 1);

     print_int32_t(obj.y, "y", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkOffset2D(const VkOffset2D * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_int32_t(obj->x, "x", 1);

     print_int32_t(obj->y, "y", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkOffset3D(VkOffset3D obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_int32_t(obj.x, "x", 1);

     print_int32_t(obj.y, "y", 1);

     print_int32_t(obj.z, "z", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkOffset3D(const VkOffset3D * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_int32_t(obj->x, "x", 1);

     print_int32_t(obj->y, "y", 1);

     print_int32_t(obj->z, "z", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkRect2D(VkRect2D obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"offset\": " << std::endl;
     {
           print_VkOffset2D(obj.offset, "offset", 1);
     }

     PRINT_SPACE
     _OUT << "\"extent\": " << std::endl;
     {
           print_VkExtent2D(obj.extent, "extent", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkRect2D(const VkRect2D * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"offset\": " << std::endl;
     {
           print_VkOffset2D(obj->offset, "offset", 1);
     }

     PRINT_SPACE
     _OUT << "\"extent\": " << std::endl;
     {
           print_VkExtent2D(obj->extent, "extent", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBaseInStructure(VkBaseInStructure obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBaseInStructure(const VkBaseInStructure * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBaseOutStructure(VkBaseOutStructure obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBaseOutStructure(const VkBaseOutStructure * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBufferMemoryBarrier(VkBufferMemoryBarrier obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkAccessFlags(obj.srcAccessMask, "srcAccessMask", 1);

     print_VkAccessFlags(obj.dstAccessMask, "dstAccessMask", 1);

     print_uint32_t(obj.srcQueueFamilyIndex, "srcQueueFamilyIndex", 1);

     print_uint32_t(obj.dstQueueFamilyIndex, "dstQueueFamilyIndex", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj.offset, "offset", 1);

     print_VkDeviceSize(obj.size, "size", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBufferMemoryBarrier(const VkBufferMemoryBarrier * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkAccessFlags(obj->srcAccessMask, "srcAccessMask", 1);

     print_VkAccessFlags(obj->dstAccessMask, "dstAccessMask", 1);

     print_uint32_t(obj->srcQueueFamilyIndex, "srcQueueFamilyIndex", 1);

     print_uint32_t(obj->dstQueueFamilyIndex, "dstQueueFamilyIndex", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj->offset, "offset", 1);

     print_VkDeviceSize(obj->size, "size", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDispatchIndirectCommand(VkDispatchIndirectCommand obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.x, "x", 1);

     print_uint32_t(obj.y, "y", 1);

     print_uint32_t(obj.z, "z", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDispatchIndirectCommand(const VkDispatchIndirectCommand * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->x, "x", 1);

     print_uint32_t(obj->y, "y", 1);

     print_uint32_t(obj->z, "z", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDrawIndexedIndirectCommand(VkDrawIndexedIndirectCommand obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.indexCount, "indexCount", 1);

     print_uint32_t(obj.instanceCount, "instanceCount", 1);

     print_uint32_t(obj.firstIndex, "firstIndex", 1);

     print_int32_t(obj.vertexOffset, "vertexOffset", 1);

     print_uint32_t(obj.firstInstance, "firstInstance", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDrawIndexedIndirectCommand(const VkDrawIndexedIndirectCommand * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->indexCount, "indexCount", 1);

     print_uint32_t(obj->instanceCount, "instanceCount", 1);

     print_uint32_t(obj->firstIndex, "firstIndex", 1);

     print_int32_t(obj->vertexOffset, "vertexOffset", 1);

     print_uint32_t(obj->firstInstance, "firstInstance", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDrawIndirectCommand(VkDrawIndirectCommand obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.vertexCount, "vertexCount", 1);

     print_uint32_t(obj.instanceCount, "instanceCount", 1);

     print_uint32_t(obj.firstVertex, "firstVertex", 1);

     print_uint32_t(obj.firstInstance, "firstInstance", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDrawIndirectCommand(const VkDrawIndirectCommand * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->vertexCount, "vertexCount", 1);

     print_uint32_t(obj->instanceCount, "instanceCount", 1);

     print_uint32_t(obj->firstVertex, "firstVertex", 1);

     print_uint32_t(obj->firstInstance, "firstInstance", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageSubresourceRange(VkImageSubresourceRange obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkImageAspectFlags(obj.aspectMask, "aspectMask", 1);

     print_uint32_t(obj.baseMipLevel, "baseMipLevel", 1);

     print_uint32_t(obj.levelCount, "levelCount", 1);

     print_uint32_t(obj.baseArrayLayer, "baseArrayLayer", 1);

     print_uint32_t(obj.layerCount, "layerCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageSubresourceRange(const VkImageSubresourceRange * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkImageAspectFlags(obj->aspectMask, "aspectMask", 1);

     print_uint32_t(obj->baseMipLevel, "baseMipLevel", 1);

     print_uint32_t(obj->levelCount, "levelCount", 1);

     print_uint32_t(obj->baseArrayLayer, "baseArrayLayer", 1);

     print_uint32_t(obj->layerCount, "layerCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageMemoryBarrier(VkImageMemoryBarrier obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkAccessFlags(obj.srcAccessMask, "srcAccessMask", 1);

     print_VkAccessFlags(obj.dstAccessMask, "dstAccessMask", 1);

     print_VkImageLayout(obj.oldLayout, "oldLayout", 1);

     print_VkImageLayout(obj.newLayout, "newLayout", 1);

     print_uint32_t(obj.srcQueueFamilyIndex, "srcQueueFamilyIndex", 1);

     print_uint32_t(obj.dstQueueFamilyIndex, "dstQueueFamilyIndex", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     PRINT_SPACE
     _OUT << "\"subresourceRange\": " << std::endl;
     {
           print_VkImageSubresourceRange(obj.subresourceRange, "subresourceRange", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageMemoryBarrier(const VkImageMemoryBarrier * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkAccessFlags(obj->srcAccessMask, "srcAccessMask", 1);

     print_VkAccessFlags(obj->dstAccessMask, "dstAccessMask", 1);

     print_VkImageLayout(obj->oldLayout, "oldLayout", 1);

     print_VkImageLayout(obj->newLayout, "newLayout", 1);

     print_uint32_t(obj->srcQueueFamilyIndex, "srcQueueFamilyIndex", 1);

     print_uint32_t(obj->dstQueueFamilyIndex, "dstQueueFamilyIndex", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     PRINT_SPACE
     _OUT << "\"subresourceRange\": " << std::endl;
     {
           print_VkImageSubresourceRange(obj->subresourceRange, "subresourceRange", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkMemoryBarrier(VkMemoryBarrier obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkAccessFlags(obj.srcAccessMask, "srcAccessMask", 1);

     print_VkAccessFlags(obj.dstAccessMask, "dstAccessMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkMemoryBarrier(const VkMemoryBarrier * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkAccessFlags(obj->srcAccessMask, "srcAccessMask", 1);

     print_VkAccessFlags(obj->dstAccessMask, "dstAccessMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineCacheHeaderVersionOne(VkPipelineCacheHeaderVersionOne obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.headerSize, "headerSize", 1);

     print_VkPipelineCacheHeaderVersion(obj.headerVersion, "headerVersion", 1);

     print_uint32_t(obj.vendorID, "vendorID", 1);

     print_uint32_t(obj.deviceID, "deviceID", 1);

     PRINT_SPACE
     _OUT << "\"pipelineCacheUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj.pipelineCacheUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineCacheHeaderVersionOne(const VkPipelineCacheHeaderVersionOne * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->headerSize, "headerSize", 1);

     print_VkPipelineCacheHeaderVersion(obj->headerVersion, "headerVersion", 1);

     print_uint32_t(obj->vendorID, "vendorID", 1);

     print_uint32_t(obj->deviceID, "deviceID", 1);

     PRINT_SPACE
     _OUT << "\"pipelineCacheUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj->pipelineCacheUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkAllocationCallbacks(VkAllocationCallbacks obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: Ignoring void* data. **/

     /** Note: Ignoring function pointer (PFN_vkAllocationFunction). **/

     /** Note: Ignoring function pointer (PFN_vkReallocationFunction). **/

     /** Note: Ignoring function pointer (PFN_vkFreeFunction). **/

     /** Note: Ignoring function pointer (PFN_vkInternalAllocationNotification). **/

     /** Note: Ignoring function pointer (PFN_vkInternalFreeNotification). **/

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkAllocationCallbacks(const VkAllocationCallbacks * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: Ignoring void* data. **/

     /** Note: Ignoring function pointer (PFN_vkAllocationFunction). **/

     /** Note: Ignoring function pointer (PFN_vkReallocationFunction). **/

     /** Note: Ignoring function pointer (PFN_vkFreeFunction). **/

     /** Note: Ignoring function pointer (PFN_vkInternalAllocationNotification). **/

     /** Note: Ignoring function pointer (PFN_vkInternalFreeNotification). **/

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkApplicationInfo(VkApplicationInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_char(obj.pApplicationName, "pApplicationName", 1);

     print_uint32_t(obj.applicationVersion, "applicationVersion", 1);

     print_char(obj.pEngineName, "pEngineName", 1);

     print_uint32_t(obj.engineVersion, "engineVersion", 1);

     print_uint32_t(obj.apiVersion, "apiVersion", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkApplicationInfo(const VkApplicationInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_char(obj->pApplicationName, "pApplicationName", 1);

     print_uint32_t(obj->applicationVersion, "applicationVersion", 1);

     print_char(obj->pEngineName, "pEngineName", 1);

     print_uint32_t(obj->engineVersion, "engineVersion", 1);

     print_uint32_t(obj->apiVersion, "apiVersion", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkFormatProperties(VkFormatProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkFormatFeatureFlags(obj.linearTilingFeatures, "linearTilingFeatures", 1);

     print_VkFormatFeatureFlags(obj.optimalTilingFeatures, "optimalTilingFeatures", 1);

     print_VkFormatFeatureFlags(obj.bufferFeatures, "bufferFeatures", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkFormatProperties(const VkFormatProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkFormatFeatureFlags(obj->linearTilingFeatures, "linearTilingFeatures", 1);

     print_VkFormatFeatureFlags(obj->optimalTilingFeatures, "optimalTilingFeatures", 1);

     print_VkFormatFeatureFlags(obj->bufferFeatures, "bufferFeatures", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageFormatProperties(VkImageFormatProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"maxExtent\": " << std::endl;
     {
           print_VkExtent3D(obj.maxExtent, "maxExtent", 1);
     }

     print_uint32_t(obj.maxMipLevels, "maxMipLevels", 1);

     print_uint32_t(obj.maxArrayLayers, "maxArrayLayers", 1);

     print_VkSampleCountFlags(obj.sampleCounts, "sampleCounts", 1);

     print_VkDeviceSize(obj.maxResourceSize, "maxResourceSize", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageFormatProperties(const VkImageFormatProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"maxExtent\": " << std::endl;
     {
           print_VkExtent3D(obj->maxExtent, "maxExtent", 1);
     }

     print_uint32_t(obj->maxMipLevels, "maxMipLevels", 1);

     print_uint32_t(obj->maxArrayLayers, "maxArrayLayers", 1);

     print_VkSampleCountFlags(obj->sampleCounts, "sampleCounts", 1);

     print_VkDeviceSize(obj->maxResourceSize, "maxResourceSize", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkInstanceCreateInfo(VkInstanceCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkInstanceCreateFlags(obj.flags, "flags", 1);

     PRINT_SPACE
     _OUT << "\"pApplicationInfo\": " << std::endl;
     if (obj.pApplicationInfo) {
           print_VkApplicationInfo(obj.pApplicationInfo, "pApplicationInfo", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.enabledLayerCount, "enabledLayerCount", 1);

     print_char(obj.ppEnabledLayerNames, "ppEnabledLayerNames", 1);

     print_uint32_t(obj.enabledExtensionCount, "enabledExtensionCount", 1);

     print_char(obj.ppEnabledExtensionNames, "ppEnabledExtensionNames", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkInstanceCreateInfo(const VkInstanceCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkInstanceCreateFlags(obj->flags, "flags", 1);

     PRINT_SPACE
     _OUT << "\"pApplicationInfo\": " << std::endl;
     if (obj->pApplicationInfo) {
           print_VkApplicationInfo(obj->pApplicationInfo, "pApplicationInfo", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->enabledLayerCount, "enabledLayerCount", 1);

     print_char(obj->ppEnabledLayerNames, "ppEnabledLayerNames", 1);

     print_uint32_t(obj->enabledExtensionCount, "enabledExtensionCount", 1);

     print_char(obj->ppEnabledExtensionNames, "ppEnabledExtensionNames", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkMemoryHeap(VkMemoryHeap obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj.size, "size", 1);

     print_VkMemoryHeapFlags(obj.flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkMemoryHeap(const VkMemoryHeap * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj->size, "size", 1);

     print_VkMemoryHeapFlags(obj->flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkMemoryType(VkMemoryType obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkMemoryPropertyFlags(obj.propertyFlags, "propertyFlags", 1);

     print_uint32_t(obj.heapIndex, "heapIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkMemoryType(const VkMemoryType * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkMemoryPropertyFlags(obj->propertyFlags, "propertyFlags", 1);

     print_uint32_t(obj->heapIndex, "heapIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceFeatures(VkPhysicalDeviceFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkBool32(obj.robustBufferAccess, "robustBufferAccess", 1);

     print_VkBool32(obj.fullDrawIndexUint32, "fullDrawIndexUint32", 1);

     print_VkBool32(obj.imageCubeArray, "imageCubeArray", 1);

     print_VkBool32(obj.independentBlend, "independentBlend", 1);

     print_VkBool32(obj.geometryShader, "geometryShader", 1);

     print_VkBool32(obj.tessellationShader, "tessellationShader", 1);

     print_VkBool32(obj.sampleRateShading, "sampleRateShading", 1);

     print_VkBool32(obj.dualSrcBlend, "dualSrcBlend", 1);

     print_VkBool32(obj.logicOp, "logicOp", 1);

     print_VkBool32(obj.multiDrawIndirect, "multiDrawIndirect", 1);

     print_VkBool32(obj.drawIndirectFirstInstance, "drawIndirectFirstInstance", 1);

     print_VkBool32(obj.depthClamp, "depthClamp", 1);

     print_VkBool32(obj.depthBiasClamp, "depthBiasClamp", 1);

     print_VkBool32(obj.fillModeNonSolid, "fillModeNonSolid", 1);

     print_VkBool32(obj.depthBounds, "depthBounds", 1);

     print_VkBool32(obj.wideLines, "wideLines", 1);

     print_VkBool32(obj.largePoints, "largePoints", 1);

     print_VkBool32(obj.alphaToOne, "alphaToOne", 1);

     print_VkBool32(obj.multiViewport, "multiViewport", 1);

     print_VkBool32(obj.samplerAnisotropy, "samplerAnisotropy", 1);

     print_VkBool32(obj.textureCompressionETC2, "textureCompressionETC2", 1);

     print_VkBool32(obj.textureCompressionASTC_LDR, "textureCompressionASTC_LDR", 1);

     print_VkBool32(obj.textureCompressionBC, "textureCompressionBC", 1);

     print_VkBool32(obj.occlusionQueryPrecise, "occlusionQueryPrecise", 1);

     print_VkBool32(obj.pipelineStatisticsQuery, "pipelineStatisticsQuery", 1);

     print_VkBool32(obj.vertexPipelineStoresAndAtomics, "vertexPipelineStoresAndAtomics", 1);

     print_VkBool32(obj.fragmentStoresAndAtomics, "fragmentStoresAndAtomics", 1);

     print_VkBool32(obj.shaderTessellationAndGeometryPointSize, "shaderTessellationAndGeometryPointSize", 1);

     print_VkBool32(obj.shaderImageGatherExtended, "shaderImageGatherExtended", 1);

     print_VkBool32(obj.shaderStorageImageExtendedFormats, "shaderStorageImageExtendedFormats", 1);

     print_VkBool32(obj.shaderStorageImageMultisample, "shaderStorageImageMultisample", 1);

     print_VkBool32(obj.shaderStorageImageReadWithoutFormat, "shaderStorageImageReadWithoutFormat", 1);

     print_VkBool32(obj.shaderStorageImageWriteWithoutFormat, "shaderStorageImageWriteWithoutFormat", 1);

     print_VkBool32(obj.shaderUniformBufferArrayDynamicIndexing, "shaderUniformBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj.shaderSampledImageArrayDynamicIndexing, "shaderSampledImageArrayDynamicIndexing", 1);

     print_VkBool32(obj.shaderStorageBufferArrayDynamicIndexing, "shaderStorageBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj.shaderStorageImageArrayDynamicIndexing, "shaderStorageImageArrayDynamicIndexing", 1);

     print_VkBool32(obj.shaderClipDistance, "shaderClipDistance", 1);

     print_VkBool32(obj.shaderCullDistance, "shaderCullDistance", 1);

     print_VkBool32(obj.shaderFloat64, "shaderFloat64", 1);

     print_VkBool32(obj.shaderInt64, "shaderInt64", 1);

     print_VkBool32(obj.shaderInt16, "shaderInt16", 1);

     print_VkBool32(obj.shaderResourceResidency, "shaderResourceResidency", 1);

     print_VkBool32(obj.shaderResourceMinLod, "shaderResourceMinLod", 1);

     print_VkBool32(obj.sparseBinding, "sparseBinding", 1);

     print_VkBool32(obj.sparseResidencyBuffer, "sparseResidencyBuffer", 1);

     print_VkBool32(obj.sparseResidencyImage2D, "sparseResidencyImage2D", 1);

     print_VkBool32(obj.sparseResidencyImage3D, "sparseResidencyImage3D", 1);

     print_VkBool32(obj.sparseResidency2Samples, "sparseResidency2Samples", 1);

     print_VkBool32(obj.sparseResidency4Samples, "sparseResidency4Samples", 1);

     print_VkBool32(obj.sparseResidency8Samples, "sparseResidency8Samples", 1);

     print_VkBool32(obj.sparseResidency16Samples, "sparseResidency16Samples", 1);

     print_VkBool32(obj.sparseResidencyAliased, "sparseResidencyAliased", 1);

     print_VkBool32(obj.variableMultisampleRate, "variableMultisampleRate", 1);

     print_VkBool32(obj.inheritedQueries, "inheritedQueries", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkBool32(obj->robustBufferAccess, "robustBufferAccess", 1);

     print_VkBool32(obj->fullDrawIndexUint32, "fullDrawIndexUint32", 1);

     print_VkBool32(obj->imageCubeArray, "imageCubeArray", 1);

     print_VkBool32(obj->independentBlend, "independentBlend", 1);

     print_VkBool32(obj->geometryShader, "geometryShader", 1);

     print_VkBool32(obj->tessellationShader, "tessellationShader", 1);

     print_VkBool32(obj->sampleRateShading, "sampleRateShading", 1);

     print_VkBool32(obj->dualSrcBlend, "dualSrcBlend", 1);

     print_VkBool32(obj->logicOp, "logicOp", 1);

     print_VkBool32(obj->multiDrawIndirect, "multiDrawIndirect", 1);

     print_VkBool32(obj->drawIndirectFirstInstance, "drawIndirectFirstInstance", 1);

     print_VkBool32(obj->depthClamp, "depthClamp", 1);

     print_VkBool32(obj->depthBiasClamp, "depthBiasClamp", 1);

     print_VkBool32(obj->fillModeNonSolid, "fillModeNonSolid", 1);

     print_VkBool32(obj->depthBounds, "depthBounds", 1);

     print_VkBool32(obj->wideLines, "wideLines", 1);

     print_VkBool32(obj->largePoints, "largePoints", 1);

     print_VkBool32(obj->alphaToOne, "alphaToOne", 1);

     print_VkBool32(obj->multiViewport, "multiViewport", 1);

     print_VkBool32(obj->samplerAnisotropy, "samplerAnisotropy", 1);

     print_VkBool32(obj->textureCompressionETC2, "textureCompressionETC2", 1);

     print_VkBool32(obj->textureCompressionASTC_LDR, "textureCompressionASTC_LDR", 1);

     print_VkBool32(obj->textureCompressionBC, "textureCompressionBC", 1);

     print_VkBool32(obj->occlusionQueryPrecise, "occlusionQueryPrecise", 1);

     print_VkBool32(obj->pipelineStatisticsQuery, "pipelineStatisticsQuery", 1);

     print_VkBool32(obj->vertexPipelineStoresAndAtomics, "vertexPipelineStoresAndAtomics", 1);

     print_VkBool32(obj->fragmentStoresAndAtomics, "fragmentStoresAndAtomics", 1);

     print_VkBool32(obj->shaderTessellationAndGeometryPointSize, "shaderTessellationAndGeometryPointSize", 1);

     print_VkBool32(obj->shaderImageGatherExtended, "shaderImageGatherExtended", 1);

     print_VkBool32(obj->shaderStorageImageExtendedFormats, "shaderStorageImageExtendedFormats", 1);

     print_VkBool32(obj->shaderStorageImageMultisample, "shaderStorageImageMultisample", 1);

     print_VkBool32(obj->shaderStorageImageReadWithoutFormat, "shaderStorageImageReadWithoutFormat", 1);

     print_VkBool32(obj->shaderStorageImageWriteWithoutFormat, "shaderStorageImageWriteWithoutFormat", 1);

     print_VkBool32(obj->shaderUniformBufferArrayDynamicIndexing, "shaderUniformBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj->shaderSampledImageArrayDynamicIndexing, "shaderSampledImageArrayDynamicIndexing", 1);

     print_VkBool32(obj->shaderStorageBufferArrayDynamicIndexing, "shaderStorageBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj->shaderStorageImageArrayDynamicIndexing, "shaderStorageImageArrayDynamicIndexing", 1);

     print_VkBool32(obj->shaderClipDistance, "shaderClipDistance", 1);

     print_VkBool32(obj->shaderCullDistance, "shaderCullDistance", 1);

     print_VkBool32(obj->shaderFloat64, "shaderFloat64", 1);

     print_VkBool32(obj->shaderInt64, "shaderInt64", 1);

     print_VkBool32(obj->shaderInt16, "shaderInt16", 1);

     print_VkBool32(obj->shaderResourceResidency, "shaderResourceResidency", 1);

     print_VkBool32(obj->shaderResourceMinLod, "shaderResourceMinLod", 1);

     print_VkBool32(obj->sparseBinding, "sparseBinding", 1);

     print_VkBool32(obj->sparseResidencyBuffer, "sparseResidencyBuffer", 1);

     print_VkBool32(obj->sparseResidencyImage2D, "sparseResidencyImage2D", 1);

     print_VkBool32(obj->sparseResidencyImage3D, "sparseResidencyImage3D", 1);

     print_VkBool32(obj->sparseResidency2Samples, "sparseResidency2Samples", 1);

     print_VkBool32(obj->sparseResidency4Samples, "sparseResidency4Samples", 1);

     print_VkBool32(obj->sparseResidency8Samples, "sparseResidency8Samples", 1);

     print_VkBool32(obj->sparseResidency16Samples, "sparseResidency16Samples", 1);

     print_VkBool32(obj->sparseResidencyAliased, "sparseResidencyAliased", 1);

     print_VkBool32(obj->variableMultisampleRate, "variableMultisampleRate", 1);

     print_VkBool32(obj->inheritedQueries, "inheritedQueries", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceLimits(VkPhysicalDeviceLimits obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.maxImageDimension1D, "maxImageDimension1D", 1);

     print_uint32_t(obj.maxImageDimension2D, "maxImageDimension2D", 1);

     print_uint32_t(obj.maxImageDimension3D, "maxImageDimension3D", 1);

     print_uint32_t(obj.maxImageDimensionCube, "maxImageDimensionCube", 1);

     print_uint32_t(obj.maxImageArrayLayers, "maxImageArrayLayers", 1);

     print_uint32_t(obj.maxTexelBufferElements, "maxTexelBufferElements", 1);

     print_uint32_t(obj.maxUniformBufferRange, "maxUniformBufferRange", 1);

     print_uint32_t(obj.maxStorageBufferRange, "maxStorageBufferRange", 1);

     print_uint32_t(obj.maxPushConstantsSize, "maxPushConstantsSize", 1);

     print_uint32_t(obj.maxMemoryAllocationCount, "maxMemoryAllocationCount", 1);

     print_uint32_t(obj.maxSamplerAllocationCount, "maxSamplerAllocationCount", 1);

     print_VkDeviceSize(obj.bufferImageGranularity, "bufferImageGranularity", 1);

     print_VkDeviceSize(obj.sparseAddressSpaceSize, "sparseAddressSpaceSize", 1);

     print_uint32_t(obj.maxBoundDescriptorSets, "maxBoundDescriptorSets", 1);

     print_uint32_t(obj.maxPerStageDescriptorSamplers, "maxPerStageDescriptorSamplers", 1);

     print_uint32_t(obj.maxPerStageDescriptorUniformBuffers, "maxPerStageDescriptorUniformBuffers", 1);

     print_uint32_t(obj.maxPerStageDescriptorStorageBuffers, "maxPerStageDescriptorStorageBuffers", 1);

     print_uint32_t(obj.maxPerStageDescriptorSampledImages, "maxPerStageDescriptorSampledImages", 1);

     print_uint32_t(obj.maxPerStageDescriptorStorageImages, "maxPerStageDescriptorStorageImages", 1);

     print_uint32_t(obj.maxPerStageDescriptorInputAttachments, "maxPerStageDescriptorInputAttachments", 1);

     print_uint32_t(obj.maxPerStageResources, "maxPerStageResources", 1);

     print_uint32_t(obj.maxDescriptorSetSamplers, "maxDescriptorSetSamplers", 1);

     print_uint32_t(obj.maxDescriptorSetUniformBuffers, "maxDescriptorSetUniformBuffers", 1);

     print_uint32_t(obj.maxDescriptorSetUniformBuffersDynamic, "maxDescriptorSetUniformBuffersDynamic", 1);

     print_uint32_t(obj.maxDescriptorSetStorageBuffers, "maxDescriptorSetStorageBuffers", 1);

     print_uint32_t(obj.maxDescriptorSetStorageBuffersDynamic, "maxDescriptorSetStorageBuffersDynamic", 1);

     print_uint32_t(obj.maxDescriptorSetSampledImages, "maxDescriptorSetSampledImages", 1);

     print_uint32_t(obj.maxDescriptorSetStorageImages, "maxDescriptorSetStorageImages", 1);

     print_uint32_t(obj.maxDescriptorSetInputAttachments, "maxDescriptorSetInputAttachments", 1);

     print_uint32_t(obj.maxVertexInputAttributes, "maxVertexInputAttributes", 1);

     print_uint32_t(obj.maxVertexInputBindings, "maxVertexInputBindings", 1);

     print_uint32_t(obj.maxVertexInputAttributeOffset, "maxVertexInputAttributeOffset", 1);

     print_uint32_t(obj.maxVertexInputBindingStride, "maxVertexInputBindingStride", 1);

     print_uint32_t(obj.maxVertexOutputComponents, "maxVertexOutputComponents", 1);

     print_uint32_t(obj.maxTessellationGenerationLevel, "maxTessellationGenerationLevel", 1);

     print_uint32_t(obj.maxTessellationPatchSize, "maxTessellationPatchSize", 1);

     print_uint32_t(obj.maxTessellationControlPerVertexInputComponents, "maxTessellationControlPerVertexInputComponents", 1);

     print_uint32_t(obj.maxTessellationControlPerVertexOutputComponents, "maxTessellationControlPerVertexOutputComponents", 1);

     print_uint32_t(obj.maxTessellationControlPerPatchOutputComponents, "maxTessellationControlPerPatchOutputComponents", 1);

     print_uint32_t(obj.maxTessellationControlTotalOutputComponents, "maxTessellationControlTotalOutputComponents", 1);

     print_uint32_t(obj.maxTessellationEvaluationInputComponents, "maxTessellationEvaluationInputComponents", 1);

     print_uint32_t(obj.maxTessellationEvaluationOutputComponents, "maxTessellationEvaluationOutputComponents", 1);

     print_uint32_t(obj.maxGeometryShaderInvocations, "maxGeometryShaderInvocations", 1);

     print_uint32_t(obj.maxGeometryInputComponents, "maxGeometryInputComponents", 1);

     print_uint32_t(obj.maxGeometryOutputComponents, "maxGeometryOutputComponents", 1);

     print_uint32_t(obj.maxGeometryOutputVertices, "maxGeometryOutputVertices", 1);

     print_uint32_t(obj.maxGeometryTotalOutputComponents, "maxGeometryTotalOutputComponents", 1);

     print_uint32_t(obj.maxFragmentInputComponents, "maxFragmentInputComponents", 1);

     print_uint32_t(obj.maxFragmentOutputAttachments, "maxFragmentOutputAttachments", 1);

     print_uint32_t(obj.maxFragmentDualSrcAttachments, "maxFragmentDualSrcAttachments", 1);

     print_uint32_t(obj.maxFragmentCombinedOutputResources, "maxFragmentCombinedOutputResources", 1);

     print_uint32_t(obj.maxComputeSharedMemorySize, "maxComputeSharedMemorySize", 1);

     PRINT_SPACE
     _OUT << "\"maxComputeWorkGroupCount\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 3; i++) {
           bool isCommaNeeded = (i+1) != 3;
           print_uint32_t(obj.maxComputeWorkGroupCount[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj.maxComputeWorkGroupInvocations, "maxComputeWorkGroupInvocations", 1);

     PRINT_SPACE
     _OUT << "\"maxComputeWorkGroupSize\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 3; i++) {
           bool isCommaNeeded = (i+1) != 3;
           print_uint32_t(obj.maxComputeWorkGroupSize[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj.subPixelPrecisionBits, "subPixelPrecisionBits", 1);

     print_uint32_t(obj.subTexelPrecisionBits, "subTexelPrecisionBits", 1);

     print_uint32_t(obj.mipmapPrecisionBits, "mipmapPrecisionBits", 1);

     print_uint32_t(obj.maxDrawIndexedIndexValue, "maxDrawIndexedIndexValue", 1);

     print_uint32_t(obj.maxDrawIndirectCount, "maxDrawIndirectCount", 1);

     print_float(obj.maxSamplerLodBias, "maxSamplerLodBias", 1);

     print_float(obj.maxSamplerAnisotropy, "maxSamplerAnisotropy", 1);

     print_uint32_t(obj.maxViewports, "maxViewports", 1);

     PRINT_SPACE
     _OUT << "\"maxViewportDimensions\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_uint32_t(obj.maxViewportDimensions[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"viewportBoundsRange\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_float(obj.viewportBoundsRange[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj.viewportSubPixelBits, "viewportSubPixelBits", 1);

     print_size_t(obj.minMemoryMapAlignment, "minMemoryMapAlignment", 1);

     print_VkDeviceSize(obj.minTexelBufferOffsetAlignment, "minTexelBufferOffsetAlignment", 1);

     print_VkDeviceSize(obj.minUniformBufferOffsetAlignment, "minUniformBufferOffsetAlignment", 1);

     print_VkDeviceSize(obj.minStorageBufferOffsetAlignment, "minStorageBufferOffsetAlignment", 1);

     print_int32_t(obj.minTexelOffset, "minTexelOffset", 1);

     print_uint32_t(obj.maxTexelOffset, "maxTexelOffset", 1);

     print_int32_t(obj.minTexelGatherOffset, "minTexelGatherOffset", 1);

     print_uint32_t(obj.maxTexelGatherOffset, "maxTexelGatherOffset", 1);

     print_float(obj.minInterpolationOffset, "minInterpolationOffset", 1);

     print_float(obj.maxInterpolationOffset, "maxInterpolationOffset", 1);

     print_uint32_t(obj.subPixelInterpolationOffsetBits, "subPixelInterpolationOffsetBits", 1);

     print_uint32_t(obj.maxFramebufferWidth, "maxFramebufferWidth", 1);

     print_uint32_t(obj.maxFramebufferHeight, "maxFramebufferHeight", 1);

     print_uint32_t(obj.maxFramebufferLayers, "maxFramebufferLayers", 1);

     print_VkSampleCountFlags(obj.framebufferColorSampleCounts, "framebufferColorSampleCounts", 1);

     print_VkSampleCountFlags(obj.framebufferDepthSampleCounts, "framebufferDepthSampleCounts", 1);

     print_VkSampleCountFlags(obj.framebufferStencilSampleCounts, "framebufferStencilSampleCounts", 1);

     print_VkSampleCountFlags(obj.framebufferNoAttachmentsSampleCounts, "framebufferNoAttachmentsSampleCounts", 1);

     print_uint32_t(obj.maxColorAttachments, "maxColorAttachments", 1);

     print_VkSampleCountFlags(obj.sampledImageColorSampleCounts, "sampledImageColorSampleCounts", 1);

     print_VkSampleCountFlags(obj.sampledImageIntegerSampleCounts, "sampledImageIntegerSampleCounts", 1);

     print_VkSampleCountFlags(obj.sampledImageDepthSampleCounts, "sampledImageDepthSampleCounts", 1);

     print_VkSampleCountFlags(obj.sampledImageStencilSampleCounts, "sampledImageStencilSampleCounts", 1);

     print_VkSampleCountFlags(obj.storageImageSampleCounts, "storageImageSampleCounts", 1);

     print_uint32_t(obj.maxSampleMaskWords, "maxSampleMaskWords", 1);

     print_VkBool32(obj.timestampComputeAndGraphics, "timestampComputeAndGraphics", 1);

     print_float(obj.timestampPeriod, "timestampPeriod", 1);

     print_uint32_t(obj.maxClipDistances, "maxClipDistances", 1);

     print_uint32_t(obj.maxCullDistances, "maxCullDistances", 1);

     print_uint32_t(obj.maxCombinedClipAndCullDistances, "maxCombinedClipAndCullDistances", 1);

     print_uint32_t(obj.discreteQueuePriorities, "discreteQueuePriorities", 1);

     PRINT_SPACE
     _OUT << "\"pointSizeRange\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_float(obj.pointSizeRange[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"lineWidthRange\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_float(obj.lineWidthRange[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_float(obj.pointSizeGranularity, "pointSizeGranularity", 1);

     print_float(obj.lineWidthGranularity, "lineWidthGranularity", 1);

     print_VkBool32(obj.strictLines, "strictLines", 1);

     print_VkBool32(obj.standardSampleLocations, "standardSampleLocations", 1);

     print_VkDeviceSize(obj.optimalBufferCopyOffsetAlignment, "optimalBufferCopyOffsetAlignment", 1);

     print_VkDeviceSize(obj.optimalBufferCopyRowPitchAlignment, "optimalBufferCopyRowPitchAlignment", 1);

     print_VkDeviceSize(obj.nonCoherentAtomSize, "nonCoherentAtomSize", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceLimits(const VkPhysicalDeviceLimits * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->maxImageDimension1D, "maxImageDimension1D", 1);

     print_uint32_t(obj->maxImageDimension2D, "maxImageDimension2D", 1);

     print_uint32_t(obj->maxImageDimension3D, "maxImageDimension3D", 1);

     print_uint32_t(obj->maxImageDimensionCube, "maxImageDimensionCube", 1);

     print_uint32_t(obj->maxImageArrayLayers, "maxImageArrayLayers", 1);

     print_uint32_t(obj->maxTexelBufferElements, "maxTexelBufferElements", 1);

     print_uint32_t(obj->maxUniformBufferRange, "maxUniformBufferRange", 1);

     print_uint32_t(obj->maxStorageBufferRange, "maxStorageBufferRange", 1);

     print_uint32_t(obj->maxPushConstantsSize, "maxPushConstantsSize", 1);

     print_uint32_t(obj->maxMemoryAllocationCount, "maxMemoryAllocationCount", 1);

     print_uint32_t(obj->maxSamplerAllocationCount, "maxSamplerAllocationCount", 1);

     print_VkDeviceSize(obj->bufferImageGranularity, "bufferImageGranularity", 1);

     print_VkDeviceSize(obj->sparseAddressSpaceSize, "sparseAddressSpaceSize", 1);

     print_uint32_t(obj->maxBoundDescriptorSets, "maxBoundDescriptorSets", 1);

     print_uint32_t(obj->maxPerStageDescriptorSamplers, "maxPerStageDescriptorSamplers", 1);

     print_uint32_t(obj->maxPerStageDescriptorUniformBuffers, "maxPerStageDescriptorUniformBuffers", 1);

     print_uint32_t(obj->maxPerStageDescriptorStorageBuffers, "maxPerStageDescriptorStorageBuffers", 1);

     print_uint32_t(obj->maxPerStageDescriptorSampledImages, "maxPerStageDescriptorSampledImages", 1);

     print_uint32_t(obj->maxPerStageDescriptorStorageImages, "maxPerStageDescriptorStorageImages", 1);

     print_uint32_t(obj->maxPerStageDescriptorInputAttachments, "maxPerStageDescriptorInputAttachments", 1);

     print_uint32_t(obj->maxPerStageResources, "maxPerStageResources", 1);

     print_uint32_t(obj->maxDescriptorSetSamplers, "maxDescriptorSetSamplers", 1);

     print_uint32_t(obj->maxDescriptorSetUniformBuffers, "maxDescriptorSetUniformBuffers", 1);

     print_uint32_t(obj->maxDescriptorSetUniformBuffersDynamic, "maxDescriptorSetUniformBuffersDynamic", 1);

     print_uint32_t(obj->maxDescriptorSetStorageBuffers, "maxDescriptorSetStorageBuffers", 1);

     print_uint32_t(obj->maxDescriptorSetStorageBuffersDynamic, "maxDescriptorSetStorageBuffersDynamic", 1);

     print_uint32_t(obj->maxDescriptorSetSampledImages, "maxDescriptorSetSampledImages", 1);

     print_uint32_t(obj->maxDescriptorSetStorageImages, "maxDescriptorSetStorageImages", 1);

     print_uint32_t(obj->maxDescriptorSetInputAttachments, "maxDescriptorSetInputAttachments", 1);

     print_uint32_t(obj->maxVertexInputAttributes, "maxVertexInputAttributes", 1);

     print_uint32_t(obj->maxVertexInputBindings, "maxVertexInputBindings", 1);

     print_uint32_t(obj->maxVertexInputAttributeOffset, "maxVertexInputAttributeOffset", 1);

     print_uint32_t(obj->maxVertexInputBindingStride, "maxVertexInputBindingStride", 1);

     print_uint32_t(obj->maxVertexOutputComponents, "maxVertexOutputComponents", 1);

     print_uint32_t(obj->maxTessellationGenerationLevel, "maxTessellationGenerationLevel", 1);

     print_uint32_t(obj->maxTessellationPatchSize, "maxTessellationPatchSize", 1);

     print_uint32_t(obj->maxTessellationControlPerVertexInputComponents, "maxTessellationControlPerVertexInputComponents", 1);

     print_uint32_t(obj->maxTessellationControlPerVertexOutputComponents, "maxTessellationControlPerVertexOutputComponents", 1);

     print_uint32_t(obj->maxTessellationControlPerPatchOutputComponents, "maxTessellationControlPerPatchOutputComponents", 1);

     print_uint32_t(obj->maxTessellationControlTotalOutputComponents, "maxTessellationControlTotalOutputComponents", 1);

     print_uint32_t(obj->maxTessellationEvaluationInputComponents, "maxTessellationEvaluationInputComponents", 1);

     print_uint32_t(obj->maxTessellationEvaluationOutputComponents, "maxTessellationEvaluationOutputComponents", 1);

     print_uint32_t(obj->maxGeometryShaderInvocations, "maxGeometryShaderInvocations", 1);

     print_uint32_t(obj->maxGeometryInputComponents, "maxGeometryInputComponents", 1);

     print_uint32_t(obj->maxGeometryOutputComponents, "maxGeometryOutputComponents", 1);

     print_uint32_t(obj->maxGeometryOutputVertices, "maxGeometryOutputVertices", 1);

     print_uint32_t(obj->maxGeometryTotalOutputComponents, "maxGeometryTotalOutputComponents", 1);

     print_uint32_t(obj->maxFragmentInputComponents, "maxFragmentInputComponents", 1);

     print_uint32_t(obj->maxFragmentOutputAttachments, "maxFragmentOutputAttachments", 1);

     print_uint32_t(obj->maxFragmentDualSrcAttachments, "maxFragmentDualSrcAttachments", 1);

     print_uint32_t(obj->maxFragmentCombinedOutputResources, "maxFragmentCombinedOutputResources", 1);

     print_uint32_t(obj->maxComputeSharedMemorySize, "maxComputeSharedMemorySize", 1);

     PRINT_SPACE
     _OUT << "\"maxComputeWorkGroupCount\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 3; i++) {
           bool isCommaNeeded = (i+1) != 3;
           print_uint32_t(obj->maxComputeWorkGroupCount[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj->maxComputeWorkGroupInvocations, "maxComputeWorkGroupInvocations", 1);

     PRINT_SPACE
     _OUT << "\"maxComputeWorkGroupSize\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 3; i++) {
           bool isCommaNeeded = (i+1) != 3;
           print_uint32_t(obj->maxComputeWorkGroupSize[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj->subPixelPrecisionBits, "subPixelPrecisionBits", 1);

     print_uint32_t(obj->subTexelPrecisionBits, "subTexelPrecisionBits", 1);

     print_uint32_t(obj->mipmapPrecisionBits, "mipmapPrecisionBits", 1);

     print_uint32_t(obj->maxDrawIndexedIndexValue, "maxDrawIndexedIndexValue", 1);

     print_uint32_t(obj->maxDrawIndirectCount, "maxDrawIndirectCount", 1);

     print_float(obj->maxSamplerLodBias, "maxSamplerLodBias", 1);

     print_float(obj->maxSamplerAnisotropy, "maxSamplerAnisotropy", 1);

     print_uint32_t(obj->maxViewports, "maxViewports", 1);

     PRINT_SPACE
     _OUT << "\"maxViewportDimensions\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_uint32_t(obj->maxViewportDimensions[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"viewportBoundsRange\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_float(obj->viewportBoundsRange[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj->viewportSubPixelBits, "viewportSubPixelBits", 1);

     print_size_t(obj->minMemoryMapAlignment, "minMemoryMapAlignment", 1);

     print_VkDeviceSize(obj->minTexelBufferOffsetAlignment, "minTexelBufferOffsetAlignment", 1);

     print_VkDeviceSize(obj->minUniformBufferOffsetAlignment, "minUniformBufferOffsetAlignment", 1);

     print_VkDeviceSize(obj->minStorageBufferOffsetAlignment, "minStorageBufferOffsetAlignment", 1);

     print_int32_t(obj->minTexelOffset, "minTexelOffset", 1);

     print_uint32_t(obj->maxTexelOffset, "maxTexelOffset", 1);

     print_int32_t(obj->minTexelGatherOffset, "minTexelGatherOffset", 1);

     print_uint32_t(obj->maxTexelGatherOffset, "maxTexelGatherOffset", 1);

     print_float(obj->minInterpolationOffset, "minInterpolationOffset", 1);

     print_float(obj->maxInterpolationOffset, "maxInterpolationOffset", 1);

     print_uint32_t(obj->subPixelInterpolationOffsetBits, "subPixelInterpolationOffsetBits", 1);

     print_uint32_t(obj->maxFramebufferWidth, "maxFramebufferWidth", 1);

     print_uint32_t(obj->maxFramebufferHeight, "maxFramebufferHeight", 1);

     print_uint32_t(obj->maxFramebufferLayers, "maxFramebufferLayers", 1);

     print_VkSampleCountFlags(obj->framebufferColorSampleCounts, "framebufferColorSampleCounts", 1);

     print_VkSampleCountFlags(obj->framebufferDepthSampleCounts, "framebufferDepthSampleCounts", 1);

     print_VkSampleCountFlags(obj->framebufferStencilSampleCounts, "framebufferStencilSampleCounts", 1);

     print_VkSampleCountFlags(obj->framebufferNoAttachmentsSampleCounts, "framebufferNoAttachmentsSampleCounts", 1);

     print_uint32_t(obj->maxColorAttachments, "maxColorAttachments", 1);

     print_VkSampleCountFlags(obj->sampledImageColorSampleCounts, "sampledImageColorSampleCounts", 1);

     print_VkSampleCountFlags(obj->sampledImageIntegerSampleCounts, "sampledImageIntegerSampleCounts", 1);

     print_VkSampleCountFlags(obj->sampledImageDepthSampleCounts, "sampledImageDepthSampleCounts", 1);

     print_VkSampleCountFlags(obj->sampledImageStencilSampleCounts, "sampledImageStencilSampleCounts", 1);

     print_VkSampleCountFlags(obj->storageImageSampleCounts, "storageImageSampleCounts", 1);

     print_uint32_t(obj->maxSampleMaskWords, "maxSampleMaskWords", 1);

     print_VkBool32(obj->timestampComputeAndGraphics, "timestampComputeAndGraphics", 1);

     print_float(obj->timestampPeriod, "timestampPeriod", 1);

     print_uint32_t(obj->maxClipDistances, "maxClipDistances", 1);

     print_uint32_t(obj->maxCullDistances, "maxCullDistances", 1);

     print_uint32_t(obj->maxCombinedClipAndCullDistances, "maxCombinedClipAndCullDistances", 1);

     print_uint32_t(obj->discreteQueuePriorities, "discreteQueuePriorities", 1);

     PRINT_SPACE
     _OUT << "\"pointSizeRange\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_float(obj->pointSizeRange[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"lineWidthRange\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_float(obj->lineWidthRange[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_float(obj->pointSizeGranularity, "pointSizeGranularity", 1);

     print_float(obj->lineWidthGranularity, "lineWidthGranularity", 1);

     print_VkBool32(obj->strictLines, "strictLines", 1);

     print_VkBool32(obj->standardSampleLocations, "standardSampleLocations", 1);

     print_VkDeviceSize(obj->optimalBufferCopyOffsetAlignment, "optimalBufferCopyOffsetAlignment", 1);

     print_VkDeviceSize(obj->optimalBufferCopyRowPitchAlignment, "optimalBufferCopyRowPitchAlignment", 1);

     print_VkDeviceSize(obj->nonCoherentAtomSize, "nonCoherentAtomSize", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.memoryTypeCount, "memoryTypeCount", 1);

     PRINT_SPACE
     _OUT << "\"memoryTypes\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_MEMORY_TYPES;
           print_VkMemoryType(obj.memoryTypes[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj.memoryHeapCount, "memoryHeapCount", 1);

     PRINT_SPACE
     _OUT << "\"memoryHeaps\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_MEMORY_HEAPS; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_MEMORY_HEAPS;
           print_VkMemoryHeap(obj.memoryHeaps[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceMemoryProperties(const VkPhysicalDeviceMemoryProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->memoryTypeCount, "memoryTypeCount", 1);

     PRINT_SPACE
     _OUT << "\"memoryTypes\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_MEMORY_TYPES;
           print_VkMemoryType(obj->memoryTypes[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj->memoryHeapCount, "memoryHeapCount", 1);

     PRINT_SPACE
     _OUT << "\"memoryHeaps\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_MEMORY_HEAPS; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_MEMORY_HEAPS;
           print_VkMemoryHeap(obj->memoryHeaps[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceSparseProperties(VkPhysicalDeviceSparseProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkBool32(obj.residencyStandard2DBlockShape, "residencyStandard2DBlockShape", 1);

     print_VkBool32(obj.residencyStandard2DMultisampleBlockShape, "residencyStandard2DMultisampleBlockShape", 1);

     print_VkBool32(obj.residencyStandard3DBlockShape, "residencyStandard3DBlockShape", 1);

     print_VkBool32(obj.residencyAlignedMipSize, "residencyAlignedMipSize", 1);

     print_VkBool32(obj.residencyNonResidentStrict, "residencyNonResidentStrict", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceSparseProperties(const VkPhysicalDeviceSparseProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkBool32(obj->residencyStandard2DBlockShape, "residencyStandard2DBlockShape", 1);

     print_VkBool32(obj->residencyStandard2DMultisampleBlockShape, "residencyStandard2DMultisampleBlockShape", 1);

     print_VkBool32(obj->residencyStandard3DBlockShape, "residencyStandard3DBlockShape", 1);

     print_VkBool32(obj->residencyAlignedMipSize, "residencyAlignedMipSize", 1);

     print_VkBool32(obj->residencyNonResidentStrict, "residencyNonResidentStrict", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceProperties(VkPhysicalDeviceProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.apiVersion, "apiVersion", 1);

     print_uint32_t(obj.driverVersion, "driverVersion", 1);

     print_uint32_t(obj.vendorID, "vendorID", 1);

     print_uint32_t(obj.deviceID, "deviceID", 1);

     print_VkPhysicalDeviceType(obj.deviceType, "deviceType", 1);

     PRINT_SPACE
     _OUT << "\"deviceName\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_PHYSICAL_DEVICE_NAME_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_PHYSICAL_DEVICE_NAME_SIZE;
           print_char(obj.deviceName[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"pipelineCacheUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj.pipelineCacheUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"limits\": " << std::endl;
     {
           print_VkPhysicalDeviceLimits(obj.limits, "limits", 1);
     }

     PRINT_SPACE
     _OUT << "\"sparseProperties\": " << std::endl;
     {
           print_VkPhysicalDeviceSparseProperties(obj.sparseProperties, "sparseProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceProperties(const VkPhysicalDeviceProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->apiVersion, "apiVersion", 1);

     print_uint32_t(obj->driverVersion, "driverVersion", 1);

     print_uint32_t(obj->vendorID, "vendorID", 1);

     print_uint32_t(obj->deviceID, "deviceID", 1);

     print_VkPhysicalDeviceType(obj->deviceType, "deviceType", 1);

     PRINT_SPACE
     _OUT << "\"deviceName\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_PHYSICAL_DEVICE_NAME_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_PHYSICAL_DEVICE_NAME_SIZE;
           print_char(obj->deviceName[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"pipelineCacheUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj->pipelineCacheUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"limits\": " << std::endl;
     {
           print_VkPhysicalDeviceLimits(obj->limits, "limits", 1);
     }

     PRINT_SPACE
     _OUT << "\"sparseProperties\": " << std::endl;
     {
           print_VkPhysicalDeviceSparseProperties(obj->sparseProperties, "sparseProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkQueueFamilyProperties(VkQueueFamilyProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkQueueFlags(obj.queueFlags, "queueFlags", 1);

     print_uint32_t(obj.queueCount, "queueCount", 1);

     print_uint32_t(obj.timestampValidBits, "timestampValidBits", 1);

     PRINT_SPACE
     _OUT << "\"minImageTransferGranularity\": " << std::endl;
     {
           print_VkExtent3D(obj.minImageTransferGranularity, "minImageTransferGranularity", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkQueueFamilyProperties(const VkQueueFamilyProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkQueueFlags(obj->queueFlags, "queueFlags", 1);

     print_uint32_t(obj->queueCount, "queueCount", 1);

     print_uint32_t(obj->timestampValidBits, "timestampValidBits", 1);

     PRINT_SPACE
     _OUT << "\"minImageTransferGranularity\": " << std::endl;
     {
           print_VkExtent3D(obj->minImageTransferGranularity, "minImageTransferGranularity", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDeviceQueueCreateInfo(VkDeviceQueueCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDeviceQueueCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.queueFamilyIndex, "queueFamilyIndex", 1);

     print_uint32_t(obj.queueCount, "queueCount", 1);

     PRINT_SPACE
     _OUT << "\"pQueuePriorities\":" << std::endl;
     PRINT_SPACE
     if (obj.pQueuePriorities) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.queueCount; i++) {
           bool isCommaNeeded = (i+1) != obj.queueCount;
           print_float(obj.pQueuePriorities[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDeviceQueueCreateInfo(const VkDeviceQueueCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDeviceQueueCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->queueFamilyIndex, "queueFamilyIndex", 1);

     print_uint32_t(obj->queueCount, "queueCount", 1);

     PRINT_SPACE
     _OUT << "\"pQueuePriorities\":" << std::endl;
     PRINT_SPACE
     if (obj->pQueuePriorities) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->queueCount; i++) {
           bool isCommaNeeded = (i+1) != obj->queueCount;
           print_float(obj->pQueuePriorities[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDeviceCreateInfo(VkDeviceCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDeviceCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.queueCreateInfoCount, "queueCreateInfoCount", 1);

     PRINT_SPACE
     _OUT << "\"pQueueCreateInfos\": " << std::endl;
     if (obj.pQueueCreateInfos) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.queueCreateInfoCount; i++) {
           if (i+1 == obj.queueCreateInfoCount)
               print_VkDeviceQueueCreateInfo(obj.pQueueCreateInfos[i], "pQueueCreateInfos", 0);
           else
               print_VkDeviceQueueCreateInfo(obj.pQueueCreateInfos[i], "pQueueCreateInfos", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.enabledLayerCount, "enabledLayerCount", 1);

     print_char(obj.ppEnabledLayerNames, "ppEnabledLayerNames", 1);

     print_uint32_t(obj.enabledExtensionCount, "enabledExtensionCount", 1);

     print_char(obj.ppEnabledExtensionNames, "ppEnabledExtensionNames", 1);

     PRINT_SPACE
     _OUT << "\"pEnabledFeatures\": " << std::endl;
     if (obj.pEnabledFeatures) {
           print_VkPhysicalDeviceFeatures(obj.pEnabledFeatures, "pEnabledFeatures", 0);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDeviceCreateInfo(const VkDeviceCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDeviceCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->queueCreateInfoCount, "queueCreateInfoCount", 1);

     PRINT_SPACE
     _OUT << "\"pQueueCreateInfos\": " << std::endl;
     if (obj->pQueueCreateInfos) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->queueCreateInfoCount; i++) {
           if (i+1 == obj->queueCreateInfoCount)
               print_VkDeviceQueueCreateInfo(obj->pQueueCreateInfos[i], "pQueueCreateInfos", 0);
           else
               print_VkDeviceQueueCreateInfo(obj->pQueueCreateInfos[i], "pQueueCreateInfos", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->enabledLayerCount, "enabledLayerCount", 1);

     print_char(obj->ppEnabledLayerNames, "ppEnabledLayerNames", 1);

     print_uint32_t(obj->enabledExtensionCount, "enabledExtensionCount", 1);

     print_char(obj->ppEnabledExtensionNames, "ppEnabledExtensionNames", 1);

     PRINT_SPACE
     _OUT << "\"pEnabledFeatures\": " << std::endl;
     if (obj->pEnabledFeatures) {
           print_VkPhysicalDeviceFeatures(obj->pEnabledFeatures, "pEnabledFeatures", 0);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExtensionProperties(VkExtensionProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"extensionName\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_EXTENSION_NAME_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_EXTENSION_NAME_SIZE;
           print_char(obj.extensionName[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj.specVersion, "specVersion", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExtensionProperties(const VkExtensionProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"extensionName\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_EXTENSION_NAME_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_EXTENSION_NAME_SIZE;
           print_char(obj->extensionName[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj->specVersion, "specVersion", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkLayerProperties(VkLayerProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"layerName\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_EXTENSION_NAME_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_EXTENSION_NAME_SIZE;
           print_char(obj.layerName[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj.specVersion, "specVersion", 1);

     print_uint32_t(obj.implementationVersion, "implementationVersion", 1);

     PRINT_SPACE
     _OUT << "\"description\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DESCRIPTION_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_DESCRIPTION_SIZE;
           print_char(obj.description[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkLayerProperties(const VkLayerProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"layerName\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_EXTENSION_NAME_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_EXTENSION_NAME_SIZE;
           print_char(obj->layerName[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj->specVersion, "specVersion", 1);

     print_uint32_t(obj->implementationVersion, "implementationVersion", 1);

     PRINT_SPACE
     _OUT << "\"description\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DESCRIPTION_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_DESCRIPTION_SIZE;
           print_char(obj->description[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSubmitInfo(VkSubmitInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.waitSemaphoreCount, "waitSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pWaitSemaphores\":" << std::endl;
     PRINT_SPACE
     if (obj.pWaitSemaphores) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.waitSemaphoreCount; i++) {
           std:: stringstream tmp;
           tmp << "pWaitSemaphores" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.waitSemaphoreCount;
           print_VkSemaphore(obj.pWaitSemaphores[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pWaitDstStageMask\":" << std::endl;
     PRINT_SPACE
     if (obj.pWaitDstStageMask) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.waitSemaphoreCount; i++) {
           bool isCommaNeeded = (i+1) != obj.waitSemaphoreCount;
           print_VkPipelineStageFlags(obj.pWaitDstStageMask[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj.commandBufferCount, "commandBufferCount", 1);

     PRINT_SPACE
     _OUT << "\"pCommandBuffers\":" << std::endl;
     PRINT_SPACE
     if (obj.pCommandBuffers) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.commandBufferCount; i++) {
           std:: stringstream tmp;
           tmp << "pCommandBuffers" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.commandBufferCount;
           print_VkCommandBuffer(obj.pCommandBuffers[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj.signalSemaphoreCount, "signalSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pSignalSemaphores\":" << std::endl;
     PRINT_SPACE
     if (obj.pSignalSemaphores) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.signalSemaphoreCount; i++) {
           std:: stringstream tmp;
           tmp << "pSignalSemaphores" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.signalSemaphoreCount;
           print_VkSemaphore(obj.pSignalSemaphores[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSubmitInfo(const VkSubmitInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->waitSemaphoreCount, "waitSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pWaitSemaphores\":" << std::endl;
     PRINT_SPACE
     if (obj->pWaitSemaphores) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->waitSemaphoreCount; i++) {
           std:: stringstream tmp;
           tmp << "pWaitSemaphores" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->waitSemaphoreCount;
           print_VkSemaphore(obj->pWaitSemaphores[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pWaitDstStageMask\":" << std::endl;
     PRINT_SPACE
     if (obj->pWaitDstStageMask) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->waitSemaphoreCount; i++) {
           bool isCommaNeeded = (i+1) != obj->waitSemaphoreCount;
           print_VkPipelineStageFlags(obj->pWaitDstStageMask[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj->commandBufferCount, "commandBufferCount", 1);

     PRINT_SPACE
     _OUT << "\"pCommandBuffers\":" << std::endl;
     PRINT_SPACE
     if (obj->pCommandBuffers) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->commandBufferCount; i++) {
           std:: stringstream tmp;
           tmp << "pCommandBuffers" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->commandBufferCount;
           print_VkCommandBuffer(obj->pCommandBuffers[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj->signalSemaphoreCount, "signalSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pSignalSemaphores\":" << std::endl;
     PRINT_SPACE
     if (obj->pSignalSemaphores) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->signalSemaphoreCount; i++) {
           std:: stringstream tmp;
           tmp << "pSignalSemaphores" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->signalSemaphoreCount;
           print_VkSemaphore(obj->pSignalSemaphores[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkMappedMemoryRange(VkMappedMemoryRange obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj.offset, "offset", 1);

     print_VkDeviceSize(obj.size, "size", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkMappedMemoryRange(const VkMappedMemoryRange * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj->offset, "offset", 1);

     print_VkDeviceSize(obj->size, "size", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkMemoryAllocateInfo(VkMemoryAllocateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDeviceSize(obj.allocationSize, "allocationSize", 1);

     print_uint32_t(obj.memoryTypeIndex, "memoryTypeIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkMemoryAllocateInfo(const VkMemoryAllocateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDeviceSize(obj->allocationSize, "allocationSize", 1);

     print_uint32_t(obj->memoryTypeIndex, "memoryTypeIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkMemoryRequirements(VkMemoryRequirements obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj.size, "size", 1);

     print_VkDeviceSize(obj.alignment, "alignment", 1);

     print_uint32_t(obj.memoryTypeBits, "memoryTypeBits", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkMemoryRequirements(const VkMemoryRequirements * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj->size, "size", 1);

     print_VkDeviceSize(obj->alignment, "alignment", 1);

     print_uint32_t(obj->memoryTypeBits, "memoryTypeBits", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSparseMemoryBind(VkSparseMemoryBind obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj.resourceOffset, "resourceOffset", 1);

     print_VkDeviceSize(obj.size, "size", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj.memoryOffset, "memoryOffset", 1);

     print_VkSparseMemoryBindFlags(obj.flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSparseMemoryBind(const VkSparseMemoryBind * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj->resourceOffset, "resourceOffset", 1);

     print_VkDeviceSize(obj->size, "size", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj->memoryOffset, "memoryOffset", 1);

     print_VkSparseMemoryBindFlags(obj->flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSparseBufferMemoryBindInfo(VkSparseBufferMemoryBindInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj.bindCount, "bindCount", 1);

     PRINT_SPACE
     _OUT << "\"pBinds\": " << std::endl;
     if (obj.pBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.bindCount; i++) {
           if (i+1 == obj.bindCount)
               print_VkSparseMemoryBind(obj.pBinds[i], "pBinds", 0);
           else
               print_VkSparseMemoryBind(obj.pBinds[i], "pBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSparseBufferMemoryBindInfo(const VkSparseBufferMemoryBindInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj->bindCount, "bindCount", 1);

     PRINT_SPACE
     _OUT << "\"pBinds\": " << std::endl;
     if (obj->pBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->bindCount; i++) {
           if (i+1 == obj->bindCount)
               print_VkSparseMemoryBind(obj->pBinds[i], "pBinds", 0);
           else
               print_VkSparseMemoryBind(obj->pBinds[i], "pBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSparseImageOpaqueMemoryBindInfo(VkSparseImageOpaqueMemoryBindInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj.bindCount, "bindCount", 1);

     PRINT_SPACE
     _OUT << "\"pBinds\": " << std::endl;
     if (obj.pBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.bindCount; i++) {
           if (i+1 == obj.bindCount)
               print_VkSparseMemoryBind(obj.pBinds[i], "pBinds", 0);
           else
               print_VkSparseMemoryBind(obj.pBinds[i], "pBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSparseImageOpaqueMemoryBindInfo(const VkSparseImageOpaqueMemoryBindInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj->bindCount, "bindCount", 1);

     PRINT_SPACE
     _OUT << "\"pBinds\": " << std::endl;
     if (obj->pBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->bindCount; i++) {
           if (i+1 == obj->bindCount)
               print_VkSparseMemoryBind(obj->pBinds[i], "pBinds", 0);
           else
               print_VkSparseMemoryBind(obj->pBinds[i], "pBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageSubresource(VkImageSubresource obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkImageAspectFlags(obj.aspectMask, "aspectMask", 1);

     print_uint32_t(obj.mipLevel, "mipLevel", 1);

     print_uint32_t(obj.arrayLayer, "arrayLayer", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageSubresource(const VkImageSubresource * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkImageAspectFlags(obj->aspectMask, "aspectMask", 1);

     print_uint32_t(obj->mipLevel, "mipLevel", 1);

     print_uint32_t(obj->arrayLayer, "arrayLayer", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSparseImageMemoryBind(VkSparseImageMemoryBind obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"subresource\": " << std::endl;
     {
           print_VkImageSubresource(obj.subresource, "subresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"offset\": " << std::endl;
     {
           print_VkOffset3D(obj.offset, "offset", 1);
     }

     PRINT_SPACE
     _OUT << "\"extent\": " << std::endl;
     {
           print_VkExtent3D(obj.extent, "extent", 1);
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj.memoryOffset, "memoryOffset", 1);

     print_VkSparseMemoryBindFlags(obj.flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSparseImageMemoryBind(const VkSparseImageMemoryBind * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"subresource\": " << std::endl;
     {
           print_VkImageSubresource(obj->subresource, "subresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"offset\": " << std::endl;
     {
           print_VkOffset3D(obj->offset, "offset", 1);
     }

     PRINT_SPACE
     _OUT << "\"extent\": " << std::endl;
     {
           print_VkExtent3D(obj->extent, "extent", 1);
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj->memoryOffset, "memoryOffset", 1);

     print_VkSparseMemoryBindFlags(obj->flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSparseImageMemoryBindInfo(VkSparseImageMemoryBindInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj.bindCount, "bindCount", 1);

     PRINT_SPACE
     _OUT << "\"pBinds\": " << std::endl;
     if (obj.pBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.bindCount; i++) {
           if (i+1 == obj.bindCount)
               print_VkSparseImageMemoryBind(obj.pBinds[i], "pBinds", 0);
           else
               print_VkSparseImageMemoryBind(obj.pBinds[i], "pBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSparseImageMemoryBindInfo(const VkSparseImageMemoryBindInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj->bindCount, "bindCount", 1);

     PRINT_SPACE
     _OUT << "\"pBinds\": " << std::endl;
     if (obj->pBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->bindCount; i++) {
           if (i+1 == obj->bindCount)
               print_VkSparseImageMemoryBind(obj->pBinds[i], "pBinds", 0);
           else
               print_VkSparseImageMemoryBind(obj->pBinds[i], "pBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBindSparseInfo(VkBindSparseInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.waitSemaphoreCount, "waitSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pWaitSemaphores\":" << std::endl;
     PRINT_SPACE
     if (obj.pWaitSemaphores) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.waitSemaphoreCount; i++) {
           std:: stringstream tmp;
           tmp << "pWaitSemaphores" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.waitSemaphoreCount;
           print_VkSemaphore(obj.pWaitSemaphores[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj.bufferBindCount, "bufferBindCount", 1);

     PRINT_SPACE
     _OUT << "\"pBufferBinds\": " << std::endl;
     if (obj.pBufferBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.bufferBindCount; i++) {
           if (i+1 == obj.bufferBindCount)
               print_VkSparseBufferMemoryBindInfo(obj.pBufferBinds[i], "pBufferBinds", 0);
           else
               print_VkSparseBufferMemoryBindInfo(obj.pBufferBinds[i], "pBufferBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.imageOpaqueBindCount, "imageOpaqueBindCount", 1);

     PRINT_SPACE
     _OUT << "\"pImageOpaqueBinds\": " << std::endl;
     if (obj.pImageOpaqueBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.imageOpaqueBindCount; i++) {
           if (i+1 == obj.imageOpaqueBindCount)
               print_VkSparseImageOpaqueMemoryBindInfo(obj.pImageOpaqueBinds[i], "pImageOpaqueBinds", 0);
           else
               print_VkSparseImageOpaqueMemoryBindInfo(obj.pImageOpaqueBinds[i], "pImageOpaqueBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.imageBindCount, "imageBindCount", 1);

     PRINT_SPACE
     _OUT << "\"pImageBinds\": " << std::endl;
     if (obj.pImageBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.imageBindCount; i++) {
           if (i+1 == obj.imageBindCount)
               print_VkSparseImageMemoryBindInfo(obj.pImageBinds[i], "pImageBinds", 0);
           else
               print_VkSparseImageMemoryBindInfo(obj.pImageBinds[i], "pImageBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.signalSemaphoreCount, "signalSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pSignalSemaphores\":" << std::endl;
     PRINT_SPACE
     if (obj.pSignalSemaphores) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.signalSemaphoreCount; i++) {
           std:: stringstream tmp;
           tmp << "pSignalSemaphores" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.signalSemaphoreCount;
           print_VkSemaphore(obj.pSignalSemaphores[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBindSparseInfo(const VkBindSparseInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->waitSemaphoreCount, "waitSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pWaitSemaphores\":" << std::endl;
     PRINT_SPACE
     if (obj->pWaitSemaphores) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->waitSemaphoreCount; i++) {
           std:: stringstream tmp;
           tmp << "pWaitSemaphores" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->waitSemaphoreCount;
           print_VkSemaphore(obj->pWaitSemaphores[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj->bufferBindCount, "bufferBindCount", 1);

     PRINT_SPACE
     _OUT << "\"pBufferBinds\": " << std::endl;
     if (obj->pBufferBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->bufferBindCount; i++) {
           if (i+1 == obj->bufferBindCount)
               print_VkSparseBufferMemoryBindInfo(obj->pBufferBinds[i], "pBufferBinds", 0);
           else
               print_VkSparseBufferMemoryBindInfo(obj->pBufferBinds[i], "pBufferBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->imageOpaqueBindCount, "imageOpaqueBindCount", 1);

     PRINT_SPACE
     _OUT << "\"pImageOpaqueBinds\": " << std::endl;
     if (obj->pImageOpaqueBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->imageOpaqueBindCount; i++) {
           if (i+1 == obj->imageOpaqueBindCount)
               print_VkSparseImageOpaqueMemoryBindInfo(obj->pImageOpaqueBinds[i], "pImageOpaqueBinds", 0);
           else
               print_VkSparseImageOpaqueMemoryBindInfo(obj->pImageOpaqueBinds[i], "pImageOpaqueBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->imageBindCount, "imageBindCount", 1);

     PRINT_SPACE
     _OUT << "\"pImageBinds\": " << std::endl;
     if (obj->pImageBinds) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->imageBindCount; i++) {
           if (i+1 == obj->imageBindCount)
               print_VkSparseImageMemoryBindInfo(obj->pImageBinds[i], "pImageBinds", 0);
           else
               print_VkSparseImageMemoryBindInfo(obj->pImageBinds[i], "pImageBinds", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->signalSemaphoreCount, "signalSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pSignalSemaphores\":" << std::endl;
     PRINT_SPACE
     if (obj->pSignalSemaphores) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->signalSemaphoreCount; i++) {
           std:: stringstream tmp;
           tmp << "pSignalSemaphores" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->signalSemaphoreCount;
           print_VkSemaphore(obj->pSignalSemaphores[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSparseImageFormatProperties(VkSparseImageFormatProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkImageAspectFlags(obj.aspectMask, "aspectMask", 1);

     PRINT_SPACE
     _OUT << "\"imageGranularity\": " << std::endl;
     {
           print_VkExtent3D(obj.imageGranularity, "imageGranularity", 1);
     }

     print_VkSparseImageFormatFlags(obj.flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSparseImageFormatProperties(const VkSparseImageFormatProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkImageAspectFlags(obj->aspectMask, "aspectMask", 1);

     PRINT_SPACE
     _OUT << "\"imageGranularity\": " << std::endl;
     {
           print_VkExtent3D(obj->imageGranularity, "imageGranularity", 1);
     }

     print_VkSparseImageFormatFlags(obj->flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSparseImageMemoryRequirements(VkSparseImageMemoryRequirements obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"formatProperties\": " << std::endl;
     {
           print_VkSparseImageFormatProperties(obj.formatProperties, "formatProperties", 1);
     }

     print_uint32_t(obj.imageMipTailFirstLod, "imageMipTailFirstLod", 1);

     print_VkDeviceSize(obj.imageMipTailSize, "imageMipTailSize", 1);

     print_VkDeviceSize(obj.imageMipTailOffset, "imageMipTailOffset", 1);

     print_VkDeviceSize(obj.imageMipTailStride, "imageMipTailStride", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSparseImageMemoryRequirements(const VkSparseImageMemoryRequirements * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"formatProperties\": " << std::endl;
     {
           print_VkSparseImageFormatProperties(obj->formatProperties, "formatProperties", 1);
     }

     print_uint32_t(obj->imageMipTailFirstLod, "imageMipTailFirstLod", 1);

     print_VkDeviceSize(obj->imageMipTailSize, "imageMipTailSize", 1);

     print_VkDeviceSize(obj->imageMipTailOffset, "imageMipTailOffset", 1);

     print_VkDeviceSize(obj->imageMipTailStride, "imageMipTailStride", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkFenceCreateInfo(VkFenceCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkFenceCreateFlags(obj.flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkFenceCreateInfo(const VkFenceCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkFenceCreateFlags(obj->flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSemaphoreCreateInfo(VkSemaphoreCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSemaphoreCreateFlags(obj.flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSemaphoreCreateInfo(const VkSemaphoreCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSemaphoreCreateFlags(obj->flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkEventCreateInfo(VkEventCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkEventCreateFlags(obj.flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkEventCreateInfo(const VkEventCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkEventCreateFlags(obj->flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkQueryPoolCreateInfo(VkQueryPoolCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkQueryPoolCreateFlags(obj.flags, "flags", 1);

     print_VkQueryType(obj.queryType, "queryType", 1);

     print_uint32_t(obj.queryCount, "queryCount", 1);

     print_VkQueryPipelineStatisticFlags(obj.pipelineStatistics, "pipelineStatistics", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkQueryPoolCreateInfo(const VkQueryPoolCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkQueryPoolCreateFlags(obj->flags, "flags", 1);

     print_VkQueryType(obj->queryType, "queryType", 1);

     print_uint32_t(obj->queryCount, "queryCount", 1);

     print_VkQueryPipelineStatisticFlags(obj->pipelineStatistics, "pipelineStatistics", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBufferCreateInfo(VkBufferCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBufferCreateFlags(obj.flags, "flags", 1);

     print_VkDeviceSize(obj.size, "size", 1);

     print_VkBufferUsageFlags(obj.usage, "usage", 1);

     print_VkSharingMode(obj.sharingMode, "sharingMode", 1);

     print_uint32_t(obj.queueFamilyIndexCount, "queueFamilyIndexCount", 1);

     PRINT_SPACE
     _OUT << "\"pQueueFamilyIndices\":" << std::endl;
     PRINT_SPACE
     if (obj.pQueueFamilyIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.queueFamilyIndexCount; i++) {
           bool isCommaNeeded = (i+1) != obj.queueFamilyIndexCount;
           print_uint32_t(obj.pQueueFamilyIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBufferCreateInfo(const VkBufferCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBufferCreateFlags(obj->flags, "flags", 1);

     print_VkDeviceSize(obj->size, "size", 1);

     print_VkBufferUsageFlags(obj->usage, "usage", 1);

     print_VkSharingMode(obj->sharingMode, "sharingMode", 1);

     print_uint32_t(obj->queueFamilyIndexCount, "queueFamilyIndexCount", 1);

     PRINT_SPACE
     _OUT << "\"pQueueFamilyIndices\":" << std::endl;
     PRINT_SPACE
     if (obj->pQueueFamilyIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->queueFamilyIndexCount; i++) {
           bool isCommaNeeded = (i+1) != obj->queueFamilyIndexCount;
           print_uint32_t(obj->pQueueFamilyIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBufferViewCreateInfo(VkBufferViewCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBufferViewCreateFlags(obj.flags, "flags", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkFormat(obj.format, "format", 1);

     print_VkDeviceSize(obj.offset, "offset", 1);

     print_VkDeviceSize(obj.range, "range", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBufferViewCreateInfo(const VkBufferViewCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBufferViewCreateFlags(obj->flags, "flags", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkFormat(obj->format, "format", 1);

     print_VkDeviceSize(obj->offset, "offset", 1);

     print_VkDeviceSize(obj->range, "range", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageCreateInfo(VkImageCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageCreateFlags(obj.flags, "flags", 1);

     print_VkImageType(obj.imageType, "imageType", 1);

     print_VkFormat(obj.format, "format", 1);

     PRINT_SPACE
     _OUT << "\"extent\": " << std::endl;
     {
           print_VkExtent3D(obj.extent, "extent", 1);
     }

     print_uint32_t(obj.mipLevels, "mipLevels", 1);

     print_uint32_t(obj.arrayLayers, "arrayLayers", 1);

     print_VkSampleCountFlagBits(obj.samples, "samples", 1);

     print_VkImageTiling(obj.tiling, "tiling", 1);

     print_VkImageUsageFlags(obj.usage, "usage", 1);

     print_VkSharingMode(obj.sharingMode, "sharingMode", 1);

     print_uint32_t(obj.queueFamilyIndexCount, "queueFamilyIndexCount", 1);

     PRINT_SPACE
     _OUT << "\"pQueueFamilyIndices\":" << std::endl;
     PRINT_SPACE
     if (obj.pQueueFamilyIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.queueFamilyIndexCount; i++) {
           bool isCommaNeeded = (i+1) != obj.queueFamilyIndexCount;
           print_uint32_t(obj.pQueueFamilyIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_VkImageLayout(obj.initialLayout, "initialLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageCreateInfo(const VkImageCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageCreateFlags(obj->flags, "flags", 1);

     print_VkImageType(obj->imageType, "imageType", 1);

     print_VkFormat(obj->format, "format", 1);

     PRINT_SPACE
     _OUT << "\"extent\": " << std::endl;
     {
           print_VkExtent3D(obj->extent, "extent", 1);
     }

     print_uint32_t(obj->mipLevels, "mipLevels", 1);

     print_uint32_t(obj->arrayLayers, "arrayLayers", 1);

     print_VkSampleCountFlagBits(obj->samples, "samples", 1);

     print_VkImageTiling(obj->tiling, "tiling", 1);

     print_VkImageUsageFlags(obj->usage, "usage", 1);

     print_VkSharingMode(obj->sharingMode, "sharingMode", 1);

     print_uint32_t(obj->queueFamilyIndexCount, "queueFamilyIndexCount", 1);

     PRINT_SPACE
     _OUT << "\"pQueueFamilyIndices\":" << std::endl;
     PRINT_SPACE
     if (obj->pQueueFamilyIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->queueFamilyIndexCount; i++) {
           bool isCommaNeeded = (i+1) != obj->queueFamilyIndexCount;
           print_uint32_t(obj->pQueueFamilyIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_VkImageLayout(obj->initialLayout, "initialLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSubresourceLayout(VkSubresourceLayout obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj.offset, "offset", 1);

     print_VkDeviceSize(obj.size, "size", 1);

     print_VkDeviceSize(obj.rowPitch, "rowPitch", 1);

     print_VkDeviceSize(obj.arrayPitch, "arrayPitch", 1);

     print_VkDeviceSize(obj.depthPitch, "depthPitch", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSubresourceLayout(const VkSubresourceLayout * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj->offset, "offset", 1);

     print_VkDeviceSize(obj->size, "size", 1);

     print_VkDeviceSize(obj->rowPitch, "rowPitch", 1);

     print_VkDeviceSize(obj->arrayPitch, "arrayPitch", 1);

     print_VkDeviceSize(obj->depthPitch, "depthPitch", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkComponentMapping(VkComponentMapping obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkComponentSwizzle(obj.r, "r", 1);

     print_VkComponentSwizzle(obj.g, "g", 1);

     print_VkComponentSwizzle(obj.b, "b", 1);

     print_VkComponentSwizzle(obj.a, "a", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkComponentMapping(const VkComponentMapping * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkComponentSwizzle(obj->r, "r", 1);

     print_VkComponentSwizzle(obj->g, "g", 1);

     print_VkComponentSwizzle(obj->b, "b", 1);

     print_VkComponentSwizzle(obj->a, "a", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageViewCreateInfo(VkImageViewCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageViewCreateFlags(obj.flags, "flags", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkImageViewType(obj.viewType, "viewType", 1);

     print_VkFormat(obj.format, "format", 1);

     PRINT_SPACE
     _OUT << "\"components\": " << std::endl;
     {
           print_VkComponentMapping(obj.components, "components", 1);
     }

     PRINT_SPACE
     _OUT << "\"subresourceRange\": " << std::endl;
     {
           print_VkImageSubresourceRange(obj.subresourceRange, "subresourceRange", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageViewCreateInfo(const VkImageViewCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageViewCreateFlags(obj->flags, "flags", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkImageViewType(obj->viewType, "viewType", 1);

     print_VkFormat(obj->format, "format", 1);

     PRINT_SPACE
     _OUT << "\"components\": " << std::endl;
     {
           print_VkComponentMapping(obj->components, "components", 1);
     }

     PRINT_SPACE
     _OUT << "\"subresourceRange\": " << std::endl;
     {
           print_VkImageSubresourceRange(obj->subresourceRange, "subresourceRange", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkShaderModuleCreateInfo(VkShaderModuleCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkShaderModuleCreateFlags(obj.flags, "flags", 1);

     print_size_t(obj.codeSize, "codeSize", 1);

     print_uint32_t(obj.pCode, "pCode", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkShaderModuleCreateInfo(const VkShaderModuleCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkShaderModuleCreateFlags(obj->flags, "flags", 1);

     print_size_t(obj->codeSize, "codeSize", 1);

     print_uint32_t(obj->pCode, "pCode", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineCacheCreateInfo(VkPipelineCacheCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineCacheCreateFlags(obj.flags, "flags", 1);

     print_size_t(obj.initialDataSize, "initialDataSize", 1);

     /** Note: Ignoring void* data. **/

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineCacheCreateInfo(const VkPipelineCacheCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineCacheCreateFlags(obj->flags, "flags", 1);

     print_size_t(obj->initialDataSize, "initialDataSize", 1);

     /** Note: Ignoring void* data. **/

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSpecializationMapEntry(VkSpecializationMapEntry obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.constantID, "constantID", 1);

     print_uint32_t(obj.offset, "offset", 1);

     print_size_t(obj.size, "size", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSpecializationMapEntry(const VkSpecializationMapEntry * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->constantID, "constantID", 1);

     print_uint32_t(obj->offset, "offset", 1);

     print_size_t(obj->size, "size", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSpecializationInfo(VkSpecializationInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.mapEntryCount, "mapEntryCount", 1);

     PRINT_SPACE
     _OUT << "\"pMapEntries\": " << std::endl;
     if (obj.pMapEntries) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.mapEntryCount; i++) {
           if (i+1 == obj.mapEntryCount)
               print_VkSpecializationMapEntry(obj.pMapEntries[i], "pMapEntries", 0);
           else
               print_VkSpecializationMapEntry(obj.pMapEntries[i], "pMapEntries", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_size_t(obj.dataSize, "dataSize", 1);

     print_void_data(obj.pData, int(obj.dataSize), "pData", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSpecializationInfo(const VkSpecializationInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->mapEntryCount, "mapEntryCount", 1);

     PRINT_SPACE
     _OUT << "\"pMapEntries\": " << std::endl;
     if (obj->pMapEntries) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->mapEntryCount; i++) {
           if (i+1 == obj->mapEntryCount)
               print_VkSpecializationMapEntry(obj->pMapEntries[i], "pMapEntries", 0);
           else
               print_VkSpecializationMapEntry(obj->pMapEntries[i], "pMapEntries", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_size_t(obj->dataSize, "dataSize", 1);

     print_void_data(obj->pData, int(obj->dataSize), "pData", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineShaderStageCreateInfo(VkPipelineShaderStageCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineShaderStageCreateFlags(obj.flags, "flags", 1);

     print_VkShaderStageFlagBits(obj.stage, "stage", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "module" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_char(obj.pName, "pName", 1);

     PRINT_SPACE
     _OUT << "\"pSpecializationInfo\": " << std::endl;
     if (obj.pSpecializationInfo) {
           print_VkSpecializationInfo(obj.pSpecializationInfo, "pSpecializationInfo", 0);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineShaderStageCreateInfo(const VkPipelineShaderStageCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineShaderStageCreateFlags(obj->flags, "flags", 1);

     print_VkShaderStageFlagBits(obj->stage, "stage", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "module" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_char(obj->pName, "pName", 1);

     PRINT_SPACE
     _OUT << "\"pSpecializationInfo\": " << std::endl;
     if (obj->pSpecializationInfo) {
           print_VkSpecializationInfo(obj->pSpecializationInfo, "pSpecializationInfo", 0);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkComputePipelineCreateInfo(VkComputePipelineCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineCreateFlags(obj.flags, "flags", 1);

     PRINT_SPACE
     _OUT << "\"stage\": " << std::endl;
     {
           print_VkPipelineShaderStageCreateInfo(obj.stage, "stage", 1);
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "layout" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "basePipelineHandle" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_int32_t(obj.basePipelineIndex, "basePipelineIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkComputePipelineCreateInfo(const VkComputePipelineCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineCreateFlags(obj->flags, "flags", 1);

     PRINT_SPACE
     _OUT << "\"stage\": " << std::endl;
     {
           print_VkPipelineShaderStageCreateInfo(obj->stage, "stage", 1);
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "layout" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "basePipelineHandle" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_int32_t(obj->basePipelineIndex, "basePipelineIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkVertexInputBindingDescription(VkVertexInputBindingDescription obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.binding, "binding", 1);

     print_uint32_t(obj.stride, "stride", 1);

     print_VkVertexInputRate(obj.inputRate, "inputRate", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkVertexInputBindingDescription(const VkVertexInputBindingDescription * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->binding, "binding", 1);

     print_uint32_t(obj->stride, "stride", 1);

     print_VkVertexInputRate(obj->inputRate, "inputRate", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkVertexInputAttributeDescription(VkVertexInputAttributeDescription obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.location, "location", 1);

     print_uint32_t(obj.binding, "binding", 1);

     print_VkFormat(obj.format, "format", 1);

     print_uint32_t(obj.offset, "offset", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkVertexInputAttributeDescription(const VkVertexInputAttributeDescription * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->location, "location", 1);

     print_uint32_t(obj->binding, "binding", 1);

     print_VkFormat(obj->format, "format", 1);

     print_uint32_t(obj->offset, "offset", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineVertexInputStateCreateInfo(VkPipelineVertexInputStateCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineVertexInputStateCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.vertexBindingDescriptionCount, "vertexBindingDescriptionCount", 1);

     PRINT_SPACE
     _OUT << "\"pVertexBindingDescriptions\": " << std::endl;
     if (obj.pVertexBindingDescriptions) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.vertexBindingDescriptionCount; i++) {
           if (i+1 == obj.vertexBindingDescriptionCount)
               print_VkVertexInputBindingDescription(obj.pVertexBindingDescriptions[i], "pVertexBindingDescriptions", 0);
           else
               print_VkVertexInputBindingDescription(obj.pVertexBindingDescriptions[i], "pVertexBindingDescriptions", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.vertexAttributeDescriptionCount, "vertexAttributeDescriptionCount", 1);

     PRINT_SPACE
     _OUT << "\"pVertexAttributeDescriptions\": " << std::endl;
     if (obj.pVertexAttributeDescriptions) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.vertexAttributeDescriptionCount; i++) {
           if (i+1 == obj.vertexAttributeDescriptionCount)
               print_VkVertexInputAttributeDescription(obj.pVertexAttributeDescriptions[i], "pVertexAttributeDescriptions", 0);
           else
               print_VkVertexInputAttributeDescription(obj.pVertexAttributeDescriptions[i], "pVertexAttributeDescriptions", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineVertexInputStateCreateInfo(const VkPipelineVertexInputStateCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineVertexInputStateCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->vertexBindingDescriptionCount, "vertexBindingDescriptionCount", 1);

     PRINT_SPACE
     _OUT << "\"pVertexBindingDescriptions\": " << std::endl;
     if (obj->pVertexBindingDescriptions) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->vertexBindingDescriptionCount; i++) {
           if (i+1 == obj->vertexBindingDescriptionCount)
               print_VkVertexInputBindingDescription(obj->pVertexBindingDescriptions[i], "pVertexBindingDescriptions", 0);
           else
               print_VkVertexInputBindingDescription(obj->pVertexBindingDescriptions[i], "pVertexBindingDescriptions", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->vertexAttributeDescriptionCount, "vertexAttributeDescriptionCount", 1);

     PRINT_SPACE
     _OUT << "\"pVertexAttributeDescriptions\": " << std::endl;
     if (obj->pVertexAttributeDescriptions) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->vertexAttributeDescriptionCount; i++) {
           if (i+1 == obj->vertexAttributeDescriptionCount)
               print_VkVertexInputAttributeDescription(obj->pVertexAttributeDescriptions[i], "pVertexAttributeDescriptions", 0);
           else
               print_VkVertexInputAttributeDescription(obj->pVertexAttributeDescriptions[i], "pVertexAttributeDescriptions", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineInputAssemblyStateCreateInfo(VkPipelineInputAssemblyStateCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineInputAssemblyStateCreateFlags(obj.flags, "flags", 1);

     print_VkPrimitiveTopology(obj.topology, "topology", 1);

     print_VkBool32(obj.primitiveRestartEnable, "primitiveRestartEnable", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineInputAssemblyStateCreateInfo(const VkPipelineInputAssemblyStateCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineInputAssemblyStateCreateFlags(obj->flags, "flags", 1);

     print_VkPrimitiveTopology(obj->topology, "topology", 1);

     print_VkBool32(obj->primitiveRestartEnable, "primitiveRestartEnable", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineTessellationStateCreateInfo(VkPipelineTessellationStateCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineTessellationStateCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.patchControlPoints, "patchControlPoints", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineTessellationStateCreateInfo(const VkPipelineTessellationStateCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineTessellationStateCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->patchControlPoints, "patchControlPoints", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkViewport(VkViewport obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_float(obj.x, "x", 1);

     print_float(obj.y, "y", 1);

     print_float(obj.width, "width", 1);

     print_float(obj.height, "height", 1);

     print_float(obj.minDepth, "minDepth", 1);

     print_float(obj.maxDepth, "maxDepth", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkViewport(const VkViewport * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_float(obj->x, "x", 1);

     print_float(obj->y, "y", 1);

     print_float(obj->width, "width", 1);

     print_float(obj->height, "height", 1);

     print_float(obj->minDepth, "minDepth", 1);

     print_float(obj->maxDepth, "maxDepth", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineViewportStateCreateInfo(VkPipelineViewportStateCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineViewportStateCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.viewportCount, "viewportCount", 1);

     PRINT_SPACE
     _OUT << "\"pViewports\": " << std::endl;
     if (obj.pViewports) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.viewportCount; i++) {
           if (i+1 == obj.viewportCount)
               print_VkViewport(obj.pViewports[i], "pViewports", 0);
           else
               print_VkViewport(obj.pViewports[i], "pViewports", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.scissorCount, "scissorCount", 1);

     PRINT_SPACE
     _OUT << "\"pScissors\": " << std::endl;
     if (obj.pScissors) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.scissorCount; i++) {
           if (i+1 == obj.scissorCount)
               print_VkRect2D(obj.pScissors[i], "pScissors", 0);
           else
               print_VkRect2D(obj.pScissors[i], "pScissors", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineViewportStateCreateInfo(const VkPipelineViewportStateCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineViewportStateCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->viewportCount, "viewportCount", 1);

     PRINT_SPACE
     _OUT << "\"pViewports\": " << std::endl;
     if (obj->pViewports) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->viewportCount; i++) {
           if (i+1 == obj->viewportCount)
               print_VkViewport(obj->pViewports[i], "pViewports", 0);
           else
               print_VkViewport(obj->pViewports[i], "pViewports", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->scissorCount, "scissorCount", 1);

     PRINT_SPACE
     _OUT << "\"pScissors\": " << std::endl;
     if (obj->pScissors) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->scissorCount; i++) {
           if (i+1 == obj->scissorCount)
               print_VkRect2D(obj->pScissors[i], "pScissors", 0);
           else
               print_VkRect2D(obj->pScissors[i], "pScissors", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineRasterizationStateCreateInfo(VkPipelineRasterizationStateCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineRasterizationStateCreateFlags(obj.flags, "flags", 1);

     print_VkBool32(obj.depthClampEnable, "depthClampEnable", 1);

     print_VkBool32(obj.rasterizerDiscardEnable, "rasterizerDiscardEnable", 1);

     print_VkPolygonMode(obj.polygonMode, "polygonMode", 1);

     print_VkCullModeFlags(obj.cullMode, "cullMode", 1);

     print_VkFrontFace(obj.frontFace, "frontFace", 1);

     print_VkBool32(obj.depthBiasEnable, "depthBiasEnable", 1);

     print_float(obj.depthBiasConstantFactor, "depthBiasConstantFactor", 1);

     print_float(obj.depthBiasClamp, "depthBiasClamp", 1);

     print_float(obj.depthBiasSlopeFactor, "depthBiasSlopeFactor", 1);

     print_float(obj.lineWidth, "lineWidth", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineRasterizationStateCreateInfo(const VkPipelineRasterizationStateCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineRasterizationStateCreateFlags(obj->flags, "flags", 1);

     print_VkBool32(obj->depthClampEnable, "depthClampEnable", 1);

     print_VkBool32(obj->rasterizerDiscardEnable, "rasterizerDiscardEnable", 1);

     print_VkPolygonMode(obj->polygonMode, "polygonMode", 1);

     print_VkCullModeFlags(obj->cullMode, "cullMode", 1);

     print_VkFrontFace(obj->frontFace, "frontFace", 1);

     print_VkBool32(obj->depthBiasEnable, "depthBiasEnable", 1);

     print_float(obj->depthBiasConstantFactor, "depthBiasConstantFactor", 1);

     print_float(obj->depthBiasClamp, "depthBiasClamp", 1);

     print_float(obj->depthBiasSlopeFactor, "depthBiasSlopeFactor", 1);

     print_float(obj->lineWidth, "lineWidth", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineMultisampleStateCreateInfo(VkPipelineMultisampleStateCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineMultisampleStateCreateFlags(obj.flags, "flags", 1);

     print_VkSampleCountFlagBits(obj.rasterizationSamples, "rasterizationSamples", 1);

     print_VkBool32(obj.sampleShadingEnable, "sampleShadingEnable", 1);

     print_float(obj.minSampleShading, "minSampleShading", 1);

     uint32_t sampleMaskSize = ((obj.rasterizationSamples + 31) / 32);
     PRINT_SPACE
     _OUT << "\"pSampleMask\":" << std::endl;
     PRINT_SPACE
     if (obj.pSampleMask) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < sampleMaskSize; i++) {
           bool isCommaNeeded = (i+1) != sampleMaskSize;
           print_uint32_t(obj.pSampleMask[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_VkBool32(obj.alphaToCoverageEnable, "alphaToCoverageEnable", 1);

     print_VkBool32(obj.alphaToOneEnable, "alphaToOneEnable", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineMultisampleStateCreateInfo(const VkPipelineMultisampleStateCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineMultisampleStateCreateFlags(obj->flags, "flags", 1);

     print_VkSampleCountFlagBits(obj->rasterizationSamples, "rasterizationSamples", 1);

     print_VkBool32(obj->sampleShadingEnable, "sampleShadingEnable", 1);

     print_float(obj->minSampleShading, "minSampleShading", 1);

     uint32_t sampleMaskSize = ((obj->rasterizationSamples + 31) / 32);
     PRINT_SPACE
     _OUT << "\"pSampleMask\":" << std::endl;
     PRINT_SPACE
     if (obj->pSampleMask) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < sampleMaskSize; i++) {
           bool isCommaNeeded = (i+1) != sampleMaskSize;
           print_uint32_t(obj->pSampleMask[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_VkBool32(obj->alphaToCoverageEnable, "alphaToCoverageEnable", 1);

     print_VkBool32(obj->alphaToOneEnable, "alphaToOneEnable", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkStencilOpState(VkStencilOpState obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStencilOp(obj.failOp, "failOp", 1);

     print_VkStencilOp(obj.passOp, "passOp", 1);

     print_VkStencilOp(obj.depthFailOp, "depthFailOp", 1);

     print_VkCompareOp(obj.compareOp, "compareOp", 1);

     print_uint32_t(obj.compareMask, "compareMask", 1);

     print_uint32_t(obj.writeMask, "writeMask", 1);

     print_uint32_t(obj.reference, "reference", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkStencilOpState(const VkStencilOpState * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStencilOp(obj->failOp, "failOp", 1);

     print_VkStencilOp(obj->passOp, "passOp", 1);

     print_VkStencilOp(obj->depthFailOp, "depthFailOp", 1);

     print_VkCompareOp(obj->compareOp, "compareOp", 1);

     print_uint32_t(obj->compareMask, "compareMask", 1);

     print_uint32_t(obj->writeMask, "writeMask", 1);

     print_uint32_t(obj->reference, "reference", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineDepthStencilStateCreateInfo(VkPipelineDepthStencilStateCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineDepthStencilStateCreateFlags(obj.flags, "flags", 1);

     print_VkBool32(obj.depthTestEnable, "depthTestEnable", 1);

     print_VkBool32(obj.depthWriteEnable, "depthWriteEnable", 1);

     print_VkCompareOp(obj.depthCompareOp, "depthCompareOp", 1);

     print_VkBool32(obj.depthBoundsTestEnable, "depthBoundsTestEnable", 1);

     print_VkBool32(obj.stencilTestEnable, "stencilTestEnable", 1);

     PRINT_SPACE
     _OUT << "\"front\": " << std::endl;
     {
           print_VkStencilOpState(obj.front, "front", 1);
     }

     PRINT_SPACE
     _OUT << "\"back\": " << std::endl;
     {
           print_VkStencilOpState(obj.back, "back", 1);
     }

     print_float(obj.minDepthBounds, "minDepthBounds", 1);

     print_float(obj.maxDepthBounds, "maxDepthBounds", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineDepthStencilStateCreateInfo(const VkPipelineDepthStencilStateCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineDepthStencilStateCreateFlags(obj->flags, "flags", 1);

     print_VkBool32(obj->depthTestEnable, "depthTestEnable", 1);

     print_VkBool32(obj->depthWriteEnable, "depthWriteEnable", 1);

     print_VkCompareOp(obj->depthCompareOp, "depthCompareOp", 1);

     print_VkBool32(obj->depthBoundsTestEnable, "depthBoundsTestEnable", 1);

     print_VkBool32(obj->stencilTestEnable, "stencilTestEnable", 1);

     PRINT_SPACE
     _OUT << "\"front\": " << std::endl;
     {
           print_VkStencilOpState(obj->front, "front", 1);
     }

     PRINT_SPACE
     _OUT << "\"back\": " << std::endl;
     {
           print_VkStencilOpState(obj->back, "back", 1);
     }

     print_float(obj->minDepthBounds, "minDepthBounds", 1);

     print_float(obj->maxDepthBounds, "maxDepthBounds", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineColorBlendAttachmentState(VkPipelineColorBlendAttachmentState obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkBool32(obj.blendEnable, "blendEnable", 1);

     print_VkBlendFactor(obj.srcColorBlendFactor, "srcColorBlendFactor", 1);

     print_VkBlendFactor(obj.dstColorBlendFactor, "dstColorBlendFactor", 1);

     print_VkBlendOp(obj.colorBlendOp, "colorBlendOp", 1);

     print_VkBlendFactor(obj.srcAlphaBlendFactor, "srcAlphaBlendFactor", 1);

     print_VkBlendFactor(obj.dstAlphaBlendFactor, "dstAlphaBlendFactor", 1);

     print_VkBlendOp(obj.alphaBlendOp, "alphaBlendOp", 1);

     print_VkColorComponentFlags(obj.colorWriteMask, "colorWriteMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineColorBlendAttachmentState(const VkPipelineColorBlendAttachmentState * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkBool32(obj->blendEnable, "blendEnable", 1);

     print_VkBlendFactor(obj->srcColorBlendFactor, "srcColorBlendFactor", 1);

     print_VkBlendFactor(obj->dstColorBlendFactor, "dstColorBlendFactor", 1);

     print_VkBlendOp(obj->colorBlendOp, "colorBlendOp", 1);

     print_VkBlendFactor(obj->srcAlphaBlendFactor, "srcAlphaBlendFactor", 1);

     print_VkBlendFactor(obj->dstAlphaBlendFactor, "dstAlphaBlendFactor", 1);

     print_VkBlendOp(obj->alphaBlendOp, "alphaBlendOp", 1);

     print_VkColorComponentFlags(obj->colorWriteMask, "colorWriteMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineColorBlendStateCreateInfo(VkPipelineColorBlendStateCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineColorBlendStateCreateFlags(obj.flags, "flags", 1);

     print_VkBool32(obj.logicOpEnable, "logicOpEnable", 1);

     print_VkLogicOp(obj.logicOp, "logicOp", 1);

     print_uint32_t(obj.attachmentCount, "attachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachments\": " << std::endl;
     if (obj.pAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.attachmentCount; i++) {
           if (i+1 == obj.attachmentCount)
               print_VkPipelineColorBlendAttachmentState(obj.pAttachments[i], "pAttachments", 0);
           else
               print_VkPipelineColorBlendAttachmentState(obj.pAttachments[i], "pAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"blendConstants\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 4; i++) {
           bool isCommaNeeded = (i+1) != 4;
           print_float(obj.blendConstants[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendStateCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineColorBlendStateCreateFlags(obj->flags, "flags", 1);

     print_VkBool32(obj->logicOpEnable, "logicOpEnable", 1);

     print_VkLogicOp(obj->logicOp, "logicOp", 1);

     print_uint32_t(obj->attachmentCount, "attachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachments\": " << std::endl;
     if (obj->pAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->attachmentCount; i++) {
           if (i+1 == obj->attachmentCount)
               print_VkPipelineColorBlendAttachmentState(obj->pAttachments[i], "pAttachments", 0);
           else
               print_VkPipelineColorBlendAttachmentState(obj->pAttachments[i], "pAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"blendConstants\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 4; i++) {
           bool isCommaNeeded = (i+1) != 4;
           print_float(obj->blendConstants[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineDynamicStateCreateInfo(VkPipelineDynamicStateCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineDynamicStateCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.dynamicStateCount, "dynamicStateCount", 1);

     PRINT_SPACE
     _OUT << "\"pDynamicStates\":" << std::endl;
     PRINT_SPACE
     if (obj.pDynamicStates) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.dynamicStateCount; i++) {
           bool isCommaNeeded = (i+1) != obj.dynamicStateCount;
           print_VkDynamicState(obj.pDynamicStates[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineDynamicStateCreateInfo(const VkPipelineDynamicStateCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineDynamicStateCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->dynamicStateCount, "dynamicStateCount", 1);

     PRINT_SPACE
     _OUT << "\"pDynamicStates\":" << std::endl;
     PRINT_SPACE
     if (obj->pDynamicStates) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->dynamicStateCount; i++) {
           bool isCommaNeeded = (i+1) != obj->dynamicStateCount;
           print_VkDynamicState(obj->pDynamicStates[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkGraphicsPipelineCreateInfo(VkGraphicsPipelineCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.stageCount, "stageCount", 1);

     PRINT_SPACE
     _OUT << "\"pStages\": " << std::endl;
     if (obj.pStages) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.stageCount; i++) {
           if (i+1 == obj.stageCount)
               print_VkPipelineShaderStageCreateInfo(obj.pStages[i], "pStages", 0);
           else
               print_VkPipelineShaderStageCreateInfo(obj.pStages[i], "pStages", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pVertexInputState\": " << std::endl;
     if (obj.pVertexInputState) {
           print_VkPipelineVertexInputStateCreateInfo(obj.pVertexInputState, "pVertexInputState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pInputAssemblyState\": " << std::endl;
     if (obj.pInputAssemblyState) {
           print_VkPipelineInputAssemblyStateCreateInfo(obj.pInputAssemblyState, "pInputAssemblyState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pTessellationState\": " << std::endl;
     if (obj.pTessellationState) {
           print_VkPipelineTessellationStateCreateInfo(obj.pTessellationState, "pTessellationState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pViewportState\": " << std::endl;
     if (obj.pViewportState) {
           print_VkPipelineViewportStateCreateInfo(obj.pViewportState, "pViewportState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pRasterizationState\": " << std::endl;
     if (obj.pRasterizationState) {
           print_VkPipelineRasterizationStateCreateInfo(obj.pRasterizationState, "pRasterizationState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pMultisampleState\": " << std::endl;
     if (obj.pMultisampleState) {
           print_VkPipelineMultisampleStateCreateInfo(obj.pMultisampleState, "pMultisampleState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pDepthStencilState\": " << std::endl;
     if (obj.pDepthStencilState) {
           print_VkPipelineDepthStencilStateCreateInfo(obj.pDepthStencilState, "pDepthStencilState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pColorBlendState\": " << std::endl;
     if (obj.pColorBlendState) {
           print_VkPipelineColorBlendStateCreateInfo(obj.pColorBlendState, "pColorBlendState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pDynamicState\": " << std::endl;
     if (obj.pDynamicState) {
           print_VkPipelineDynamicStateCreateInfo(obj.pDynamicState, "pDynamicState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "layout" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "renderPass" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj.subpass, "subpass", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "basePipelineHandle" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_int32_t(obj.basePipelineIndex, "basePipelineIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkGraphicsPipelineCreateInfo(const VkGraphicsPipelineCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->stageCount, "stageCount", 1);

     PRINT_SPACE
     _OUT << "\"pStages\": " << std::endl;
     if (obj->pStages) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->stageCount; i++) {
           if (i+1 == obj->stageCount)
               print_VkPipelineShaderStageCreateInfo(obj->pStages[i], "pStages", 0);
           else
               print_VkPipelineShaderStageCreateInfo(obj->pStages[i], "pStages", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pVertexInputState\": " << std::endl;
     if (obj->pVertexInputState) {
           print_VkPipelineVertexInputStateCreateInfo(obj->pVertexInputState, "pVertexInputState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pInputAssemblyState\": " << std::endl;
     if (obj->pInputAssemblyState) {
           print_VkPipelineInputAssemblyStateCreateInfo(obj->pInputAssemblyState, "pInputAssemblyState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pTessellationState\": " << std::endl;
     if (obj->pTessellationState) {
           print_VkPipelineTessellationStateCreateInfo(obj->pTessellationState, "pTessellationState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pViewportState\": " << std::endl;
     if (obj->pViewportState) {
           print_VkPipelineViewportStateCreateInfo(obj->pViewportState, "pViewportState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pRasterizationState\": " << std::endl;
     if (obj->pRasterizationState) {
           print_VkPipelineRasterizationStateCreateInfo(obj->pRasterizationState, "pRasterizationState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pMultisampleState\": " << std::endl;
     if (obj->pMultisampleState) {
           print_VkPipelineMultisampleStateCreateInfo(obj->pMultisampleState, "pMultisampleState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pDepthStencilState\": " << std::endl;
     if (obj->pDepthStencilState) {
           print_VkPipelineDepthStencilStateCreateInfo(obj->pDepthStencilState, "pDepthStencilState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pColorBlendState\": " << std::endl;
     if (obj->pColorBlendState) {
           print_VkPipelineColorBlendStateCreateInfo(obj->pColorBlendState, "pColorBlendState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pDynamicState\": " << std::endl;
     if (obj->pDynamicState) {
           print_VkPipelineDynamicStateCreateInfo(obj->pDynamicState, "pDynamicState", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "layout" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "renderPass" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj->subpass, "subpass", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "basePipelineHandle" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_int32_t(obj->basePipelineIndex, "basePipelineIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPushConstantRange(VkPushConstantRange obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkShaderStageFlags(obj.stageFlags, "stageFlags", 1);

     print_uint32_t(obj.offset, "offset", 1);

     print_uint32_t(obj.size, "size", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPushConstantRange(const VkPushConstantRange * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkShaderStageFlags(obj->stageFlags, "stageFlags", 1);

     print_uint32_t(obj->offset, "offset", 1);

     print_uint32_t(obj->size, "size", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineLayoutCreateInfo(VkPipelineLayoutCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineLayoutCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.setLayoutCount, "setLayoutCount", 1);

     PRINT_SPACE
     _OUT << "\"pSetLayouts\":" << std::endl;
     PRINT_SPACE
     if (obj.pSetLayouts) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.setLayoutCount; i++) {
           std:: stringstream tmp;
           tmp << "pSetLayouts" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.setLayoutCount;
           print_VkDescriptorSetLayout(obj.pSetLayouts[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj.pushConstantRangeCount, "pushConstantRangeCount", 1);

     PRINT_SPACE
     _OUT << "\"pPushConstantRanges\": " << std::endl;
     if (obj.pPushConstantRanges) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.pushConstantRangeCount; i++) {
           if (i+1 == obj.pushConstantRangeCount)
               print_VkPushConstantRange(obj.pPushConstantRanges[i], "pPushConstantRanges", 0);
           else
               print_VkPushConstantRange(obj.pPushConstantRanges[i], "pPushConstantRanges", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineLayoutCreateInfo(const VkPipelineLayoutCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPipelineLayoutCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->setLayoutCount, "setLayoutCount", 1);

     PRINT_SPACE
     _OUT << "\"pSetLayouts\":" << std::endl;
     PRINT_SPACE
     if (obj->pSetLayouts) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->setLayoutCount; i++) {
           std:: stringstream tmp;
           tmp << "pSetLayouts" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->setLayoutCount;
           print_VkDescriptorSetLayout(obj->pSetLayouts[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj->pushConstantRangeCount, "pushConstantRangeCount", 1);

     PRINT_SPACE
     _OUT << "\"pPushConstantRanges\": " << std::endl;
     if (obj->pPushConstantRanges) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->pushConstantRangeCount; i++) {
           if (i+1 == obj->pushConstantRangeCount)
               print_VkPushConstantRange(obj->pPushConstantRanges[i], "pPushConstantRanges", 0);
           else
               print_VkPushConstantRange(obj->pPushConstantRanges[i], "pPushConstantRanges", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSamplerCreateInfo(VkSamplerCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSamplerCreateFlags(obj.flags, "flags", 1);

     print_VkFilter(obj.magFilter, "magFilter", 1);

     print_VkFilter(obj.minFilter, "minFilter", 1);

     print_VkSamplerMipmapMode(obj.mipmapMode, "mipmapMode", 1);

     print_VkSamplerAddressMode(obj.addressModeU, "addressModeU", 1);

     print_VkSamplerAddressMode(obj.addressModeV, "addressModeV", 1);

     print_VkSamplerAddressMode(obj.addressModeW, "addressModeW", 1);

     print_float(obj.mipLodBias, "mipLodBias", 1);

     print_VkBool32(obj.anisotropyEnable, "anisotropyEnable", 1);

     print_float(obj.maxAnisotropy, "maxAnisotropy", 1);

     print_VkBool32(obj.compareEnable, "compareEnable", 1);

     print_VkCompareOp(obj.compareOp, "compareOp", 1);

     print_float(obj.minLod, "minLod", 1);

     print_float(obj.maxLod, "maxLod", 1);

     print_VkBorderColor(obj.borderColor, "borderColor", 1);

     print_VkBool32(obj.unnormalizedCoordinates, "unnormalizedCoordinates", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSamplerCreateInfo(const VkSamplerCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSamplerCreateFlags(obj->flags, "flags", 1);

     print_VkFilter(obj->magFilter, "magFilter", 1);

     print_VkFilter(obj->minFilter, "minFilter", 1);

     print_VkSamplerMipmapMode(obj->mipmapMode, "mipmapMode", 1);

     print_VkSamplerAddressMode(obj->addressModeU, "addressModeU", 1);

     print_VkSamplerAddressMode(obj->addressModeV, "addressModeV", 1);

     print_VkSamplerAddressMode(obj->addressModeW, "addressModeW", 1);

     print_float(obj->mipLodBias, "mipLodBias", 1);

     print_VkBool32(obj->anisotropyEnable, "anisotropyEnable", 1);

     print_float(obj->maxAnisotropy, "maxAnisotropy", 1);

     print_VkBool32(obj->compareEnable, "compareEnable", 1);

     print_VkCompareOp(obj->compareOp, "compareOp", 1);

     print_float(obj->minLod, "minLod", 1);

     print_float(obj->maxLod, "maxLod", 1);

     print_VkBorderColor(obj->borderColor, "borderColor", 1);

     print_VkBool32(obj->unnormalizedCoordinates, "unnormalizedCoordinates", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkCopyDescriptorSet(VkCopyDescriptorSet obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "srcSet" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj.srcBinding, "srcBinding", 1);

     print_uint32_t(obj.srcArrayElement, "srcArrayElement", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "dstSet" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj.dstBinding, "dstBinding", 1);

     print_uint32_t(obj.dstArrayElement, "dstArrayElement", 1);

     print_uint32_t(obj.descriptorCount, "descriptorCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkCopyDescriptorSet(const VkCopyDescriptorSet * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "srcSet" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj->srcBinding, "srcBinding", 1);

     print_uint32_t(obj->srcArrayElement, "srcArrayElement", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "dstSet" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj->dstBinding, "dstBinding", 1);

     print_uint32_t(obj->dstArrayElement, "dstArrayElement", 1);

     print_uint32_t(obj->descriptorCount, "descriptorCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorBufferInfo(VkDescriptorBufferInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj.offset, "offset", 1);

     print_VkDeviceSize(obj.range, "range", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorBufferInfo(const VkDescriptorBufferInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj->offset, "offset", 1);

     print_VkDeviceSize(obj->range, "range", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorImageInfo(VkDescriptorImageInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "sampler" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "imageView" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkImageLayout(obj.imageLayout, "imageLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorImageInfo(const VkDescriptorImageInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "sampler" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "imageView" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkImageLayout(obj->imageLayout, "imageLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorPoolSize(VkDescriptorPoolSize obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDescriptorType(obj.type, "type", 1);

     print_uint32_t(obj.descriptorCount, "descriptorCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorPoolSize(const VkDescriptorPoolSize * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDescriptorType(obj->type, "type", 1);

     print_uint32_t(obj->descriptorCount, "descriptorCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorPoolCreateInfo(VkDescriptorPoolCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDescriptorPoolCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.maxSets, "maxSets", 1);

     print_uint32_t(obj.poolSizeCount, "poolSizeCount", 1);

     PRINT_SPACE
     _OUT << "\"pPoolSizes\": " << std::endl;
     if (obj.pPoolSizes) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.poolSizeCount; i++) {
           if (i+1 == obj.poolSizeCount)
               print_VkDescriptorPoolSize(obj.pPoolSizes[i], "pPoolSizes", 0);
           else
               print_VkDescriptorPoolSize(obj.pPoolSizes[i], "pPoolSizes", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorPoolCreateInfo(const VkDescriptorPoolCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDescriptorPoolCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->maxSets, "maxSets", 1);

     print_uint32_t(obj->poolSizeCount, "poolSizeCount", 1);

     PRINT_SPACE
     _OUT << "\"pPoolSizes\": " << std::endl;
     if (obj->pPoolSizes) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->poolSizeCount; i++) {
           if (i+1 == obj->poolSizeCount)
               print_VkDescriptorPoolSize(obj->pPoolSizes[i], "pPoolSizes", 0);
           else
               print_VkDescriptorPoolSize(obj->pPoolSizes[i], "pPoolSizes", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorSetAllocateInfo(VkDescriptorSetAllocateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "descriptorPool" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj.descriptorSetCount, "descriptorSetCount", 1);

     PRINT_SPACE
     _OUT << "\"pSetLayouts\":" << std::endl;
     PRINT_SPACE
     if (obj.pSetLayouts) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.descriptorSetCount; i++) {
           std:: stringstream tmp;
           tmp << "pSetLayouts" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.descriptorSetCount;
           print_VkDescriptorSetLayout(obj.pSetLayouts[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorSetAllocateInfo(const VkDescriptorSetAllocateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "descriptorPool" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj->descriptorSetCount, "descriptorSetCount", 1);

     PRINT_SPACE
     _OUT << "\"pSetLayouts\":" << std::endl;
     PRINT_SPACE
     if (obj->pSetLayouts) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->descriptorSetCount; i++) {
           std:: stringstream tmp;
           tmp << "pSetLayouts" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->descriptorSetCount;
           print_VkDescriptorSetLayout(obj->pSetLayouts[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorSetLayoutBinding(VkDescriptorSetLayoutBinding obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.binding, "binding", 1);

     print_VkDescriptorType(obj.descriptorType, "descriptorType", 1);

     print_uint32_t(obj.descriptorCount, "descriptorCount", 1);

     print_VkShaderStageFlags(obj.stageFlags, "stageFlags", 1);

     PRINT_SPACE
     _OUT << "\"pImmutableSamplers\":" << std::endl;
     PRINT_SPACE
     if (obj.pImmutableSamplers) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.descriptorCount; i++) {
           std:: stringstream tmp;
           tmp << "pImmutableSamplers" << "_" << (obj.binding) << "_" << i;
           bool isCommaNeeded = (i+1) != obj.descriptorCount;
           print_VkSampler(obj.pImmutableSamplers[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorSetLayoutBinding(const VkDescriptorSetLayoutBinding * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->binding, "binding", 1);

     print_VkDescriptorType(obj->descriptorType, "descriptorType", 1);

     print_uint32_t(obj->descriptorCount, "descriptorCount", 1);

     print_VkShaderStageFlags(obj->stageFlags, "stageFlags", 1);

     PRINT_SPACE
     _OUT << "\"pImmutableSamplers\":" << std::endl;
     PRINT_SPACE
     if (obj->pImmutableSamplers) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->descriptorCount; i++) {
           std:: stringstream tmp;
           tmp << "pImmutableSamplers" << "_" << (obj->binding) << "_" << i;
           bool isCommaNeeded = (i+1) != obj->descriptorCount;
           print_VkSampler(obj->pImmutableSamplers[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorSetLayoutCreateInfo(VkDescriptorSetLayoutCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDescriptorSetLayoutCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.bindingCount, "bindingCount", 1);

     PRINT_SPACE
     _OUT << "\"pBindings\": " << std::endl;
     if (obj.pBindings) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.bindingCount; i++) {
           if (i+1 == obj.bindingCount)
               print_VkDescriptorSetLayoutBinding(obj.pBindings[i], "pBindings", 0);
           else
               print_VkDescriptorSetLayoutBinding(obj.pBindings[i], "pBindings", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDescriptorSetLayoutCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->bindingCount, "bindingCount", 1);

     PRINT_SPACE
     _OUT << "\"pBindings\": " << std::endl;
     if (obj->pBindings) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->bindingCount; i++) {
           if (i+1 == obj->bindingCount)
               print_VkDescriptorSetLayoutBinding(obj->pBindings[i], "pBindings", 0);
           else
               print_VkDescriptorSetLayoutBinding(obj->pBindings[i], "pBindings", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkWriteDescriptorSet(VkWriteDescriptorSet obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "dstSet" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj.dstBinding, "dstBinding", 1);

     print_uint32_t(obj.dstArrayElement, "dstArrayElement", 1);

     print_uint32_t(obj.descriptorCount, "descriptorCount", 1);

     print_VkDescriptorType(obj.descriptorType, "descriptorType", 1);

     PRINT_SPACE
     _OUT << "\"pImageInfo\": " << std::endl;
     if (obj.pImageInfo) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.descriptorCount; i++) {
           if (i+1 == obj.descriptorCount)
               print_VkDescriptorImageInfo(obj.pImageInfo[i], "pImageInfo", 0);
           else
               print_VkDescriptorImageInfo(obj.pImageInfo[i], "pImageInfo", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pBufferInfo\": " << std::endl;
     if (obj.pBufferInfo) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.descriptorCount; i++) {
           if (i+1 == obj.descriptorCount)
               print_VkDescriptorBufferInfo(obj.pBufferInfo[i], "pBufferInfo", 0);
           else
               print_VkDescriptorBufferInfo(obj.pBufferInfo[i], "pBufferInfo", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pTexelBufferView\":" << std::endl;
     PRINT_SPACE
     if (obj.pTexelBufferView) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.descriptorCount; i++) {
           std:: stringstream tmp;
           tmp << "pTexelBufferView" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.descriptorCount;
           print_VkBufferView(obj.pTexelBufferView[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkWriteDescriptorSet(const VkWriteDescriptorSet * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "dstSet" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj->dstBinding, "dstBinding", 1);

     print_uint32_t(obj->dstArrayElement, "dstArrayElement", 1);

     print_uint32_t(obj->descriptorCount, "descriptorCount", 1);

     print_VkDescriptorType(obj->descriptorType, "descriptorType", 1);

     PRINT_SPACE
     _OUT << "\"pImageInfo\": " << std::endl;
     if (obj->pImageInfo) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->descriptorCount; i++) {
           if (i+1 == obj->descriptorCount)
               print_VkDescriptorImageInfo(obj->pImageInfo[i], "pImageInfo", 0);
           else
               print_VkDescriptorImageInfo(obj->pImageInfo[i], "pImageInfo", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pBufferInfo\": " << std::endl;
     if (obj->pBufferInfo) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->descriptorCount; i++) {
           if (i+1 == obj->descriptorCount)
               print_VkDescriptorBufferInfo(obj->pBufferInfo[i], "pBufferInfo", 0);
           else
               print_VkDescriptorBufferInfo(obj->pBufferInfo[i], "pBufferInfo", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pTexelBufferView\":" << std::endl;
     PRINT_SPACE
     if (obj->pTexelBufferView) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->descriptorCount; i++) {
           std:: stringstream tmp;
           tmp << "pTexelBufferView" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->descriptorCount;
           print_VkBufferView(obj->pTexelBufferView[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkAttachmentDescription(VkAttachmentDescription obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkAttachmentDescriptionFlags(obj.flags, "flags", 1);

     print_VkFormat(obj.format, "format", 1);

     print_VkSampleCountFlagBits(obj.samples, "samples", 1);

     print_VkAttachmentLoadOp(obj.loadOp, "loadOp", 1);

     print_VkAttachmentStoreOp(obj.storeOp, "storeOp", 1);

     print_VkAttachmentLoadOp(obj.stencilLoadOp, "stencilLoadOp", 1);

     print_VkAttachmentStoreOp(obj.stencilStoreOp, "stencilStoreOp", 1);

     print_VkImageLayout(obj.initialLayout, "initialLayout", 1);

     print_VkImageLayout(obj.finalLayout, "finalLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkAttachmentDescription(const VkAttachmentDescription * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkAttachmentDescriptionFlags(obj->flags, "flags", 1);

     print_VkFormat(obj->format, "format", 1);

     print_VkSampleCountFlagBits(obj->samples, "samples", 1);

     print_VkAttachmentLoadOp(obj->loadOp, "loadOp", 1);

     print_VkAttachmentStoreOp(obj->storeOp, "storeOp", 1);

     print_VkAttachmentLoadOp(obj->stencilLoadOp, "stencilLoadOp", 1);

     print_VkAttachmentStoreOp(obj->stencilStoreOp, "stencilStoreOp", 1);

     print_VkImageLayout(obj->initialLayout, "initialLayout", 1);

     print_VkImageLayout(obj->finalLayout, "finalLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkAttachmentReference(VkAttachmentReference obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.attachment, "attachment", 1);

     print_VkImageLayout(obj.layout, "layout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkAttachmentReference(const VkAttachmentReference * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->attachment, "attachment", 1);

     print_VkImageLayout(obj->layout, "layout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkFramebufferCreateInfo(VkFramebufferCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkFramebufferCreateFlags(obj.flags, "flags", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "renderPass" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj.attachmentCount, "attachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachments\":" << std::endl;
     PRINT_SPACE
     if (obj.pAttachments) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.attachmentCount; i++) {
           std:: stringstream tmp;
           tmp << "pAttachments" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.attachmentCount;
           print_VkImageView(obj.pAttachments[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj.width, "width", 1);

     print_uint32_t(obj.height, "height", 1);

     print_uint32_t(obj.layers, "layers", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkFramebufferCreateInfo(const VkFramebufferCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkFramebufferCreateFlags(obj->flags, "flags", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "renderPass" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj->attachmentCount, "attachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachments\":" << std::endl;
     PRINT_SPACE
     if (obj->pAttachments) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->attachmentCount; i++) {
           std:: stringstream tmp;
           tmp << "pAttachments" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->attachmentCount;
           print_VkImageView(obj->pAttachments[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj->width, "width", 1);

     print_uint32_t(obj->height, "height", 1);

     print_uint32_t(obj->layers, "layers", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSubpassDescription(VkSubpassDescription obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkSubpassDescriptionFlags(obj.flags, "flags", 1);

     print_VkPipelineBindPoint(obj.pipelineBindPoint, "pipelineBindPoint", 1);

     print_uint32_t(obj.inputAttachmentCount, "inputAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pInputAttachments\": " << std::endl;
     if (obj.pInputAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.inputAttachmentCount; i++) {
           if (i+1 == obj.inputAttachmentCount)
               print_VkAttachmentReference(obj.pInputAttachments[i], "pInputAttachments", 0);
           else
               print_VkAttachmentReference(obj.pInputAttachments[i], "pInputAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.colorAttachmentCount, "colorAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pColorAttachments\": " << std::endl;
     if (obj.pColorAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.colorAttachmentCount; i++) {
           if (i+1 == obj.colorAttachmentCount)
               print_VkAttachmentReference(obj.pColorAttachments[i], "pColorAttachments", 0);
           else
               print_VkAttachmentReference(obj.pColorAttachments[i], "pColorAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pResolveAttachments\": " << std::endl;
     if (obj.pResolveAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.colorAttachmentCount; i++) {
           if (i+1 == obj.colorAttachmentCount)
               print_VkAttachmentReference(obj.pResolveAttachments[i], "pResolveAttachments", 0);
           else
               print_VkAttachmentReference(obj.pResolveAttachments[i], "pResolveAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pDepthStencilAttachment\": " << std::endl;
     if (obj.pDepthStencilAttachment) {
           print_VkAttachmentReference(obj.pDepthStencilAttachment, "pDepthStencilAttachment", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.preserveAttachmentCount, "preserveAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pPreserveAttachments\":" << std::endl;
     PRINT_SPACE
     if (obj.pPreserveAttachments) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.preserveAttachmentCount; i++) {
           bool isCommaNeeded = (i+1) != obj.preserveAttachmentCount;
           print_uint32_t(obj.pPreserveAttachments[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSubpassDescription(const VkSubpassDescription * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkSubpassDescriptionFlags(obj->flags, "flags", 1);

     print_VkPipelineBindPoint(obj->pipelineBindPoint, "pipelineBindPoint", 1);

     print_uint32_t(obj->inputAttachmentCount, "inputAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pInputAttachments\": " << std::endl;
     if (obj->pInputAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->inputAttachmentCount; i++) {
           if (i+1 == obj->inputAttachmentCount)
               print_VkAttachmentReference(obj->pInputAttachments[i], "pInputAttachments", 0);
           else
               print_VkAttachmentReference(obj->pInputAttachments[i], "pInputAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->colorAttachmentCount, "colorAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pColorAttachments\": " << std::endl;
     if (obj->pColorAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->colorAttachmentCount; i++) {
           if (i+1 == obj->colorAttachmentCount)
               print_VkAttachmentReference(obj->pColorAttachments[i], "pColorAttachments", 0);
           else
               print_VkAttachmentReference(obj->pColorAttachments[i], "pColorAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pResolveAttachments\": " << std::endl;
     if (obj->pResolveAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->colorAttachmentCount; i++) {
           if (i+1 == obj->colorAttachmentCount)
               print_VkAttachmentReference(obj->pResolveAttachments[i], "pResolveAttachments", 0);
           else
               print_VkAttachmentReference(obj->pResolveAttachments[i], "pResolveAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pDepthStencilAttachment\": " << std::endl;
     if (obj->pDepthStencilAttachment) {
           print_VkAttachmentReference(obj->pDepthStencilAttachment, "pDepthStencilAttachment", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->preserveAttachmentCount, "preserveAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pPreserveAttachments\":" << std::endl;
     PRINT_SPACE
     if (obj->pPreserveAttachments) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->preserveAttachmentCount; i++) {
           bool isCommaNeeded = (i+1) != obj->preserveAttachmentCount;
           print_uint32_t(obj->pPreserveAttachments[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSubpassDependency(VkSubpassDependency obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.srcSubpass, "srcSubpass", 1);

     print_uint32_t(obj.dstSubpass, "dstSubpass", 1);

     print_VkPipelineStageFlags(obj.srcStageMask, "srcStageMask", 1);

     print_VkPipelineStageFlags(obj.dstStageMask, "dstStageMask", 1);

     print_VkAccessFlags(obj.srcAccessMask, "srcAccessMask", 1);

     print_VkAccessFlags(obj.dstAccessMask, "dstAccessMask", 1);

     print_VkDependencyFlags(obj.dependencyFlags, "dependencyFlags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSubpassDependency(const VkSubpassDependency * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->srcSubpass, "srcSubpass", 1);

     print_uint32_t(obj->dstSubpass, "dstSubpass", 1);

     print_VkPipelineStageFlags(obj->srcStageMask, "srcStageMask", 1);

     print_VkPipelineStageFlags(obj->dstStageMask, "dstStageMask", 1);

     print_VkAccessFlags(obj->srcAccessMask, "srcAccessMask", 1);

     print_VkAccessFlags(obj->dstAccessMask, "dstAccessMask", 1);

     print_VkDependencyFlags(obj->dependencyFlags, "dependencyFlags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkRenderPassCreateInfo(VkRenderPassCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkRenderPassCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.attachmentCount, "attachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachments\": " << std::endl;
     if (obj.pAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.attachmentCount; i++) {
           if (i+1 == obj.attachmentCount)
               print_VkAttachmentDescription(obj.pAttachments[i], "pAttachments", 0);
           else
               print_VkAttachmentDescription(obj.pAttachments[i], "pAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.subpassCount, "subpassCount", 1);

     PRINT_SPACE
     _OUT << "\"pSubpasses\": " << std::endl;
     if (obj.pSubpasses) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.subpassCount; i++) {
           if (i+1 == obj.subpassCount)
               print_VkSubpassDescription(obj.pSubpasses[i], "pSubpasses", 0);
           else
               print_VkSubpassDescription(obj.pSubpasses[i], "pSubpasses", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.dependencyCount, "dependencyCount", 1);

     PRINT_SPACE
     _OUT << "\"pDependencies\": " << std::endl;
     if (obj.pDependencies) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.dependencyCount; i++) {
           if (i+1 == obj.dependencyCount)
               print_VkSubpassDependency(obj.pDependencies[i], "pDependencies", 0);
           else
               print_VkSubpassDependency(obj.pDependencies[i], "pDependencies", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkRenderPassCreateInfo(const VkRenderPassCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkRenderPassCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->attachmentCount, "attachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachments\": " << std::endl;
     if (obj->pAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->attachmentCount; i++) {
           if (i+1 == obj->attachmentCount)
               print_VkAttachmentDescription(obj->pAttachments[i], "pAttachments", 0);
           else
               print_VkAttachmentDescription(obj->pAttachments[i], "pAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->subpassCount, "subpassCount", 1);

     PRINT_SPACE
     _OUT << "\"pSubpasses\": " << std::endl;
     if (obj->pSubpasses) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->subpassCount; i++) {
           if (i+1 == obj->subpassCount)
               print_VkSubpassDescription(obj->pSubpasses[i], "pSubpasses", 0);
           else
               print_VkSubpassDescription(obj->pSubpasses[i], "pSubpasses", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->dependencyCount, "dependencyCount", 1);

     PRINT_SPACE
     _OUT << "\"pDependencies\": " << std::endl;
     if (obj->pDependencies) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->dependencyCount; i++) {
           if (i+1 == obj->dependencyCount)
               print_VkSubpassDependency(obj->pDependencies[i], "pDependencies", 0);
           else
               print_VkSubpassDependency(obj->pDependencies[i], "pDependencies", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkCommandPoolCreateInfo(VkCommandPoolCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkCommandPoolCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.queueFamilyIndex, "queueFamilyIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkCommandPoolCreateInfo(const VkCommandPoolCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkCommandPoolCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->queueFamilyIndex, "queueFamilyIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkCommandBufferAllocateInfo(VkCommandBufferAllocateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "commandPool" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkCommandBufferLevel(obj.level, "level", 1);

     print_uint32_t(obj.commandBufferCount, "commandBufferCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkCommandBufferAllocateInfo(const VkCommandBufferAllocateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "commandPool" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkCommandBufferLevel(obj->level, "level", 1);

     print_uint32_t(obj->commandBufferCount, "commandBufferCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkCommandBufferInheritanceInfo(VkCommandBufferInheritanceInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "renderPass" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj.subpass, "subpass", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "framebuffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkBool32(obj.occlusionQueryEnable, "occlusionQueryEnable", 1);

     print_VkQueryControlFlags(obj.queryFlags, "queryFlags", 1);

     print_VkQueryPipelineStatisticFlags(obj.pipelineStatistics, "pipelineStatistics", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkCommandBufferInheritanceInfo(const VkCommandBufferInheritanceInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "renderPass" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj->subpass, "subpass", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "framebuffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkBool32(obj->occlusionQueryEnable, "occlusionQueryEnable", 1);

     print_VkQueryControlFlags(obj->queryFlags, "queryFlags", 1);

     print_VkQueryPipelineStatisticFlags(obj->pipelineStatistics, "pipelineStatistics", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkCommandBufferBeginInfo(VkCommandBufferBeginInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkCommandBufferUsageFlags(obj.flags, "flags", 1);

     PRINT_SPACE
     _OUT << "\"pInheritanceInfo\": " << std::endl;
     if (obj.pInheritanceInfo) {
           print_VkCommandBufferInheritanceInfo(obj.pInheritanceInfo, "pInheritanceInfo", 0);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkCommandBufferBeginInfo(const VkCommandBufferBeginInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkCommandBufferUsageFlags(obj->flags, "flags", 1);

     PRINT_SPACE
     _OUT << "\"pInheritanceInfo\": " << std::endl;
     if (obj->pInheritanceInfo) {
           print_VkCommandBufferInheritanceInfo(obj->pInheritanceInfo, "pInheritanceInfo", 0);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBufferCopy(VkBufferCopy obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj.srcOffset, "srcOffset", 1);

     print_VkDeviceSize(obj.dstOffset, "dstOffset", 1);

     print_VkDeviceSize(obj.size, "size", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBufferCopy(const VkBufferCopy * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj->srcOffset, "srcOffset", 1);

     print_VkDeviceSize(obj->dstOffset, "dstOffset", 1);

     print_VkDeviceSize(obj->size, "size", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageSubresourceLayers(VkImageSubresourceLayers obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkImageAspectFlags(obj.aspectMask, "aspectMask", 1);

     print_uint32_t(obj.mipLevel, "mipLevel", 1);

     print_uint32_t(obj.baseArrayLayer, "baseArrayLayer", 1);

     print_uint32_t(obj.layerCount, "layerCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageSubresourceLayers(const VkImageSubresourceLayers * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkImageAspectFlags(obj->aspectMask, "aspectMask", 1);

     print_uint32_t(obj->mipLevel, "mipLevel", 1);

     print_uint32_t(obj->baseArrayLayer, "baseArrayLayer", 1);

     print_uint32_t(obj->layerCount, "layerCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBufferImageCopy(VkBufferImageCopy obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj.bufferOffset, "bufferOffset", 1);

     print_uint32_t(obj.bufferRowLength, "bufferRowLength", 1);

     print_uint32_t(obj.bufferImageHeight, "bufferImageHeight", 1);

     PRINT_SPACE
     _OUT << "\"imageSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj.imageSubresource, "imageSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"imageOffset\": " << std::endl;
     {
           print_VkOffset3D(obj.imageOffset, "imageOffset", 1);
     }

     PRINT_SPACE
     _OUT << "\"imageExtent\": " << std::endl;
     {
           print_VkExtent3D(obj.imageExtent, "imageExtent", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBufferImageCopy(const VkBufferImageCopy * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkDeviceSize(obj->bufferOffset, "bufferOffset", 1);

     print_uint32_t(obj->bufferRowLength, "bufferRowLength", 1);

     print_uint32_t(obj->bufferImageHeight, "bufferImageHeight", 1);

     PRINT_SPACE
     _OUT << "\"imageSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj->imageSubresource, "imageSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"imageOffset\": " << std::endl;
     {
           print_VkOffset3D(obj->imageOffset, "imageOffset", 1);
     }

     PRINT_SPACE
     _OUT << "\"imageExtent\": " << std::endl;
     {
           print_VkExtent3D(obj->imageExtent, "imageExtent", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkClearColorValue(VkClearColorValue obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"float32\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 4; i++) {
           bool isCommaNeeded = (i+1) != 4;
           print_float(obj.float32[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"int32\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 4; i++) {
           bool isCommaNeeded = (i+1) != 4;
           print_int32_t(obj.int32[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"uint32\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 4; i++) {
           bool isCommaNeeded = (i+1) != 4;
           print_uint32_t(obj.uint32[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkClearColorValue(const VkClearColorValue * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"float32\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 4; i++) {
           bool isCommaNeeded = (i+1) != 4;
           print_float(obj->float32[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"int32\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 4; i++) {
           bool isCommaNeeded = (i+1) != 4;
           print_int32_t(obj->int32[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"uint32\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 4; i++) {
           bool isCommaNeeded = (i+1) != 4;
           print_uint32_t(obj->uint32[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkClearDepthStencilValue(VkClearDepthStencilValue obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_float(obj.depth, "depth", 1);

     print_uint32_t(obj.stencil, "stencil", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkClearDepthStencilValue(const VkClearDepthStencilValue * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_float(obj->depth, "depth", 1);

     print_uint32_t(obj->stencil, "stencil", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkClearValue(VkClearValue obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkClearColorValue(obj.color, "color", 1);

     PRINT_SPACE
     _OUT << "\"depthStencil\": " << std::endl;
     {
           print_VkClearDepthStencilValue(obj.depthStencil, "depthStencil", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkClearValue(const VkClearValue * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkClearColorValue(obj->color, "color", 1);

     PRINT_SPACE
     _OUT << "\"depthStencil\": " << std::endl;
     {
           print_VkClearDepthStencilValue(obj->depthStencil, "depthStencil", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkClearAttachment(VkClearAttachment obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkImageAspectFlags(obj.aspectMask, "aspectMask", 1);

     print_uint32_t(obj.colorAttachment, "colorAttachment", 1);

     print_VkClearValue(obj.clearValue, "clearValue", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkClearAttachment(const VkClearAttachment * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkImageAspectFlags(obj->aspectMask, "aspectMask", 1);

     print_uint32_t(obj->colorAttachment, "colorAttachment", 1);

     print_VkClearValue(obj->clearValue, "clearValue", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkClearRect(VkClearRect obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"rect\": " << std::endl;
     {
           print_VkRect2D(obj.rect, "rect", 1);
     }

     print_uint32_t(obj.baseArrayLayer, "baseArrayLayer", 1);

     print_uint32_t(obj.layerCount, "layerCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkClearRect(const VkClearRect * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"rect\": " << std::endl;
     {
           print_VkRect2D(obj->rect, "rect", 1);
     }

     print_uint32_t(obj->baseArrayLayer, "baseArrayLayer", 1);

     print_uint32_t(obj->layerCount, "layerCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageBlit(VkImageBlit obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"srcSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj.srcSubresource, "srcSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"srcOffsets\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_VkOffset3D(obj.srcOffsets[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"dstSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj.dstSubresource, "dstSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"dstOffsets\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_VkOffset3D(obj.dstOffsets[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageBlit(const VkImageBlit * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"srcSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj->srcSubresource, "srcSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"srcOffsets\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_VkOffset3D(obj->srcOffsets[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"dstSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj->dstSubresource, "dstSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"dstOffsets\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < 2; i++) {
           bool isCommaNeeded = (i+1) != 2;
           print_VkOffset3D(obj->dstOffsets[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageCopy(VkImageCopy obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"srcSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj.srcSubresource, "srcSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"srcOffset\": " << std::endl;
     {
           print_VkOffset3D(obj.srcOffset, "srcOffset", 1);
     }

     PRINT_SPACE
     _OUT << "\"dstSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj.dstSubresource, "dstSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"dstOffset\": " << std::endl;
     {
           print_VkOffset3D(obj.dstOffset, "dstOffset", 1);
     }

     PRINT_SPACE
     _OUT << "\"extent\": " << std::endl;
     {
           print_VkExtent3D(obj.extent, "extent", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageCopy(const VkImageCopy * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"srcSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj->srcSubresource, "srcSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"srcOffset\": " << std::endl;
     {
           print_VkOffset3D(obj->srcOffset, "srcOffset", 1);
     }

     PRINT_SPACE
     _OUT << "\"dstSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj->dstSubresource, "dstSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"dstOffset\": " << std::endl;
     {
           print_VkOffset3D(obj->dstOffset, "dstOffset", 1);
     }

     PRINT_SPACE
     _OUT << "\"extent\": " << std::endl;
     {
           print_VkExtent3D(obj->extent, "extent", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageResolve(VkImageResolve obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"srcSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj.srcSubresource, "srcSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"srcOffset\": " << std::endl;
     {
           print_VkOffset3D(obj.srcOffset, "srcOffset", 1);
     }

     PRINT_SPACE
     _OUT << "\"dstSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj.dstSubresource, "dstSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"dstOffset\": " << std::endl;
     {
           print_VkOffset3D(obj.dstOffset, "dstOffset", 1);
     }

     PRINT_SPACE
     _OUT << "\"extent\": " << std::endl;
     {
           print_VkExtent3D(obj.extent, "extent", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageResolve(const VkImageResolve * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     PRINT_SPACE
     _OUT << "\"srcSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj->srcSubresource, "srcSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"srcOffset\": " << std::endl;
     {
           print_VkOffset3D(obj->srcOffset, "srcOffset", 1);
     }

     PRINT_SPACE
     _OUT << "\"dstSubresource\": " << std::endl;
     {
           print_VkImageSubresourceLayers(obj->dstSubresource, "dstSubresource", 1);
     }

     PRINT_SPACE
     _OUT << "\"dstOffset\": " << std::endl;
     {
           print_VkOffset3D(obj->dstOffset, "dstOffset", 1);
     }

     PRINT_SPACE
     _OUT << "\"extent\": " << std::endl;
     {
           print_VkExtent3D(obj->extent, "extent", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkRenderPassBeginInfo(VkRenderPassBeginInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "renderPass" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "framebuffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     PRINT_SPACE
     _OUT << "\"renderArea\": " << std::endl;
     {
           print_VkRect2D(obj.renderArea, "renderArea", 1);
     }

     print_uint32_t(obj.clearValueCount, "clearValueCount", 1);

     PRINT_SPACE
     _OUT << "\"pClearValues\":" << std::endl;
     PRINT_SPACE
     if (obj.pClearValues) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.clearValueCount; i++) {
           bool isCommaNeeded = (i+1) != obj.clearValueCount;
           print_VkClearValue(obj.pClearValues[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkRenderPassBeginInfo(const VkRenderPassBeginInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "renderPass" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "framebuffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     PRINT_SPACE
     _OUT << "\"renderArea\": " << std::endl;
     {
           print_VkRect2D(obj->renderArea, "renderArea", 1);
     }

     print_uint32_t(obj->clearValueCount, "clearValueCount", 1);

     PRINT_SPACE
     _OUT << "\"pClearValues\":" << std::endl;
     PRINT_SPACE
     if (obj->pClearValues) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->clearValueCount; i++) {
           bool isCommaNeeded = (i+1) != obj->clearValueCount;
           print_VkClearValue(obj->pClearValues[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSamplerYcbcrConversion(VkSamplerYcbcrConversion obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkSamplerYcbcrConversion(const VkSamplerYcbcrConversion * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static void print_VkDescriptorUpdateTemplate(VkDescriptorUpdateTemplate obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}
static void print_VkDescriptorUpdateTemplate(const VkDescriptorUpdateTemplate * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSubgroupFeatureFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_SUBGROUP_FEATURE_BASIC_BIT"),
    std::make_pair(1ULL << 1, "VK_SUBGROUP_FEATURE_VOTE_BIT"),
    std::make_pair(1ULL << 2, "VK_SUBGROUP_FEATURE_ARITHMETIC_BIT"),
    std::make_pair(1ULL << 3, "VK_SUBGROUP_FEATURE_BALLOT_BIT"),
    std::make_pair(1ULL << 4, "VK_SUBGROUP_FEATURE_SHUFFLE_BIT"),
    std::make_pair(1ULL << 5, "VK_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT"),
    std::make_pair(1ULL << 6, "VK_SUBGROUP_FEATURE_CLUSTERED_BIT"),
    std::make_pair(1ULL << 7, "VK_SUBGROUP_FEATURE_QUAD_BIT"),
    std::make_pair(1ULL << 8, "VK_SUBGROUP_FEATURE_PARTITIONED_BIT_NV"),
};
static void print_VkSubgroupFeatureFlagBits(VkSubgroupFeatureFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSubgroupFeatureFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSubgroupFeatureFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkSubgroupFeatureFlagBits(const VkSubgroupFeatureFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSubgroupFeatureFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSubgroupFeatureFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPeerMemoryFeatureFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_PEER_MEMORY_FEATURE_COPY_SRC_BIT"),
    std::make_pair(1ULL << 1, "VK_PEER_MEMORY_FEATURE_COPY_DST_BIT"),
    std::make_pair(1ULL << 2, "VK_PEER_MEMORY_FEATURE_GENERIC_SRC_BIT"),
    std::make_pair(1ULL << 3, "VK_PEER_MEMORY_FEATURE_GENERIC_DST_BIT"),
};
static void print_VkPeerMemoryFeatureFlagBits(VkPeerMemoryFeatureFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPeerMemoryFeatureFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPeerMemoryFeatureFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkPeerMemoryFeatureFlagBits(const VkPeerMemoryFeatureFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPeerMemoryFeatureFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPeerMemoryFeatureFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkMemoryAllocateFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT"),
    std::make_pair(1ULL << 1, "VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT"),
    std::make_pair(1ULL << 2, "VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT"),
};
static void print_VkMemoryAllocateFlagBits(VkMemoryAllocateFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkMemoryAllocateFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkMemoryAllocateFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkMemoryAllocateFlagBits(const VkMemoryAllocateFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkMemoryAllocateFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkMemoryAllocateFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkPointClippingBehavior_map = {
    std::make_pair(0, "VK_POINT_CLIPPING_BEHAVIOR_ALL_CLIP_PLANES"),
    std::make_pair(1, "VK_POINT_CLIPPING_BEHAVIOR_USER_CLIP_PLANES_ONLY"),
};
static void print_VkPointClippingBehavior(VkPointClippingBehavior obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPointClippingBehavior_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPointClippingBehavior_map[obj] << "\"" << std::endl;
}
static void print_VkPointClippingBehavior(const VkPointClippingBehavior * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkPointClippingBehavior_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkPointClippingBehavior_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkTessellationDomainOrigin_map = {
    std::make_pair(0, "VK_TESSELLATION_DOMAIN_ORIGIN_UPPER_LEFT"),
    std::make_pair(1, "VK_TESSELLATION_DOMAIN_ORIGIN_LOWER_LEFT"),
};
static void print_VkTessellationDomainOrigin(VkTessellationDomainOrigin obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkTessellationDomainOrigin_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkTessellationDomainOrigin_map[obj] << "\"" << std::endl;
}
static void print_VkTessellationDomainOrigin(const VkTessellationDomainOrigin * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkTessellationDomainOrigin_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkTessellationDomainOrigin_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSamplerYcbcrModelConversion_map = {
    std::make_pair(0, "VK_SAMPLER_YCBCR_MODEL_CONVERSION_RGB_IDENTITY"),
    std::make_pair(1, "VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_IDENTITY"),
    std::make_pair(2, "VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_709"),
    std::make_pair(3, "VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_601"),
    std::make_pair(4, "VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_2020"),
};
static void print_VkSamplerYcbcrModelConversion(VkSamplerYcbcrModelConversion obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerYcbcrModelConversion_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerYcbcrModelConversion_map[obj] << "\"" << std::endl;
}
static void print_VkSamplerYcbcrModelConversion(const VkSamplerYcbcrModelConversion * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerYcbcrModelConversion_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerYcbcrModelConversion_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSamplerYcbcrRange_map = {
    std::make_pair(0, "VK_SAMPLER_YCBCR_RANGE_ITU_FULL"),
    std::make_pair(1, "VK_SAMPLER_YCBCR_RANGE_ITU_NARROW"),
};
static void print_VkSamplerYcbcrRange(VkSamplerYcbcrRange obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerYcbcrRange_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerYcbcrRange_map[obj] << "\"" << std::endl;
}
static void print_VkSamplerYcbcrRange(const VkSamplerYcbcrRange * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerYcbcrRange_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerYcbcrRange_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkChromaLocation_map = {
    std::make_pair(0, "VK_CHROMA_LOCATION_COSITED_EVEN"),
    std::make_pair(1, "VK_CHROMA_LOCATION_MIDPOINT"),
};
static void print_VkChromaLocation(VkChromaLocation obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkChromaLocation_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkChromaLocation_map[obj] << "\"" << std::endl;
}
static void print_VkChromaLocation(const VkChromaLocation * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkChromaLocation_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkChromaLocation_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkDescriptorUpdateTemplateType_map = {
    std::make_pair(0, "VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_DESCRIPTOR_SET"),
    std::make_pair(1, "VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_PUSH_DESCRIPTORS_KHR"),
    std::make_pair(1, "VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_PUSH_DESCRIPTORS_KHR"),
    std::make_pair(1, "VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_PUSH_DESCRIPTORS_KHR"),
};
static void print_VkDescriptorUpdateTemplateType(VkDescriptorUpdateTemplateType obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDescriptorUpdateTemplateType_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDescriptorUpdateTemplateType_map[obj] << "\"" << std::endl;
}
static void print_VkDescriptorUpdateTemplateType(const VkDescriptorUpdateTemplateType * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDescriptorUpdateTemplateType_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDescriptorUpdateTemplateType_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkExternalMemoryHandleTypeFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT"),
    std::make_pair(1ULL << 1, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT"),
    std::make_pair(1ULL << 2, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT"),
    std::make_pair(1ULL << 3, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_BIT"),
    std::make_pair(1ULL << 4, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_KMT_BIT"),
    std::make_pair(1ULL << 5, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_HEAP_BIT"),
    std::make_pair(1ULL << 6, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_RESOURCE_BIT"),
    std::make_pair(1ULL << 9, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_DMA_BUF_BIT_EXT"),
    std::make_pair(1ULL << 10, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_ANDROID_HARDWARE_BUFFER_BIT_ANDROID"),
    std::make_pair(1ULL << 7, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_HOST_ALLOCATION_BIT_EXT"),
    std::make_pair(1ULL << 8, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_HOST_MAPPED_FOREIGN_MEMORY_BIT_EXT"),
    std::make_pair(1ULL << 11, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_ZIRCON_VMO_BIT_FUCHSIA"),
    std::make_pair(1ULL << 12, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_RDMA_ADDRESS_BIT_NV"),
    std::make_pair(1ULL << 13, "VK_EXTERNAL_MEMORY_HANDLE_TYPE_SCI_BUF_BIT_NV"),
};
static void print_VkExternalMemoryHandleTypeFlagBits(VkExternalMemoryHandleTypeFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalMemoryHandleTypeFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalMemoryHandleTypeFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkExternalMemoryHandleTypeFlagBits(const VkExternalMemoryHandleTypeFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalMemoryHandleTypeFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalMemoryHandleTypeFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkExternalMemoryFeatureFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_EXTERNAL_MEMORY_FEATURE_DEDICATED_ONLY_BIT"),
    std::make_pair(1ULL << 1, "VK_EXTERNAL_MEMORY_FEATURE_EXPORTABLE_BIT"),
    std::make_pair(1ULL << 2, "VK_EXTERNAL_MEMORY_FEATURE_IMPORTABLE_BIT"),
};
static void print_VkExternalMemoryFeatureFlagBits(VkExternalMemoryFeatureFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalMemoryFeatureFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalMemoryFeatureFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkExternalMemoryFeatureFlagBits(const VkExternalMemoryFeatureFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalMemoryFeatureFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalMemoryFeatureFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkExternalFenceHandleTypeFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_FD_BIT"),
    std::make_pair(1ULL << 1, "VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_BIT"),
    std::make_pair(1ULL << 2, "VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT"),
    std::make_pair(1ULL << 3, "VK_EXTERNAL_FENCE_HANDLE_TYPE_SYNC_FD_BIT"),
    std::make_pair(1ULL << 4, "VK_EXTERNAL_FENCE_HANDLE_TYPE_SCI_SYNC_OBJ_BIT_NV"),
    std::make_pair(1ULL << 5, "VK_EXTERNAL_FENCE_HANDLE_TYPE_SCI_SYNC_FENCE_BIT_NV"),
    std::make_pair(1ULL << 4, "VK_EXTERNAL_FENCE_HANDLE_TYPE_SCI_SYNC_OBJ_BIT_NV"),
    std::make_pair(1ULL << 5, "VK_EXTERNAL_FENCE_HANDLE_TYPE_SCI_SYNC_FENCE_BIT_NV"),
};
static void print_VkExternalFenceHandleTypeFlagBits(VkExternalFenceHandleTypeFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalFenceHandleTypeFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalFenceHandleTypeFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkExternalFenceHandleTypeFlagBits(const VkExternalFenceHandleTypeFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalFenceHandleTypeFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalFenceHandleTypeFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkExternalFenceFeatureFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_EXTERNAL_FENCE_FEATURE_EXPORTABLE_BIT"),
    std::make_pair(1ULL << 1, "VK_EXTERNAL_FENCE_FEATURE_IMPORTABLE_BIT"),
};
static void print_VkExternalFenceFeatureFlagBits(VkExternalFenceFeatureFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalFenceFeatureFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalFenceFeatureFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkExternalFenceFeatureFlagBits(const VkExternalFenceFeatureFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalFenceFeatureFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalFenceFeatureFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkFenceImportFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_FENCE_IMPORT_TEMPORARY_BIT"),
};
static void print_VkFenceImportFlagBits(VkFenceImportFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFenceImportFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFenceImportFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkFenceImportFlagBits(const VkFenceImportFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkFenceImportFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkFenceImportFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSemaphoreImportFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_SEMAPHORE_IMPORT_TEMPORARY_BIT"),
};
static void print_VkSemaphoreImportFlagBits(VkSemaphoreImportFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSemaphoreImportFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSemaphoreImportFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkSemaphoreImportFlagBits(const VkSemaphoreImportFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSemaphoreImportFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSemaphoreImportFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkExternalSemaphoreHandleTypeFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT"),
    std::make_pair(1ULL << 1, "VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT"),
    std::make_pair(1ULL << 2, "VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT"),
    std::make_pair(1ULL << 3, "VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT"),
    std::make_pair(1ULL << 4, "VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SYNC_FD_BIT"),
    std::make_pair(1ULL << 7, "VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_ZIRCON_EVENT_BIT_FUCHSIA"),
    std::make_pair(1ULL << 5, "VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SCI_SYNC_OBJ_BIT_NV"),
};
static void print_VkExternalSemaphoreHandleTypeFlagBits(VkExternalSemaphoreHandleTypeFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalSemaphoreHandleTypeFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalSemaphoreHandleTypeFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkExternalSemaphoreHandleTypeFlagBits(const VkExternalSemaphoreHandleTypeFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalSemaphoreHandleTypeFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalSemaphoreHandleTypeFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkExternalSemaphoreFeatureFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_EXTERNAL_SEMAPHORE_FEATURE_EXPORTABLE_BIT"),
    std::make_pair(1ULL << 1, "VK_EXTERNAL_SEMAPHORE_FEATURE_IMPORTABLE_BIT"),
};
static void print_VkExternalSemaphoreFeatureFlagBits(VkExternalSemaphoreFeatureFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalSemaphoreFeatureFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalSemaphoreFeatureFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkExternalSemaphoreFeatureFlagBits(const VkExternalSemaphoreFeatureFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkExternalSemaphoreFeatureFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkExternalSemaphoreFeatureFlagBits_map[*obj] << "\"" << std::endl;
}

static void print_VkSubgroupFeatureFlags(VkSubgroupFeatureFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSubgroupFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSubgroupFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkSubgroupFeatureFlags(const VkSubgroupFeatureFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSubgroupFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSubgroupFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkPeerMemoryFeatureFlags(VkPeerMemoryFeatureFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPeerMemoryFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPeerMemoryFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkPeerMemoryFeatureFlags(const VkPeerMemoryFeatureFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkPeerMemoryFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkPeerMemoryFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkMemoryAllocateFlags(VkMemoryAllocateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkMemoryAllocateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkMemoryAllocateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkMemoryAllocateFlags(const VkMemoryAllocateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkMemoryAllocateFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkMemoryAllocateFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkCommandPoolTrimFlags(VkCommandPoolTrimFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkCommandPoolTrimFlags(const VkCommandPoolTrimFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkDescriptorUpdateTemplateCreateFlags(VkDescriptorUpdateTemplateCreateFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}
static void print_VkDescriptorUpdateTemplateCreateFlags(const VkDescriptorUpdateTemplateCreateFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "\"" << str << "\"" << " : " << obj << "," << std::endl;
     else
         _OUT << "\"" << str << "\"" << " : " << obj << std::endl;
}

static void print_VkExternalMemoryHandleTypeFlags(VkExternalMemoryHandleTypeFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalMemoryHandleTypeFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalMemoryHandleTypeFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkExternalMemoryHandleTypeFlags(const VkExternalMemoryHandleTypeFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalMemoryHandleTypeFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalMemoryHandleTypeFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkExternalMemoryFeatureFlags(VkExternalMemoryFeatureFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalMemoryFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalMemoryFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkExternalMemoryFeatureFlags(const VkExternalMemoryFeatureFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalMemoryFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalMemoryFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkExternalFenceHandleTypeFlags(VkExternalFenceHandleTypeFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalFenceHandleTypeFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalFenceHandleTypeFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkExternalFenceHandleTypeFlags(const VkExternalFenceHandleTypeFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalFenceHandleTypeFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalFenceHandleTypeFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkExternalFenceFeatureFlags(VkExternalFenceFeatureFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalFenceFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalFenceFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkExternalFenceFeatureFlags(const VkExternalFenceFeatureFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalFenceFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalFenceFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkFenceImportFlags(VkFenceImportFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkFenceImportFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkFenceImportFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkFenceImportFlags(const VkFenceImportFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkFenceImportFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkFenceImportFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkSemaphoreImportFlags(VkSemaphoreImportFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSemaphoreImportFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSemaphoreImportFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkSemaphoreImportFlags(const VkSemaphoreImportFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSemaphoreImportFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSemaphoreImportFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkExternalSemaphoreHandleTypeFlags(VkExternalSemaphoreHandleTypeFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalSemaphoreHandleTypeFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalSemaphoreHandleTypeFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkExternalSemaphoreHandleTypeFlags(const VkExternalSemaphoreHandleTypeFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalSemaphoreHandleTypeFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalSemaphoreHandleTypeFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkExternalSemaphoreFeatureFlags(VkExternalSemaphoreFeatureFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalSemaphoreFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalSemaphoreFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkExternalSemaphoreFeatureFlags(const VkExternalSemaphoreFeatureFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkExternalSemaphoreFeatureFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkExternalSemaphoreFeatureFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkPhysicalDeviceSubgroupProperties(VkPhysicalDeviceSubgroupProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.subgroupSize, "subgroupSize", 1);

     print_VkShaderStageFlags(obj.supportedStages, "supportedStages", 1);

     print_VkSubgroupFeatureFlags(obj.supportedOperations, "supportedOperations", 1);

     print_VkBool32(obj.quadOperationsInAllStages, "quadOperationsInAllStages", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceSubgroupProperties(const VkPhysicalDeviceSubgroupProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->subgroupSize, "subgroupSize", 1);

     print_VkShaderStageFlags(obj->supportedStages, "supportedStages", 1);

     print_VkSubgroupFeatureFlags(obj->supportedOperations, "supportedOperations", 1);

     print_VkBool32(obj->quadOperationsInAllStages, "quadOperationsInAllStages", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBindBufferMemoryInfo(VkBindBufferMemoryInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj.memoryOffset, "memoryOffset", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBindBufferMemoryInfo(const VkBindBufferMemoryInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj->memoryOffset, "memoryOffset", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBindImageMemoryInfo(VkBindImageMemoryInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj.memoryOffset, "memoryOffset", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBindImageMemoryInfo(const VkBindImageMemoryInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkDeviceSize(obj->memoryOffset, "memoryOffset", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDevice16BitStorageFeatures(VkPhysicalDevice16BitStorageFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.storageBuffer16BitAccess, "storageBuffer16BitAccess", 1);

     print_VkBool32(obj.uniformAndStorageBuffer16BitAccess, "uniformAndStorageBuffer16BitAccess", 1);

     print_VkBool32(obj.storagePushConstant16, "storagePushConstant16", 1);

     print_VkBool32(obj.storageInputOutput16, "storageInputOutput16", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDevice16BitStorageFeatures(const VkPhysicalDevice16BitStorageFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->storageBuffer16BitAccess, "storageBuffer16BitAccess", 1);

     print_VkBool32(obj->uniformAndStorageBuffer16BitAccess, "uniformAndStorageBuffer16BitAccess", 1);

     print_VkBool32(obj->storagePushConstant16, "storagePushConstant16", 1);

     print_VkBool32(obj->storageInputOutput16, "storageInputOutput16", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkMemoryDedicatedRequirements(VkMemoryDedicatedRequirements obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.prefersDedicatedAllocation, "prefersDedicatedAllocation", 1);

     print_VkBool32(obj.requiresDedicatedAllocation, "requiresDedicatedAllocation", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkMemoryDedicatedRequirements(const VkMemoryDedicatedRequirements * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->prefersDedicatedAllocation, "prefersDedicatedAllocation", 1);

     print_VkBool32(obj->requiresDedicatedAllocation, "requiresDedicatedAllocation", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkMemoryDedicatedAllocateInfo(VkMemoryDedicatedAllocateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkMemoryDedicatedAllocateInfo(const VkMemoryDedicatedAllocateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"," << std::endl;

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkMemoryAllocateFlagsInfo(VkMemoryAllocateFlagsInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkMemoryAllocateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.deviceMask, "deviceMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkMemoryAllocateFlagsInfo(const VkMemoryAllocateFlagsInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkMemoryAllocateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->deviceMask, "deviceMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDeviceGroupRenderPassBeginInfo(VkDeviceGroupRenderPassBeginInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.deviceMask, "deviceMask", 1);

     print_uint32_t(obj.deviceRenderAreaCount, "deviceRenderAreaCount", 1);

     PRINT_SPACE
     _OUT << "\"pDeviceRenderAreas\": " << std::endl;
     if (obj.pDeviceRenderAreas) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.deviceRenderAreaCount; i++) {
           if (i+1 == obj.deviceRenderAreaCount)
               print_VkRect2D(obj.pDeviceRenderAreas[i], "pDeviceRenderAreas", 0);
           else
               print_VkRect2D(obj.pDeviceRenderAreas[i], "pDeviceRenderAreas", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDeviceGroupRenderPassBeginInfo(const VkDeviceGroupRenderPassBeginInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->deviceMask, "deviceMask", 1);

     print_uint32_t(obj->deviceRenderAreaCount, "deviceRenderAreaCount", 1);

     PRINT_SPACE
     _OUT << "\"pDeviceRenderAreas\": " << std::endl;
     if (obj->pDeviceRenderAreas) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->deviceRenderAreaCount; i++) {
           if (i+1 == obj->deviceRenderAreaCount)
               print_VkRect2D(obj->pDeviceRenderAreas[i], "pDeviceRenderAreas", 0);
           else
               print_VkRect2D(obj->pDeviceRenderAreas[i], "pDeviceRenderAreas", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDeviceGroupCommandBufferBeginInfo(VkDeviceGroupCommandBufferBeginInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.deviceMask, "deviceMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDeviceGroupCommandBufferBeginInfo(const VkDeviceGroupCommandBufferBeginInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->deviceMask, "deviceMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDeviceGroupSubmitInfo(VkDeviceGroupSubmitInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.waitSemaphoreCount, "waitSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pWaitSemaphoreDeviceIndices\":" << std::endl;
     PRINT_SPACE
     if (obj.pWaitSemaphoreDeviceIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.waitSemaphoreCount; i++) {
           bool isCommaNeeded = (i+1) != obj.waitSemaphoreCount;
           print_uint32_t(obj.pWaitSemaphoreDeviceIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj.commandBufferCount, "commandBufferCount", 1);

     PRINT_SPACE
     _OUT << "\"pCommandBufferDeviceMasks\":" << std::endl;
     PRINT_SPACE
     if (obj.pCommandBufferDeviceMasks) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.commandBufferCount; i++) {
           bool isCommaNeeded = (i+1) != obj.commandBufferCount;
           print_uint32_t(obj.pCommandBufferDeviceMasks[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj.signalSemaphoreCount, "signalSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pSignalSemaphoreDeviceIndices\":" << std::endl;
     PRINT_SPACE
     if (obj.pSignalSemaphoreDeviceIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.signalSemaphoreCount; i++) {
           bool isCommaNeeded = (i+1) != obj.signalSemaphoreCount;
           print_uint32_t(obj.pSignalSemaphoreDeviceIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDeviceGroupSubmitInfo(const VkDeviceGroupSubmitInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->waitSemaphoreCount, "waitSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pWaitSemaphoreDeviceIndices\":" << std::endl;
     PRINT_SPACE
     if (obj->pWaitSemaphoreDeviceIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->waitSemaphoreCount; i++) {
           bool isCommaNeeded = (i+1) != obj->waitSemaphoreCount;
           print_uint32_t(obj->pWaitSemaphoreDeviceIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj->commandBufferCount, "commandBufferCount", 1);

     PRINT_SPACE
     _OUT << "\"pCommandBufferDeviceMasks\":" << std::endl;
     PRINT_SPACE
     if (obj->pCommandBufferDeviceMasks) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->commandBufferCount; i++) {
           bool isCommaNeeded = (i+1) != obj->commandBufferCount;
           print_uint32_t(obj->pCommandBufferDeviceMasks[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj->signalSemaphoreCount, "signalSemaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pSignalSemaphoreDeviceIndices\":" << std::endl;
     PRINT_SPACE
     if (obj->pSignalSemaphoreDeviceIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->signalSemaphoreCount; i++) {
           bool isCommaNeeded = (i+1) != obj->signalSemaphoreCount;
           print_uint32_t(obj->pSignalSemaphoreDeviceIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDeviceGroupBindSparseInfo(VkDeviceGroupBindSparseInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.resourceDeviceIndex, "resourceDeviceIndex", 1);

     print_uint32_t(obj.memoryDeviceIndex, "memoryDeviceIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDeviceGroupBindSparseInfo(const VkDeviceGroupBindSparseInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->resourceDeviceIndex, "resourceDeviceIndex", 1);

     print_uint32_t(obj->memoryDeviceIndex, "memoryDeviceIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBindBufferMemoryDeviceGroupInfo(VkBindBufferMemoryDeviceGroupInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.deviceIndexCount, "deviceIndexCount", 1);

     PRINT_SPACE
     _OUT << "\"pDeviceIndices\":" << std::endl;
     PRINT_SPACE
     if (obj.pDeviceIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.deviceIndexCount; i++) {
           bool isCommaNeeded = (i+1) != obj.deviceIndexCount;
           print_uint32_t(obj.pDeviceIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBindBufferMemoryDeviceGroupInfo(const VkBindBufferMemoryDeviceGroupInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->deviceIndexCount, "deviceIndexCount", 1);

     PRINT_SPACE
     _OUT << "\"pDeviceIndices\":" << std::endl;
     PRINT_SPACE
     if (obj->pDeviceIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->deviceIndexCount; i++) {
           bool isCommaNeeded = (i+1) != obj->deviceIndexCount;
           print_uint32_t(obj->pDeviceIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBindImageMemoryDeviceGroupInfo(VkBindImageMemoryDeviceGroupInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.deviceIndexCount, "deviceIndexCount", 1);

     PRINT_SPACE
     _OUT << "\"pDeviceIndices\":" << std::endl;
     PRINT_SPACE
     if (obj.pDeviceIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.deviceIndexCount; i++) {
           bool isCommaNeeded = (i+1) != obj.deviceIndexCount;
           print_uint32_t(obj.pDeviceIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj.splitInstanceBindRegionCount, "splitInstanceBindRegionCount", 1);

     PRINT_SPACE
     _OUT << "\"pSplitInstanceBindRegions\": " << std::endl;
     if (obj.pSplitInstanceBindRegions) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.splitInstanceBindRegionCount; i++) {
           if (i+1 == obj.splitInstanceBindRegionCount)
               print_VkRect2D(obj.pSplitInstanceBindRegions[i], "pSplitInstanceBindRegions", 0);
           else
               print_VkRect2D(obj.pSplitInstanceBindRegions[i], "pSplitInstanceBindRegions", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBindImageMemoryDeviceGroupInfo(const VkBindImageMemoryDeviceGroupInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->deviceIndexCount, "deviceIndexCount", 1);

     PRINT_SPACE
     _OUT << "\"pDeviceIndices\":" << std::endl;
     PRINT_SPACE
     if (obj->pDeviceIndices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->deviceIndexCount; i++) {
           bool isCommaNeeded = (i+1) != obj->deviceIndexCount;
           print_uint32_t(obj->pDeviceIndices[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj->splitInstanceBindRegionCount, "splitInstanceBindRegionCount", 1);

     PRINT_SPACE
     _OUT << "\"pSplitInstanceBindRegions\": " << std::endl;
     if (obj->pSplitInstanceBindRegions) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->splitInstanceBindRegionCount; i++) {
           if (i+1 == obj->splitInstanceBindRegionCount)
               print_VkRect2D(obj->pSplitInstanceBindRegions[i], "pSplitInstanceBindRegions", 0);
           else
               print_VkRect2D(obj->pSplitInstanceBindRegions[i], "pSplitInstanceBindRegions", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceGroupProperties(VkPhysicalDeviceGroupProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.physicalDeviceCount, "physicalDeviceCount", 1);

     PRINT_SPACE
     _OUT << "\"physicalDevices\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DEVICE_GROUP_SIZE; i++) {
           std:: stringstream tmp;
           tmp << "physicalDevices" << "_" << i;
           bool isCommaNeeded = (i+1) != VK_MAX_DEVICE_GROUP_SIZE;
           print_VkPhysicalDevice(obj.physicalDevices[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_VkBool32(obj.subsetAllocation, "subsetAllocation", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceGroupProperties(const VkPhysicalDeviceGroupProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->physicalDeviceCount, "physicalDeviceCount", 1);

     PRINT_SPACE
     _OUT << "\"physicalDevices\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DEVICE_GROUP_SIZE; i++) {
           std:: stringstream tmp;
           tmp << "physicalDevices" << "_" << i;
           bool isCommaNeeded = (i+1) != VK_MAX_DEVICE_GROUP_SIZE;
           print_VkPhysicalDevice(obj->physicalDevices[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_VkBool32(obj->subsetAllocation, "subsetAllocation", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDeviceGroupDeviceCreateInfo(VkDeviceGroupDeviceCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.physicalDeviceCount, "physicalDeviceCount", 1);

     PRINT_SPACE
     _OUT << "\"pPhysicalDevices\":" << std::endl;
     PRINT_SPACE
     if (obj.pPhysicalDevices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.physicalDeviceCount; i++) {
           std:: stringstream tmp;
           tmp << "pPhysicalDevices" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.physicalDeviceCount;
           print_VkPhysicalDevice(obj.pPhysicalDevices[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDeviceGroupDeviceCreateInfo(const VkDeviceGroupDeviceCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->physicalDeviceCount, "physicalDeviceCount", 1);

     PRINT_SPACE
     _OUT << "\"pPhysicalDevices\":" << std::endl;
     PRINT_SPACE
     if (obj->pPhysicalDevices) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->physicalDeviceCount; i++) {
           std:: stringstream tmp;
           tmp << "pPhysicalDevices" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->physicalDeviceCount;
           print_VkPhysicalDevice(obj->pPhysicalDevices[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBufferMemoryRequirementsInfo2(VkBufferMemoryRequirementsInfo2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBufferMemoryRequirementsInfo2(const VkBufferMemoryRequirementsInfo2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageMemoryRequirementsInfo2(VkImageMemoryRequirementsInfo2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageMemoryRequirementsInfo2(const VkImageMemoryRequirementsInfo2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageSparseMemoryRequirementsInfo2(VkImageSparseMemoryRequirementsInfo2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageSparseMemoryRequirementsInfo2(const VkImageSparseMemoryRequirementsInfo2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "image" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkMemoryRequirements2(VkMemoryRequirements2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"memoryRequirements\": " << std::endl;
     {
           print_VkMemoryRequirements(obj.memoryRequirements, "memoryRequirements", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkMemoryRequirements2(const VkMemoryRequirements2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"memoryRequirements\": " << std::endl;
     {
           print_VkMemoryRequirements(obj->memoryRequirements, "memoryRequirements", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSparseImageMemoryRequirements2(VkSparseImageMemoryRequirements2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"memoryRequirements\": " << std::endl;
     {
           print_VkSparseImageMemoryRequirements(obj.memoryRequirements, "memoryRequirements", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSparseImageMemoryRequirements2(const VkSparseImageMemoryRequirements2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"memoryRequirements\": " << std::endl;
     {
           print_VkSparseImageMemoryRequirements(obj->memoryRequirements, "memoryRequirements", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceFeatures2(VkPhysicalDeviceFeatures2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"features\": " << std::endl;
     {
           print_VkPhysicalDeviceFeatures(obj.features, "features", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceFeatures2(const VkPhysicalDeviceFeatures2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"features\": " << std::endl;
     {
           print_VkPhysicalDeviceFeatures(obj->features, "features", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceProperties2(VkPhysicalDeviceProperties2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"properties\": " << std::endl;
     {
           print_VkPhysicalDeviceProperties(obj.properties, "properties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceProperties2(const VkPhysicalDeviceProperties2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"properties\": " << std::endl;
     {
           print_VkPhysicalDeviceProperties(obj->properties, "properties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkFormatProperties2(VkFormatProperties2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"formatProperties\": " << std::endl;
     {
           print_VkFormatProperties(obj.formatProperties, "formatProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkFormatProperties2(const VkFormatProperties2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"formatProperties\": " << std::endl;
     {
           print_VkFormatProperties(obj->formatProperties, "formatProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageFormatProperties2(VkImageFormatProperties2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"imageFormatProperties\": " << std::endl;
     {
           print_VkImageFormatProperties(obj.imageFormatProperties, "imageFormatProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageFormatProperties2(const VkImageFormatProperties2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"imageFormatProperties\": " << std::endl;
     {
           print_VkImageFormatProperties(obj->imageFormatProperties, "imageFormatProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceImageFormatInfo2(VkPhysicalDeviceImageFormatInfo2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkFormat(obj.format, "format", 1);

     print_VkImageType(obj.type, "type", 1);

     print_VkImageTiling(obj.tiling, "tiling", 1);

     print_VkImageUsageFlags(obj.usage, "usage", 1);

     print_VkImageCreateFlags(obj.flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceImageFormatInfo2(const VkPhysicalDeviceImageFormatInfo2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkFormat(obj->format, "format", 1);

     print_VkImageType(obj->type, "type", 1);

     print_VkImageTiling(obj->tiling, "tiling", 1);

     print_VkImageUsageFlags(obj->usage, "usage", 1);

     print_VkImageCreateFlags(obj->flags, "flags", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkQueueFamilyProperties2(VkQueueFamilyProperties2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"queueFamilyProperties\": " << std::endl;
     {
           print_VkQueueFamilyProperties(obj.queueFamilyProperties, "queueFamilyProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkQueueFamilyProperties2(const VkQueueFamilyProperties2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"queueFamilyProperties\": " << std::endl;
     {
           print_VkQueueFamilyProperties(obj->queueFamilyProperties, "queueFamilyProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceMemoryProperties2(VkPhysicalDeviceMemoryProperties2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"memoryProperties\": " << std::endl;
     {
           print_VkPhysicalDeviceMemoryProperties(obj.memoryProperties, "memoryProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceMemoryProperties2(const VkPhysicalDeviceMemoryProperties2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"memoryProperties\": " << std::endl;
     {
           print_VkPhysicalDeviceMemoryProperties(obj->memoryProperties, "memoryProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSparseImageFormatProperties2(VkSparseImageFormatProperties2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"properties\": " << std::endl;
     {
           print_VkSparseImageFormatProperties(obj.properties, "properties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSparseImageFormatProperties2(const VkSparseImageFormatProperties2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"properties\": " << std::endl;
     {
           print_VkSparseImageFormatProperties(obj->properties, "properties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceSparseImageFormatInfo2(VkPhysicalDeviceSparseImageFormatInfo2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkFormat(obj.format, "format", 1);

     print_VkImageType(obj.type, "type", 1);

     print_VkSampleCountFlagBits(obj.samples, "samples", 1);

     print_VkImageUsageFlags(obj.usage, "usage", 1);

     print_VkImageTiling(obj.tiling, "tiling", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceSparseImageFormatInfo2(const VkPhysicalDeviceSparseImageFormatInfo2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkFormat(obj->format, "format", 1);

     print_VkImageType(obj->type, "type", 1);

     print_VkSampleCountFlagBits(obj->samples, "samples", 1);

     print_VkImageUsageFlags(obj->usage, "usage", 1);

     print_VkImageTiling(obj->tiling, "tiling", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDevicePointClippingProperties(VkPhysicalDevicePointClippingProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPointClippingBehavior(obj.pointClippingBehavior, "pointClippingBehavior", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDevicePointClippingProperties(const VkPhysicalDevicePointClippingProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkPointClippingBehavior(obj->pointClippingBehavior, "pointClippingBehavior", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkInputAttachmentAspectReference(VkInputAttachmentAspectReference obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.subpass, "subpass", 1);

     print_uint32_t(obj.inputAttachmentIndex, "inputAttachmentIndex", 1);

     print_VkImageAspectFlags(obj.aspectMask, "aspectMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkInputAttachmentAspectReference(const VkInputAttachmentAspectReference * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->subpass, "subpass", 1);

     print_uint32_t(obj->inputAttachmentIndex, "inputAttachmentIndex", 1);

     print_VkImageAspectFlags(obj->aspectMask, "aspectMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkRenderPassInputAttachmentAspectCreateInfo(VkRenderPassInputAttachmentAspectCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.aspectReferenceCount, "aspectReferenceCount", 1);

     PRINT_SPACE
     _OUT << "\"pAspectReferences\": " << std::endl;
     if (obj.pAspectReferences) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.aspectReferenceCount; i++) {
           if (i+1 == obj.aspectReferenceCount)
               print_VkInputAttachmentAspectReference(obj.pAspectReferences[i], "pAspectReferences", 0);
           else
               print_VkInputAttachmentAspectReference(obj.pAspectReferences[i], "pAspectReferences", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkRenderPassInputAttachmentAspectCreateInfo(const VkRenderPassInputAttachmentAspectCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->aspectReferenceCount, "aspectReferenceCount", 1);

     PRINT_SPACE
     _OUT << "\"pAspectReferences\": " << std::endl;
     if (obj->pAspectReferences) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->aspectReferenceCount; i++) {
           if (i+1 == obj->aspectReferenceCount)
               print_VkInputAttachmentAspectReference(obj->pAspectReferences[i], "pAspectReferences", 0);
           else
               print_VkInputAttachmentAspectReference(obj->pAspectReferences[i], "pAspectReferences", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageViewUsageCreateInfo(VkImageViewUsageCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageUsageFlags(obj.usage, "usage", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageViewUsageCreateInfo(const VkImageViewUsageCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageUsageFlags(obj->usage, "usage", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPipelineTessellationDomainOriginStateCreateInfo(VkPipelineTessellationDomainOriginStateCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkTessellationDomainOrigin(obj.domainOrigin, "domainOrigin", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPipelineTessellationDomainOriginStateCreateInfo(const VkPipelineTessellationDomainOriginStateCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkTessellationDomainOrigin(obj->domainOrigin, "domainOrigin", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkRenderPassMultiviewCreateInfo(VkRenderPassMultiviewCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.subpassCount, "subpassCount", 1);

     PRINT_SPACE
     _OUT << "\"pViewMasks\":" << std::endl;
     PRINT_SPACE
     if (obj.pViewMasks) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.subpassCount; i++) {
           bool isCommaNeeded = (i+1) != obj.subpassCount;
           print_uint32_t(obj.pViewMasks[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj.dependencyCount, "dependencyCount", 1);

     PRINT_SPACE
     _OUT << "\"pViewOffsets\":" << std::endl;
     PRINT_SPACE
     if (obj.pViewOffsets) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.dependencyCount; i++) {
           bool isCommaNeeded = (i+1) != obj.dependencyCount;
           print_int32_t(obj.pViewOffsets[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj.correlationMaskCount, "correlationMaskCount", 1);

     PRINT_SPACE
     _OUT << "\"pCorrelationMasks\":" << std::endl;
     PRINT_SPACE
     if (obj.pCorrelationMasks) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.correlationMaskCount; i++) {
           bool isCommaNeeded = (i+1) != obj.correlationMaskCount;
           print_uint32_t(obj.pCorrelationMasks[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkRenderPassMultiviewCreateInfo(const VkRenderPassMultiviewCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->subpassCount, "subpassCount", 1);

     PRINT_SPACE
     _OUT << "\"pViewMasks\":" << std::endl;
     PRINT_SPACE
     if (obj->pViewMasks) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->subpassCount; i++) {
           bool isCommaNeeded = (i+1) != obj->subpassCount;
           print_uint32_t(obj->pViewMasks[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj->dependencyCount, "dependencyCount", 1);

     PRINT_SPACE
     _OUT << "\"pViewOffsets\":" << std::endl;
     PRINT_SPACE
     if (obj->pViewOffsets) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->dependencyCount; i++) {
           bool isCommaNeeded = (i+1) != obj->dependencyCount;
           print_int32_t(obj->pViewOffsets[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj->correlationMaskCount, "correlationMaskCount", 1);

     PRINT_SPACE
     _OUT << "\"pCorrelationMasks\":" << std::endl;
     PRINT_SPACE
     if (obj->pCorrelationMasks) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->correlationMaskCount; i++) {
           bool isCommaNeeded = (i+1) != obj->correlationMaskCount;
           print_uint32_t(obj->pCorrelationMasks[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceMultiviewFeatures(VkPhysicalDeviceMultiviewFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.multiview, "multiview", 1);

     print_VkBool32(obj.multiviewGeometryShader, "multiviewGeometryShader", 1);

     print_VkBool32(obj.multiviewTessellationShader, "multiviewTessellationShader", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceMultiviewFeatures(const VkPhysicalDeviceMultiviewFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->multiview, "multiview", 1);

     print_VkBool32(obj->multiviewGeometryShader, "multiviewGeometryShader", 1);

     print_VkBool32(obj->multiviewTessellationShader, "multiviewTessellationShader", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceMultiviewProperties(VkPhysicalDeviceMultiviewProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.maxMultiviewViewCount, "maxMultiviewViewCount", 1);

     print_uint32_t(obj.maxMultiviewInstanceIndex, "maxMultiviewInstanceIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceMultiviewProperties(const VkPhysicalDeviceMultiviewProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->maxMultiviewViewCount, "maxMultiviewViewCount", 1);

     print_uint32_t(obj->maxMultiviewInstanceIndex, "maxMultiviewInstanceIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceVariablePointersFeatures(VkPhysicalDeviceVariablePointersFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.variablePointersStorageBuffer, "variablePointersStorageBuffer", 1);

     print_VkBool32(obj.variablePointers, "variablePointers", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceVariablePointersFeatures(const VkPhysicalDeviceVariablePointersFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->variablePointersStorageBuffer, "variablePointersStorageBuffer", 1);

     print_VkBool32(obj->variablePointers, "variablePointers", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

typedef VkPhysicalDeviceVariablePointersFeatures VkPhysicalDeviceVariablePointerFeatures;

static void print_VkPhysicalDeviceProtectedMemoryFeatures(VkPhysicalDeviceProtectedMemoryFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.protectedMemory, "protectedMemory", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceProtectedMemoryFeatures(const VkPhysicalDeviceProtectedMemoryFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->protectedMemory, "protectedMemory", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceProtectedMemoryProperties(VkPhysicalDeviceProtectedMemoryProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.protectedNoFault, "protectedNoFault", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceProtectedMemoryProperties(const VkPhysicalDeviceProtectedMemoryProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->protectedNoFault, "protectedNoFault", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDeviceQueueInfo2(VkDeviceQueueInfo2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDeviceQueueCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.queueFamilyIndex, "queueFamilyIndex", 1);

     print_uint32_t(obj.queueIndex, "queueIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDeviceQueueInfo2(const VkDeviceQueueInfo2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDeviceQueueCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->queueFamilyIndex, "queueFamilyIndex", 1);

     print_uint32_t(obj->queueIndex, "queueIndex", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkProtectedSubmitInfo(VkProtectedSubmitInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.protectedSubmit, "protectedSubmit", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkProtectedSubmitInfo(const VkProtectedSubmitInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->protectedSubmit, "protectedSubmit", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSamplerYcbcrConversionCreateInfo(VkSamplerYcbcrConversionCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkFormat(obj.format, "format", 1);

     print_VkSamplerYcbcrModelConversion(obj.ycbcrModel, "ycbcrModel", 1);

     print_VkSamplerYcbcrRange(obj.ycbcrRange, "ycbcrRange", 1);

     PRINT_SPACE
     _OUT << "\"components\": " << std::endl;
     {
           print_VkComponentMapping(obj.components, "components", 1);
     }

     print_VkChromaLocation(obj.xChromaOffset, "xChromaOffset", 1);

     print_VkChromaLocation(obj.yChromaOffset, "yChromaOffset", 1);

     print_VkFilter(obj.chromaFilter, "chromaFilter", 1);

     print_VkBool32(obj.forceExplicitReconstruction, "forceExplicitReconstruction", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSamplerYcbcrConversionCreateInfo(const VkSamplerYcbcrConversionCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkFormat(obj->format, "format", 1);

     print_VkSamplerYcbcrModelConversion(obj->ycbcrModel, "ycbcrModel", 1);

     print_VkSamplerYcbcrRange(obj->ycbcrRange, "ycbcrRange", 1);

     PRINT_SPACE
     _OUT << "\"components\": " << std::endl;
     {
           print_VkComponentMapping(obj->components, "components", 1);
     }

     print_VkChromaLocation(obj->xChromaOffset, "xChromaOffset", 1);

     print_VkChromaLocation(obj->yChromaOffset, "yChromaOffset", 1);

     print_VkFilter(obj->chromaFilter, "chromaFilter", 1);

     print_VkBool32(obj->forceExplicitReconstruction, "forceExplicitReconstruction", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSamplerYcbcrConversionInfo(VkSamplerYcbcrConversionInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "conversion" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSamplerYcbcrConversionInfo(const VkSamplerYcbcrConversionInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "conversion" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBindImagePlaneMemoryInfo(VkBindImagePlaneMemoryInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageAspectFlagBits(obj.planeAspect, "planeAspect", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBindImagePlaneMemoryInfo(const VkBindImagePlaneMemoryInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageAspectFlagBits(obj->planeAspect, "planeAspect", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImagePlaneMemoryRequirementsInfo(VkImagePlaneMemoryRequirementsInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageAspectFlagBits(obj.planeAspect, "planeAspect", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImagePlaneMemoryRequirementsInfo(const VkImagePlaneMemoryRequirementsInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageAspectFlagBits(obj->planeAspect, "planeAspect", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceSamplerYcbcrConversionFeatures(VkPhysicalDeviceSamplerYcbcrConversionFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.samplerYcbcrConversion, "samplerYcbcrConversion", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceSamplerYcbcrConversionFeatures(const VkPhysicalDeviceSamplerYcbcrConversionFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->samplerYcbcrConversion, "samplerYcbcrConversion", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSamplerYcbcrConversionImageFormatProperties(VkSamplerYcbcrConversionImageFormatProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.combinedImageSamplerDescriptorCount, "combinedImageSamplerDescriptorCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSamplerYcbcrConversionImageFormatProperties(const VkSamplerYcbcrConversionImageFormatProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->combinedImageSamplerDescriptorCount, "combinedImageSamplerDescriptorCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorUpdateTemplateEntry(VkDescriptorUpdateTemplateEntry obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj.dstBinding, "dstBinding", 1);

     print_uint32_t(obj.dstArrayElement, "dstArrayElement", 1);

     print_uint32_t(obj.descriptorCount, "descriptorCount", 1);

     print_VkDescriptorType(obj.descriptorType, "descriptorType", 1);

     print_size_t(obj.offset, "offset", 1);

     print_size_t(obj.stride, "stride", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorUpdateTemplateEntry(const VkDescriptorUpdateTemplateEntry * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint32_t(obj->dstBinding, "dstBinding", 1);

     print_uint32_t(obj->dstArrayElement, "dstArrayElement", 1);

     print_uint32_t(obj->descriptorCount, "descriptorCount", 1);

     print_VkDescriptorType(obj->descriptorType, "descriptorType", 1);

     print_size_t(obj->offset, "offset", 1);

     print_size_t(obj->stride, "stride", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorUpdateTemplateCreateInfo(VkDescriptorUpdateTemplateCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDescriptorUpdateTemplateCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.descriptorUpdateEntryCount, "descriptorUpdateEntryCount", 1);

     PRINT_SPACE
     _OUT << "\"pDescriptorUpdateEntries\": " << std::endl;
     if (obj.pDescriptorUpdateEntries) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.descriptorUpdateEntryCount; i++) {
           if (i+1 == obj.descriptorUpdateEntryCount)
               print_VkDescriptorUpdateTemplateEntry(obj.pDescriptorUpdateEntries[i], "pDescriptorUpdateEntries", 0);
           else
               print_VkDescriptorUpdateTemplateEntry(obj.pDescriptorUpdateEntries[i], "pDescriptorUpdateEntries", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDescriptorUpdateTemplateType(obj.templateType, "templateType", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "descriptorSetLayout" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkPipelineBindPoint(obj.pipelineBindPoint, "pipelineBindPoint", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "pipelineLayout" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj.set, "set", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorUpdateTemplateCreateInfo(const VkDescriptorUpdateTemplateCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDescriptorUpdateTemplateCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->descriptorUpdateEntryCount, "descriptorUpdateEntryCount", 1);

     PRINT_SPACE
     _OUT << "\"pDescriptorUpdateEntries\": " << std::endl;
     if (obj->pDescriptorUpdateEntries) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->descriptorUpdateEntryCount; i++) {
           if (i+1 == obj->descriptorUpdateEntryCount)
               print_VkDescriptorUpdateTemplateEntry(obj->pDescriptorUpdateEntries[i], "pDescriptorUpdateEntries", 0);
           else
               print_VkDescriptorUpdateTemplateEntry(obj->pDescriptorUpdateEntries[i], "pDescriptorUpdateEntries", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDescriptorUpdateTemplateType(obj->templateType, "templateType", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "descriptorSetLayout" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_VkPipelineBindPoint(obj->pipelineBindPoint, "pipelineBindPoint", 1);

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "pipelineLayout" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint32_t(obj->set, "set", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExternalMemoryProperties(VkExternalMemoryProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkExternalMemoryFeatureFlags(obj.externalMemoryFeatures, "externalMemoryFeatures", 1);

     print_VkExternalMemoryHandleTypeFlags(obj.exportFromImportedHandleTypes, "exportFromImportedHandleTypes", 1);

     print_VkExternalMemoryHandleTypeFlags(obj.compatibleHandleTypes, "compatibleHandleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExternalMemoryProperties(const VkExternalMemoryProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkExternalMemoryFeatureFlags(obj->externalMemoryFeatures, "externalMemoryFeatures", 1);

     print_VkExternalMemoryHandleTypeFlags(obj->exportFromImportedHandleTypes, "exportFromImportedHandleTypes", 1);

     print_VkExternalMemoryHandleTypeFlags(obj->compatibleHandleTypes, "compatibleHandleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceExternalImageFormatInfo(VkPhysicalDeviceExternalImageFormatInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalMemoryHandleTypeFlagBits(obj.handleType, "handleType", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceExternalImageFormatInfo(const VkPhysicalDeviceExternalImageFormatInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalMemoryHandleTypeFlagBits(obj->handleType, "handleType", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExternalImageFormatProperties(VkExternalImageFormatProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"externalMemoryProperties\": " << std::endl;
     {
           print_VkExternalMemoryProperties(obj.externalMemoryProperties, "externalMemoryProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExternalImageFormatProperties(const VkExternalImageFormatProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"externalMemoryProperties\": " << std::endl;
     {
           print_VkExternalMemoryProperties(obj->externalMemoryProperties, "externalMemoryProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceExternalBufferInfo(VkPhysicalDeviceExternalBufferInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBufferCreateFlags(obj.flags, "flags", 1);

     print_VkBufferUsageFlags(obj.usage, "usage", 1);

     print_VkExternalMemoryHandleTypeFlagBits(obj.handleType, "handleType", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceExternalBufferInfo(const VkPhysicalDeviceExternalBufferInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBufferCreateFlags(obj->flags, "flags", 1);

     print_VkBufferUsageFlags(obj->usage, "usage", 1);

     print_VkExternalMemoryHandleTypeFlagBits(obj->handleType, "handleType", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExternalBufferProperties(VkExternalBufferProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"externalMemoryProperties\": " << std::endl;
     {
           print_VkExternalMemoryProperties(obj.externalMemoryProperties, "externalMemoryProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExternalBufferProperties(const VkExternalBufferProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"externalMemoryProperties\": " << std::endl;
     {
           print_VkExternalMemoryProperties(obj->externalMemoryProperties, "externalMemoryProperties", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceIDProperties(VkPhysicalDeviceIDProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"deviceUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj.deviceUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"driverUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj.driverUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"deviceLUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_LUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_LUID_SIZE;
           print_uint8_t(obj.deviceLUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj.deviceNodeMask, "deviceNodeMask", 1);

     print_VkBool32(obj.deviceLUIDValid, "deviceLUIDValid", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceIDProperties(const VkPhysicalDeviceIDProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"deviceUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj->deviceUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"driverUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj->driverUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"deviceLUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_LUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_LUID_SIZE;
           print_uint8_t(obj->deviceLUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj->deviceNodeMask, "deviceNodeMask", 1);

     print_VkBool32(obj->deviceLUIDValid, "deviceLUIDValid", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExternalMemoryImageCreateInfo(VkExternalMemoryImageCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalMemoryHandleTypeFlags(obj.handleTypes, "handleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExternalMemoryImageCreateInfo(const VkExternalMemoryImageCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalMemoryHandleTypeFlags(obj->handleTypes, "handleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExternalMemoryBufferCreateInfo(VkExternalMemoryBufferCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalMemoryHandleTypeFlags(obj.handleTypes, "handleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExternalMemoryBufferCreateInfo(const VkExternalMemoryBufferCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalMemoryHandleTypeFlags(obj->handleTypes, "handleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExportMemoryAllocateInfo(VkExportMemoryAllocateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalMemoryHandleTypeFlags(obj.handleTypes, "handleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExportMemoryAllocateInfo(const VkExportMemoryAllocateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalMemoryHandleTypeFlags(obj->handleTypes, "handleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceExternalFenceInfo(VkPhysicalDeviceExternalFenceInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalFenceHandleTypeFlagBits(obj.handleType, "handleType", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceExternalFenceInfo(const VkPhysicalDeviceExternalFenceInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalFenceHandleTypeFlagBits(obj->handleType, "handleType", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExternalFenceProperties(VkExternalFenceProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalFenceHandleTypeFlags(obj.exportFromImportedHandleTypes, "exportFromImportedHandleTypes", 1);

     print_VkExternalFenceHandleTypeFlags(obj.compatibleHandleTypes, "compatibleHandleTypes", 1);

     print_VkExternalFenceFeatureFlags(obj.externalFenceFeatures, "externalFenceFeatures", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExternalFenceProperties(const VkExternalFenceProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalFenceHandleTypeFlags(obj->exportFromImportedHandleTypes, "exportFromImportedHandleTypes", 1);

     print_VkExternalFenceHandleTypeFlags(obj->compatibleHandleTypes, "compatibleHandleTypes", 1);

     print_VkExternalFenceFeatureFlags(obj->externalFenceFeatures, "externalFenceFeatures", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExportFenceCreateInfo(VkExportFenceCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalFenceHandleTypeFlags(obj.handleTypes, "handleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExportFenceCreateInfo(const VkExportFenceCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalFenceHandleTypeFlags(obj->handleTypes, "handleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExportSemaphoreCreateInfo(VkExportSemaphoreCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalSemaphoreHandleTypeFlags(obj.handleTypes, "handleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExportSemaphoreCreateInfo(const VkExportSemaphoreCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalSemaphoreHandleTypeFlags(obj->handleTypes, "handleTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceExternalSemaphoreInfo(VkPhysicalDeviceExternalSemaphoreInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalSemaphoreHandleTypeFlagBits(obj.handleType, "handleType", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceExternalSemaphoreInfo(const VkPhysicalDeviceExternalSemaphoreInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalSemaphoreHandleTypeFlagBits(obj->handleType, "handleType", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkExternalSemaphoreProperties(VkExternalSemaphoreProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalSemaphoreHandleTypeFlags(obj.exportFromImportedHandleTypes, "exportFromImportedHandleTypes", 1);

     print_VkExternalSemaphoreHandleTypeFlags(obj.compatibleHandleTypes, "compatibleHandleTypes", 1);

     print_VkExternalSemaphoreFeatureFlags(obj.externalSemaphoreFeatures, "externalSemaphoreFeatures", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkExternalSemaphoreProperties(const VkExternalSemaphoreProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkExternalSemaphoreHandleTypeFlags(obj->exportFromImportedHandleTypes, "exportFromImportedHandleTypes", 1);

     print_VkExternalSemaphoreHandleTypeFlags(obj->compatibleHandleTypes, "compatibleHandleTypes", 1);

     print_VkExternalSemaphoreFeatureFlags(obj->externalSemaphoreFeatures, "externalSemaphoreFeatures", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceMaintenance3Properties(VkPhysicalDeviceMaintenance3Properties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.maxPerSetDescriptors, "maxPerSetDescriptors", 1);

     print_VkDeviceSize(obj.maxMemoryAllocationSize, "maxMemoryAllocationSize", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceMaintenance3Properties(const VkPhysicalDeviceMaintenance3Properties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->maxPerSetDescriptors, "maxPerSetDescriptors", 1);

     print_VkDeviceSize(obj->maxMemoryAllocationSize, "maxMemoryAllocationSize", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorSetLayoutSupport(VkDescriptorSetLayoutSupport obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.supported, "supported", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorSetLayoutSupport(const VkDescriptorSetLayoutSupport * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->supported, "supported", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceShaderDrawParametersFeatures(VkPhysicalDeviceShaderDrawParametersFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.shaderDrawParameters, "shaderDrawParameters", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceShaderDrawParametersFeatures(const VkPhysicalDeviceShaderDrawParametersFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->shaderDrawParameters, "shaderDrawParameters", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

typedef VkPhysicalDeviceShaderDrawParametersFeatures VkPhysicalDeviceShaderDrawParameterFeatures;

static std::map<uint64_t, std::string> VkDriverId_map = {
    std::make_pair(1, "VK_DRIVER_ID_AMD_PROPRIETARY"),
    std::make_pair(2, "VK_DRIVER_ID_AMD_OPEN_SOURCE"),
    std::make_pair(3, "VK_DRIVER_ID_MESA_RADV"),
    std::make_pair(4, "VK_DRIVER_ID_NVIDIA_PROPRIETARY"),
    std::make_pair(5, "VK_DRIVER_ID_INTEL_PROPRIETARY_WINDOWS"),
    std::make_pair(6, "VK_DRIVER_ID_INTEL_OPEN_SOURCE_MESA"),
    std::make_pair(7, "VK_DRIVER_ID_IMAGINATION_PROPRIETARY"),
    std::make_pair(8, "VK_DRIVER_ID_QUALCOMM_PROPRIETARY"),
    std::make_pair(9, "VK_DRIVER_ID_ARM_PROPRIETARY"),
    std::make_pair(10, "VK_DRIVER_ID_GOOGLE_SWIFTSHADER"),
    std::make_pair(11, "VK_DRIVER_ID_GGP_PROPRIETARY"),
    std::make_pair(12, "VK_DRIVER_ID_BROADCOM_PROPRIETARY"),
    std::make_pair(13, "VK_DRIVER_ID_MESA_LLVMPIPE"),
    std::make_pair(14, "VK_DRIVER_ID_MOLTENVK"),
    std::make_pair(15, "VK_DRIVER_ID_COREAVI_PROPRIETARY"),
    std::make_pair(16, "VK_DRIVER_ID_JUICE_PROPRIETARY"),
    std::make_pair(17, "VK_DRIVER_ID_VERISILICON_PROPRIETARY"),
    std::make_pair(18, "VK_DRIVER_ID_MESA_TURNIP"),
    std::make_pair(19, "VK_DRIVER_ID_MESA_V3DV"),
    std::make_pair(20, "VK_DRIVER_ID_MESA_PANVK"),
    std::make_pair(21, "VK_DRIVER_ID_SAMSUNG_PROPRIETARY"),
};
static void print_VkDriverId(VkDriverId obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDriverId_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDriverId_map[obj] << "\"" << std::endl;
}
static void print_VkDriverId(const VkDriverId * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDriverId_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDriverId_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkShaderFloatControlsIndependence_map = {
    std::make_pair(0, "VK_SHADER_FLOAT_CONTROLS_INDEPENDENCE_32_BIT_ONLY"),
    std::make_pair(1, "VK_SHADER_FLOAT_CONTROLS_INDEPENDENCE_ALL"),
    std::make_pair(2, "VK_SHADER_FLOAT_CONTROLS_INDEPENDENCE_NONE"),
};
static void print_VkShaderFloatControlsIndependence(VkShaderFloatControlsIndependence obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkShaderFloatControlsIndependence_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkShaderFloatControlsIndependence_map[obj] << "\"" << std::endl;
}
static void print_VkShaderFloatControlsIndependence(const VkShaderFloatControlsIndependence * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkShaderFloatControlsIndependence_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkShaderFloatControlsIndependence_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkResolveModeFlagBits_map = {
    std::make_pair(0, "VK_RESOLVE_MODE_NONE"),
    std::make_pair(1ULL << 0, "VK_RESOLVE_MODE_SAMPLE_ZERO_BIT"),
    std::make_pair(1ULL << 1, "VK_RESOLVE_MODE_AVERAGE_BIT"),
    std::make_pair(1ULL << 2, "VK_RESOLVE_MODE_MIN_BIT"),
    std::make_pair(1ULL << 3, "VK_RESOLVE_MODE_MAX_BIT"),
};
static void print_VkResolveModeFlagBits(VkResolveModeFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkResolveModeFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkResolveModeFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkResolveModeFlagBits(const VkResolveModeFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkResolveModeFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkResolveModeFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkDescriptorBindingFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT"),
    std::make_pair(1ULL << 1, "VK_DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT"),
    std::make_pair(1ULL << 2, "VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT"),
    std::make_pair(1ULL << 3, "VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT"),
    std::make_pair(1ULL << 4, "VK_DESCRIPTOR_BINDING_RESERVED_4_BIT_QCOM"),
};
static void print_VkDescriptorBindingFlagBits(VkDescriptorBindingFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDescriptorBindingFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDescriptorBindingFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkDescriptorBindingFlagBits(const VkDescriptorBindingFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkDescriptorBindingFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkDescriptorBindingFlagBits_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSamplerReductionMode_map = {
    std::make_pair(0, "VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE"),
    std::make_pair(1, "VK_SAMPLER_REDUCTION_MODE_MIN"),
    std::make_pair(2, "VK_SAMPLER_REDUCTION_MODE_MAX"),
};
static void print_VkSamplerReductionMode(VkSamplerReductionMode obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerReductionMode_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerReductionMode_map[obj] << "\"" << std::endl;
}
static void print_VkSamplerReductionMode(const VkSamplerReductionMode * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSamplerReductionMode_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSamplerReductionMode_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSemaphoreType_map = {
    std::make_pair(0, "VK_SEMAPHORE_TYPE_BINARY"),
    std::make_pair(1, "VK_SEMAPHORE_TYPE_TIMELINE"),
};
static void print_VkSemaphoreType(VkSemaphoreType obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSemaphoreType_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSemaphoreType_map[obj] << "\"" << std::endl;
}
static void print_VkSemaphoreType(const VkSemaphoreType * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSemaphoreType_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSemaphoreType_map[*obj] << "\"" << std::endl;
}

static std::map<uint64_t, std::string> VkSemaphoreWaitFlagBits_map = {
    std::make_pair(1ULL << 0, "VK_SEMAPHORE_WAIT_ANY_BIT"),
};
static void print_VkSemaphoreWaitFlagBits(VkSemaphoreWaitFlagBits obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSemaphoreWaitFlagBits_map[obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSemaphoreWaitFlagBits_map[obj] << "\"" << std::endl;
}
static void print_VkSemaphoreWaitFlagBits(const VkSemaphoreWaitFlagBits * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     if (commaNeeded)
         _OUT << "\"" <<  VkSemaphoreWaitFlagBits_map[*obj] << "\"," << std::endl;
     else
         _OUT << "\"" << VkSemaphoreWaitFlagBits_map[*obj] << "\"" << std::endl;
}

static void print_VkResolveModeFlags(VkResolveModeFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkResolveModeFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkResolveModeFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkResolveModeFlags(const VkResolveModeFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkResolveModeFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkResolveModeFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkDescriptorBindingFlags(VkDescriptorBindingFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkDescriptorBindingFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkDescriptorBindingFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkDescriptorBindingFlags(const VkDescriptorBindingFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkDescriptorBindingFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkDescriptorBindingFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkSemaphoreWaitFlags(VkSemaphoreWaitFlags obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSemaphoreWaitFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSemaphoreWaitFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}
static void print_VkSemaphoreWaitFlags(const VkSemaphoreWaitFlags * obj, const std::string& str, bool commaNeeded=true) {
     PRINT_SPACE
     if (str != "") _OUT << "\"" << str << "\"" << " : ";
     const int max_bits = 64; // We don't expect the number to be larger.
     std::bitset<max_bits> b(obj);
     _OUT << "\"";
     if (obj == 0) _OUT << "0";
     for (unsigned int i = 0, bitCount = 0; i < b.size(); i++) {
         if (b[i] == 1) {
             bitCount++;
             if (bitCount < b.count())
                 _OUT << VkSemaphoreWaitFlagBits_map[1ULL<<i] << " | ";
             else
                 _OUT << VkSemaphoreWaitFlagBits_map[1ULL<<i];
         }
     }
     if (commaNeeded)
       _OUT << "\"" << ",";
     else
       _OUT << "\""<< "";
     _OUT << std::endl;
}

static void print_VkPhysicalDeviceVulkan11Features(VkPhysicalDeviceVulkan11Features obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.storageBuffer16BitAccess, "storageBuffer16BitAccess", 1);

     print_VkBool32(obj.uniformAndStorageBuffer16BitAccess, "uniformAndStorageBuffer16BitAccess", 1);

     print_VkBool32(obj.storagePushConstant16, "storagePushConstant16", 1);

     print_VkBool32(obj.storageInputOutput16, "storageInputOutput16", 1);

     print_VkBool32(obj.multiview, "multiview", 1);

     print_VkBool32(obj.multiviewGeometryShader, "multiviewGeometryShader", 1);

     print_VkBool32(obj.multiviewTessellationShader, "multiviewTessellationShader", 1);

     print_VkBool32(obj.variablePointersStorageBuffer, "variablePointersStorageBuffer", 1);

     print_VkBool32(obj.variablePointers, "variablePointers", 1);

     print_VkBool32(obj.protectedMemory, "protectedMemory", 1);

     print_VkBool32(obj.samplerYcbcrConversion, "samplerYcbcrConversion", 1);

     print_VkBool32(obj.shaderDrawParameters, "shaderDrawParameters", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceVulkan11Features(const VkPhysicalDeviceVulkan11Features * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->storageBuffer16BitAccess, "storageBuffer16BitAccess", 1);

     print_VkBool32(obj->uniformAndStorageBuffer16BitAccess, "uniformAndStorageBuffer16BitAccess", 1);

     print_VkBool32(obj->storagePushConstant16, "storagePushConstant16", 1);

     print_VkBool32(obj->storageInputOutput16, "storageInputOutput16", 1);

     print_VkBool32(obj->multiview, "multiview", 1);

     print_VkBool32(obj->multiviewGeometryShader, "multiviewGeometryShader", 1);

     print_VkBool32(obj->multiviewTessellationShader, "multiviewTessellationShader", 1);

     print_VkBool32(obj->variablePointersStorageBuffer, "variablePointersStorageBuffer", 1);

     print_VkBool32(obj->variablePointers, "variablePointers", 1);

     print_VkBool32(obj->protectedMemory, "protectedMemory", 1);

     print_VkBool32(obj->samplerYcbcrConversion, "samplerYcbcrConversion", 1);

     print_VkBool32(obj->shaderDrawParameters, "shaderDrawParameters", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceVulkan11Properties(VkPhysicalDeviceVulkan11Properties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"deviceUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj.deviceUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"driverUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj.driverUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"deviceLUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_LUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_LUID_SIZE;
           print_uint8_t(obj.deviceLUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj.deviceNodeMask, "deviceNodeMask", 1);

     print_VkBool32(obj.deviceLUIDValid, "deviceLUIDValid", 1);

     print_uint32_t(obj.subgroupSize, "subgroupSize", 1);

     print_VkShaderStageFlags(obj.subgroupSupportedStages, "subgroupSupportedStages", 1);

     print_VkSubgroupFeatureFlags(obj.subgroupSupportedOperations, "subgroupSupportedOperations", 1);

     print_VkBool32(obj.subgroupQuadOperationsInAllStages, "subgroupQuadOperationsInAllStages", 1);

     print_VkPointClippingBehavior(obj.pointClippingBehavior, "pointClippingBehavior", 1);

     print_uint32_t(obj.maxMultiviewViewCount, "maxMultiviewViewCount", 1);

     print_uint32_t(obj.maxMultiviewInstanceIndex, "maxMultiviewInstanceIndex", 1);

     print_VkBool32(obj.protectedNoFault, "protectedNoFault", 1);

     print_uint32_t(obj.maxPerSetDescriptors, "maxPerSetDescriptors", 1);

     print_VkDeviceSize(obj.maxMemoryAllocationSize, "maxMemoryAllocationSize", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceVulkan11Properties(const VkPhysicalDeviceVulkan11Properties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"deviceUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj->deviceUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"driverUUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_UUID_SIZE;
           print_uint8_t(obj->driverUUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"deviceLUID\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_LUID_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_LUID_SIZE;
           print_uint8_t(obj->deviceLUID[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     print_uint32_t(obj->deviceNodeMask, "deviceNodeMask", 1);

     print_VkBool32(obj->deviceLUIDValid, "deviceLUIDValid", 1);

     print_uint32_t(obj->subgroupSize, "subgroupSize", 1);

     print_VkShaderStageFlags(obj->subgroupSupportedStages, "subgroupSupportedStages", 1);

     print_VkSubgroupFeatureFlags(obj->subgroupSupportedOperations, "subgroupSupportedOperations", 1);

     print_VkBool32(obj->subgroupQuadOperationsInAllStages, "subgroupQuadOperationsInAllStages", 1);

     print_VkPointClippingBehavior(obj->pointClippingBehavior, "pointClippingBehavior", 1);

     print_uint32_t(obj->maxMultiviewViewCount, "maxMultiviewViewCount", 1);

     print_uint32_t(obj->maxMultiviewInstanceIndex, "maxMultiviewInstanceIndex", 1);

     print_VkBool32(obj->protectedNoFault, "protectedNoFault", 1);

     print_uint32_t(obj->maxPerSetDescriptors, "maxPerSetDescriptors", 1);

     print_VkDeviceSize(obj->maxMemoryAllocationSize, "maxMemoryAllocationSize", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceVulkan12Features(VkPhysicalDeviceVulkan12Features obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.samplerMirrorClampToEdge, "samplerMirrorClampToEdge", 1);

     print_VkBool32(obj.drawIndirectCount, "drawIndirectCount", 1);

     print_VkBool32(obj.storageBuffer8BitAccess, "storageBuffer8BitAccess", 1);

     print_VkBool32(obj.uniformAndStorageBuffer8BitAccess, "uniformAndStorageBuffer8BitAccess", 1);

     print_VkBool32(obj.storagePushConstant8, "storagePushConstant8", 1);

     print_VkBool32(obj.shaderBufferInt64Atomics, "shaderBufferInt64Atomics", 1);

     print_VkBool32(obj.shaderSharedInt64Atomics, "shaderSharedInt64Atomics", 1);

     print_VkBool32(obj.shaderFloat16, "shaderFloat16", 1);

     print_VkBool32(obj.shaderInt8, "shaderInt8", 1);

     print_VkBool32(obj.descriptorIndexing, "descriptorIndexing", 1);

     print_VkBool32(obj.shaderInputAttachmentArrayDynamicIndexing, "shaderInputAttachmentArrayDynamicIndexing", 1);

     print_VkBool32(obj.shaderUniformTexelBufferArrayDynamicIndexing, "shaderUniformTexelBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj.shaderStorageTexelBufferArrayDynamicIndexing, "shaderStorageTexelBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj.shaderUniformBufferArrayNonUniformIndexing, "shaderUniformBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderSampledImageArrayNonUniformIndexing, "shaderSampledImageArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderStorageBufferArrayNonUniformIndexing, "shaderStorageBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderStorageImageArrayNonUniformIndexing, "shaderStorageImageArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderInputAttachmentArrayNonUniformIndexing, "shaderInputAttachmentArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderUniformTexelBufferArrayNonUniformIndexing, "shaderUniformTexelBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderStorageTexelBufferArrayNonUniformIndexing, "shaderStorageTexelBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj.descriptorBindingUniformBufferUpdateAfterBind, "descriptorBindingUniformBufferUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingSampledImageUpdateAfterBind, "descriptorBindingSampledImageUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingStorageImageUpdateAfterBind, "descriptorBindingStorageImageUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingStorageBufferUpdateAfterBind, "descriptorBindingStorageBufferUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingUniformTexelBufferUpdateAfterBind, "descriptorBindingUniformTexelBufferUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingStorageTexelBufferUpdateAfterBind, "descriptorBindingStorageTexelBufferUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingUpdateUnusedWhilePending, "descriptorBindingUpdateUnusedWhilePending", 1);

     print_VkBool32(obj.descriptorBindingPartiallyBound, "descriptorBindingPartiallyBound", 1);

     print_VkBool32(obj.descriptorBindingVariableDescriptorCount, "descriptorBindingVariableDescriptorCount", 1);

     print_VkBool32(obj.runtimeDescriptorArray, "runtimeDescriptorArray", 1);

     print_VkBool32(obj.samplerFilterMinmax, "samplerFilterMinmax", 1);

     print_VkBool32(obj.scalarBlockLayout, "scalarBlockLayout", 1);

     print_VkBool32(obj.imagelessFramebuffer, "imagelessFramebuffer", 1);

     print_VkBool32(obj.uniformBufferStandardLayout, "uniformBufferStandardLayout", 1);

     print_VkBool32(obj.shaderSubgroupExtendedTypes, "shaderSubgroupExtendedTypes", 1);

     print_VkBool32(obj.separateDepthStencilLayouts, "separateDepthStencilLayouts", 1);

     print_VkBool32(obj.hostQueryReset, "hostQueryReset", 1);

     print_VkBool32(obj.timelineSemaphore, "timelineSemaphore", 1);

     print_VkBool32(obj.bufferDeviceAddress, "bufferDeviceAddress", 1);

     print_VkBool32(obj.bufferDeviceAddressCaptureReplay, "bufferDeviceAddressCaptureReplay", 1);

     print_VkBool32(obj.bufferDeviceAddressMultiDevice, "bufferDeviceAddressMultiDevice", 1);

     print_VkBool32(obj.vulkanMemoryModel, "vulkanMemoryModel", 1);

     print_VkBool32(obj.vulkanMemoryModelDeviceScope, "vulkanMemoryModelDeviceScope", 1);

     print_VkBool32(obj.vulkanMemoryModelAvailabilityVisibilityChains, "vulkanMemoryModelAvailabilityVisibilityChains", 1);

     print_VkBool32(obj.shaderOutputViewportIndex, "shaderOutputViewportIndex", 1);

     print_VkBool32(obj.shaderOutputLayer, "shaderOutputLayer", 1);

     print_VkBool32(obj.subgroupBroadcastDynamicId, "subgroupBroadcastDynamicId", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceVulkan12Features(const VkPhysicalDeviceVulkan12Features * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->samplerMirrorClampToEdge, "samplerMirrorClampToEdge", 1);

     print_VkBool32(obj->drawIndirectCount, "drawIndirectCount", 1);

     print_VkBool32(obj->storageBuffer8BitAccess, "storageBuffer8BitAccess", 1);

     print_VkBool32(obj->uniformAndStorageBuffer8BitAccess, "uniformAndStorageBuffer8BitAccess", 1);

     print_VkBool32(obj->storagePushConstant8, "storagePushConstant8", 1);

     print_VkBool32(obj->shaderBufferInt64Atomics, "shaderBufferInt64Atomics", 1);

     print_VkBool32(obj->shaderSharedInt64Atomics, "shaderSharedInt64Atomics", 1);

     print_VkBool32(obj->shaderFloat16, "shaderFloat16", 1);

     print_VkBool32(obj->shaderInt8, "shaderInt8", 1);

     print_VkBool32(obj->descriptorIndexing, "descriptorIndexing", 1);

     print_VkBool32(obj->shaderInputAttachmentArrayDynamicIndexing, "shaderInputAttachmentArrayDynamicIndexing", 1);

     print_VkBool32(obj->shaderUniformTexelBufferArrayDynamicIndexing, "shaderUniformTexelBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj->shaderStorageTexelBufferArrayDynamicIndexing, "shaderStorageTexelBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj->shaderUniformBufferArrayNonUniformIndexing, "shaderUniformBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderSampledImageArrayNonUniformIndexing, "shaderSampledImageArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderStorageBufferArrayNonUniformIndexing, "shaderStorageBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderStorageImageArrayNonUniformIndexing, "shaderStorageImageArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderInputAttachmentArrayNonUniformIndexing, "shaderInputAttachmentArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderUniformTexelBufferArrayNonUniformIndexing, "shaderUniformTexelBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderStorageTexelBufferArrayNonUniformIndexing, "shaderStorageTexelBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj->descriptorBindingUniformBufferUpdateAfterBind, "descriptorBindingUniformBufferUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingSampledImageUpdateAfterBind, "descriptorBindingSampledImageUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingStorageImageUpdateAfterBind, "descriptorBindingStorageImageUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingStorageBufferUpdateAfterBind, "descriptorBindingStorageBufferUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingUniformTexelBufferUpdateAfterBind, "descriptorBindingUniformTexelBufferUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingStorageTexelBufferUpdateAfterBind, "descriptorBindingStorageTexelBufferUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingUpdateUnusedWhilePending, "descriptorBindingUpdateUnusedWhilePending", 1);

     print_VkBool32(obj->descriptorBindingPartiallyBound, "descriptorBindingPartiallyBound", 1);

     print_VkBool32(obj->descriptorBindingVariableDescriptorCount, "descriptorBindingVariableDescriptorCount", 1);

     print_VkBool32(obj->runtimeDescriptorArray, "runtimeDescriptorArray", 1);

     print_VkBool32(obj->samplerFilterMinmax, "samplerFilterMinmax", 1);

     print_VkBool32(obj->scalarBlockLayout, "scalarBlockLayout", 1);

     print_VkBool32(obj->imagelessFramebuffer, "imagelessFramebuffer", 1);

     print_VkBool32(obj->uniformBufferStandardLayout, "uniformBufferStandardLayout", 1);

     print_VkBool32(obj->shaderSubgroupExtendedTypes, "shaderSubgroupExtendedTypes", 1);

     print_VkBool32(obj->separateDepthStencilLayouts, "separateDepthStencilLayouts", 1);

     print_VkBool32(obj->hostQueryReset, "hostQueryReset", 1);

     print_VkBool32(obj->timelineSemaphore, "timelineSemaphore", 1);

     print_VkBool32(obj->bufferDeviceAddress, "bufferDeviceAddress", 1);

     print_VkBool32(obj->bufferDeviceAddressCaptureReplay, "bufferDeviceAddressCaptureReplay", 1);

     print_VkBool32(obj->bufferDeviceAddressMultiDevice, "bufferDeviceAddressMultiDevice", 1);

     print_VkBool32(obj->vulkanMemoryModel, "vulkanMemoryModel", 1);

     print_VkBool32(obj->vulkanMemoryModelDeviceScope, "vulkanMemoryModelDeviceScope", 1);

     print_VkBool32(obj->vulkanMemoryModelAvailabilityVisibilityChains, "vulkanMemoryModelAvailabilityVisibilityChains", 1);

     print_VkBool32(obj->shaderOutputViewportIndex, "shaderOutputViewportIndex", 1);

     print_VkBool32(obj->shaderOutputLayer, "shaderOutputLayer", 1);

     print_VkBool32(obj->subgroupBroadcastDynamicId, "subgroupBroadcastDynamicId", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkConformanceVersion(VkConformanceVersion obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint8_t(obj.major, "major", 1);

     print_uint8_t(obj.minor, "minor", 1);

     print_uint8_t(obj.subminor, "subminor", 1);

     print_uint8_t(obj.patch, "patch", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkConformanceVersion(const VkConformanceVersion * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_uint8_t(obj->major, "major", 1);

     print_uint8_t(obj->minor, "minor", 1);

     print_uint8_t(obj->subminor, "subminor", 1);

     print_uint8_t(obj->patch, "patch", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceVulkan12Properties(VkPhysicalDeviceVulkan12Properties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDriverId(obj.driverID, "driverID", 1);

     PRINT_SPACE
     _OUT << "\"driverName\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DRIVER_NAME_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_DRIVER_NAME_SIZE;
           print_char(obj.driverName[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"driverInfo\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DRIVER_INFO_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_DRIVER_INFO_SIZE;
           print_char(obj.driverInfo[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"conformanceVersion\": " << std::endl;
     {
           print_VkConformanceVersion(obj.conformanceVersion, "conformanceVersion", 1);
     }

     print_VkShaderFloatControlsIndependence(obj.denormBehaviorIndependence, "denormBehaviorIndependence", 1);

     print_VkShaderFloatControlsIndependence(obj.roundingModeIndependence, "roundingModeIndependence", 1);

     print_VkBool32(obj.shaderSignedZeroInfNanPreserveFloat16, "shaderSignedZeroInfNanPreserveFloat16", 1);

     print_VkBool32(obj.shaderSignedZeroInfNanPreserveFloat32, "shaderSignedZeroInfNanPreserveFloat32", 1);

     print_VkBool32(obj.shaderSignedZeroInfNanPreserveFloat64, "shaderSignedZeroInfNanPreserveFloat64", 1);

     print_VkBool32(obj.shaderDenormPreserveFloat16, "shaderDenormPreserveFloat16", 1);

     print_VkBool32(obj.shaderDenormPreserveFloat32, "shaderDenormPreserveFloat32", 1);

     print_VkBool32(obj.shaderDenormPreserveFloat64, "shaderDenormPreserveFloat64", 1);

     print_VkBool32(obj.shaderDenormFlushToZeroFloat16, "shaderDenormFlushToZeroFloat16", 1);

     print_VkBool32(obj.shaderDenormFlushToZeroFloat32, "shaderDenormFlushToZeroFloat32", 1);

     print_VkBool32(obj.shaderDenormFlushToZeroFloat64, "shaderDenormFlushToZeroFloat64", 1);

     print_VkBool32(obj.shaderRoundingModeRTEFloat16, "shaderRoundingModeRTEFloat16", 1);

     print_VkBool32(obj.shaderRoundingModeRTEFloat32, "shaderRoundingModeRTEFloat32", 1);

     print_VkBool32(obj.shaderRoundingModeRTEFloat64, "shaderRoundingModeRTEFloat64", 1);

     print_VkBool32(obj.shaderRoundingModeRTZFloat16, "shaderRoundingModeRTZFloat16", 1);

     print_VkBool32(obj.shaderRoundingModeRTZFloat32, "shaderRoundingModeRTZFloat32", 1);

     print_VkBool32(obj.shaderRoundingModeRTZFloat64, "shaderRoundingModeRTZFloat64", 1);

     print_uint32_t(obj.maxUpdateAfterBindDescriptorsInAllPools, "maxUpdateAfterBindDescriptorsInAllPools", 1);

     print_VkBool32(obj.shaderUniformBufferArrayNonUniformIndexingNative, "shaderUniformBufferArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj.shaderSampledImageArrayNonUniformIndexingNative, "shaderSampledImageArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj.shaderStorageBufferArrayNonUniformIndexingNative, "shaderStorageBufferArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj.shaderStorageImageArrayNonUniformIndexingNative, "shaderStorageImageArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj.shaderInputAttachmentArrayNonUniformIndexingNative, "shaderInputAttachmentArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj.robustBufferAccessUpdateAfterBind, "robustBufferAccessUpdateAfterBind", 1);

     print_VkBool32(obj.quadDivergentImplicitLod, "quadDivergentImplicitLod", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindSamplers, "maxPerStageDescriptorUpdateAfterBindSamplers", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindUniformBuffers, "maxPerStageDescriptorUpdateAfterBindUniformBuffers", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindStorageBuffers, "maxPerStageDescriptorUpdateAfterBindStorageBuffers", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindSampledImages, "maxPerStageDescriptorUpdateAfterBindSampledImages", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindStorageImages, "maxPerStageDescriptorUpdateAfterBindStorageImages", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindInputAttachments, "maxPerStageDescriptorUpdateAfterBindInputAttachments", 1);

     print_uint32_t(obj.maxPerStageUpdateAfterBindResources, "maxPerStageUpdateAfterBindResources", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindSamplers, "maxDescriptorSetUpdateAfterBindSamplers", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindUniformBuffers, "maxDescriptorSetUpdateAfterBindUniformBuffers", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic, "maxDescriptorSetUpdateAfterBindUniformBuffersDynamic", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindStorageBuffers, "maxDescriptorSetUpdateAfterBindStorageBuffers", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic, "maxDescriptorSetUpdateAfterBindStorageBuffersDynamic", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindSampledImages, "maxDescriptorSetUpdateAfterBindSampledImages", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindStorageImages, "maxDescriptorSetUpdateAfterBindStorageImages", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindInputAttachments, "maxDescriptorSetUpdateAfterBindInputAttachments", 1);

     print_VkResolveModeFlags(obj.supportedDepthResolveModes, "supportedDepthResolveModes", 1);

     print_VkResolveModeFlags(obj.supportedStencilResolveModes, "supportedStencilResolveModes", 1);

     print_VkBool32(obj.independentResolveNone, "independentResolveNone", 1);

     print_VkBool32(obj.independentResolve, "independentResolve", 1);

     print_VkBool32(obj.filterMinmaxSingleComponentFormats, "filterMinmaxSingleComponentFormats", 1);

     print_VkBool32(obj.filterMinmaxImageComponentMapping, "filterMinmaxImageComponentMapping", 1);

     print_uint64_t(obj.maxTimelineSemaphoreValueDifference, "maxTimelineSemaphoreValueDifference", 1);

     print_VkSampleCountFlags(obj.framebufferIntegerColorSampleCounts, "framebufferIntegerColorSampleCounts", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceVulkan12Properties(const VkPhysicalDeviceVulkan12Properties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDriverId(obj->driverID, "driverID", 1);

     PRINT_SPACE
     _OUT << "\"driverName\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DRIVER_NAME_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_DRIVER_NAME_SIZE;
           print_char(obj->driverName[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"driverInfo\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DRIVER_INFO_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_DRIVER_INFO_SIZE;
           print_char(obj->driverInfo[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"conformanceVersion\": " << std::endl;
     {
           print_VkConformanceVersion(obj->conformanceVersion, "conformanceVersion", 1);
     }

     print_VkShaderFloatControlsIndependence(obj->denormBehaviorIndependence, "denormBehaviorIndependence", 1);

     print_VkShaderFloatControlsIndependence(obj->roundingModeIndependence, "roundingModeIndependence", 1);

     print_VkBool32(obj->shaderSignedZeroInfNanPreserveFloat16, "shaderSignedZeroInfNanPreserveFloat16", 1);

     print_VkBool32(obj->shaderSignedZeroInfNanPreserveFloat32, "shaderSignedZeroInfNanPreserveFloat32", 1);

     print_VkBool32(obj->shaderSignedZeroInfNanPreserveFloat64, "shaderSignedZeroInfNanPreserveFloat64", 1);

     print_VkBool32(obj->shaderDenormPreserveFloat16, "shaderDenormPreserveFloat16", 1);

     print_VkBool32(obj->shaderDenormPreserveFloat32, "shaderDenormPreserveFloat32", 1);

     print_VkBool32(obj->shaderDenormPreserveFloat64, "shaderDenormPreserveFloat64", 1);

     print_VkBool32(obj->shaderDenormFlushToZeroFloat16, "shaderDenormFlushToZeroFloat16", 1);

     print_VkBool32(obj->shaderDenormFlushToZeroFloat32, "shaderDenormFlushToZeroFloat32", 1);

     print_VkBool32(obj->shaderDenormFlushToZeroFloat64, "shaderDenormFlushToZeroFloat64", 1);

     print_VkBool32(obj->shaderRoundingModeRTEFloat16, "shaderRoundingModeRTEFloat16", 1);

     print_VkBool32(obj->shaderRoundingModeRTEFloat32, "shaderRoundingModeRTEFloat32", 1);

     print_VkBool32(obj->shaderRoundingModeRTEFloat64, "shaderRoundingModeRTEFloat64", 1);

     print_VkBool32(obj->shaderRoundingModeRTZFloat16, "shaderRoundingModeRTZFloat16", 1);

     print_VkBool32(obj->shaderRoundingModeRTZFloat32, "shaderRoundingModeRTZFloat32", 1);

     print_VkBool32(obj->shaderRoundingModeRTZFloat64, "shaderRoundingModeRTZFloat64", 1);

     print_uint32_t(obj->maxUpdateAfterBindDescriptorsInAllPools, "maxUpdateAfterBindDescriptorsInAllPools", 1);

     print_VkBool32(obj->shaderUniformBufferArrayNonUniformIndexingNative, "shaderUniformBufferArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj->shaderSampledImageArrayNonUniformIndexingNative, "shaderSampledImageArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj->shaderStorageBufferArrayNonUniformIndexingNative, "shaderStorageBufferArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj->shaderStorageImageArrayNonUniformIndexingNative, "shaderStorageImageArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj->shaderInputAttachmentArrayNonUniformIndexingNative, "shaderInputAttachmentArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj->robustBufferAccessUpdateAfterBind, "robustBufferAccessUpdateAfterBind", 1);

     print_VkBool32(obj->quadDivergentImplicitLod, "quadDivergentImplicitLod", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindSamplers, "maxPerStageDescriptorUpdateAfterBindSamplers", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindUniformBuffers, "maxPerStageDescriptorUpdateAfterBindUniformBuffers", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindStorageBuffers, "maxPerStageDescriptorUpdateAfterBindStorageBuffers", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindSampledImages, "maxPerStageDescriptorUpdateAfterBindSampledImages", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindStorageImages, "maxPerStageDescriptorUpdateAfterBindStorageImages", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindInputAttachments, "maxPerStageDescriptorUpdateAfterBindInputAttachments", 1);

     print_uint32_t(obj->maxPerStageUpdateAfterBindResources, "maxPerStageUpdateAfterBindResources", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindSamplers, "maxDescriptorSetUpdateAfterBindSamplers", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindUniformBuffers, "maxDescriptorSetUpdateAfterBindUniformBuffers", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic, "maxDescriptorSetUpdateAfterBindUniformBuffersDynamic", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindStorageBuffers, "maxDescriptorSetUpdateAfterBindStorageBuffers", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic, "maxDescriptorSetUpdateAfterBindStorageBuffersDynamic", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindSampledImages, "maxDescriptorSetUpdateAfterBindSampledImages", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindStorageImages, "maxDescriptorSetUpdateAfterBindStorageImages", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindInputAttachments, "maxDescriptorSetUpdateAfterBindInputAttachments", 1);

     print_VkResolveModeFlags(obj->supportedDepthResolveModes, "supportedDepthResolveModes", 1);

     print_VkResolveModeFlags(obj->supportedStencilResolveModes, "supportedStencilResolveModes", 1);

     print_VkBool32(obj->independentResolveNone, "independentResolveNone", 1);

     print_VkBool32(obj->independentResolve, "independentResolve", 1);

     print_VkBool32(obj->filterMinmaxSingleComponentFormats, "filterMinmaxSingleComponentFormats", 1);

     print_VkBool32(obj->filterMinmaxImageComponentMapping, "filterMinmaxImageComponentMapping", 1);

     print_uint64_t(obj->maxTimelineSemaphoreValueDifference, "maxTimelineSemaphoreValueDifference", 1);

     print_VkSampleCountFlags(obj->framebufferIntegerColorSampleCounts, "framebufferIntegerColorSampleCounts", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageFormatListCreateInfo(VkImageFormatListCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.viewFormatCount, "viewFormatCount", 1);

     PRINT_SPACE
     _OUT << "\"pViewFormats\":" << std::endl;
     PRINT_SPACE
     if (obj.pViewFormats) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.viewFormatCount; i++) {
           bool isCommaNeeded = (i+1) != obj.viewFormatCount;
           print_VkFormat(obj.pViewFormats[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageFormatListCreateInfo(const VkImageFormatListCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->viewFormatCount, "viewFormatCount", 1);

     PRINT_SPACE
     _OUT << "\"pViewFormats\":" << std::endl;
     PRINT_SPACE
     if (obj->pViewFormats) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->viewFormatCount; i++) {
           bool isCommaNeeded = (i+1) != obj->viewFormatCount;
           print_VkFormat(obj->pViewFormats[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkAttachmentDescription2(VkAttachmentDescription2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkAttachmentDescriptionFlags(obj.flags, "flags", 1);

     print_VkFormat(obj.format, "format", 1);

     print_VkSampleCountFlagBits(obj.samples, "samples", 1);

     print_VkAttachmentLoadOp(obj.loadOp, "loadOp", 1);

     print_VkAttachmentStoreOp(obj.storeOp, "storeOp", 1);

     print_VkAttachmentLoadOp(obj.stencilLoadOp, "stencilLoadOp", 1);

     print_VkAttachmentStoreOp(obj.stencilStoreOp, "stencilStoreOp", 1);

     print_VkImageLayout(obj.initialLayout, "initialLayout", 1);

     print_VkImageLayout(obj.finalLayout, "finalLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkAttachmentDescription2(const VkAttachmentDescription2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkAttachmentDescriptionFlags(obj->flags, "flags", 1);

     print_VkFormat(obj->format, "format", 1);

     print_VkSampleCountFlagBits(obj->samples, "samples", 1);

     print_VkAttachmentLoadOp(obj->loadOp, "loadOp", 1);

     print_VkAttachmentStoreOp(obj->storeOp, "storeOp", 1);

     print_VkAttachmentLoadOp(obj->stencilLoadOp, "stencilLoadOp", 1);

     print_VkAttachmentStoreOp(obj->stencilStoreOp, "stencilStoreOp", 1);

     print_VkImageLayout(obj->initialLayout, "initialLayout", 1);

     print_VkImageLayout(obj->finalLayout, "finalLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkAttachmentReference2(VkAttachmentReference2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.attachment, "attachment", 1);

     print_VkImageLayout(obj.layout, "layout", 1);

     print_VkImageAspectFlags(obj.aspectMask, "aspectMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkAttachmentReference2(const VkAttachmentReference2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->attachment, "attachment", 1);

     print_VkImageLayout(obj->layout, "layout", 1);

     print_VkImageAspectFlags(obj->aspectMask, "aspectMask", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSubpassDescription2(VkSubpassDescription2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSubpassDescriptionFlags(obj.flags, "flags", 1);

     print_VkPipelineBindPoint(obj.pipelineBindPoint, "pipelineBindPoint", 1);

     print_uint32_t(obj.viewMask, "viewMask", 1);

     print_uint32_t(obj.inputAttachmentCount, "inputAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pInputAttachments\": " << std::endl;
     if (obj.pInputAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.inputAttachmentCount; i++) {
           if (i+1 == obj.inputAttachmentCount)
               print_VkAttachmentReference2(obj.pInputAttachments[i], "pInputAttachments", 0);
           else
               print_VkAttachmentReference2(obj.pInputAttachments[i], "pInputAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.colorAttachmentCount, "colorAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pColorAttachments\": " << std::endl;
     if (obj.pColorAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.colorAttachmentCount; i++) {
           if (i+1 == obj.colorAttachmentCount)
               print_VkAttachmentReference2(obj.pColorAttachments[i], "pColorAttachments", 0);
           else
               print_VkAttachmentReference2(obj.pColorAttachments[i], "pColorAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pResolveAttachments\": " << std::endl;
     if (obj.pResolveAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.colorAttachmentCount; i++) {
           if (i+1 == obj.colorAttachmentCount)
               print_VkAttachmentReference2(obj.pResolveAttachments[i], "pResolveAttachments", 0);
           else
               print_VkAttachmentReference2(obj.pResolveAttachments[i], "pResolveAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pDepthStencilAttachment\": " << std::endl;
     if (obj.pDepthStencilAttachment) {
           print_VkAttachmentReference2(obj.pDepthStencilAttachment, "pDepthStencilAttachment", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.preserveAttachmentCount, "preserveAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pPreserveAttachments\":" << std::endl;
     PRINT_SPACE
     if (obj.pPreserveAttachments) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.preserveAttachmentCount; i++) {
           bool isCommaNeeded = (i+1) != obj.preserveAttachmentCount;
           print_uint32_t(obj.pPreserveAttachments[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSubpassDescription2(const VkSubpassDescription2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSubpassDescriptionFlags(obj->flags, "flags", 1);

     print_VkPipelineBindPoint(obj->pipelineBindPoint, "pipelineBindPoint", 1);

     print_uint32_t(obj->viewMask, "viewMask", 1);

     print_uint32_t(obj->inputAttachmentCount, "inputAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pInputAttachments\": " << std::endl;
     if (obj->pInputAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->inputAttachmentCount; i++) {
           if (i+1 == obj->inputAttachmentCount)
               print_VkAttachmentReference2(obj->pInputAttachments[i], "pInputAttachments", 0);
           else
               print_VkAttachmentReference2(obj->pInputAttachments[i], "pInputAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->colorAttachmentCount, "colorAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pColorAttachments\": " << std::endl;
     if (obj->pColorAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->colorAttachmentCount; i++) {
           if (i+1 == obj->colorAttachmentCount)
               print_VkAttachmentReference2(obj->pColorAttachments[i], "pColorAttachments", 0);
           else
               print_VkAttachmentReference2(obj->pColorAttachments[i], "pColorAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pResolveAttachments\": " << std::endl;
     if (obj->pResolveAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->colorAttachmentCount; i++) {
           if (i+1 == obj->colorAttachmentCount)
               print_VkAttachmentReference2(obj->pResolveAttachments[i], "pResolveAttachments", 0);
           else
               print_VkAttachmentReference2(obj->pResolveAttachments[i], "pResolveAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pDepthStencilAttachment\": " << std::endl;
     if (obj->pDepthStencilAttachment) {
           print_VkAttachmentReference2(obj->pDepthStencilAttachment, "pDepthStencilAttachment", 1);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->preserveAttachmentCount, "preserveAttachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pPreserveAttachments\":" << std::endl;
     PRINT_SPACE
     if (obj->pPreserveAttachments) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->preserveAttachmentCount; i++) {
           bool isCommaNeeded = (i+1) != obj->preserveAttachmentCount;
           print_uint32_t(obj->pPreserveAttachments[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSubpassDependency2(VkSubpassDependency2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.srcSubpass, "srcSubpass", 1);

     print_uint32_t(obj.dstSubpass, "dstSubpass", 1);

     print_VkPipelineStageFlags(obj.srcStageMask, "srcStageMask", 1);

     print_VkPipelineStageFlags(obj.dstStageMask, "dstStageMask", 1);

     print_VkAccessFlags(obj.srcAccessMask, "srcAccessMask", 1);

     print_VkAccessFlags(obj.dstAccessMask, "dstAccessMask", 1);

     print_VkDependencyFlags(obj.dependencyFlags, "dependencyFlags", 1);

     print_int32_t(obj.viewOffset, "viewOffset", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSubpassDependency2(const VkSubpassDependency2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->srcSubpass, "srcSubpass", 1);

     print_uint32_t(obj->dstSubpass, "dstSubpass", 1);

     print_VkPipelineStageFlags(obj->srcStageMask, "srcStageMask", 1);

     print_VkPipelineStageFlags(obj->dstStageMask, "dstStageMask", 1);

     print_VkAccessFlags(obj->srcAccessMask, "srcAccessMask", 1);

     print_VkAccessFlags(obj->dstAccessMask, "dstAccessMask", 1);

     print_VkDependencyFlags(obj->dependencyFlags, "dependencyFlags", 1);

     print_int32_t(obj->viewOffset, "viewOffset", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkRenderPassCreateInfo2(VkRenderPassCreateInfo2 obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkRenderPassCreateFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.attachmentCount, "attachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachments\": " << std::endl;
     if (obj.pAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.attachmentCount; i++) {
           if (i+1 == obj.attachmentCount)
               print_VkAttachmentDescription2(obj.pAttachments[i], "pAttachments", 0);
           else
               print_VkAttachmentDescription2(obj.pAttachments[i], "pAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.subpassCount, "subpassCount", 1);

     PRINT_SPACE
     _OUT << "\"pSubpasses\": " << std::endl;
     if (obj.pSubpasses) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.subpassCount; i++) {
           if (i+1 == obj.subpassCount)
               print_VkSubpassDescription2(obj.pSubpasses[i], "pSubpasses", 0);
           else
               print_VkSubpassDescription2(obj.pSubpasses[i], "pSubpasses", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.dependencyCount, "dependencyCount", 1);

     PRINT_SPACE
     _OUT << "\"pDependencies\": " << std::endl;
     if (obj.pDependencies) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.dependencyCount; i++) {
           if (i+1 == obj.dependencyCount)
               print_VkSubpassDependency2(obj.pDependencies[i], "pDependencies", 0);
           else
               print_VkSubpassDependency2(obj.pDependencies[i], "pDependencies", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.correlatedViewMaskCount, "correlatedViewMaskCount", 1);

     PRINT_SPACE
     _OUT << "\"pCorrelatedViewMasks\":" << std::endl;
     PRINT_SPACE
     if (obj.pCorrelatedViewMasks) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.correlatedViewMaskCount; i++) {
           bool isCommaNeeded = (i+1) != obj.correlatedViewMaskCount;
           print_uint32_t(obj.pCorrelatedViewMasks[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkRenderPassCreateInfo2(const VkRenderPassCreateInfo2 * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkRenderPassCreateFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->attachmentCount, "attachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachments\": " << std::endl;
     if (obj->pAttachments) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->attachmentCount; i++) {
           if (i+1 == obj->attachmentCount)
               print_VkAttachmentDescription2(obj->pAttachments[i], "pAttachments", 0);
           else
               print_VkAttachmentDescription2(obj->pAttachments[i], "pAttachments", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->subpassCount, "subpassCount", 1);

     PRINT_SPACE
     _OUT << "\"pSubpasses\": " << std::endl;
     if (obj->pSubpasses) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->subpassCount; i++) {
           if (i+1 == obj->subpassCount)
               print_VkSubpassDescription2(obj->pSubpasses[i], "pSubpasses", 0);
           else
               print_VkSubpassDescription2(obj->pSubpasses[i], "pSubpasses", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->dependencyCount, "dependencyCount", 1);

     PRINT_SPACE
     _OUT << "\"pDependencies\": " << std::endl;
     if (obj->pDependencies) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->dependencyCount; i++) {
           if (i+1 == obj->dependencyCount)
               print_VkSubpassDependency2(obj->pDependencies[i], "pDependencies", 0);
           else
               print_VkSubpassDependency2(obj->pDependencies[i], "pDependencies", 1);
         }
         PRINT_SPACE
         _OUT << "]," << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->correlatedViewMaskCount, "correlatedViewMaskCount", 1);

     PRINT_SPACE
     _OUT << "\"pCorrelatedViewMasks\":" << std::endl;
     PRINT_SPACE
     if (obj->pCorrelatedViewMasks) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->correlatedViewMaskCount; i++) {
           bool isCommaNeeded = (i+1) != obj->correlatedViewMaskCount;
           print_uint32_t(obj->pCorrelatedViewMasks[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSubpassBeginInfo(VkSubpassBeginInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSubpassContents(obj.contents, "contents", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSubpassBeginInfo(const VkSubpassBeginInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSubpassContents(obj->contents, "contents", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSubpassEndInfo(VkSubpassEndInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSubpassEndInfo(const VkSubpassEndInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDevice8BitStorageFeatures(VkPhysicalDevice8BitStorageFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.storageBuffer8BitAccess, "storageBuffer8BitAccess", 1);

     print_VkBool32(obj.uniformAndStorageBuffer8BitAccess, "uniformAndStorageBuffer8BitAccess", 1);

     print_VkBool32(obj.storagePushConstant8, "storagePushConstant8", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDevice8BitStorageFeatures(const VkPhysicalDevice8BitStorageFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->storageBuffer8BitAccess, "storageBuffer8BitAccess", 1);

     print_VkBool32(obj->uniformAndStorageBuffer8BitAccess, "uniformAndStorageBuffer8BitAccess", 1);

     print_VkBool32(obj->storagePushConstant8, "storagePushConstant8", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceDriverProperties(VkPhysicalDeviceDriverProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDriverId(obj.driverID, "driverID", 1);

     PRINT_SPACE
     _OUT << "\"driverName\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DRIVER_NAME_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_DRIVER_NAME_SIZE;
           print_char(obj.driverName[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"driverInfo\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DRIVER_INFO_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_DRIVER_INFO_SIZE;
           print_char(obj.driverInfo[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"conformanceVersion\": " << std::endl;
     {
           print_VkConformanceVersion(obj.conformanceVersion, "conformanceVersion", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceDriverProperties(const VkPhysicalDeviceDriverProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkDriverId(obj->driverID, "driverID", 1);

     PRINT_SPACE
     _OUT << "\"driverName\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DRIVER_NAME_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_DRIVER_NAME_SIZE;
           print_char(obj->driverName[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"driverInfo\":" << std::endl;
     PRINT_SPACE
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < VK_MAX_DRIVER_INFO_SIZE; i++) {
           bool isCommaNeeded = (i+1) != VK_MAX_DRIVER_INFO_SIZE;
           print_char(obj->driverInfo[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;

     PRINT_SPACE
     _OUT << "\"conformanceVersion\": " << std::endl;
     {
           print_VkConformanceVersion(obj->conformanceVersion, "conformanceVersion", 0);
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceShaderAtomicInt64Features(VkPhysicalDeviceShaderAtomicInt64Features obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.shaderBufferInt64Atomics, "shaderBufferInt64Atomics", 1);

     print_VkBool32(obj.shaderSharedInt64Atomics, "shaderSharedInt64Atomics", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceShaderAtomicInt64Features(const VkPhysicalDeviceShaderAtomicInt64Features * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->shaderBufferInt64Atomics, "shaderBufferInt64Atomics", 1);

     print_VkBool32(obj->shaderSharedInt64Atomics, "shaderSharedInt64Atomics", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceShaderFloat16Int8Features(VkPhysicalDeviceShaderFloat16Int8Features obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.shaderFloat16, "shaderFloat16", 1);

     print_VkBool32(obj.shaderInt8, "shaderInt8", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceShaderFloat16Int8Features(const VkPhysicalDeviceShaderFloat16Int8Features * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->shaderFloat16, "shaderFloat16", 1);

     print_VkBool32(obj->shaderInt8, "shaderInt8", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceFloatControlsProperties(VkPhysicalDeviceFloatControlsProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkShaderFloatControlsIndependence(obj.denormBehaviorIndependence, "denormBehaviorIndependence", 1);

     print_VkShaderFloatControlsIndependence(obj.roundingModeIndependence, "roundingModeIndependence", 1);

     print_VkBool32(obj.shaderSignedZeroInfNanPreserveFloat16, "shaderSignedZeroInfNanPreserveFloat16", 1);

     print_VkBool32(obj.shaderSignedZeroInfNanPreserveFloat32, "shaderSignedZeroInfNanPreserveFloat32", 1);

     print_VkBool32(obj.shaderSignedZeroInfNanPreserveFloat64, "shaderSignedZeroInfNanPreserveFloat64", 1);

     print_VkBool32(obj.shaderDenormPreserveFloat16, "shaderDenormPreserveFloat16", 1);

     print_VkBool32(obj.shaderDenormPreserveFloat32, "shaderDenormPreserveFloat32", 1);

     print_VkBool32(obj.shaderDenormPreserveFloat64, "shaderDenormPreserveFloat64", 1);

     print_VkBool32(obj.shaderDenormFlushToZeroFloat16, "shaderDenormFlushToZeroFloat16", 1);

     print_VkBool32(obj.shaderDenormFlushToZeroFloat32, "shaderDenormFlushToZeroFloat32", 1);

     print_VkBool32(obj.shaderDenormFlushToZeroFloat64, "shaderDenormFlushToZeroFloat64", 1);

     print_VkBool32(obj.shaderRoundingModeRTEFloat16, "shaderRoundingModeRTEFloat16", 1);

     print_VkBool32(obj.shaderRoundingModeRTEFloat32, "shaderRoundingModeRTEFloat32", 1);

     print_VkBool32(obj.shaderRoundingModeRTEFloat64, "shaderRoundingModeRTEFloat64", 1);

     print_VkBool32(obj.shaderRoundingModeRTZFloat16, "shaderRoundingModeRTZFloat16", 1);

     print_VkBool32(obj.shaderRoundingModeRTZFloat32, "shaderRoundingModeRTZFloat32", 1);

     print_VkBool32(obj.shaderRoundingModeRTZFloat64, "shaderRoundingModeRTZFloat64", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceFloatControlsProperties(const VkPhysicalDeviceFloatControlsProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkShaderFloatControlsIndependence(obj->denormBehaviorIndependence, "denormBehaviorIndependence", 1);

     print_VkShaderFloatControlsIndependence(obj->roundingModeIndependence, "roundingModeIndependence", 1);

     print_VkBool32(obj->shaderSignedZeroInfNanPreserveFloat16, "shaderSignedZeroInfNanPreserveFloat16", 1);

     print_VkBool32(obj->shaderSignedZeroInfNanPreserveFloat32, "shaderSignedZeroInfNanPreserveFloat32", 1);

     print_VkBool32(obj->shaderSignedZeroInfNanPreserveFloat64, "shaderSignedZeroInfNanPreserveFloat64", 1);

     print_VkBool32(obj->shaderDenormPreserveFloat16, "shaderDenormPreserveFloat16", 1);

     print_VkBool32(obj->shaderDenormPreserveFloat32, "shaderDenormPreserveFloat32", 1);

     print_VkBool32(obj->shaderDenormPreserveFloat64, "shaderDenormPreserveFloat64", 1);

     print_VkBool32(obj->shaderDenormFlushToZeroFloat16, "shaderDenormFlushToZeroFloat16", 1);

     print_VkBool32(obj->shaderDenormFlushToZeroFloat32, "shaderDenormFlushToZeroFloat32", 1);

     print_VkBool32(obj->shaderDenormFlushToZeroFloat64, "shaderDenormFlushToZeroFloat64", 1);

     print_VkBool32(obj->shaderRoundingModeRTEFloat16, "shaderRoundingModeRTEFloat16", 1);

     print_VkBool32(obj->shaderRoundingModeRTEFloat32, "shaderRoundingModeRTEFloat32", 1);

     print_VkBool32(obj->shaderRoundingModeRTEFloat64, "shaderRoundingModeRTEFloat64", 1);

     print_VkBool32(obj->shaderRoundingModeRTZFloat16, "shaderRoundingModeRTZFloat16", 1);

     print_VkBool32(obj->shaderRoundingModeRTZFloat32, "shaderRoundingModeRTZFloat32", 1);

     print_VkBool32(obj->shaderRoundingModeRTZFloat64, "shaderRoundingModeRTZFloat64", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorSetLayoutBindingFlagsCreateInfo(VkDescriptorSetLayoutBindingFlagsCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.bindingCount, "bindingCount", 1);

     PRINT_SPACE
     _OUT << "\"pBindingFlags\":" << std::endl;
     PRINT_SPACE
     if (obj.pBindingFlags) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.bindingCount; i++) {
           bool isCommaNeeded = (i+1) != obj.bindingCount;
           print_VkDescriptorBindingFlags(obj.pBindingFlags[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorSetLayoutBindingFlagsCreateInfo(const VkDescriptorSetLayoutBindingFlagsCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->bindingCount, "bindingCount", 1);

     PRINT_SPACE
     _OUT << "\"pBindingFlags\":" << std::endl;
     PRINT_SPACE
     if (obj->pBindingFlags) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->bindingCount; i++) {
           bool isCommaNeeded = (i+1) != obj->bindingCount;
           print_VkDescriptorBindingFlags(obj->pBindingFlags[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceDescriptorIndexingFeatures(VkPhysicalDeviceDescriptorIndexingFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.shaderInputAttachmentArrayDynamicIndexing, "shaderInputAttachmentArrayDynamicIndexing", 1);

     print_VkBool32(obj.shaderUniformTexelBufferArrayDynamicIndexing, "shaderUniformTexelBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj.shaderStorageTexelBufferArrayDynamicIndexing, "shaderStorageTexelBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj.shaderUniformBufferArrayNonUniformIndexing, "shaderUniformBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderSampledImageArrayNonUniformIndexing, "shaderSampledImageArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderStorageBufferArrayNonUniformIndexing, "shaderStorageBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderStorageImageArrayNonUniformIndexing, "shaderStorageImageArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderInputAttachmentArrayNonUniformIndexing, "shaderInputAttachmentArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderUniformTexelBufferArrayNonUniformIndexing, "shaderUniformTexelBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj.shaderStorageTexelBufferArrayNonUniformIndexing, "shaderStorageTexelBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj.descriptorBindingUniformBufferUpdateAfterBind, "descriptorBindingUniformBufferUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingSampledImageUpdateAfterBind, "descriptorBindingSampledImageUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingStorageImageUpdateAfterBind, "descriptorBindingStorageImageUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingStorageBufferUpdateAfterBind, "descriptorBindingStorageBufferUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingUniformTexelBufferUpdateAfterBind, "descriptorBindingUniformTexelBufferUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingStorageTexelBufferUpdateAfterBind, "descriptorBindingStorageTexelBufferUpdateAfterBind", 1);

     print_VkBool32(obj.descriptorBindingUpdateUnusedWhilePending, "descriptorBindingUpdateUnusedWhilePending", 1);

     print_VkBool32(obj.descriptorBindingPartiallyBound, "descriptorBindingPartiallyBound", 1);

     print_VkBool32(obj.descriptorBindingVariableDescriptorCount, "descriptorBindingVariableDescriptorCount", 1);

     print_VkBool32(obj.runtimeDescriptorArray, "runtimeDescriptorArray", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceDescriptorIndexingFeatures(const VkPhysicalDeviceDescriptorIndexingFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->shaderInputAttachmentArrayDynamicIndexing, "shaderInputAttachmentArrayDynamicIndexing", 1);

     print_VkBool32(obj->shaderUniformTexelBufferArrayDynamicIndexing, "shaderUniformTexelBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj->shaderStorageTexelBufferArrayDynamicIndexing, "shaderStorageTexelBufferArrayDynamicIndexing", 1);

     print_VkBool32(obj->shaderUniformBufferArrayNonUniformIndexing, "shaderUniformBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderSampledImageArrayNonUniformIndexing, "shaderSampledImageArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderStorageBufferArrayNonUniformIndexing, "shaderStorageBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderStorageImageArrayNonUniformIndexing, "shaderStorageImageArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderInputAttachmentArrayNonUniformIndexing, "shaderInputAttachmentArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderUniformTexelBufferArrayNonUniformIndexing, "shaderUniformTexelBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj->shaderStorageTexelBufferArrayNonUniformIndexing, "shaderStorageTexelBufferArrayNonUniformIndexing", 1);

     print_VkBool32(obj->descriptorBindingUniformBufferUpdateAfterBind, "descriptorBindingUniformBufferUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingSampledImageUpdateAfterBind, "descriptorBindingSampledImageUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingStorageImageUpdateAfterBind, "descriptorBindingStorageImageUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingStorageBufferUpdateAfterBind, "descriptorBindingStorageBufferUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingUniformTexelBufferUpdateAfterBind, "descriptorBindingUniformTexelBufferUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingStorageTexelBufferUpdateAfterBind, "descriptorBindingStorageTexelBufferUpdateAfterBind", 1);

     print_VkBool32(obj->descriptorBindingUpdateUnusedWhilePending, "descriptorBindingUpdateUnusedWhilePending", 1);

     print_VkBool32(obj->descriptorBindingPartiallyBound, "descriptorBindingPartiallyBound", 1);

     print_VkBool32(obj->descriptorBindingVariableDescriptorCount, "descriptorBindingVariableDescriptorCount", 1);

     print_VkBool32(obj->runtimeDescriptorArray, "runtimeDescriptorArray", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceDescriptorIndexingProperties(VkPhysicalDeviceDescriptorIndexingProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.maxUpdateAfterBindDescriptorsInAllPools, "maxUpdateAfterBindDescriptorsInAllPools", 1);

     print_VkBool32(obj.shaderUniformBufferArrayNonUniformIndexingNative, "shaderUniformBufferArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj.shaderSampledImageArrayNonUniformIndexingNative, "shaderSampledImageArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj.shaderStorageBufferArrayNonUniformIndexingNative, "shaderStorageBufferArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj.shaderStorageImageArrayNonUniformIndexingNative, "shaderStorageImageArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj.shaderInputAttachmentArrayNonUniformIndexingNative, "shaderInputAttachmentArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj.robustBufferAccessUpdateAfterBind, "robustBufferAccessUpdateAfterBind", 1);

     print_VkBool32(obj.quadDivergentImplicitLod, "quadDivergentImplicitLod", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindSamplers, "maxPerStageDescriptorUpdateAfterBindSamplers", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindUniformBuffers, "maxPerStageDescriptorUpdateAfterBindUniformBuffers", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindStorageBuffers, "maxPerStageDescriptorUpdateAfterBindStorageBuffers", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindSampledImages, "maxPerStageDescriptorUpdateAfterBindSampledImages", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindStorageImages, "maxPerStageDescriptorUpdateAfterBindStorageImages", 1);

     print_uint32_t(obj.maxPerStageDescriptorUpdateAfterBindInputAttachments, "maxPerStageDescriptorUpdateAfterBindInputAttachments", 1);

     print_uint32_t(obj.maxPerStageUpdateAfterBindResources, "maxPerStageUpdateAfterBindResources", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindSamplers, "maxDescriptorSetUpdateAfterBindSamplers", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindUniformBuffers, "maxDescriptorSetUpdateAfterBindUniformBuffers", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic, "maxDescriptorSetUpdateAfterBindUniformBuffersDynamic", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindStorageBuffers, "maxDescriptorSetUpdateAfterBindStorageBuffers", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic, "maxDescriptorSetUpdateAfterBindStorageBuffersDynamic", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindSampledImages, "maxDescriptorSetUpdateAfterBindSampledImages", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindStorageImages, "maxDescriptorSetUpdateAfterBindStorageImages", 1);

     print_uint32_t(obj.maxDescriptorSetUpdateAfterBindInputAttachments, "maxDescriptorSetUpdateAfterBindInputAttachments", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceDescriptorIndexingProperties(const VkPhysicalDeviceDescriptorIndexingProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->maxUpdateAfterBindDescriptorsInAllPools, "maxUpdateAfterBindDescriptorsInAllPools", 1);

     print_VkBool32(obj->shaderUniformBufferArrayNonUniformIndexingNative, "shaderUniformBufferArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj->shaderSampledImageArrayNonUniformIndexingNative, "shaderSampledImageArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj->shaderStorageBufferArrayNonUniformIndexingNative, "shaderStorageBufferArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj->shaderStorageImageArrayNonUniformIndexingNative, "shaderStorageImageArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj->shaderInputAttachmentArrayNonUniformIndexingNative, "shaderInputAttachmentArrayNonUniformIndexingNative", 1);

     print_VkBool32(obj->robustBufferAccessUpdateAfterBind, "robustBufferAccessUpdateAfterBind", 1);

     print_VkBool32(obj->quadDivergentImplicitLod, "quadDivergentImplicitLod", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindSamplers, "maxPerStageDescriptorUpdateAfterBindSamplers", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindUniformBuffers, "maxPerStageDescriptorUpdateAfterBindUniformBuffers", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindStorageBuffers, "maxPerStageDescriptorUpdateAfterBindStorageBuffers", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindSampledImages, "maxPerStageDescriptorUpdateAfterBindSampledImages", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindStorageImages, "maxPerStageDescriptorUpdateAfterBindStorageImages", 1);

     print_uint32_t(obj->maxPerStageDescriptorUpdateAfterBindInputAttachments, "maxPerStageDescriptorUpdateAfterBindInputAttachments", 1);

     print_uint32_t(obj->maxPerStageUpdateAfterBindResources, "maxPerStageUpdateAfterBindResources", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindSamplers, "maxDescriptorSetUpdateAfterBindSamplers", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindUniformBuffers, "maxDescriptorSetUpdateAfterBindUniformBuffers", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic, "maxDescriptorSetUpdateAfterBindUniformBuffersDynamic", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindStorageBuffers, "maxDescriptorSetUpdateAfterBindStorageBuffers", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic, "maxDescriptorSetUpdateAfterBindStorageBuffersDynamic", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindSampledImages, "maxDescriptorSetUpdateAfterBindSampledImages", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindStorageImages, "maxDescriptorSetUpdateAfterBindStorageImages", 1);

     print_uint32_t(obj->maxDescriptorSetUpdateAfterBindInputAttachments, "maxDescriptorSetUpdateAfterBindInputAttachments", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorSetVariableDescriptorCountAllocateInfo(VkDescriptorSetVariableDescriptorCountAllocateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.descriptorSetCount, "descriptorSetCount", 1);

     PRINT_SPACE
     _OUT << "\"pDescriptorCounts\":" << std::endl;
     PRINT_SPACE
     if (obj.pDescriptorCounts) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.descriptorSetCount; i++) {
           bool isCommaNeeded = (i+1) != obj.descriptorSetCount;
           print_uint32_t(obj.pDescriptorCounts[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorSetVariableDescriptorCountAllocateInfo(const VkDescriptorSetVariableDescriptorCountAllocateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->descriptorSetCount, "descriptorSetCount", 1);

     PRINT_SPACE
     _OUT << "\"pDescriptorCounts\":" << std::endl;
     PRINT_SPACE
     if (obj->pDescriptorCounts) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->descriptorSetCount; i++) {
           bool isCommaNeeded = (i+1) != obj->descriptorSetCount;
           print_uint32_t(obj->pDescriptorCounts[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDescriptorSetVariableDescriptorCountLayoutSupport(VkDescriptorSetVariableDescriptorCountLayoutSupport obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.maxVariableDescriptorCount, "maxVariableDescriptorCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDescriptorSetVariableDescriptorCountLayoutSupport(const VkDescriptorSetVariableDescriptorCountLayoutSupport * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->maxVariableDescriptorCount, "maxVariableDescriptorCount", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSubpassDescriptionDepthStencilResolve(VkSubpassDescriptionDepthStencilResolve obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkResolveModeFlagBits(obj.depthResolveMode, "depthResolveMode", 1);

     print_VkResolveModeFlagBits(obj.stencilResolveMode, "stencilResolveMode", 1);

     PRINT_SPACE
     _OUT << "\"pDepthStencilResolveAttachment\": " << std::endl;
     if (obj.pDepthStencilResolveAttachment) {
           print_VkAttachmentReference2(obj.pDepthStencilResolveAttachment, "pDepthStencilResolveAttachment", 0);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSubpassDescriptionDepthStencilResolve(const VkSubpassDescriptionDepthStencilResolve * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkResolveModeFlagBits(obj->depthResolveMode, "depthResolveMode", 1);

     print_VkResolveModeFlagBits(obj->stencilResolveMode, "stencilResolveMode", 1);

     PRINT_SPACE
     _OUT << "\"pDepthStencilResolveAttachment\": " << std::endl;
     if (obj->pDepthStencilResolveAttachment) {
           print_VkAttachmentReference2(obj->pDepthStencilResolveAttachment, "pDepthStencilResolveAttachment", 0);
     }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceDepthStencilResolveProperties(VkPhysicalDeviceDepthStencilResolveProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkResolveModeFlags(obj.supportedDepthResolveModes, "supportedDepthResolveModes", 1);

     print_VkResolveModeFlags(obj.supportedStencilResolveModes, "supportedStencilResolveModes", 1);

     print_VkBool32(obj.independentResolveNone, "independentResolveNone", 1);

     print_VkBool32(obj.independentResolve, "independentResolve", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceDepthStencilResolveProperties(const VkPhysicalDeviceDepthStencilResolveProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkResolveModeFlags(obj->supportedDepthResolveModes, "supportedDepthResolveModes", 1);

     print_VkResolveModeFlags(obj->supportedStencilResolveModes, "supportedStencilResolveModes", 1);

     print_VkBool32(obj->independentResolveNone, "independentResolveNone", 1);

     print_VkBool32(obj->independentResolve, "independentResolve", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceScalarBlockLayoutFeatures(VkPhysicalDeviceScalarBlockLayoutFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.scalarBlockLayout, "scalarBlockLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceScalarBlockLayoutFeatures(const VkPhysicalDeviceScalarBlockLayoutFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->scalarBlockLayout, "scalarBlockLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkImageStencilUsageCreateInfo(VkImageStencilUsageCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageUsageFlags(obj.stencilUsage, "stencilUsage", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkImageStencilUsageCreateInfo(const VkImageStencilUsageCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageUsageFlags(obj->stencilUsage, "stencilUsage", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSamplerReductionModeCreateInfo(VkSamplerReductionModeCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSamplerReductionMode(obj.reductionMode, "reductionMode", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSamplerReductionModeCreateInfo(const VkSamplerReductionModeCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSamplerReductionMode(obj->reductionMode, "reductionMode", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceSamplerFilterMinmaxProperties(VkPhysicalDeviceSamplerFilterMinmaxProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.filterMinmaxSingleComponentFormats, "filterMinmaxSingleComponentFormats", 1);

     print_VkBool32(obj.filterMinmaxImageComponentMapping, "filterMinmaxImageComponentMapping", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceSamplerFilterMinmaxProperties(const VkPhysicalDeviceSamplerFilterMinmaxProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->filterMinmaxSingleComponentFormats, "filterMinmaxSingleComponentFormats", 1);

     print_VkBool32(obj->filterMinmaxImageComponentMapping, "filterMinmaxImageComponentMapping", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceVulkanMemoryModelFeatures(VkPhysicalDeviceVulkanMemoryModelFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.vulkanMemoryModel, "vulkanMemoryModel", 1);

     print_VkBool32(obj.vulkanMemoryModelDeviceScope, "vulkanMemoryModelDeviceScope", 1);

     print_VkBool32(obj.vulkanMemoryModelAvailabilityVisibilityChains, "vulkanMemoryModelAvailabilityVisibilityChains", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceVulkanMemoryModelFeatures(const VkPhysicalDeviceVulkanMemoryModelFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->vulkanMemoryModel, "vulkanMemoryModel", 1);

     print_VkBool32(obj->vulkanMemoryModelDeviceScope, "vulkanMemoryModelDeviceScope", 1);

     print_VkBool32(obj->vulkanMemoryModelAvailabilityVisibilityChains, "vulkanMemoryModelAvailabilityVisibilityChains", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceImagelessFramebufferFeatures(VkPhysicalDeviceImagelessFramebufferFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.imagelessFramebuffer, "imagelessFramebuffer", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceImagelessFramebufferFeatures(const VkPhysicalDeviceImagelessFramebufferFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->imagelessFramebuffer, "imagelessFramebuffer", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkFramebufferAttachmentImageInfo(VkFramebufferAttachmentImageInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageCreateFlags(obj.flags, "flags", 1);

     print_VkImageUsageFlags(obj.usage, "usage", 1);

     print_uint32_t(obj.width, "width", 1);

     print_uint32_t(obj.height, "height", 1);

     print_uint32_t(obj.layerCount, "layerCount", 1);

     print_uint32_t(obj.viewFormatCount, "viewFormatCount", 1);

     PRINT_SPACE
     _OUT << "\"pViewFormats\":" << std::endl;
     PRINT_SPACE
     if (obj.pViewFormats) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.viewFormatCount; i++) {
           bool isCommaNeeded = (i+1) != obj.viewFormatCount;
           print_VkFormat(obj.pViewFormats[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkFramebufferAttachmentImageInfo(const VkFramebufferAttachmentImageInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageCreateFlags(obj->flags, "flags", 1);

     print_VkImageUsageFlags(obj->usage, "usage", 1);

     print_uint32_t(obj->width, "width", 1);

     print_uint32_t(obj->height, "height", 1);

     print_uint32_t(obj->layerCount, "layerCount", 1);

     print_uint32_t(obj->viewFormatCount, "viewFormatCount", 1);

     PRINT_SPACE
     _OUT << "\"pViewFormats\":" << std::endl;
     PRINT_SPACE
     if (obj->pViewFormats) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->viewFormatCount; i++) {
           bool isCommaNeeded = (i+1) != obj->viewFormatCount;
           print_VkFormat(obj->pViewFormats[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkFramebufferAttachmentsCreateInfo(VkFramebufferAttachmentsCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.attachmentImageInfoCount, "attachmentImageInfoCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachmentImageInfos\": " << std::endl;
     if (obj.pAttachmentImageInfos) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj.attachmentImageInfoCount; i++) {
           if (i+1 == obj.attachmentImageInfoCount)
               print_VkFramebufferAttachmentImageInfo(obj.pAttachmentImageInfos[i], "pAttachmentImageInfos", 0);
           else
               print_VkFramebufferAttachmentImageInfo(obj.pAttachmentImageInfos[i], "pAttachmentImageInfos", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkFramebufferAttachmentsCreateInfo(const VkFramebufferAttachmentsCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->attachmentImageInfoCount, "attachmentImageInfoCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachmentImageInfos\": " << std::endl;
     if (obj->pAttachmentImageInfos) {
         PRINT_SPACE
         _OUT << "[" << std::endl;
         for (unsigned int i = 0; i < obj->attachmentImageInfoCount; i++) {
           if (i+1 == obj->attachmentImageInfoCount)
               print_VkFramebufferAttachmentImageInfo(obj->pAttachmentImageInfos[i], "pAttachmentImageInfos", 0);
           else
               print_VkFramebufferAttachmentImageInfo(obj->pAttachmentImageInfos[i], "pAttachmentImageInfos", 1);
         }
         PRINT_SPACE
         _OUT << "]" << std::endl;
    }
     else
     {
         PRINT_SPACE _OUT << "\"NULL\""<< ""<< std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkRenderPassAttachmentBeginInfo(VkRenderPassAttachmentBeginInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.attachmentCount, "attachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachments\":" << std::endl;
     PRINT_SPACE
     if (obj.pAttachments) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.attachmentCount; i++) {
           std:: stringstream tmp;
           tmp << "pAttachments" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.attachmentCount;
           print_VkImageView(obj.pAttachments[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkRenderPassAttachmentBeginInfo(const VkRenderPassAttachmentBeginInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->attachmentCount, "attachmentCount", 1);

     PRINT_SPACE
     _OUT << "\"pAttachments\":" << std::endl;
     PRINT_SPACE
     if (obj->pAttachments) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->attachmentCount; i++) {
           std:: stringstream tmp;
           tmp << "pAttachments" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->attachmentCount;
           print_VkImageView(obj->pAttachments[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceUniformBufferStandardLayoutFeatures(VkPhysicalDeviceUniformBufferStandardLayoutFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.uniformBufferStandardLayout, "uniformBufferStandardLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceUniformBufferStandardLayoutFeatures(const VkPhysicalDeviceUniformBufferStandardLayoutFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->uniformBufferStandardLayout, "uniformBufferStandardLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures(VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.shaderSubgroupExtendedTypes, "shaderSubgroupExtendedTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures(const VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->shaderSubgroupExtendedTypes, "shaderSubgroupExtendedTypes", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures(VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.separateDepthStencilLayouts, "separateDepthStencilLayouts", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures(const VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->separateDepthStencilLayouts, "separateDepthStencilLayouts", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkAttachmentReferenceStencilLayout(VkAttachmentReferenceStencilLayout obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageLayout(obj.stencilLayout, "stencilLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkAttachmentReferenceStencilLayout(const VkAttachmentReferenceStencilLayout * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageLayout(obj->stencilLayout, "stencilLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkAttachmentDescriptionStencilLayout(VkAttachmentDescriptionStencilLayout obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageLayout(obj.stencilInitialLayout, "stencilInitialLayout", 1);

     print_VkImageLayout(obj.stencilFinalLayout, "stencilFinalLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkAttachmentDescriptionStencilLayout(const VkAttachmentDescriptionStencilLayout * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkImageLayout(obj->stencilInitialLayout, "stencilInitialLayout", 1);

     print_VkImageLayout(obj->stencilFinalLayout, "stencilFinalLayout", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceHostQueryResetFeatures(VkPhysicalDeviceHostQueryResetFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.hostQueryReset, "hostQueryReset", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceHostQueryResetFeatures(const VkPhysicalDeviceHostQueryResetFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->hostQueryReset, "hostQueryReset", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceTimelineSemaphoreFeatures(VkPhysicalDeviceTimelineSemaphoreFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.timelineSemaphore, "timelineSemaphore", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceTimelineSemaphoreFeatures(const VkPhysicalDeviceTimelineSemaphoreFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->timelineSemaphore, "timelineSemaphore", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceTimelineSemaphoreProperties(VkPhysicalDeviceTimelineSemaphoreProperties obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint64_t(obj.maxTimelineSemaphoreValueDifference, "maxTimelineSemaphoreValueDifference", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceTimelineSemaphoreProperties(const VkPhysicalDeviceTimelineSemaphoreProperties * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint64_t(obj->maxTimelineSemaphoreValueDifference, "maxTimelineSemaphoreValueDifference", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSemaphoreTypeCreateInfo(VkSemaphoreTypeCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSemaphoreType(obj.semaphoreType, "semaphoreType", 1);

     print_uint64_t(obj.initialValue, "initialValue", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSemaphoreTypeCreateInfo(const VkSemaphoreTypeCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSemaphoreType(obj->semaphoreType, "semaphoreType", 1);

     print_uint64_t(obj->initialValue, "initialValue", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkTimelineSemaphoreSubmitInfo(VkTimelineSemaphoreSubmitInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj.waitSemaphoreValueCount, "waitSemaphoreValueCount", 1);

     PRINT_SPACE
     _OUT << "\"pWaitSemaphoreValues\":" << std::endl;
     PRINT_SPACE
     if (obj.pWaitSemaphoreValues) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.waitSemaphoreValueCount; i++) {
           bool isCommaNeeded = (i+1) != obj.waitSemaphoreValueCount;
           print_uint64_t(obj.pWaitSemaphoreValues[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj.signalSemaphoreValueCount, "signalSemaphoreValueCount", 1);

     PRINT_SPACE
     _OUT << "\"pSignalSemaphoreValues\":" << std::endl;
     PRINT_SPACE
     if (obj.pSignalSemaphoreValues) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.signalSemaphoreValueCount; i++) {
           bool isCommaNeeded = (i+1) != obj.signalSemaphoreValueCount;
           print_uint64_t(obj.pSignalSemaphoreValues[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkTimelineSemaphoreSubmitInfo(const VkTimelineSemaphoreSubmitInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint32_t(obj->waitSemaphoreValueCount, "waitSemaphoreValueCount", 1);

     PRINT_SPACE
     _OUT << "\"pWaitSemaphoreValues\":" << std::endl;
     PRINT_SPACE
     if (obj->pWaitSemaphoreValues) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->waitSemaphoreValueCount; i++) {
           bool isCommaNeeded = (i+1) != obj->waitSemaphoreValueCount;
           print_uint64_t(obj->pWaitSemaphoreValues[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     print_uint32_t(obj->signalSemaphoreValueCount, "signalSemaphoreValueCount", 1);

     PRINT_SPACE
     _OUT << "\"pSignalSemaphoreValues\":" << std::endl;
     PRINT_SPACE
     if (obj->pSignalSemaphoreValues) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->signalSemaphoreValueCount; i++) {
           bool isCommaNeeded = (i+1) != obj->signalSemaphoreValueCount;
           print_uint64_t(obj->pSignalSemaphoreValues[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSemaphoreWaitInfo(VkSemaphoreWaitInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSemaphoreWaitFlags(obj.flags, "flags", 1);

     print_uint32_t(obj.semaphoreCount, "semaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pSemaphores\":" << std::endl;
     PRINT_SPACE
     if (obj.pSemaphores) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.semaphoreCount; i++) {
           std:: stringstream tmp;
           tmp << "pSemaphores" << "_" << i;
           bool isCommaNeeded = (i+1) != obj.semaphoreCount;
           print_VkSemaphore(obj.pSemaphores[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pValues\":" << std::endl;
     PRINT_SPACE
     if (obj.pValues) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj.semaphoreCount; i++) {
           bool isCommaNeeded = (i+1) != obj.semaphoreCount;
           print_uint64_t(obj.pValues[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSemaphoreWaitInfo(const VkSemaphoreWaitInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkSemaphoreWaitFlags(obj->flags, "flags", 1);

     print_uint32_t(obj->semaphoreCount, "semaphoreCount", 1);

     PRINT_SPACE
     _OUT << "\"pSemaphores\":" << std::endl;
     PRINT_SPACE
     if (obj->pSemaphores) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->semaphoreCount; i++) {
           std:: stringstream tmp;
           tmp << "pSemaphores" << "_" << i;
           bool isCommaNeeded = (i+1) != obj->semaphoreCount;
           print_VkSemaphore(obj->pSemaphores[i], tmp.str(), isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "," << std::endl;
     } else {
       _OUT << "\"NULL\"" << "," << std::endl;
     }

     PRINT_SPACE
     _OUT << "\"pValues\":" << std::endl;
     PRINT_SPACE
     if (obj->pValues) {
       _OUT << "[" << std::endl;
       for (unsigned int i = 0; i < obj->semaphoreCount; i++) {
           bool isCommaNeeded = (i+1) != obj->semaphoreCount;
           print_uint64_t(obj->pValues[i], "", isCommaNeeded);
       }
       PRINT_SPACE
       _OUT << "]" << "" << std::endl;
     } else {
       _OUT << "\"NULL\"" << "" << std::endl;
     }

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkSemaphoreSignalInfo(VkSemaphoreSignalInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "semaphore" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint64_t(obj.value, "value", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkSemaphoreSignalInfo(const VkSemaphoreSignalInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "semaphore" << "\"" << " : " << "\"" << "\"," << std::endl;

     print_uint64_t(obj->value, "value", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkPhysicalDeviceBufferDeviceAddressFeatures(VkPhysicalDeviceBufferDeviceAddressFeatures obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj.bufferDeviceAddress, "bufferDeviceAddress", 1);

     print_VkBool32(obj.bufferDeviceAddressCaptureReplay, "bufferDeviceAddressCaptureReplay", 1);

     print_VkBool32(obj.bufferDeviceAddressMultiDevice, "bufferDeviceAddressMultiDevice", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkPhysicalDeviceBufferDeviceAddressFeatures(const VkPhysicalDeviceBufferDeviceAddressFeatures * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_VkBool32(obj->bufferDeviceAddress, "bufferDeviceAddress", 1);

     print_VkBool32(obj->bufferDeviceAddressCaptureReplay, "bufferDeviceAddressCaptureReplay", 1);

     print_VkBool32(obj->bufferDeviceAddressMultiDevice, "bufferDeviceAddressMultiDevice", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBufferDeviceAddressInfo(VkBufferDeviceAddressInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBufferDeviceAddressInfo(const VkBufferDeviceAddressInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "buffer" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkBufferOpaqueCaptureAddressCreateInfo(VkBufferOpaqueCaptureAddressCreateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint64_t(obj.opaqueCaptureAddress, "opaqueCaptureAddress", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkBufferOpaqueCaptureAddressCreateInfo(const VkBufferOpaqueCaptureAddressCreateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint64_t(obj->opaqueCaptureAddress, "opaqueCaptureAddress", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkMemoryOpaqueCaptureAddressAllocateInfo(VkMemoryOpaqueCaptureAddressAllocateInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint64_t(obj.opaqueCaptureAddress, "opaqueCaptureAddress", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkMemoryOpaqueCaptureAddressAllocateInfo(const VkMemoryOpaqueCaptureAddressAllocateInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     print_uint64_t(obj->opaqueCaptureAddress, "opaqueCaptureAddress", 0);

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

static void print_VkDeviceMemoryOpaqueCaptureAddressInfo(VkDeviceMemoryOpaqueCaptureAddressInfo obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj.sType, "sType", 1);

      if (obj.pNext) {
         dumpPNextChain(obj.pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}
static void print_VkDeviceMemoryOpaqueCaptureAddressInfo(const VkDeviceMemoryOpaqueCaptureAddressInfo * obj, const std::string& s, bool commaNeeded=true) {
     PRINT_SPACE
     _OUT << "{" << std::endl;
     INDENT(4);

     print_VkStructureType(obj->sType, "sType", 1);

      if (obj->pNext) {
         dumpPNextChain(obj->pNext);
      } else {
         PRINT_SPACE
         _OUT << "\"pNext\":" << "\"NULL\""<< ","<< std::endl;
     }

     /** Note: printing just an empty entry here **/
     PRINT_SPACE    _OUT << "\"" << "memory" << "\"" << " : " << "\"" << "\"" << std::endl;

     INDENT(-4);
     PRINT_SPACE
     if (commaNeeded)
         _OUT << "}," << std::endl;
     else
         _OUT << "}" << std::endl;
}

/*************************************** Begin prototypes ***********************************/
/*************************************** End prototypes ***********************************/

static void dumpPNextChain(const void* pNext) {
      VkBaseInStructure *pBase = (VkBaseInStructure*)pNext;
      if (pNext) {
           PRINT_SPACE
           _OUT << "\"pNext\":"<< std::endl;

          switch (pBase->sType) {
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2:print_VkPhysicalDeviceFeatures2((VkPhysicalDeviceFeatures2 *) pNext, "VkPhysicalDeviceFeatures2", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES:print_VkPhysicalDeviceDriverProperties((VkPhysicalDeviceDriverProperties *) pNext, "VkPhysicalDeviceDriverProperties", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES:print_VkPhysicalDeviceVariablePointersFeatures((VkPhysicalDeviceVariablePointersFeatures *) pNext, "VkPhysicalDeviceVariablePointersFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_IMAGE_FORMAT_INFO:print_VkPhysicalDeviceExternalImageFormatInfo((VkPhysicalDeviceExternalImageFormatInfo *) pNext, "VkPhysicalDeviceExternalImageFormatInfo", true);
             break;
             case VK_STRUCTURE_TYPE_EXTERNAL_IMAGE_FORMAT_PROPERTIES:print_VkExternalImageFormatProperties((VkExternalImageFormatProperties *) pNext, "VkExternalImageFormatProperties", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES:print_VkPhysicalDeviceIDProperties((VkPhysicalDeviceIDProperties *) pNext, "VkPhysicalDeviceIDProperties", true);
             break;
             case VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO:print_VkExternalMemoryImageCreateInfo((VkExternalMemoryImageCreateInfo *) pNext, "VkExternalMemoryImageCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO:print_VkExternalMemoryBufferCreateInfo((VkExternalMemoryBufferCreateInfo *) pNext, "VkExternalMemoryBufferCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO:print_VkExportMemoryAllocateInfo((VkExportMemoryAllocateInfo *) pNext, "VkExportMemoryAllocateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO:print_VkExportSemaphoreCreateInfo((VkExportSemaphoreCreateInfo *) pNext, "VkExportSemaphoreCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_EXPORT_FENCE_CREATE_INFO:print_VkExportFenceCreateInfo((VkExportFenceCreateInfo *) pNext, "VkExportFenceCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES:print_VkPhysicalDeviceMultiviewFeatures((VkPhysicalDeviceMultiviewFeatures *) pNext, "VkPhysicalDeviceMultiviewFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES:print_VkPhysicalDeviceMultiviewProperties((VkPhysicalDeviceMultiviewProperties *) pNext, "VkPhysicalDeviceMultiviewProperties", true);
             break;
             case VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO:print_VkRenderPassMultiviewCreateInfo((VkRenderPassMultiviewCreateInfo *) pNext, "VkRenderPassMultiviewCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO:print_VkMemoryAllocateFlagsInfo((VkMemoryAllocateFlagsInfo *) pNext, "VkMemoryAllocateFlagsInfo", true);
             break;
             case VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_DEVICE_GROUP_INFO:print_VkBindBufferMemoryDeviceGroupInfo((VkBindBufferMemoryDeviceGroupInfo *) pNext, "VkBindBufferMemoryDeviceGroupInfo", true);
             break;
             case VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_DEVICE_GROUP_INFO:print_VkBindImageMemoryDeviceGroupInfo((VkBindImageMemoryDeviceGroupInfo *) pNext, "VkBindImageMemoryDeviceGroupInfo", true);
             break;
             case VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO:print_VkDeviceGroupRenderPassBeginInfo((VkDeviceGroupRenderPassBeginInfo *) pNext, "VkDeviceGroupRenderPassBeginInfo", true);
             break;
             case VK_STRUCTURE_TYPE_DEVICE_GROUP_COMMAND_BUFFER_BEGIN_INFO:print_VkDeviceGroupCommandBufferBeginInfo((VkDeviceGroupCommandBufferBeginInfo *) pNext, "VkDeviceGroupCommandBufferBeginInfo", true);
             break;
             case VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO:print_VkDeviceGroupSubmitInfo((VkDeviceGroupSubmitInfo *) pNext, "VkDeviceGroupSubmitInfo", true);
             break;
             case VK_STRUCTURE_TYPE_DEVICE_GROUP_BIND_SPARSE_INFO:print_VkDeviceGroupBindSparseInfo((VkDeviceGroupBindSparseInfo *) pNext, "VkDeviceGroupBindSparseInfo", true);
             break;
             case VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO:print_VkDeviceGroupDeviceCreateInfo((VkDeviceGroupDeviceCreateInfo *) pNext, "VkDeviceGroupDeviceCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_RENDER_PASS_INPUT_ATTACHMENT_ASPECT_CREATE_INFO:print_VkRenderPassInputAttachmentAspectCreateInfo((VkRenderPassInputAttachmentAspectCreateInfo *) pNext, "VkRenderPassInputAttachmentAspectCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES:print_VkPhysicalDevice16BitStorageFeatures((VkPhysicalDevice16BitStorageFeatures *) pNext, "VkPhysicalDevice16BitStorageFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES:print_VkPhysicalDeviceSubgroupProperties((VkPhysicalDeviceSubgroupProperties *) pNext, "VkPhysicalDeviceSubgroupProperties", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES:print_VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures((VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures *) pNext, "VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES:print_VkPhysicalDevicePointClippingProperties((VkPhysicalDevicePointClippingProperties *) pNext, "VkPhysicalDevicePointClippingProperties", true);
             break;
             case VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS:print_VkMemoryDedicatedRequirements((VkMemoryDedicatedRequirements *) pNext, "VkMemoryDedicatedRequirements", true);
             break;
             case VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO:print_VkMemoryDedicatedAllocateInfo((VkMemoryDedicatedAllocateInfo *) pNext, "VkMemoryDedicatedAllocateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO:print_VkImageViewUsageCreateInfo((VkImageViewUsageCreateInfo *) pNext, "VkImageViewUsageCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_DOMAIN_ORIGIN_STATE_CREATE_INFO:print_VkPipelineTessellationDomainOriginStateCreateInfo((VkPipelineTessellationDomainOriginStateCreateInfo *) pNext, "VkPipelineTessellationDomainOriginStateCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_INFO:print_VkSamplerYcbcrConversionInfo((VkSamplerYcbcrConversionInfo *) pNext, "VkSamplerYcbcrConversionInfo", true);
             break;
             case VK_STRUCTURE_TYPE_BIND_IMAGE_PLANE_MEMORY_INFO:print_VkBindImagePlaneMemoryInfo((VkBindImagePlaneMemoryInfo *) pNext, "VkBindImagePlaneMemoryInfo", true);
             break;
             case VK_STRUCTURE_TYPE_IMAGE_PLANE_MEMORY_REQUIREMENTS_INFO:print_VkImagePlaneMemoryRequirementsInfo((VkImagePlaneMemoryRequirementsInfo *) pNext, "VkImagePlaneMemoryRequirementsInfo", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES:print_VkPhysicalDeviceSamplerYcbcrConversionFeatures((VkPhysicalDeviceSamplerYcbcrConversionFeatures *) pNext, "VkPhysicalDeviceSamplerYcbcrConversionFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_IMAGE_FORMAT_PROPERTIES:print_VkSamplerYcbcrConversionImageFormatProperties((VkSamplerYcbcrConversionImageFormatProperties *) pNext, "VkSamplerYcbcrConversionImageFormatProperties", true);
             break;
             case VK_STRUCTURE_TYPE_PROTECTED_SUBMIT_INFO:print_VkProtectedSubmitInfo((VkProtectedSubmitInfo *) pNext, "VkProtectedSubmitInfo", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_FEATURES:print_VkPhysicalDeviceProtectedMemoryFeatures((VkPhysicalDeviceProtectedMemoryFeatures *) pNext, "VkPhysicalDeviceProtectedMemoryFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_PROPERTIES:print_VkPhysicalDeviceProtectedMemoryProperties((VkPhysicalDeviceProtectedMemoryProperties *) pNext, "VkPhysicalDeviceProtectedMemoryProperties", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_FILTER_MINMAX_PROPERTIES:print_VkPhysicalDeviceSamplerFilterMinmaxProperties((VkPhysicalDeviceSamplerFilterMinmaxProperties *) pNext, "VkPhysicalDeviceSamplerFilterMinmaxProperties", true);
             break;
             case VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO:print_VkSamplerReductionModeCreateInfo((VkSamplerReductionModeCreateInfo *) pNext, "VkSamplerReductionModeCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_IMAGE_FORMAT_LIST_CREATE_INFO:print_VkImageFormatListCreateInfo((VkImageFormatListCreateInfo *) pNext, "VkImageFormatListCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES:print_VkPhysicalDeviceMaintenance3Properties((VkPhysicalDeviceMaintenance3Properties *) pNext, "VkPhysicalDeviceMaintenance3Properties", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES:print_VkPhysicalDeviceShaderDrawParametersFeatures((VkPhysicalDeviceShaderDrawParametersFeatures *) pNext, "VkPhysicalDeviceShaderDrawParametersFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES:print_VkPhysicalDeviceShaderFloat16Int8Features((VkPhysicalDeviceShaderFloat16Int8Features *) pNext, "VkPhysicalDeviceShaderFloat16Int8Features", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES:print_VkPhysicalDeviceFloatControlsProperties((VkPhysicalDeviceFloatControlsProperties *) pNext, "VkPhysicalDeviceFloatControlsProperties", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES:print_VkPhysicalDeviceHostQueryResetFeatures((VkPhysicalDeviceHostQueryResetFeatures *) pNext, "VkPhysicalDeviceHostQueryResetFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES:print_VkPhysicalDeviceDescriptorIndexingFeatures((VkPhysicalDeviceDescriptorIndexingFeatures *) pNext, "VkPhysicalDeviceDescriptorIndexingFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES:print_VkPhysicalDeviceDescriptorIndexingProperties((VkPhysicalDeviceDescriptorIndexingProperties *) pNext, "VkPhysicalDeviceDescriptorIndexingProperties", true);
             break;
             case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO:print_VkDescriptorSetLayoutBindingFlagsCreateInfo((VkDescriptorSetLayoutBindingFlagsCreateInfo *) pNext, "VkDescriptorSetLayoutBindingFlagsCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO:print_VkDescriptorSetVariableDescriptorCountAllocateInfo((VkDescriptorSetVariableDescriptorCountAllocateInfo *) pNext, "VkDescriptorSetVariableDescriptorCountAllocateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_LAYOUT_SUPPORT:print_VkDescriptorSetVariableDescriptorCountLayoutSupport((VkDescriptorSetVariableDescriptorCountLayoutSupport *) pNext, "VkDescriptorSetVariableDescriptorCountLayoutSupport", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES:print_VkPhysicalDeviceTimelineSemaphoreFeatures((VkPhysicalDeviceTimelineSemaphoreFeatures *) pNext, "VkPhysicalDeviceTimelineSemaphoreFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES:print_VkPhysicalDeviceTimelineSemaphoreProperties((VkPhysicalDeviceTimelineSemaphoreProperties *) pNext, "VkPhysicalDeviceTimelineSemaphoreProperties", true);
             break;
             case VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO:print_VkSemaphoreTypeCreateInfo((VkSemaphoreTypeCreateInfo *) pNext, "VkSemaphoreTypeCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO:print_VkTimelineSemaphoreSubmitInfo((VkTimelineSemaphoreSubmitInfo *) pNext, "VkTimelineSemaphoreSubmitInfo", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES:print_VkPhysicalDevice8BitStorageFeatures((VkPhysicalDevice8BitStorageFeatures *) pNext, "VkPhysicalDevice8BitStorageFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES:print_VkPhysicalDeviceVulkanMemoryModelFeatures((VkPhysicalDeviceVulkanMemoryModelFeatures *) pNext, "VkPhysicalDeviceVulkanMemoryModelFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES:print_VkPhysicalDeviceShaderAtomicInt64Features((VkPhysicalDeviceShaderAtomicInt64Features *) pNext, "VkPhysicalDeviceShaderAtomicInt64Features", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES:print_VkPhysicalDeviceDepthStencilResolveProperties((VkPhysicalDeviceDepthStencilResolveProperties *) pNext, "VkPhysicalDeviceDepthStencilResolveProperties", true);
             break;
             case VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_DEPTH_STENCIL_RESOLVE:print_VkSubpassDescriptionDepthStencilResolve((VkSubpassDescriptionDepthStencilResolve *) pNext, "VkSubpassDescriptionDepthStencilResolve", true);
             break;
             case VK_STRUCTURE_TYPE_IMAGE_STENCIL_USAGE_CREATE_INFO:print_VkImageStencilUsageCreateInfo((VkImageStencilUsageCreateInfo *) pNext, "VkImageStencilUsageCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES:print_VkPhysicalDeviceScalarBlockLayoutFeatures((VkPhysicalDeviceScalarBlockLayoutFeatures *) pNext, "VkPhysicalDeviceScalarBlockLayoutFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES:print_VkPhysicalDeviceUniformBufferStandardLayoutFeatures((VkPhysicalDeviceUniformBufferStandardLayoutFeatures *) pNext, "VkPhysicalDeviceUniformBufferStandardLayoutFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES:print_VkPhysicalDeviceBufferDeviceAddressFeatures((VkPhysicalDeviceBufferDeviceAddressFeatures *) pNext, "VkPhysicalDeviceBufferDeviceAddressFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_BUFFER_OPAQUE_CAPTURE_ADDRESS_CREATE_INFO:print_VkBufferOpaqueCaptureAddressCreateInfo((VkBufferOpaqueCaptureAddressCreateInfo *) pNext, "VkBufferOpaqueCaptureAddressCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES:print_VkPhysicalDeviceImagelessFramebufferFeatures((VkPhysicalDeviceImagelessFramebufferFeatures *) pNext, "VkPhysicalDeviceImagelessFramebufferFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO:print_VkFramebufferAttachmentsCreateInfo((VkFramebufferAttachmentsCreateInfo *) pNext, "VkFramebufferAttachmentsCreateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO:print_VkRenderPassAttachmentBeginInfo((VkRenderPassAttachmentBeginInfo *) pNext, "VkRenderPassAttachmentBeginInfo", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES:print_VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures((VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures *) pNext, "VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures", true);
             break;
             case VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_STENCIL_LAYOUT:print_VkAttachmentReferenceStencilLayout((VkAttachmentReferenceStencilLayout *) pNext, "VkAttachmentReferenceStencilLayout", true);
             break;
             case VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_STENCIL_LAYOUT:print_VkAttachmentDescriptionStencilLayout((VkAttachmentDescriptionStencilLayout *) pNext, "VkAttachmentDescriptionStencilLayout", true);
             break;
             case VK_STRUCTURE_TYPE_MEMORY_OPAQUE_CAPTURE_ADDRESS_ALLOCATE_INFO:print_VkMemoryOpaqueCaptureAddressAllocateInfo((VkMemoryOpaqueCaptureAddressAllocateInfo *) pNext, "VkMemoryOpaqueCaptureAddressAllocateInfo", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES:print_VkPhysicalDeviceVulkan11Features((VkPhysicalDeviceVulkan11Features *) pNext, "VkPhysicalDeviceVulkan11Features", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES:print_VkPhysicalDeviceVulkan11Properties((VkPhysicalDeviceVulkan11Properties *) pNext, "VkPhysicalDeviceVulkan11Properties", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES:print_VkPhysicalDeviceVulkan12Features((VkPhysicalDeviceVulkan12Features *) pNext, "VkPhysicalDeviceVulkan12Features", true);
             break;
             case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES:print_VkPhysicalDeviceVulkan12Properties((VkPhysicalDeviceVulkan12Properties *) pNext, "VkPhysicalDeviceVulkan12Properties", true);
             break;
             default: assert(false); // No structure type matching
         }
     }
  }

}//End of namespace vk_json

#endif // _VULKAN_JSON_DATA_HPP