/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/path.h"

#include <QMessageBox>

struct Alert {
    static QMessageBox::Button StartSingleton();
    static void StartLoaderFailure();
    static void StartLoaderIncompatibleVersions(const Version& system_loader_version, const Version& required_loader_version);
    static void StartPhysicalDeviceFailure();

    static void LayerInvalid(const Path& path, const char* message);
    static QMessageBox::Button LayerImplicit(const char* layer_name);

    static QMessageBox::Button LayerProfiles();

    static void PathInvalid(const Path& path, const char* message);

    static void ConfiguratorRestart();
    static QMessageBox::Button ConfiguratorResetAll();
    static QMessageBox::Button ConfiguratorReloadDefault();
    static QMessageBox::Button ConfiguratorCrashed();

    static void ConfigurationRenamingFailed();
    static void ConfigurationNameEmpty();
    static void ConfigurationNameInvalid();
    static void ConfigurationNameASCII();

    static void LayerProperties(const Layer* layer);

    static void LogFileFailed();

    static void FileNotFound(const char* message);
    static void FileNotJson(const char* message);
};
