/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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
 * - Lenny Komow
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "configurator.h"
#include "util.h"
#include "path.h"
#include "date.h"
#if VKC_ENV == VKC_ENV_WIN32
#include "registry.h"
#endif
#include "vulkan_util.h"
#include "generate_layers_settings_env.h"
#include "generate_layers_settings_file.h"
#include "generate_layers_settings_hpp.h"
#include "generate_layers_settings_html.h"
#include "generate_layers_settings_md.h"

#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QCheckBox>
#include <QJsonArray>
#include <QSettings>
#include <QApplication>
#include <QProcess>
#include <QLibraryInfo>
#include <QSysInfo>
#include <QOperatingSystemVersion>

#include <cassert>
#include <cstdio>
#include <thread>

const char* MAINWINDOW_GEOMETRY = "vkconfig/mainwindow_geometry";
const char* MAINWINDOW_STATE = "vkconfig/mainwindow_state";

ConfiguratorGuard::ConfiguratorGuard(ConfiguratorMode mode) {
    Configurator& configurator = Configurator::Get();
    configurator.Init(mode);
}

ConfiguratorGuard::~ConfiguratorGuard() { Configurator::Release(); }

Configurator& ConfiguratorGuard::Get() { return Configurator::Get(); }

static std::unique_ptr<Configurator> configurator = nullptr;

Configurator& Configurator::Get() {
    if (::configurator == nullptr) {
        ::configurator.reset(new Configurator);
    }

    return *::configurator;
}

void Configurator::Release() { ::configurator.reset(); }

Configurator::Configurator() : mode(init_mode) {
#ifdef SDK_VERSION
    const Version sdk_version(SDK_VERSION);
    if (sdk_version != Version::NONE) {
        this->current_sdk_version = sdk_version;
    }
#endif
}

Configurator::~Configurator() {
    if (this->init_mode != CONFIGURATOR_MODE_CMD) {
        this->Surrender(OVERRIDE_AREA_ALL);
    }

    if (this->reset_hard) {
        return;
    }

    this->has_crashed = false;
    this->Save();
}

bool Configurator::Init(ConfiguratorMode configurator_mode) {
    this->init_mode = configurator_mode;

    const bool result = this->Load();

    if (result && this->has_crashed) {
        switch (this->mode) {
            default:
            case CONFIGURATOR_MODE_NONE: {
            } break;
            case CONFIGURATOR_MODE_GUI: {
                QMessageBox alert;
                alert.setWindowTitle(format("%s crashed during last run...", VKCONFIG_NAME).c_str());
                alert.setText("Do you want to reset to default resolve the issue?");
                alert.setInformativeText("All layers configurations will be lost...");
                alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                alert.setDefaultButton(QMessageBox::No);
                alert.setIcon(QMessageBox::Critical);
                int result = alert.exec();

                if (result == QMessageBox::Yes) {
                    this->Reset(true);
                    return false;
                }
            } break;
            case CONFIGURATOR_MODE_CMD: {
                fprintf(stderr, "%s: [ERROR] crashed during last run...\n", VKCONFIG_SHORT_NAME);
                fprintf(stderr, "\n  (Run \"%s reset --hard\" to reset to default %s if the problem continue)\n",
                        VKCONFIG_SHORT_NAME, VKCONFIG_NAME);
            } break;
        }
    }

    this->has_crashed = true;  // This is set to `false` when saving on exit.
    this->Save();

    if (::RequireLoading(this->mode)) {
        this->UpdateVulkanSystemInfo();
    }

    return true;
}

static QJsonObject CreateDeviceConfigurations(const VulkanPhysicalDeviceInfo& info) {
    QJsonObject json_device;
    json_device.insert("deviceName", info.deviceName.c_str());

    if (!info.driverName.empty()) {
        json_device.insert("driverName", info.driverName.c_str());
    }

    QJsonArray json_device_uuid;
    for (std::size_t i = 0, n = std::size(info.deviceUUID); i < n; ++i) {
        json_device_uuid.append(info.deviceUUID[i]);
    }
    json_device.insert("deviceUUID", json_device_uuid);

    QJsonArray json_driver_uuid;
    for (std::size_t i = 0, n = std::size(info.driverUUID); i < n; ++i) {
        json_driver_uuid.append(info.driverUUID[i]);
    }
    json_device.insert("driverUUID", json_driver_uuid);

    json_device.insert("driverVersion", static_cast<int>(info.driverVersion));

    return json_device;
}

QJsonObject Configurator::CreateJsonSettingObject(const Configurator::LoaderSettings& loader_settings) const {
    QJsonObject json_settings;

    QJsonArray json_layers;
    for (std::size_t j = 0, o = loader_settings.layers.size(); j < o; ++j) {
        const Configurator::LoaderLayerSettings& layer = loader_settings.layers[j];

        QJsonObject json_layer;
        if (layer.builtin == LAYER_BUILTIN_NONE) {
            json_layer.insert("name", layer.key.c_str());
            json_layer.insert("path", layer.path.c_str());
            json_layer.insert("treat_as_implicit_manifest", layer.implicit);
            json_layer.insert("control", ToLowerCase(::GetToken(layer.control)).c_str());
        } else {
            json_layer.insert("control", ::GetToken(layer.builtin));
        }

        json_layers.append(json_layer);
    }

    // This new message is shown in SDK integration test
    if (false && !loader_settings.executable_path.Exists()) {
        this->Log(LOG_ERROR,
                  format("Executable path not found: %s", loader_settings.executable_path.AbsolutePath().c_str()).c_str());
    }

    if (!loader_settings.executable_path.Empty()) {
        QJsonArray json_app_keys;
        json_app_keys.append(loader_settings.executable_path.AbsolutePath().c_str());
        json_settings.insert("app_keys", json_app_keys);
    }

    if (loader_settings.override_layers) {
        json_settings.insert("layers", json_layers);
    }

    if (this->loader_log_enabled) {
        QJsonArray json_stderr_log;
        const std::vector<std::string>& stderr_log = GetLogTokens(this->loader_log_messages_flags);
        for (std::size_t i = 0, n = stderr_log.size(); i < n; ++i) {
            json_stderr_log.append(stderr_log[i].c_str());
        }
        json_settings.insert("stderr_log", json_stderr_log);
    }

    if (this->driver_paths_enabled) {
        QJsonArray json_drivers_paths;

        for (auto it = driver_paths.begin(); it != driver_paths.end(); ++it) {
            if (!it->second) {
                continue;
            }

            QJsonObject json_drivers_path;
            json_drivers_path.insert("path", it->first.AbsolutePath().c_str());
            json_drivers_paths.append(json_drivers_path);
        }
        json_settings.insert("additional_drivers", json_drivers_paths);
    }

    if (this->driver_override_enabled) {
        switch (this->driver_override_mode) {
            case DRIVER_MODE_SINGLE: {
                const VulkanPhysicalDeviceInfo* info = this->GetActivePhysicalDevice();
                if (info != nullptr) {
                    QJsonArray json_devices;
                    json_devices.append(::CreateDeviceConfigurations(*info));
                    json_settings.insert("device_configurations", json_devices);
                }
            } break;
            case DRIVER_MODE_SORTED: {
                QJsonArray json_devices;
                for (std::size_t i = 0, n = this->driver_override_list.size(); i < n; ++i) {
                    const VulkanPhysicalDeviceInfo* info = this->GetPhysicalDevice(this->driver_override_list[i]);
                    if (info != nullptr) {
                        json_devices.append(::CreateDeviceConfigurations(*info));
                    }
                }
                json_settings.insert("device_configurations", json_devices);
            } break;
        }
    }

    return json_settings;
}

