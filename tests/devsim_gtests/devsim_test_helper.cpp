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

#include <sstream>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "devsim_test_helper.h"

void devsim_test::setEnvironmentSetting(std::string setting, const char* val) {
#ifdef _WIN32
    _putenv_s(setting.c_str(), val);
#else
    setenv(setting.c_str(), val, 1);
#endif
}

void devsim_test::unsetEnvironmentSetting(std::string setting) {
#ifdef _WIN32
    _putenv_s(setting.c_str(), "");
#else
    unsetenv(setting.c_str());
#endif
}

std::string devsim_test::getAbsolutePath(std::string filepath) {
    std::string out;
#ifdef _WIN32
    char abs_path[_MAX_PATH];
    _fullpath(abs_path, filepath.c_str(), _MAX_PATH);
    out = abs_path;
#else
    char* abs_path = realpath(filepath.c_str(), nullptr);
    out = abs_path;
    delete[] abs_path;
#endif
    return out;
}

void devsim_test::initLayerPath() {
#ifdef _WIN32
#ifdef NDEBUG
    std::string layer_path = TOP_SOURCE_PATH "/build/layersvt/Release";
#else
    std::string layer_path = TOP_SOURCE_PATH "/build/layersvt/Debug";
#endif
#else
    std::string layer_path = TOP_SOURCE_PATH "/build/layersvt";
#endif  // _WIN32

    setEnvironmentSetting("VK_LAYER_PATH", layer_path.c_str());
}

std::string devsim_test::arrayCombinationModeToString(ArrayCombinationMode mode) {
    switch (mode) {
    case ARRAY_COMBINATION_MODE_NONE:
        return "none";
    case ARRAY_COMBINATION_MODE_REPLACE:
        return "replace";
    case ARRAY_COMBINATION_MODE_WHITELIST:
        return "whitelist";
    case ARRAY_COMBINATION_MODE_BLACKLIST:
        return "blacklist";
    case ARRAY_COMBINATION_MODE_INTERSECT:
        return "intersect";
    default:
        break;
    }

    return "none";
}

void devsim_test::setDevsimFilenames(std::vector<std::string>& filepaths) {
    std::stringstream out;
#ifdef _WIN32
    char seperator = ';';
#else
    char seperator = ':';
#endif // _WIN32

    for (std::string& filepath : filepaths) {
        std::string config_filepath = devsim_test::getAbsolutePath(filepath);
        out << filepath << seperator;
    }
    devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_FILENAME", out.str().c_str());
}

void devsim_test::setDevsimDebugEnable(bool enable) {
    if (enable)
        devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_DEBUG_ENABLE", "true");
    else
        devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_DEBUG_ENABLE", "false");
}

void devsim_test::setDevsimExitOnError(bool enable) {
    if (enable)
        devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_EXIT_ON_ERROR", "true");
    else
        devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_EXIT_ON_ERROR", "false");
}

void devsim_test::setDevsimEmulatePortabilitySubsetExtension(bool enable) {
    if (enable)
        devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_EMULATE_PORTABILITY", "true");
    else
        devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_EMULATE_PORTABILITY", "false");
}

void devsim_test::setDevsimModifyExtensionList(ArrayCombinationMode mode) {
    devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_MODIFY_EXTENSION_LIST",
                                       arrayCombinationModeToString(mode).c_str());
}

void devsim_test::setDevsimModifyMemoryFags(bool enable) {
    if (enable)
        devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_MODIFY_MEMORY_FLAGS", "true");
    else
        devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_MODIFY_MEMORY_FLAGS", "false");
}

void devsim_test::setDevsimModifyFormatList(ArrayCombinationMode mode) {
    devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_MODIFY_FORMAT_LIST",
                                       arrayCombinationModeToString(mode).c_str());
}

void devsim_test::setDevsimModifyFormatProperties(ArrayCombinationMode mode) {
    devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_MODIFY_FORMAT_PROPERTIES",
                                       arrayCombinationModeToString(mode).c_str());
}

void devsim_test::setDevsimModifySurfaceFormats(ArrayCombinationMode mode) {
    devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_MODIFY_SURFACE_FORMAT",
                                       arrayCombinationModeToString(mode).c_str());
}

void devsim_test::setDevsimModifyPresentModes(ArrayCombinationMode mode) {
    devsim_test::setEnvironmentSetting("VK_LUNARG_DEVICE_SIMULATION_MODIFY_PRESENT_MODES",
                                       arrayCombinationModeToString(mode).c_str());
}

VkApplicationInfo devsim_test::GetDefaultApplicationInfo() {
    VkApplicationInfo out{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
    out.apiVersion = VK_API_VERSION_1_0;
    out.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    out.pApplicationName = "devsim_tests";
    out.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    out.pEngineName = "devsim_tests";

    return out;
}

VkResult devsim_test::VulkanInstanceBuilder::makeInstance() {
    _inst_create_info.pApplicationInfo = &_app_info;
    _inst_create_info.enabledLayerCount = static_cast<uint32_t>(_layer_names.size());
    _inst_create_info.ppEnabledLayerNames = _layer_names.data();
    _inst_create_info.enabledExtensionCount = static_cast<uint32_t>(_extension_names.size());
    _inst_create_info.ppEnabledExtensionNames = _extension_names.data();

    return vkCreateInstance(&_inst_create_info, nullptr, &_instance);
}

void devsim_test::VulkanInstanceBuilder::reset() {
    _app_info = GetDefaultApplicationInfo();
    _inst_create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    _instance = VK_NULL_HANDLE;

    _layer_names.clear();
    _extension_names.clear();
}