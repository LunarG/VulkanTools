/* Copyright (c) 2015-2019 The Khronos Group Inc.
 * Copyright (c) 2015-2019 Valve Corporation
 * Copyright (c) 2015-2019 LunarG, Inc.
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

#define NOMINMAX

#include "vk_loader_platform.h"
#include "vulkan/vk_layer.h"
#include "vk_layer_config.h"
#include "vk_layer_table.h"
#include "vk_layer_extension_utils.h"
#include "vk_layer_utils.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string.h>
#include <string>
#include <type_traits>
#include <map>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <utility>

#ifdef ANDROID

#include <android/log.h>
#include <sys/system_properties.h>

#endif  // ANDROID

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

    inline NumberToken parseNumber(std::string str, uint32_t current_char) {
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

    inline void printErrorMsg(const char *msg) {
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
    ApiDumpSettings() {
        std::string filename_string = "";
        // If the layer settings file has a flag indicating to output to a file,
        // do so, to the appropriate filename.
        const char *file_option = getLayerOption("lunarg_api_dump.file");
        if (file_option != NULL && strcmp(file_option, "TRUE") == 0) {
            const char *filename_option = getLayerOption("lunarg_api_dump.log_filename");
            if (filename_option != NULL && strcmp(filename_option, "") != 0) {
                filename_string = filename_option;
            } else {
                filename_string = "vk_apidump.txt";
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
            use_cout = false;
            output_stream.open(filename_string, std::ofstream::out | std::ostream::trunc);
            size_t last_slash_idx = filename_string.find_last_of("\\/");
            if (std::string::npos != last_slash_idx) {
                output_dir = filename_string.substr(0, last_slash_idx + 1);
            }
        } else {
            // Otherwise, fallback to cout only
            use_cout = true;
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
        show_type = readBoolOption("lunarg_api_dump.show_types", true);
        name_size = std::max(readIntOption("lunarg_api_dump.name_size", 32), 0);
        type_size = std::max(readIntOption("lunarg_api_dump.type_size", 0), 0);
        use_spaces = readBoolOption("lunarg_api_dump.use_spaces", true);
        show_shader = readBoolOption("lunarg_api_dump.show_shader", false);

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

        // Generate HTML heading if specified
        if (output_format == ApiDumpFormat::Html) {
            // clang-format off
            // Insert html heading
            stream() <<
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
            stream() << "[\n";
        }

        if (isFrameInRange(0)) {
            setupInterFrameOutputFormatting(0);
        }
    }

    ~ApiDumpSettings() {
        if (output_format == ApiDumpFormat::Html) {
            // Close off html
            stream() << "</div></body></html>";
        } else if (output_format == ApiDumpFormat::Json) {
            // Close off json
            stream() << "\n]" << std::endl;
        }
        if (!use_cout) output_stream.close();
    }

    void setupInterFrameOutputFormatting(uint64_t frame_count) const /*name change? */
    {
        static bool hasPrintedAFrame = false;
        switch (format()) {
            case (ApiDumpFormat::Html):
                if (frame_count > 0) {
                    if (condFrameOutput.isFrameInRange(frame_count - 1)) stream() << "</details>";
                }
                if (condFrameOutput.isFrameInRange(frame_count))
                    stream() << "<details class='frm'><summary>Frame " << frame_count << "</summary>";
                break;

            case (ApiDumpFormat::Json):

                if (frame_count > 0) {
                    if (condFrameOutput.isFrameInRange(frame_count - 1)) stream() << "\n" << indentation(1) << "]\n}";
                }
                if (condFrameOutput.isFrameInRange(frame_count)) {
                    if (!hasPrintedAFrame) {
                        hasPrintedAFrame = true;
                    } else {
                        stream() << ",\n";
                    }
                    stream() << "{\n" << indentation(1) << "\"frameNumber\" : \"" << frame_count << "\",\n";
                    stream() << indentation(1) << "\"apiCalls\" :\n";
                    stream() << indentation(1) << "[\n";
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
                stream() << "</details>";
                break;
            case (ApiDumpFormat::Json):
                stream() << "\n" << indentation(1) << "]\n}";
                break;
            case (ApiDumpFormat::Text):
                break;
            default:
                break;
        }
    }

    inline ApiDumpFormat format() const { return output_format; }

    std::ostream &formatNameType(std::ostream &stream, int indents, const char *name, const char *type) const {
        stream << indentation(indents) << name << ": ";

        if (use_spaces)
            stream << spaces(name_size - (int)strlen(name) - 2);
        else
            stream << tabs((name_size - (int)strlen(name) - 3 + indent_size) / indent_size);

        if (show_type && use_spaces)
            stream << type << spaces(type_size - (int)strlen(type));
        else if (show_type && !use_spaces)
            stream << type << tabs((type_size - (int)strlen(type) - 1 + indent_size) / indent_size);

        return stream << " = ";
    }

    inline const char *indentation(int indents) const {
        if (use_spaces)
            return spaces(indents * indent_size);
        else
            return tabs(indents);
    }

    inline bool shouldFlush() const { return should_flush; }

    inline bool showAddress() const { return show_address; }

    inline bool showParams() const { return show_params; }

    inline bool showShader() const { return show_shader; }

    inline bool showType() const { return show_type; }

    inline bool showTimestamp() const { return show_timestamp; }

    inline std::ostream &stream() const { return use_cout ? std::cout : *(std::ofstream *)&output_stream; }

    inline std::string directory() const { return output_dir; }

    inline bool isFrameInRange(uint64_t frame) const { return condFrameOutput.isFrameInRange(frame); }

   private:
    // Utility member to enable easier comparison by forcing a string to all lower-case
    inline std::string ToLowerString(const std::string &value) {
        std::string lower_value = value;
        std::transform(lower_value.begin(), lower_value.end(), lower_value.begin(), ::tolower);
        return lower_value;
    }

    // Utility member for getting a platform environment variable on various platforms.
    inline std::string GetPlatformEnvVar(const std::string &var) {
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
    inline bool GetStringBooleanValue(const std::string &value) {
        if (ToLowerString(value) == "true") {
            return true;
        }
        return false;
    }

    inline static bool readBoolOption(const char *option, bool default_value) {
        const char *string_option = getLayerOption(option);
        if (string_option != NULL && strcmp(string_option, "TRUE") == 0)
            return true;
        else if (string_option != NULL && strcmp(string_option, "FALSE") == 0)
            return false;
        else
            return default_value;
    }

    inline static int readIntOption(const char *option, int default_value) {
        const char *string_option = getLayerOption(option);
        int value;
        if (sscanf(string_option, "%d", &value) != 1) {
            return default_value;
        } else {
            return value;
        }
    }

    inline static ApiDumpFormat readFormatOption(const char *option, ApiDumpFormat default_value) {
        const char *string_option = getLayerOption(option);
        if (strcmp(string_option, "Text") == 0)
            return ApiDumpFormat::Text;
        else if (strcmp(string_option, "Html") == 0)
            return ApiDumpFormat::Html;
        else if (strcmp(string_option, "Json") == 0)
            return ApiDumpFormat::Json;
        else
            return default_value;
    }

    inline static const char *spaces(int count) { return SPACES + (MAX_SPACES - std::max(count, 0)); }

    inline static const char *tabs(int count) { return TABS + (MAX_TABS - std::max(count, 0)); }

    bool use_cout;
    std::string output_dir = "";
    std::ofstream output_stream;
    ApiDumpFormat output_format;
    bool show_params;
    bool show_address;
    bool should_flush;
    bool show_timestamp;

    bool show_type;
    int indent_size;
    int name_size;
    int type_size;
    bool use_spaces;
    bool show_shader;

    bool use_conditional_output = false;
    ConditionalFrameOutput condFrameOutput;

    static const char *const SPACES;
    static const int MAX_SPACES = 144;
    static const char *const TABS;
    static const int MAX_TABS = 36;
};

const char *const ApiDumpSettings::SPACES =
    "                                                                                                                          "
    "    "
    "                  ";
const char *const ApiDumpSettings::TABS = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

class ApiDumpInstance {
   public:
    inline ApiDumpInstance() : dump_settings(NULL), frame_count(0), thread_count(0) {
        loader_platform_thread_create_mutex(&output_mutex);
        loader_platform_thread_create_mutex(&frame_mutex);
        loader_platform_thread_create_mutex(&thread_mutex);
        loader_platform_thread_create_mutex(&cmd_buffer_state_mutex);

        program_start = std::chrono::system_clock::now();
    }

    inline ~ApiDumpInstance() {
        if (!first_func_call_on_frame) settings().closeFrameOutput();

        if (dump_settings != NULL) delete dump_settings;

        loader_platform_thread_delete_mutex(&thread_mutex);
        loader_platform_thread_delete_mutex(&frame_mutex);
        loader_platform_thread_delete_mutex(&output_mutex);
        loader_platform_thread_delete_mutex(&cmd_buffer_state_mutex);
    }

    inline uint64_t frameCount() {
        loader_platform_thread_lock_mutex(&frame_mutex);
        uint64_t count = frame_count;
        loader_platform_thread_unlock_mutex(&frame_mutex);
        return count;
    }

    inline void nextFrame() {
        loader_platform_thread_lock_mutex(&frame_mutex);
        ++frame_count;

        should_dump_output = settings().isFrameInRange(frame_count);
        settings().setupInterFrameOutputFormatting(frame_count);
        first_func_call_on_frame = true;

        loader_platform_thread_unlock_mutex(&frame_mutex);
    }

    inline bool shouldDumpOutput() {
        if (!conditional_initialized) {
            should_dump_output = settings().isFrameInRange(frame_count);
            conditional_initialized = true;
        }
        return should_dump_output;
    }

    inline bool firstFunctionCallOnFrame() {
        if (first_func_call_on_frame) {
            first_func_call_on_frame = false;
            return true;
        }
        return false;
    }

    inline loader_platform_thread_mutex *outputMutex() { return &output_mutex; }

    inline const ApiDumpSettings &settings() {
        if (dump_settings == NULL) dump_settings = new ApiDumpSettings();

        return *dump_settings;
    }

    // Only used in vktracedump to print thread id in trace file
    void setThreadID(uint64_t trace_thread_id) { thread_id = trace_thread_id; }

    uint64_t threadID() {
        if (thread_id != UINT64_MAX) {
            return thread_id;
        }
        loader_platform_thread_id id = loader_platform_get_thread_id();
        loader_platform_thread_lock_mutex(&thread_mutex);
        for (uint32_t i = 0; i < thread_count; ++i) {
            if (thread_map[i] == id) {
                loader_platform_thread_unlock_mutex(&thread_mutex);
                return i;
            }
        }

        uint32_t new_index = thread_count;
        thread_map[thread_count++] = id;
        assert(thread_count < MAX_THREADS);
        loader_platform_thread_unlock_mutex(&thread_mutex);
        return new_index;
    }

    inline VkCommandBufferLevel getCmdBufferLevel(VkCommandBuffer cmd_buffer) {
        loader_platform_thread_lock_mutex(&cmd_buffer_state_mutex);

        const auto level_iter = cmd_buffer_level.find(cmd_buffer);
        assert(level_iter != cmd_buffer_level.end());
        const auto level = level_iter->second;

        loader_platform_thread_unlock_mutex(&cmd_buffer_state_mutex);
        return level;
    }

    inline void eraseCmdBuffers(VkDevice device, VkCommandPool cmd_pool, std::vector<VkCommandBuffer> cmd_buffers) {
        cmd_buffers.erase(std::remove(cmd_buffers.begin(), cmd_buffers.end(), nullptr), cmd_buffers.end());
        if (!cmd_buffers.empty()) {
            loader_platform_thread_lock_mutex(&cmd_buffer_state_mutex);

            const auto pool_cmd_buffers_iter = cmd_buffer_pools.find(std::make_pair(device, cmd_pool));
            assert(pool_cmd_buffers_iter != cmd_buffer_pools.end());

            for (const auto cmd_buffer : cmd_buffers) {
                pool_cmd_buffers_iter->second.erase(cmd_buffer);

                assert(cmd_buffer_level.count(cmd_buffer) > 0);
                cmd_buffer_level.erase(cmd_buffer);
            }

            loader_platform_thread_unlock_mutex(&cmd_buffer_state_mutex);
        }
    }

    inline void addCmdBuffers(VkDevice device, VkCommandPool cmd_pool, std::vector<VkCommandBuffer> cmd_buffers,
                              VkCommandBufferLevel level) {
        loader_platform_thread_lock_mutex(&cmd_buffer_state_mutex);

        auto &pool_cmd_buffers = cmd_buffer_pools[std::make_pair(device, cmd_pool)];
        pool_cmd_buffers.insert(cmd_buffers.begin(), cmd_buffers.end());

        for (const auto cmd_buffer : cmd_buffers) {
            assert(cmd_buffer_level.count(cmd_buffer) == 0);
            cmd_buffer_level[cmd_buffer] = level;
        }

        loader_platform_thread_unlock_mutex(&cmd_buffer_state_mutex);
    }

    inline void eraseCmdBufferPool(VkDevice device, VkCommandPool cmd_pool) {
        if (cmd_pool != VK_NULL_HANDLE) {
            loader_platform_thread_lock_mutex(&cmd_buffer_state_mutex);

            const auto cmd_buffers_iter = cmd_buffer_pools.find(std::make_pair(device, cmd_pool));
            if (cmd_buffers_iter != cmd_buffer_pools.end()) {
                for (const auto cmd_buffer : cmd_buffers_iter->second) {
                    assert(cmd_buffer_level.count(cmd_buffer) > 0);
                    cmd_buffer_level.erase(cmd_buffer);
                }
                cmd_buffers_iter->second.clear();
            }

            loader_platform_thread_unlock_mutex(&cmd_buffer_state_mutex);
        }
    }

    inline std::chrono::microseconds current_time_since_start() {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now - program_start);
    }

    static inline ApiDumpInstance &current() { return current_instance; }

    std::unordered_map<uint64_t, std::string> object_name_map;

   private:
    static ApiDumpInstance current_instance;

    ApiDumpSettings *dump_settings;
    loader_platform_thread_mutex output_mutex;
    loader_platform_thread_mutex frame_mutex;
    uint64_t frame_count;

    static const size_t MAX_THREADS = 513;
    loader_platform_thread_mutex thread_mutex;
    loader_platform_thread_id thread_map[MAX_THREADS];
    uint32_t thread_count;
    uint64_t thread_id = UINT64_MAX;

    loader_platform_thread_mutex cmd_buffer_state_mutex;
    std::map<std::pair<VkDevice, VkCommandPool>, std::unordered_set<VkCommandBuffer> > cmd_buffer_pools;
    std::unordered_map<VkCommandBuffer, VkCommandBufferLevel> cmd_buffer_level;

    bool conditional_initialized = false;
    bool should_dump_output = true;
    bool first_func_call_on_frame = false;

    std::chrono::system_clock::time_point program_start;
};

