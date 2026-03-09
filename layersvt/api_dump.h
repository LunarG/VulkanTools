/* Copyright (c) 2015-2026 The Khronos Group Inc.
 * Copyright (c) 2015-2026 Valve Corporation
 * Copyright (c) 2015-2026 LunarG, Inc.
 * Copyright (C) 2015-2016 Google Inc.
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
 * Author: Lenny Komow <lenny@lunarg.com>
 * Author: Shannon McPherson <shannon@lunarg.com>
 * Author: David Pinedo <david@lunarg.com>
 * Author: Charles Giessen <charles@lunarg.com>
 */

#pragma once

#define VK_NO_PROTOTYPES

#include "vulkan/vk_layer.h"
#include "vk_layer_table.h"
#include <vulkan/utility/vk_dispatch_table.h>

#include <vulkan/layer/vk_layer_settings.hpp>

// Include the video headers so we can print types that come from them
#include "vk_video/vulkan_video_codecs_common.h"
#include "vk_video/vulkan_video_codec_h264std.h"
#include "vk_video/vulkan_video_codec_h264std_decode.h"
#include "vk_video/vulkan_video_codec_h264std_encode.h"
#include "vk_video/vulkan_video_codec_h265std.h"
#include "vk_video/vulkan_video_codec_h265std_decode.h"
#include "vk_video/vulkan_video_codec_h265std_encode.h"
#include "vk_video/vulkan_video_codec_av1std.h"
#include "vk_video/vulkan_video_codec_av1std_decode.h"

#include <string.h>
#include <stdint.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <mutex>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <map>
#include <set>
#include <thread>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <utility>

#if defined(_WIN32) && !defined(NDEBUG)
#include <crtdbg.h>
#endif

#ifdef ANDROID
#include <memory>
#include <string_view>

#include <android/log.h>
#include <sys/system_properties.h>
// Disable warning about bitshift precedence
#pragma GCC diagnostic ignored "-Wshift-op-parentheses"

#endif  // ANDROID

#if defined(__GNUC__) && __GNUC__ >= 4
#define EXPORT_FUNCTION __attribute__((visibility("default")))
#elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590)
#define EXPORT_FUNCTION __attribute__((visibility("default")))
#else
#define EXPORT_FUNCTION
#endif

#if defined(WIN32)
// Disable warning about bitshift precedence
#pragma warning(disable : 4554)
#endif

extern "C" {
// Forward declarations for dispatch
EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char *pName);
EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char *pName);
}

#define MAX_STRING_LENGTH 1024

// Defines for utilized environment variables.
#define kSettingsKeyFile "file"
#define kSettingsKeyLogFilename "log_filename"
#define kSettingsKeyOutputFormat "output_format"
#define kSettingsKeyDetailedOutput "detailed"
#define kSettingsKeyNoAddr "no_addr"
#define kSettingsKeyFlush "flush"
#define kSettingsKeyPreDump "pre_dump"
#define kSettingsKeyOutputRange "output_range"
#define kSettingsKeyTimestamp "timestamp"
#define kSettingsKeyIndentSize "indent_size"
#define kSettingsKeyShowTypes "show_types"
#define kSettingsKeyNameSize "name_size"
#define kSettingsKeyTypeSize "type_size"
#define kSettingsKeyUseSpaces "use_spaces"
#define kSettingsKeyShowShader "show_shader"
#define kSettingsKeyShowThreadAndFrame "show_thread_and_frame"

// We want to dump all extensions even beta extensions.
#ifndef VK_ENABLE_BETA_EXTENSIONS
#error "VK_ENABLE_BETA_EXTENSIONS not defined!"
#endif

// Ensure we are properly setting VK_USE_PLATFORM_METAL_EXT, VK_USE_PLATFORM_IOS_MVK, and VK_USE_PLATFORM_MACOS_MVK.
#if __APPLE__

// TODO: Add Metal support
// #ifndef VK_USE_PLATFORM_METAL_EXT
// #error "VK_USE_PLATFORM_METAL_EXT not defined!"
// #endif

#include <TargetConditionals.h>

#if TARGET_OS_IOS

#ifndef VK_USE_PLATFORM_IOS_MVK
#error "VK_USE_PLATFORM_IOS_MVK not defined!"
#endif

#endif  //  TARGET_OS_IOS

#if TARGET_OS_OSX

#ifndef VK_USE_PLATFORM_MACOS_MVK
#error "VK_USE_PLATFORM_MACOS_MVK not defined!"
#endif

#endif  // TARGET_OS_OSX

#endif  // __APPLE__

#if defined(VK_USE_64_BIT_PTR_DEFINES) && VK_USE_64_BIT_PTR_DEFINES == 1
#define TYPE_ERASE_HANDLE(handle) static_cast<void *>(handle)
#else
#define TYPE_ERASE_HANDLE(handle) static_cast<uint64_t>(handle)
#endif

enum class ApiDumpFormat {
    Text,
    Html,
    Json,
};

enum class OutputConstruct {
    pointer,
    value,
    api_struct,
    api_union,
};

static const uint64_t OUTPUT_RANGE_UNLIMITED = 0;
static const uint64_t OUTPUT_RANGE_INTERVAL_DEFAULT = 1;

struct FrameRange {
    uint64_t start_frame;  // The range begins on this frame, inclusive.
    uint64_t frame_count;  // If value is OUTPUT_RANGE_UNLIMITED, dump continues without limit.
    uint64_t interval;     // Rate at which frames are dumped. A value of 3 will dump every third frame.
};

class ConditionalFrameOutput {
    bool use_conditional_output = false;
    std::set<uint64_t> frames;
    std::vector<FrameRange> ranges;

    struct NumberToken {
        uint64_t value;
        uint32_t length;
    };

    NumberToken parseNumber(std::string str, uint32_t current_char) {
        uint32_t length = 0;
        while (current_char + length < str.size() && str[current_char + length] >= '0' && str[current_char + length] <= '9') {
            length++;
        }
        if (length > 0) {
            uint64_t value = std::atol(&str[current_char]);
            return NumberToken{value, length};
        } else {
            return NumberToken{0, 0};
        }
    }

    void printErrorMsg(const char *msg) {
#ifdef ANDROID
        __android_log_print(ANDROID_LOG_DEBUG, "api_dump", "%s", msg);
#else
        fprintf(stderr, "%s", msg);
#endif
    }

   public:
    /* Parses a string for a comma seperated list of frames & frame ranges
     * where frames are singular integers and frame ranges are of the following
     * format: "S-C-I" with S is the start frame, C is the count of frames to dump,
     * and I the interval between dumped frames.
     * Valid range strings: "2,3,5", "4-4-2", "3-6, 10-2"
     */
    bool parseConditionalFrameRange(std::string range_str) {
        uint32_t current_char = 0;

        if (range_str.empty()) {
            printErrorMsg("Conditional range error: format string was empty\n");
            return false;
        }

        while (current_char < range_str.size()) {
            NumberToken frame_number = parseNumber(range_str, current_char);
            if (frame_number.length <= 0) {
                printErrorMsg("Conditional range error: Invalid frame number\n");
                return false;
            }
            current_char += frame_number.length;

            // Range of frames
            if (range_str[current_char] == '-') {
                current_char++;
                if (current_char >= range_str.size()) {
                    printErrorMsg("Conditional range error: Must have number for frame count\n");
                    return false;
                }
                NumberToken frame_count = parseNumber(range_str, current_char);
                if (frame_count.length <= 0) {
                    printErrorMsg("Conditional range error: Invalid frame count\n");
                    return false;
                }
                current_char += frame_count.length;

                if (current_char >= range_str.size()) {
                    // Frame Range w/o interval
                    ranges.push_back(FrameRange{frame_number.value, frame_count.value, 1l});
                    use_conditional_output = true;
                    return true;
                }

                else if (range_str[current_char] == '-') {
                    current_char++;
                    if (current_char >= range_str.size()) {
                        printErrorMsg("Conditional range error: Must have number for frame interval \n");
                        return false;
                    }

                    NumberToken frame_interval = parseNumber(range_str, current_char);
                    if (frame_interval.length <= 0) {
                        printErrorMsg("Conditional range error: Invalid interval\n");
                        return false;
                    }

                    // Frame Range w/ interval
                    ranges.push_back(FrameRange{frame_number.value, frame_count.value, frame_interval.value});

                    current_char += frame_interval.length;
                    if (current_char >= range_str.size()) {
                        use_conditional_output = true;
                        return true;
                    }
                } else {
                    // Frame Range w/o interval
                    ranges.push_back(FrameRange{frame_number.value, frame_count.value, 1l});
                }
            } else {
                // Single frame capture
                frames.insert(frame_number.value);
            }
            if (range_str[current_char] == ',') {
                current_char++;
            }
        }
        use_conditional_output = true;
        return true;
    }

