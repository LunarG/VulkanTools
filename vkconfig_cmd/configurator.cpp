/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

#include "configurator.h"
#include "vulkan_util.h"

#include "../vkconfig_core/util.h"
#include "../vkconfig_core/path.h"
#include "../vkconfig_core/override.h"
#include "../vkconfig_core/alert.h"

#include <QDir>
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
    : path(VULKAN_SDK), environment(path), layers(environment), configurations(environment) {}

Configurator::~Configurator() {
    configurations.SaveAllConfigurations(layers.selected_layers);

    SurrenderConfiguration(environment);
}

bool Configurator::Init() {
    // Load simple app settings, the additional search paths, and the
    // override app list.
    this->layers.LoadAllInstalledLayers();

    if (this->environment.has_crashed) {
        this->environment.has_crashed = false;

        if (Alert::ConfiguratorCrashed() == QMessageBox::No) {
            this->configurations.LoadAllConfigurations(this->layers.selected_layers);
        }
    } else {
        this->configurations.LoadAllConfigurations(this->layers.selected_layers);
    }

    if (this->configurations.Empty()) {
        this->configurations.ResetDefaultsConfigurations(layers.selected_layers);
    } else {
        this->configurations.FirstDefaultsConfigurations(layers.selected_layers);
    }

    this->ActivateConfiguration(this->environment.GetSelectedConfiguration());

    return true;
}

void Configurator::ActivateConfiguration(const std::string &configuration_name) {
    Configuration *configuration = nullptr;
    if (!configuration_name.empty()) {
        configuration = FindByKey(this->configurations.available_configurations, configuration_name.c_str());
    }

    if (configuration_name.empty()) {
        this->environment.SetSelectedConfiguration("");
        this->configurations.Configure(this->layers.selected_layers);
    } else if (configuration == nullptr) {
        this->environment.SetSelectedConfiguration("");
        this->environment.SetMode(LAYERS_CONTROLLED_BY_APPLICATIONS);
        this->configurations.Configure(this->layers.selected_layers);
    } else {
        // If the layers paths are differents, we need to reload the layers and the configurations
        const std::vector<std::string> paths = this->environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_GUI);
        if (configuration->user_defined_paths != paths) {
            this->configurations.SaveAllConfigurations(this->layers.selected_layers);
            this->environment.SetPerConfigUserDefinedLayersPaths(configuration->user_defined_paths);
            this->layers.LoadAllInstalledLayers();
            this->configurations.LoadAllConfigurations(this->layers.selected_layers);
        }

        std::string missing_layer;
        if (!::HasMissingLayer(configuration->parameters, layers.selected_layers, missing_layer)) {
            this->configurations.Configure(this->layers.selected_layers);
        }

        this->environment.SetSelectedConfiguration(configuration_name.c_str());
    }
}

bool Configurator::SupportLoaderSettings(Version *return_loader_version) const {
    // Check loader version
    const Version version = GetVulkanLoaderVersion();
    assert(version != Version::VERSION_NULL);

    if (return_loader_version) {
        *return_loader_version = version;
    }

    return version >= Version("1.3.261");
}

void Configurator::ResetToDefault(bool hard) {
    if (hard) {
        this->environment.Reset(Environment::CLEAR);
        this->layers.LoadAllInstalledLayers();
        this->configurations.ResetDefaultsConfigurations(this->layers.selected_layers);

        this->ActivateConfiguration(this->environment.GetSelectedConfiguration());
    } else {
        this->configurations.ReloadDefaultsConfigurations(this->layers.selected_layers);
    }
}