static QJsonObject CreateJsonDriverObject(const std::map<Path, bool>& driver_paths) {
    QJsonObject json_settings;

    QJsonArray json_drivers_paths;

    for (auto it = driver_paths.begin(); it != driver_paths.end(); ++it) {
        if (!it->second) {
            continue;
        }

        QJsonObject json_drivers_path;
        json_drivers_path.insert("path", it->first.AbsolutePath().c_str());
        json_drivers_paths.append(json_drivers_path);
    }
    json_settings.insert("additional_drivers", json_drivers_paths);

    return json_settings;
}

QJsonObject Configurator::CreateJsonGlobalObject() const {
    QJsonObject json_settings;

    if (this->loader_log_enabled) {
        QJsonArray json_stderr_log;
        const std::vector<std::string>& stderr_log = GetLogTokens(this->loader_log_messages_flags);
        for (std::size_t i = 0, n = stderr_log.size(); i < n; ++i) {
            json_stderr_log.append(stderr_log[i].c_str());
        }
        json_settings.insert("stderr_log", json_stderr_log);
    }

    if (this->driver_paths_enabled) {
        QJsonArray json_drivers_paths;

        for (auto it = driver_paths.begin(); it != driver_paths.end(); ++it) {
            if (!it->second) {
                continue;
            }

            QJsonObject json_drivers_path;
            json_drivers_path.insert("path", it->first.AbsolutePath().c_str());
            json_drivers_paths.append(json_drivers_path);
        }
        json_settings.insert("additional_drivers", json_drivers_paths);
    }

    if (this->driver_override_enabled) {
        switch (this->driver_override_mode) {
            case DRIVER_MODE_SINGLE: {
                const VulkanPhysicalDeviceInfo* info = this->GetActivePhysicalDevice();
                if (info != nullptr) {
                    QJsonArray json_devices;
                    json_devices.append(::CreateDeviceConfigurations(*info));
                    json_settings.insert("device_configurations", json_devices);
                }
            } break;
            case DRIVER_MODE_SORTED: {
                QJsonArray json_devices;
                for (std::size_t i = 0, n = this->driver_override_list.size(); i < n; ++i) {
                    const VulkanPhysicalDeviceInfo* info = this->GetPhysicalDevice(this->driver_override_list[i]);
                    if (info != nullptr) {
                        json_devices.append(::CreateDeviceConfigurations(*info));
                    }
                }
                json_settings.insert("device_configurations", json_devices);
            } break;
        }
    }

    return json_settings;
}

void Configurator::BuildLoaderSettings(const std::string& configuration_key, const std::string& executable_path,
                                       std::vector<LoaderSettings>& loader_settings_array) const {
    if (configuration_key.empty()) {
        return;
    }

    const Configuration* configuration = this->configurations.FindConfiguration(configuration_key);
    if (configuration == nullptr) {
        return;
    }

    LoaderSettings result;
    result.executable_path = executable_path;

    for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
        LoaderLayerSettings loader_layer_settings;

        const Parameter& parameter = configuration->parameters[i];
        if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
            continue;
        }

        if (parameter.control == LAYER_CONTROL_DISCARD) {
            continue;
        }

        loader_layer_settings.key = parameter.key;
        loader_layer_settings.builtin = parameter.builtin;
        loader_layer_settings.control = parameter.control;

        if (parameter.builtin == LAYER_BUILTIN_NONE) {
            const Layer* layer = this->layers.FindFromManifest(parameter.manifest);
            if (layer == nullptr) {
                continue;
            }

            // Extract just the path
            loader_layer_settings.key = parameter.key;
            if (layer != nullptr) {
                loader_layer_settings.path = layer->manifest_path.AbsolutePath();
            }
            loader_layer_settings.control = parameter.control;
            loader_layer_settings.implicit = layer != nullptr ? layer->type == LAYER_TYPE_IMPLICIT : false;
        }

        result.layers.push_back(loader_layer_settings);
    }

    loader_settings_array.push_back(result);
}

bool Configurator::Generate(GenerateSettingsMode mode, const Path& output_path) {
    bool result = false;

    switch (mode) {
        default: {
            assert(0);
        } break;
        case GENERATE_SETTINGS_HTML: {
            result = ::GenerateSettingsHTML(*this, nullptr, output_path);
        } break;
        case GENERATE_SETTINGS_MARKDOWN: {
            result = ::GenerateSettingsMarkdown(*this, nullptr, output_path);
        } break;
        case GENERATE_SETTINGS_TXT: {
            result = ::GenerateSettingsTXT(*this, OVERRIDE_AREA_LAYERS_SETTINGS_BIT, output_path);
        } break;
        case GENERATE_SETTINGS_BASH: {
            result = ::GenerateSettingsEnv(*this, EXPORT_ENV_BASH, output_path);
        } break;
        case GENERATE_SETTINGS_CMD: {
            result = ::GenerateSettingsEnv(*this, EXPORT_ENV_CMD, output_path);
        } break;
        case GENERATE_SETTINGS_HPP: {
            result = ::GenerateSettingsCode(*this, output_path);
        } break;
    }

    if (result)
        this->Log(LOG_INFO, format("File written to '%s'\n", output_path.AbsolutePath().c_str()));
    else {
        this->Log(LOG_ERROR, format("Could not write %s\n", output_path.AbsolutePath().c_str()));
    }

    return result;
}

// Create and write vk_loader_settings.json file
bool Configurator::WriteLoaderSettings(OverrideArea override_area, const Path& loader_settings_path) {
    assert(!loader_settings_path.Empty());

    if (override_area & OVERRIDE_AREA_LOADER_SETTINGS_BIT || override_area == OVERRIDE_DRIVER) {
        std::vector<LoaderSettings> loader_settings_array;

        std::string configuration = this->selected_global_configuration;

        switch (this->executable_scope) {
            case EXECUTABLE_ANY:
                this->BuildLoaderSettings(configuration, "", loader_settings_array);
                break;
            case EXECUTABLE_ALL:
            case EXECUTABLE_PER: {
                const std::vector<Executable>& executables = this->executables.GetExecutables();
                for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
                    if (!executables[i].enabled) {
                        continue;
                    }

                    if (this->executable_scope == EXECUTABLE_PER) {
                        configuration = executables[i].configuration;
                    }

                    this->BuildLoaderSettings(configuration, executables[i].path.AbsolutePath(), loader_settings_array);
                }
                break;
            }
            default:
            case EXECUTABLE_NONE:
                break;
        }

        QJsonObject root;
        root.insert("file_format_version", "1.0.0");
        if (override_area == OVERRIDE_DRIVER) {
            if (this->driver_paths_enabled) {
                root.insert("settings", CreateJsonDriverObject(this->driver_paths));
            }
        } else {
            QJsonArray json_settings_array;

            if (this->executable_scope != EXECUTABLE_ANY) {
                json_settings_array.append(this->CreateJsonGlobalObject());
            }

            for (std::size_t i = 0, n = loader_settings_array.size(); i < n; ++i) {
                json_settings_array.append(this->CreateJsonSettingObject(loader_settings_array[i]));
            }

            root.insert("settings_array", json_settings_array);
        }
        QJsonDocument doc(root);

        QFile json_file(loader_settings_path.AbsolutePath().c_str());
        const bool result_layers_file = json_file.open(QIODevice::WriteOnly | QIODevice::Text);
        assert(result_layers_file);
        json_file.write(doc.toJson());
        json_file.close();

        return result_layers_file;

        return true;
    } else {
        return true;
    }
}

