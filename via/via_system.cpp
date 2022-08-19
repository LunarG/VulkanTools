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

#include <iostream>
#include <sstream>
#include <cstring>
#include <map>

#include <time.h>
#include <vulkan/vulkan.h>

#include "via_system.hpp"

#ifdef VIA_WINDOWS_TARGET
#include <windows.h>
#endif

ViaSystem::ViaSystem() {
    _generate_unique_file = false;
    _out_file = "";
    _directory_symbol = '/';
    _home_path = "~/";
    _app_version = "Version 1.3";
}

bool ViaSystem::Init(int argc, char** argv) {
    char* output_path = nullptr;
    // Check and handle command-line arguments
    _run_cube_tests = true;
    _out_file_format = VIA_HTML_FORMAT;
    if (argc > 1) {
        for (int iii = 1; iii < argc; iii++) {
            if (0 == strcmp("--unique_output", argv[iii])) {
                _generate_unique_file = true;
            } else if (0 == strcmp("--output_path", argv[iii]) && argc > (iii + 1)) {
                output_path = argv[iii + 1];
                ++iii;
            } else if (0 == strcmp("--disable_cube_tests", argv[iii])) {
                _run_cube_tests = false;
            } else if (0 == strcmp("--vkconfig_output", argv[iii])) {
                _out_file_format = VIA_VKCONFIG_FORMAT;
            } else {
                std::cout << "Usage of " << argv[0] << ":" << std::endl
                          << "    " << argv[0]
                          << " [--unique_output] "
                             "[--output_path <path>]"
                             " [--disable_cube_tests]"
                          << std::endl
                          << "          [--unique_output] Optional "
                             "parameter to generate a unique html"
                          << std::endl
                          << "                            "
                             "output file in the form "
                             "\'via_YYYY_MM_DD_HH_MM.html\'"
                          << std::endl
                          << "          [--output_path <path>"
                             "] Optional parameter to generate the output at"
                          << std::endl
                          << "                               "
                             "  a given path"
                          << std::endl
                          << "          [--disable_cube_tests] Optional parameter to disable running cube to test the Vulkan SDK "
                             "installation."
                          << std::endl;
                return false;
            }
        }
    }

    // If the user wants a specific output path, write it to the buffer
    // and then continue writing the rest of the name below
    std::string file_path = "";
    if (output_path != NULL) {
        file_path = output_path;
        file_path += _directory_symbol;

        // Expand any environment variables in the path
        ExpandPathWithEnvVar(file_path);
    }

    // If the user wants a unique file, generate a file with the current
    // time and date incorporated into it.
    _out_file = argv[0];
    if (output_path != NULL) {
        auto dir_pos = _out_file.rfind(_directory_symbol);
        if (dir_pos != std::string::npos) {
            // Erase everything before the last directory symbol
            _out_file.erase(0, dir_pos + 1);
        }
    }
#ifdef VIA_WINDOWS_TARGET
    // If it has an ending with a period, remove it.
    auto period_pos = _out_file.rfind('.');
    if (period_pos != std::string::npos) {
        // Erase everything after the last directory symbol
        _out_file.erase(period_pos);
    }
#endif
    if (_generate_unique_file) {
        time_t time_raw_format;
        time(&time_raw_format);
        tm* ptr_time = localtime(&time_raw_format);
        char time_date_filename[512];
        if (strftime(time_date_filename, 511, "_%Y_%m_%d_%H_%M.html", ptr_time) == 0) {
            LogError("Couldn't generate unique HTML file name for output");
            return false;
        }
        _out_file += time_date_filename;
    } else {
        if (_out_file_format == VIA_HTML_FORMAT) {
            _out_file += ".html";
        } else if (_out_file_format == VIA_VKCONFIG_FORMAT) {
            _out_file += ".json";
        }
    }

    // Write the output file to the current executing directory, or, if
    // that fails, write it out to the user's home folder.

    _full_out_file = file_path + _out_file;
    _out_ofstream.open(_full_out_file);
    if (_out_ofstream.fail()) {
        _full_out_file = _home_path + _out_file;
        _out_ofstream.open(_full_out_file);
        if (_out_ofstream.fail()) {
            LogError("Failed creating output file!");
            return false;
        }
    }

    // Initialize various variables
    _found_sdk = false;
    _ran_tests = false;
    _is_system_installed_sdk = false;

    // Initialize the detected versions to 1.0.0
    _vulkan_1_0_info = {};
    _vulkan_1_0_info.desired_api_version.major = 1;
    _vulkan_1_0_info.max_api_version.major = 1;
    _vulkan_max_info = {};
    _vulkan_max_info.desired_api_version.major = 1;
    _vulkan_max_info.max_api_version.major = 1;
    return true;
}

bool ViaSystem::GenerateInfo() {
    StartOutput("LunarG VIA");
    ViaResults results = GenerateSystemInfo();
    if (results != VIA_SUCCESSFUL) {
        goto print_results;
    }
    results = GenerateVulkanInfo();
    if (results != VIA_SUCCESSFUL) {
        goto print_results;
    }

    if (_run_cube_tests) {
        results = GenerateTestInfo();
        if (results != VIA_SUCCESSFUL) {
            goto print_results;
        }
    }

print_results:
    EndOutput();

    // Print out a useful message for any common errors.
    switch (results) {
        case VIA_SUCCESSFUL: {
            std::string vulkan_version_string = "Vulkan ";
            vulkan_version_string += std::to_string(_vulkan_max_info.desired_api_version.major);
            vulkan_version_string += ".";
            vulkan_version_string += std::to_string(_vulkan_max_info.desired_api_version.minor);
            if (!_found_sdk) {
                std::cerr << "SUCCESS: Vulkan analysis able to create " << vulkan_version_string
                          << " instance/devices - However, No SDK Detected" << std::endl;
            } else if (!_ran_tests) {
                if (_run_cube_tests) {
                    std::cerr << "SUCCESS: Vulkan analysis able to create " << vulkan_version_string
                              << " instance/devices, SDK was found, but failed to run external tests" << std::endl;
                } else {
                    std::cerr << "SUCCESS: Vulkan analysis able to create " << vulkan_version_string
                              << " instance/devices, SDK was found, but external tests were disabled and not run" << std::endl;
                }
            } else {
                std::cerr << "SUCCESS: Vulkan analysis completed properly using " << vulkan_version_string << std::endl;
            }
            break;
        }
        case VIA_SYSTEM_CALL_FAILURE:
            LogError("Failure occurred during system call.");
            break;
        case VIA_MISSING_DRIVER_REGISTRY:
            LogError("Failed to find Vulkan Driver JSON in registry.");
            break;
        case VIA_MISSING_DRIVER_JSON:
            LogError("Failed to find Vulkan Driver JSON.");
            break;
        case VIA_DRIVER_JSON_PARSING_ERROR:
            LogError("Failed to properly parse Vulkan Driver JSON.");
            break;
        case VIA_MISSING_DRIVER_LIB:
            LogError("Failed to find Vulkan Driver Lib.");
            break;
        case VIA_VULKAN_CANT_FIND_RUNTIME:
            LogError("Vulkan failed to find a Vulkan Runtime to use.");
            break;
        case VIA_VULKAN_CANT_FIND_DRIVER:
            LogError("Vulkan failed to find a compatible driver.");
            break;
        case VIA_VULKAN_CANT_FIND_EXTENSIONS:
            LogError("Failed to find expected Vulkan Extensions.  This may indicate a bad driver install.");
            break;
        case VIA_VULKAN_FAILED_CREATE_INSTANCE:
            LogError("Unknown error while attempting to create Vulkan Instance.");
            break;
        case VIA_VULKAN_FAILED_CREATE_DEVICE:
            LogError("Unknown error while attempting to create Vulkan Device.");
            break;
        case VIA_VULKAN_FAILED_OUT_OF_MEM:
            LogError("Vulkan Loader, Layer, or Driver ran out of memory.");
            break;
        case VIA_TEST_FAILED:
            LogError("Unknown Test failure occurred.");
            break;
        case VIA_UNKNOWN_ERROR:
        default:
            LogError("Uknown failure occurred.  Refer to HTML for more info");
            break;
    }

    return (results == VIA_SUCCESSFUL);
}

ViaSystem::~ViaSystem() {
    _out_ofstream.close();
#ifdef VIA_WINDOWS_TARGET
    if (_out_file_format == VIA_HTML_FORMAT) {
        // Open the html file in a browser
        // Sleep 1 second before bringing up the browser so console window is visible for a little
        // while before browser comes up and covers it.
        Sleep(1000);
        ShellExecute(NULL, "open", _full_out_file.c_str(), NULL, NULL, SW_SHOWNORMAL);

        // If we are running in a console, wait for user to to press enter before
        // exitting. We do this so that the user can read the output.
        // Note that running vkvia from a Windows CMD prompt or from a Cygwin shell
        // is not considered running in a console, so we don't wait for user to press
        // enter in those cases.
        HWND consoleWnd = GetConsoleWindow();
        DWORD dwProcessId;
        GetWindowThreadProcessId(consoleWnd, &dwProcessId);
        if (GetCurrentProcessId()==dwProcessId)
        {
            std::cout <<std::endl << "Press enter to close this window" << std::endl;
            std::cin.get();
        }
    }
#endif
}

void ViaSystem::LogError(const std::string& error) { std::cerr << "VIA_ERROR:   " << error << std::endl; }

void ViaSystem::LogWarning(const std::string& warning) { std::cerr << "VIA_WARNING: " << warning << std::endl; }

void ViaSystem::LogInfo(const std::string& info) { std::cerr << "VIA_INFO:    " << info << std::endl; }

