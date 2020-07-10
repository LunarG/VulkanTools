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

#include "util.h"
#include "configurator.h"
#include "dlgcustompaths.h"

#include <Qt>
#include <QDir>
#include <QSettings>
#include <QTextStream>
#include <QLibrary>
#include <QMessageBox>
#include <QCheckBox>
#include <QDebug>

#ifdef _WIN32
#include <shlobj_core.h>
#endif

#include <vulkan/vulkan.h>

#include <cassert>

Application::Application(const QString &executable_full_path, const QString &arguments)
    : executable_path(QDir::toNativeSeparators(executable_full_path)),
      working_folder(QDir::toNativeSeparators(QFileInfo(executable_full_path).path())),
      arguments(arguments),
      log_file(QDir::toNativeSeparators(working_folder + QDir::separator() + QFileInfo(executable_full_path).baseName() + ".txt")),
      override_layers(true) {}

const char *GetPhysicalDeviceType(VkPhysicalDeviceType type) {
    const char *translation[] = {"Other", "Integrated GPU", "Discrete GPU", "Virtual GPU", "CPU"};
    return translation[type];
}

//////////////////////////////////////////////////////////////////////////////
// Constructor does all the work. Abstracts away instances where we might
// be searching a disk path, or a registry path.
// TBD, does this really need it's own file/module?
PathFinder::PathFinder(const QString &qsPath, bool bForceFileSystem) {
    if (!bForceFileSystem) {
        QSettings files(qsPath, QSettings::NativeFormat);
        file_list_ = files.allKeys();
    } else {
        QDir dir(qsPath);
        QFileInfoList file_info_list = dir.entryInfoList(QStringList() << "*.json", QDir::Files);

        for (int file_index = 0; file_index < file_info_list.size(); file_index++)
            file_list_ << file_info_list[file_index].filePath();
    }
}

//////////////////////////////////////////////////////////////////////////////
// These are the built-in configurations that are pulled in from the resource
// file.

struct DefaultConfiguration {
    const char *name;
    const char *required_layer;
    Version required_api_version;
    const char *preset_label;
    ValidationPreset preset;
};

static const DefaultConfiguration default_configurations[] = {
    {"Validation - Standard", "VK_LAYER_KHRONOS_validation", Version("1.0.0"), "Standard", ValidationPresetStandard},
    {"Validation - GPU-Assisted", "VK_LAYER_KHRONOS_validation", Version("1.1.126"), "GPU-Assisted", ValidationPresetGPUAssisted},
    {"Validation - Shader Printf", "VK_LAYER_KHRONOS_validation", Version("1.1.126"), "Shader Printf",
     ValidationPresetShaderPrintf},
    {"Validation - Reduced-Overhead", "VK_LAYER_KHRONOS_validation", Version("1.0.0"), "Reduced-Overhead",
     ValidationPresetReducedOverhead},
    {"Validation - Best Practices", "VK_LAYER_KHRONOS_validation", Version("1.1.126"), "Best Practices",
     ValidationPresetBestPractices},
#if ENABLE_VALIDATION_SYNC
    {"Validation - Synchronization (Beta)", "VK_LAYER_KHRONOS_validation", "1.2.145", "Synchronization (Beta)",
     ValidationPresetSynchronization},
#endif
#ifndef __APPLE__
    {"Frame Capture - First two frames", "VK_LAYER_LUNARG_gfxreconstruct", "1.2.145", "", ValidationPresetNone},
    {"Frame Capture - Range (F10 to start and to stop)", "VK_LAYER_LUNARG_gfxreconstruct", "1.2.145", "", ValidationPresetNone},
#endif
    {"API dump", "VK_LAYER_LUNARG_api_dump", "1.1.126", "", ValidationPresetNone}};

static const DefaultConfiguration *FindDefaultConfiguration(const char *name) {
    assert(name);

    for (std::size_t i = 0, n = countof(default_configurations); i < n; ++i) {
        if (strcmp(default_configurations[i].name, name) != 0) continue;
        return &default_configurations[i];
    }

    return nullptr;  // Not found
}

static const DefaultConfiguration *FindDefaultConfiguration(ValidationPreset preset) {
    assert(preset >= ValidationPresetFirst && preset <= ValidationPresetLast);

    for (std::size_t i = 0, n = countof(default_configurations); i < n; ++i) {
        if (default_configurations[i].preset != preset) continue;
        return &default_configurations[i];
    }

    return nullptr;  // Not found
}

const char *Configurator::GetValidationPresetName(ValidationPreset preset) const {
    // 0 is user defined, there is no file for that
    assert(preset > ValidationPresetUserDefined);

    const DefaultConfiguration *configuration = FindDefaultConfiguration(preset);
    if (configuration) return configuration->name;

    assert(0);
    return nullptr;
}

const char *Configurator::GetValidationPresetLabel(ValidationPreset preset) const {
    const DefaultConfiguration *configuration = FindDefaultConfiguration(preset);
    if (configuration) return configuration->preset_label;

    assert(0);
    return nullptr;
}

// I am purposly not flagging these as explicit or implicit as this can be parsed from the location
// and future updates to layer locations will only require a smaller change.
#ifdef _WIN32
static const QString szSearchPaths[] = {"HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ExplicitLayers",
                                        "HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ImplicitLayers",
                                        "HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ExplicitLayers",
                                        "HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ImplicitLayers",
                                        "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanExplicitLayers",
                                        "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanImplicitLayers"};
#else
static const QString szSearchPaths[] = {"/usr/local/etc/vulkan/explicit_layer.d",  // Not used on macOS, okay to just ignore
                                        "/usr/local/etc/vulkan/implicit_layer.d",  // Not used on macOS, okay to just ignore
                                        "/usr/local/share/vulkan/explicit_layer.d",
                                        "/usr/local/share/vulkan/implicit_layer.d",
                                        "/etc/vulkan/explicit_layer.d",
                                        "/etc/vulkan/implicit_layer.d",
                                        "/usr/share/vulkan/explicit_layer.d",
                                        "/usr/share/vulkan/implicit_layer.d",
                                        ".local/share/vulkan/explicit_layer.d",
                                        ".local/share/vulkan/implicit_layer.d"};
#endif  // QDir().homePath() + "./local...." do this inline?

Configurator &Configurator::Get() {
    static Configurator configurator;
    return configurator;
}

Configurator::Configurator()
    : has_old_loader(false),
      first_run_(true),
      override_application_list_updated(false),
      saved_configuration(nullptr),
      active_configuration_(nullptr) {
    available_Layers.reserve(10);

    {
        QSettings settings;
        const char *saved_version = settings.value(VKCONFIG_KEY_VKCONFIG_VERSION).toString().toUtf8().constData();
        const char *current_version =
            QString()
                .asprintf("%d.%d.%d", VK_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE), VK_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE),
                          VK_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE))
                .toUtf8()
                .constData();

        if (Version(saved_version) != Version(current_version)) {
            settings.setValue(VKCONFIG_KEY_VKCONFIG_VERSION, current_version);
            settings.setValue(VKCONFIG_KEY_FIRST_RUN, true);
            settings.setValue(VKCONFIG_KEY_ACTIVEPROFILE, "Validation - Standard");
            settings.setValue(VKCONFIG_KEY_RESTORE_GEOMETRY, false);
        }
    }

#if defined(_WIN32) && QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    running_as_administrator_ = IsUserAnAdmin();
#else
    running_as_administrator_ = false;
#endif

    // Where is stuff