bool Configurator::Override(OverrideArea override_area) {
    if (this->mode == CONFIGURATOR_MODE_DRY) {
        return true;
    }

    const Path& loader_settings_path = ::Path(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Path(Path::LAYERS_SETTINGS);

    // Clean up
    this->Surrender(override_area);

    // vk_loader_settings.json
    bool result_loader_settings = this->WriteLoaderSettings(override_area, loader_settings_path);

    // vk_layer_settings.txt
    bool result_layers_settings = ::GenerateSettingsTXT(*this, override_area, layers_settings_path);

    // On Windows only, we need to write these values to the registry
#if VKC_ENV == VKC_ENV_WIN32
    AppendRegistryEntriesForLayers(loader_settings_path.AbsolutePath().c_str(), layers_settings_path.AbsolutePath().c_str());
#endif

    return result_loader_settings && result_layers_settings;
}

bool Configurator::Surrender(OverrideArea override_area) {
    if (this->mode == CONFIGURATOR_MODE_DRY) {
        return true;
    }

    const Path& loader_settings_path = ::Path(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Path(Path::LAYERS_SETTINGS);

    bool result_loader_settings = true;
    if (override_area & OVERRIDE_AREA_LOADER_SETTINGS_BIT) {
        result_loader_settings = loader_settings_path.Remove();
    }

    bool result_layers_settings = true;
    if (override_area & OVERRIDE_AREA_LAYERS_SETTINGS_BIT) {
        bool global_removed = layers_settings_path.Remove();
        if (this->executable_scope == EXECUTABLE_ANY) {
            result_layers_settings = global_removed;
        }

        if (::EnabledExecutables(this->executable_scope)) {
            const std::vector<Executable>& executables = this->executables.GetExecutables();
            for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
                Path path(executables[i].GetActiveOptions()->working_folder.RelativePath() + "/vk_layer_settings.txt");
                if (path.Exists()) {
                    bool local_removed = path.Remove();

                    result_layers_settings = result_layers_settings && local_removed;
                }
            }
        }
    }

#if VKC_ENV == VKC_ENV_WIN32
    RemoveRegistryEntriesForLayers();
#endif

    return result_loader_settings && result_layers_settings;
}

bool Configurator::HasOverride() const {
    const Path& loader_settings_path = ::Path(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Path(Path::LAYERS_SETTINGS);

    return loader_settings_path.Exists() || layers_settings_path.Exists();
}

void Configurator::Reset(bool hard) {
    this->Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    const Path& vkconfig_init_path = ::Path(Path::INIT);
    vkconfig_init_path.Remove();

    QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
    const QStringList& keys = settings.allKeys();
    for (int i = 0, n = keys.size(); i < n; ++i) {
        settings.remove(keys[i]);
    }

    if (hard) {
        this->configurations.RemoveConfigurationFiles();
    }

    this->use_system_tray = false;  // This prevents Vulkan Configurator to abort the restart
    this->reset_hard = true;
}

void Configurator::SetActiveConfigurationName(const std::string& configuration_name) {
    if (this->executable_scope == EXECUTABLE_PER) {
        Executable* executable = this->executables.GetActiveExecutable();
        if (executable != nullptr) {
            executable->configuration = configuration_name;
        }
    } else {
        this->selected_global_configuration = configuration_name;
    }
}

int Configurator::GetPhysicalDeviceIndex(const DeviceInfo& device_info) const {
    if (device_info.deviceName.empty()) {
        return -1;
    }

    const auto& devices = this->vulkan_system_info.physicalDevices;
    int count_multiple_version = 0;
    for (std::size_t i = 0, n = devices.size(); i < n; ++i) {
        if (devices[i].deviceName == device_info.deviceName) {
            ++count_multiple_version;
        }
    }

    // We has multiple version, we deal with user removing a driver version of updating drivers
    const bool has_multiple_version = count_multiple_version > 1;

    for (std::size_t i = 0, n = devices.size(); i < n; ++i) {
        if (devices[i].deviceName != device_info.deviceName) {
            continue;
        }

        if (has_multiple_version) {
            if (GetDeviceInfo(devices[i]) != device_info) {
                continue;
            }
        }

        return static_cast<int>(i);
    }

    return -1;
}

bool Configurator::Found(const DeviceInfo& device_info) const {
    for (std::size_t i = 0, n = this->driver_override_list.size(); i < n; ++i) {
        if (this->driver_override_list[i] == device_info) {
            return true;
        }
    }

    return false;
}

int Configurator::GetActivePhysicalDeviceIndex() const { return this->GetPhysicalDeviceIndex(this->driver_override_info); }

const VulkanPhysicalDeviceInfo* Configurator::GetPhysicalDevice(const DeviceInfo& device_info) const {
    int index = this->GetPhysicalDeviceIndex(device_info);
    if (index >= 0 && index < static_cast<int>(this->vulkan_system_info.physicalDevices.size())) {
        return &this->vulkan_system_info.physicalDevices[index];
    } else {
        return nullptr;
    }
}

const VulkanPhysicalDeviceInfo* Configurator::GetActivePhysicalDevice() const {
    return this->GetPhysicalDevice(this->driver_override_info);
}

void Configurator::UpdateVulkanSystemInfo() {
    if (this->vulkan_system_info_hardware.physicalDevices.empty()) {
        this->Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
        this->vulkan_system_info_hardware = ::BuildVulkanSystemInfo();
    }

    this->Override(OVERRIDE_DRIVER);  // Allow loading additional drivers

    this->vulkan_system_info = ::BuildVulkanSystemInfo();

    // Add new driver
    for (std::size_t i = 0, n = this->vulkan_system_info.physicalDevices.size(); i < n; ++i) {
        const DeviceInfo& device_info = ::GetDeviceInfo(this->vulkan_system_info.physicalDevices[i]);
        if (!this->Found(device_info)) {
            this->driver_override_list.push_back(device_info);
        }
    }

    // Remove missing driver
    std::vector<DeviceInfo> new_driver_override_list;
    for (std::size_t i = 0, n = this->driver_override_list.size(); i < n; ++i) {
        const VulkanPhysicalDeviceInfo* info = this->GetPhysicalDevice(this->driver_override_list[i]);
        if (info == nullptr) {
            continue;
        }

        new_driver_override_list.push_back(::GetDeviceInfo(*info));
    }
    std::swap(this->driver_override_list, new_driver_override_list);

    // Remove duplicated value (may happen when removing a driver version)
    this->driver_override_list.erase(remove_duplicates(this->driver_override_list.begin(), this->driver_override_list.end()),
                                     this->driver_override_list.end());

    if (this->GetPhysicalDeviceIndex(this->driver_override_info) == -1) {
        if (!this->vulkan_system_info.physicalDevices.empty()) {
            this->driver_override_info = GetDeviceInfo(this->vulkan_system_info.physicalDevices[0]);
        }
    }

    this->Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
}

void Configurator::UpdateConfigurations() {
    this->configurations.UpdateConfigurations(this->layers);
    return;
}

Configuration* Configurator::GetActiveConfiguration() {
    if (this->executable_scope == EXECUTABLE_PER) {
        const Executable* executable = this->executables.GetActiveExecutable();
        if (executable != nullptr) {
            return this->configurations.FindConfiguration(executable->configuration);
        } else {
            return nullptr;
        }
    } else {
        return this->configurations.FindConfiguration(this->selected_global_configuration);
    }
}

const Configuration* Configurator::GetActiveConfiguration() const {
    return const_cast<Configurator*>(this)->GetActiveConfiguration();
}

Parameter* Configurator::GetActiveParameter() {
    Configuration* configuration = this->GetActiveConfiguration();
    if (configuration != nullptr) {
        return configuration->GetActiveParameter();
    } else {
        return nullptr;
    }
}

const Parameter* Configurator::GetActiveParameter() const { return const_cast<Configurator*>(this)->GetActiveParameter(); }

bool Configurator::HasActiveParameter() const { return this->GetActiveParameter() != nullptr; }

Executable* Configurator::GetActiveExecutable() { return this->executables.GetActiveExecutable(); }

const Executable* Configurator::GetActiveExecutable() const { return this->executables.GetActiveExecutable(); }

void Configurator::Log(LogType type, const std::string& message) const {
    if (this->mode != CONFIGURATOR_MODE_CMD) {
        return;
    }

    fprintf(stderr, "vkconfig: [%s] %s\n", ToUpperCase(GetToken(type)).c_str(), message.c_str());
}

std::string Configurator::LogLayers(const std::string& configuration_key) const {
    const Configuration* configuration = this->configurations.FindConfiguration(configuration_key);
    assert(configuration != nullptr);

    std::string log = format("'%s' Layers Configuration:\n", configuration->key.c_str());

    {
        log += " - Vulkan Layers Selection and Execution Order:\n";
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            const Parameter& parameter = configuration->parameters[i];

            if (parameter.builtin == LAYER_BUILTIN_UNORDERED) {
                log += format("   * %s: %s\n", parameter.key.c_str(), ::GetToken(parameter.control));
            } else {
                const Layer* layer = this->layers.Find(parameter.key, parameter.api_version);
                if (layer == nullptr) {
                    log += format("   * %s - Missing: %s\n", parameter.key.c_str(), ::GetToken(parameter.control));
                } else {
                    log += format("   * %s - %s (%s layer): %s\n", parameter.key.c_str(), layer->api_version.str().c_str(),
                                  ::GetToken(layer->type), ::GetToken(parameter.control));
                }
                log += format("     Layer manifest path: %s\n", parameter.manifest.AbsolutePath().c_str());
                log += format("     Layer settings export: %s\n", parameter.override_settings ? "enabled" : "disabled");
            }
        }
    }

    log += "\n";

    return log;
}

std::string Configurator::LogLoaderMessage() const {
    std::string log;

    if (this->loader_log_enabled) {
        log += format(" - Vulkan Loader Messages: %s\n", GetLogString(this->loader_log_messages_flags).c_str());
    } else {
        log += " - Vulkan Loader Messages: disabled\n";
    }

    log += "\n";

    return log;
}

std::string Configurator::Log() const {
    std::string log;

#ifdef _DEBUG
    std::string build = "Debug";
#else
    std::string build = "Release";
#endif

    log += format("%s %s - %s:\n", VKCONFIG_NAME, Version::VKCONFIG.str().c_str(), GetBuildDate().c_str());
    log += format(" - Vulkan API version: %s\n", Version::VKHEADER.str().c_str());
    log += format(" - Build: %s %s\n", GetLabel(VKC_PLATFORM), build.c_str());
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
    log += format(" - Qt version: %s %s\n", QLibraryInfo::version().toString().toStdString().c_str(),
                  QLibraryInfo::isDebugBuild() ? "Debug" : "Release");
#else
    log += format(" - Qt version: %s %s-%s\n", QLibraryInfo::version().toString().toStdString().c_str(),
                  QLibraryInfo::isSharedBuild() ? "Shared" : "Static", QLibraryInfo::isDebugBuild() ? "Debug" : "Release");
#endif
#ifdef SDK_VERSION
    log += format(" - ${SDK_VERSION}: %s\n", SDK_VERSION);
#else
    log += "${SDK_VERSION}: unset\n";
#endif

    if (Path(Path::SDK).Empty()) {
        log += " - ${VULKAN_SDK}: unset\n";
    } else {
        log += format(" - ${VULKAN_SDK}: %s\n", Path(Path::SDK).AbsolutePath().c_str());
    }
    log += "\n";

    log += "System Information:\n";
#if VKC_PLATFORM == PLATFORM_LINUX || QT_VERSION < QT_VERSION_CHECK(6, 1, 0)
    log += format(" - %s\n", QSysInfo::prettyProductName().toStdString().c_str());
#else
    const QOperatingSystemVersion& current = QOperatingSystemVersion::current();
    log += format(" - %s %s\n", current.name().toStdString().c_str(), current.version().toString().toStdString().c_str());
#endif

    if (qEnvironmentVariableIsSet("XDG_SESSION_TYPE")) {
        log += format(" - ${XDG_SESSION_TYPE}: %s\n", qgetenv("XDG_SESSION_TYPE").toStdString().c_str());
    }
    if (qEnvironmentVariableIsSet("XDG_CURRENT_DESKTOP")) {
        log += format(" - ${XDG_CURRENT_DESKTOP}: %s\n", qgetenv("XDG_CURRENT_DESKTOP").toStdString().c_str());
    }

    log += format(" - CPU architecture: %s\n", QSysInfo::currentCpuArchitecture().toStdString().c_str());
    log += format(" - Logical CPU core count: %d\n", std::thread::hardware_concurrency());

    log += "\n";

    log += format("%s Settings:\n", VKCONFIG_NAME);
    log += format(" - Vulkan Layers configuration scope: %s\n", ::GetLabel(this->GetExecutableScope()));
    log += format("   * Loader settings: %s\n", Path(Path::LOADER_SETTINGS).AbsolutePath().c_str());

    if (this->GetExecutableScope() == EXECUTABLE_ANY) {
        log += format("   * Layers settings: %s\n", Path(Path::LAYERS_SETTINGS).AbsolutePath().c_str());
    }

    if (this->GetExecutableScope() == EXECUTABLE_ANY || this->GetExecutableScope() == EXECUTABLE_ALL) {
        const Configuration* configuration = this->GetActiveConfiguration();
        if (configuration != nullptr) {
            log += format(" - Active Vulkan Loader Configuration: '%s'\n", configuration->key.c_str());
        } else {
            log += " - No Active Vulkan Loader Configuration\n";
        }
    }
    /*
    if (this->GetExecutableScope() == EXECUTABLE_ALL) {
        log += " - Listed Executables:\n";
        const std::vector<Executable>& executables = this->executables.GetExecutables();
        for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
            log += format("   * %s: %s\n", executables[i].path.AbsolutePath().c_str(),
                          executables[i].enabled ? "enabled" : "disabled");
        }
    }
    */

    if (this->GetExecutableScope() == EXECUTABLE_PER || this->GetExecutableScope() == EXECUTABLE_ALL) {
        log += " - Listed Executables:\n";
        const std::vector<Executable>& executables = this->executables.GetExecutables();
        for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
            if (this->GetExecutableScope() == EXECUTABLE_PER) {
                const Configuration* configuration = this->configurations.FindConfiguration(executables[i].configuration);

                log += format("   * %s: '%s'\n", executables[i].path.AbsolutePath().c_str(),
                              executables[i].enabled && configuration != nullptr ? executables[i].configuration.c_str() : "None");
            } else {
                log += format("   * %s: %s\n", executables[i].path.AbsolutePath().c_str(),
                              executables[i].enabled ? "enabled" : "disabled");
            }

            if (!executables[i].enabled) {
                continue;
            }

            log += format("     Vulkan Loader settings file: %s\n",
                          executables[i].GetLocalLayersSettingsPath().AbsolutePath().c_str());
        }
    }

    log += format(" - Use system tray: %s\n", this->use_system_tray ? "true" : "false");
    log += format(" - Use layer developer mode: %s\n", this->use_layer_debug_mode ? "true" : "false");
    log += format(" - ${VULKAN_BIN}: %s\n", ::Path(Path::BIN).AbsolutePath().c_str());
    log += format(" - ${VULKAN_PROFILES}: %s\n", ::Path(Path::PROFILES).AbsolutePath().c_str());
    log += format(" - ${VULKAN_HOME}: %s\n", ::Path(Path::HOME).AbsolutePath().c_str());
    log += format(" - ${VULKAN_DOWNLOAD}: %s\n", ::Path(Path::DOWNLOAD).AbsolutePath().c_str());
    log += "\n";

    if (this->GetExecutableScope() == EXECUTABLE_ANY || this->GetExecutableScope() == EXECUTABLE_ALL) {
        const Configuration* configuration = this->GetActiveConfiguration();

        if (configuration != nullptr) {
            log += this->LogLayers(configuration->key.c_str());
        }
    } else if (this->GetExecutableScope() == EXECUTABLE_PER) {
        const std::vector<Executable>& executables = this->executables.GetExecutables();

        for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
            if (!executables[i].enabled) {
                continue;
            }
            if (executables[i].configuration.empty()) {
                continue;
            }
            const Configuration* configuration = this->configurations.FindConfiguration(executables[i].configuration);
            if (configuration == nullptr) {
                continue;
            }
            log += this->LogLayers(configuration->key.c_str());
        }
    }

    log += this->LogLoaderMessage();

    log += "Vulkan Physical Devices:\n";
    for (std::size_t i = 0, n = this->vulkan_system_info.physicalDevices.size(); i < n; ++i) {
        const VulkanPhysicalDeviceInfo& info = this->vulkan_system_info.physicalDevices[i];
        log += format(" - %s with Vulkan %s (%s)\n", info.deviceName.c_str(), info.apiVersion.str().c_str(),
                      GetLabel(info.deviceType));

        if (info.vendorID == 0x10DE) {
            log += format("   * Driver: %s %s\n", GetLabel(info.vendorID).c_str(), FormatNvidia(info.driverVersion).c_str());
        } else if ((info.vendorID == 0x8086) && (VKC_PLATFORM & PLATFORM_WINDOWS_BIT)) {
            log += format("   * Driver: %s %s\n", GetLabel(info.vendorID).c_str(), FormatIntelWindows(info.driverVersion).c_str());
        } else {
            log += format("   * Driver: %s %s\n", GetLabel(info.vendorID).c_str(), Version(info.driverVersion).str().c_str());
        }
        log += format("   * deviceUUID: %s\n", ::GetUUIDString(info.deviceUUID).c_str());
        log += format("   * driverUUID: %s\n", ::GetUUIDString(info.driverUUID).c_str());
        log += format("   * deviceLUID: %s\n", ::GetLUIDString(info.deviceLUID).c_str());
    }
    log += "\n";

    log += "Vulkan Loader Settings:\n";
    if (this->vulkan_system_info.loaderVersion != Version::NONE) {
        log += format(" - API version: %s\n", this->vulkan_system_info.loaderVersion.str().c_str());
    } else {
        log += format(" - Couldn't Find a Vulkan Loader\n");
    }

    log += " - Vulkan Loader found Vulkan layers:\n";
    for (std::size_t i = 0, n = this->vulkan_system_info.instanceLayerProperties.size(); i < n; ++i) {
        log += format("   * %s\n", this->vulkan_system_info.instanceLayerProperties[i].layerName);
    }
    log += " - Vulkan implementation or by implicitly enabled layers extensions:\n";
    for (std::size_t i = 0, n = this->vulkan_system_info.instanceExtensionPropertie.size(); i < n; ++i) {
        log += format("   * %s\n", this->vulkan_system_info.instanceExtensionPropertie[i].extensionName);
    }

    if (qEnvironmentVariableIsSet("VK_LOADER_DISABLE_DYNAMIC_LIBRARY_UNLOADING")) {
        log += format(" - ${VK_LOADER_DISABLE_DYNAMIC_LIBRARY_UNLOADING}: %s\n",
                      qgetenv("VK_LOADER_DISABLE_DYNAMIC_LIBRARY_UNLOADING").toStdString().c_str());
    } else {
        log += " - ${VK_LOADER_DISABLE_DYNAMIC_LIBRARY_UNLOADING}: unset\n";
    }

    log += " - Vulkan Loader Layers environment variables:\n";
    if (qEnvironmentVariableIsSet("VK_LOADER_DEBUG")) {
        log += format("   * ${VK_LOADER_DEBUG}: %s\n", qgetenv("VK_LOADER_DEBUG").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_DEBUG}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_LAYERS_ENABLE")) {
        log += format("   * ${VK_LOADER_LAYERS_ENABLE}: %s\n", qgetenv("VK_LOADER_LAYERS_ENABLE").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_LAYERS_ENABLE}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_LAYERS_DISABLE")) {
        log += format("   * ${VK_LOADER_LAYERS_DISABLE}: %s\n", qgetenv("VK_LOADER_LAYERS_DISABLE").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_LAYERS_DISABLE}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_LAYERS_ALLOW")) {
        log += format("   * ${VK_LOADER_LAYERS_ALLOW}: %s\n", qgetenv("VK_LOADER_LAYERS_ALLOW").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_LAYERS_ALLOW}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_INSTANCE_LAYERS")) {
        log += format("   * ${VK_INSTANCE_LAYERS}: %s\n", qgetenv("VK_INSTANCE_LAYERS").toStdString().c_str());
    } else {
        log += "   * ${VK_INSTANCE_LAYERS}: unset\n";
    }
    log += " - Vulkan Loader Drivers environment variables:\n";
    if (qEnvironmentVariableIsSet("VK_DRIVER_FILES")) {
        log += format("   * ${VK_DRIVER_FILES}: %s\n", qgetenv("VK_DRIVER_FILES").toStdString().c_str());
    } else {
        log += "   * ${VK_DRIVER_FILES}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_ADD_DRIVER_FILES")) {
        log += format("   * ${VK_ADD_DRIVER_FILES}: %s\n", qgetenv("VK_ADD_DRIVER_FILES").toStdString().c_str());
    } else {
        log += "   * ${VK_ADD_DRIVER_FILES}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_DRIVERS_SELECT")) {
        log += format("   * ${VK_LOADER_DRIVERS_SELECT}: %s\n", qgetenv("VK_LOADER_DRIVERS_SELECT").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_DRIVERS_SELECT}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_DRIVERS_DISABLE")) {
        log += format("   * ${VK_LOADER_DRIVERS_DISABLE}: %s\n", qgetenv("VK_LOADER_DRIVERS_DISABLE").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_DRIVERS_DISABLE}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_DISABLE_INST_EXT_FILTER")) {
        log += format("   * ${VK_LOADER_DISABLE_INST_EXT_FILTER}: %s\n",
                      qgetenv("VK_LOADER_DISABLE_INST_EXT_FILTER").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_DISABLE_INST_EXT_FILTER}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_ICD_FILENAMES")) {
        log += format("   * ${VK_ICD_FILENAMES}: %s\n", qgetenv("VK_ICD_FILENAMES").toStdString().c_str());
    }
    log += "\n";

    return log;
}