bool ViaSystem::IsAbsolutePath(const std::string& path) {
    if (path[0] == _directory_symbol) {
        return true;
    }
    return false;
}

bool ViaSystem::DetermineJsonLibraryPath(const std::string& json_location, const std::string& json_library_info,
                                         std::string& library_location) {
    bool success = false;

    // Determine if the library is relative or absolute.  If it's absolute,
    // then just use the path.
    if (IsAbsolutePath(json_library_info)) {
        library_location = json_library_info;
        success = true;
    } else {
        std::string final_path = json_location;

        // Remove json file from json path to get just the file base location
        auto dir_pos = final_path.rfind(_directory_symbol);
        if (dir_pos != std::string::npos) {
            // Erase everything after the last directory symbol
            final_path.erase(dir_pos);
        }

        uint32_t i = 0;
        // Relative path, so we need to use the JSON's location
        while (json_library_info[i] == '.' && json_library_info[i + 1] == '.' && (json_library_info[i + 2] == _directory_symbol)) {
            i += 3;
            // Go up a folder in the json path
            auto dir_pos = final_path.rfind(_directory_symbol);
            if (dir_pos != std::string::npos) {
                // Erase everything after the last directory symbol
                final_path.erase(dir_pos);
            }
        }
        while (json_library_info[i] == '.' && (json_library_info[i + 1] == _directory_symbol)) {
            i += 2;
        }
        library_location = final_path;
        library_location += _directory_symbol;
        library_location += &json_library_info[i];
        success = true;
    }

    return success;
}

ViaSystem::ViaResults ViaSystem::GenerateSystemInfo() {
    ViaResults overall_result = VIA_SUCCESSFUL;

    BeginSection("System Info");

    ViaResults result = PrintSystemEnvironmentInfo();
    if (VIA_SUCCESSFUL != result) {
        overall_result = result;
    }
    result = PrintSystemHardwareInfo();
    if (VIA_SUCCESSFUL != result) {
        overall_result = result;
    }
    result = PrintSystemExecutableInfo();
    if (VIA_SUCCESSFUL != result) {
        overall_result = result;
    }
    result = PrintSystemDriverInfo();
    if (VIA_SUCCESSFUL != result) {
        overall_result = result;
    }
    result = PrintSystemLoaderInfo();
    if (VIA_SUCCESSFUL != result) {
        overall_result = result;
    }
    result = PrintSystemSdkInfo();
    if (VIA_SUCCESSFUL != result) {
        overall_result = result;
    }
    result = PrintSystemImplicitLayerInfo();
    if (VIA_SUCCESSFUL != result) {
        overall_result = result;
    }
    result = PrintSystemExplicitLayerInfo();
    if (VIA_SUCCESSFUL != result) {
        overall_result = result;
    }
    result = PrintSystemSettingsFileInfo();
    if (VIA_SUCCESSFUL != result) {
        overall_result = result;
    }

    EndSection();
    return overall_result;
}

