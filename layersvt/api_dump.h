/* Copyright (c) 2015-2023 The Khronos Group Inc.
 * Copyright (c) 2015-2023 Valve Corporation
 * Copyright (c) 2015-2023 LunarG, Inc.
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

#ifndef NOMINMAX
#define NOMINMAX
#endif  // NOMINMAX

#include "vulkan/vk_layer.h"
#include "vk_layer_config.h"
#include "vk_layer_table.h"
#include "vk_layer_extension_utils.h"
#include "vk_layer_utils.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <mutex>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string.h>
#include <string>
#include <type_traits>
#include <map>
#include <thread>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <utility>

#ifdef ANDROID

#include <android/log.h>
#include <sys/system_properties.h>
// Disable warning about bitshift precedence
#pragma GCC diagnostic ignored "-Wshift-op-parentheses"

#endif  // ANDROID

#if defined(WIN32)
// Disable warning about bitshift precedence
#pragma warning(disable : 4554)
#endif

#define MAX_STRING_LENGTH 1024

// Defines for utilized environment variables.
#define API_DUMP_ENV_VAR_LOG_FILE "VK_APIDUMP_LOG_FILENAME"
#define API_DUMP_ENV_VAR_OUTPUT_FMT "VK_APIDUMP_OUTPUT_FORMAT"
#define API_DUMP_ENV_VAR_DETAILED_OUTPUT "VK_APIDUMP_DETAILED"
#define API_DUMP_ENV_VAR_NO_ADDRESSES "VK_APIDUMP_NO_ADDR"
#define API_DUMP_ENV_VAR_FLUSH_FILE "VK_APIDUMP_FLUSH"
#define API_DUMP_ENV_VAR_OUTPUT_RANGE "VK_APIDUMP_OUTPUT_RANGE"
#define API_DUMP_ENV_VAR_TIMESTAMP "VK_APIDUMP_TIMESTAMP"

enum class ApiDumpFormat {
    Text,
    Html,
    Json,
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

class ApiDumpSettings {
   public:
    ApiDumpSettings() : output_stream(std::cout.rdbuf()) {
        std::string filename_string = "";
        // If the layer settings file has a flag indicating to output to a file,
        // do so, to the appropriate filename.
        const char *file_option = getLayerOption("lunarg_api_dump.file");
        if (file_option != NULL) {
            std::string lowered_option = ToLowerString(std::string(file_option));
            if (lowered_option == "true") {
                const char *filename_option = getLayerOption("lunarg_api_dump.log_filename");
                if (filename_option != NULL && strcmp(filename_option, "") != 0) {
                    filename_string = filename_option;
                } else {
                    filename_string = "vk_apidump.txt";
                }
            }
        }
        // If an environment variable is set, always output to that filename instead,
        // whether or not the settings file enables the option.  Just assume a non-empty
        // string is asking for the file output to the given name.
        std::string env_value = GetPlatformEnvVar(API_DUMP_ENV_VAR_LOG_FILE);
        if (!env_value.empty()) {
            filename_string = env_value;
        }
        // If one of the above has set a filename, open the file as an output stream.
        if (!filename_string.empty()) {
            output_file_stream.open(filename_string, std::ofstream::out | std::ostream::trunc);
            output_stream.rdbuf(output_file_stream.rdbuf());
        }

        // Get the remaining settings (some we also want to provide the ability to override
        // using environment variables).

        output_format = readFormatOption("lunarg_api_dump.output_format", ApiDumpFormat::Text);
        env_value = GetPlatformEnvVar(API_DUMP_ENV_VAR_OUTPUT_FMT);
        if (!env_value.empty()) {
            if (ToLowerString(env_value) == "html") {
                output_format = ApiDumpFormat::Html;
            } else if (ToLowerString(env_value) == "json") {
                output_format = ApiDumpFormat::Json;
            } else {
                output_format = ApiDumpFormat::Text;
            }
        }

        show_params = readBoolOption("lunarg_api_dump.detailed", true);
        env_value = GetPlatformEnvVar(API_DUMP_ENV_VAR_DETAILED_OUTPUT);
        if (!env_value.empty()) {
            show_params = GetStringBooleanValue(env_value);
        }

        show_address = !readBoolOption("lunarg_api_dump.no_addr", false);
        env_value = GetPlatformEnvVar(API_DUMP_ENV_VAR_NO_ADDRESSES);
        if (!env_value.empty()) {
            show_address = !GetStringBooleanValue(env_value);
        }

        should_flush = readBoolOption("lunarg_api_dump.flush", true);
        env_value = GetPlatformEnvVar(API_DUMP_ENV_VAR_FLUSH_FILE);
        if (!env_value.empty()) {
            should_flush = !GetStringBooleanValue(env_value);
        }

        show_timestamp = readBoolOption("lunarg_api_dump.show_timestamp", false);
        env_value = GetPlatformEnvVar(API_DUMP_ENV_VAR_TIMESTAMP);
        if (!env_value.empty()) {
            show_timestamp = GetStringBooleanValue(env_value);
        }

        indent_size = std::max(readIntOption("lunarg_api_dump.indent_size", 4), 0);
        tab_size = indent_size;
        show_type = readBoolOption("lunarg_api_dump.show_types", true);
        name_size = std::max(readIntOption("lunarg_api_dump.name_size", 32), 0);
        type_size = std::max(readIntOption("lunarg_api_dump.type_size", 0), 0);
        use_spaces = readBoolOption("lunarg_api_dump.use_spaces", true);
        show_shader = readBoolOption("lunarg_api_dump.show_shader", false);
        show_thread_and_frame = readBoolOption("lunarg_api_dump.show_thread_and_frame", true);

        std::string cond_range_string;
        env_value = GetPlatformEnvVar(API_DUMP_ENV_VAR_OUTPUT_RANGE);
        if (!env_value.empty()) {
            cond_range_string = env_value;
        } else {
            cond_range_string = getLayerOption("lunarg_api_dump.output_range");
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
                            "<img src='https://lunarg.com/wp-content/uploads/2016/02/LunarG-wReg-150.png' />"
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
    }

    ~ApiDumpSettings() {
        if (output_format == ApiDumpFormat::Html) {
            // Close off html
            output_stream << "</div></body></html>";
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

    bool shouldFlush() const { return should_flush; }

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

   private:
    // Utility member to enable easier comparison by forcing a string to all lower-case
    static std::string ToLowerString(const std::string &value) {
        std::string lower_value = value;
        std::transform(lower_value.begin(), lower_value.end(), lower_value.begin(), ::tolower);
        return lower_value;
    }

    // Utility member for getting a platform environment variable on various platforms.
    std::string GetPlatformEnvVar(const std::string &var) {
        std::string ret_string = "";
#ifdef _WIN32
        char temp[MAX_STRING_LENGTH];
        int bytes = GetEnvironmentVariableA(var.c_str(), temp, MAX_STRING_LENGTH - 1);
        if (0 < bytes) {
            ret_string = temp;
        }
#elif defined(__ANDROID__)
        std::string command = "getprop debug.";
        std::string lower_var = ToLowerString(var);

        // Remove any prefix "VK_" for Android properties
        if (lower_var.rfind("vk_", 0) == 0) {
            lower_var = lower_var.substr(3);
        }
        command += lower_var;

        FILE *pipe = popen(command.c_str(), "r");
        if (pipe != nullptr) {
            char result[255];
            result[0] = '\0';
            fgets(result, 255, pipe);
            pclose(pipe);
            size_t count = strcspn(result, "\r\n");
            if (count > 0) {
                ret_string = std::string(result, count);
            }
        }
#else
        const char *ret_value = getenv(var.c_str());
        if (nullptr != ret_value) {
            ret_string = ret_value;
        }
#endif
        return ret_string;
    }

    // Utility member to convert from string to a boolean
    bool GetStringBooleanValue(const std::string &value) {
        auto lower_str = ToLowerString(value);
        if (lower_str == "true") {
            return true;
        }
        if (lower_str == "on") {
            return true;
        }
        if (lower_str == "1") {
            return true;
        }
        return false;
    }

    static bool readBoolOption(const char *option, bool default_value) {
        const char *string_option = getLayerOption(option);
        if (string_option == NULL) return default_value;
        std::string lowered_option = ToLowerString(std::string(string_option));
        if (lowered_option == "true")
            return true;
        else if (lowered_option == "false")
            return false;
        else
            return default_value;
    }

    static int readIntOption(const char *option, int default_value) {
        const char *string_option = getLayerOption(option);
        int value;
        if (sscanf(string_option, "%d", &value) != 1) {
            return default_value;
        } else {
            return value;
        }
    }

    static ApiDumpFormat readFormatOption(const char *option, ApiDumpFormat default_value) {
        const char *string_option = getLayerOption(option);
        std::string lowered_option = ToLowerString(std::string(string_option));
        if (lowered_option == "text")
            return ApiDumpFormat::Text;
        else if (lowered_option == "html")
            return ApiDumpFormat::Html;
        else if (lowered_option == "json")
            return ApiDumpFormat::Json;
        else
            return default_value;
    }

    // The mutable is necessary because everyone who 'writes' to the stream necessarily must be able to modify it.
    // Since basically every function in this struct is const, we have to work around that.
    mutable std::ostream output_stream;
    std::ofstream output_file_stream;
    ApiDumpFormat output_format;
    bool show_params;
    bool show_address;
    bool should_flush;
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

    uint64_t frameCount() {
        std::lock_guard<std::recursive_mutex> lg(frame_mutex);
        uint64_t count = frame_count;
        return count;
    }

    void nextFrame() {
        std::lock_guard<std::recursive_mutex> lg(frame_mutex);
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

    std::recursive_mutex *outputMutex() { return &output_mutex; }

    ApiDumpSettings &settings() { return dump_settings; }

    uint64_t threadID() {
        std::thread::id this_id = std::this_thread::get_id();
        std::lock_guard<std::recursive_mutex> lg(thread_mutex);

        auto it = thread_map.find(this_id);
        if (it != thread_map.end()) {
            return it->second;
        }

        thread_map.insert({this_id, thread_map.size()});
        return thread_map.size() - 1;
    }

    void setCmdBuffer(VkCommandBuffer cmd_buffer) { this->cmd_buffer = cmd_buffer; }

    VkCommandBufferLevel getCmdBufferLevel() {
        std::lock_guard<std::recursive_mutex> lg(cmd_buffer_state_mutex);
        const auto level_iter = cmd_buffer_level.find(cmd_buffer);
        assert(level_iter != cmd_buffer_level.end());
        const auto level = level_iter->second;
        return level;
    }

    void eraseCmdBuffers(VkDevice device, VkCommandPool cmd_pool, std::vector<VkCommandBuffer> cmd_buffers) {
        cmd_buffers.erase(std::remove(cmd_buffers.begin(), cmd_buffers.end(), nullptr), cmd_buffers.end());
        if (!cmd_buffers.empty()) {
            std::lock_guard<std::recursive_mutex> lg(cmd_buffer_state_mutex);

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
        std::lock_guard<std::recursive_mutex> lg(cmd_buffer_state_mutex);
        auto &pool_cmd_buffers = cmd_buffer_pools[std::make_pair(device, cmd_pool)];
        pool_cmd_buffers.insert(cmd_buffers.begin(), cmd_buffers.end());

        for (const auto cmd_buffer : cmd_buffers) {
            assert(cmd_buffer_level.count(cmd_buffer) == 0);
            cmd_buffer_level[cmd_buffer] = level;
        }
    }

    void eraseCmdBufferPool(VkDevice device, VkCommandPool cmd_pool) {
        if (cmd_pool != VK_NULL_HANDLE) {
            std::lock_guard<std::recursive_mutex> lg(cmd_buffer_state_mutex);

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
    uint32_t getMemoryHeapCount() { return memory_heap_count; }

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
            object_name_map.emplace(pNameInfo->object, pNameInfo->pObjectName);
        else
            object_name_map.erase(pNameInfo->object);
    }
    void update_object_name_map(const VkDebugUtilsObjectNameInfoEXT *pNameInfo) {
        if (pNameInfo->pObjectName)
            object_name_map.emplace(pNameInfo->objectHandle, pNameInfo->pObjectName);
        else
            object_name_map.erase(pNameInfo->objectHandle);
    }

   private:
    ApiDumpSettings dump_settings;
    std::recursive_mutex output_mutex;
    std::recursive_mutex frame_mutex;
    uint64_t frame_count;

    std::recursive_mutex thread_mutex;
    std::unordered_map<std::thread::id, uint64_t> thread_map;

    std::recursive_mutex cmd_buffer_state_mutex;
    std::map<std::pair<VkDevice, VkCommandPool>, std::unordered_set<VkCommandBuffer> > cmd_buffer_pools;
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
};

// Utility to output an address.
// If the quotes arg is true, the address is encloded in quotes.
// Used for text, html, and json output.
void OutputAddress(const ApiDumpSettings &settings, const void *addr) {
    if (settings.showAddress())
        if (addr == NULL)
            settings.stream() << "NULL";
        else
            settings.stream() << addr;
    else
        settings.stream() << "address";
}

void OutputAddressJSON(const ApiDumpSettings &settings, const void *addr) {
    settings.stream() << "\"";
    OutputAddress(settings, addr);
    settings.stream() << "\"";
}

//==================================== Text Backend Helpers ======================================//

void dump_text_function_head(ApiDumpInstance &dump_inst, const char *funcName, const char *funcNamedParams,
                             const char *funcReturn) {
    const ApiDumpSettings &settings(dump_inst.settings());
    if (settings.showThreadAndFrame()) {
        settings.stream() << "Thread " << dump_inst.threadID() << ", Frame " << dump_inst.frameCount();
    }
    if (settings.showTimestamp() && settings.showThreadAndFrame()) {
        settings.stream() << ", ";
    }
    if (settings.showTimestamp()) {
        settings.stream() << "Time " << dump_inst.current_time_since_start().count() << " us";
    }
    if (settings.showTimestamp() || settings.showThreadAndFrame()) {
        settings.stream() << ":\n";
    }
    settings.stream() << funcName << "(" << funcNamedParams << ") returns " << funcReturn;

    settings.shouldFlush() ? settings.stream() << std::flush : settings.stream();
}

template <typename T>
void dump_text_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string, const char *child_type,
                     const char *name, int indents, void (*dump)(const T, const ApiDumpSettings &, int)) {
    settings.formatNameType(indents, name, type_string);
    if (array == NULL) {
        settings.stream() << "NULL\n";
        return;
    }
    OutputAddress(settings, array);
    settings.stream() << "\n";
    for (size_t i = 0; i < len && array != NULL; ++i) {
        std::stringstream stream;
        stream << name << '[' << i << ']';
        std::string indexName = stream.str();
        dump_text_value(array[i], settings, child_type, indexName.c_str(), indents + 1, dump);
    }
}

template <typename T>
void dump_text_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string, const char *child_type,
                     const char *name, int indents, void (*dump)(const T &, const ApiDumpSettings &, int)) {
    settings.formatNameType(indents, name, type_string);
    if (array == NULL) {
        settings.stream() << "NULL\n";
        return;
    }
    OutputAddress(settings, array);
    settings.stream() << "\n";
    for (size_t i = 0; i < len && array != NULL; ++i) {
        std::stringstream stream;
        stream << name << '[' << i << ']';
        std::string indexName = stream.str();
        dump_text_value(array[i], settings, child_type, indexName.c_str(), indents + 1, dump);
    }
}

template <typename T>
void dump_text_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                       void (*dump)(const T, const ApiDumpSettings &, int)) {
    if (pointer == NULL) {
        settings.formatNameType(indents, name, type_string);
        settings.stream() << "NULL\n";
    } else {
        dump_text_value(*pointer, settings, type_string, name, indents, dump);
    }
}

template <typename T>
void dump_text_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                       void (*dump)(const T &, const ApiDumpSettings &, int)) {
    if (pointer == NULL) {
        settings.formatNameType(indents, name, type_string);
        settings.stream() << "NULL\n";
    } else {
        dump_text_value(*pointer, settings, type_string, name, indents, dump);
    }
}

template <typename T>
void dump_text_value(const T object, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                     void (*dump)(const T, const ApiDumpSettings &, int)) {
    settings.formatNameType(indents, name, type_string);
    dump(object, settings, indents);
    settings.stream() << "\n";
}

template <typename T>
void dump_text_value(const T &object, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                     void (*dump)(const T &, const ApiDumpSettings &, int)) {
    settings.formatNameType(indents, name, type_string);
    dump(object, settings, indents);
}

void dump_text_special(const char *text, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents) {
    settings.formatNameType(indents, name, type_string);
    settings.stream() << text << "\n";
}

void dump_text_cstring(const char *object, const ApiDumpSettings &settings, int indents) {
    if (object == NULL)
        settings.stream() << "NULL";
    else
        settings.stream() << "\"" << object << "\"";
}

void dump_text_void(const void *object, const ApiDumpSettings &settings, int indents) {
    if (object == NULL) {
        settings.stream() << "NULL";
        return;
    }
    OutputAddress(settings, object);
}

void dump_text_int(int object, const ApiDumpSettings &settings, int indents) { settings.stream() << object; }

template <typename T>
void dump_text_pNext(const T *object, const ApiDumpSettings &settings, const char *type_string, int indents,
                     void (*dump)(const T &, const ApiDumpSettings &, int)) {
    if (object == NULL) {
        settings.stream() << "NULL";
    } else {
        settings.formatNameType(indents, "pNext", type_string);
        dump(*object, settings, indents);
    }
}

//==================================== Html Backend Helpers ======================================//

void dump_html_nametype(std::ostream &stream, bool showType, const char *name, const char *type) {
    stream << "<div class='var'>" << name << "</div>";
    if (showType) {
        stream << "<div class='type'>" << type << "</div>";
    }
}

void dump_html_function_head(ApiDumpInstance &dump_inst, const char *funcName, const char *funcNamedParams,
                             const char *funcReturn) {
    const ApiDumpSettings &settings(dump_inst.settings());
    if (settings.showThreadAndFrame()) {
        settings.stream() << "<div class='thd'>Thread: " << dump_inst.threadID() << "</div>";
    }
    if (settings.showTimestamp())
        settings.stream() << "<div class='time'>Time: " << dump_inst.current_time_since_start().count() << " us</div>";
    settings.stream() << "<details class='fn'><summary>";
    settings.stream() << "<div class='var'>" << funcName << "(" << funcNamedParams << ")</div>";
    if (settings.showType()) {
        settings.stream() << "<div class='type'>" << funcReturn << "</div>";
    }
    settings.shouldFlush() ? settings.stream() << std::flush : settings.stream();
}

template <typename T>
void dump_html_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string, const char *child_type,
                     const char *name, int indents, void (*dump)(const T, const ApiDumpSettings &, int)) {
    if (array == NULL) {
        settings.stream() << "<details class='data'><summary>";
        dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
        settings.stream() << "<div class='val'>NULL</div></summary></details>";
        return;
    }
    settings.stream() << "<details class='data'><summary>";
    dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
    settings.stream() << "<div class='val'>";
    OutputAddress(settings, array);
    settings.stream() << "\n";
    settings.stream() << "</div></summary>";
    for (size_t i = 0; i < len && array != NULL; ++i) {
        std::stringstream stream;
        stream << name << '[' << i << ']';
        std::string indexName = stream.str();
        dump_html_value(array[i], settings, child_type, indexName.c_str(), indents + 1, dump);
    }
    settings.stream() << "</details>";
}

template <typename T>
void dump_html_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string, const char *child_type,
                     const char *name, int indents, void (*dump)(const T &, const ApiDumpSettings &, int)) {
    if (array == NULL) {
        settings.stream() << "<details class='data'><summary>";
        dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
        settings.stream() << "<div class='val'>NULL</div></summary></details>";
        return;
    }
    settings.stream() << "<details class='data'><summary>";
    dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
    settings.stream() << "<div class='val'>";
    OutputAddress(settings, array);
    settings.stream() << "\n";
    settings.stream() << "</div></summary>";
    for (size_t i = 0; i < len && array != NULL; ++i) {
        std::stringstream stream;
        stream << name << '[' << i << ']';
        std::string indexName = stream.str();
        dump_html_value(array[i], settings, child_type, indexName.c_str(), indents + 1, dump);
    }
    settings.stream() << "</details>";
}

template <typename T>
void dump_html_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                       void (*dump)(const T, const ApiDumpSettings &, int)) {
    if (pointer == NULL) {
        settings.stream() << "<details class='data'><summary>";
        dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
        settings.stream() << "<div class='val'>NULL</div></summary></details>";
    } else {
        dump_html_value(*pointer, settings, type_string, name, indents, dump);
    }
}

template <typename T>
void dump_html_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                       void (*dump)(const T &, const ApiDumpSettings &, int)) {
    if (pointer == NULL) {
        settings.stream() << "<details class='data'><summary>";
        dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
        settings.stream() << "<div class='val'>NULL</div></summary></details>";
    } else {
        dump_html_value(*pointer, settings, type_string, name, indents, dump);
    }
}

template <typename T>
void dump_html_value(const T object, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                     void (*dump)(const T, const ApiDumpSettings &, int)) {
    settings.stream() << "<details class='data'><summary>";
    dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
    dump(object, settings, indents);
    settings.stream() << "</details>";
}

template <typename T>
void dump_html_value(const T &object, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                     void (*dump)(const T &, const ApiDumpSettings &, int)) {
    settings.stream() << "<details class='data'><summary>";
    dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
    dump(object, settings, indents);
    settings.stream() << "</details>";
}

void dump_html_special(const char *text, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents) {
    settings.stream() << "<details class='data'><summary>";
    dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
    settings.stream() << "<div class='val'>" << text << "</div></summary></details>";
}

void dump_html_cstring(const char *object, const ApiDumpSettings &settings, int indents) {
    settings.stream() << "<div class='val'>";
    if (object == NULL)
        settings.stream() << "NULL";
    else
        settings.stream() << "\"" << object << "\"";
    settings.stream() << "</div>";
}

void dump_html_void(const void *object, const ApiDumpSettings &settings, int indents) {
    settings.stream() << "<div class='val'>";
    OutputAddress(settings, object);
    settings.stream() << "</div>";
}

void dump_html_int(int object, const ApiDumpSettings &settings, int indents) {
    settings.stream() << "<div class='val'>";
    settings.stream() << object;
    settings.stream() << "</div>";
}

template <typename T>
void dump_html_pNext(const T *object, const ApiDumpSettings &settings, const char *type_string, int indents,
                     void (*dump)(const T &, const ApiDumpSettings &, int)) {
    if (object == NULL) {
        settings.stream() << "<details class='data'><summary>";
        dump_html_nametype(settings.stream(), settings.showType(), "pNext", type_string);
        settings.stream() << "<div class='val'>NULL</div></summary></details>";
    } else {
        dump_html_value(*object, settings, type_string, "pNext", indents, dump);
    }
}

//==================================== Json Backend Helpers ======================================//

void dump_json_function_head(ApiDumpInstance &dump_inst, const char *funcName, const char *funcReturn) {
    const ApiDumpSettings &settings(dump_inst.settings());

    if (!dump_inst.firstFunctionCallOnFrame()) settings.stream() << ",\n";

    // Display api call name
    settings.stream() << settings.indentation(2) << "{\n";
    settings.stream() << settings.indentation(3) << "\"name\" : \"" << funcName << "\",\n";

    // Display thread info
    if (settings.showThreadAndFrame()) {
        { settings.stream() << settings.indentation(3) << "\"thread\" : \"Thread " << dump_inst.threadID() << "\",\n"; }
    }

    // Display elapsed time
    if (settings.showTimestamp()) {
        {
            settings.stream() << settings.indentation(3) << "\"time\" : \"" << dump_inst.current_time_since_start().count()
                              << " us\",\n";
        }
    }

    // Display return value
    settings.stream() << settings.indentation(3) << "\"returnType\" : \"" << funcReturn << "\"";
    // Add a trailing comma if the return type isn't void or detailed mode is false - JSON doesn't allow trailing commas in object
    if (strcmp("void", funcReturn) != 0 || settings.showParams()) {
        settings.stream() << ",";
    }
    settings.stream() << "\n";

    settings.shouldFlush() ? settings.stream() << std::flush : settings.stream();
}

template <typename T>
void dump_json_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string, const char *child_type,
                     const char *name, int indents, void (*dump)(const T, const ApiDumpSettings &, int)) {
    if (len == 0 || array == NULL) {
        settings.stream() << settings.indentation(indents) << "{\n";
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : ";
        OutputAddressJSON(settings, array);
        settings.stream() << "\n";
        settings.stream() << settings.indentation(indents) << "}";
        return;
    }
    settings.stream() << settings.indentation(indents) << "{\n";

    if (len > 0 && array != NULL) {
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : ";
        OutputAddressJSON(settings, array);
        settings.stream() << ",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"elements\" :\n";
        settings.stream() << settings.indentation(indents + 1) << "[\n";
        for (size_t i = 0; i < len && array != NULL; ++i) {
            std::stringstream stream;
            stream << "[" << i << "]";
            std::string indexName = stream.str();
            dump_json_value(array[i], &array[i], settings, child_type, indexName.c_str(), indents + 2, dump);
            if (i < len - 1) settings.stream() << ',';
            settings.stream() << "\n";
        }
        settings.stream() << settings.indentation(indents + 1) << "]";
    }
    settings.stream() << "\n" << settings.indentation(indents) << "}";
}

template <typename T>
void dump_json_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string, const char *child_type,
                     const char *name, int indents, void (*dump)(const T &, const ApiDumpSettings &, int)) {
    if (len == 0 || array == NULL) {
        settings.stream() << settings.indentation(indents) << "{\n";
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : ";
        OutputAddressJSON(settings, array);
        settings.stream() << "\n";
        settings.stream() << settings.indentation(indents) << "}";
        return;
    }
    settings.stream() << settings.indentation(indents) << "{\n";
    if (len > 0 && array != NULL) {
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : ";
        OutputAddressJSON(settings, array);
        settings.stream() << ",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"elements\" :\n";
        settings.stream() << settings.indentation(indents + 1) << "[\n";
        for (size_t i = 0; i < len && array != NULL; ++i) {
            std::stringstream stream;
            stream << "[" << i << "]";
            std::string indexName = stream.str();
            dump_json_value(array[i], &array[i], settings, child_type, indexName.c_str(), indents + 2, dump);
            if (i < len - 1) settings.stream() << ',';
            settings.stream() << "\n";
        }
        settings.stream() << settings.indentation(indents + 1) << "]";
    }
    settings.stream() << "\n" << settings.indentation(indents) << "}";
}

template <typename T>
void dump_json_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                       void (*dump)(const T, const ApiDumpSettings &, int)) {
    if (pointer == NULL) {
        settings.stream() << settings.indentation(indents) << "{\n";
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : ";
        settings.stream() << (settings.showAddress() ? "\"NULL\"" : "\"address\"") << "\n";
        settings.stream() << settings.indentation(indents) << "}";
    } else {
        dump_json_value(*pointer, pointer, settings, type_string, name, indents, dump);
    }
}

template <typename T>
void dump_json_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                       void (*dump)(const T &, const ApiDumpSettings &, int)) {
    if (pointer == NULL) {
        settings.stream() << settings.indentation(indents) << "{\n";
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : ";
        settings.stream() << (settings.showAddress() ? "\"NULL\"" : "\"address\"") << "\n";
        settings.stream() << settings.indentation(indents) << "}";
    } else {
        dump_json_value(*pointer, pointer, settings, type_string, name, indents, dump);
    }
}

extern bool is_union(const char *t);
extern bool is_struct(const char *t);

template <typename T>
void dump_json_value(const T object, const void *pObject, const ApiDumpSettings &settings, const char *type_string,
                     const char *name, int indents, void (*dump)(const T, const ApiDumpSettings &, int)) {
    bool isPnext = !strcmp(name, "pNext") || !strcmp(name, "pUserData");
    const char *star = (isPnext && !strstr(type_string, "void")) ? "*" : "";
    settings.stream() << settings.indentation(indents) << "{\n";
    if (is_union(type_string))
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << star << " (Union)\",\n";
    else
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << star << "\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\"";
    if (isPnext || (strchr(type_string, '*') && strcmp(type_string, "const char*") && strcmp(type_string, "const char* const"))) {
        // Print pointers, except for char string pointers
        settings.stream() << ",\n" << settings.indentation(indents + 1) << "\"address\" : ";
        OutputAddressJSON(settings, pObject);
    }
    if (!isPnext || (isPnext && pObject != nullptr)) {
        settings.stream() << ",\n";
        if (is_union(type_string) || is_struct(type_string))
            settings.stream() << settings.indentation(indents + 1) << "\"members\" :\n";
        else
            settings.stream() << settings.indentation(indents + 1) << "\"value\" : ";
        dump(object, settings, indents + 1);
    }
    settings.stream() << "\n";
    settings.stream() << settings.indentation(indents) << "}";
}

template <typename T>
void dump_json_value(const T &object, const void *pObject, const ApiDumpSettings &settings, const char *type_string,
                     const char *name, int indents, void (*dump)(const T &, const ApiDumpSettings &, int)) {
    bool isPnext = !strcmp(name, "pNext") || !strcmp(name, "pUserData");
    const char *star = (isPnext && !strstr(type_string, "void")) ? "*" : "";
    settings.stream() << settings.indentation(indents) << "{\n";
    if (is_union(type_string))
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << star << " (Union)\",\n";
    else
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << star << "\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\"";
    if (isPnext || (strchr(type_string, '*') && strcmp(type_string, "const char*") && strcmp(type_string, "const char* const"))) {
        // Print pointers, except for char string pointers
        settings.stream() << ",\n" << settings.indentation(indents + 1) << "\"address\" : ";
        OutputAddressJSON(settings, pObject);
    }
    if (!isPnext || (isPnext && pObject != nullptr)) {
        settings.stream() << ",\n";
        if (is_union(type_string) || is_struct(type_string))
            settings.stream() << settings.indentation(indents + 1) << "\"members\" :\n";
        else
            settings.stream() << settings.indentation(indents + 1) << "\"value\" : ";
        dump(object, settings, indents + 1);
    }
    settings.stream() << "\n";
    settings.stream() << settings.indentation(indents) << "}";
}

void dump_json_special(const char *text, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents) {
    settings.stream() << settings.indentation(indents) << "{\n";
    settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"address\" : ";
    OutputAddressJSON(settings, text);
    settings.stream() << ",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"value\" : ";
    settings.stream() << "\"" << text << "\"\n";
    settings.stream() << settings.indentation(indents) << "}";
}

void dump_json_UNUSED(const ApiDumpSettings &settings, const char *type_string, const char *name, int indents) {
    settings.stream() << settings.indentation(indents) << "{\n";
    settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"UNUSED\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"value\" : \"UNUSED\"\n";
    settings.stream() << settings.indentation(indents) << "}";
}

void dump_json_cstring(const char *object, const ApiDumpSettings &settings, int indents) {
    if (object == NULL)
        settings.stream() << "\"\"";
    else
        settings.stream() << "\"" << object << "\"";
}

void dump_json_void(const void *object, const ApiDumpSettings &settings, int indents) {
    OutputAddressJSON(settings, object);
    settings.stream() << "\n";
}

void dump_json_int(int object, const ApiDumpSettings &settings, int indents) {
    settings.stream() << settings.indentation(indents) << "\"value\" : " << '"' << object << "\"";
    settings.stream() << '"' << object << "\"";
}

template <typename T>
void dump_json_pNext(const T *object, const ApiDumpSettings &settings, const char *type_string, int indents,
                     void (*dump)(const T, const ApiDumpSettings &, int)) {
    if (object == NULL) {
        settings.stream() << settings.indentation(indents) << "{\n";
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "*\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"pNext\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"NULL\",\n";
        settings.stream() << settings.indentation(indents) << "}";
    } else {
        dump_json_value(*object, object, settings, type_string, "pNext", indents, dump);
    }
}

template <typename T>
void dump_json_pNext(const T *object, const ApiDumpSettings &settings, const char *type_string, int indents,
                     void (*dump)(const T &, const ApiDumpSettings &, int)) {
    if (object == NULL) {
        settings.stream() << settings.indentation(indents) << "{\n";
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "*\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"pNext\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"NULL\",\n";
        settings.stream() << settings.indentation(indents) << "}";
    } else {
        dump_json_value(*object, object, settings, type_string, "pNext", indents, dump);
    }
}

//==================================== Common Helpers ======================================//

void dump_function_head(ApiDumpInstance &dump_inst, const char *funcName, const char *funcNamedParams, const char *funcReturn) {
    if (dump_inst.shouldDumpOutput()) {
        switch (dump_inst.settings().format()) {
            case ApiDumpFormat::Text:
                dump_text_function_head(dump_inst, funcName, funcNamedParams, funcReturn);
                break;
            case ApiDumpFormat::Html:
                dump_html_function_head(dump_inst, funcName, funcNamedParams, funcReturn);
                break;
            case ApiDumpFormat::Json:
                dump_json_function_head(dump_inst, funcName, funcReturn);
                break;
        }
    }
}