bool Configurator::Load() {
    const Path& vkconfig_init_path = ::Path(Path::INIT);

    QFile file(vkconfig_init_path.AbsolutePath().c_str());
    const bool has_init_file = file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (has_init_file && ::RequireLoading(this->mode)) {
        QString init_data = file.readAll();
        file.close();

        QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
        this->window_geometry = settings.value(MAINWINDOW_GEOMETRY).toByteArray();
        this->window_state = settings.value(MAINWINDOW_STATE).toByteArray();

        const QJsonDocument& json_doc = QJsonDocument::fromJson(init_data.toLocal8Bit());
        const QJsonObject& json_root_object = json_doc.object();

        const Version file_format_version = Version(json_root_object.value("file_format_version").toString().toStdString());
        if (file_format_version > Version::VKCONFIG) {
            switch (this->mode) {
                default:
                case CONFIGURATOR_MODE_NONE:
                    break;
                case CONFIGURATOR_MODE_GUI: {
                    QMessageBox alert;
                    alert.setWindowTitle(format("Launching an older version of %s...", VKCONFIG_NAME).c_str());
                    alert.setText(format("Running a Vulkan Configurator %s but a newer %s version was previously launched.",
                                         Version::VKCONFIG.str().c_str(), file_format_version.str().c_str())
                                      .c_str());
                    alert.setInformativeText("Do you want to continue? This may cause crashes...");
                    alert.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
                    alert.setDefaultButton(QMessageBox::Cancel);
                    alert.setIcon(QMessageBox::Critical);
                    if (alert.exec() == QMessageBox::Cancel) {
                        return false;  // Vulkan Configurator is reset to default
                    }
                } break;
                case CONFIGURATOR_MODE_CMD: {
                    fprintf(stderr, "vkconfig: [WARNING] Launching an older version %s of %s...\n", Version::VKCONFIG.str().c_str(),
                            VKCONFIG_NAME);
                    fprintf(stderr, "\n  (Run \"%s reset --hard\" to reset to default %s if the problem continue)\n",
                            VKCONFIG_SHORT_NAME, VKCONFIG_NAME);
                } break;
            }
        }

        // interface json object
        const QJsonObject& json_interface_object = json_root_object.value("interface").toObject();
        this->has_crashed = json_interface_object.value("has_crashed").toBool();

        const QJsonArray& json_hide_message_boxes_array = json_interface_object.value("hide_message_boxes").toArray();
        this->hide_message_boxes_flags = 0;
        for (int i = 0, n = json_hide_message_boxes_array.size(); i < n; ++i) {
            const std::string& token = json_hide_message_boxes_array[i].toString().toStdString();
            this->hide_message_boxes_flags |= GetHideMessageBit(token.c_str());
        }

        const QJsonObject& json_hide_layer_message_boxes_object =
            json_interface_object.value("hide_layer_message_boxes").toObject();
        this->ignored_messages.clear();
        const QStringList json_hide_layer_message_boxes_keys = json_hide_layer_message_boxes_object.keys();
        for (int i = 0, n = json_hide_layer_message_boxes_keys.size(); i < n; ++i) {
            const std::string& key = json_hide_layer_message_boxes_keys[i].toStdString();
            const int version = json_hide_layer_message_boxes_object.value(key.c_str()).toInt();
            this->ignored_messages.insert(std::make_pair(key, version));
        }

        this->active_tab = GetTabType(json_interface_object.value("active_tab").toString().toStdString().c_str());

        // TAB_CONFIGURATIONS
        if (json_interface_object.value(GetToken(TAB_CONFIGURATIONS)) != QJsonValue::Undefined) {
            const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_CONFIGURATIONS)).toObject();
            this->advanced = json_object.value("advanced").toBool();
            this->executable_scope = ::GetExecutableScope(json_object.value("executable_scope").toString().toStdString().c_str());
            this->selected_global_configuration = json_object.value("selected_global_configuration").toString().toStdString();
        }

        // TAB_LAYERS
        if (json_interface_object.value(GetToken(TAB_LAYERS)) != QJsonValue::Undefined) {
            const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_LAYERS)).toObject();
            (void)json_object;
        }

        // TAB_DRIVERS
        if (json_interface_object.value(::GetToken(TAB_DRIVERS)) != QJsonValue::Undefined) {
            const QJsonObject& json_object = json_interface_object.value(::GetToken(TAB_DRIVERS)).toObject();

            if (json_object.value("driver_override_enabled") != QJsonValue::Undefined) {
                this->driver_override_enabled = json_object.value("driver_override_enabled").toBool();
            }
            if (json_object.value("driver_override_mode") != QJsonValue::Undefined) {
                this->driver_override_mode =
                    ::GetDriverMode(json_object.value("driver_override_mode").toString().toStdString().c_str());
            }

            if (json_object.value("driver_override_info") != QJsonValue::Undefined) {
                const QJsonObject& json_object_info = json_object.value("driver_override_info").toObject();

                this->driver_override_info.deviceName = json_object_info.value("deviceName").toString().toStdString();
                const QJsonArray& json_device_uuid = json_object_info.value("deviceUUID").toArray();
                for (std::size_t i = 0, n = std::size(this->driver_override_info.deviceUUID); i < n; ++i) {
                    this->driver_override_info.deviceUUID[i] = json_device_uuid[i].toInt();
                }
                this->driver_override_info.driverVersion =
                    static_cast<std::uint32_t>(json_object_info.value("driverVersion").toInt());
            }

            this->driver_override_list.clear();
            const QJsonArray& json_driver_override_list_array = json_object.value("driver_override_list").toArray();
            if (json_object.value("driver_override_list") != QJsonValue::Undefined) {
                for (int i = 0, n = json_driver_override_list_array.size(); i < n; ++i) {
                    const QJsonObject& json_object = json_driver_override_list_array[i].toObject();
                    const QJsonArray& json_device_uuid = json_object.value("deviceUUID").toArray();

                    DeviceInfo info;
                    info.deviceName = json_object.value("deviceName").toString().toStdString();
                    for (std::size_t i = 0, n = std::size(this->driver_override_info.deviceUUID); i < n; ++i) {
                        info.deviceUUID[i] = json_device_uuid[i].toInt();
                    }
                    info.driverVersion = static_cast<std::uint32_t>(json_object.value("driverVersion").toInt());
                    this->driver_override_list.push_back(info);
                }
            }

            if (json_object.value("last_driver_path") != QJsonValue::Undefined) {
                this->last_driver_path = json_object.value("last_driver_path").toString().toStdString();
            }
            if (json_object.value("driver_paths") != QJsonValue::Undefined) {
                const QJsonObject& json_object_paths = json_object.value("driver_paths").toObject();
                const QStringList& keys = json_object_paths.keys();
                for (int i = 0, n = keys.size(); i < n; ++i) {
                    const std::string& key = keys[i].toStdString();
                    const QJsonObject& json_object_path = json_object_paths.value(key.c_str()).toObject();
                    this->driver_paths.insert(std::make_pair(key, json_object_path.value("enabled").toBool()));
                }
            }
            if (json_object.value("driver_paths_enabled") != QJsonValue::Undefined) {
                this->driver_paths_enabled = json_object.value("driver_paths_enabled").toBool();
            }
        }

        // TAB_APPLICATIONS
        if (json_interface_object.value(GetToken(TAB_APPLICATIONS)) != QJsonValue::Undefined) {
            const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_APPLICATIONS)).toObject();
            (void)json_object;
        }

        // TAB_DIAGNOSTICS
        if (json_interface_object.value(GetToken(TAB_DIAGNOSTIC)) != QJsonValue::Undefined) {
            const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_DIAGNOSTIC)).toObject();
            if (json_object.value("loader_message_types") != QJsonValue::Undefined) {
                const std::vector<std::string>& loader_messsage_types = ReadStringArray(json_object, "loader_message_types");
                this->loader_log_messages_flags = GetLogFlags(loader_messsage_types);
            }
            if (json_object.value("loader_log_enabled") != QJsonValue::Undefined) {
                this->loader_log_enabled = json_object.value("loader_log_enabled").toBool();
            }
        }

        // TAB_PREFERENCES
        if (json_interface_object.value(GetToken(TAB_PREFERENCES)) != QJsonValue::Undefined) {
            const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_PREFERENCES)).toObject();

            if (json_object.value("use_notify_releases") != QJsonValue::Undefined) {
                this->use_notify_releases = json_object.value("use_notify_releases").toBool();
            }

            if (json_object.value("use_layer_debug_mode") != QJsonValue::Undefined) {
                this->use_layer_debug_mode = json_object.value("use_layer_debug_mode").toBool();
            }

            if (json_object.value("current_theme_mode") != QJsonValue::Undefined) {
                this->current_theme_mode = ::GetThemeMode(json_object.value("current_theme_mode").toString().toStdString().c_str());
            }

            if (json_object.value("theme_light_alternate_enabled") != QJsonValue::Undefined) {
                this->theme_light_alternate_enabled = json_object.value("theme_light_alternate_enabled").toBool();
            }

            if (json_object.value("theme_dark_alternate_enabled") != QJsonValue::Undefined) {
                this->theme_dark_alternate_enabled = json_object.value("theme_dark_alternate_enabled").toBool();
            }

            if (json_object.value("theme_light_alternate_color") != QJsonValue::Undefined) {
                this->theme_light_alternate_color = json_object.value("theme_light_alternate_color").toString();
            }

            if (json_object.value("theme_dark_alternate_color") != QJsonValue::Undefined) {
                this->theme_dark_alternate_color = json_object.value("theme_dark_alternate_color").toString();
            }

            if (json_object.value("latest_sdk_version") != QJsonValue::Undefined) {
                this->latest_sdk_version = Version(json_object.value("latest_sdk_version").toString().toStdString().c_str());
            }

            if (json_object.value("use_system_tray") != QJsonValue::Undefined) {
                this->use_system_tray = json_object.value("use_system_tray").toBool();
            }

            if (json_object.value("show_external_layers_settings") != QJsonValue::Undefined) {
                this->show_external_layers_settings = json_object.value("show_external_layers_settings").toBool();
            }

            if (json_object.value("VULKAN_HOME") != QJsonValue::Undefined) {
                ::SetHomePath(json_object.value("VULKAN_HOME").toString().toStdString());
            }

            if (json_object.value("all_enabled_executables_behavior") != QJsonValue::Undefined) {
                this->executable_behavior = ::GetExecutableAllEnabledBehavior(
                    json_object.value("all_enabled_executables_behavior").toString().toStdString().c_str());
            }
            if (json_object.value("VULKAN_DOWNLOAD") != QJsonValue::Undefined) {
                ::SetDownloadPath(json_object.value("VULKAN_DOWNLOAD").toString().toStdString());
            }
        }

        this->executables.Load(json_root_object, this->mode);
        this->layers.Load(json_root_object, this->mode);
        this->configurations.Load(json_root_object, this->mode);
    } else {
        this->executables.Reset();
        if (this->mode != CONFIGURATOR_MODE_NONE) {
            this->layers.LoadAllInstalledLayers(this->mode);
        }
    }

    if (this->driver_override_list.empty()) {
        for (std::size_t i = 0, n = this->vulkan_system_info.physicalDevices.size(); i < n; ++i) {
            this->driver_override_list.push_back(GetDeviceInfo(this->vulkan_system_info.physicalDevices[i]));
        }
    }

    if (this->latest_sdk_version < this->current_sdk_version) {
        this->latest_sdk_version = this->current_sdk_version;
    }

    this->configurations.LoadAllConfigurations(this->layers, this->mode);

    return has_init_file;
}