    // Return true if either use_conditional_output is false or if frame_count is within
    // the provided frame ranges
    bool isFrameInRange(uint64_t frame_number) const {
        if (!use_conditional_output) return true;
        for (auto &range : ranges) {
            if (range.start_frame <= frame_number) {
                if (range.frame_count == OUTPUT_RANGE_UNLIMITED) {
                    if (range.interval == OUTPUT_RANGE_INTERVAL_DEFAULT) {
                        return true;
                    } else {
                        return (frame_number - range.start_frame) % range.interval == 0;
                    }

                } else if (range.start_frame + range.frame_count > frame_number) {
                    if (range.interval == OUTPUT_RANGE_INTERVAL_DEFAULT) {
                        return true;
                    } else {
                        return (frame_number - range.start_frame) % range.interval == 0;
                    }
                }
            }
        }
        if (frames.count(frame_number) > 0) {
            return true;
        }
        return false;
    }
};

#ifdef __ANDROID__
template <class char_type = char, class traits = std::char_traits<char_type>>
class AndroidLogcatBuf final : public std::basic_streambuf<char_type, traits> {
   public:
    class LogWriter {
       public:
        virtual void write(const std::basic_string<char_type, traits> &data) = 0;
        virtual ~LogWriter() {}
    };

    // bufsize should be smaller than 0x400, because `__android_log_print` uses `vsnprintf` with a 1024 buffer size. If bufsize here
    // is greater or close to 1024, the log will be truncated.
    AndroidLogcatBuf(std::unique_ptr<LogWriter> log_writer, size_t bufsize = 0x200)
        : buffer_(std::make_unique<char_type[]>(bufsize)), bufsize_(bufsize), log_writer_(std::move(log_writer)) {
        // We use the last position of buffer_ to store the overflow character. We use 0 as a sentinel element to indicate if this
        // overflow slot is in use.
        this->setp(buffer_.get(), buffer_.get() + bufsize_ - 1);
    }
    ~AndroidLogcatBuf() = default;

   private:
    using int_type = typename traits::int_type;
    int_type overflow(int_type c) override {
        // Conform with the C++ standard. Return not eof when called with eof:
        // https://en.cppreference.com/w/cpp/io/basic_stringbuf/overflow.
        if (c == traits::eof()) {
            return traits::not_eof(c);
        }
        buffer_[bufsize_ - 1] = traits::to_char_type(c);
        flushPending();
        buffer_[bufsize_ - 1] = traits::to_char_type(0);
        return traits::not_eof(c);
    }

    int sync() override {
        flushPending();
        if (!pending_content_.empty()) {
            // Also write data after the last new line.
            log_writer_->write(pending_content_);
            pending_content_.clear();
        }
        return 0;
    }

    // Flush pending_content_ + buffer_ up to the last new line to the log writer, and always move whatever is left in buffer_ to
    // pending_content_.
    void flushPending() {
        auto len = this->pptr() - this->pbase();
        if (this->pptr() == buffer_.get() + bufsize_ - 1 && buffer_[bufsize_ - 1] != traits::to_char_type(0)) {
            len++;
        }
        constexpr size_t npos = std::basic_string_view<char_type, traits>::npos;
        std::basic_string_view<char_type, traits> buf_content(this->pbase(), len);
        std::size_t last_new_line_pos = buf_content.find_last_of(traits::to_char_type('\n'));
        if (last_new_line_pos == npos) {
            pending_content_.append(buf_content);
            this->setp(buffer_.get(), buffer_.get() + bufsize_ - 1);
            return;
        }
        std::basic_string_view<char_type, traits> before_new_line = buf_content.substr(0, last_new_line_pos);
        std::basic_string<char_type, traits> to_print = std::move(pending_content_);
        to_print.append(before_new_line);
        if (!to_print.empty()) {
            log_writer_->write(to_print);
        }
        std::basic_string_view<char_type, traits> after_new_line = buf_content.substr(last_new_line_pos + 1);
        pending_content_ = std::basic_string<char_type, traits>(after_new_line);
        this->setp(buffer_.get(), buffer_.get() + bufsize_ - 1);
    }

    std::unique_ptr<char_type[]> buffer_;
    size_t bufsize_;
    std::basic_string<char_type, traits> pending_content_;
    std::unique_ptr<LogWriter> log_writer_;
};

class AndroidLogcatWriter final : public AndroidLogcatBuf<>::LogWriter {
   public:
    AndroidLogcatWriter() = default;
    void write(const std::string &content) override { __android_log_print(ANDROID_LOG_INFO, "api_dump", "%s", content.c_str()); }
};
#endif

static const char *GetDefaultPrefix() {
#ifdef __ANDROID__
    return "apidump";
#else
    return "APIDUMP";
#endif
}

class ApiDumpSettings {
   public:
    ApiDumpSettings() : output_stream(std::cout.rdbuf()) {
#ifdef __ANDROID__
        android_logcat_buf = std::make_unique<AndroidLogcatBuf<>>(std::make_unique<AndroidLogcatWriter>());
        output_stream.rdbuf(android_logcat_buf.get());
#endif
    }

    ~ApiDumpSettings() {
        if (output_format == ApiDumpFormat::Html) {
            // Close off html
            output_stream << "</details></div></body></html>";
        } else if (output_format == ApiDumpFormat::Json) {
            // Close off json
            output_stream << "\n]" << std::endl;
        }
    }

    void setupInterFrameOutputFormatting(uint64_t frame_count) const /*name change? */
    {
        static bool hasPrintedAFrame = false;
        switch (format()) {
            case (ApiDumpFormat::Html):
                if (frame_count > 0) {
                    if (condFrameOutput.isFrameInRange(frame_count - 1)) output_stream << "</details>";
                }
                if (condFrameOutput.isFrameInRange(frame_count)) {
                    output_stream << "<details class='frm'><summary>Frame ";
                    if (show_thread_and_frame) {
                        output_stream << frame_count;
                    }
                    output_stream << "</summary>";
                }
                break;

            case (ApiDumpFormat::Json):

                if (frame_count > 0) {
                    if (condFrameOutput.isFrameInRange(frame_count - 1)) output_stream << "\n" << indentation(1) << "]\n}";
                }
                if (condFrameOutput.isFrameInRange(frame_count)) {
                    if (!hasPrintedAFrame) {
                        hasPrintedAFrame = true;
                    } else {
                        output_stream << ",\n";
                    }
                    output_stream << "{\n";
                    if (show_thread_and_frame) {
                        output_stream << indentation(1) << "\"frameNumber\" : \"" << frame_count << "\",\n";
                    }
                    output_stream << indentation(1) << "\"apiCalls\" :\n";
                    output_stream << indentation(1) << "[\n";
                }
                break;
            case (ApiDumpFormat::Text):
                break;
            default:
                break;
        }
    }

    void closeFrameOutput() const {
        switch (format()) {
            case (ApiDumpFormat::Html):
                output_stream << "</details>";
                break;
            case (ApiDumpFormat::Json):
                output_stream << "\n" << indentation(1) << "]\n}";
                break;
            case (ApiDumpFormat::Text):
                break;
            default:
                break;
        }
    }

    ApiDumpFormat format() const { return output_format; }

    void formatNameType(int indents, const char *name, const char *type) const {
        output_stream << indentation(indents) << name << ": ";
        // We have to 'print' an empty string for the setw to actually add the desired padding.
        if (use_spaces)
            output_stream << std::setw(name_size - (int)strlen(name) - 2) << "";
        else
            output_stream << std::setw((name_size - (int)strlen(name) - 3 + tab_size) / tab_size) << "";

        if (show_type) {
            if (use_spaces)
                output_stream << std::left << std::setw(type_size) << type << " = ";
            else
                output_stream << type << std::setw((type_size - (int)strlen(type) - 1 + tab_size) / tab_size) << ""
                              << " = ";
        } else {
            output_stream << " = ";
        }
    }

    inline const char *indentation(int indents) const {
        // We have to 'print' an empty string for the setw to actually add the desired padding.
        output_stream << std::setw(indents * indent_size) << "";
        return "";
    }

    bool shouldPreDump() const { return should_pre_dump; }

