/* Copyright (c) 2015-2016 The Khronos Group Inc.
 * Copyright (c) 2015-2016 Valve Corporation
 * Copyright (c) 2015-2016 LunarG, Inc.
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
 */

#pragma once

#define NOMINMAX

#include "vk_loader_platform.h"
#include "vulkan/vk_layer.h"
#include "vk_layer_config.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string.h>
#include <string>
#include <type_traits>

class ApiDumpSettings {
  public:
    ApiDumpSettings() {
        // Get the output file settings and create a stream for it
        const char *file_option = getLayerOption("lunarg_api_dump.file");
        if (file_option != NULL && strcmp(file_option, "TRUE") == 0) {
            use_cout = false;
            const char *filename_option =
                getLayerOption("lunarg_api_dump.log_filename");
            if (filename_option != NULL && strcmp(filename_option, "") != 0)
                output_stream.open(filename_option,
                                   std::ofstream::out | std::ostream::trunc);
            else
                output_stream.open("vk_apidump.txt",
                                   std::ofstream::out | std::ostream::trunc);
        } else {
            use_cout = true;
        }

        // Get the remaining settings
        show_params = readBoolOption("lunarg_api_dump.detailed", true);
        show_address = !readBoolOption("lunarg_api_dump.no_addr", false);
        should_flush = readBoolOption("lunarg_api_dump.flush", true);
        indent_size =
            std::max(readIntOption("lunarg_api_dump.indent_size", 4), 0);
        show_type = readBoolOption("lunarg_api_dump.show_types", true);
        name_size = std::max(readIntOption("lunarg_api_dump.name_size", 32), 0);
        type_size = std::max(readIntOption("lunarg_api_dump.type_size", 0), 0);
        use_spaces = readBoolOption("lunarg_api_dump.use_spaces", true);
        show_shader = readBoolOption("lunarg_api_dump.show_shader", false);
    }

    ~ApiDumpSettings() {
        if (!use_cout)
            output_stream.close();
    }

    std::ostream &formatNameType(std::ostream &stream, int indents,
                                 const char *name, const char *type) const {
        stream << indentation(indents) << name << ": ";

        if (use_spaces)
            stream << spaces(name_size - (int)strlen(name) - 2);
        else
            stream << tabs((name_size - (int)strlen(name) - 3 + indent_size) /
                           indent_size);

        if (show_type && use_spaces)
            stream << type << spaces(type_size - (int)strlen(type));
        else if (show_type && !use_spaces)
            stream << type
                   << tabs((type_size - (int)strlen(type) - 1 + indent_size) /
                           indent_size);

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

    inline std::ostream &stream() const {
        return use_cout ? std::cout : *(std::ofstream *)&output_stream;
    }

  private:
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

    inline static const char *spaces(int count) {
        return SPACES + (MAX_SPACES - std::max(count, 0));
    }

    inline static const char *tabs(int count) {
        return TABS + (MAX_TABS - std::max(count, 0));
    }

    bool use_cout;
    std::ofstream output_stream;
    bool show_params;
    bool show_address;
    bool should_flush;

    bool show_type;
    int indent_size;
    int name_size;
    int type_size;
    bool use_spaces;
    bool show_shader;

    static const char *const SPACES;
    static const int MAX_SPACES = 72;
    static const char *const TABS;
    static const int MAX_TABS = 18;
};

const char *const ApiDumpSettings::SPACES =
    "                                                                        ";
const char *const ApiDumpSettings::TABS =
    "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

class ApiDumpInstance {
  public:
    inline ApiDumpInstance()
        : dump_settings(NULL), frame_count(0), thread_count(0) {
        loader_platform_thread_create_mutex(&output_mutex);
        loader_platform_thread_create_mutex(&frame_mutex);
        loader_platform_thread_create_mutex(&thread_mutex);
    }

    inline ~ApiDumpInstance() {
        if (dump_settings != NULL)
            delete dump_settings;
        
        loader_platform_thread_delete_mutex(&thread_mutex);
        loader_platform_thread_delete_mutex(&frame_mutex);
        loader_platform_thread_delete_mutex(&output_mutex);
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
        loader_platform_thread_unlock_mutex(&frame_mutex);
    }

