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

#include "dialog_layers.h"

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

Configurator &Configurator::Get(const std::string &VULKAN_SDK) {
    static Configurator configurator(VULKAN_SDK);
    return configurator;
}

Configurator::Configurator(const std::string &VULKAN_SDK)
    : path(VULKAN_SDK), environment(path), layers(environment), configurations(environment), request_vulkan_status(true) {}

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

    const std::string configuration_name = this->environment.Get(ACTIVE_CONFIGURATION);
    this->environment.Set(ACTIVE_CONFIGURATION, "");  // Force ActivateConfiguration
    this->ActivateConfiguration(configuration_name);

    return true;
}

void Configurator::ActivateConfiguration(const std::string &configuration_name) {
    const std::string name = configuration_name;

    if (this->environment.Get(ACTIVE_CONFIGURATION) == name) return;

    Configuration *configuration = FindByKey(this->configurations.available_configurations, name.c_str());
    if (configuration == nullptr) return;

    this->environment.Set(ACTIVE_CONFIGURATION, configuration->key.c_str());

    // If the layers paths are differents, we need to reload the layers and the configurations
    const std::vector<std::string> paths = this->environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_GUI);
    if (configuration->user_defined_paths != paths) {
        this->environment.SetPerConfigUserDefinedLayersPaths(configuration->user_defined_paths);
        this->layers.LoadAllInstalledLayers();
        this->configurations.LoadAllConfigurations(this->layers.available_layers);
    }
    this->configurations.RefreshConfiguration(this->layers.available_layers);

    std::string missing_layer;
    if (HasMissingLayer(configuration->parameters, layers.available_layers, missing_layer)) {
        QMessageBox alert;
        alert.QDialog::setWindowTitle("Vulkan layer missing...");
        alert.setText(format("%s couldn't find '%s' layer required by '%s' configuration:", VKCONFIG_NAME, missing_layer.c_str(),
                             configuration->key.c_str())
                          .c_str());
        alert.setInformativeText("Do you want to edit the configuration to locate the layer?");
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setDefaultButton(QMessageBox::Yes);
        alert.setIcon(QMessageBox::Warning);
        if (alert.exec() == QMessageBox::Yes) {
            LayersDialog dlg(nullptr, *configuration);
            dlg.exec();

            ActivateConfiguration(configuration->key);
        }
    }

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

        const std::string configuration_name = this->environment.Get(ACTIVE_CONFIGURATION);
        this->environment.Set(ACTIVE_CONFIGURATION, "");  // Force ActivateConfiguration
        this->ActivateConfiguration(configuration_name);

        this->configurations.ResetDefaultsConfigurations(this->layers.available_layers);
    } else {
        this->configurations.ReloadDefaultsConfigurations(this->layers.available_layers);
    }

    this->configurations.RefreshConfiguration(this->layers.available_layers);
}