    bool showAddress() const { return show_address; }

    bool showParams() const { return show_params; }

    bool showShader() const { return show_shader; }

    bool showType() const { return show_type; }

    bool showTimestamp() const { return show_timestamp; }

    bool showThreadAndFrame() const { return show_thread_and_frame; }

    // The const cast is necessary because everyone who 'writes' to the stream necessarily must be able to modify it.
    // Since basically every function in this struct is const, we have to work around that.
    std::ostream &stream() const { return output_stream; }

    bool isFrameInRange(uint64_t frame) const { return condFrameOutput.isFrameInRange(frame); }

    void init(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator) {
        VkuLayerSettingSet layerSettingSet = VK_NULL_HANDLE;
        vkuCreateLayerSettingSet("VK_LAYER_LUNARG_api_dump", vkuFindLayerSettingsCreateInfo(pCreateInfo), pAllocator, nullptr,
                                 &layerSettingSet);

        vkuSetLayerSettingCompatibilityNamespace(layerSettingSet, GetDefaultPrefix());

        // Read the format type first as it may be used in the output file extension
        output_format = ApiDumpFormat::Text;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyOutputFormat)) {
            std::string value;
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyOutputFormat, value);
            value = ToLowerString(value);
            if (value == "html") {
                output_format = ApiDumpFormat::Html;
            } else if (value == "json") {
                output_format = ApiDumpFormat::Json;
            } else {
                output_format = ApiDumpFormat::Text;
            }
        }

        // If the layer settings file has a flag indicating to output to a file,
        // do so, to the appropriate default filename.
        std::string filename_string = "";
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyFile)) {
            bool file = false;
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyFile, file);

            if (file) {
                if (output_format == ApiDumpFormat::Html) {
                    filename_string = "vk_apidump.html";
                } else if (output_format == ApiDumpFormat::Json) {
                    filename_string = "vk_apidump.json";
                } else {
                    filename_string = "vk_apidump.txt";
                }
            }
        }

        // Check if there is a specific filename that should be used
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyLogFilename)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyLogFilename, filename_string);
        }

        // Append file extension if one doesn't exist or is the wrong extension. Make sure the found extension is at the end
        if (!filename_string.empty()) {
            size_t txt_pos = filename_string.find(".txt", filename_string.size() - 4);
            size_t html_pos = filename_string.find(".html", filename_string.size() - 5);
            size_t json_pos = filename_string.find(".json", filename_string.size() - 5);

            if (output_format == ApiDumpFormat::Html) {
                if (json_pos != std::string::npos) filename_string.erase(json_pos);
                if (txt_pos != std::string::npos) filename_string.erase(txt_pos);
                if (html_pos == std::string::npos) filename_string.append(".html");
            } else if (output_format == ApiDumpFormat::Json) {
                if (html_pos != std::string::npos) filename_string.erase(html_pos);
                if (txt_pos != std::string::npos) filename_string.erase(txt_pos);
                if (json_pos == std::string::npos) filename_string.append(".json");
            } else {
                if (html_pos != std::string::npos) filename_string.erase(html_pos);
                if (json_pos != std::string::npos) filename_string.erase(json_pos);
                if (txt_pos == std::string::npos) filename_string.append(".txt");
            }
        }

        // If one of the above has set a filename, open the file as an output stream.
        if (!filename_string.empty()) {
            output_file_stream.open(filename_string, std::ofstream::out | std::ostream::trunc);
            output_stream.rdbuf(output_file_stream.rdbuf());
        }

        show_params = true;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyDetailedOutput)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyDetailedOutput, show_params);
        }

        show_address = true;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyNoAddr)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyNoAddr, show_address);
            // must invert the setting since the setting is called "no address", which is the logical
            // opposite of show_address
            show_address = !show_address;
        }

        should_flush = true;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyFlush)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyFlush, should_flush);
        }

        should_pre_dump = false;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyPreDump)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyPreDump, should_pre_dump);
        }

        show_timestamp = false;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyTimestamp)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyTimestamp, show_timestamp);
        }

        indent_size = 4;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyIndentSize)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyIndentSize, indent_size);
            indent_size = std::max(indent_size, 0);
        }
        tab_size = indent_size;

        show_type = true;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyShowTypes)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyShowTypes, show_type);
        }

        name_size = 32;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyNameSize)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyNameSize, name_size);
            name_size = std::max(name_size, 0);
        }

        type_size = 0;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyTypeSize)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyTypeSize, type_size);
            type_size = std::max(type_size, 0);
        }

        use_spaces = true;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyUseSpaces)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyUseSpaces, use_spaces);
        }

        show_shader = false;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyShowShader)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyShowShader, show_shader);
        }

        show_thread_and_frame = true;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyShowThreadAndFrame)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyShowThreadAndFrame, show_thread_and_frame);
        }

        std::string cond_range_string;
        if (vkuHasLayerSetting(layerSettingSet, kSettingsKeyOutputRange)) {
            vkuGetLayerSettingValue(layerSettingSet, kSettingsKeyOutputRange, cond_range_string);
        }

        if (cond_range_string == "" || cond_range_string == "0-0") {  //"0-0" is every frame, no need to check
            use_conditional_output = false;
        } else {
            bool parsingStatus = condFrameOutput.parseConditionalFrameRange(cond_range_string);
            if (!parsingStatus) {
                use_conditional_output = false;
            }
        }

        // Setfill stays active for the duration of the stream. Setting it during construction
        // means it doesn't have to be set again whenever setw() is called.
        output_stream << std::setfill(use_spaces ? ' ' : '\t');

        if (!use_spaces) {
            indent_size = 1;  // setting this allows indentation to not need a branch on use_spaces
        }

        // Generate HTML heading if specified
        if (output_format == ApiDumpFormat::Html) {
            // clang-format off
            // Insert html heading
            output_stream <<
                "<!doctype html>"
                "<html>"
                    "<head>"
                        "<title>Vulkan API Dump</title>"
                        "<style type='text/css'>"
                        "html {"
                            "background-color: #0b1e48;"
                            "background-image: url('https://vulkan.lunarg.com/img/bg-starfield.jpg');"
                            "background-position: center;"
                            "-webkit-background-size: cover;"
                            "-moz-background-size: cover;"
                            "-o-background-size: cover;"
                            "background-size: cover;"
                            "background-attachment: fixed;"
                            "background-repeat: no-repeat;"
                            "height: 100%;"
                        "}"
                        "#header {"
                            "z-index: -1;"
                        "}"
                        "#header>img {"
                            "position: absolute;"
                            "width: 160px;"
                            "margin-left: -280px;"
                            "top: -10px;"
                            "left: 50%;"
                        "}"
                        "#header>h1 {"
                            "font-family: Arial, 'Helvetica Neue', Helvetica, sans-serif;"
                            "font-size: 44px;"
                            "font-weight: 200;"
                            "text-shadow: 4px 4px 5px #000;"
                            "color: #eee;"
                            "position: absolute;"
                            "width: 400px;"
                            "margin-left: -80px;"
                            "top: 8px;"
                            "left: 50%;"
                        "}"
                        "body {"
                            "font-family: Consolas, monaco, monospace;"
                            "font-size: 14px;"
                            "line-height: 20px;"
                            "color: #eee;"
                            "height: 100%;"
                            "margin: 0;"
                            "overflow: hidden;"
                        "}"
                        "#wrapper {"
                            "background-color: rgba(0, 0, 0, 0.7);"
                            "border: 1px solid #446;"
                            "box-shadow: 0px 0px 10px #000;"
                            "padding: 8px 12px;"
                            "display: inline-block;"
                            "position: absolute;"
                            "top: 80px;"
                            "bottom: 25px;"
                            "left: 50px;"
                            "right: 50px;"
                            "overflow: auto;"
                        "}"
                        "details>*:not(summary) {"
                            "margin-left: 22px;"
                        "}"
                        "summary:only-child {"
                          "display: block;"
                          "padding-left: 15px;"
                        "}"
                        "details>summary:only-child::-webkit-details-marker {"
                            "display: none;"
                            "padding-left: 15px;"
                        "}"
                        ".var, .type, .val {"
                            "display: inline;"
                            "margin: 0 6px;"
                        "}"
                        ".type {"
                            "color: #acf;"
                        "}"
                        ".val {"
                            "color: #afa;"
                            "text-align: right;"
                        "}"
                        ".thd {"
                            "color: #888;"
                        "}"
                        ".time {"
                            "color: #888;"
                        "}"
                        "</style>"
                    "</head>"
                    "<body>"
                        "<div id='header'>"
                            "<img src='https://lunarg.com/wp-content/uploads/2016/02/LunarG-wReg-150.png' alt='LunarG Logo'/>"
                            "<h1>Vulkan API Dump</h1>"
                        "</div>"
                        "<div id='wrapper'>";
            // clang-format on
        } else if (output_format == ApiDumpFormat::Json) {
            output_stream << "[\n";
        }

        if (isFrameInRange(0)) {
            setupInterFrameOutputFormatting(0);
        }

        vkuDestroyLayerSettingSet(layerSettingSet, pAllocator);
    }

    void flush() {
        if (should_flush) {
            output_stream.flush();
        }
    }

   private:
    // Utility member to enable easier comparison by forcing a string to all lower-case
    static std::string ToLowerString(const std::string &value) {
        std::string lower_value = value;
        std::transform(lower_value.begin(), lower_value.end(), lower_value.begin(), ::tolower);
        return lower_value;
    }

    // The mutable is necessary because everyone who 'writes' to the stream necessarily must be able to modify it.
    // Since basically every function in this struct is const, we have to work around that.
    mutable std::ostream output_stream;
    std::ofstream output_file_stream;