// Perform Vulkan commands to find out what extensions are available
// to a Vulkan Instance, and attempt to create one.
ViaSystem::ViaResults ViaSystem::GenerateInstanceInfo(void) {
    ViaResults res = VIA_SUCCESSFUL;
    VkApplicationInfo app_info{};
    VkInstanceCreateInfo inst_info{};
    uint32_t ext_count = 0;
    std::vector<VkExtensionProperties> ext_props;
    VkResult status = VK_SUCCESS;
    char generic_string[1024];
    VulkanApiVersion max_inst_api_version = {};
    max_inst_api_version.major = 1;

    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "via";
    app_info.applicationVersion = 1;
    app_info.pEngineName = "via";
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_API_VERSION_1_0;

    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pApplicationInfo = &app_info;

    PrintBeginTable("Instance", 3);

    PFN_vkVoidFunction pfn_inst_version = vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
    PrintBeginTableRow();
    PrintTableElement("vkEnumerateInstanceVersion");
    if (nullptr == pfn_inst_version) {
        PrintTableElement("Not exposed by loader");
        PrintTableElement("");
    } else {
        typedef VkResult(VKAPI_PTR * pfn_enum_inst_ver)(uint32_t * pApiVersion);
        pfn_enum_inst_ver enum_inst_version = reinterpret_cast<pfn_enum_inst_ver>(pfn_inst_version);
        uint32_t instance_version;
        enum_inst_version(&instance_version);
        PrintTableElement("Max Instance Version");
        max_inst_api_version.major = VK_VERSION_MAJOR(instance_version);
        max_inst_api_version.minor = VK_VERSION_MINOR(instance_version);
        max_inst_api_version.patch = VK_VERSION_PATCH(instance_version);
        PrintTableElement(std::to_string(max_inst_api_version.major) + "." + std::to_string(max_inst_api_version.minor) + "." +
                          std::to_string(max_inst_api_version.patch));
    }
    PrintEndTableRow();

    PrintBeginTableRow();
    PrintTableElement("vkEnumerateInstanceExtensionProperties");
    status = vkEnumerateInstanceExtensionProperties(NULL, &ext_count, NULL);
    if (status) {
        snprintf(generic_string, 1023, "ERROR: Failed to determine num inst extensions - %d", status);
        PrintTableElement(generic_string);
        PrintTableElement("");
        PrintEndTableRow();
        res = VIA_VULKAN_CANT_FIND_EXTENSIONS;
    } else {
        snprintf(generic_string, 1023, "%d extensions found", ext_count);
        PrintTableElement(generic_string);
        PrintTableElement("");
        PrintEndTableRow();

        ext_props.resize(ext_count);
        status = vkEnumerateInstanceExtensionProperties(NULL, &ext_count, ext_props.data());
        if (status) {
            PrintBeginTableRow();
            PrintTableElement("");
            snprintf(generic_string, 1023, "ERROR: Failed to enumerate inst extensions - %d", status);
            PrintTableElement(generic_string);
            PrintTableElement("");
            PrintEndTableRow();
            res = VIA_VULKAN_CANT_FIND_EXTENSIONS;
        } else {
            for (uint32_t iii = 0; iii < ext_count; iii++) {
                PrintBeginTableRow();
                snprintf(generic_string, 1023, "[%d]", iii);
                PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
                PrintTableElement(ext_props[iii].extensionName);
                snprintf(generic_string, 1023, "Spec Vers %d", ext_props[iii].specVersion);
                PrintTableElement(generic_string);
                PrintEndTableRow();
            }
        }
    }

    // Need to specify VK_KHR_portability_enumeration extension on Mac in vkCreateInstance. This
    // is because the Vulkan on Metal driver is not fully Vulkan conformant, and VK_KHR_portability
    // needs to be enabled in order for the Vulkan Loader to report the Vulkan device.

    // Also need to specify the VK_KHR_get_physical_device_properties2 extension because we will be
    // be using the VK_KHR_portability_subset extension in vkCreateDevice.

    std::vector<const char *> portability_instance_extension_list;
    inst_info.flags = 0;
    inst_info.enabledExtensionCount = 0;
    inst_info.ppEnabledExtensionNames = NULL;
    for (const auto& extension : ext_props) {
        if (strcmp(extension.extensionName, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) == 0) {
            portability_instance_extension_list.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            inst_info.ppEnabledExtensionNames = portability_instance_extension_list.data();
            inst_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            inst_info.enabledExtensionCount++;
        }
        if (strcmp(extension.extensionName, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == 0) {
            portability_instance_extension_list.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
            inst_info.ppEnabledExtensionNames = portability_instance_extension_list.data();
            inst_info.enabledExtensionCount++;
        }
    }

    // Create a 1.0 instance
    PrintBeginTableRow();
    PrintTableElement("vkCreateInstance [1.0]");
    status = vkCreateInstance(&inst_info, NULL, &_vulkan_1_0_info.vk_instance);
    _vulkan_1_0_info.desired_api_version.major = 1;
    _vulkan_1_0_info.desired_api_version.minor = 0;
    _vulkan_1_0_info.desired_api_version.patch = 0;
    if (status == VK_ERROR_INCOMPATIBLE_DRIVER) {
        PrintTableElement("ERROR: Incompatible Driver");
        res = VIA_VULKAN_CANT_FIND_DRIVER;
    } else if (status == VK_ERROR_OUT_OF_HOST_MEMORY) {
        PrintTableElement("ERROR: Out of memory");
        res = VIA_VULKAN_FAILED_OUT_OF_MEM;
    } else if (status) {
        snprintf(generic_string, 1023, "ERROR: Failed to create - %d", status);
        PrintTableElement(generic_string);
        res = VIA_VULKAN_FAILED_CREATE_INSTANCE;
    } else {
        PrintTableElement("VIA_SUCCESSFUL");
    }
    PrintTableElement("");
    PrintEndTableRow();

    // Create an instance up to the max version possible
    if (nullptr != pfn_inst_version) {
        app_info.apiVersion = VK_MAKE_VERSION(max_inst_api_version.major, max_inst_api_version.minor, 0);
        _vulkan_max_info.max_api_version = max_inst_api_version;
        PrintBeginTableRow();
        PrintTableElement("vkCreateInstance [" + std::to_string(max_inst_api_version.major) + "." +
                          std::to_string(max_inst_api_version.minor) + "]");
        status = vkCreateInstance(&inst_info, NULL, &_vulkan_max_info.vk_instance);
        if (status == VK_ERROR_INCOMPATIBLE_DRIVER) {
            PrintTableElement("ERROR: Incompatible Driver");
            res = VIA_VULKAN_CANT_FIND_DRIVER;
        } else if (status == VK_ERROR_OUT_OF_HOST_MEMORY) {
            PrintTableElement("ERROR: Out of memory");
            res = VIA_VULKAN_FAILED_OUT_OF_MEM;
        } else if (status) {
            snprintf(generic_string, 1023, "ERROR: Failed to create - %d", status);
            PrintTableElement(generic_string);
            res = VIA_VULKAN_FAILED_CREATE_INSTANCE;
        } else {
            PrintTableElement("VIA_SUCCESSFUL");
        }
        PrintTableElement("");
        PrintEndTableRow();
    } else {
        _vulkan_max_info.vk_instance = VK_NULL_HANDLE;
        _vulkan_max_info.desired_api_version.major = 1;
        _vulkan_max_info.desired_api_version.minor = 0;
        _vulkan_max_info.desired_api_version.patch = 0;
    }

    PrintEndTable();

    return res;
}

// Print out any information we can find out about physical devices using
// the Vulkan commands.  There should be one for each Vulkan capable device
// on the system.
ViaSystem::ViaResults ViaSystem::GeneratePhysDevInfo(void) {
    ViaResults res = VIA_SUCCESSFUL;
    VkPhysicalDeviceProperties props;
    std::vector<VkPhysicalDevice> min_phys_devices;
    std::vector<VkPhysicalDevice> max_phys_devices;
    VkResult status;
    char generic_string[1024];
    uint32_t gpu_count = 0;
    uint32_t max_api_gpu_count = 0;
    VulkanApiVersion max_overall_version = {};
    max_overall_version.major = 1;
    uint32_t iii;
    uint32_t jjj;

    PrintBeginTable("Physical Devices", 4);

    PrintBeginTableRow();
    PrintTableElement("vkEnumeratePhysicalDevices [1.0]");
    status = vkEnumeratePhysicalDevices(_vulkan_1_0_info.vk_instance, &gpu_count, NULL);
    if (status) {
        snprintf(generic_string, 1023, "ERROR: Failed to query - %d", status);
        PrintTableElement(generic_string);
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
        res = VIA_VULKAN_CANT_FIND_DRIVER;
        goto out;
    } else {
        snprintf(generic_string, 1023, "%d", gpu_count);
        PrintTableElement(generic_string);
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
    }

    min_phys_devices.resize(gpu_count);
    _vulkan_1_0_info.vk_physical_devices.resize(gpu_count);
    status = vkEnumeratePhysicalDevices(_vulkan_1_0_info.vk_instance, &gpu_count, min_phys_devices.data());
    if (VK_SUCCESS != status && VK_INCOMPLETE != status) {
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("Failed to enumerate physical devices!");
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
        res = VIA_VULKAN_CANT_FIND_DRIVER;
        goto out;
    }

    for (iii = 0; iii < gpu_count; iii++) {
        _vulkan_1_0_info.vk_physical_devices[iii].vk_phys_dev = min_phys_devices[iii];

        PrintBeginTableRow();
        snprintf(generic_string, 1023, "[%d]", iii);
        PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
        if (status) {
            snprintf(generic_string, 1023, "ERROR: Failed to query - %d", status);
            PrintTableElement(generic_string);
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();
        } else {
            snprintf(generic_string, 1023, "0x%p", min_phys_devices[iii]);
            PrintTableElement(generic_string);
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();

            vkGetPhysicalDeviceProperties(min_phys_devices[iii], &props);

            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Vendor");
            switch (props.vendorID) {
                case 0x8086:
                case 0x8087:
                    snprintf(generic_string, 1023, "Intel [0x%04x]", props.vendorID);
                    break;
                case 0x1002:
                case 0x1022:
                    snprintf(generic_string, 1023, "AMD [0x%04x]", props.vendorID);
                    break;
                case 0x10DE:
                    snprintf(generic_string, 1023, "Nvidia [0x%04x]", props.vendorID);
                    break;
                case 0x1EB5:
                    snprintf(generic_string, 1023, "ARM [0x%04x]", props.vendorID);
                    break;
                case 0x5143:
                    snprintf(generic_string, 1023, "Qualcomm [0x%04x]", props.vendorID);
                    break;
                case 0x1099:
                case 0x10C3:
                case 0x1249:
                case 0x4E8:
                    snprintf(generic_string, 1023, "Samsung [0x%04x]", props.vendorID);
                    break;
                default:
                    snprintf(generic_string, 1023, "0x%04x", props.vendorID);
                    break;
            }
            PrintTableElement(generic_string);
            PrintTableElement("");
            PrintEndTableRow();

            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Device Name");
            PrintTableElement(props.deviceName);
            PrintTableElement("");
            PrintEndTableRow();

            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Device ID");
            snprintf(generic_string, 1023, "0x%x", props.deviceID);
            PrintTableElement(generic_string);
            PrintTableElement("");
            PrintEndTableRow();

            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Device Type");
            switch (props.deviceType) {
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    PrintTableElement("Integrated GPU");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    PrintTableElement("Discrete GPU");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    PrintTableElement("Virtual GPU");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    PrintTableElement("CPU");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                    PrintTableElement("Other");
                    break;
                default:
                    PrintTableElement("INVALID!");
                    break;
            }
            PrintTableElement("");
            PrintEndTableRow();

            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Driver Version");
            snprintf(generic_string, 1023, "%d.%d.%d", VK_VERSION_MAJOR(props.driverVersion), VK_VERSION_MINOR(props.driverVersion),
                     VK_VERSION_PATCH(props.driverVersion));
            PrintTableElement(generic_string);
            PrintTableElement("");
            PrintEndTableRow();

            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("API Version");
            snprintf(generic_string, 1023, "%d.%d.%d", VK_VERSION_MAJOR(props.apiVersion), VK_VERSION_MINOR(props.apiVersion),
                     VK_VERSION_PATCH(props.apiVersion));
            PrintTableElement(generic_string);
            PrintTableElement("");
            PrintEndTableRow();

            uint32_t queue_fam_count;
            vkGetPhysicalDeviceQueueFamilyProperties(min_phys_devices[iii], &queue_fam_count, NULL);
            if (queue_fam_count > 0) {
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("Queue Families");
                snprintf(generic_string, 1023, "%d", queue_fam_count);
                PrintTableElement(generic_string);
                PrintTableElement("");
                PrintEndTableRow();

                _vulkan_1_0_info.vk_physical_devices[iii].vk_queue_fam_props.resize(queue_fam_count);
                vkGetPhysicalDeviceQueueFamilyProperties(min_phys_devices[iii], &queue_fam_count,
                                                         _vulkan_1_0_info.vk_physical_devices[iii].vk_queue_fam_props.data());
                for (jjj = 0; jjj < queue_fam_count; jjj++) {
                    PrintBeginTableRow();
                    PrintTableElement("");
                    snprintf(generic_string, 1023, "[%d]", jjj);
                    PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
                    PrintTableElement("Queue Count");
                    snprintf(generic_string, 1023, "%d",
                             _vulkan_1_0_info.vk_physical_devices[iii].vk_queue_fam_props[jjj].queueCount);
                    PrintTableElement(generic_string);
                    PrintEndTableRow();

                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement("Queue Flags");
                    generic_string[0] = '\0';
                    bool prev_set = false;
                    if (_vulkan_1_0_info.vk_physical_devices[iii].vk_queue_fam_props[jjj].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                        strncat(generic_string, "GRAPHICS", 1023);
                        prev_set = true;
                    }
                    if (_vulkan_1_0_info.vk_physical_devices[iii].vk_queue_fam_props[jjj].queueFlags & VK_QUEUE_COMPUTE_BIT) {
                        if (prev_set) {
                            strncat(generic_string, " | ", 1023);
                        }
                        strncat(generic_string, "COMPUTE", 1023);
                        prev_set = true;
                    }
                    if (_vulkan_1_0_info.vk_physical_devices[iii].vk_queue_fam_props[jjj].queueFlags & VK_QUEUE_TRANSFER_BIT) {
                        if (prev_set) {
                            strncat(generic_string, " | ", 1023);
                        }
                        strncat(generic_string, "TRANSFER", 1023);
                        prev_set = true;
                    }
                    if (_vulkan_1_0_info.vk_physical_devices[iii].vk_queue_fam_props[jjj].queueFlags &
                        VK_QUEUE_SPARSE_BINDING_BIT) {
                        if (prev_set) {
                            strncat(generic_string, " | ", 1023);
                        }
                        strncat(generic_string, "SPARSE_BINDING", 1023);
                        prev_set = true;
                    }
                    if (!prev_set) {
                        strncat(generic_string, "--NONE--", 1023);
                    }
                    PrintTableElement(generic_string);
                    PrintEndTableRow();

                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement("Timestamp Valid Bits");
                    snprintf(generic_string, 1023, "0x%x",
                             _vulkan_1_0_info.vk_physical_devices[iii].vk_queue_fam_props[jjj].timestampValidBits);
                    PrintTableElement(generic_string);
                    PrintEndTableRow();

                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement("Image Granularity");
                    PrintTableElement("");
                    PrintEndTableRow();

                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement("Width", VIA_ALIGN_RIGHT);
                    snprintf(generic_string, 1023, "0x%x",
                             _vulkan_1_0_info.vk_physical_devices[iii].vk_queue_fam_props[jjj].minImageTransferGranularity.width);
                    PrintTableElement(generic_string);
                    PrintEndTableRow();

                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement("Height", VIA_ALIGN_RIGHT);
                    snprintf(generic_string, 1023, "0x%x",
                             _vulkan_1_0_info.vk_physical_devices[iii].vk_queue_fam_props[jjj].minImageTransferGranularity.height);
                    PrintTableElement(generic_string);
                    PrintEndTableRow();

                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement("Depth", VIA_ALIGN_RIGHT);
                    snprintf(generic_string, 1023, "0x%x",
                             _vulkan_1_0_info.vk_physical_devices[iii].vk_queue_fam_props[jjj].minImageTransferGranularity.depth);
                    PrintTableElement(generic_string);
                    PrintEndTableRow();
                }
            } else {
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("vkGetPhysicalDeviceQueueFamilyProperties");
                PrintTableElement("FAILED: Returned 0!");
                PrintTableElement("");
                PrintEndTableRow();
            }

            VkPhysicalDeviceMemoryProperties memory_props;
            vkGetPhysicalDeviceMemoryProperties(min_phys_devices[iii], &memory_props);

            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Memory Heaps");
            snprintf(generic_string, 1023, "%d", memory_props.memoryHeapCount);
            PrintTableElement(generic_string);
            PrintTableElement("");
            PrintEndTableRow();

            for (jjj = 0; jjj < memory_props.memoryHeapCount; jjj++) {
                PrintBeginTableRow();
                PrintTableElement("");
                snprintf(generic_string, 1023, "[%d]", jjj);
                PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
                PrintTableElement("Property Flags");
                generic_string[0] = '\0';
                bool prev_set = false;
                if (memory_props.memoryHeaps[jjj].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                    strncat(generic_string, "DEVICE_LOCAL", 1023);
                    prev_set = true;
                }
                if (!prev_set) {
                    strncat(generic_string, "--NONE--", 1023);
                }
                PrintTableElement(generic_string);
                PrintEndTableRow();

                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("");
                PrintTableElement("Heap Size");
                std::ostringstream heap_string;
                heap_string << static_cast<uint64_t>(memory_props.memoryHeaps[jjj].size);
                PrintTableElement(heap_string.str());
                PrintEndTableRow();
            }

            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Memory Types");
            snprintf(generic_string, 1023, "%d", memory_props.memoryTypeCount);
            PrintTableElement(generic_string);
            PrintTableElement("");
            PrintEndTableRow();

            for (jjj = 0; jjj < memory_props.memoryTypeCount; jjj++) {
                PrintBeginTableRow();
                PrintTableElement("");
                snprintf(generic_string, 1023, "[%d]", jjj);
                PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
                PrintTableElement("Property Flags");
                generic_string[0] = '\0';
                bool prev_set = false;
                if (memory_props.memoryTypes[jjj].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
                    strncat(generic_string, "DEVICE_LOCAL", 1023);
                    prev_set = true;
                }
                if (memory_props.memoryTypes[jjj].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
                    if (prev_set) {
                        strncat(generic_string, " | ", 1023);
                    }
                    strncat(generic_string, "HOST_VISIBLE", 1023);
                    prev_set = true;
                }
                if (memory_props.memoryTypes[jjj].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
                    if (prev_set) {
                        strncat(generic_string, " | ", 1023);
                    }
                    strncat(generic_string, "HOST_COHERENT", 1023);
                    prev_set = true;
                }
                if (memory_props.memoryTypes[jjj].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) {
                    if (prev_set) {
                        strncat(generic_string, " | ", 1023);
                    }
                    strncat(generic_string, "HOST_CACHED", 1023);
                    prev_set = true;
                }
                if (memory_props.memoryTypes[jjj].propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) {
                    if (prev_set) {
                        strncat(generic_string, " | ", 1023);
                    }
                    strncat(generic_string, "LAZILY_ALLOC", 1023);
                    prev_set = true;
                }
                if (!prev_set) {
                    strncat(generic_string, "--NONE--", 1023);
                }
                PrintTableElement(generic_string);
                PrintEndTableRow();

                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("");
                PrintTableElement("Heap Index");
                snprintf(generic_string, 1023, "%d", memory_props.memoryTypes[jjj].heapIndex);
                PrintTableElement(generic_string);
                PrintEndTableRow();
            }

            uint32_t num_ext_props;
            std::vector<VkExtensionProperties> ext_props;

            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Device Extensions");
            status = vkEnumerateDeviceExtensionProperties(min_phys_devices[iii], NULL, &num_ext_props, NULL);
            if (VK_SUCCESS != status) {
                PrintTableElement("FAILED querying number of extensions");
                PrintTableElement("");
                PrintEndTableRow();

                res = VIA_VULKAN_CANT_FIND_EXTENSIONS;
            } else {
                snprintf(generic_string, 1023, "%d", num_ext_props);
                PrintTableElement(generic_string);
                ext_props.resize(num_ext_props);
                status = vkEnumerateDeviceExtensionProperties(min_phys_devices[iii], NULL, &num_ext_props, ext_props.data());
                if (VK_SUCCESS != status) {
                    PrintTableElement("FAILED querying actual extension info");
                    PrintEndTableRow();

                    res = VIA_VULKAN_CANT_FIND_EXTENSIONS;
                } else {
                    PrintTableElement("");
                    PrintEndTableRow();

                    for (jjj = 0; jjj < num_ext_props; jjj++) {
                        PrintBeginTableRow();
                        PrintTableElement("");
                        snprintf(generic_string, 1023, "[%d]", jjj);
                        PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
                        PrintTableElement(ext_props[jjj].extensionName);
                        snprintf(generic_string, 1023, "Spec Vers %d", ext_props[jjj].specVersion);
                        PrintTableElement(generic_string);
                        PrintEndTableRow();
                    }
                }
            }
        }
    }

    // Find out the max physical device API version first and set the max total version
    // to the minimum of the instance version and the highest phsycial device version.
    if (_vulkan_max_info.vk_instance != VK_NULL_HANDLE &&
        (_vulkan_max_info.max_api_version.major > 1 || _vulkan_max_info.max_api_version.minor >= 1)) {
        PrintBeginTableRow();
        PrintTableElement("vkEnumeratePhysicalDevices [" + std::to_string(_vulkan_max_info.max_api_version.major) + "." +
                          std::to_string(_vulkan_max_info.max_api_version.minor) + "]");
        status = vkEnumeratePhysicalDevices(_vulkan_max_info.vk_instance, &gpu_count, NULL);
        if (status) {
            snprintf(generic_string, 1023, "ERROR: Failed to query - %d", status);
            PrintTableElement(generic_string);
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();
            res = VIA_VULKAN_CANT_FIND_DRIVER;
            goto out;
        }

        max_phys_devices.resize(gpu_count);
        status = vkEnumeratePhysicalDevices(_vulkan_max_info.vk_instance, &gpu_count, max_phys_devices.data());
        if (VK_SUCCESS != status && VK_INCOMPLETE != status) {
            PrintTableElement("Failed to enumerate physical devices!");
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();
            res = VIA_VULKAN_CANT_FIND_DRIVER;
            goto out;
        }

        VulkanApiVersion max_api_gpu_version = {};
        max_api_gpu_version.major = 1;
        for (iii = 0; iii < gpu_count; iii++) {
            vkGetPhysicalDeviceProperties(max_phys_devices[iii], &props);
            if (VK_VERSION_MAJOR(props.apiVersion) > max_api_gpu_version.major ||
                (VK_VERSION_MAJOR(props.apiVersion) == max_api_gpu_version.major &&
                 VK_VERSION_MINOR(props.apiVersion) >= max_api_gpu_version.minor)) {
                max_api_gpu_version.major = VK_VERSION_MAJOR(props.apiVersion);
                max_api_gpu_version.minor = VK_VERSION_MINOR(props.apiVersion);
            }
        }

        if (_vulkan_max_info.max_api_version.major > max_api_gpu_version.major ||
            (_vulkan_max_info.max_api_version.major == max_api_gpu_version.major &&
             _vulkan_max_info.max_api_version.minor > max_api_gpu_version.minor)) {
            max_overall_version = max_api_gpu_version;
        } else {
            max_overall_version = _vulkan_max_info.max_api_version;
        }
        _vulkan_max_info.desired_api_version = max_overall_version;

        if (max_overall_version.major > 1 || max_overall_version.minor >= 1) {
            for (iii = 0; iii < gpu_count; iii++) {
                vkGetPhysicalDeviceProperties(max_phys_devices[iii], &props);
                if (VK_VERSION_MAJOR(props.apiVersion) == max_overall_version.major &&
                    VK_VERSION_MINOR(props.apiVersion) == max_overall_version.minor) {
                    // Resize the max's physical device list by one more
                    size_t cur_size = _vulkan_max_info.vk_physical_devices.size();
                    _vulkan_max_info.vk_physical_devices.resize(cur_size + 1);
                    _vulkan_max_info.vk_physical_devices[max_api_gpu_count].vk_phys_dev = max_phys_devices[iii];
                    _vulkan_max_info.vk_physical_devices[max_api_gpu_count].api_version = max_overall_version;

                    uint32_t queue_fam_count = 0;
                    vkGetPhysicalDeviceQueueFamilyProperties(min_phys_devices[iii], &queue_fam_count, NULL);
                    if (0 < queue_fam_count) {
                        _vulkan_max_info.vk_physical_devices[max_api_gpu_count].vk_queue_fam_props.resize(queue_fam_count);
                        vkGetPhysicalDeviceQueueFamilyProperties(
                            max_phys_devices[iii], &queue_fam_count,
                            _vulkan_max_info.vk_physical_devices[max_api_gpu_count].vk_queue_fam_props.data());
                    }
                    max_api_gpu_count++;
                }
            }
        }
        PrintTableElement(std::to_string(max_api_gpu_count));
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
    }

    PrintEndTable();

out:

    return res;
}

// Using the previously determine information, attempt to create a logical
// device for each physical device we found.
ViaSystem::ViaResults ViaSystem::GenerateLogicalDeviceInfo() {
    ViaResults res = VIA_SUCCESSFUL;
    VkDeviceCreateInfo device_create_info{};
    VkDeviceQueueCreateInfo queue_create_info{};
    VkResult status = VK_SUCCESS;
    uint32_t dev_count;
    char generic_string[1024];
    bool found_driver = false;

    PrintBeginTable("Logical Devices", 3);

    for (uint32_t vers_index = 0; vers_index < 2; ++vers_index) {
        VulkanInstanceInfo* vulkan_info;

        if (vers_index == 0) {
            PrintBeginTableRow();
            vulkan_info = &_vulkan_1_0_info;
            PrintTableElement("vkCreateDevice [1.0]");
        } else {
            vulkan_info = &_vulkan_max_info;
            if (VK_NULL_HANDLE == vulkan_info->vk_instance || 0 >= vulkan_info->vk_physical_devices.size() ||
                (vulkan_info->max_api_version.major == 1 && vulkan_info->max_api_version.minor == 0)) {
                continue;
            }
            PrintBeginTableRow();
            snprintf(generic_string, 1023, "vkCreateDevice [%d.%d]", vulkan_info->max_api_version.major,
                     vulkan_info->max_api_version.minor);
            PrintTableElement(generic_string);
        }
        std::vector<VulkanPhysicalDeviceInfo>& phys_devices = vulkan_info->vk_physical_devices;
        dev_count = (uint32_t)phys_devices.size();

        snprintf(generic_string, 1023, "%d", dev_count);
        PrintTableElement(generic_string);
        PrintTableElement("");
        PrintEndTableRow();

        vulkan_info->vk_logical_devices.resize(dev_count);
        for (uint32_t dev = 0; dev < dev_count; dev++) {
            device_create_info = {};
            device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

            float queue_priority = 0;
            queue_create_info = {};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.pNext = NULL;
            queue_create_info.queueCount = 1;
            queue_create_info.pQueuePriorities = &queue_priority;

            for (uint32_t queue = 0; queue < phys_devices[dev].vk_queue_fam_props.size(); queue++) {
                if (0 != (phys_devices[dev].vk_queue_fam_props[queue].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                    queue_create_info.queueFamilyIndex = queue;
                    break;
                }
            }
            device_create_info.queueCreateInfoCount = 1;
            device_create_info.pQueueCreateInfos = &queue_create_info;

            std::vector<const char *> portability_device_extension_list;
            std::vector<VkExtensionProperties> ext_props;
            uint32_t prop_count = 0;
            status = vkEnumerateDeviceExtensionProperties(phys_devices[dev].vk_phys_dev, NULL, &prop_count, NULL);
            ext_props.resize(prop_count);
            if (VK_SUCCESS == status) {
                status = vkEnumerateDeviceExtensionProperties(phys_devices[dev].vk_phys_dev, NULL, &prop_count, ext_props.data());
            }
            if (VK_SUCCESS != status) {
                prop_count = 0;
            }
            device_create_info.flags = 0;
            device_create_info.enabledExtensionCount = 0;
            device_create_info.ppEnabledExtensionNames = NULL;
            if (prop_count) {
                // Need to specify the VK_KHR_portability_subset extension in vkCreateDevice on Mac.
                // This is because the Vulkan on Metal driver is not fully Vulkan conformant.
                for (const auto& extension : ext_props) {
                    if (strcmp(extension.extensionName, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME) == 0) {
                        portability_device_extension_list.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
                        device_create_info.ppEnabledExtensionNames = portability_device_extension_list.data();
                        device_create_info.enabledExtensionCount++;
                        break;
                     }
                 }
            }

            PrintBeginTableRow();
            PrintTableElement("");
            snprintf(generic_string, 1023, "[%d]", dev);
            PrintTableElement(generic_string);

            status =
                vkCreateDevice(phys_devices[dev].vk_phys_dev, &device_create_info, NULL, &vulkan_info->vk_logical_devices[dev]);
            if (VK_ERROR_INCOMPATIBLE_DRIVER == status) {
                PrintTableElement("FAILED: Incompatible Driver");
                if (!found_driver) {
                    res = VIA_VULKAN_CANT_FIND_DRIVER;
                }
            } else if (VK_ERROR_OUT_OF_HOST_MEMORY == status) {
                PrintTableElement("FAILED: Out of Host Memory");
                // If we haven't already found a driver, set an error
                if (!found_driver) {
                    res = VIA_VULKAN_FAILED_OUT_OF_MEM;
                }
            } else if (VK_SUCCESS != status) {
                snprintf(generic_string, 1023, "FAILED : VkResult code = 0x%x", status);
                PrintTableElement(generic_string);
                // If we haven't already found a driver, set an error
                if (!found_driver) {
                    res = VIA_VULKAN_FAILED_CREATE_DEVICE;
                }
            } else {
                PrintTableElement("VIA_SUCCESSFUL");
                found_driver = true;
                // Clear any potential previous errors
                res = VIA_SUCCESSFUL;
            }

            PrintEndTableRow();
        }
    }

    PrintEndTable();

    return res;
}

// Clean up all the Vulkan items we previously created and print
// out if there are any problems.
void ViaSystem::GenerateCleanupInfo(void) {
    char generic_string[1024];
    uint32_t dev_count = static_cast<uint32_t>(_vulkan_1_0_info.vk_physical_devices.size());

    PrintBeginTable("Cleanup", 3);

    PrintBeginTableRow();
    PrintTableElement("vkDestroyDevice [1.0]");
    snprintf(generic_string, 1023, "%d", dev_count);
    PrintTableElement(generic_string);
    PrintTableElement("");
    PrintEndTableRow();
    for (uint32_t dev = 0; dev < dev_count; dev++) {
        vkDestroyDevice(_vulkan_1_0_info.vk_logical_devices[dev], NULL);
        PrintBeginTableRow();
        PrintTableElement("");
        snprintf(generic_string, 1023, "[%d]", dev);
        PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
        PrintTableElement("VIA_SUCCESSFUL");
        PrintEndTableRow();
    }

    PrintBeginTableRow();
    PrintTableElement("vkDestroyInstance [1.0]");
    vkDestroyInstance(_vulkan_1_0_info.vk_instance, NULL);
    PrintTableElement("VIA_SUCCESSFUL");
    PrintTableElement("");
    PrintEndTableRow();

    if (VK_NULL_HANDLE != _vulkan_max_info.vk_instance &&
        (_vulkan_max_info.desired_api_version.major > 1 || _vulkan_max_info.desired_api_version.minor >= 1) &&
        0 < _vulkan_max_info.vk_physical_devices.size()) {
        dev_count = static_cast<uint32_t>(_vulkan_max_info.vk_logical_devices.size());
        if (0 < dev_count) {
            PrintBeginTableRow();
            snprintf(generic_string, 1023, "vkDestroyDevice [%d.%d]", _vulkan_max_info.desired_api_version.major,
                     _vulkan_max_info.desired_api_version.minor);
            PrintTableElement(generic_string);
            snprintf(generic_string, 1023, "%d", dev_count);
            PrintTableElement(generic_string);
            PrintTableElement("");
            PrintEndTableRow();
            for (uint32_t dev = 0; dev < dev_count; dev++) {
                vkDestroyDevice(_vulkan_max_info.vk_logical_devices[dev], NULL);
                PrintBeginTableRow();
                PrintTableElement("");
                snprintf(generic_string, 1023, "[%d]", dev);
                PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
                PrintTableElement("VIA_SUCCESSFUL");
                PrintEndTableRow();
            }
        }

        PrintBeginTableRow();
        snprintf(generic_string, 1023, "vkDestroyInstance [%d.%d]", _vulkan_max_info.desired_api_version.major,
                 _vulkan_max_info.desired_api_version.minor);
        PrintTableElement(generic_string);
        vkDestroyInstance(_vulkan_max_info.vk_instance, NULL);
        PrintTableElement("VIA_SUCCESSFUL");
        PrintTableElement("");
        PrintEndTableRow();
    }
    PrintEndTable();
}

// Print information on any Vulkan commands we can (or can't) execute.
ViaSystem::ViaResults ViaSystem::GenerateVulkanInfo(void) {
    ViaResults res = VIA_SUCCESSFUL;
    bool created = false;

    BeginSection("Vulkan API Calls");

    res = GenerateInstanceInfo();
    if (res != VIA_SUCCESSFUL) {
        goto out;
    }
    created = true;
    res = GeneratePhysDevInfo();
    if (res != VIA_SUCCESSFUL) {
        goto out;
    }
    res = GenerateLogicalDeviceInfo();
    if (res != VIA_SUCCESSFUL) {
        goto out;
    }

out:

    if (created) {
        GenerateCleanupInfo();
    }

    EndSection();

    return res;
}

// Print methods

void ViaSystem::StartOutput(const std::string& title) {
    if (_out_file_format == VIA_HTML_FORMAT) {
        StartOutputHTML(title);
    } else if (_out_file_format == VIA_VKCONFIG_FORMAT) {
        StartOutputVkConfig(title);
    }
}

// Close out writing to the file.
void ViaSystem::EndOutput() {
    if (_out_file_format == VIA_HTML_FORMAT) {
        EndOutputHTML();
    } else if (_out_file_format == VIA_VKCONFIG_FORMAT) {
        EndOutputVkConfig();
    }
}

void ViaSystem::BeginSection(const std::string& section_str) {
    if (_out_file_format == VIA_HTML_FORMAT) {
        BeginSectionHTML(section_str);
    } else if (_out_file_format == VIA_VKCONFIG_FORMAT) {
        BeginSectionVkConfig(section_str);
    }
}

void ViaSystem::EndSection() {
    if (_out_file_format == VIA_HTML_FORMAT) {
        EndSectionHTML();
    } else if (_out_file_format == VIA_VKCONFIG_FORMAT) {
        EndSectionVkConfig();
    }
}

void ViaSystem::PrintStandardText(const std::string& text_str) {
    if (_out_file_format == VIA_HTML_FORMAT) {
        PrintStandardTextHTML(text_str);
    } else if (_out_file_format == VIA_VKCONFIG_FORMAT) {
        PrintStandardTextVkConfig(text_str);
    }
}

void ViaSystem::PrintBeginTable(const std::string& table_name, uint32_t num_cols) {
    if (_out_file_format == VIA_HTML_FORMAT) {
        PrintBeginTableHTML(table_name, num_cols);
    } else if (_out_file_format == VIA_VKCONFIG_FORMAT) {
        PrintBeginTableVkConfig(table_name);
    }
}

void ViaSystem::PrintBeginTableRow() {
    if (_out_file_format == VIA_HTML_FORMAT) {
        PrintBeginTableRowHTML();
    } else if (_out_file_format == VIA_VKCONFIG_FORMAT) {
        PrintBeginTableRowVkConfig();
    }
}

void ViaSystem::PrintTableElement(const std::string& element, ViaElementAlign align) {
    if (_out_file_format == VIA_HTML_FORMAT) {
        PrintTableElementHTML(element, align);
    } else if (_out_file_format == VIA_VKCONFIG_FORMAT) {
        PrintTableElementVkConfig(element);
    }
}

void ViaSystem::PrintEndTableRow() {
    if (_out_file_format == VIA_HTML_FORMAT) {
        PrintEndTableRowHTML();
    } else if (_out_file_format == VIA_VKCONFIG_FORMAT) {
        PrintEndTableRowVkConfig();
    }
}

void ViaSystem::PrintEndTable() {
    if (_out_file_format == VIA_HTML_FORMAT) {
        PrintEndTableHTML();
    } else if (_out_file_format == VIA_VKCONFIG_FORMAT) {
        PrintEndTableVkConfig();
    }
}

// HTML methods

// Start writing to the HTML file by creating the appropriate
// header information including the appropriate CSS and JavaScript
// items.
void ViaSystem::StartOutputHTML(const std::string& title) {
    _out_ofstream << "<!DOCTYPE html>" << std::endl;
    _out_ofstream << "<HTML lang=\"en\" xml:lang=\"en\" "
                     "xmlns=\"http://www.w3.org/1999/xhtml\">"
                  << std::endl;
    _out_ofstream << std::endl << "<HEAD>" << std::endl << "    <TITLE>" << title << "</TITLE>" << std::endl;

    _out_ofstream << "    <META charset=\"UTF-8\">" << std::endl
                  << "    <style media=\"screen\" type=\"text/css\">" << std::endl
                  << "        html {"
                  << std::endl
                  // By defining the color first, this won't override the background image
                  // (unless the images aren't there).
                  << "            background-color: #0b1e48;"
                  << std::endl
                  // The following changes try to load the text image twice (locally, then
                  // off the web) followed by the background image twice (locally, then
                  // off the web).  The background color will only show if both background
                  // image loads fail.  In this way, a user will see their local copy on
                  // their machine, while a person they share it with will see the web
                  // images (or the background color).
                  << "            background-image: url(\"https://vulkan.lunarg.com/img/VIATitle.png\"), "
                  << "url(\"https://vulkan.lunarg.com/img/VIABackground.jpg\");" << std::endl
                  << "            background-position: center top, center;" << std::endl
                  << "            -webkit-background-size: auto, cover;" << std::endl
                  << "            -moz-background-size: auto, cover;" << std::endl
                  << "            -o-background-size: auto, cover;" << std::endl
                  << "            background-size: auto, cover;" << std::endl
                  << "            background-attachment: scroll, fixed;" << std::endl
                  << "            background-repeat: no-repeat, no-repeat;" << std::endl
                  << "        }"
                  << std::endl
                  // h1.section is used for section headers, and h1.version is used to
                  // print out the application version text (which shows up just under
                  // the title).
                  << "        h1.section {" << std::endl
                  << "            font-family: sans-serif;" << std::endl
                  << "            font-size: 35px;" << std::endl
                  << "            color: #FFFFFF;" << std::endl
                  << "        }" << std::endl
                  << "        h1.version {" << std::endl
                  << "            font-family: sans-serif;" << std::endl
                  << "            font-size: 25px;" << std::endl
                  << "            color: #FFFFFF;" << std::endl
                  << "        }" << std::endl
                  << "        h2.note {" << std::endl
                  << "            font-family: sans-serif;" << std::endl
                  << "            font-size: 12px;" << std::endl
                  << "            color: #FFFFFF;" << std::endl
                  << "        }" << std::endl
                  << "        table {" << std::endl
                  << "            min-width: 600px;" << std::endl
                  << "            width: 70%;" << std::endl
                  << "            border-collapse: collapse;" << std::endl
                  << "            border-color: grey;" << std::endl
                  << "            font-family: sans-serif;" << std::endl
                  << "        }" << std::endl
                  << "        td.header {" << std::endl
                  << "            padding: 18px;" << std::endl
                  << "            border: 1px solid #ccc;" << std::endl
                  << "            font-size: 18px;" << std::endl
                  << "            color: #fff;" << std::endl
                  << "        }" << std::endl
                  << "        td.odd {" << std::endl
                  << "            padding: 10px;" << std::endl
                  << "            border: 1px solid #ccc;" << std::endl
                  << "            font-size: 16px;" << std::endl
                  << "            color: rgb(255, 255, 255);" << std::endl
                  << "        }" << std::endl
                  << "        td.even {" << std::endl
                  << "            padding: 10px;" << std::endl
                  << "            border: 1px solid #ccc;" << std::endl
                  << "            font-size: 16px;" << std::endl
                  << "            color: rgb(220, 220, 220);" << std::endl
                  << "        }" << std::endl
                  << "        tr.header {" << std::endl
                  << "            background-color: rgba(64,64,64,0.75);" << std::endl
                  << "        }" << std::endl
                  << "        tr.odd {" << std::endl
                  << "            background-color: rgba(0,0,0,0.6);" << std::endl
                  << "        }" << std::endl
                  << "        tr.even {" << std::endl
                  << "            background-color: rgba(0,0,0,0.7);" << std::endl
                  << "        }" << std::endl
                  << "    </style>" << std::endl
                  << "    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/"
                  << "2.2.4/jquery.min.js\"></script>" << std::endl
                  << "    <script type=\"text/javascript\">" << std::endl
                  << "        $( document ).ready(function() {" << std::endl
                  << "            $('table tr:not(.header)').hide();" << std::endl
                  << "            $('.header').click(function() {" << std::endl
                  << "                "
                     "$(this).nextUntil('tr.header').slideToggle(300);"
                  << std::endl
                  << "            });" << std::endl
                  << "        });" << std::endl
                  << "    </script>" << std::endl
                  << "</HEAD>" << std::endl
                  << std::endl
                  << "<BODY>" << std::endl
                  << std::endl;
    // We need space from the top for the VIA texture
    for (uint32_t space = 0; space < 15; space++) {
        _out_ofstream << "    <BR />" << std::endl;
    }

    _out_ofstream << "<center><h2 class=\"note\">< NOTE: Click on section name to expand "
                     "table ></h2></center>"
                  << std::endl
                  << "    <BR />" << std::endl;
}

// Close out writing to the HTML file.
void ViaSystem::EndOutputHTML() { _out_ofstream << "</BODY>" << std::endl << std::endl << "</HTML>" << std::endl; }

void ViaSystem::BeginSectionHTML(const std::string& section_str) {
    _out_ofstream << "    <H1 class=\"section\"><center>" << section_str << "</center></h1>" << std::endl;
}

void ViaSystem::EndSectionHTML() { _out_ofstream << "    <BR/>" << std::endl << "    <BR/>" << std::endl; }

void ViaSystem::PrintStandardTextHTML(const std::string& text_str) {
    _out_ofstream << "    <H2><font color=\"White\">" << text_str << "</font></H2>" << std::endl;
}

void ViaSystem::PrintBeginTableHTML(const std::string& table_name, uint32_t num_cols) {
    _out_ofstream << "    <table align=\"center\">" << std::endl
                  << "        <tr class=\"header\">" << std::endl
                  << "            <td colspan=\"" << num_cols << "\" class=\"header\">" << table_name << "</td>" << std::endl
                  << "        </tr>" << std::endl;

    _outputting_to_odd_row = true;
}

void ViaSystem::PrintBeginTableRowHTML() {
    std::string class_str = "";
    if (_outputting_to_odd_row) {
        class_str = " class=\"odd\"";
    } else {
        class_str = " class=\"even\"";
    }
    _out_ofstream << "        <tr" << class_str << ">" << std::endl;
}

void ViaSystem::PrintTableElementHTML(const std::string& element, ViaElementAlign align) {
    std::string align_str = "";
    std::string class_str = "";
    if (align == VIA_ALIGN_RIGHT) {
        align_str = " align=\"right\"";
    } else if (align == VIA_ALIGN_CENTER) {
        align_str = " align=\"center\"";
    }
    if (_outputting_to_odd_row) {
        class_str = " class=\"odd\"";
    } else {
        class_str = " class=\"even\"";
    }
    _out_ofstream << "            <td" << align_str << class_str << ">" << element << "</td>" << std::endl;
}

void ViaSystem::PrintEndTableRowHTML() {
    _out_ofstream << "        </tr>" << std::endl;
    _outputting_to_odd_row = !_outputting_to_odd_row;
}

void ViaSystem::PrintEndTableHTML() { _out_ofstream << "    </table>" << std::endl; }

// VkConfig print methods

void ViaSystem::StartOutputVkConfig(const std::string& title) {
    _table_count = 0;
    _standard_text_count = 0;
    _out_ofstream << "{" << std::endl;
}

void ViaSystem::EndOutputVkConfig() { _out_ofstream << "\n}" << std::endl; }

void ViaSystem::BeginSectionVkConfig(const std::string& section_str) { return; }

void ViaSystem::EndSectionVkConfig() { return; }

void ViaSystem::PrintStandardTextVkConfig(const std::string& text_str) {
    if (_table_count > 0) {
        _out_ofstream << "," << std::endl;
    }
    _out_ofstream << "\t\"" << _standard_text_count << "\": \"" << text_str << "\"";
    _table_count++;
    _standard_text_count++;
}

void ViaSystem::PrintBeginTableVkConfig(const std::string& table_name) {
    _row_count = 0;
    if (_table_count > 0) {
        _out_ofstream << "," << std::endl;
    }
    _out_ofstream << "\t\"" << table_name << "\": {" << std::endl;
    _table_count++;
}
void ViaSystem::PrintBeginTableRowVkConfig() {
    _col_count = 0;
    if (_row_count > 0) {
        _out_ofstream << "," << std::endl;
    }
    _out_ofstream << "\t\t\"" << _row_count << "\": {" << std::endl;
    _row_count++;
}

void ViaSystem::PrintTableElementVkConfig(const std::string& element) {
    if (_col_count > 0) {
        _out_ofstream << "," << std::endl;
    }
    _out_ofstream << "\t\t\t\"" << _col_count << "\": \"" << element << "\"";
    _col_count++;
}

void ViaSystem::PrintEndTableRowVkConfig() { _out_ofstream << "\n\t\t}"; }

void ViaSystem::PrintEndTableVkConfig() { _out_ofstream << "\n\t}"; }

// Trim any whitespace preceeding or following the actual
// content inside of a string.  The actual items labeled
// as whitespace are passed in as the second set of
// parameters.
std::string ViaSystem::TrimWhitespace(const std::string& str, const std::string& whitespace) {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) {
        return "";  // no content
    }

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string ViaSystem::ConvertPathFormat(std::string str) {
    size_t index = 0;
    while (true) {
        index = str.find(_directory_symbol, index);
        if (index == std::string::npos) {
            break;
        }
        str.replace(index, 1, "/");
        index++;
    }

    return str;
}

static void ReplaceAllBackslashes(std::string& str) {
    size_t bs_pos = str.find("\\");
    while (bs_pos != std::string::npos) {
#ifdef VIA_WINDOWS_TARGET
        // Accept existing double-backslashes on Windows
        if (str.at(bs_pos + 1) != '\\') {
            str.replace(bs_pos, 1, "\\\\");
        }
        bs_pos = str.find("\\", bs_pos + 2);
#else
        // Just always replace backslashes on other platforms
        str.replace(bs_pos, 1, "/");
        bs_pos = str.find("\\", bs_pos + 1);
#endif
    }
}

void ViaSystem::GenerateSettingsFileJsonInfo(const std::string& settings_file) {
    std::map<std::string, std::vector<VulkanSettingPair>> settings;

    // Load the file from the appropriate location
    PrintBeginTableRow();
    PrintTableElement("");

    std::ifstream* settings_stream = new std::ifstream(settings_file, std::ifstream::in);
    if (nullptr == settings_stream || settings_stream->fail()) {
        // No file was found.  This is NOT an error.
        PrintTableElement(ConvertPathFormat(settings_file));
        PrintTableElement("File not found");
        PrintTableElement("");
        PrintEndTableRow();
    } else {
        // We found a file, so parse it.
        PrintTableElement(ConvertPathFormat(settings_file));
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();

        // The settings file is a text file where:
        //  - # indicates a comment
        //  - Settings are stored in the fashion:
        //        <layer_name>.<setting> = <value>
        while (settings_stream->good()) {
            std::string cur_line;
            getline(*settings_stream, cur_line);
            std::string trimmed_line = TrimWhitespace(cur_line);

            // Skip blank and comment lines
            if (trimmed_line.length() == 0 || trimmed_line.c_str()[0] == '#') {
                continue;
            }

            // If no equal, treat as unknown
            size_t equal_loc = trimmed_line.find("=");
            if (equal_loc == std::string::npos) {
                continue;
            }

            VulkanSettingPair new_pair;

            std::string before_equal = trimmed_line.substr(0, equal_loc);
            std::string after_equal = trimmed_line.substr(equal_loc + 1, std::string::npos);
            new_pair.value = TrimWhitespace(after_equal);
            ReplaceAllBackslashes(new_pair.value);

            std::string trimmed_setting = TrimWhitespace(before_equal);

            // Look for period
            std::string setting_layer = "--None--";
            std::string setting_name = "";
            size_t period_loc = trimmed_setting.find(".");
            if (period_loc == std::string::npos) {
                setting_name = trimmed_setting;
            } else {
                setting_layer = trimmed_setting.substr(0, period_loc);
                setting_name = trimmed_setting.substr(period_loc + 1, std::string::npos);
            }
            new_pair.name = setting_name;

            // Add items to settings map for now
            if (settings.find(setting_layer) == settings.end()) {
                // Not found
                std::vector<VulkanSettingPair> new_vector;
                new_vector.push_back(new_pair);
                settings[setting_layer] = new_vector;
            } else {
                // Already exists
                std::vector<VulkanSettingPair>& cur_vector = settings[setting_layer];
                cur_vector.push_back(new_pair);
            }
        }

        // Now that all items have been grouped in the settings map
        // appropriately, print
        // them out
        for (auto layer_iter = settings.begin(); layer_iter != settings.end(); layer_iter++) {
            std::vector<VulkanSettingPair>& cur_vector = layer_iter->second;
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement(layer_iter->first, VIA_ALIGN_RIGHT);
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();
            for (uint32_t cur_item = 0; cur_item < cur_vector.size(); cur_item++) {
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("");
                PrintTableElement(cur_vector[cur_item].name);
                PrintTableElement(cur_vector[cur_item].value);
                PrintEndTableRow();
            }
        }

        settings_stream->close();
        delete settings_stream;
    }
}

// Print out the information stored in an explicit layer's JSON file.
void ViaSystem::GenerateExplicitLayerJsonInfo(const char* layer_json_filename, Json::Value root) {
    char generic_string[1024];
    uint32_t ext;
    if (!root["layer"].isNull()) {
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("");
        PrintTableElement("Name");
        if (!root["layer"]["name"].isNull()) {
            PrintTableElement(root["layer"]["name"].asString());
        } else {
            PrintTableElement("MISSING!");
        }
        PrintEndTableRow();

        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("");
        PrintTableElement("Description");
        if (!root["layer"]["description"].isNull()) {
            PrintTableElement(root["layer"]["description"].asString());
        } else {
            PrintTableElement("MISSING!");
        }
        PrintEndTableRow();

        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("");
        PrintTableElement("API Version");
        if (!root["layer"]["api_version"].isNull()) {
            PrintTableElement(root["layer"]["api_version"].asString());
        } else {
            PrintTableElement("MISSING!");
        }
        PrintEndTableRow();

        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("");
        PrintTableElement("JSON File Version");
        if (!root["file_format_version"].isNull()) {
            PrintTableElement(root["file_format_version"].asString());
        } else {
            PrintTableElement("MISSING!");
        }
        PrintEndTableRow();

        Json::Value component_layers = root["layer"]["component_layers"];
        Json::Value library_path = root["layer"]["library_path"];
        if (!component_layers.isNull() && !library_path.isNull()) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("Library Path / Component Layers");
            PrintTableElement("BOTH DEFINED!");
            PrintEndTableRow();
        } else if (!library_path.isNull()) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("Library Path");
            PrintTableElement(ConvertPathFormat(library_path.asString()));
            PrintEndTableRow();

            PrintFileVersionInfo(layer_json_filename, library_path.asString().c_str());

            char count_str[1024];
            Json::Value dev_exts = root["layer"]["device_extensions"];
            ext = 0;
            if (!dev_exts.isNull() && dev_exts.isArray()) {
                snprintf(count_str, 1023, "%d", dev_exts.size());
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("");
                PrintTableElement("Device Extensions");
                PrintTableElement(count_str);
                PrintEndTableRow();

                for (Json::ValueIterator dev_ext_it = dev_exts.begin(); dev_ext_it != dev_exts.end(); dev_ext_it++) {
                    Json::Value dev_ext = (*dev_ext_it);
                    Json::Value dev_ext_name = dev_ext["name"];
                    if (!dev_ext_name.isNull()) {
                        snprintf(generic_string, 1023, "[%d]", ext);
                        PrintBeginTableRow();
                        PrintTableElement("");
                        PrintTableElement("");
                        PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
                        PrintTableElement(dev_ext_name.asString());
                        PrintEndTableRow();
                    }
                }
            }
            Json::Value inst_exts = root["layer"]["instance_extensions"];
            ext = 0;
            if (!inst_exts.isNull() && inst_exts.isArray()) {
                snprintf(count_str, 1023, "%d", inst_exts.size());
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("");
                PrintTableElement("Instance Extensions");
                PrintTableElement(count_str);
                PrintEndTableRow();

                for (Json::ValueIterator inst_ext_it = inst_exts.begin(); inst_ext_it != inst_exts.end(); inst_ext_it++) {
                    Json::Value inst_ext = (*inst_ext_it);
                    Json::Value inst_ext_name = inst_ext["name"];
                    if (!inst_ext_name.isNull()) {
                        snprintf(generic_string, 1023, "[%d]", ext);
                        PrintBeginTableRow();
                        PrintTableElement("");
                        PrintTableElement("");
                        PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
                        PrintTableElement(inst_ext_name.asString());
                        PrintEndTableRow();
                    }
                }
            }
        } else if (!component_layers.isNull()) {
            if (component_layers.isArray()) {
                snprintf(generic_string, 1023, "%d", component_layers.size());
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("");
                PrintTableElement("Component Layers");
                PrintTableElement(generic_string);
                PrintEndTableRow();

                for (Json::ValueIterator cl_it = component_layers.begin(); cl_it != component_layers.end(); cl_it++) {
                    Json::Value comp_layer = (*cl_it);
                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement(comp_layer.asString(), VIA_ALIGN_RIGHT);
                    PrintEndTableRow();
                }
            } else {
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("");
                PrintTableElement("Component Layers");
                PrintTableElement("NOT AN ARRAY!");
                PrintEndTableRow();
            }
        } else {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("Library Path / Component Layers");
            PrintTableElement("MISSING!");
            PrintEndTableRow();
        }

    } else {
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("Layer Section");
        PrintTableElement("MISSING!");
        PrintTableElement("");
        PrintEndTableRow();
    }
}