ApiDumpInstance ApiDumpInstance::current_instance;

//==================================== Text Backend Helpers ======================================//

template <typename T, typename... Args>
inline void dump_text_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string,
                            const char *child_type, const char *name, int indents,
                            std::ostream &(*dump)(const T, const ApiDumpSettings &, int, Args... args), Args... args) {
    settings.formatNameType(settings.stream(), indents, name, type_string);
    if (array == NULL) {
        settings.stream() << "NULL\n";
        return;
    }
    if (settings.showAddress())
        settings.stream() << (void *)array << "\n";
    else
        settings.stream() << "address\n";

    for (size_t i = 0; i < len && array != NULL; ++i) {
        std::stringstream stream;
        stream << name << '[' << i << ']';
        std::string indexName = stream.str();
        dump_text_value(array[i], settings, child_type, indexName.c_str(), indents + 1, dump, args...);
    }
}

template <typename T, typename... Args>
inline void dump_text_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string,
                            const char *child_type, const char *name, int indents,
                            std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args), Args... args) {
    settings.formatNameType(settings.stream(), indents, name, type_string);
    if (array == NULL) {
        settings.stream() << "NULL\n";
        return;
    }
    if (settings.showAddress())
        settings.stream() << (void *)array << "\n";
    else
        settings.stream() << "address\n";

    for (size_t i = 0; i < len && array != NULL; ++i) {
        std::stringstream stream;
        stream << name << '[' << i << ']';
        std::string indexName = stream.str();
        dump_text_value(array[i], settings, child_type, indexName.c_str(), indents + 1, dump, args...);
    }
}