#ifdef __ANDROID__
    std::unique_ptr<AndroidLogcatBuf<>> android_logcat_buf = nullptr;
#endif
    ApiDumpFormat output_format;
    bool show_params;
    bool show_address;
    bool should_flush;
    bool should_pre_dump;
    bool show_timestamp;

    bool show_type;
    int indent_size;  // how many indent levels to use - also sets the tab_size
    int name_size;
    int type_size;
    bool use_spaces;
    bool show_shader;
    bool show_thread_and_frame;

    bool use_conditional_output = false;
    ConditionalFrameOutput condFrameOutput;

    int tab_size;  // equal to the indent size if using spaces, otherwise is equal to 1
};

class ApiDumpInstance {
   public:
    ApiDumpInstance() noexcept : frame_count(0) { program_start = std::chrono::system_clock::now(); }
    // Can't copy or move this type
    ApiDumpInstance(const ApiDumpInstance &) = delete;
    ApiDumpInstance &operator=(const ApiDumpInstance &) = delete;
    ApiDumpInstance(ApiDumpInstance &&) = delete;
    ApiDumpInstance &operator=(ApiDumpInstance &&) = delete;

    ~ApiDumpInstance() {
        if (!first_func_call_on_frame) settings().closeFrameOutput();
    }

    void initLayerSettings(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator) {
        this->dump_settings.init(pCreateInfo, pAllocator);
    }

    uint64_t frameCount() {
        std::lock_guard<std::mutex> lg(frame_mutex);
        uint64_t count = frame_count;
        return count;
    }

    void nextFrame() {
        std::lock_guard<std::mutex> lg(frame_mutex);
        ++frame_count;

        should_dump_output = settings().isFrameInRange(frame_count);
        settings().setupInterFrameOutputFormatting(frame_count);
        first_func_call_on_frame = true;
    }

    bool shouldDumpOutput() {
        if (!conditional_initialized) {
            should_dump_output = settings().isFrameInRange(frame_count);
            conditional_initialized = true;
        }
        return should_dump_output;
    }

    bool firstFunctionCallOnFrame() {
        if (first_func_call_on_frame) {
            first_func_call_on_frame = false;
            return true;
        }
        return false;
    }

    std::mutex &outputMutex() { return output_mutex; }

    ApiDumpSettings &settings() { return dump_settings; }
    ApiDumpSettings const &settings() const { return dump_settings; }

    uint64_t threadID() {
        std::thread::id this_id = std::this_thread::get_id();
        std::lock_guard<std::mutex> lg(thread_mutex);

        auto it = thread_map.find(this_id);
        if (it != thread_map.end()) {
            return it->second;
        }

        thread_map.insert({this_id, thread_map.size()});
        return thread_map.size() - 1;
    }

    void setCmdBuffer(VkCommandBuffer cmd_buffer) { this->cmd_buffer = cmd_buffer; }

    VkCommandBufferLevel getCmdBufferLevel() {
        std::lock_guard<std::mutex> lg(cmd_buffer_state_mutex);
        const auto level_iter = cmd_buffer_level.find(cmd_buffer);
        assert(level_iter != cmd_buffer_level.end());
        const auto level = level_iter->second;
        return level;
    }

    void eraseCmdBuffers(VkDevice device, VkCommandPool cmd_pool, std::vector<VkCommandBuffer> cmd_buffers) {
        cmd_buffers.erase(std::remove(cmd_buffers.begin(), cmd_buffers.end(), nullptr), cmd_buffers.end());
        if (!cmd_buffers.empty()) {
            std::lock_guard<std::mutex> lg(cmd_buffer_state_mutex);

            const auto pool_cmd_buffers_iter = cmd_buffer_pools.find(std::make_pair(device, cmd_pool));
            assert(pool_cmd_buffers_iter != cmd_buffer_pools.end());

            for (const auto cmd_buffer : cmd_buffers) {
                pool_cmd_buffers_iter->second.erase(cmd_buffer);

                assert(cmd_buffer_level.count(cmd_buffer) > 0);
                cmd_buffer_level.erase(cmd_buffer);
            }
        }
    }

    void addCmdBuffers(VkDevice device, VkCommandPool cmd_pool, std::vector<VkCommandBuffer> cmd_buffers,
                       VkCommandBufferLevel level) {
        std::lock_guard<std::mutex> lg(cmd_buffer_state_mutex);
        auto &pool_cmd_buffers = cmd_buffer_pools[std::make_pair(device, cmd_pool)];
        pool_cmd_buffers.insert(cmd_buffers.begin(), cmd_buffers.end());

        for (const auto cmd_buffer : cmd_buffers) {
            assert(cmd_buffer_level.count(cmd_buffer) == 0);
            cmd_buffer_level[cmd_buffer] = level;
        }
    }

    void eraseCmdBufferPool(VkDevice device, VkCommandPool cmd_pool) {
        if (cmd_pool != VK_NULL_HANDLE) {
            std::lock_guard<std::mutex> lg(cmd_buffer_state_mutex);

            const auto cmd_buffers_iter = cmd_buffer_pools.find(std::make_pair(device, cmd_pool));
            if (cmd_buffers_iter != cmd_buffer_pools.end()) {
                for (const auto cmd_buffer : cmd_buffers_iter->second) {
                    assert(cmd_buffer_level.count(cmd_buffer) > 0);
                    cmd_buffer_level.erase(cmd_buffer);
                }
                cmd_buffers_iter->second.clear();
            }
        }
    }

    void setIsDynamicScissor(bool is_dynamic_scissor) { this->is_dynamic_scissor = is_dynamic_scissor; }
    void setIsDynamicViewport(bool is_dynamic_viewport) { this->is_dynamic_viewport = is_dynamic_viewport; }
    bool getIsDynamicScissor() const { return is_dynamic_scissor; }
    bool getIsDynamicViewport() const { return is_dynamic_viewport; }
    void setMemoryHeapCount(uint32_t memory_heap_count) { this->memory_heap_count = memory_heap_count; }
    uint32_t getMemoryHeapCount() const { return memory_heap_count; }
    void setDescriptorType(VkDescriptorType type) { this->descriptor_type = type; }
    VkDescriptorType getDescriptorType() { return this->descriptor_type; }
    void setIsGPLPreRasterOrFragmentShader(bool in) { this->GPLPreRasterOrFragmentShader = in; }
    bool getIsGPLPreRasterOrFragmentShader() const { return this->GPLPreRasterOrFragmentShader; }
    void setIndirectExecutionSetInfoType(VkIndirectExecutionSetInfoTypeEXT type) { this->indirectExecutionSetInfoType = type; }
    VkIndirectExecutionSetInfoTypeEXT getIndirectExecutionSetInfoType() const { return this->indirectExecutionSetInfoType; }
    void setIndirectCommandsLayoutToken(VkIndirectCommandsTokenTypeEXT type) { this->indirectCommandsLayoutToken = type; }
    VkIndirectCommandsTokenTypeEXT getIndirectCommandsLayoutToken() const { return this->indirectCommandsLayoutToken; }
    void setDescriptorMappingSource(VkDescriptorMappingSourceEXT source) { this->descriptorMappingSource = source; }
    VkDescriptorMappingSourceEXT getDescriptorMappingSource() const { return this->descriptorMappingSource; }

