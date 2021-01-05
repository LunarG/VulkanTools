/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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
 * - Lenny Komow <lenny@lunarg.com>
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "configurator.h"
#include "vulkan.h"

#include "dialog_custom_paths.h"

#include "../vkconfig_core/util.h"
#include "../vkconfig_core/path.h"
#include "../vkconfig_core/override.h"

#include <QDir>
#include <QSettings>
#include <QLibrary>
#include <QTextStream>
#include <QMessageBox>
#include <QCheckBox>

#include <cassert>
#include <cstdio>
#include <algorithm>

Configurator &Configurator::Get() {
    static Configurator configurator;
    return configurator;
}

Configurator::Configurator() : environment(path), layers(environment), request_vulkan_status(true) {
    if (0) {
        CopyResourceFiles();
    }
}

Configurator::~Configurator() {
    configurations.SaveAllConfigurations(layers.available_layers, path);

    if (!environment.UsePersistentOverrideMode()) {
        SurrenderLayers(environment);
    }
}

void Configurator::CopyResourceFiles() {
    const std::string dir_save(GetPath(BUILTIN_PATH_VULKAN_LAYER_CONFIG) + "/VK_LAYER_LUNARG_device_simulation");
    CheckPathsExist(dir_save);

    const QFileInfoList &devsim_files = GetJSONFiles(":/resourcefiles/devsim/");
    for (int i = 0, n = devsim_files.size(); i < n; ++i) {
        const std::string filename_exist(dir_save + "/" + devsim_files[i].fileName().toStdString());
        QFile file_exist(filename_exist.c_str());
        const bool opened_exist = file_exist.open(QIODevice::ReadOnly | QIODevice::Text);
        if (opened_exist) {
            file_exist.close();
            continue;
        }

        const std::string filename_load(devsim_files[i].absoluteFilePath().toStdString());
        QFile file_load(filename_load.c_str());
        const bool opened_load = file_load.open(QIODevice::ReadOnly | QIODevice::Text);
        assert(opened_load);
        QString json_text = file_load.readAll();
        file_load.close();

        const std::string filename_save(dir_save + "/" + devsim_files[i].fileName().toStdString());
        QFile file_save(filename_save.c_str());
        const bool opened_save = file_save.open(QIODevice::WriteOnly | QIODevice::Text);
        assert(opened_save);
        file_save.write(json_text.toStdString().c_str(), json_text.size());
        file_save.close();
    }
}

bool Configurator::Init() {
    // Load simple app settings, the additional search paths, and the
    // override app list.
    layers.LoadAllInstalledLayers();

    const bool has_layers = !layers.Empty();

    // If no layers are found, give the user another chance to add some custom paths
    if (!has_layers) {
        QMessageBox alert;
        alert.setWindowTitle("No Vulkan Layers found");
        alert.setText(
            "No Vulkan Layers were found in standard paths or in the SDK path. Vulkan Layers are required in order to use Vulkan "
            "Configurator.");
        alert.setInformativeText("Please select the path where you have your layers located.");
        alert.setIcon(QMessageBox::Warning);
        alert.exec();

        CustomPathsDialog dlg;
        dlg.exec();

        // Give it one more chance... If there are still no layers, bail
        layers.LoadAllInstalledLayers();
    }

    if (!has_layers) {
        QMessageBox alert;
        alert.setWindowTitle(VKCONFIG_NAME);
        alert.setText("Could not initialize Vulkan Configurator.");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        return false;
    }

    configurations.LoadAllConfigurations(layers.available_layers, path, environment);

    if (configurations.Empty()) {
        configurations.ResetDefaultsConfigurations(layers.available_layers, path, environment);
    }

    if (configurations.HasActiveConfiguration(layers.available_layers)) {
        auto active_configuration = configurations.GetActiveConfiguration();

        if (HasMissingLayer(active_configuration->parameters, layers.available_layers)) {
            QSettings settings;
            if (settings.value("VKCONFIG_WARN_MISSING_LAYERS_IGNORE").toBool() == false) {
                QMessageBox alert;
                alert.setWindowTitle("Vulkan Configurator couldn't find some Vulkan layers...");
                alert.setText(format("%s is missing layers", active_configuration->key.c_str()).c_str());
                alert.setInformativeText("Do you want to add a custom path to find the layers?");
                alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                alert.setDefaultButton(QMessageBox::Yes);
                alert.setIcon(QMessageBox::Warning);
                alert.setCheckBox(new QCheckBox("Do not show again."));
                if (alert.exec() == QMessageBox::Yes) {
                    CustomPathsDialog dlg;
                    dlg.exec();

                    layers.LoadAllInstalledLayers();
                }
                if (alert.checkBox()->isChecked()) {
                    settings.setValue("VKCONFIG_WARN_MISSING_LAYERS_IGNORE", true);
                }
            }
        }
    }

    return true;
}

bool Configurator::SupportApplicationList(Version *return_loader_version) const {
    // Check loader version
    const Version version = GetVulkanLoaderVersion();
    assert(version != Version::VERSION_NULL);

    if (return_loader_version) {
        *return_loader_version = version;
    }

    return version >= Version("1.2.141");
}