// Print out the information about an Implicit layer stored in
// it's JSON file.  For the most part, it is similar to an
// explicit layer, so we re-use that code.  However, implicit
// layers have a DISABLE environment variable that can be used
// to disable the layer by default.  Additionally, some implicit
// layers have an ENABLE environment variable so that they are
// disabled by default, but can be enabled.
void ViaSystem::GenerateImplicitLayerJsonInfo(const char* layer_json_filename, Json::Value root,
                                              std::vector<std::string>& override_paths) {
    bool enabled = true;
    bool expired = false;
    std::string enable_env_variable = "--NONE--";
    bool enable_var_set = false;
    std::string enable_return = "";
    std::string disable_env_variable = "--NONE--";
    bool disable_var_set = false;
    std::string disable_return = "";

    GenerateExplicitLayerJsonInfo(layer_json_filename, root);

    // Record any override paths that may be present forcing us to look for explicit layers in a
    // particular location.
    Json::Value override_paths_json = root["layer"]["override_paths"];
    if (!override_paths_json.isNull() && override_paths_json.isArray()) {
        std::string combined_paths;
        uint32_t num_paths = 0;

        for (Json::ValueIterator override_it = override_paths_json.begin(); override_it != override_paths_json.end();
             override_it++) {
            Json::Value override_path = (*override_it);
            if (num_paths++ > 0) {
                combined_paths += ":";
            }
            combined_paths += override_path.asString();
            override_paths.push_back(override_path.asString());
        }
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("Override Paths");
        PrintTableElement(std::to_string(num_paths));
        PrintTableElement(combined_paths);
        PrintEndTableRow();
    }

    Json::Value enable = root["layer"]["enable_environment"];
    if (!enable.isNull()) {
        for (Json::Value::iterator en_iter = enable.begin(); en_iter != enable.end(); en_iter++) {
            if (en_iter.key().isNull()) {
                continue;
            }
            enable_env_variable = en_iter.key().asString();
            // If an enable define exists, set it to disabled by default.
            enabled = false;
            enable_return = GetEnvironmentalVariableValue(enable_env_variable);
            if (atoi(enable_return.c_str()) != 0) {
                enable_var_set = true;
                enabled = true;
            }
            break;
        }
    }

    Json::Value disable = root["layer"]["disable_environment"];
    if (!disable.isNull()) {
        for (Json::Value::iterator dis_iter = disable.begin(); dis_iter != disable.end(); dis_iter++) {
            if (dis_iter.key().isNull()) {
                continue;
            }
            disable_env_variable = dis_iter.key().asString();
            disable_return = GetEnvironmentalVariableValue(disable_env_variable);
            if (atoi(disable_return.c_str()) != 0) {
                disable_var_set = true;
                enabled = false;
            }
            break;
        }
    }

    // See if this implicit layer has an expiration.  If it's expired, mark it as disabled.
    Json::Value expiration_json = root["layer"]["expiration"];
    if (!expiration_json.isNull()) {
        char date_copy[32];
        OverrideExpiration expiration = {};
        uint8_t cur_item = 0;
        strncpy(date_copy, expiration_json.asString().c_str(), 31);
        if (strlen(date_copy) == 16) {
            char* cur_start = &date_copy[0];
            char* next_dash = strchr(date_copy, '-');
            if (NULL != next_dash) {
                while (cur_item < 5 && strlen(cur_start)) {
                    if (next_dash != NULL) {
                        *next_dash = '\0';
                    }
                    switch (cur_item) {
                        case 0:  // Year
                            expiration.year = atoi(cur_start);
                            break;
                        case 1:  // Month
                            expiration.month = atoi(cur_start);
                            break;
                        case 2:  // Day
                            expiration.day = atoi(cur_start);
                            break;
                        case 3:  // Hour
                            expiration.hour = atoi(cur_start);
                            break;
                        case 4:  // Minute
                            expiration.minute = atoi(cur_start);
                            break;
                        default:  // Ignore
                            break;
                    }
                    if (next_dash != NULL) {
                        cur_start = next_dash + 1;
                        next_dash = strchr(cur_start, '-');
                    }
                    cur_item++;
                }
            }
            if (enabled) {
                expired = CheckExpiration(expiration);
                enabled = expired;
            }
            std::string expiration_string = std::to_string(expiration.year);
            expiration_string += "/";
            expiration_string += std::to_string(expiration.month);
            expiration_string += "/";
            expiration_string += std::to_string(expiration.day);
            expiration_string += " ";
            expiration_string += std::to_string(expiration.hour);
            expiration_string += ":";
            expiration_string += std::to_string(expiration.minute);
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Expiration");
            PrintTableElement(expiration_string);
            PrintTableElement("");
            PrintEndTableRow();
        }
    }

    // Print the overall state (ENABLED or DISABLED) so we can
    // quickly determine if this layer is being used.
    PrintBeginTableRow();
    PrintTableElement("");
    PrintTableElement("");
    PrintTableElement("Enabled State");
    PrintTableElement(expired ? "EXPIRED" : (enabled ? "ENABLED" : "DISABLED"));
    PrintEndTableRow();
    PrintBeginTableRow();
    PrintTableElement("");
    PrintTableElement("Enable Env Var", VIA_ALIGN_RIGHT);
    PrintTableElement(enable_env_variable);
    if (enable_var_set) {
        PrintTableElement(enable_return);
    } else {
        PrintTableElement("Not Defined");
    }
    PrintEndTableRow();
    PrintBeginTableRow();
    PrintTableElement("");
    PrintTableElement("Disable Env Var", VIA_ALIGN_RIGHT);
    PrintTableElement(disable_env_variable);
    if (disable_var_set) {
        PrintTableElement(disable_return);
    } else {
        PrintTableElement("Not Defined");
    }
    PrintEndTableRow();
}