bool Configurator::Save() const {
    if (this->mode == CONFIGURATOR_MODE_NONE) {
        return true;
    }

    if (!this->window_geometry.isEmpty()) {
        QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
        settings.setValue(MAINWINDOW_GEOMETRY, this->window_geometry);
        settings.setValue(MAINWINDOW_STATE, this->window_state);
    }

    QJsonObject json_root_object;
    json_root_object.insert("file_format_version", Version::VKCONFIG.str().c_str());

    QJsonObject json_interface_object;

    // TAB_CONFIGURATIONS
    {
        QJsonObject json_object;
        json_object.insert("advanced", this->advanced);
        json_object.insert("executable_scope", ::GetToken(this->executable_scope));
        json_object.insert("selected_global_configuration", this->selected_global_configuration.c_str());
        json_interface_object.insert(::GetToken(TAB_CONFIGURATIONS), json_object);
    }

    // TAB_LAYERS
    {
        QJsonObject json_object;
        json_interface_object.insert(GetToken(TAB_LAYERS), json_object);
    }

    // TAB_DRIVER
    {
        QJsonObject json_object;
        json_object.insert("driver_override_enabled", this->driver_override_enabled);
        json_object.insert("driver_override_mode", ::GetToken(this->driver_override_mode));

        if (!this->driver_override_info.deviceName.empty()) {
            QJsonObject json_object_info;
            json_object_info.insert("deviceName", this->driver_override_info.deviceName.c_str());
            QJsonArray json_device_uuid;
            for (std::size_t i = 0, n = std::size(this->driver_override_info.deviceUUID); i < n; ++i) {
                json_device_uuid.append(this->driver_override_info.deviceUUID[i]);
            }
            json_object_info.insert("deviceUUID", json_device_uuid);
            json_object_info.insert("driverVersion", static_cast<std::int32_t>(this->driver_override_info.driverVersion));
            json_object.insert("driver_override_info", json_object_info);
        }

        QJsonArray json_driver_override_list_array;
        for (auto it = this->driver_override_list.begin(); it != this->driver_override_list.end(); ++it) {
            if (it->deviceName.empty()) {
                continue;
            }

            QJsonObject json_object_info;
            json_object_info.insert("deviceName", it->deviceName.c_str());
            QJsonArray json_device_uuid;
            for (std::size_t i = 0, n = std::size(this->driver_override_info.deviceUUID); i < n; ++i) {
                json_device_uuid.append(it->deviceUUID[i]);
            }
            json_object_info.insert("deviceUUID", json_device_uuid);
            json_object_info.insert("driverVersion", static_cast<std::int32_t>(it->driverVersion));
            json_driver_override_list_array.append(json_object_info);
        }
        json_object.insert("driver_override_list", json_driver_override_list_array);

        json_object.insert("last_driver_path", this->last_driver_path.RelativePath().c_str());
        QJsonObject json_object_paths;
        for (auto it = this->driver_paths.begin(); it != this->driver_paths.end(); ++it) {
            QJsonObject json_object_path;
            json_object_path.insert("enabled", it->second);
            json_object_paths.insert(it->first.RelativePath().c_str(), json_object_path);
        }

        json_object.insert("driver_paths_enabled", this->driver_paths_enabled);
        json_object.insert("driver_paths", json_object_paths);

        json_interface_object.insert(::GetToken(TAB_DRIVERS), json_object);
    }

    // TAB_APPLICATIONS
    {
        QJsonObject json_object;
        json_interface_object.insert(GetToken(TAB_APPLICATIONS), json_object);
    }

    // TAB_DIAGNOSTICS
    {
        QJsonObject json_object;
        json_object.insert("loader_log_enabled", this->loader_log_enabled);
        SaveStringArray(json_object, "loader_message_types", ::GetLogTokens(this->loader_log_messages_flags));
        json_interface_object.insert(GetToken(TAB_DIAGNOSTIC), json_object);
    }

    // TAB_PREFERENCES
    {
        QJsonObject json_object;
        json_object.insert("use_system_tray", this->use_system_tray);
        json_object.insert("use_layer_debug_mode", this->use_layer_debug_mode);
        json_object.insert("current_theme_mode", ::GetToken(this->current_theme_mode));
        json_object.insert("theme_light_alternate_enabled", this->theme_light_alternate_enabled);
        json_object.insert("theme_dark_alternate_enabled", this->theme_dark_alternate_enabled);
        json_object.insert("theme_light_alternate_color", this->theme_light_alternate_color.name());
        json_object.insert("theme_dark_alternate_color", this->theme_dark_alternate_color.name());
        json_object.insert("use_notify_releases", this->use_notify_releases);
        json_object.insert("latest_sdk_version", this->latest_sdk_version.str().c_str());
        json_object.insert("show_external_layers_settings", this->show_external_layers_settings);
        json_object.insert("VULKAN_HOME", ::Path(Path::HOME).RelativePath().c_str());
        json_object.insert("VULKAN_DOWNLOAD", ::Path(Path::DOWNLOAD).RelativePath().c_str());
        json_object.insert("all_enabled_executables_behavior", ::GetToken(this->executable_behavior));

        json_interface_object.insert(GetToken(TAB_PREFERENCES), json_object);
    }

    // interface json object
    {
        json_interface_object.insert("has_crashed", this->has_crashed);

        QJsonArray json_hide_message_boxes_array;
        for (int i = HIDE_MESSAGE_FIRST, n = HIDE_MESSAGE_COUNT; i < n; ++i) {
            HideMessageType type = static_cast<HideMessageType>(i);
            if (this->hide_message_boxes_flags & (1 << i)) {
                json_hide_message_boxes_array.append(GetToken(type));
            }
        }
        json_interface_object.insert("hide_message_boxes", json_hide_message_boxes_array);

        QJsonObject json_hide_layer_message_boxes_object;
        for (auto it = this->ignored_messages.begin(); it != this->ignored_messages.end(); ++it) {
            json_hide_layer_message_boxes_object.insert(it->first.c_str(), it->second);
        }
        json_interface_object.insert("hide_layer_message_boxes", json_hide_layer_message_boxes_object);

        json_interface_object.insert("active_tab", GetToken(this->active_tab));

        json_root_object.insert("interface", json_interface_object);
    }

    this->configurations.Save(json_root_object);
    this->layers.Save(json_root_object);
    this->executables.Save(json_root_object);

    QJsonDocument json_doc(json_root_object);

    const Path& vkconfig_init_path = ::Path(Path::INIT);
    QFile file(vkconfig_init_path.AbsolutePath().c_str());
    const bool result = file.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result);

    file.write(json_doc.toJson());
    file.close();

    return true;
}

