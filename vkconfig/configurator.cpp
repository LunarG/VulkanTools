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
 * - Lenny Komow <lenny@lunarg.com>
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "configurator.h"
#include "vulkan_util.h"

#include "dialog_custom_paths.h"

#include "../vkconfig_core/util.h"
#include "../vkconfig_core/path.h"
#include "../vkconfig_core/override.h"
#include "../vkconfig_core/alert.h"

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

Configurator::Configurator() : environment(path), layers(environment), configurations(environment), request_vulkan_status(true) {}

Configurator::~Configurator() {
    configurations.SaveAllConfigurations(layers.available_layers);

    if (!environment.UsePersistentOverrideMode()) {
        SurrenderConfiguration(environment);
    }
}

bool Configurator::Init() {
    // Load simple app settings, the additional search paths, and the
    // override app list.
    layers.LoadAllInstalledLayers();

    const bool has_layers = !layers.Empty();

    // If no layers are found, give the user another chance to add some custom paths
    if (!has_layers) {
        Alert::LayerInitFailed();

        UserDefinedPathsDialog dlg;
        dlg.exec();
    }

    if (!has_layers) {
        Alert::ConfiguratorInitFailed();

        return false;
    }

    QSettings settings;
    if (settings.value("crashed", QVariant(false)).toBool()) {
        settings.setValue("crashed", false);

        if (Alert::ConfiguratorCrashed() == QMessageBox::No) {
            configurations.LoadAllConfigurations(layers.available_layers);
        }
    } else {
        configurations.LoadAllConfigurations(layers.available_layers);
    }

    if (configurations.Empty()) {
        configurations.ResetDefaultsConfigurations(layers.available_layers);
    } else {
        configurations.FirstDefaultsConfigurations(layers.available_layers);
    }

    if (configurations.HasActiveConfiguration(layers.available_layers)) {
        Configuration *active_configuration = configurations.GetActiveConfiguration();
        assert(active_configuration != nullptr);

        if (HasMissingLayer(active_configuration->parameters, layers.available_layers)) {
            if (settings.value("VKCONFIG_WARN_MISSING_LAYERS_IGNORE").toBool() == false) {
                QMessageBox alert;
                alert.QDialog::setWindowTitle(format("%s couldn't find some Vulkan layers...", VKCONFIG_NAME).c_str());
                alert.setText(format("%s is missing layers", active_configuration->key.c_str()).c_str());
                alert.setInformativeText("Do you want to add a path to find the layers?");
                alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                alert.setDefaultButton(QMessageBox::Yes);
                alert.setIcon(QMessageBox::Warning);
                alert.setCheckBox(new QCheckBox("Do not show again."));
                if (alert.exec() == QMessageBox::Yes) {
                    UserDefinedPathsDialog dlg;
                    dlg.exec();
                }
                if (alert.checkBox()->isChecked()) {
                    settings.setValue("VKCONFIG_WARN_MISSING_LAYERS_IGNORE", true);
                }
            }
        }
    }

    /*
    qputenv("VK_REF_ENUM", "value2");
    qputenv("VK_REF_FLAGS", "flag0,flag2");
    qputenv("VK_REF_STRING", "Hello world");
    qputenv("VK_REF_BOOL", "true");
    qputenv("VK_REF_LOAD_FILE", "Pouet.txt");
    qputenv("VK_REF_SAVE_FILE", "Pouet.txt");
    qputenv("VK_REF_SAVE_FOLDER", "~/VulkanSDK/Pouet");
    qputenv("VK_REF_INT", "76");
    qputenv("VK_REF_FLOAT", "0.9999");
    qputenv("VK_REF_FRAMES", "75-76,82");
    qputenv("VK_REF_LIST", "76,stringB");
    */

    return true;
}

void Configurator::ActivateConfiguration(const std::string &configuration_name) {
    const std::string name = configuration_name;

    Configuration *configuration = FindByKey(this->configurations.available_configurations, name.c_str());
    if (configuration == nullptr) return;

    // If the layers paths are differents, we need to reload the layers and the configurations
    if (configuration->user_defined_paths != this->environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_GUI)) {
        this->environment.SetUserDefinedLayersPaths(configuration->user_defined_paths);
        this->layers.LoadAllInstalledLayers();
        this->configurations.LoadAllConfigurations(this->layers.available_layers);
    }
    this->configurations.SetActiveConfiguration(this->layers.available_layers, name);
    this->request_vulkan_status = true;
}

bool Configurator::SupportDifferentLayerVersions(Version *return_loader_version) const {
    // Check loader version
    const Version version = GetVulkanLoaderVersion();
    assert(version != Version::VERSION_NULL);

    if (return_loader_version) {
        *return_loader_version = version;
    }

    return version >= Version("1.3.212");
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

void Configurator::ResetToDefault(bool hard) {
    if (hard) {
        this->environment.Reset(Environment::CLEAR);
        this->environment.ClearCustomLayerPath();
        this->layers.LoadAllInstalledLayers();
        this->configurations.ResetDefaultsConfigurations(this->layers.available_layers);
    } else {
        this->configurations.ReloadDefaultsConfigurations(this->layers.available_layers);
    }
    this->configurations.RefreshConfiguration(this->layers.available_layers);
}