    void setSpsMaxSubLayersMinus1(uint8_t sps_max_sub_layers_minus1) {
        this->sps_max_sub_layers_minus1 = sps_max_sub_layers_minus1;
    }
    uint8_t getSpsMaxSubLayersMinus1() const { return sps_max_sub_layers_minus1; }
    void setVpsMaxSubLayersMinus1(uint8_t vps_max_sub_layers_minus1) {
        this->vps_max_sub_layers_minus1 = vps_max_sub_layers_minus1;
    }
    uint8_t getVpsMaxSubLayersMinus1() const { return vps_max_sub_layers_minus1; }
    void setIsInVps(bool is_in_vps) { this->is_in_vps = is_in_vps; }
    bool getIsInVps() const { return is_in_vps; }

    std::chrono::microseconds current_time_since_start() {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now - program_start);
    }

    static ApiDumpInstance &current() {
        // Because ApiDumpInstance is a static variable in a static function, there will only be one instance of it.
        // Additionally, the object will be constructed on the *first* call to current(), rather than at process startup time.
        static ApiDumpInstance current_instance;
        return current_instance;
    }

    std::unordered_map<uint64_t, std::string> object_name_map;

    void set_vk_instance(VkPhysicalDevice phys_dev, VkInstance instance) { vk_instance_map.insert({phys_dev, instance}); }
    VkInstance get_vk_instance(VkPhysicalDevice phys_dev) const {
        if (vk_instance_map.count(phys_dev) == 0) return VK_NULL_HANDLE;
        return vk_instance_map.at(phys_dev);
    }

    void update_object_name_map(const VkDebugMarkerObjectNameInfoEXT *pNameInfo) {
        if (pNameInfo->pObjectName)
            object_name_map[pNameInfo->object] = pNameInfo->pObjectName;
        else
            object_name_map.erase(pNameInfo->object);
    }
    void update_object_name_map(const VkDebugUtilsObjectNameInfoEXT *pNameInfo) {
        if (pNameInfo->pObjectName)
            object_name_map[pNameInfo->objectHandle] = pNameInfo->pObjectName;
        else
            object_name_map.erase(pNameInfo->objectHandle);
    }

   private:
    ApiDumpSettings dump_settings;
    std::mutex output_mutex;
    std::mutex frame_mutex;
    uint64_t frame_count;

    std::mutex thread_mutex;
    std::unordered_map<std::thread::id, uint64_t> thread_map;

    std::mutex cmd_buffer_state_mutex;
    std::map<std::pair<VkDevice, VkCommandPool>, std::unordered_set<VkCommandBuffer>> cmd_buffer_pools;
    std::unordered_map<VkCommandBuffer, VkCommandBufferLevel> cmd_buffer_level;

    bool conditional_initialized = false;
    bool should_dump_output = true;
    bool first_func_call_on_frame = true;

    std::chrono::system_clock::time_point program_start;

    // Store the VkInstance handle so we don't use null in the call to
    // vkGetInstanceProcAddr(instance_handle, "vkCreateDevice");
    std::unordered_map<VkPhysicalDevice, VkInstance> vk_instance_map;

    // Storage for getCmdBufferLevel() which is called in a place where it needs access to the cmd_buffer but it isn't present in
    // the current structure.
    VkCommandBuffer cmd_buffer;

    // Storage for VkPipelineViewportStateCreateInfo which needs to ignore the scissor and viewport pipeline state if their
    // respective dynamic state is set.
    bool is_dynamic_scissor;
    bool is_dynamic_viewport;

    // Storage for VkPhysicalDeviceMemoryBudgetPropertiesEXT which needs the number of heaps from VkPhysicalDeviceMemoryProperties
    uint32_t memory_heap_count;

    // Storage for the VkDescriptorDataEXT/VkResourceDescriptorDataEXT union to know what is the active element
    VkDescriptorType descriptor_type;

    // True when creating a graphics pipeline library with VK_GRAPHICS_PIPELINE_LIBRARY_PRE_RASTERIZATION_SHADERS_BIT_EXT or
    // VK_GRAPHICS_PIPELINE_LIBRARY_FRAGMENT_SHADER_BIT_EXT set in the VkGraphicsPipelineLibraryCreateInfoEXT struct.
    bool GPLPreRasterOrFragmentShader;

    // Storage for the VkIndirectExecutionSetInfoEXT union to know which is the active element
    VkIndirectExecutionSetInfoTypeEXT indirectExecutionSetInfoType;

    // Storage for the VkIndirectCommandsTokenDataEXT union to know which is the active element
    VkIndirectCommandsTokenTypeEXT indirectCommandsLayoutToken;

    // Storage for the VkDescriptorMappingSourceDataEXT union
    VkDescriptorMappingSourceEXT descriptorMappingSource;

    // Storage for StdVideoH265HrdParameters's array length for pSubLayerHrdParametersNal and pSubLayerHrdParametersVcl
    uint8_t sps_max_sub_layers_minus1;
    uint8_t vps_max_sub_layers_minus1;

    // True if StdVideoH265HrdParameters is currently in StdVideoH265VideoParameterSet, false if it is in
    // StdVideoH265SequenceParameterSetVui. Needed to determine whether to use vps_max_sub_layers_minus1 or
    // sps_max_sub_layers_minus1
    bool is_in_vps;
};

// Helper function to determine the value of GPLPreRasterOrFragmentShader;
inline bool checkForGPLPreRasterOrFragmentShader(const VkGraphicsPipelineCreateInfo &object) {
    VkGraphicsPipelineLibraryFlagsEXT flags{};
    const VkBaseInStructure *pNext_chain = reinterpret_cast<const VkBaseInStructure *>(object.pNext);
    while (pNext_chain) {
        if (pNext_chain->sType == VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT) {
            const auto *gpl_create_info = reinterpret_cast<const VkGraphicsPipelineLibraryCreateInfoEXT *>(pNext_chain);
            flags = gpl_create_info->flags;
        }
        pNext_chain = reinterpret_cast<const VkBaseInStructure *>(pNext_chain->pNext);
    }
    return flags &
           (VK_GRAPHICS_PIPELINE_LIBRARY_PRE_RASTERIZATION_SHADERS_BIT_EXT | VK_GRAPHICS_PIPELINE_LIBRARY_FRAGMENT_SHADER_BIT_EXT);
}
// -------- Dump functions --------------- //

inline void dump_value_start(ApiDumpInstance &api_dump) {
    if (api_dump.settings().format() == ApiDumpFormat::Html) api_dump.settings().stream() << "<div class=\'val\'>";
    if (api_dump.settings().format() == ApiDumpFormat::Json) api_dump.settings().stream() << " \"";
}

inline void dump_value_end(ApiDumpInstance &api_dump) {
    if (api_dump.settings().format() == ApiDumpFormat::Html) api_dump.settings().stream() << "</div>";
    if (api_dump.settings().format() == ApiDumpFormat::Json) api_dump.settings().stream() << '"';
}

template <typename... T>
void dump_value(ApiDumpInstance &api_dump, T &&...values) {
    dump_value_start(api_dump);
    (api_dump.settings().stream() << ... << values);
    dump_value_end(api_dump);
}

template <typename T>
void dump_value_hex(ApiDumpInstance &api_dump, T &&value) {
    dump_value_start(api_dump);
    api_dump.settings().stream() << "0x" << std::hex << value << std::dec;
    dump_value_end(api_dump);
}

template <typename T>
void dump_enum_with_value(ApiDumpInstance &api_dump, const char *name, T value) {
    dump_value(api_dump, name, " (", value, ")");
}

template <typename T>
void dump_enum(ApiDumpInstance &api_dump, const char *name, T value) {
    if (api_dump.settings().format() == ApiDumpFormat::Text || api_dump.settings().format() == ApiDumpFormat::Html) {
        dump_value(api_dump, name, " (", value, ")");
    } else if (api_dump.settings().format() == ApiDumpFormat::Json) {
        dump_value(api_dump, name);
    }
}

void dump_pNext_struct_name(ApiDumpInstance &api_dump, const void *object, const char *type_name, const char *var_name,
                            int indents);

void dump_pNext_trampoline(ApiDumpInstance &api_dump, const void *object, const char *type_name, const char *var_name, int indents);

inline void dump_pNext(ApiDumpInstance &api_dump, const void *object, const char *type_string, const char *name, int indents) {
    if (api_dump.settings().format() == ApiDumpFormat::Text) {
        dump_pNext_struct_name(api_dump, object, type_string, name, indents);
    } else if (api_dump.settings().format() == ApiDumpFormat::Html || api_dump.settings().format() == ApiDumpFormat::Json) {
        dump_pNext_trampoline(api_dump, object, type_string, name, indents);
    }
}