void Configurator::Set(HideMessageType type) {
    assert(type >= HIDE_MESSAGE_FIRST && type <= HIDE_MESSAGE_LAST);
    this->hide_message_boxes_flags |= GetBit(type);
}

bool Configurator::Get(HideMessageType type) const { return this->hide_message_boxes_flags & GetBit(type); }

ExecutableScope Configurator::GetExecutableScope() const { return this->executable_scope; }

void Configurator::SetExecutableScope(ExecutableScope scope) {
    assert(scope >= EXECUTABLE_SCOPE_FIRST && scope <= EXECUTABLE_SCOPE_LAST);
    this->executable_scope = scope;
}

bool Configurator::GetUseSystemTray() const { return this->use_system_tray; }

void Configurator::SetUseSystemTray(bool enabled) { this->use_system_tray = enabled; }

ExecutableAllEnabledBehavior Configurator::GetAllEnabledExecutableBehavior() const { return this->executable_behavior; }

void Configurator::SetAllEnabledExecutableBehavior(ExecutableAllEnabledBehavior behavior) { this->executable_behavior = behavior; }

bool Configurator::GetUseLayerDebugMode() const { return this->use_layer_debug_mode; }

void Configurator::SetUseLayerDebugMode(bool enabled) { this->use_layer_debug_mode = enabled; }

