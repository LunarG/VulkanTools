/*
 * Copyright (c) 2015-2021 The Khronos Group Inc.
 * Copyright (c) 2015-2021 Valve Corporation
 * Copyright (c) 2015-2021 LunarG, Inc.
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
 * Authors:
 * - Christophe Riccio <christophe@lunarg.com>
 * - Mark Lobodzinski <mark@lunarg.com>
 * - Jon Ashburn
 * - Courtney Goeltzenleuchter
 * - Tobin Ehlis
 */

#include "vk_layer_config.h"

#include <cassert>
#include <cstring>
#include <cctype>
#include <fstream>
#include <array>
#include <map>
#include <sstream>
#include <regex>
#include <sys/stat.h>

#include <vulkan/vk_layer.h>
// sdk_platform header redefines NOMINMAX
#undef NOMINMAX
#include <vulkan/vk_sdk_platform.h>

#if defined(_WIN32)
#include <windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

namespace vku {

typedef enum {
    kVkConfig,
    kEnvVar,
    kLocal,
} SettingsFileSource;

typedef struct SettingsFileInfo {
    bool file_found = false;
    std::string location{};
    SettingsFileSource source = kLocal;
} SettingsFileInfo;

class ConfigFile {
   public:
    ConfigFile();
    ~ConfigFile(){};

    const char *GetOption(const std::string &option);
    void SetOption(const std::string &option, const std::string &value);
    std::string vk_layer_disables_env_var;
    SettingsFileInfo settings_info{};

   private:
    bool file_is_parsed_;
    std::map<std::string, std::string> value_map_;