inline void dump_address(ApiDumpInstance &api_dump, const void *address) {
    if (address == NULL) {
        // TEMPORARY to minimize diff in output while developing.
        if (api_dump.settings().format() == ApiDumpFormat::Json) {
            dump_value(api_dump, "address");
        } else {
            dump_value(api_dump, "NULL");
        }
    } else if (api_dump.settings().showAddress())
        dump_value(api_dump, address);
    else
        dump_value(api_dump, "address");
}

inline void dump_separate_members(ApiDumpInstance &api_dump) {
    if (api_dump.settings().format() == ApiDumpFormat::Json) {
        api_dump.settings().stream() << ",\n";
    }
}

//============================== Json formatting helper functions ==============================//

inline void dump_json_key_start_array(ApiDumpInstance &api_dump, int indents, const char *key) {
    api_dump.settings().stream() << api_dump.settings().indentation(indents) << "\"" << key << "\" :\n"
                                 << api_dump.settings().indentation(indents) << "[\n";
}
inline void dump_json_end_array(ApiDumpInstance &api_dump, int indents) {
    api_dump.settings().stream() << api_dump.settings().indentation(indents) << "]";
}
inline void dump_json_newline_end_array(ApiDumpInstance &api_dump, int indents) {
    api_dump.settings().stream() << "\n";
    dump_json_end_array(api_dump, indents);
}
inline void dump_json_start_object(ApiDumpInstance &api_dump, int indents) {
    api_dump.settings().stream() << api_dump.settings().indentation(indents) << "{\n";
}
inline void dump_json_end_object(ApiDumpInstance &api_dump, int indents) {
    api_dump.settings().stream() << "\n" << api_dump.settings().indentation(indents) << "}";
}
inline void dump_json_key(ApiDumpInstance &api_dump, int indents, const char *key) {
    api_dump.settings().stream() << api_dump.settings().indentation(indents) << "\"" << key << "\" :";
}

template <typename... T>
void dump_json_key_value(ApiDumpInstance &api_dump, int indents, const char *key, T &&...values) {
    dump_json_key(api_dump, indents, key);
    dump_value(api_dump, values...);
}

inline void dump_json_key_address(ApiDumpInstance &api_dump, int indents, const void *address) {
    dump_json_key(api_dump, indents, "address");
    dump_address(api_dump, address);
}

//================================ Common Output Functions ================================//

inline void dump_start(ApiDumpInstance &api_dump, OutputConstruct construct, const char *type_string, const char *name, int indents,
                       const void *address = nullptr) {
    auto &settings = api_dump.settings();

    switch (settings.format()) {
        case (ApiDumpFormat::Text): {
            settings.formatNameType(indents, name, type_string);
            if (construct == OutputConstruct::pointer) {
                dump_address(api_dump, address);
            } else if (construct == OutputConstruct::api_struct || construct == OutputConstruct::api_union) {
                if (settings.showAddress())
                    dump_value(api_dump, address);
                else
                    dump_value(api_dump, "address");
                if (construct == OutputConstruct::api_struct) {
                    settings.stream() << ":\n";
                } else if (construct == OutputConstruct::api_union) {
                    settings.stream() << " (Union):\n";
                }
            }
        }
        case (ApiDumpFormat::Html): {
            settings.stream() << "<details class='data'><summary>";
            settings.stream() << "<div class='var'>" << name << "</div>";
            if (settings.showType()) {
                settings.stream() << "<div class='type'>" << type_string << "</div>";
            }
            if (construct == OutputConstruct::pointer) {
                dump_address(api_dump, address);
            } else if (construct == OutputConstruct::api_struct) {
                if (settings.showAddress())
                    dump_value(api_dump, address, "\n");
                else
                    dump_value(api_dump, "address\n");
                settings.stream() << "</summary>";
            } else if (construct == OutputConstruct::api_union) {
                if (settings.showAddress())
                    dump_value(api_dump, address, " (Union):\n");
                else
                    dump_value(api_dump, "address (Union):\n");
                settings.stream() << "</summary>";
            }
        }
        case (ApiDumpFormat::Json): {
            dump_json_start_object(api_dump, indents);
            if (construct == OutputConstruct::api_union)
                dump_json_key_value(api_dump, indents + 1, "type", type_string, " (Union)");
            else
                dump_json_key_value(api_dump, indents + 1, "type", type_string);

            dump_separate_members(api_dump);
            dump_json_key_value(api_dump, indents + 1, "name", name);

            if (construct == OutputConstruct::pointer || address != nullptr) {
                dump_separate_members(api_dump);
                dump_json_key_address(api_dump, indents + 1, address);
            }
            if (construct != OutputConstruct::pointer) {
                dump_separate_members(api_dump);
                if (construct == OutputConstruct::value) {
                    dump_json_key(api_dump, indents + 1, "value");
                } else if (construct == OutputConstruct::api_struct || construct == OutputConstruct::api_union) {
                    dump_json_key_start_array(api_dump, indents + 1, "members");
                }
            }
        }
    }
}

inline void dump_end(ApiDumpInstance &api_dump, OutputConstruct construct, int indents) {
    auto &settings = api_dump.settings();
    switch (settings.format()) {
        case (ApiDumpFormat::Text): {
            if (construct == OutputConstruct::value || construct == OutputConstruct::pointer) settings.stream() << "\n";
        }
        case (ApiDumpFormat::Html): {
            if (construct == OutputConstruct::value || construct == OutputConstruct::pointer) {
                settings.stream() << "</summary></details>";
            } else {
                settings.stream() << "</details>";
            }
        }
        case (ApiDumpFormat::Json): {
            if (construct == OutputConstruct::api_struct || construct == OutputConstruct::api_union) {
                dump_json_newline_end_array(api_dump, indents + 1);
            }
            dump_json_end_object(api_dump, indents);
        }
    }
}

inline void dump_nullptr(ApiDumpInstance &api_dump, const char *type_string, const char *name, int indents) {
    dump_start(api_dump, OutputConstruct::pointer, type_string, name, indents);
    dump_end(api_dump, OutputConstruct::pointer, indents);
}

inline void dump_char(ApiDumpInstance &api_dump, const char *object, const char *type_string, const char *name, int indents,
                      const void *address = nullptr) {
    dump_start(api_dump, OutputConstruct::value, type_string, name, indents, address);
    if (api_dump.settings().format() == ApiDumpFormat::Text || api_dump.settings().format() == ApiDumpFormat::Html) {
        if (object == NULL)
            dump_value(api_dump, "NULL");
        else
            dump_value(api_dump, '"', object, '"');
    } else if (api_dump.settings().format() == ApiDumpFormat::Json) {
        dump_value(api_dump, (object != NULL ? object : ""));
    }
    dump_end(api_dump, OutputConstruct::value, indents);
}

inline void dump_void(ApiDumpInstance &api_dump, const void *object, const char *type_string, const char *name, int indents,
                      const void *address = nullptr) {
    dump_start(api_dump, OutputConstruct::pointer, type_string, name, indents, object);
    dump_end(api_dump, OutputConstruct::pointer, indents);
}

inline void dump_api_version(ApiDumpInstance &api_dump, uint32_t api_version, const char *type_string, const char *name,
                             int indents, const void *address = nullptr) {
    dump_start(api_dump, OutputConstruct::value, type_string, name, indents, address);
    dump_value(api_dump, api_version, " (", VK_API_VERSION_MAJOR(api_version), ".", VK_API_VERSION_MINOR(api_version), ".",
               VK_API_VERSION_PATCH(api_version), ")");
    dump_end(api_dump, OutputConstruct::value, indents);
}

inline void dump_special(ApiDumpInstance &api_dump, const char *const &text, const char *type_string, const char *name,
                         int indents) {
    dump_start(api_dump, OutputConstruct::value, type_string, name, indents, text);
    dump_value(api_dump, text);
    dump_end(api_dump, OutputConstruct::value, indents);
}