template <typename T, typename... Args>
inline void dump_text_array_hex(const uint32_t *array, size_t len, const ApiDumpSettings &settings, const char *type_string,
                                const char *child_type, const char *name, int indents,
                                std::ostream &(*dump)(const T, const ApiDumpSettings &, int, Args... args), Args... args) {
    settings.formatNameType(settings.stream(), indents, name, type_string);
    if (array == NULL) {
        settings.stream() << "NULL\n";
        return;
    }
    if (settings.showAddress())
        settings.stream() << (void *)array;
    else
        settings.stream() << "address";

    std::stringstream stream;
    const uint8_t *arraybyte = reinterpret_cast<const uint8_t *>(array);
    for (size_t i = 0; i < (len * 4) && array != NULL; ++i) {
        stream << std::hex << std::setw(2) << std::setfill('0') << (int)arraybyte[i] << " ";
        if (i % 32 == 31) {
            stream << "\n";
        }
    }

    if (settings.stream().rdbuf() == std::cout.rdbuf()) {
        settings.stream() << "\n" << stream.str() << "\n";
    } else {
        static uint64_t shaderDumpIndex = 0;
        std::stringstream shaderDumpFileName;
        shaderDumpFileName << settings.directory() << "shader_" << shaderDumpIndex << ".hex";
        settings.stream() << " (" << shaderDumpFileName.str() << ")\n";
        ++shaderDumpIndex;
        std::ofstream shaderDumpFile;
        shaderDumpFile.open(shaderDumpFileName.str(), std::ofstream::out | std::ostream::trunc);
        shaderDumpFile << stream.str() << "\n";
        shaderDumpFile.close();
    }
}