    std::string FindSettings();
    void ParseFile(const char *filename);
};

static ConfigFile layer_config;

std::string GetEnvironment(const char *variable) {
#if !defined(__ANDROID__) && !defined(_WIN32)
    const char *output = getenv(variable);
    return output == NULL ? "" : output;
#elif defined(_WIN32)
    int size = GetEnvironmentVariable(variable, NULL, 0);
    if (size == 0) {
        return "";
    }
    char *buffer = new char[size];
    GetEnvironmentVariable(variable, buffer, size);
    std::string output = buffer;
    delete[] buffer;
    return output;
#elif defined(__ANDROID__)
    string command = "getprop " + string(variable);
    FILE *pPipe = popen(command.c_str(), "r");
    if (pPipe != nullptr) {
        char value[256];
        fgets(value, 256, pPipe);
        pclose(pPipe);

        // Make sure its not an empty line
        if (strcspn(value, "\r\n") == 0) {
            return "";
        } else {
            return string(value);
        }
    } else {
        return "";
    }
#else
    return "";
#endif
}

static std::string string_tolower(const std::string &s) {
    std::string result = s;
    for (auto &c : result) {
        c = std::tolower(c);
    }
    return result;
}

static std::string string_upper(const std::string &s) {
    std::string result = s;
    for (auto &c : result) {
        c = std::toupper(c);
    }
    return result;
}

static std::string GetSettingKey(const char *setting_namespace, const char *setting_key) {
    std::stringstream result;
    result << setting_namespace << "." << setting_key;
    return result.str();
}

static inline std::string TrimVendor(const std::string &s) {
    static const char *separator = "_";

    const auto trimmed_beg = s.find_first_of(separator);
    if (trimmed_beg == std::string::npos) return s;

    const auto trimmed_end = s.find_last_not_of(separator);
    assert(trimmed_end != std::string::npos && trimmed_beg <= trimmed_end);

    return s.substr(trimmed_beg + 1, s.size());
}

static std::string GetEnvVarKey(const char *setting_namespace, const char *setting_key, bool trim_vendor) {
    std::stringstream result;

#if defined(__ANDROID__)
    if (trim_vendor) {
        result << "debug.vulkan." << GetSettingKey(TrimVendor(setting_namespace), setting_key);
    } else {
        result << "debug.vulkan." << GetSettingKey(setting_namespace, setting_key);
    }
#else
    if (trim_vendor) {
        result << "VK_" << string_upper(TrimVendor(setting_namespace)) << "_" << string_upper(setting_key);
    } else {
        result << "VK_" << string_upper(setting_namespace) << "_" << string_upper(setting_key);
    }
#endif
    return result.str();
}

static std::string GetLayerSettingData(const char *setting_namespace, const char *key) {
    std::string setting = GetLayerEnvVar(GetEnvVarKey(setting_namespace, key, false).c_str());
    if (setting.empty()) {
        setting = GetLayerEnvVar(GetEnvVarKey(setting_namespace, key, true).c_str());
    }
    if (setting.empty()) {
        setting = GetLayerSetting(GetSettingKey(setting_namespace, key).c_str());
    }
    return setting;
}

VK_LAYER_EXPORT bool GetLayerSettingBool(const char *setting_namespace, const char *key) {
    bool result = false;

    std::string setting = GetLayerSettingData(setting_namespace, key);
    if (!setting.empty()) {
        setting = string_tolower(setting);
        if (setting == "true") {
            result = true;
        } else {
            result = std::atoi(setting.c_str()) != 0;
        }
    }
    return result;
}

VK_LAYER_EXPORT int GetLayerSettingInt(const char *setting_namespace, const char *key) {
    int result = 0;

    std::string setting = GetLayerSettingData(setting_namespace, key);
    if (!setting.empty()) {
        result = std::atoi(setting.c_str());
    }
    return result;
}

VK_LAYER_EXPORT double GetLayerSettingFloat(const char *setting_namespace, const char *key) {
    double result = 0.0;

    std::string setting = GetLayerSettingData(setting_namespace, key);
    if (!setting.empty()) {
        result = std::atof(setting.c_str());
    }
    return result;
}

VK_LAYER_EXPORT std::string GetLayerSettingString(const char *setting_namespace, const char *key) {
    return GetLayerSettingData(setting_namespace, key);
}

static inline std::vector<std::string> Split(const std::string &value, const std::string &delimiter) {
    std::vector<std::string> result;

    std::string parse = value;

    std::size_t start = 0;
    std::size_t end = parse.find(delimiter);
    while (end != std::string::npos) {
        result.push_back(parse.substr(start, end - start));
        start = end + delimiter.length();
        end = parse.find(delimiter, start);
    }

    const std::string last = parse.substr(start, end);
    if (!last.empty()) {
        result.push_back(last);
    }

    return result;
}

VK_LAYER_EXPORT std::vector<std::string> GetLayerSettingStrings(const char *setting_namespace, const char *key) {
    std::string setting = GetLayerSettingData(setting_namespace, key);
    if (setting.find_first_of(",") != std::string::npos) {
        return Split(setting, ",");
    } else {
#ifdef _WIN32
        const char *delimiter = ";";
#else
        const char *delimiter = ":";
#endif
        return Split(setting, delimiter);
    }
}

static bool IsInteger(const std::string &text) {
    static const std::regex FRAME_REGEX("^-?([0-9]*|0x[0-9|a-z|A-Z]*)$");

    return std::regex_search(text, FRAME_REGEX);
}

VK_LAYER_EXPORT std::vector<std::pair<std::string, int>> GetLayerSettingList(const char *setting_namespace, const char *key) {
    std::vector<std::string> inputs = GetLayerSettingStrings(setting_namespace, key);

    std::vector<std::pair<std::string, int>> result;
    for (std::size_t i = 0, n = inputs.size(); i < n; ++i) {
        std::pair<std::string, int> value;
        if (IsInteger(inputs[i])) {
            value.second = atoi(inputs[i].c_str());
        } else {
            value.first = inputs[i];
        }
        result.push_back(value);
    }
    return result;
}

VK_LAYER_EXPORT const char *GetLayerSetting(const char *option) { return layer_config.GetOption(option); }

VK_LAYER_EXPORT const char *GetLayerEnvVar(const char *option) {
    layer_config.vk_layer_disables_env_var = GetEnvironment(option);
    return layer_config.vk_layer_disables_env_var.c_str();
}

VK_LAYER_EXPORT void SetLayerSetting(const char *option, const char *value) { layer_config.SetOption(option, value); }

// Constructor for ConfigFile. Initialize layers to log error messages to stdout by default. If a vk_layer_settings file is present,
// its settings will override the defaults.
ConfigFile::ConfigFile() : file_is_parsed_(false) {}

const char *ConfigFile::GetOption(const std::string &option) {
    std::map<std::string, std::string>::const_iterator it;
    if (!file_is_parsed_) {
        std::string settings_file = FindSettings();
        ParseFile(settings_file.c_str());
    }

    if ((it = value_map_.find(option)) == value_map_.end()) {
        return "";
    } else {
        return it->second.c_str();
    }
}

void ConfigFile::SetOption(const std::string &option, const std::string &val) {
    if (!file_is_parsed_) {
        std::string settings_file = FindSettings();
        ParseFile(settings_file.c_str());
    }

    value_map_[option] = val;
}

#if defined(WIN32)
// Check for admin rights
static inline bool IsHighIntegrity() {
    HANDLE process_token;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_QUERY_SOURCE, &process_token)) {
        // Maximum possible size of SID_AND_ATTRIBUTES is maximum size of a SID + size of attributes DWORD.
        uint8_t mandatory_label_buffer[SECURITY_MAX_SID_SIZE + sizeof(DWORD)];
        DWORD buffer_size;
        if (GetTokenInformation(process_token, TokenIntegrityLevel, mandatory_label_buffer, sizeof(mandatory_label_buffer),
                                &buffer_size) != 0) {
            const TOKEN_MANDATORY_LABEL *mandatory_label = (const TOKEN_MANDATORY_LABEL *)mandatory_label_buffer;
            const DWORD sub_authority_count = *GetSidSubAuthorityCount(mandatory_label->Label.Sid);
            const DWORD integrity_level = *GetSidSubAuthority(mandatory_label->Label.Sid, sub_authority_count - 1);

            CloseHandle(process_token);
            return integrity_level > SECURITY_MANDATORY_MEDIUM_RID;
        }

        CloseHandle(process_token);
    }

    return false;
}
#endif