bool Configurator::GetUseNotifyReleases() const { return this->use_notify_releases; }

void Configurator::SetUseNotifyReleases(bool enabled) { this->use_notify_releases = enabled; }

bool Configurator::ShouldNotify() const {
    return this->latest_sdk_version < this->online_sdk_version && this->online_sdk_version != Version::NONE;
}

bool Configurator::HasActiveSettings() const {
    switch (this->executable_scope) {
        default:
        case EXECUTABLE_NONE:
            return false;
        case EXECUTABLE_ALL:
        case EXECUTABLE_PER:
        case EXECUTABLE_ANY: {
            if (::EnabledExecutables(this->executable_scope)) {
                if (this->executables.GetActiveExecutable() == nullptr) {
                    return false;
                }
            }

            const Configuration* configuration = this->GetActiveConfiguration();
            if (configuration != nullptr) {
                const Parameter* parameter = configuration->GetActiveParameter();
                if (parameter != nullptr) {
                    if (parameter->builtin != LAYER_BUILTIN_NONE) {
                        return false;
                    } else if (parameter->settings.empty()) {
                        return false;
                    } else {
                        return this->layers.Find(parameter->key, parameter->api_version) != nullptr;
                    }
                } else {
                    return false;
                }
            }
            return false;
        }
    }
}