#ifdef _WIN32
    // Assemble the path name where the overide .json file goes
    QDir temp_path = QDir::temp();
    if (!temp_path.cd("VulkanLayerManager")) {
        temp_path.mkpath("VulkanLayerManager");
        temp_path.cd("VulkanLayerManager");
    }
    SetPath(OverrideLayersPath, temp_path.absoluteFilePath("VkLayer_override.json"));
    SetPath(OverrideSettingsPath, temp_path.absoluteFilePath("vk_layer_settings.txt"));

    QDir home = QDir::home();
    QString configuration_path = home.path() + QString("/AppData/Local/");
    home.setPath(configuration_path);
    if (!home.cd("LunarG")) {
        home.mkpath("LunarG");
        home.cd("LunarG");
    }

    if (!home.cd("vkconfig")) home.mkpath("vkconfig");

    SetPath(ConfigurationPath, configuration_path + "LunarG/vkconfig");
#else
    QDir home = QDir::home();
    if (!home.cd(".local")) {
        home.mkpath(".local");
        home.cd(".local");
    }

    if (!home.cd("share")) {
        home.mkpath("share");
        home.cd("share");
    }

    if (!home.cd("vulkan")) {
        home.mkpath("vulkan");
        home.cd("vulkan");
    }

    if (!home.cd("settings.d")) {
        home.mkpath("settings.d");
        home.cd("settings.d");
    }

    home.cd("..");
    if (!home.cd("implicit_layer.d")) {
        home.mkpath("implicit_layer.d");
        home.cd("implicit_layer.d");
    }

    home = QDir::home();
    const QString configuration_path = home.path() + QString("/.local/share/vulkan/");
    SetPath(ConfigurationPath, configuration_path);  // TBD, where do configuration file go if not here...
    SetPath(OverrideLayersPath, configuration_path + "implicit_layer.d/VkLayer_override.json");
    SetPath(OverrideSettingsPath, configuration_path + "settings.d/vk_layer_settings.txt");
#endif

    // Check loader version
#ifdef WIN32
    QLibrary library("vulkan-1.dll");
#else
    QLibrary library("libvulkan");
#endif
    if (!(library.load())) {
        QMessageBox dlg(NULL);
        dlg.setText("Could not find a Vulkan Loader!");
        dlg.setIcon(QMessageBox::Critical);
        dlg.exec();
    } else {
        // Now is a good time to see if we have the old loader
        PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
        vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)library.resolve("vkEnumerateInstanceVersion");
        if (VK_SUCCESS == vkEnumerateInstanceVersion(&vulkan_instance_version)) {
            if (vulkan_instance_version < 4202633) {
                has_old_loader = true;
            }
        }
    }

    // See if the VK_LAYER_PATH environment variable is set. If so, parse it and
    // assemble a list of paths that take precidence for layer discovery.
    QString layer_path = qgetenv("VK_LAYER_PATH");
    if (!layer_path.isEmpty()) {
#ifdef _WIN32
        VK_LAYER_PATH = layer_path.split(";");  // Windows uses ; as seperator
#else
        VK_LAYER_PATH = layer_path.split(":");  // Linux/macOS uses : as seperator
#endif
    }
}

///////////////////////////////////////////////////////////////////////////////////
/// A good rule of C++ is to not put things in the constructor that can fail, or
/// that might require recursion. This initializes
///
bool Configurator::InitializeConfigurator(void) {
    // Load simple app settings, the additional search paths, and the
    // override app list.
    LoadSettings();
    LoadCustomLayersPaths();
    LoadOverriddenApplicationList();
    LoadDefaultLayerSettings();  // findAllInstalledLayers uses the results of this.
    LoadAllInstalledLayers();

    // If no layers are found, give the user another chance to add some custom paths
    if (available_Layers.empty()) {
        QMessageBox alert;
        alert.setText(
            "No Vulkan Layers were found in standard paths or in the SDK path. Vulkan Layers are required in order to use Vulkan "
            "Configurator.\n\n"
            "Please select the path where you have your layers located.");
        alert.setWindowTitle("No Vulkan Layers found");
        alert.setIcon(QMessageBox::Warning);
        alert.exec();

        dlgCustomPaths dlg;
        dlg.exec();

        // Give it one more chance... If there are still no layers, bail
        LoadAllInstalledLayers();
    }

    if (available_Layers.empty()) {
        QMessageBox alert;
        alert.setText("Could not initialize Vulkan Configurator.");
        alert.setWindowTitle(VKCONFIG_NAME);
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        return false;
    }

    LoadAllConfigurations();

    // This will reset or clear the current profile if the files have been
    // manually manipulated
    SetActiveConfiguration(active_configuration_);

    return true;
}

Configurator::~Configurator() {
    ClearLayerLists();
    qDeleteAll(available_configurations.begin(), available_configurations.end());
    available_configurations.clear();
}

bool Configurator::HasLayers() const { return !available_Layers.empty(); }

QString Configurator::CheckVulkanSetup() const {
    QString log;

    // Check Vulkan SDK path
    QString search_path = qgetenv("VULKAN_SDK");
    QFileInfo local(search_path);
    if (local.exists())
        log += QString().asprintf("- SDK path: %s\n", search_path.toUtf8().constData());
    else
        log += "- SDK path: Not detected\n";

        // Check loader version
#ifdef WIN32
    QLibrary library("vulkan-1.dll");
#else
    QLibrary library("libvulkan");
#endif

    uint32_t version = vulkan_instance_version;
    log += QString().asprintf("- Loader version: %d.%d.%d\n", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version),
                              VK_VERSION_PATCH(version));

    if (!(library.load())) {
        QMessageBox alert(NULL);
        alert.setWindowTitle("Vulkan Development Status failure...");
        alert.setText("Could not find a Vulkan Loader.");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        log += "- Could not find a Vulkan Loader.\n";
        return log;
    }

    // Check layer paths
    if (custom_layers_paths_.count() > 0) {
        log += "- Custom Layers Paths:\n";
        for (int i = 0, n = custom_layers_paths_.count(); i < n; ++i)
            log += QString().asprintf("    - %s\n", custom_layers_paths_[i].toUtf8().constData());
    } else
        log += "- Custom Layers Paths: None\n";

    PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties =
        (PFN_vkEnumerateInstanceLayerProperties)library.resolve("vkEnumerateInstanceLayerProperties");

    std::uint32_t instance_layer_count = 0;
    VkResult err = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
    Q_ASSERT(!err);

    std::vector<VkLayerProperties> layers_properties;
    layers_properties.resize(instance_layer_count);

    err = vkEnumerateInstanceLayerProperties(&instance_layer_count, &layers_properties[0]);
    Q_ASSERT(!err);

    log += "- Available Layers:\n";
    for (std::size_t i = 0, n = layers_properties.size(); i < n; ++i) {
        log += QString().asprintf("    - %s\n", layers_properties[i].layerName);
    }

    // Check Vulkan Devices

    VkApplicationInfo app;
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pNext = NULL;
    app.pApplicationName = APP_SHORT_NAME;
    app.applicationVersion = 0;
    app.pEngineName = APP_SHORT_NAME;
    app.engineVersion = 0;
    app.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo inst_info;
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.pApplicationInfo = &app;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = NULL;
    inst_info.enabledExtensionCount = 0;
    inst_info.ppEnabledExtensionNames = NULL;

    uint32_t gpu_count;

    VkInstance inst;
    PFN_vkCreateInstance vkCreateInstance = (PFN_vkCreateInstance)library.resolve("vkCreateInstance");
    err = vkCreateInstance(&inst_info, NULL, &inst);
    if (err == VK_ERROR_INCOMPATIBLE_DRIVER) {
        QMessageBox alert(NULL);
        alert.setWindowTitle("Vulkan Development Status failure...");
        alert.setText("Cannot find a compatible Vulkan installable client driver (ICD).");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        log += "- Cannot find a compatible Vulkan installable client driver (ICD).\n";
        return log;
    }

    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices =
        (PFN_vkEnumeratePhysicalDevices)library.resolve("vkEnumeratePhysicalDevices");
    err = vkEnumeratePhysicalDevices(inst, &gpu_count, NULL);

    // This can fail on a new Linux setup. Check and fail gracefully rather than crash.
    if (err != VK_SUCCESS) {
        QMessageBox alert(NULL);
        alert.setWindowTitle("Vulkan Development Status failure...");
        alert.setText("Cannot find any Vulkan Physical Devices.");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        log += "- Cannot find a compatible Vulkan installable client driver (ICD).\n";
        return log;
    }

    std::vector<VkPhysicalDevice> devices;
    devices.resize(gpu_count);

    err = vkEnumeratePhysicalDevices(inst, &gpu_count, &devices[0]);
    Q_ASSERT(!err);

    log += "- Physical Devices:\n";
    for (std::size_t i = 0, n = devices.size(); i < n; ++i) {
        VkPhysicalDeviceProperties properties;
        PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties =
            (PFN_vkGetPhysicalDeviceProperties)library.resolve("vkGetPhysicalDeviceProperties");
        vkGetPhysicalDeviceProperties(devices[i], &properties);
        log += QString().asprintf("    - %s (%s) with Vulkan %d.%d.%d\n", properties.deviceName,
                                  GetPhysicalDeviceType(properties.deviceType), VK_VERSION_MAJOR(properties.apiVersion),
                                  VK_VERSION_MINOR(properties.apiVersion), VK_VERSION_PATCH(properties.apiVersion));
    }

    PFN_vkDestroyInstance vkDestroyInstance = (PFN_vkDestroyInstance)library.resolve("vkDestroyInstance");
    vkDestroyInstance(inst, NULL);

    return log;
}