template <typename T, typename... Args>
inline void dump_text_array_hex(const uint32_t *array, size_t len, const ApiDumpSettings &settings, const char *type_string,
                                const char *child_type, const char *name, int indents,
                                std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args), Args... args) {
    settings.formatNameType(settings.stream(), indents, name, type_string);
    if (array == NULL) {
        settings.stream() << "NULL\n";
        return;
    }
    if (settings.showAddress())
        settings.stream() << (void *)array;
    else
        settings.stream() << "address";

    std::stringstream stream;
    const uint8_t *arraybyte = reinterpret_cast<const uint8_t *>(array);
    for (size_t i = 0; i < (len * 4) && array != NULL; ++i) {
        stream << std::hex << std::setw(2) << std::setfill('0') << (int)arraybyte[i] << " ";
        if (i % 32 == 31) {
            stream << "\n";
        }
    }

    if (settings.stream().rdbuf() == std::cout.rdbuf()) {
        settings.stream() << "\n" << stream.str() << "\n";
    } else {
        static uint64_t shaderDumpIndex = 0;
        std::stringstream shaderDumpFileName;
        shaderDumpFileName << settings.directory() << "shader_" << shaderDumpIndex << ".hex";
        settings.stream() << " (" << shaderDumpFileName.str() << ")\n";
        ++shaderDumpIndex;
        std::ofstream shaderDumpFile;
        shaderDumpFile.open(shaderDumpFileName.str(), std::ofstream::out | std::ostream::trunc);
        shaderDumpFile << stream.str() << "\n";
        shaderDumpFile.close();
    }
}