template <typename T>
void dump_handle(ApiDumpInstance &api_dump, T object, const char *type_string, const char *name, int indents,
                 const void *address = nullptr) {
    dump_start(api_dump, OutputConstruct::value, type_string, name, indents, address);
    if (api_dump.settings().showAddress()) {
        if (api_dump.settings().format() == ApiDumpFormat::Text || api_dump.settings().format() == ApiDumpFormat::Html) {
            std::unordered_map<uint64_t, std::string>::const_iterator it =
                ApiDumpInstance::current().object_name_map.find((uint64_t)object);
            if (it != ApiDumpInstance::current().object_name_map.end()) {
                dump_value(api_dump, object, " [", it->second, "]");
            } else {
                dump_value(api_dump, object);
            }
        } else if (api_dump.settings().format() == ApiDumpFormat::Json) {
            dump_value(api_dump, object);
        }
    } else {
        dump_value(api_dump, "address");
    }
    dump_end(api_dump, OutputConstruct::value, indents);
}

template <typename T>
std::enable_if_t<!std::is_pointer_v<T>> dump_type(ApiDumpInstance &api_dump, const T &object, const char *type_string,
                                                  const char *name, int indents, const void *address = nullptr) {
    dump_start(api_dump, OutputConstruct::value, type_string, name, indents, address);
    dump_value(api_dump, object);
    dump_end(api_dump, OutputConstruct::value, indents);
}

template <typename T>
std::enable_if_t<std::is_pointer_v<T>> dump_type(ApiDumpInstance &api_dump, const T &object, const char *type_string,
                                                 const char *name, int indents, const void *address = nullptr) {
    dump_start(api_dump, OutputConstruct::pointer, type_string, name, indents, object);
    dump_end(api_dump, OutputConstruct::pointer, indents);
}

template <typename T>
std::enable_if_t<!std::is_pointer_v<T>> dump_type_hex(ApiDumpInstance &api_dump, const T &object, const char *type_string,
                                                      const char *name, int indents, const void *address = nullptr) {
    dump_start(api_dump, OutputConstruct::value, type_string, name, indents, address);
    dump_value_hex(api_dump, object);
    dump_end(api_dump, OutputConstruct::value, indents);
}

inline void dump_uint64_t_as_pointer(ApiDumpInstance &api_dump, const uint64_t object, const char *type_string, const char *name,
                                     int indents) {
    dump_start(api_dump, OutputConstruct::pointer, type_string, name, indents,
               reinterpret_cast<const void *>(static_cast<uintptr_t>(object)));
    dump_end(api_dump, OutputConstruct::pointer, indents);
}

template <typename T, typename DumpValue>
void dump_pointer(ApiDumpInstance &api_dump, const T *object, const char *type_string, const char *name, int indents,
                  DumpValue dump_value) {
    if (object == NULL) {
        dump_nullptr(api_dump, type_string, name, indents);
    } else {
        dump_value(api_dump, *object, type_string, name, indents, object);
    }
}

inline void dump_array_start(ApiDumpInstance &api_dump, const void *array, size_t len, const char *type_string, const char *name,
                             int indents) {
    if (api_dump.settings().format() == ApiDumpFormat::Text) {
        dump_start(api_dump, OutputConstruct::pointer, type_string, name, indents, array);
        api_dump.settings().stream() << "\n";
    } else if (api_dump.settings().format() == ApiDumpFormat::Html) {
        dump_start(api_dump, OutputConstruct::api_struct, type_string, name, indents, array);
    } else if (api_dump.settings().format() == ApiDumpFormat::Json) {
        dump_json_start_object(api_dump, indents);
        dump_json_key_value(api_dump, indents + 1, "type", type_string);
        dump_separate_members(api_dump);
        dump_json_key_value(api_dump, indents + 1, "name", name);
        dump_separate_members(api_dump);
        dump_json_key_address(api_dump, indents + 1, array);
        if (len > 0 && array != NULL) {
            dump_separate_members(api_dump);
            dump_json_key_start_array(api_dump, indents + 1, "elements");
        } else {
            api_dump.settings().stream() << "\n";
        }
    }
}

inline void dump_array_end(ApiDumpInstance &api_dump, const void *array, size_t len, int indents) {
    if (api_dump.settings().format() == ApiDumpFormat::Html) {
        dump_end(api_dump, OutputConstruct::api_struct, indents);
    } else if (api_dump.settings().format() == ApiDumpFormat::Json) {
        if (len > 0 && array != NULL) {
            dump_json_end_array(api_dump, indents + 1);
        }
        dump_json_end_object(api_dump, indents);
    }
}

template <size_t N, typename T, typename DumpElement>
void dump_double_array(ApiDumpInstance &api_dump, const T(array)[][N], size_t len1, size_t len2, const char *type_string,
                       const char *name, const char *element_type, int indents, DumpElement dump_element) {
    if (len1 == 0 || len2 == 0) {
        return;
    }
    dump_array_start(api_dump, array, len1 * len2, type_string, name, indents);
    for (size_t i = 0; i < len1; ++i) {
        for (size_t j = 0; j < len2; ++j) {
            std::stringstream stream;
            if (api_dump.settings().format() == ApiDumpFormat::Text || api_dump.settings().format() == ApiDumpFormat::Html) {
                api_dump.settings().stream() << name;
            }
            api_dump.settings().stream() << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_element(api_dump, array[i][j], element_type, indexName.c_str(),
                         indents + (api_dump.settings().format() == ApiDumpFormat::Json ? 2 : 1), nullptr);
            if (api_dump.settings().format() == ApiDumpFormat::Json) {
                if (i < len1 - 1 && j < len2 - 1) api_dump.settings().stream() << ',';
                api_dump.settings().stream() << "\n";
            }
        }
    }
    dump_array_end(api_dump, array, len1 * len2, indents);
}

template <size_t N, typename T, typename DumpElement>
void dump_single_array(ApiDumpInstance &api_dump, const T (&array)[N], size_t len, const char *type_string, const char *name,
                       const char *element_type, int indents, DumpElement dump_element) {
    if (len == 0) {
        return;
    }
    dump_array_start(api_dump, array, len, type_string, name, indents);
    for (size_t i = 0; i < len; ++i) {
        std::stringstream stream;
        if (api_dump.settings().format() == ApiDumpFormat::Text || api_dump.settings().format() == ApiDumpFormat::Html) {
            api_dump.settings().stream() << name;
        }
        api_dump.settings().stream() << "[" << i << "]";
        std::string indexName = stream.str();
        dump_element(api_dump, array[i], element_type, indexName.c_str(),
                     indents + (api_dump.settings().format() == ApiDumpFormat::Json ? 2 : 1), nullptr);
        if (api_dump.settings().format() == ApiDumpFormat::Json) {
            if (i < len - 1) api_dump.settings().stream() << ',';
            api_dump.settings().stream() << "\n";
        }
    }
    dump_array_end(api_dump, array, len, indents);
}

template <typename T, typename DumpElement>
void dump_pointer_array(ApiDumpInstance &api_dump, const T *array, size_t len, const char *type_string, const char *name,
                        const char *element_type, int indents, DumpElement dump_element) {
    if (array == NULL || len == 0) {
        dump_nullptr(api_dump, type_string, name, indents);
        return;
    }
    dump_array_start(api_dump, array, len, type_string, name, indents);
    for (size_t i = 0; i < len; ++i) {
        std::stringstream stream;
        if (api_dump.settings().format() == ApiDumpFormat::Text || api_dump.settings().format() == ApiDumpFormat::Html) {
            api_dump.settings().stream() << name;
        }
        api_dump.settings().stream() << "[" << i << "]";
        std::string indexName = stream.str();
        dump_element(api_dump, array[i], element_type, indexName.c_str(),
                     indents + (api_dump.settings().format() == ApiDumpFormat::Json ? 2 : 1), array + i);
        if (api_dump.settings().format() == ApiDumpFormat::Json) {
            if (i < len - 1) api_dump.settings().stream() << ',';
            api_dump.settings().stream() << "\n";
        }
    }
    dump_array_end(api_dump, array, len, indents);
}

template <typename T, typename DumpElement>
void dump_double_pointer_array(ApiDumpInstance &api_dump, const T *const *array, size_t len, const char *type_string,
                               const char *name, const char *element_type, int indents, DumpElement dump_element) {
    if (array == nullptr || len == 0) {
        dump_nullptr(api_dump, type_string, name, indents);
        return;
    }
    dump_array_start(api_dump, array, len, type_string, name, indents);
    for (size_t i = 0; i < len; ++i) {
        std::stringstream stream;
        if (api_dump.settings().format() == ApiDumpFormat::Text || api_dump.settings().format() == ApiDumpFormat::Html) {
            api_dump.settings().stream() << name;
        }
        api_dump.settings().stream() << "[" << i << "]";
        std::string indexName = stream.str();
        dump_pointer(api_dump, array[i], element_type, indexName.c_str(),
                     indents + (api_dump.settings().format() == ApiDumpFormat::Json ? 2 : 1), dump_element);
        if (api_dump.settings().format() == ApiDumpFormat::Json) {
            if (i < len - 1) api_dump.settings().stream() << ',';
            api_dump.settings().stream() << "\n";
        }
    }
    dump_array_end(api_dump, array, len, indents);
}