bool Configurator::HasEnabledUI(EnabledUI enabled_ui) const {
    switch (enabled_ui) {
        default:
            assert(false);
            return false;
        case ENABLE_UI_CONFIG: {
            switch (this->GetExecutableScope()) {
                default:
                case EXECUTABLE_NONE:
                    return false;
                case EXECUTABLE_ALL:
                case EXECUTABLE_PER:
                    return this->executables.GetActiveExecutable() != nullptr;
                case EXECUTABLE_ANY:
                    return true;
            }
        }
        case ENABLE_UI_LOADER:
        case ENABLE_UI_LAYERS: {
            if (!this->HasEnabledUI(ENABLE_UI_CONFIG)) {
                return false;
            }
            if (::EnabledExecutables(this->GetExecutableScope())) {
                const Executable* executable = this->executables.GetActiveExecutable();
                if (executable == nullptr) {
                    return false;
                }
                if (!executable->enabled) {
                    return false;
                }
            }
            return this->GetActiveConfiguration() != nullptr;
        }
        case ENABLE_UI_SETTINGS: {
            if (!this->HasEnabledUI(ENABLE_UI_LAYERS)) {
                return false;
            }
            return this->HasActiveSettings();
        }
    }
}

std::string Configurator::GenerateVulkanStatus() const {
    std::string log;

    log += this->Log();

    log += "Vulkan Layers Locations\n";
    log += this->layers.Log();
    log += this->configurations.Log();
    log += this->executables.Log();

    return log;
}