template <typename T, typename... Args>
inline void dump_text_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name,
                              int indents, std::ostream &(*dump)(const T, const ApiDumpSettings &, int, Args... args),
                              Args... args) {
    if (pointer == NULL) {
        settings.formatNameType(settings.stream(), indents, name, type_string);
        settings.stream() << "NULL\n";
    } else {
        dump_text_value(*pointer, settings, type_string, name, indents, dump, args...);
    }
}

template <typename T, typename... Args>
inline void dump_text_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name,
                              int indents, std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args),
                              Args... args) {
    if (pointer == NULL) {
        settings.formatNameType(settings.stream(), indents, name, type_string);
        settings.stream() << "NULL\n";
    } else {
        dump_text_value(*pointer, settings, type_string, name, indents, dump, args...);
    }
}

template <typename T, typename... Args>
inline void dump_text_value(const T object, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                            std::ostream &(*dump)(const T, const ApiDumpSettings &, int, Args... args), Args... args) {
    settings.formatNameType(settings.stream(), indents, name, type_string);
    dump(object, settings, indents, args...) << "\n";
}

template <typename T, typename... Args>
inline void dump_text_value(const T &object, const ApiDumpSettings &settings, const char *type_string, const char *name,
                            int indents, std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args),
                            Args... args) {
    settings.formatNameType(settings.stream(), indents, name, type_string);
    dump(object, settings, indents, args...);
}

inline void dump_text_special(const char *text, const ApiDumpSettings &settings, const char *type_string, const char *name,
                              int indents) {
    settings.formatNameType(settings.stream(), indents, name, type_string);
    settings.stream() << text << "\n";
}

inline bool dump_text_bitmaskOption(const std::string &option, std::ostream &stream, bool isFirst) {
    if (isFirst)
        stream << " (";
    else
        stream << " | ";
    stream << option;
    return false;
}

inline std::ostream &dump_text_cstring(const char *object, const ApiDumpSettings &settings, int indents) {
    if (object == NULL)
        return settings.stream() << "NULL";
    else
        return settings.stream() << "\"" << object << "\"";
}

inline std::ostream &dump_text_void(const void *object, const ApiDumpSettings &settings, int indents) {
    if (object == NULL)
        return settings.stream() << "NULL";
    else if (settings.showAddress())
        return settings.stream() << object;
    else
        return settings.stream() << "address";
}

inline std::ostream &dump_text_int(int object, const ApiDumpSettings &settings, int indents) { return settings.stream() << object; }

template <typename T, typename... Args>
inline void dump_text_pNext(const T *object, const ApiDumpSettings &settings, const char *type_string, int indents,
                            std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args), Args... args) {
    if (object == NULL)
        settings.stream() << "NULL";
    else if (settings.showAddress()) {
        settings.formatNameType(settings.stream(), indents, "pNext", type_string);
        dump(*object, settings, indents, args...);
    } else
        settings.stream() << "address";
}

