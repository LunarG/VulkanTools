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

#ifdef VIA_WINDOWS_TARGET

#pragma once

#include <tuple>

#pragma warning(disable : 4996)
#include <shlwapi.h>
#include <Cfgmgr32.h>
#include <psapi.h>
#include <windows.h>

#include "via_system.hpp"

class ViaSystemWindows : public ViaSystem {
   public:
    ViaSystemWindows();

   protected:
    virtual bool IsAbsolutePath(const std::string& path) override;
    virtual int RunTestInDirectory(std::string path, std::string test, std::string cmd_line) override;
    virtual ViaResults PrintSystemEnvironmentInfo();
    virtual ViaResults PrintSystemHardwareInfo();
    virtual ViaResults PrintSystemExecutableInfo();
    virtual ViaResults PrintSystemDriverInfo();
    virtual ViaResults PrintSystemLoaderInfo();
    virtual ViaResults PrintSystemSdkInfo();
    virtual ViaResults PrintSystemImplicitLayerInfo();
    virtual ViaResults PrintSystemExplicitLayerInfo();
    virtual ViaResults PrintSystemSettingsFileInfo();
    virtual void PrintFileVersionInfo(const std::string& json_filename, const std::string& library);
    virtual bool CheckExpiration(OverrideExpiration expiration);
    virtual std::string GetEnvironmentalVariableValue(const std::string& env_var);
    virtual bool ExpandPathWithEnvVar(std::string& path);

   private:
    bool FindDriverIdsFromPlugAndPlay();
    bool FindDriverSpecificRegistryJsons(const std::string& key_name,
                                         std::vector<std::tuple<std::string, bool, std::string>>& json_paths);
    bool PrintDriverRegistryInfo(std::vector<std::tuple<std::string, bool, std::string>>& cur_driver_json, std::string system_path,
                                 bool& found_lib);
    bool GetFileVersion(const std::string& filename, std::string& version_string);
    void PrintUninstallRegInfo(HKEY reg_folder, char* output_string, char* count_string, char* generic_string, char* version_string,
                               unsigned int& install_count);
    bool PrintSdkUninstallRegInfo(HKEY reg_folder, char* output_string, char* count_string, char* generic_string);
    bool PrintExplicitLayersRegInfo(std::vector<std::tuple<std::string, bool, std::string>>& cur_layer_json,
                                    ViaSystem::ViaResults& res);
    bool PrintImplicitLayersRegInfo(std::vector<std::tuple<std::string, bool, std::string>>& cur_layer_json,
                                    std::vector<std::string>& override_paths, ViaResults& res);
    ViaResults FindAndPrintAllExplicitLayersInPath(const std::string& layer_path);
    void PrintDriverEnvVarInfo(const char* var, const std::string& system_path, bool& found_json, bool& found_lib);
    ViaResults PrintLayerEnvVar(const char* var, bool& printed_more_layers);

    OSVERSIONINFOEX _os_version_info;
    SYSTEM_INFO _system_info;
    std::vector<std::tuple<std::string, DEVINST>> _device_ids;
};

#endif  //  VIA_WINDOWS_TARGET
