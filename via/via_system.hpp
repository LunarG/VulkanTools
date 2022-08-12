/*
 * Copyright (c) 2016-2021 Valve Corporation
 * Copyright (c) 2016-2021 LunarG, Inc.
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
 * Author: Mark Young <marky@lunarg.com>
 */

#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <json/json.h>
#include <vulkan/vulkan.h>

#if (defined(_MSC_VER) && _MSC_VER < 1900 /*vs2015*/) || defined MINGW_HAS_SECURE_API
#include <basetsd.h>
#define snprintf sprintf_s
#endif

class ViaSystem {
   public:
    ViaSystem();
    virtual ~ViaSystem();

    bool Init(int argc, char** argv);
    bool GenerateInfo();

    // Result ids
    enum ViaResults {
        VIA_SUCCESSFUL = 0,

        VIA_UNKNOWN_ERROR = -1,
        VIA_SYSTEM_CALL_FAILURE = -2,

        VIA_MISSING_DRIVER_REGISTRY = -20,
        VIA_MISSING_DRIVER_JSON = -21,
        VIA_DRIVER_JSON_PARSING_ERROR = -22,
        VIA_MISSING_DRIVER_LIB = -23,

        VIA_VULKAN_CANT_FIND_RUNTIME = -40,
        VIA_VULKAN_CANT_FIND_DRIVER = -41,
        VIA_VULKAN_CANT_FIND_EXTENSIONS = -42,
        VIA_VULKAN_FAILED_CREATE_INSTANCE = -43,
        VIA_VULKAN_FAILED_CREATE_DEVICE = -44,
        VIA_VULKAN_FAILED_OUT_OF_MEM = -45,

        VIA_TEST_FAILED = -60,
    };

   protected:
    struct OverrideExpiration {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
    };

    enum ViaElementAlign { VIA_ALIGN_LEFT = 0, VIA_ALIGN_CENTER, VIA_ALIGN_RIGHT };

    // Print methods
    void StartOutput(const std::string& title);
    void EndOutput();
    void BeginSection(const std::string& section_str);
    void EndSection();
    void PrintStandardText(const std::string& text_str);
    void PrintBeginTable(const std::string& table_name, uint32_t num_cols);
    void PrintBeginTableRow();
    void PrintTableElement(const std::string& element, ViaElementAlign align = VIA_ALIGN_LEFT);
    void PrintEndTableRow();
    void PrintEndTable();

    // HTML methods
    void StartOutputHTML(const std::string& title);
    void EndOutputHTML();
    void BeginSectionHTML(const std::string& section_str);
    void EndSectionHTML();
    void PrintStandardTextHTML(const std::string& text_str);
    void PrintBeginTableHTML(const std::string& table_name, uint32_t num_cols);
    void PrintBeginTableRowHTML();
    void PrintTableElementHTML(const std::string& element, ViaElementAlign align = VIA_ALIGN_LEFT);
    void PrintEndTableRowHTML();
    void PrintEndTableHTML();

    // VkConfig output methods
    void StartOutputVkConfig(const std::string& title);
    void EndOutputVkConfig();
    void BeginSectionVkConfig(const std::string& section_str);
    void EndSectionVkConfig();
    void PrintStandardTextVkConfig(const std::string& text_str);
    void PrintBeginTableVkConfig(const std::string& table_name);
    void PrintBeginTableRowVkConfig();
    void PrintTableElementVkConfig(const std::string& element);
    void PrintEndTableRowVkConfig();
    void PrintEndTableVkConfig();

    // Logging methods
    void LogError(const std::string& error);
    void LogWarning(const std::string& warning);
    void LogInfo(const std::string& info);

    // Utility methods
    std::string TrimWhitespace(const std::string& str, const std::string& whitespace = " \t\n\r");
    std::string ConvertPathFormat(std::string str);
    virtual std::string GetEnvironmentalVariableValue(const std::string& env_var) = 0;
    virtual bool ExpandPathWithEnvVar(std::string& path) = 0;

    // Json Methods
    bool DetermineJsonLibraryPath(const std::string& json_location, const std::string& json_library_info,
                                  std::string& library_location);

    // Methods children need to override
    virtual bool IsAbsolutePath(const std::string& path);
    virtual ViaResults PrintSystemEnvironmentInfo() = 0;
    virtual ViaResults PrintSystemHardwareInfo() = 0;
    virtual ViaResults PrintSystemExecutableInfo() = 0;
    virtual ViaResults PrintSystemDriverInfo() = 0;
    virtual ViaResults PrintSystemLoaderInfo() = 0;
    virtual ViaResults PrintSystemSdkInfo() = 0;
    virtual ViaResults PrintSystemImplicitLayerInfo() = 0;
    virtual ViaResults PrintSystemExplicitLayerInfo() = 0;
    virtual ViaResults PrintSystemSettingsFileInfo() = 0;
    virtual int RunTestInDirectory(std::string path, std::string test, std::string cmd_line) = 0;
    virtual void PrintFileVersionInfo(const std::string& json_filename, const std::string& library) {}
    virtual bool CheckExpiration(OverrideExpiration expiration) = 0;

    // Non-overrideable capture functions
    ViaResults GenerateSystemInfo();
    ViaResults GenerateVulkanInfo();
    ViaResults GenerateTestInfo();
    void GenerateSettingsFileJsonInfo(const std::string& settings_file);
    void GenerateExplicitLayerJsonInfo(const char* layer_json_filename, Json::Value root);
    void GenerateImplicitLayerJsonInfo(const char* layer_json_filename, Json::Value root, std::vector<std::string>& override_paths);
    ViaResults GenerateInstanceInfo(void);
    ViaResults GeneratePhysDevInfo(void);
    ViaResults GenerateLogicalDeviceInfo();
    void GenerateCleanupInfo(void);

    struct VulkanApiVersion {
        uint16_t major;
        uint16_t minor;
        uint16_t patch;
    };

    struct VulkanPhysicalDeviceInfo {
        VkPhysicalDevice vk_phys_dev;
        VulkanApiVersion api_version;
        std::vector<VkQueueFamilyProperties> vk_queue_fam_props;
    };

    struct VulkanInstanceInfo {
        VkInstance vk_instance;
        VulkanApiVersion desired_api_version;
        VulkanApiVersion max_api_version;
        std::vector<VulkanPhysicalDeviceInfo> vk_physical_devices;
        std::vector<VkDevice> vk_logical_devices;
    };

    struct VulkanSettingPair {
        std::string name;
        std::string value;
    };

    // General file/html items
    bool _generate_unique_file;
    char _directory_symbol;
    std::string _os_name;
    std::string _home_path;
    std::string _app_version;
    std::string _exe_path;
    std::string _cur_path;
    std::string _out_file;
    std::string _full_out_file;
    std::ofstream _out_ofstream;

    // Command Line Argument items
    bool _run_cube_tests;

    enum ViaFileFormat { VIA_HTML_FORMAT = 0, VIA_VKCONFIG_FORMAT };
    ViaFileFormat _out_file_format;

    // SDK items
    bool _found_sdk;
    std::string _sdk_path;
    bool _is_system_installed_sdk;
    bool _ran_tests;

    // Table-specific items
    bool _outputting_to_odd_row;
    uint32_t _row_count;
    uint32_t _col_count;
    uint32_t _table_count;
    uint32_t _standard_text_count;

    VulkanInstanceInfo _vulkan_1_0_info;
    VulkanInstanceInfo _vulkan_max_info;
    std::vector<std::string> _layer_override_search_path;
};