//==================================== Html Backend Helpers ======================================//

inline std::ostream &dump_html_nametype(std::ostream &stream, bool showType, const char *name, const char *type) {
    stream << "<div class='var'>" << name << "</div>";
    if (showType) {
        stream << "<div class='type'>" << type << "</div>";
    }
    return stream;
}

template <typename T, typename... Args>
inline void dump_html_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string,
                            const char *child_type, const char *name, int indents,
                            std::ostream &(*dump)(const T, const ApiDumpSettings &, int, Args... args), Args... args) {
    if (array == NULL) {
        settings.stream() << "<details class='data'><summary>";
        dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
        settings.stream() << "<div class='val'>";
        settings.stream() << "NULL</div></summary></details>";
        return;
    }
    settings.stream() << "<details class='data'><summary>";
    dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
    settings.stream() << "<div class='val'>";
    if (settings.showAddress())
        settings.stream() << (void *)array << "\n";
    else
        settings.stream() << "address\n";
    settings.stream() << "</div></summary>";
    for (size_t i = 0; i < len && array != NULL; ++i) {
        std::stringstream stream;
        stream << name << '[' << i << ']';
        std::string indexName = stream.str();
        dump_html_value(array[i], settings, child_type, indexName.c_str(), indents + 1, dump, args...);
    }
    settings.stream() << "</details>";
}

template <typename T, typename... Args>
inline void dump_html_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string,
                            const char *child_type, const char *name, int indents,
                            std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args), Args... args) {
    if (array == NULL) {
        settings.stream() << "<details class='data'><summary>";
        dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
        settings.stream() << "<div class='val'>";
        settings.stream() << "NULL</div></summary></details>";
        return;
    }
    settings.stream() << "<details class='data'><summary>";
    dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
    settings.stream() << "<div class='val'>";
    if (settings.showAddress())
        settings.stream() << (void *)array << "\n";
    else
        settings.stream() << "address\n";
    settings.stream() << "</div></summary>";
    for (size_t i = 0; i < len && array != NULL; ++i) {
        std::stringstream stream;
        stream << name << '[' << i << ']';
        std::string indexName = stream.str();
        dump_html_value(array[i], settings, child_type, indexName.c_str(), indents + 1, dump, args...);
    }
    settings.stream() << "</details>";
}

template <typename T, typename... Args>
inline void dump_html_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name,
                              int indents, std::ostream &(*dump)(const T, const ApiDumpSettings &, int, Args... args),
                              Args... args) {
    if (pointer == NULL) {
        settings.stream() << "<details class='data'><summary>";
        dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
        settings.stream() << "<div class='val'>NULL</div></summary></details>";
    } else {
        dump_html_value(*pointer, settings, type_string, name, indents, dump, args...);
    }
}

template <typename T, typename... Args>
inline void dump_html_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name,
                              int indents, std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args),
                              Args... args) {
    if (pointer == NULL) {
        settings.stream() << "<details class='data'><summary>";
        dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
        settings.stream() << "<div class='val'>NULL</div></summary></details>";
    } else {
        dump_html_value(*pointer, settings, type_string, name, indents, dump, args...);
    }
}

template <typename T, typename... Args>
inline void dump_html_value(const T object, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                            std::ostream &(*dump)(const T, const ApiDumpSettings &, int, Args... args), Args... args) {
    settings.stream() << "<details class='data'><summary>";
    dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
    dump(object, settings, indents, args...);
    settings.stream() << "</details>";
}

template <typename T, typename... Args>
inline void dump_html_value(const T &object, const ApiDumpSettings &settings, const char *type_string, const char *name,
                            int indents, std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args),
                            Args... args) {
    settings.stream() << "<details class='data'><summary>";
    dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
    dump(object, settings, indents, args...);
    settings.stream() << "</details>";
}

inline void dump_html_special(const char *text, const ApiDumpSettings &settings, const char *type_string, const char *name,
                              int indents) {
    settings.stream() << "<details class='data'><summary>";
    dump_html_nametype(settings.stream(), settings.showType(), name, type_string);
    settings.stream() << "<div class='val'>" << text << "</div></summary></details>";
}

inline bool dump_html_bitmaskOption(const std::string &option, std::ostream &stream, bool isFirst) {
    if (isFirst)
        stream << " (";
    else
        stream << " | ";
    stream << option;
    return false;
}