std::string ConfigFile::FindSettings() {
    struct stat info;

#if defined(WIN32)
    // Look for VkConfig-specific settings location specified in the windows registry
    HKEY key;

    const std::array<HKEY, 2> hives = {HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER};
    const size_t hives_to_check_count = IsHighIntegrity() ? 1 : hives.size();  // Admin checks only the default hive

    for (size_t hive_index = 0; hive_index < hives_to_check_count; ++hive_index) {
        LSTATUS err = RegOpenKeyEx(hives[hive_index], "Software\\Khronos\\Vulkan\\Settings", 0, KEY_READ, &key);
        if (err == ERROR_SUCCESS) {
            char name[2048];
            DWORD i = 0, name_size, type, value, value_size;
            while (ERROR_SUCCESS == RegEnumValue(key, i++, name, &(name_size = sizeof(name)), nullptr, &type,
                                                 reinterpret_cast<LPBYTE>(&value), &(value_size = sizeof(value)))) {
                // Check if the registry entry is a dword with a value of zero
                if (type != REG_DWORD || value != 0) {
                    continue;
                }

                // Check if this actually points to a file
                if ((stat(name, &info) != 0) || !(info.st_mode & S_IFREG)) {
                    continue;
                }

                // Use this file
                RegCloseKey(key);
                settings_info.source = kVkConfig;
                settings_info.location = name;
                return name;
            }

            RegCloseKey(key);
        }
    }

#else
    // Look for VkConfig-specific settings location specified in a specific spot in the linux settings store
    std::string search_path = GetEnvironment("XDG_DATA_HOME");
    if (search_path == "") {
        search_path = GetEnvironment("HOME");
        if (search_path != "") {
            search_path += "/.local/share";
        }
    }
    // Use the vk_layer_settings.txt file from here, if it is present
    if (search_path != "") {
        std::string home_file = search_path + "/vulkan/settings.d/vk_layer_settings.txt";
        if (stat(home_file.c_str(), &info) == 0) {
            if (info.st_mode & S_IFREG) {
                settings_info.source = kVkConfig;
                settings_info.location = home_file;
                return home_file;
            }
        }
    }

#endif
    // Look for an enviornment variable override for the settings file location
    std::string env_path = GetEnvironment("VK_LAYER_SETTINGS_PATH");

    // If the path exists use it, else use vk_layer_settings
    if (stat(env_path.c_str(), &info) == 0) {
        // If this is a directory, append settings file name
        if (info.st_mode & S_IFDIR) {
            env_path.append("/vk_layer_settings.txt");
        }
        settings_info.source = kEnvVar;
        settings_info.location = env_path;
        return env_path;
    }

    // Default -- use the current working directory for the settings file location
    settings_info.source = kLocal;
    char buff[512];
    auto buf_ptr = GetCurrentDir(buff, 512);
    if (buf_ptr) {
        settings_info.location = buf_ptr;
        settings_info.location.append("/vk_layer_settings.txt");
    }
    return "vk_layer_settings.txt";
}

static inline std::string TrimWhitespace(const std::string &s) {
    const char *whitespace = " \t\f\v\n\r";

    const auto trimmed_beg = s.find_first_not_of(whitespace);
    if (trimmed_beg == std::string::npos) return "";

    const auto trimmed_end = s.find_last_not_of(whitespace);
    assert(trimmed_end != std::string::npos && trimmed_beg <= trimmed_end);

    return s.substr(trimmed_beg, trimmed_end - trimmed_beg + 1);
}

void ConfigFile::ParseFile(const char *filename) {
    file_is_parsed_ = true;

    // Extract option = value pairs from a file
    std::ifstream file(filename);
    if (file.good()) {
        settings_info.file_found = true;
        for (std::string line; std::getline(file, line);) {
            // discard comments, which start with '#'
            const auto comments_pos = line.find_first_of('#');
            if (comments_pos != std::string::npos) line.erase(comments_pos);

            const auto value_pos = line.find_first_of('=');
            if (value_pos != std::string::npos) {
                const std::string option = TrimWhitespace(line.substr(0, value_pos));
                const std::string value = TrimWhitespace(line.substr(value_pos + 1));
                value_map_[option] = value;
            }
        }
    }
}

}  // namespace vku