    inline loader_platform_thread_mutex *outputMutex() { return &output_mutex; }

    inline const ApiDumpSettings &settings() {
        if (dump_settings == NULL)
            dump_settings = new ApiDumpSettings();

        return *dump_settings;
    }

    uint32_t threadID() {
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

    static inline ApiDumpInstance &current() { return current_instance; }

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
};

ApiDumpInstance ApiDumpInstance::current_instance;

// Text dump helpers

template <typename T>
inline void
dump_text_array(const T *array, size_t len, const ApiDumpSettings &settings,
                const char *type_string, const char *child_type,
                const char *name, int indents,
                std::ostream &(*dump)(const T, const ApiDumpSettings &, int)) {
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
        dump_text_value(array[i], settings, child_type, indexName.c_str(),
                        indents + 1, dump);
    }
}

template <typename T>
inline void dump_text_array(
    const T *array, size_t len, const ApiDumpSettings &settings,
    const char *type_string, const char *child_type, const char *name,
    int indents,
    std::ostream &(*dump)(const T &, const ApiDumpSettings &, int)) {
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
        dump_text_value(array[i], settings, child_type, indexName.c_str(),
                        indents + 1, dump);
    }
}

template <typename T>
inline void dump_text_pointer(
    const T *pointer, const ApiDumpSettings &settings, const char *type_string,
    const char *name, int indents,
    std::ostream &(*dump)(const T, const ApiDumpSettings &, int)) {
    if (pointer == NULL) {
        settings.formatNameType(settings.stream(), indents, name, type_string);
        settings.stream() << "NULL\n";
    } else {
        dump_text_value(*pointer, settings, type_string, name, indents, dump);
    }
}

template <typename T>
inline void dump_text_pointer(
    const T *pointer, const ApiDumpSettings &settings, const char *type_string,
    const char *name, int indents,
    std::ostream &(*dump)(const T &, const ApiDumpSettings &, int)) {
    if (pointer == NULL) {
        settings.formatNameType(settings.stream(), indents, name, type_string);
        settings.stream() << "NULL\n";
    } else {
        dump_text_value(*pointer, settings, type_string, name, indents, dump);
    }
}

template <typename T>
inline void
dump_text_value(const T object, const ApiDumpSettings &settings,
                const char *type_string, const char *name, int indents,
                std::ostream &(*dump)(const T, const ApiDumpSettings &, int)) {
    settings.formatNameType(settings.stream(), indents, name, type_string);
    dump(object, settings, indents) << "\n";
}

template <typename T>
inline void dump_text_value(
    const T &object, const ApiDumpSettings &settings, const char *type_string,
    const char *name, int indents,
    std::ostream &(*dump)(const T &, const ApiDumpSettings &, int)) {
    settings.formatNameType(settings.stream(), indents, name, type_string);
    dump(object, settings, indents);
}

inline void dump_text_special(
    const char *text, const ApiDumpSettings &settings, const char *type_string,
    const char *name, int indents) {
    settings.formatNameType(settings.stream(), indents, name, type_string);
    settings.stream() << text << "\n";
}

inline bool dump_text_bitmaskOption(const std::string &option,
                                    std::ostream &stream, bool isFirst) {
    if (isFirst)
        stream << " (";
    else
        stream << " | ";
    stream << option;
    return false;
}

inline std::ostream &dump_text_cstring(const char *object,
                                       const ApiDumpSettings &settings,
                                       int indents) {
    if (object == NULL)
        return settings.stream() << "NULL";
    else
        return settings.stream() << "\"" << object << "\"";
}

inline std::ostream &dump_text_void(const void *object,
                                    const ApiDumpSettings &settings,
                                    int indents) {
    if (object == NULL)
        return settings.stream() << "NULL";
    else if (settings.showAddress())
        return settings.stream() << object;
    else
        return settings.stream() << "address";
}