// Designed to be viewed in something like https://www.khronos.org/spir/visualizer/
// There is also now support in SPIRV-Tools (https://github.com/KhronosGroup/SPIRV-Tools/pull/5870) to consume the array
template <typename T>
void dump_spirv(ApiDumpInstance &api_dump, const T *array, size_t len, const char *type_string, const char *name, int indents) {
    if (array == NULL || len == 0) {
        dump_nullptr(api_dump, type_string, name, indents);
        return;
    }
    dump_array_start(api_dump, array, len, type_string, name, indents);

    std::stringstream stream;
    // For JSON we will just dump it as a valid JSON array of string like
    // [ "0x07230203", "0x00010300", "0x0008000b", ...
    if (api_dump.settings().format() == ApiDumpFormat::Json) {
        for (size_t i = 0; i < len; ++i) {
            if (i != 0) {
                api_dump.settings().stream() << ", ";
            }
            const uint32_t dword = static_cast<const uint32_t *>(array)[i];
            api_dump.settings().stream() << "\"0x" << std::hex << std::setfill('0') << std::setw(8) << dword << "\"";
        }
        api_dump.settings().stream() << stream.str();
    } else {
        api_dump.settings().stream() << api_dump.settings().indentation(indents) << "[ ";
        for (size_t i = 0; i < len; ++i) {
            if (i != 0) {
                api_dump.settings().stream() << ", ";
            }
            const uint32_t dword = static_cast<const uint32_t *>(array)[i];
            api_dump.settings().stream() << "0x" << std::hex << std::setfill('0') << std::setw(8) << dword;
        }
        api_dump.settings().stream() << " ]\n";
        dump_value(api_dump, stream.str());
    }

    dump_array_end(api_dump, array, len, indents);
}

inline void dump_before_pre_dump_formatting(ApiDumpInstance &api_dump) {
    if (api_dump.settings().format() == ApiDumpFormat::Text) {
        if (api_dump.settings().shouldPreDump()) {
            api_dump.settings().stream() << "<before calling into implementation>\n";
        }
    }
}

template <typename T>
void dump_return_value(ApiDumpInstance &api_dump, const char *returnType, T result) {
    auto &settings = api_dump.settings();

    if (settings.format() == ApiDumpFormat::Text) {
        if (settings.shouldPreDump()) {
            settings.stream() << "return " << returnType;
        }
        settings.stream() << " ";

    } else if (settings.format() == ApiDumpFormat::Json) {
        dump_separate_members(api_dump);
        dump_json_key(api_dump, 3, "returnValue");
    }
    dump_value(api_dump, result);
    if (settings.format() == ApiDumpFormat::Text) {
        if (settings.shouldPreDump()) {
            settings.stream() << ":\n";
        }
    }
}

template <typename T, typename DumpReturnValue>
void dump_return_value(ApiDumpInstance &api_dump, const char *returnType, T result, DumpReturnValue dump_return_value) {
    auto &settings = api_dump.settings();

    if (settings.format() == ApiDumpFormat::Text) {
        if (settings.shouldPreDump()) {
            settings.stream() << "return " << returnType;
        }
        settings.stream() << " ";

    } else if (settings.format() == ApiDumpFormat::Json) {
        dump_separate_members(api_dump);
        dump_json_key(api_dump, 3, "returnValue");
    }
    dump_return_value(api_dump, result);
    if (settings.format() == ApiDumpFormat::Text) {
        if (settings.shouldPreDump()) {
            settings.stream() << ":\n";
        }
    }
}

inline void dump_pre_params_formatting(ApiDumpInstance &api_dump) {
    if (api_dump.settings().format() == ApiDumpFormat::Json) {
        dump_separate_members(api_dump);
        dump_json_key_start_array(api_dump, 3, "args");
    }
}

inline void dump_post_params_formatting(ApiDumpInstance &api_dump) {
    auto &settings = api_dump.settings();

    if (settings.format() == ApiDumpFormat::Text || settings.format() == ApiDumpFormat::Html) {
        settings.stream() << "\n";
    } else if (settings.format() == ApiDumpFormat::Json) {
        settings.stream() << "\n" << settings.indentation(3) << "]\n";
    }
}

inline void dump_pre_function_formatting(ApiDumpInstance &api_dump) {
    auto &settings = api_dump.settings();

    if (settings.format() == ApiDumpFormat::Text) {
        if (settings.shouldPreDump()) {
            settings.stream() << "<after returning from implementation>\n";
        } else {
            settings.stream() << ":\n";
        }
    } else if (settings.format() == ApiDumpFormat::Html) {
        settings.stream() << "</summary>";
    }
}

inline void dump_post_function_formatting(ApiDumpInstance &api_dump) {
    auto &settings = api_dump.settings();

    if (settings.format() == ApiDumpFormat::Html) {
        settings.stream() << "</details>";
    } else if (settings.format() == ApiDumpFormat::Json) {
        settings.stream() << settings.indentation(2) << "}";
    }
}

//==================================== Function Head Helpers ======================================//

inline void dump_function_head(ApiDumpInstance &api_dump, const char *funcName, const char *funcNamedParams,
                               const char *funcReturn) {
    auto &settings = api_dump.settings();
    switch (api_dump.settings().format()) {
        case ApiDumpFormat::Text: {
            if (settings.showThreadAndFrame()) {
                settings.stream() << "Thread " << api_dump.threadID() << ", Frame " << api_dump.frameCount();
            }
            if (settings.showTimestamp() && settings.showThreadAndFrame()) {
                settings.stream() << ", ";
            }
            if (settings.showTimestamp()) {
                settings.stream() << "Time " << api_dump.current_time_since_start().count() << " us";
            }
            if (settings.showTimestamp() || settings.showThreadAndFrame()) {
                settings.stream() << ":\n";
            }
            settings.stream() << funcName << "(" << funcNamedParams << ") returns " << funcReturn;
            if (settings.shouldPreDump() && api_dump.shouldDumpOutput()) {
                settings.stream() << ":\n";
            }
        }
        case ApiDumpFormat::Html: {
            if (settings.showThreadAndFrame()) {
                settings.stream() << "<div class='thd'>Thread: " << api_dump.threadID() << "</div>";
            }
            if (settings.showTimestamp())
                settings.stream() << "<div class='time'>Time: " << api_dump.current_time_since_start().count() << " us</div>";
            settings.stream() << "<details class='fn'><summary>";
            settings.stream() << "<div class='var'>" << funcName << "(" << funcNamedParams << ")</div>";
            if (settings.showType()) {
                settings.stream() << "<div class='type'>" << funcReturn << "</div>";
            }
        }
        case ApiDumpFormat::Json: {
            if (!api_dump.firstFunctionCallOnFrame()) {
                dump_separate_members(api_dump);
            }
            // Display api call name
            dump_json_start_object(api_dump, 2);
            dump_json_key_value(api_dump, 3, "name", funcName);

            // Display thread info
            if (settings.showThreadAndFrame()) {
                dump_separate_members(api_dump);
                dump_json_key_value(api_dump, 3, "thread", "Thread ", api_dump.threadID());
            }

            // Display elapsed time
            if (settings.showTimestamp()) {
                dump_separate_members(api_dump);
                dump_json_key_value(api_dump, 3, "time", api_dump.current_time_since_start().count(), " us");
            }

            // Display return type
            dump_separate_members(api_dump);
            dump_json_key_value(api_dump, 3, "returnType", funcReturn);
        }
    }
    settings.flush();
}

inline void dump_json_UNUSED(ApiDumpInstance &api_dump, const char *type_string, const char *name, int indents) {
    dump_json_start_object(api_dump, indents);
    dump_json_key_value(api_dump, indents + 1, "type", type_string);
    dump_separate_members(api_dump);
    dump_json_key_value(api_dump, indents + 1, "name", name);
    dump_separate_members(api_dump);
    dump_json_key_value(api_dump, indents + 1, "address", "UNUSED");
    dump_separate_members(api_dump);
    dump_json_key_value(api_dump, indents + 1, "value", "UNUSED");
    dump_json_end_object(api_dump, indents);
}