inline std::ostream &dump_html_cstring(const char *object, const ApiDumpSettings &settings, int indents) {
    settings.stream() << "<div class='val'>";
    if (object == NULL)
        settings.stream() << "NULL";
    else
        settings.stream() << "\"" << object << "\"";
    return settings.stream() << "</div>";
}

inline std::ostream &dump_html_void(const void *object, const ApiDumpSettings &settings, int indents) {
    settings.stream() << "<div class='val'>";
    if (object == NULL)
        settings.stream() << "NULL";
    else if (settings.showAddress())
        settings.stream() << object;
    else
        settings.stream() << "address";
    return settings.stream() << "</div>";
}

inline std::ostream &dump_html_int(int object, const ApiDumpSettings &settings, int indents) {
    settings.stream() << "<div class='val'>";
    settings.stream() << object;
    return settings.stream() << "</div>";
}

template <typename T, typename... Args>
inline void dump_html_pNext(const T *object, const ApiDumpSettings &settings, const char *type_string, int indents,
                            std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args), Args... args) {
    if (object == NULL) {
        settings.stream() << "<details class='data'><summary>";
        dump_html_nametype(settings.stream(), settings.showType(), "pNext", type_string);
        settings.stream() << "<div class='val'>NULL</div></summary></details>";
    } else {
        dump_html_value(*object, settings, type_string, "pNext", indents, dump, args...);
    }
}

//==================================== Json Backend Helpers ======================================//

inline void dump_json_nametype(const ApiDumpSettings &settings, bool showType, const char *name, const char *type, int indents) {
    settings.stream() << settings.indentation(indents) << "\"" << name << "\" : ";
}

template <typename T, typename... Args>
inline void dump_json_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string,
                            const char *child_type, const char *name, int indents,
                            std::ostream &(*dump)(const T, const ApiDumpSettings &, int, Args... args), Args... args) {
    if (len == 0 || array == NULL) {
        settings.stream() << settings.indentation(indents) << "{\n";
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"0x0\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"elements\" : [ ]\n";
        settings.stream() << settings.indentation(indents) << "}";
        return;
    }
    settings.stream() << settings.indentation(indents) << "{\n";

    if (len > 0 && array != NULL) {
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        if (settings.showAddress())
            settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"" << (void *)array << "\",\n";
        else
            settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"address\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"elements\" :\n";
        settings.stream() << settings.indentation(indents + 1) << "[\n";
        for (size_t i = 0; i < len && array != NULL; ++i) {
            std::stringstream stream;
            stream << "[" << i << "]";
            std::string indexName = stream.str();
            dump_json_value(array[i], settings, child_type, indexName.c_str(), indents + 2, dump, args...);
            if (i < len - 1) settings.stream() << ',';
            settings.stream() << "\n";
        }
        settings.stream() << settings.indentation(indents + 1) << "]";
    }
    settings.stream() << "\n" << settings.indentation(indents) << "}";
}

template <typename T, typename... Args>
inline void dump_json_array(const T *array, size_t len, const ApiDumpSettings &settings, const char *type_string,
                            const char *child_type, const char *name, int indents,
                            std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args), Args... args) {
    if (len == 0 || array == NULL) {
        settings.stream() << settings.indentation(indents) << "{\n";
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"0x0\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"elements\" : [ ]\n";
        settings.stream() << settings.indentation(indents) << "}";
        return;
    }
    settings.stream() << settings.indentation(indents) << "{\n";
    if (len > 0 && array != NULL) {
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        if (settings.showAddress())
            settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"" << (void *)array << "\",\n";
        else
            settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"address\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"elements\" :\n";
        settings.stream() << settings.indentation(indents + 1) << "[\n";
        for (size_t i = 0; i < len && array != NULL; ++i) {
            std::stringstream stream;
            stream << "[" << i << "]";
            std::string indexName = stream.str();
            dump_json_value(array[i], settings, child_type, indexName.c_str(), indents + 2, dump, args...);
            if (i < len - 1) settings.stream() << ',';
            settings.stream() << "\n";
        }
        settings.stream() << settings.indentation(indents + 1) << "]";
    }
    settings.stream() << "\n" << settings.indentation(indents) << "}";
}

template <typename T, typename... Args>
inline void dump_json_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name,
                              int indents, std::ostream &(*dump)(const T, const ApiDumpSettings &, int, Args... args),
                              Args... args) {
    if (pointer == NULL) {
        settings.stream() << settings.indentation(indents) << "{\n";
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"0x0\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"value\" : \"NULL\"\n";
        settings.stream() << settings.indentation(indents) << "}";
    } else {
        dump_json_value(*pointer, settings, type_string, name, indents, dump, args...);
    }
}