void Configurator::ClearLayerLists() {
    qDeleteAll(available_Layers.begin(), available_Layers.end());
    available_Layers.clear();
}

#ifdef _WIN32
///////////////////////////////////////////////////////////////////////
/// Look for device specific layers
void Configurator::LoadDeviceRegistry(DEVINST id, const QString &entry, QVector<LayerFile *> &layerList, LayerType type) {
    HKEY key;
    if (CM_Open_DevNode_Key(id, KEY_QUERY_VALUE, 0, RegDisposition_OpenExisting, &key, CM_REGISTRY_SOFTWARE) != CR_SUCCESS) return;

    DWORD path_size;
    if (RegQueryValueExW(key, (LPCWSTR)entry.utf16(), nullptr, nullptr, nullptr, &path_size) != ERROR_SUCCESS) {
        RegCloseKey(key);
        return;
    }

    DWORD data_type;
    wchar_t *path = new wchar_t[path_size];
    if (RegQueryValueExW(key, (LPCWSTR)entry.utf16(), nullptr, &data_type, (LPBYTE)path, &path_size) != ERROR_SUCCESS) {
        delete[] path;
        RegCloseKey(key);
        return;
    }

    if (data_type == REG_SZ || data_type == REG_MULTI_SZ) {
        for (wchar_t *curr_filename = path; curr_filename[0] != '\0'; curr_filename += wcslen(curr_filename) + 1) {
            LayerFile *pLayerFile = new LayerFile();
            if (pLayerFile->ReadLayerFile(QString::fromWCharArray(curr_filename), type)) layerList.push_back(pLayerFile);
            if (data_type == REG_SZ) {
                break;
            }
        }
    }

    delete[] path;
    RegCloseKey(key);
}

