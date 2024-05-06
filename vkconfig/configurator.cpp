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
    : path(VULKAN_SDK, SUPPORTED_CONFIG_FILES), environment(path), layers(environment), configurations(environment) {}

Configurator::~Configurator() {
    configurations.SaveAllConfigurations(layers.available_layers);

    SurrenderConfiguration(environment);
}

bool Configurator::Init() {
    // Load simple app settings, the additional search paths, and the
    // override app list.
    this->layers.LoadAllInstalledLayers();

    QSettings settings;
    if (settings.value("crashed", QVariant(false)).toBool()) {
        settings.setValue("crashed", false);

        if (Alert::ConfiguratorCrashed() == QMessageBox::No) {
            this->configurations.LoadAllConfigurations(this->layers.available_layers);
        }
    } else {
        this->configurations.LoadAllConfigurations(this->layers.available_layers);
    }

    if (this->configurations.Empty()) {
        this->configurations.ResetDefaultsConfigurations(layers.available_layers);
    } else {
        this->configurations.FirstDefaultsConfigurations(layers.available_layers);
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
        this->configurations.Configure(this->layers.available_layers);
    } else if (configuration == nullptr) {
        QMessageBox alert;
        alert.QDialog::setWindowTitle("Vulkan layers configuration is missing...");
        const std::string text = format("%s couldn't find '%s' layers configuration.", VKCONFIG_NAME, configuration_name.c_str());
        alert.setText(text.c_str());
        alert.setInformativeText("Vulkan Configurator is switching to Layers Controlled by Vulkan Application mode");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        this->environment.SetSelectedConfiguration("");
        this->environment.SetMode(LAYERS_MODE_BY_APPLICATIONS);
        this->configurations.Configure(this->layers.available_layers);
    } else {
        // If the layers paths are differents, we need to reload the layers and the configurations
        const std::vector<std::string> paths = this->environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_GUI);
        if (configuration->user_defined_paths != paths) {
            this->configurations.SaveAllConfigurations(this->layers.available_layers);
            this->environment.SetPerConfigUserDefinedLayersPaths(configuration->user_defined_paths);
            this->layers.LoadAllInstalledLayers();
            this->configurations.LoadAllConfigurations(this->layers.available_layers);
        }

        std::string missing_layer;
        if (HasMissingLayer(configuration->parameters, layers.available_layers, missing_layer)) {
            QMessageBox alert;
            alert.QDialog::setWindowTitle("Vulkan layer missing...");
            alert.setText(format("%s couldn't find '%s' layer required by '%s' configuration:", VKCONFIG_NAME,
                                 missing_layer.c_str(), configuration->key.c_str())
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
        } else {
            this->configurations.Configure(this->layers.available_layers);
        }

        this->environment.SetSelectedConfiguration(configuration_name.c_str());

        this->UpdateDevices();
    }
}

void Configurator::UpdateDevices() {
    QLibrary library(GetVulkanLibrary());

    VkInstance instance = VK_NULL_HANDLE;
    VkResult err = CreateInstance(library, instance, false);

    if (err != VK_SUCCESS) {
        err = CreateInstance(library, instance, true);
        if (err != VK_SUCCESS) {
            return;
        }
    }

    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices =
        (PFN_vkEnumeratePhysicalDevices)library.resolve("vkEnumeratePhysicalDevices");
    assert(vkEnumeratePhysicalDevices);

    PFN_vkDestroyInstance vkDestroyInstance = (PFN_vkDestroyInstance)library.resolve("vkDestroyInstance");
    assert(vkDestroyInstance);

    PFN_vkGetPhysicalDeviceProperties pfnGetPhysicalDeviceProperties =
        (PFN_vkGetPhysicalDeviceProperties)library.resolve("vkGetPhysicalDeviceProperties");
    assert(pfnGetPhysicalDeviceProperties);

    uint32_t gpu_count = 0;
    err = vkEnumeratePhysicalDevices(instance, &gpu_count, NULL);

    std::vector<VkPhysicalDevice> devices;
    devices.resize(gpu_count);

    err = vkEnumeratePhysicalDevices(instance, &gpu_count, &devices[0]);
    assert(!err);

    this->device_names.clear();
    for (std::size_t i = 0, n = devices.size(); i < n; ++i) {
        VkPhysicalDeviceProperties properties;
        pfnGetPhysicalDeviceProperties(devices[i], &properties);

        this->device_names.push_back(properties.deviceName);
    }

    vkDestroyInstance(instance, NULL);
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
        this->layers.LoadAllInstalledLayers();
        this->configurations.ResetDefaultsConfigurations(this->layers.available_layers);

        this->ActivateConfiguration(this->environment.GetSelectedConfiguration());
    } else {
        this->configurations.ReloadDefaultsConfigurations(this->layers.available_layers);
    }
}

std::vector<std::string> Configurator::GetDeviceNames() const { return device_names; }