// Run any external tests we can find, and print the results of those tests.
ViaSystem::ViaResults ViaSystem::GenerateTestInfo(void) {
    ViaResults res = VIA_SUCCESSFUL;

    BeginSection("External Tests");
    if (_found_sdk) {
        std::string cube_exe;
        std::string full_cmd;
        std::string path = "";

        LogInfo("SDK Found! - Will attempt to run tests");
        for (uint32_t pass = 0; pass < 2; ++pass) {
            switch (pass) {
                case 0:
                default:
                    cube_exe = "vkcube";
                    break;
                case 1:
                    cube_exe = "cube";
            }
#ifdef VIA_WINDOWS_TARGET
            cube_exe += ".exe";
            if (!_is_system_installed_sdk) {
                path = _sdk_path;
#if _WIN64
                path += "\\Bin";
#else
                path += "\\Bin32";
#endif
            }
#elif VIA_LINUX_TARGET || VIA_BSD_TARGET
            if (!_is_system_installed_sdk) {
                cube_exe = "./" + cube_exe;
                path = _sdk_path;
                path += "/bin";
            }
#elif VIA_MACOS_TARGET
            cube_exe = "./" + cube_exe;
            if (!_is_system_installed_sdk) {
                path = _sdk_path;
                path += "/../Applications/vkcube.app/Contents/MacOS";
            } else {
                path = "/Applications/vkcube.app/Contents/MacOS";
            }
#endif

            LogInfo("   Attempting to run " + cube_exe + " in " + path);

            full_cmd = cube_exe;
            full_cmd += " --c 100 --suppress_popups";
            int test_result = RunTestInDirectory(path, cube_exe, full_cmd);

            PrintBeginTable("Cube", 2);

            PrintBeginTableRow();
            PrintTableElement(full_cmd);
            if (test_result == 0) {
                PrintTableElement("VIA_SUCCESSFUL");
                _ran_tests = true;
            } else if (test_result == 1) {
                PrintTableElement("Not Found");
            } else {
                PrintTableElement("FAILED!");
                res = VIA_TEST_FAILED;
            }
            PrintEndTableRow();

            full_cmd += " --validate";

            PrintBeginTableRow();
            PrintTableElement(full_cmd);
            test_result = RunTestInDirectory(path, cube_exe, full_cmd);
            if (test_result == 0) {
                PrintTableElement("VIA_SUCCESSFUL");
                _ran_tests = true;
            } else if (test_result == 1) {
                PrintTableElement("Not Found");
            } else {
                PrintTableElement("FAILED!");
                res = VIA_TEST_FAILED;
            }
            PrintEndTableRow();

            // Make it this far, we shouldn't test anymore
            break;
        }
        PrintEndTable();
    } else {
        PrintStandardText("No SDK found by VIA, skipping test section");
    }
    EndSection();

    return res;
}