////////////////////////////////////////////////////////////////
/// This is for Windows only. It looks for device specific layers in
/// the Windows registry.
void Configurator::LoadRegistryLayers(const QString &path, QVector<LayerFile *> &layerList, LayerType type) {
    QString root_string = path.section('\\', 0, 0);
    static QHash<QString, HKEY> root_keys = {
        {"HKEY_CLASSES_ROOT", HKEY_CLASSES_ROOT},
        {"HKEY_CURRENT_CONFIG", HKEY_CURRENT_CONFIG},
        {"HKEY_CURRENT_USER", HKEY_CURRENT_USER},
        {"HKEY_LOCAL_MACHINE", HKEY_LOCAL_MACHINE},
        {"HKEY_USERS", HKEY_USERS},
    };

    HKEY root = HKEY_CURRENT_USER;
    for (auto label : root_keys.keys()) {
        if (label == root_string) {
            root = root_keys[label];
            break;
        }
    }

    static const QString DISPLAY_GUID = "{4d36e968-e325-11ce-bfc1-08002be10318}";
    static const QString SOFTWARE_COMPONENT_GUID = "{5c4c3332-344d-483c-8739-259e934c9cc8}";
    static const ULONG FLAGS = CM_GETIDLIST_FILTER_CLASS | CM_GETIDLIST_FILTER_PRESENT;

    ULONG device_names_size;
    wchar_t *device_names = nullptr;
    do {
        CM_Get_Device_ID_List_SizeW(&device_names_size, (LPCWSTR)DISPLAY_GUID.utf16(), FLAGS);
        if (device_names != nullptr) {
            delete[] device_names;
        }
        device_names = new wchar_t[device_names_size];
    } while (CM_Get_Device_ID_ListW((LPCWSTR)DISPLAY_GUID.utf16(), device_names, device_names_size, FLAGS) == CR_BUFFER_SMALL);

    if (device_names != nullptr) {
        QString entry;
        // This has already been set by now
        if (path.endsWith("VulkanExplicitLayers")) {
            entry = "VulkanExplicitLayers";
            type = LAYER_TYPE_EXPLICIT;
        } else if (path.endsWith("VulkanImplicitLayers")) {
            entry = "VulkanImplicitLayers";
            type = LAYER_TYPE_IMPLICIT;
        }

        for (wchar_t *device_name = device_names; device_name[0] != '\0'; device_name += wcslen(device_name) + 1) {
            DEVINST device_id;
            if (CM_Locate_DevNodeW(&device_id, device_name, CM_LOCATE_DEVNODE_NORMAL) != CR_SUCCESS) {
                continue;
            }
            LoadDeviceRegistry(device_id, entry, layerList, type);

            DEVINST child_id;
            if (CM_Get_Child(&child_id, device_id, 0) != CR_SUCCESS) {
                continue;
            }
            do {
                wchar_t child_buffer[MAX_DEVICE_ID_LEN];
                CM_Get_Device_IDW(child_id, child_buffer, MAX_DEVICE_ID_LEN, 0);

                wchar_t child_guid[MAX_GUID_STRING_LEN + 2];
                ULONG child_guid_size = sizeof(child_guid);
                if (CM_Get_DevNode_Registry_Property(child_id, CM_DRP_CLASSGUID, nullptr, &child_guid, &child_guid_size, 0) !=
                    CR_SUCCESS) {
                    continue;
                }
                if (wcscmp(child_guid, (LPCWSTR)SOFTWARE_COMPONENT_GUID.utf16()) == 0) {
                    LoadDeviceRegistry(child_id, entry, layerList, type);
                    break;
                }
            } while (CM_Get_Sibling(&child_id, child_id, 0) == CR_SUCCESS);
        }
    }

    if (device_names != nullptr) {
        delete[] device_names;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// On Windows the overide json file and settings file are not used unless the path to those
/// files are stored in the registry.
void Configurator::AddRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile) {
    // Layer override json file
    HKEY key;
    HKEY userKey = (running_as_administrator_) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;

    REGSAM access = KEY_WRITE;
    LSTATUS err = RegCreateKeyEx(userKey, TEXT("SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers"), 0, NULL, REG_OPTION_NON_VOLATILE,
                                 access, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) return;

    QString file_path;
    DWORD value_count;
    DWORD value = 0;
    RegQueryInfoKey(key, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, NULL, NULL, NULL);
    RegSetValueExW(key, (LPCWSTR)qsJSONFile.utf16(), 0, REG_DWORD, (BYTE *)&value, sizeof(value));
    RegCloseKey(key);

    // Layer settings file
    err = RegCreateKeyEx(userKey, TEXT("SOFTWARE\\Khronos\\Vulkan\\Settings"), 0, NULL, REG_OPTION_NON_VOLATILE, access, NULL, &key,
                         NULL);
    if (err != ERROR_SUCCESS) return;

    RegQueryInfoKeyW(key, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, NULL, NULL, NULL);
    RegSetValueExW(key, (LPCWSTR)qsSettingsFile.utf16(), 0, REG_DWORD, (BYTE *)&value, sizeof(value));
    RegCloseKey(key);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// On Windows the overide json file and settings file are not used unless the path to those
/// files are stored in the registry.
void Configurator::RemoveRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile) {
    // Layer override json file
    HKEY key;
    HKEY userKey = (running_as_administrator_) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;

    REGSAM access = KEY_WRITE;
    LSTATUS err = RegCreateKeyEx(userKey, TEXT("SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers"), 0, NULL, REG_OPTION_NON_VOLATILE,
                                 access, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) return;

    RegDeleteValueW(key, (LPCWSTR)qsJSONFile.utf16());
    RegCloseKey(key);

    // Layer settings file
    err = RegCreateKeyEx(userKey, TEXT("SOFTWARE\\Khronos\\Vulkan\\Settings"), 0, NULL, REG_OPTION_NON_VOLATILE, access, NULL, &key,
                         NULL);
    if (err != ERROR_SUCCESS) return;

    RegDeleteValueW(key, (LPCWSTR)qsSettingsFile.utf16());
    RegCloseKey(key);
}

#endif

///////////////////////////////////////////////////////////////////////////////
/// This is for the local application settings, not the system Vulkan settings
void Configurator::LoadSettings() {
    // Load the launch app name from the last session
    QSettings settings;
    active_launch_executable_path_ = settings.value(VKCONFIG_KEY_LAUNCHAPP).toString();
    override_active = settings.value(VKCONFIG_KEY_OVERRIDE_ACTIVE, true).toBool();
    overridden_application_list_only = settings.value(VKCONFIG_KEY_APPLY_ONLY_TO_LIST).toBool();
    override_permanent = settings.value(VKCONFIG_KEY_KEEP_ACTIVE_ON_EXIT).toBool();
    paths_[LastExportPath] = settings.value(VKCONFIG_KEY_LAST_EXPORT_PATH).toString();
    paths_[LastImportPath] = settings.value(VKCONFIG_KEY_LAST_IMPORT_PATH).toString();
    paths_[LastExecutablePath] = settings.value(VKCONFIG_KEY_LAST_EXECUTABLE_PATH).toString();
}

///////////////////////////////////////////////////////////////////////////////
/// This is for the local application settings, not the system Vulkan settings
void Configurator::SaveSettings() {
    QSettings settings;
    settings.setValue(VKCONFIG_KEY_LAUNCHAPP, active_launch_executable_path_);
    settings.setValue(VKCONFIG_KEY_OVERRIDE_ACTIVE, override_active);
    settings.setValue(VKCONFIG_KEY_APPLY_ONLY_TO_LIST, overridden_application_list_only);
    settings.setValue(VKCONFIG_KEY_KEEP_ACTIVE_ON_EXIT, override_permanent);
    settings.setValue(VKCONFIG_KEY_LAST_EXPORT_PATH, paths_[LastExportPath]);
    settings.setValue(VKCONFIG_KEY_LAST_IMPORT_PATH, paths_[LastImportPath]);
    settings.setValue(VKCONFIG_KEY_LAST_EXECUTABLE_PATH, paths_[LastExecutablePath]);
}

void Configurator::ResetToDefaultSettings() {
    QSettings settings;
    settings.setValue(VKCONFIG_KEY_LAUNCHAPP, "");
    settings.setValue(VKCONFIG_KEY_OVERRIDE_ACTIVE, true);
    settings.setValue(VKCONFIG_KEY_APPLY_ONLY_TO_LIST, false);
    settings.setValue(VKCONFIG_KEY_KEEP_ACTIVE_ON_EXIT, false);
    settings.setValue(VKCONFIG_KEY_LAST_EXPORT_PATH, "");
    settings.setValue(VKCONFIG_KEY_LAST_IMPORT_PATH, "");
    settings.setValue(VKCONFIG_KEY_LAST_EXECUTABLE_PATH, "");
}

QString Configurator::GetPath(Path requested_path) const {
    Q_ASSERT(requested_path >= FirstPath && requested_path <= LastPath);
    const QString path = paths_[requested_path];

    if (!path.isEmpty()) {
        return path;
    }

    // Use export path when import path is empty and import path when export path is empty
    if (requested_path == LastImportPath && !paths_[LastExportPath].isEmpty()) {
        return paths_[LastExportPath];
    } else if (requested_path == LastExportPath && !paths_[LastImportPath].isEmpty()) {
        return paths_[LastImportPath];
    } else {
        return QDir::homePath();
    }
}

void Configurator::SetPath(Path requested_path, QString path) {
    Q_ASSERT(requested_path >= FirstPath && requested_path <= LastPath);
    Q_ASSERT(!path.isEmpty());

    path = QDir::toNativeSeparators(path);

    if (requested_path == LastImportPath || requested_path == LastExportPath || requested_path == LastExecutablePath) {
        QDir directory = QFileInfo(path).absoluteDir();
        path = directory.absolutePath();
    }

    qDebug() << ((path + "\n").toUtf8().constData());
    paths_[requested_path] = path;
}

/////////////////////////////////////////////////////////////////////////////
/// We may have additional paths where we want to search for layers.
/// Load the list of paths here.
void Configurator::LoadCustomLayersPaths() {
    QSettings searchPaths;
    custom_layers_paths_ = searchPaths.value(VKCONFIG_KEY_CUSTOM_PATHS).toStringList();
}

/////////////////////////////////////////////////////////////////////////////
/// We may have additional paths where we want to search for layers.
/// Save the list of paths here.
void Configurator::SaveCustomLayersPaths() {
    QSettings searchPaths;
    searchPaths.setValue(VKCONFIG_KEY_CUSTOM_PATHS, custom_layers_paths_);
}

void Configurator::RemoveCustomLayersPath(int path_index) {
    Q_ASSERT(path_index > 0 && path_index < custom_layers_paths_.size());

    custom_layers_paths_.removeAt(path_index);

    SaveCustomLayersPaths();
    LoadAllInstalledLayers();
    LoadAllConfigurations();
}

void Configurator::RemoveCustomLayersPath(const QString &path) {
    Q_ASSERT(!path.isEmpty());

    for (int i = 0; i < custom_layers_paths_.size(); ++i) {
        if (custom_layers_paths_[i] != path) continue;

        RemoveCustomLayersPath(i);
        break;
    }
}

void Configurator::AppendCustomLayersPath(const QString &path) {
    Q_ASSERT(!path.isEmpty());

    custom_layers_paths_.append(path);

    SaveCustomLayersPaths();
    LoadAllInstalledLayers();
    LoadAllConfigurations();
}

int Configurator::GetCustomLayersPathSize() const { return custom_layers_paths_.size(); }

const QString &Configurator::GetCustomLayersPath(int path_index) const {
    Q_ASSERT(path_index >= 0 && path_index < custom_layers_paths_.size());

    return custom_layers_paths_[path_index];
}

void Configurator::SelectLaunchApplication(int application_index) {
    if (application_index < 0) return;

    active_launch_executable_path_ = overridden_application_list[application_index]->executable_path;
}

int Configurator::GetLaunchApplicationIndex() const {
    for (int i = 0; i < overridden_application_list.size(); i++) {
        if (overridden_application_list[i]->executable_path == active_launch_executable_path_) return i;
    }

    return 0;  // Not found, but the list is present, so return the first item.
}

/////////////////////////////////////////////////////////////////////////////
// Search for vkcube and add it to the app list.
void Configurator::FindVkCube() {
    // This should only be called on first run, but make sure it's not already there.
    if (overridden_application_list.size() != 0)
        for (int i = 0; i < overridden_application_list.size(); i++)
            if (overridden_application_list[i]->executable_path.contains("vkcube")) return;

                // One of these must be true, or we just aren't going to compile!
#ifdef _WIN32
    QString application_name("./vkcube.exe");
#endif

#ifdef __APPLE__
    QString application_name("../../vkcube.app");
#endif

#ifdef __linux__
    QString application_name("./vkcube");
#endif

    QString search_path = application_name;
    QFileInfo local(search_path);
    if (!local.exists()) {
        search_path = qgetenv("VULKAN_SDK");
        search_path = search_path + "/bin/" + application_name;
        QFileInfo local2(search_path);
        if (!local2.exists()) return;
        local = local2;
    }

    Application *application = new Application(local.absoluteFilePath(), "--suppress_popups --validate");
    overridden_application_list.push_back(application);
}

///////////////////////////////////////////////////////////////////////////
/// Load the custom application list. This is maintained as a json database
/// file.
void Configurator::LoadOverriddenApplicationList() {
    /////////////////////////////////////////////////////////////
    // Now, use the list
    QString application_list_json = GetPath(ConfigurationPath) + "/applist.json";
    QFile file(application_list_json);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();

    QJsonDocument json_app_list;
    json_app_list = QJsonDocument::fromJson(data.toLocal8Bit());
    if (json_app_list.isObject())
        if (!json_app_list.isEmpty()) {
            // Get the list of apps
            QStringList app_keys;
            QJsonObject json_doc_object = json_app_list.object();
            app_keys = json_doc_object.keys();

            // Get them...
            for (int i = 0; i < app_keys.length(); i++) {
                QJsonValue app_value = json_doc_object.value(app_keys[i]);
                QJsonObject app_object = app_value.toObject();

                Application *application = new Application;
                application->working_folder = app_object.value("app_folder").toString();
                application->executable_path = app_object.value("app_path").toString();
                application->override_layers = !app_object.value("exclude_override").toBool();
                application->log_file = app_object.value("log_file").toString();

                // Arguments are in an array to make room for adding more in a future version
                QJsonArray args = app_object.value("command_lines").toArray();
                application->arguments = args[0].toString();

                overridden_application_list.push_back(application);
            }
        }

    //////////////////////////////////////////////
    // On first run, search for vkcube. Do this after this list
    // is loaded in case it's already there.
    QSettings settings;
    if (settings.value(VKCONFIG_KEY_FIRST_RUN, true).toBool()) FindVkCube();
}

void Configurator::CheckApplicationRestart() const {
    // Display warning for configuration changes
    QSettings settings;
    if (!settings.value(VKCONFIG_HIDE_RESTART_WARNING).toBool()) {
        QMessageBox alert;
        alert.setText(
            "Vulkan Layers are fully configured when creating a Vulkan Instance which typically happens at Vulkan Application "
            "start.\n\n"
            "For changes to take effect, running Vulkan Applications should be restarted.");
        alert.setWindowTitle("Any change requires Vulkan Applications restart");
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
        settings.setValue(VKCONFIG_HIDE_RESTART_WARNING, true);
    }
}

//////////////////////////////////////////////////////////////////////////
/// Save the custom application list in a .json file
void Configurator::SaveOverriddenApplicationList() {
    QJsonObject root;

    for (int i = 0; i < overridden_application_list.size(); i++) {
        // Build an array of appnames with associated data
        QJsonObject application_object;
        application_object.insert("app_path", overridden_application_list[i]->executable_path);
        application_object.insert("app_folder", overridden_application_list[i]->working_folder);
        application_object.insert("exclude_override", !overridden_application_list[i]->override_layers);
        application_object.insert("log_file", overridden_application_list[i]->log_file);

        // Ground work for mulitiple sets of command line arguments
        QJsonArray argsArray;
        argsArray.append(QJsonValue(overridden_application_list[i]->arguments));  // [J] PROBABLY

        application_object.insert("command_lines", argsArray);
        root.insert(QFileInfo(overridden_application_list[i]->executable_path).fileName(), application_object);
    }

    QString app_list_json = GetPath(ConfigurationPath) + "/applist.json";
    QFile file(app_list_json);
    file.open(QFile::WriteOnly);
    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();

    override_application_list_updated = true;
}

bool Configurator::HasOverriddenApplications() const {
    for (int i = 0, n = overridden_application_list.size(); i < n; i++) {
        if (overridden_application_list[i]->override_layers) return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
// Find all installed layers on the system.
void Configurator::LoadAllInstalledLayers() {
    // This is called initially, but also when custom search paths are set, so
    // we need to clear out the old data and just do a clean refresh
    ClearLayerLists();

    // FIRST: If VK_LAYER_PATH is set it has precedence over other layers.
    int lp = VK_LAYER_PATH.count();
    if (lp != 0)
        for (int i = 0; i < lp; i++) LoadLayersFromPath(VK_LAYER_PATH[i], available_Layers);

    // SECOND: Standard layer paths, in standard locations
    for (std::size_t i = 0, n = countof(szSearchPaths); i < n; i++) LoadLayersFromPath(szSearchPaths[i], available_Layers);

    // THIRD: Any custom paths? Search for those too
    for (int i = 0; i < custom_layers_paths_.size(); i++) LoadLayersFromPath(custom_layers_paths_[i], available_Layers);

    // FOURTH: Finally, see if thee is anyting in the VULKAN_SDK path that wasn't already found elsewhere
    QString vulkanSDK = qgetenv("VULKAN_SDK");
    if (!vulkanSDK.isEmpty()) {
        vulkanSDK += "/etc/vulkan/explicit_layer.d";
        LoadLayersFromPath(vulkanSDK, available_Layers);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Search a folder and load up all the layers found there. This does NOT
/// load the default settings for each layer. This is just a master list of
/// layers found. Do NOT load duplicate layer names. The type of layer (explicit or implicit) is
/// determined from the path name.
void Configurator::LoadLayersFromPath(const QString &path, QVector<LayerFile *> &layer_list) {
    // On Windows custom files are in the file system. On non Windows all layers are
    // searched this way
    LayerType type = LAYER_TYPE_CUSTOM;
    if (path.contains("explicit", Qt::CaseInsensitive)) type = LAYER_TYPE_EXPLICIT;

    if (path.contains("implicit", Qt::CaseInsensitive)) type = LAYER_TYPE_IMPLICIT;

#ifdef _WIN32
    if (path.contains("...")) {
        LoadRegistryLayers(path, layer_list, type);
        return;
    }

    PathFinder file_list(path, (type == LAYER_TYPE_CUSTOM));
#else
    // On Linux/Mac, we also need the home folder
    QString search_path = path;
    if (path[0] == '.') {
        search_path = QDir().homePath();
        search_path += "/";
        search_path += path;
    }

    PathFinder file_list(search_path, true);
#endif
    if (file_list.FileCount() == 0) return;

    // We have a list of layer files. Add to the list as long as the layer name has
    // not already been added.
    for (int file_index = 0; file_index < file_list.FileCount(); file_index++) {
        LayerFile *layer_file = new LayerFile();
        if (layer_file->ReadLayerFile(file_list.GetFileName(file_index), type)) {
            // Do not load VK_LAYER_LUNARG_override
            for (int i = 0; i < layer_list.size(); i++)
                if (QString("VK_LAYER_LUNARG_override") == layer_file->name) {
                    delete layer_file;
                    layer_file = nullptr;
                    break;
                }

            if (layer_file == nullptr) continue;

            // Make sure this layer name has not already been added
            if (FindLayerNamed(layer_file->name)) {
                delete layer_file;
                continue;
            }

            // Good to go, add the layer
            layer_list.push_back(layer_file);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Find the settings for this named layer. If none found, return nullptr
const LayerSettingsDefaults *Configurator::FindLayerSettings(const QString &layer_name) const {
    for (int i = 0; i < default_layers_settings.size(); i++)
        if (layer_name == default_layers_settings[i]->layer_name) return default_layers_settings[i];

    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////
/// Search the list of loaded profiles and return a pointer
Configuration *Configurator::FindConfiguration(const QString &configuration_name) const {
    for (int i = 0, n = available_configurations.size(); i < n; i++)
        if (available_configurations[i]->name == configuration_name) return available_configurations[i];
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// Load all the  profiles. If the canned profiles don't exist,
/// they are created from the embedded json files
void Configurator::LoadAllConfigurations() {
    // This might be called to refresh the list...
    qDeleteAll(available_configurations.begin(), available_configurations.end());
    available_configurations.clear();
    active_configuration_ = nullptr;

    // //////////////////////////////////////////////////////////////////////////
    // If this is the first time, we need to create the initial set of
    // configuration files.
    QSettings settings;
    first_run_ = settings.value(VKCONFIG_KEY_FIRST_RUN, true).toBool();
    if (first_run_) {
        // Delete all the *.json files in the storage folder
        QDir dir(GetPath(ConfigurationPath));
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        dir.setNameFilters(QStringList() << "*.json");
        QFileInfoList configuration_files = dir.entryInfoList();

        // Loop through all the configurations found and remove them
        for (int i = 0, n = configuration_files.size(); i < n; i++) {
            QFileInfo info = configuration_files.at(i);
            if (info.absoluteFilePath().contains("applist.json")) continue;
            remove(info.filePath().toUtf8().constData());
        }

        for (std::size_t i = 0, n = countof(default_configurations); i < n; ++i) {
            // Search the list of loaded configurations
            const QString file = QString(":/resourcefiles/") + default_configurations[i].name + ".json";

            qDebug() << file.toUtf8().constData();

            Configuration *configuration = LoadConfiguration(file);
            if (configuration != nullptr) SaveConfiguration(configuration);
        }

        first_run_ = false;
        settings.setValue(VKCONFIG_KEY_FIRST_RUN, false);
    }

    // Get a list of all files that end in .json in the folder where
    // we store them. TBD... don't hard code this here.
    QDir dir(GetPath(ConfigurationPath));
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.json");
    QFileInfoList configuration_files = dir.entryInfoList();

    // Loop through all the configurations found and load them
    for (int i = 0, n = configuration_files.size(); i < n; i++) {
        QFileInfo info = configuration_files.at(i);
        if (info.absoluteFilePath().contains("applist.json")) continue;

        Configuration *configuration = LoadConfiguration(info.absoluteFilePath());
        qDebug() << ((configuration->name).toUtf8().constData());

        if (configuration != nullptr) {
            configuration->file = info.fileName();  // Easier than parsing it myself ;-)
            available_configurations.push_back(configuration);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Which of these profiles is currently active?
    active_configuration_ = nullptr;
    QString active_configuration_name = settings.value(VKCONFIG_KEY_ACTIVEPROFILE).toString();
    for (int i = 0; i < available_configurations.size(); i++) {
        if (available_configurations[i]->name == active_configuration_name) {
            active_configuration_ = available_configurations[i];
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// This function loads the (currently four) sets of profile settings into
// the defaults. These are all stored in layer_info.json
// 4/8/2020
void Configurator::LoadDefaultLayerSettings() {
    // Load the main object into the json document
    QFile file(":/resourcefiles/layer_info.json");
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();

    QJsonDocument json_layer_info_doc;
    json_layer_info_doc = QJsonDocument::fromJson(data.toLocal8Bit());
    if (!json_layer_info_doc.isObject()) return;

    // Isolate the Json object for each layer
    QJsonObject doc_object = json_layer_info_doc.object();
    QJsonValue layer_options_value = doc_object.value("layer_options");
    QJsonObject layers_options_object = layer_options_value.toObject();

    // This is a list of layers for which we have user editable settings.
    // there are nine as of this writing, but this code should accomodate
    // if more are added at a later time.
    // All the layers have been loaded, so we can look for matches
    // and let the layers parse the json data to create their own list
    // of settings.
    QStringList layers_with_settings = layers_options_object.keys();
    for (int i = 0; i < layers_with_settings.size(); i++) {  // For each setting
        LayerSettingsDefaults *settings_defaults = new LayerSettingsDefaults();
        settings_defaults->layer_name = layers_with_settings[i];

        // Save the name of the layer, and by default none are read only
        settings_defaults->layer_name = layers_with_settings[i];

        // Get the object for just this layer
        QJsonValue layerValue = layers_options_object.value(layers_with_settings[i]);
        QJsonObject layerObject = layerValue.toObject();

        LayerFile::LoadSettingsFromJson(layerObject, settings_defaults->default_settings);

        // Add to my list of layer settings
        default_layers_settings.push_back(settings_defaults);
    }
}

//////////////////////////////////////////////////////////////////////////////
/// To do a full match, not only the layer name, but the layer path/location
/// must also be a match. It IS possible to have two layers with the same name
/// as long as they are in different locations.
const LayerFile *Configurator::FindLayerNamed(QString layer_name) {
    for (int i = 0; i < available_Layers.size(); ++i) {
        const LayerFile *layer_file = available_Layers[i];

        if (!(layer_name == layer_file->name)) continue;
        return layer_file;
    }

    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// Load from a configuration file (.json really)
Configuration *Configurator::LoadConfiguration(const QString &path_to_configuration) {
    // Just load the name for now, and if it's read only
    if (path_to_configuration.isEmpty()) return nullptr;

    QFile file(path_to_configuration);
    bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);
    QString json_text = file.readAll();
    file.close();

    // Tease it apart, get the name of the profile
    QJsonDocument json_doc;
    QJsonParseError parse_error;
    json_doc = QJsonDocument::fromJson(json_text.toUtf8(), &parse_error);

    if (parse_error.error != QJsonParseError::NoError) return nullptr;

    // Allocate a new profile container
    Configuration *configuration = new Configuration();
    configuration->file = QFileInfo(path_to_configuration).fileName();

    QJsonObject json_top_object = json_doc.object();
    QStringList key = json_top_object.keys();

    // The file name overrides the stored name. Otherwise
    // we can end up with duplicate profiles
    configuration->name = configuration->file.left(configuration->file.length() - 5);

    QJsonValue configuration_entry_value = json_top_object.value(key[0]);
    QJsonObject configuration_entry_object = configuration_entry_value.toObject();

    QJsonValue excluded_value = configuration_entry_object.value("blacklisted_layers");
    QJsonArray excluded_array = excluded_value.toArray();
    for (int i = 0; i < excluded_array.size(); i++) configuration->excluded_layers << excluded_array[i].toString();

    QJsonValue preset_index = configuration_entry_object.value("preset");
    configuration->preset = static_cast<ValidationPreset>(preset_index.toInt());

    QJsonValue editorState = configuration_entry_object.value("editor_state");
    configuration->setting_tree_state = editorState.toVariant().toByteArray();

    QJsonValue description = configuration_entry_object.value("description");
    configuration->description = description.toString();

    QJsonValue options_value = configuration_entry_object.value("layer_options");

    QJsonObject layer_objects = options_value.toObject();
    QStringList layer_list = layer_objects.keys();

    // Build the list of layers with their settings
    if (layer_list.length() == 0) configuration->all_layers_available = false;

    for (int layer_index = 0; layer_index < layer_list.length(); layer_index++) {
        const LayerFile *layer_file = nullptr;
        QJsonValue layer_value = layer_objects.value(layer_list[layer_index]);
        QJsonObject layer_object = layer_value.toObject();

        // To match the layer we just need the name, paths are not
        // hard-matched to the configuration.
        // Find this in our lookup of layers. The standard layers are listed first
        layer_file = FindLayerNamed(layer_list[layer_index]);
        if (layer_file == nullptr) {  // If not found, we have a layer missing....
            configuration->all_layers_available = false;
            continue;
        }

        // Make a copy add it to this layer
        LayerFile *layer_copy = new LayerFile();
        layer_file->CopyLayer(layer_copy);
        configuration->layers.push_back(layer_copy);

        QJsonValue layerRank = layer_object.value("layer_rank");
        layer_copy->rank = layerRank.toInt();
        layer_copy->enabled = true;  // Always because it's present in the file

        // Load the layer
        LayerFile::LoadSettingsFromJson(layer_object, layer_copy->layer_settings);
    }

    // We need to sort the layers by their rank. The json sorts alphebetically and we
    // need to undo it.... A bubble quick sort is fine, it's a small list
    if (configuration->layers.size() > 1) {
        for (int i = 0; i < configuration->layers.size() - 1; i++) {
            for (int j = i + 1; j < configuration->layers.size(); j++) {
                if (configuration->layers[i]->rank > configuration->layers[j]->rank) {
                    LayerFile *temp = configuration->layers[i];
                    configuration->layers[i] = configuration->layers[j];
                    configuration->layers[j] = temp;
                }
            }
        }
    }

    return configuration;
}

/////////////////////////////////////////////////////////////////////////////////////
// This saves or resaves a configuration. Bear in mind it is called everytime
// any edit is made to a configuration at all.
bool Configurator::SaveConfiguration(Configuration *configuration) {
    // Build the json document
    QJsonArray excluded_list;
    for (int i = 0; i < configuration->excluded_layers.size(); i++) excluded_list.append(configuration->excluded_layers[i]);

    QJsonObject layer_list;  // This list of layers

    for (int layer_index = 0; layer_index < configuration->layers.size(); layer_index++) {
        LayerFile *pLayer = configuration->layers[layer_index];

        QJsonObject json_settings;

        // Rank goes in here with settings
        json_settings.insert("layer_rank", pLayer->rank);

        // Loop through the actual settings
        for (int setting_index = 0; setting_index < pLayer->layer_settings.size(); setting_index++) {
            QJsonObject setting;
            LayerSettings *layer_settings = pLayer->layer_settings[setting_index];

            setting.insert("name", layer_settings->settings_prompt);
            setting.insert("description", layer_settings->settings_desc);

            switch (layer_settings->settings_type) {
                case LAYER_SETTINGS_STRING:
                    setting.insert("type", "string");
                    setting.insert("default", layer_settings->settings_value);
                    break;

                case LAYER_SETTINGS_FILE:
                    setting.insert("type", "save_file");
                    setting.insert("default", layer_settings->settings_value);
                    break;

                case LAYER_SETTINGS_SAVE_FOLDER:
                    setting.insert("type", "save_folder");
                    setting.insert("default", layer_settings->settings_value);
                    break;

                case LAYER_SETTINGS_BOOL:
                    setting.insert("type", "bool");
                    setting.insert("default", layer_settings->settings_value);
                    break;

                case LAYER_SETTINGS_BOOL_NUMERIC:
                    setting.insert("type", "bool_numeric");
                    setting.insert("default", layer_settings->settings_value);
                    break;

                case LAYER_SETTINGS_VUID_FILTER:
                    setting.insert("type", "vuid_exclude");
                    setting.insert("default", layer_settings->settings_value);
                    break;

                case LAYER_SETTINGS_EXCLUSIVE_LIST: {
                    setting.insert("type", "enum");
                    setting.insert("default", layer_settings->settings_value);

                    QJsonObject options;
                    for (int i = 0; i < layer_settings->settings_list_exclusive_prompt.size(); i++)
                        options.insert(layer_settings->settings_list_exclusive_value[i],
                                       layer_settings->settings_list_exclusive_prompt[i]);
                    setting.insert("options", options);
                } break;

                case LAYER_SETTINGS_INCLUSIVE_LIST: {
                    setting.insert("type", "multi_enum");
                    QJsonObject options;
                    for (int i = 0; i < layer_settings->settings_list_inclusive_prompt.size(); i++)
                        options.insert(layer_settings->settings_list_inclusive_value[i],
                                       layer_settings->settings_list_inclusive_prompt[i]);
                    setting.insert("options", options);

                    QJsonArray defaults;
                    if (!layer_settings->settings_value.isEmpty()) {
                        QStringList list = layer_settings->settings_value.split(",");
                        for (int i = 0; i < list.size(); i++) defaults.append(list[i]);
                    }

                    setting.insert("default", defaults);
                } break;

                // There is a string field that is actually a complicted series of number or
                // ranges of numbers. We should at some point add this to allow more error free editing of it.
                case LAYER_SETTINGS_RANGE_INT:

                    break;

                // We missed somethhing
                default:
                    setting.insert("type", "unknown type");
                    setting.insert("default", "unknown data");
            }

            json_settings.insert(layer_settings->settings_name, setting);
        }

        layer_list.insert(pLayer->name, json_settings);
    }

    //////////////////////////////////////////////////////////
    // Assemble the json
    QJsonObject root;
    QJsonObject json_configuration;
    json_configuration.insert("blacklisted_layers", excluded_list);
    json_configuration.insert("description", configuration->description);
    json_configuration.insert("preset", configuration->preset);
    json_configuration.insert("editor_state", configuration->setting_tree_state.data());
    json_configuration.insert("layer_options", layer_list);
    root.insert(configuration->name, json_configuration);
    QJsonDocument doc(root);

    ///////////////////////////////////////////////////////////
    // Write it out - file name is same as name. If it's been
    // changed, this corrects the behavior.
    QString path_to_configuration = GetPath(ConfigurationPath);
    path_to_configuration += "/";
    path_to_configuration += configuration->name;
    path_to_configuration += QString(".json");

    QFile jsonFile(path_to_configuration);
    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox alert;
        alert.setText("Could not save configuration file!");
        alert.setWindowTitle(configuration->name);
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
        return false;
    }

    jsonFile.write(doc.toJson());
    jsonFile.close();
    return true;
}

///////////////////////////////////////////////////////////////////////////////
/// Create an empty profile definition that contains all available layers.
/// All settings are the default, and the layer order is just the order at
/// which they have come.
Configuration *Configurator::CreateEmptyConfiguration() {
    Configuration *new_configuration = new Configuration();

    LayerFile *temp_layer;
    int nRank = 0;

    // Add layers
    for (int i = 0; i < available_Layers.size(); i++) {
        temp_layer = new LayerFile();
        available_Layers[i]->CopyLayer(temp_layer);
        temp_layer->rank = nRank++;
        new_configuration->layers.push_back(temp_layer);
    }

    // Now grab settings defaults
    for (int i = 0; i < new_configuration->layers.size(); i++) LoadDefaultSettings(new_configuration->layers[i]);

    return new_configuration;
}

//////////////////////////////////////////////////////////////////////////////
/// Load the default settings into an empty layer file container
void Configurator::LoadDefaultSettings(LayerFile *pBlankLayer) {
    const LayerSettingsDefaults *layer_settings_defaults = FindLayerSettings(pBlankLayer->name);

    if (layer_settings_defaults == nullptr)  // Did we find any?
        return;

    // Create and pop them in....
    for (int s = 0; s < layer_settings_defaults->default_settings.size(); s++) {
        LayerSettings *layer_settings = new LayerSettings();
        *layer_settings = *layer_settings_defaults->default_settings[s];
        pBlankLayer->layer_settings.push_back(layer_settings);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Set this as the current override profile. The profile definition passed in
// is used to construct the override and settings files.
// Passing in nullptr IS valid, and will clear the current profile
void Configurator::SetActiveConfiguration(Configuration *configuration) {
    active_configuration_ = configuration;
    QSettings settings;

    const QString override_settings_path = GetPath(OverrideSettingsPath);
    const QString override_layers_path = GetPath(OverrideLayersPath);

    // Clear the profile if null
    if (configuration == nullptr) {
        // Delete a bunch of stuff
        remove(override_settings_path.toUtf8().constData());
        remove(override_layers_path.toUtf8().constData());

        // On Windows only, we need clear these values from the registry
        // This works without any Win32 specific functions for the registry
#ifdef _WIN32
        RemoveRegistryEntriesForLayers(override_layers_path, override_settings_path);  // Clear out the registry settings
#endif
        return;
    }

    /////////////////////////////////////////////
    // Now the fun starts, we need to write out the json file
    // that describes the layers being employed and the settings file

    // Save this as the last active profile (and we do NOT want to clear it when
    // no profile is made active.
    settings.setValue(VKCONFIG_KEY_ACTIVEPROFILE, active_configuration_->name);

    /////////////////////////
    // vk_layer_settings.txt
    QFile file(override_settings_path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);

    // Loop through all the layers
    for (int layer_index = 0; layer_index < configuration->layers.size(); layer_index++) {
        LayerFile *layer_file = configuration->layers[layer_index];
        stream << endl;
        stream << "# " << layer_file->name << endl;

        QString short_layer_name = layer_file->name;
        short_layer_name.remove("VK_LAYER_");
        QString lc_layer_name = short_layer_name.toLower();

        for (int setting_index = 0; setting_index < layer_file->layer_settings.size(); setting_index++) {
            LayerSettings *layer_settings = layer_file->layer_settings[setting_index];
            stream << lc_layer_name << "." << layer_settings->settings_name << " = " << layer_settings->settings_value << endl;

            // Temporary hack due to a gfxrecontruct bug for 2020 July SDK only. Remove after that release.
            if (lc_layer_name == QString("lunarg_gfxreconstruct"))
                stream << "lunarg_gfxrecon"
                       << "." << layer_settings->settings_name << " = " << layer_settings->settings_value << endl;
        }
    }
    file.close();

    ////////////////////////
    // VkLayer_override.json

    ///////////////////////////////////////////////////////////////////////
    // Paths are listed in the same order as the layers. Earlier, the value
    // of VK_LAYER_PATH was used to discover layers, then the user override
    // paths, then the standard paths, then the SDK path if the VULKAN_SDK
    // environment variable was set. All these paths must go in the
    // override_paths array, or the standard default system paths will be
    // used instead. A major departure from vkConfig1 is that now ALL
    // layer paths go in here.
    QStringList layer_override_paths;

    for (int i = 0; i < configuration->layers.size(); i++) {
        // Extract just the path
        QFileInfo file(configuration->layers[i]->layer_path);
        QString qsPath = QDir().toNativeSeparators(file.absolutePath());

        // Make sure the path is not already in the list
        if (layer_override_paths.contains(qsPath)) continue;

        // Okay, add to the list
        layer_override_paths << qsPath;
    }

    QJsonArray json_paths;
    for (int i = 0; i < layer_override_paths.count(); i++) json_paths.append(QDir::toNativeSeparators(layer_override_paths[i]));

    QJsonArray json_layers;
    for (int i = 0; i < configuration->layers.size(); i++) json_layers.append(configuration->layers[i]->name);

    QJsonArray json_excluded_layer_list;
    for (int i = 0; i < configuration->excluded_layers.size(); i++)
        json_excluded_layer_list.append(configuration->excluded_layers[i]);

    // Only supply this list if an app list is specified
    QJsonArray json_applist;
    for (int i = 0, n = overridden_application_list.size(); i < n; i++) {
        if (overridden_application_list[i]->override_layers) {
            json_applist.append(QDir::toNativeSeparators(overridden_application_list[i]->executable_path));
        }
    }

    QJsonObject disable;
    disable.insert("DISABLE_VK_LAYER_LUNARG_override", QString("1"));

    QJsonObject layer;
    layer.insert("name", QString("VK_LAYER_LUNARG_override"));
    layer.insert("type", QString("GLOBAL"));
    layer.insert("api_version", "1.2." + QString::number(VK_HEADER_VERSION));
    layer.insert("implementation_version", QString("1"));
    layer.insert("description", QString("LunarG Override Layer"));
    layer.insert("override_paths", json_paths);
    layer.insert("component_layers", json_layers);
    layer.insert("blacklisted_layers", json_excluded_layer_list);
    layer.insert("disable_environment", disable);

    // This has to contain something, or it will apply globally!
    if (overridden_application_list_only) {
        layer.insert("app_keys", json_applist);
    }

    QJsonObject root;
    root.insert("file_format_version", QJsonValue(QString("1.1.2")));
    root.insert("layer", layer);
    QJsonDocument doc(root);

    QFile jsonFile(override_layers_path);
    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) return;  // TBD, should we report an error
    jsonFile.write(doc.toJson());
    jsonFile.close();

    // On Windows only, we need to write these values to the registry
#ifdef _WIN32
    AddRegistryEntriesForLayers(override_layers_path, override_settings_path);
#endif
}

///////////////////////////////////////////////////////////////
/// Make a temporary copy of this configuration and activate it.
/// Any layer output settings need to be set to stderr
void Configurator::PushConfiguration(Configuration *new_configuration) {
    // Copy the working profile
    saved_configuration = active_configuration_;
    Configuration *copy = new_configuration->DuplicateConfiguration();
    copy->CollapseConfiguration();

    for (int layer_index = 0; layer_index < copy->layers.size(); layer_index++) {  // For each layer
        for (int setting_index = 0; setting_index < copy->layers[layer_index]->layer_settings.size(); setting_index++) {
            // Change to stdout if not already so it will get captured.
            if (copy->layers[layer_index]->layer_settings[setting_index]->settings_name == QString("log_filename"))
                copy->layers[layer_index]->layer_settings[setting_index]->settings_value = QString("stdout");

            // API Dump also has this setting
            if (copy->layers[layer_index]->layer_settings[setting_index]->settings_name == QString("file"))
                copy->layers[layer_index]->layer_settings[setting_index]->settings_value = QString("false");
        }
    }

    SetActiveConfiguration(copy);
}

/////////////////////////////////////////////////////////////
/// Restore the original working configuration
void Configurator::PopConfiguration() {
    delete GetActiveConfiguration();
    SetActiveConfiguration(saved_configuration);
    saved_configuration = nullptr;
}

void Configurator::ImportConfiguration(const QString &full_import_path) {
    QFile input(full_import_path);
    QString full_dest_name = GetPath(ConfigurationPath) + "/" + QFileInfo(full_import_path).fileName();

    if (!input.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("File Error");
        msg.setText("Cannot access the profile");
        msg.exec();
        return;
    }

    QFile output(full_dest_name);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("File Error");
        msg.setText("Cannot create the destination file.");
        msg.exec();
        return;
    }

    SetPath(Configurator::LastImportPath, full_import_path);

    QTextStream in(&input);
    QTextStream out(&output);

    while (!in.atEnd()) {
        QString line = in.readLine();
        out << line << "\n";
    }

    output.close();
    input.close();
    LoadAllConfigurations();
}

void Configurator::ExportConfiguration(const QString &source_file, const QString &full_export_path) {
    QFile input(GetPath(ConfigurationPath) + "/" + source_file);
    if (!input.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("File Error");
        msg.setText("Cannot access the configuration file.");
        msg.exec();
        return;
    }

    QFile output(full_export_path);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("File Error");
        msg.setText("Cannot create the destination file.");
        msg.exec();
        return;
    }

    SetPath(LastExportPath, full_export_path);

    QTextStream in(&input);
    QTextStream out(&output);

    while (!in.atEnd()) {
        QString line = in.readLine();
        out << line << "\n";
    }

    output.close();
    input.close();
}
