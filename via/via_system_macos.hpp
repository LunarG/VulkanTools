/*
 * Copyright (c) 2016-2019 Valve Corporation
 * Copyright (c) 2016-2019 LunarG, Inc.
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
 * Author: Jeremy Kniager <jeremyk@lunarg.com>
 */

#ifdef VIA_MACOS_TARGET

#pragma once

#include "via_system.hpp"

class ViaSystemMacOS : public ViaSystem {
   public:
    ViaSystemMacOS();

    ViaResults PrintRuntimesInFolder(std::string &folder_loc, std::string &object_name, bool print_header = true);

   protected:
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
    virtual bool CheckExpiration(OverrideExpiration expiration);
    virtual std::string GetEnvironmentalVariableValue(const std::string &env_var);
    virtual bool ExpandPathWithEnvVar(std::string &path);

   private:
    bool ReadDriverJson(std::string cur_driver_json, bool &found_lib);
    ViaResults PrintExplicitLayersInFolder(const std::string &id, std::string &folder_loc);
};

#endif  // VIA_MACOS_TARGET