template <typename T, typename... Args>
inline void dump_json_pointer(const T *pointer, const ApiDumpSettings &settings, const char *type_string, const char *name,
                              int indents, std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args),
                              Args... args) {
    if (pointer == NULL) {
        settings.stream() << settings.indentation(indents) << "{\n";
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"0x0\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"value\" : \"NULL\"\n";
        settings.stream() << settings.indentation(indents) << "}";
    } else {
        dump_json_value(*pointer, settings, type_string, name, indents, dump, args...);
    }
}

extern bool is_union(const char *t);
extern bool is_struct(const char *t);

template <typename T, typename... Args>
inline void dump_json_value(const T object, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents,
                            std::ostream &(*dump)(const T, const ApiDumpSettings &, int, Args... args), Args... args) {
    settings.stream() << settings.indentation(indents) << "{\n";
    if (is_union(type_string))
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << " (Union)\",\n";
    else
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
    if (strchr(type_string, '*')) {
        if (settings.showAddress())
            settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"" << &object << "\",\n";
        else
            settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"address\",\n";
    }
    if (is_union(type_string) || is_struct(type_string))
        settings.stream() << settings.indentation(indents + 1) << "\"members\" :\n";
    else
        settings.stream() << settings.indentation(indents + 1) << "\"value\" : ";
    dump(object, settings, indents + 1, args...);
    settings.stream() << "\n" << settings.indentation(indents) << "}";
}

template <typename T, typename... Args>
inline void dump_json_value(const T &object, const ApiDumpSettings &settings, const char *type_string, const char *name,
                            int indents, std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args),
                            Args... args) {
    settings.stream() << settings.indentation(indents) << "{\n";
    if (is_union(type_string))
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << " (Union)\",\n";
    else
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
    if (strchr(type_string, '*')) {
        if (settings.showAddress())
            settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"" << &object << "\",\n";
        else
            settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"address\",\n";
    }
    if (is_union(type_string) || is_struct(type_string))
        settings.stream() << settings.indentation(indents + 1) << "\"members\" :\n";
    else
        settings.stream() << settings.indentation(indents + 1) << "\"value\" : ";
    dump(object, settings, indents + 1, args...);
    settings.stream() << "\n" << settings.indentation(indents) << "}";
}

inline void dump_json_special(const char *text, const ApiDumpSettings &settings, const char *type_string, const char *name,
                              int indents) {
    settings.stream() << settings.indentation(indents) << "{\n";
    settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"name\" : \"" << name << "\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"" << (void *)text << "\",\n";
    settings.stream() << settings.indentation(indents + 1) << "\"value\" : ";
    settings.stream() << "\"" << text << "\"\n";
    settings.stream() << settings.indentation(indents) << "}";
}

inline bool dump_json_bitmaskOption(const std::string &option, std::ostream &stream, bool isFirst) {
    if (isFirst)
        stream << " (";
    else
        stream << " | ";
    stream << option;
    return false;
}

inline std::ostream &dump_json_cstring(const char *object, const ApiDumpSettings &settings, int indents) {
    if (object == NULL)
        settings.stream() << "\"\"";
    else
        settings.stream() << "\"" << object << "\"";
    return settings.stream();
}

inline std::ostream &dump_json_void(const void *object, const ApiDumpSettings &settings, int indents) {
    if (object == NULL)
        settings.stream() << "\"NULL\"";
    else if (settings.showAddress())
        settings.stream() << '"' << object << "\"";
    else
        settings.stream() << "\"address\"";
    return settings.stream();
}

inline std::ostream &dump_json_int(int object, const ApiDumpSettings &settings, int indents) {
    settings.stream() << settings.indentation(indents) << "\"value\" : ";
    settings.stream() << '"' << object << "\"";
    return settings.stream();
}

template <typename T, typename... Args>
inline void dump_json_pNext(const T *object, const ApiDumpSettings &settings, const char *type_string, int indents,
                            std::ostream &(*dump)(const T &, const ApiDumpSettings &, int, Args... args), Args... args) {
    if (object == NULL) {
        settings.stream() << settings.indentation(indents) << "{\n";
        settings.stream() << settings.indentation(indents + 1) << "\"type\" : \"" << type_string << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"name\" : \""
                          << "pNext"
                          << "\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"address\" : \"0x0\",\n";
        settings.stream() << settings.indentation(indents + 1) << "\"value\" : \"NULL\"\n";
        settings.stream() << settings.indentation(indents) << "}";
    } else {
        dump_json_value(*object, settings, type_string, "pNext", indents, dump, args...);
    }
}