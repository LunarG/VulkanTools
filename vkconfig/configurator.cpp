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
#include "dlgcustompaths.h"

#include <Qt>
#include <QDir>
#include <QSettings>
#include <QTextStream>
#include <QLibrary>
#include <QMessageBox>
#include <QCheckBox>

#ifdef _WIN32
#include <shlobj_core.h>
#endif

#include <vulkan/vulkan.h>

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
        QFileInfoList fileInfoList = dir.entryInfoList(QStringList() << "*.json", QDir::Files);

        for (int iFile = 0; iFile < fileInfoList.size(); iFile++) file_list_ << fileInfoList[iFile].filePath();
    }
}

//////////////////////////////////////////////////////////////////////////////
// These are the built-in configurations that are pulled in from the resource
// file.
#ifdef __APPLE__
const int Configurator::DefaultConfigurationsCount = 6;
#else
const int Configurator::default_configurations_count = 8;
#endif

// If any new profiles are added, make sure the graphics reconstruct based config
// are last. Account for this in the number set for DefaultConfigurationCount
const char *Configurator::default_configurations[8] = {"Validation - Standard",
                                                      "Validation - Best Practices",
                                                      "Validation - GPU-Assisted",
                                                      "Validation - Shader Printf",
                                                      "Validation - Reduced-Overhead",
                                                      "API dump",
                                                      "Frame Capture - First two frames",
                                                      "Frame Capture - Range (F10 to start and to stop)"};

// I am purposly not flagging these as explicit or implicit as this can be parsed from the location
// and future updates to layer locations will only require a smaller change.
#ifdef _WIN32
const int nSearchPaths = 6;
const QString szSearchPaths[nSearchPaths] = {
    "HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ExplicitLayers",
    "HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ImplicitLayers",
    "HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ExplicitLayers",
    "HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ImplicitLayers",
    "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanExplicitLayers",
    "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanImplicitLayers"};
#else
const int nSearchPaths = 10;
const QString szSearchPaths[nSearchPaths] = {"/usr/local/etc/vulkan/explicit_layer.d",  // Not used on macOS, okay to just ignore
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
    static Configurator configuration;
    return configuration;
}

Configurator::Configurator() {
    available_Layers.reserve(10);
    active_configuration_ = nullptr;
    saved_configuration = nullptr;
    bHasOldLoader = false;
    first_run_ = true;

#ifdef _WIN32
    running_as_administrator_ = IsUserAnAdmin();
#else
    bRunningAsAdministrator = false;
#endif

    // Where is stuff
#ifdef _WIN32
    // Assemble the path name where the overide .json file goes
    QDir tempPath = QDir::temp();
    if (!tempPath.cd("VulkanLayerManager")) {
        tempPath.mkpath("VulkanLayerManager");
        tempPath.cd("VulkanLayerManager");
    }
    override_json_path = QDir::toNativeSeparators(tempPath.absoluteFilePath("VkLayer_override.json"));
    override_settings_path = QDir::toNativeSeparators(tempPath.absoluteFilePath("vk_layer_settings.txt"));

    QDir home = QDir::home();
    configuration_path_ = home.path() + QString("/AppData/Local/");
    home.setPath(configuration_path_);
    if (!home.cd("LunarG")) {
        home.mkpath("LunarG");
        home.cd("LunarG");
    }

    if (!home.cd("vkconfig")) home.mkpath("vkconfig");

    configuration_path_ += "LunarG/vkconfig";
    configuration_path_ = QDir::toNativeSeparators(configuration_path_);
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
    qsProfileFilesPath = home.path() + QString("/.local/share/vulkan/");  // TBD, where do profiles go if not here...
    qsOverrideSettingsPath = qsProfileFilesPath + "settings.d/vk_layer_settings.txt";
    qsOverrideJsonPath = qsProfileFilesPath + "implicit_layer.d/VkLayer_override.json";
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
                bHasOldLoader = true;
            }
        }
    }

    // Load simple app settings, the additional search paths, and the
    // override app list.
    LoadSettings();
    LoadCustomLayersPaths();
    LoadOverriddenApplicationList();
    LoadDefaultLayerSettings();  // findAllInstalledLayers uses the results of this.

    LoadAllInstalledLayers();

    // If no layers are found, give the user another chance to add some custom paths
    if (available_Layers.size() == 0) {
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
        if (available_Layers.size() == 0) return;
    }

    LoadAllConfigurations();

    // This will reset or clear the current profile if the files have been
    // manually manipulated
    SetActiveConfiguration(active_configuration_);
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
    QString searchPath = std::getenv("VULKAN_SDK");
    QFileInfo local(searchPath);
    if (local.exists())
        log += QString().asprintf("- SDK path: %s\n", searchPath.toUtf8().constData());
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
    assert(!err);

    std::vector<VkLayerProperties> layers_properties;
    layers_properties.resize(instance_layer_count);

    err = vkEnumerateInstanceLayerProperties(&instance_layer_count, &layers_properties[0]);
    assert(!err);

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
    assert(!err);

    std::vector<VkPhysicalDevice> devices;
    devices.resize(gpu_count);

    err = vkEnumeratePhysicalDevices(inst, &gpu_count, &devices[0]);
    assert(!err);

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
/// \brief CVulkanConfiguration::LoadDeviceRegistry
/// \param id
/// \param entry
/// \param layerList
/// \param type
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
/// \brief CVulkanConfiguration::AddRegistryEntriesForLayers
/// \param qsJSONFile
/// \param qsSettingsFile
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
/// \brief CVulkanConfiguration::AddRegistryEntriesForLayers
/// \param qsJSONFile
/// \param qsSettingsFile
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
    override_application_list_only = settings.value(VKCONFIG_KEY_APPLY_ONLY_TO_LIST).toBool();
    override_permanent = settings.value(VKCONFIG_KEY_KEEP_ACTIVE_ON_EXIT).toBool();
}

///////////////////////////////////////////////////////////////////////////////
/// This is for the local application settings, not the system Vulkan settings
void Configurator::SaveSettings() {
    QSettings settings;
    settings.setValue(VKCONFIG_KEY_LAUNCHAPP, active_launch_executable_path_);
    settings.setValue(VKCONFIG_KEY_OVERRIDE_ACTIVE, override_active);
    settings.setValue(VKCONFIG_KEY_APPLY_ONLY_TO_LIST, override_application_list_only);
    settings.setValue(VKCONFIG_KEY_KEEP_ACTIVE_ON_EXIT, override_permanent);
}

void Configurator::ResetToDefaultSettings() {
    QSettings settings;
    settings.setValue(VKCONFIG_KEY_LAUNCHAPP, "");
    settings.setValue(VKCONFIG_KEY_OVERRIDE_ACTIVE, true);
    settings.setValue(VKCONFIG_KEY_APPLY_ONLY_TO_LIST, false);
    settings.setValue(VKCONFIG_KEY_KEEP_ACTIVE_ON_EXIT, false);
}

/////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadAdditionalSearchPaths
/// We may have additional paths where we want to search for layers.
/// Load the list of paths here.
void Configurator::LoadCustomLayersPaths() {
    QSettings searchPaths;
    custom_layers_paths_ = searchPaths.value(VKCONFIG_KEY_CUSTOM_PATHS).toStringList();
}

/////////////////////////////////////////////////////////////////////////////
/// \brief saveAdditionalSearchPaths
/// We may have additional paths where we want to search for layers.
/// Save the list of paths here.
void Configurator::SaveCustomLayersPaths() {
    QSettings searchPaths;
    searchPaths.setValue(VKCONFIG_KEY_CUSTOM_PATHS, custom_layers_paths_);
}

void Configurator::RemoveCustomLayersPath(int path_index) {
    assert(path_index > 0 && path_index < custom_layers_paths_.size());

    custom_layers_paths_.removeAt(path_index);

    SaveCustomLayersPaths();
    LoadAllInstalledLayers();
    LoadAllConfigurations();
}

void Configurator::RemoveCustomLayersPath(const QString &path) {
    assert(!path.isEmpty());

    for (int i = 0; i < custom_layers_paths_.size(); ++i) {
        if (custom_layers_paths_[i] != path) continue;

        RemoveCustomLayersPath(i);
        break;
    }
}

void Configurator::AppendCustomLayersPath(const QString &path) {
    assert(!path.isEmpty());

    custom_layers_paths_.append(path);

    SaveCustomLayersPaths();
    LoadAllInstalledLayers();
    LoadAllConfigurations();
}

int Configurator::GetCustomLayersPathSize() const { return custom_layers_paths_.size(); }

const QString &Configurator::GetCustomLayersPath(int path_index) const {
    assert(path_index > 0 && path_index < custom_layers_paths_.size());

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

    assert(0);
    return -1;  // Not found
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
    QString appName("./vkcube.exe");
#endif

#ifdef __APPLE__
    QString appName("../../vkcube.app");
#endif

#ifdef __linux__
    QString appName("./vkcube");
#endif

    QString searchPath = appName;
    QFileInfo local(searchPath);
    if (!local.exists()) {
        searchPath = std::getenv("VULKAN_SDK");
        searchPath = searchPath + "/bin/" + appName;
        QFileInfo local2(searchPath);
        if (!local2.exists()) return;
        local = local2;
    }

    Application *appEntry = new Application;
    appEntry->working_folder = QDir::toNativeSeparators(local.absolutePath());
    appEntry->executable_path = QDir::toNativeSeparators(local.absoluteFilePath());
    appEntry->arguments = QString("--suppress_popups --validate");
    appEntry->override_layers = true;
    appEntry->log_file = "vkcube_out.txt";
    overridden_application_list.push_back(appEntry);
}

///////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadAppList
/// Load the custom application list. This is maintained as a json database
/// file.
void Configurator::LoadOverriddenApplicationList() {
    /////////////////////////////////////////////////////////////
    // Now, use the list
    QString appListJson = configuration_path_ + "/applist.json";
    QFile file(appListJson);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();

    QJsonDocument jsonAppList;
    jsonAppList = QJsonDocument::fromJson(data.toLocal8Bit());
    if (jsonAppList.isObject())
        if (!jsonAppList.isEmpty()) {
            // Get the list of apps
            QStringList appKeys;
            QJsonObject jsonDocObject = jsonAppList.object();
            appKeys = jsonDocObject.keys();

            // Get them...
            for (int i = 0; i < appKeys.length(); i++) {
                QJsonValue appValue = jsonDocObject.value(appKeys[i]);
                QJsonObject appObject = appValue.toObject();

                Application *appEntry = new Application;
                appEntry->working_folder = appObject.value("app_folder").toString();
                appEntry->executable_path = appObject.value("app_path").toString();
                appEntry->override_layers = !appObject.value("exclude_override").toBool();
                appEntry->log_file = appObject.value("log_file").toString();

                // Arguments are in an array to make room for adding more in a future version
                QJsonArray args = appObject.value("command_lines").toArray();
                appEntry->arguments = args[0].toString();

                overridden_application_list.push_back(appEntry);
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
/// \brief CVulkanConfiguration::saveAppList
/// Save the custom application list in a .json file
void Configurator::SaveOverriddenApplicationList() {
    QJsonObject root;

    for (int i = 0; i < overridden_application_list.size(); i++) {
        // Build an array of appnames with associated data
        QJsonObject applicationObject;
        applicationObject.insert("app_path", overridden_application_list[i]->executable_path);
        applicationObject.insert("app_folder", overridden_application_list[i]->working_folder);
        applicationObject.insert("exclude_override", !overridden_application_list[i]->override_layers);
        applicationObject.insert("log_file", overridden_application_list[i]->log_file);

        // Ground work for mulitiple sets of command line arguments
        QJsonArray argsArray;
        argsArray.append(QJsonValue(overridden_application_list[i]->arguments));  // [J] PROBABLY

        applicationObject.insert("command_lines", argsArray);
        root.insert(QFileInfo(overridden_application_list[i]->executable_path).fileName(), applicationObject);
    }

    QString appListJson = configuration_path_ + "/applist.json";
    QFile file(appListJson);
    file.open(QFile::WriteOnly);
    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();
}

///////////////////////////////////////////////////////////////////////////////
// Find all installed layers on the system.
void Configurator::LoadAllInstalledLayers() {
    // This is called initially, but also when custom search paths are set, so
    // we need to clear out the old data and just do a clean refresh
    ClearLayerLists();

    // Standard layer paths
    for (uint32_t i = 0; i < nSearchPaths; i++) {
        LayerType type = (szSearchPaths[i].contains("implicit", Qt::CaseInsensitive)) ? LAYER_TYPE_IMPLICIT : LAYER_TYPE_EXPLICIT;
        if (type == LAYER_TYPE_IMPLICIT)
            LoadLayersFromPath(szSearchPaths[i], available_Layers, type);
        else
            LoadLayersFromPath(szSearchPaths[i], available_Layers, type);
    }

#ifndef _WIN32
    // On Linux systems, the path might also be an extracted tar ball at just about any arbitrary place. Use the environment
    // variable VULKAN_SDK to look for additional layers.
    // (Go ahead and let macOS do this as well).
    // This will create duplicate entries if the layers ARE installed, so only do this if
    // No layers have been found yet.
    if (allLayers.size() == 0) {
        char *vulkanSDK = getenv("VULKAN_SDK");
        if (vulkanSDK != nullptr) {
            QString searchPath = vulkanSDK;
            searchPath += "/etc/vulkan/explicit_layer.d";
            LoadLayersFromPath(searchPath, allLayers, LAYER_TYPE_EXPLICIT);
        }
    }
#endif

    // Any custom paths? All layers from all paths are appended together here
    for (int i = 0; i < custom_layers_paths_.size(); i++)
        LoadLayersFromPath(custom_layers_paths_[i], available_Layers, LAYER_TYPE_CUSTOM);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadLayersFromPath
/// \param szPath
/// \param layerList
/// Search a folder and load up all the layers found there. This does NOT
/// load the default settings for each layer. This is just a master list of
/// settings to be copied from, and referenced.
void Configurator::LoadLayersFromPath(const QString &qsPath, QVector<LayerFile *> &layerList, LayerType type) {
    // On Windows custom files are in the file system. On non Windows all layers are
    // searched this way

#ifdef _WIN32
    if (qsPath.contains("...")) {
        LoadRegistryLayers(qsPath, layerList, type);
        return;
    }

    PathFinder fileList(qsPath, (type == LAYER_TYPE_CUSTOM));
#else
    // On Linux/Mac, we also need the home folder
    QString searchPath = qsPath;
    if (qsPath[0] == '.') {
        searchPath = QDir().homePath();
        searchPath += "/";
        searchPath += qsPath;
    }

    CPathFinder fileList(searchPath, true);
#endif
    if (fileList.FileCount() == 0) return;

    for (int iFile = 0; iFile < fileList.FileCount(); iFile++) {
        LayerFile *pLayerFile = new LayerFile();
        if (pLayerFile->ReadLayerFile(fileList.GetFileName(iFile), type)) {
            // Do not load VK_LAYER_LUNARG_override
            for (int i = 0; i < layerList.size(); i++)
                if (QString("VK_LAYER_LUNARG_override") == pLayerFile->name) {
                    delete pLayerFile;
                    pLayerFile = nullptr;
                    break;
                }

            // We have a layer! See if we need to add the settings list to it, and then add it to our list
            if (pLayerFile != nullptr) layerList.push_back(pLayerFile);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::findSettingsFor
/// \param layerName
/// \return
/// Find the settings for this named layer. If none found, return nullptr
const LayerSettingsDefaults *Configurator::FindLayerSettings(const QString& layer_name) const {
    for (int i = 0; i < default_layers_settings.size(); i++)
        if (layer_name == default_layers_settings[i]->layer_name) return default_layers_settings[i];

    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::FindConfiguration
/// Search the list of loaded profiles and return a pointer
Configuration *Configurator::FindConfiguration(const QString& configuration_name) const {
    for (int i = 0; i < available_configurations.size(); i++)
        if (available_configurations[i]->name == configuration_name) return available_configurations[i];
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadProfiles
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
        QDir dir(configuration_path_);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        dir.setNameFilters(QStringList() << "*.json");
        QFileInfoList profileFiles = dir.entryInfoList();

        // Loop through all the profiles found and remove them
        for (int iProfile = 0; iProfile < profileFiles.size(); iProfile++) {
            QFileInfo info = profileFiles.at(iProfile);
            if (info.absoluteFilePath().contains("applist.json")) continue;
            remove(info.filePath().toUtf8().constData());
        }

        for (int i = 0; i < default_configurations_count; i += 1) {
            // Search the list of loaded profiles
            QString qsFile = ":/resourcefiles/";
            qsFile += default_configurations[i];
            qsFile += ".json";
            Configuration *pProfile = LoadConfiguration(qsFile);
            if (pProfile != nullptr) SaveConfiguration(pProfile);
        }

        first_run_ = false;
        settings.setValue(VKCONFIG_KEY_FIRST_RUN, false);
    }

    // Get a list of all files that end in .json in the folder where
    // we store them. TBD... don't hard code this here.
    QDir dir(configuration_path_);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.json");
    QFileInfoList profileFiles = dir.entryInfoList();

    // Loop through all the profiles found and load them
    for (int iProfile = 0; iProfile < profileFiles.size(); iProfile++) {
        QFileInfo info = profileFiles.at(iProfile);
        if (info.absoluteFilePath().contains("applist.json")) continue;

        Configuration *pProfile = LoadConfiguration(info.absoluteFilePath());
        if (pProfile != nullptr) {
            pProfile->file = info.fileName();  // Easier than parsing it myself ;-)
            available_configurations.push_back(pProfile);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Which of these profiles is currently active?
    active_configuration_ = nullptr;
    QString qsActiveProfile = settings.value(VKCONFIG_KEY_ACTIVEPROFILE).toString();
    for (int i = 0; i < available_configurations.size(); i++)
        if (available_configurations[i]->name == qsActiveProfile) {
            active_configuration_ = available_configurations[i];
            break;
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

    QJsonDocument jsonlayerInfoDoc;
    jsonlayerInfoDoc = QJsonDocument::fromJson(data.toLocal8Bit());
    if (!jsonlayerInfoDoc.isObject()) return;

    // Isolate the Json object for each layer
    QJsonObject docObject = jsonlayerInfoDoc.object();
    QJsonValue layerOptionsValue = docObject.value("layer_options");
    QJsonObject layersOptionsObject = layerOptionsValue.toObject();

    // This is a list of layers for which we have user editable settings.
    // there are nine as of this writing, but this code should accomodate
    // if more are added at a later time.
    // All the layers have been loaded, so we can look for matches
    // and let the layers parse the json data to create their own list
    // of settings.
    QStringList layersWithSettings = layersOptionsObject.keys();
    for (int i = 0; i < layersWithSettings.size(); i++) {  // For each setting
        LayerSettingsDefaults *pDefaults = new LayerSettingsDefaults();
        pDefaults->layer_name = layersWithSettings[i];

        // Save the name of the layer, and by default none are read only
        pDefaults->layer_name = layersWithSettings[i];

        // Get the object for just this layer
        QJsonValue layerValue = layersOptionsObject.value(layersWithSettings[i]);
        QJsonObject layerObject = layerValue.toObject();

        LayerFile::LoadSettingsFromJson(layerObject, pDefaults->default_settings);

        // Add to my list of layer settings
        default_layers_settings.push_back(pDefaults);
    }
}

//////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::findLayerNamed
/// \param qsLayerName
/// \param location
/// \return
/// To do a full match, not only the layer name, but the layer path/location
/// must also be a match. It IS possible to have two layers with the same name
/// as long as they are in different locations.
const LayerFile *Configurator::FindLayerNamed(QString qsLayerName, const char *location) {
    // Search just by name
    if (location == nullptr) {
        for (int i = 0; i < available_Layers.size(); i++)
            if (qsLayerName == available_Layers[i]->name) {
                return available_Layers[i];
            }

        return nullptr;  // Not found
    }

    // Match both
    for (int i = 0; i < available_Layers.size(); i++)
        if (qsLayerName == available_Layers[i]->name && QString(location) == available_Layers[i]->qsLayerPath)
            return available_Layers[i];

    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// Load from a .profile file (.json really)
Configuration *Configurator::LoadConfiguration(const QString& path_to_configuration) {
    // Just load the name for now, and if it's read only
    if (path_to_configuration.isEmpty()) return nullptr;

    QFile file(path_to_configuration);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString json_text = file.readAll();
    file.close();

    // Tease it apart, get the name of the profile
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    jsonDoc = QJsonDocument::fromJson(json_text.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) return nullptr;

    // Allocate a new profile container
    Configuration *configuration = new Configuration();
    configuration->file = QFileInfo(path_to_configuration).fileName();

    QJsonObject json_top_object = jsonDoc.object();
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
    configuration->preset_index = preset_index.toInt();

    QJsonValue editorState = configuration_entry_object.value("editor_state");
    configuration->settingTreeState = editorState.toVariant().toByteArray();

    QJsonValue description = configuration_entry_object.value("description");
    configuration->description = description.toString();

    QJsonValue optionsValue = configuration_entry_object.value("layer_options");

    QJsonObject layerObjects = optionsValue.toObject();
    QStringList layerList = layerObjects.keys();

    // Build the list of layers with their settings
    if (layerList.length() == 0) configuration->all_layers_available = false;

    for (int iLayer = 0; iLayer < layerList.length(); iLayer++) {
        const LayerFile *pLayer = nullptr;
        QJsonValue layerValue = layerObjects.value(layerList[iLayer]);
        QJsonObject layerObject = layerValue.toObject();

        // To match the layer we need not just the name, but the path
        // If the path doesn't exist, assume it's from the standard layer locations
        // Get the path of this layer
        QJsonValue layerPathValue = layerObject.value("layer_path");
        QString layerPath = layerPathValue.toString();

        if (layerPath.isEmpty()) {  // No layer path exists
            // Find this in our lookup of layers. The standard layers are listed first
            pLayer = FindLayerNamed(layerList[iLayer]);
            if (pLayer == nullptr) {  // If not found, we have a layer missing....
                configuration->all_layers_available = false;
                continue;
            }
        } else {
            // We have a layer path, find the exact match. If an exact match doesn't
            // exist, allow just the name to match
            // (this fixes the problem with the API dump tool)
            // WAIT? Why both checking for path then?
            pLayer = FindLayerNamed(layerList[iLayer], layerPath.toUtf8().constData());
            if (pLayer == nullptr) {
                configuration->all_layers_available = false;
                continue;  // We still can't find the layer
            }
        }

        // Make a copy add it to this layer
        LayerFile *pProfileLayer = new LayerFile();
        pLayer->CopyLayer(pProfileLayer);
        configuration->layers.push_back(pProfileLayer);

        QJsonValue layerRank = layerObject.value("layer_rank");
        pProfileLayer->nRank = layerRank.toInt();
        pProfileLayer->bActive = true;  // Always because it's present in the file

        // Load the layer
        LayerFile::LoadSettingsFromJson(layerObject, pProfileLayer->layerSettings);
    }

    // We need to sort the layers by their rank. The json sorts alphebetically and we
    // need to undo it.... A bubble quick sort is fine, it's a small list
    if (configuration->layers.size() > 1)
        for (int i = 0; i < configuration->layers.size() - 1; i++)
            for (int j = i + 1; j < configuration->layers.size(); j++)
                if (configuration->layers[i]->nRank > configuration->layers[j]->nRank) {
                    LayerFile *pTemp = configuration->layers[i];
                    configuration->layers[i] = configuration->layers[j];
                    configuration->layers[j] = pTemp;
                }

    return configuration;
}

/////////////////////////////////////////////////////////////////////////////////////
// This saves or resaves a configuration. Bear in mind it is called everytime
// any edit is made to a configuration at all.
bool Configurator::SaveConfiguration(Configuration *pProfile) {
    // Build the json document
    QJsonArray blackList;
    for (int i = 0; i < pProfile->excluded_layers.size(); i++) blackList.append(pProfile->excluded_layers[i]);

    QJsonObject layerList;  // This list of layers

    for (int iLayer = 0; iLayer < pProfile->layers.size(); iLayer++) {
        LayerFile *pLayer = pProfile->layers[iLayer];

        QJsonObject jsonSettings;

        // Rank goes in here with settings
        jsonSettings.insert("layer_rank", pLayer->nRank);

        // We also need the path to the layer
        jsonSettings.insert("layer_path", pLayer->qsLayerPath);

        // Loop through the actual settings
        for (int iSetting = 0; iSetting < pLayer->layerSettings.size(); iSetting++) {
            QJsonObject setting;
            LayerSettings *pSettingsDetails = pLayer->layerSettings[iSetting];

            setting.insert("name", pSettingsDetails->settingsPrompt);
            setting.insert("description", pSettingsDetails->settingsDesc);

            switch (pSettingsDetails->settingsType) {
                case LAYER_SETTINGS_STRING:
                    setting.insert("type", "string");
                    setting.insert("default", pSettingsDetails->settingsValue);
                    break;

                case LAYER_SETTINGS_FILE:
                    setting.insert("type", "save_file");
                    setting.insert("default", pSettingsDetails->settingsValue);
                    break;

                case LAYER_SETTINGS_SAVE_FOLDER:
                    setting.insert("type", "save_folder");
                    setting.insert("default", pSettingsDetails->settingsValue);
                    break;

                case LAYER_SETTINGS_BOOL:
                    setting.insert("type", "bool");
                    setting.insert("default", pSettingsDetails->settingsValue);
                    break;

                case LAYER_SETTINGS_BOOL_NUMERIC:
                    setting.insert("type", "bool_numeric");
                    setting.insert("default", pSettingsDetails->settingsValue);
                    break;

                case LAYER_SETTINGS_VUID_FILTER:
                    setting.insert("type", "vuid_exclude");
                    setting.insert("default", pSettingsDetails->settingsValue);
                    break;

                case LAYER_SETTINGS_EXCLUSIVE_LIST: {
                    setting.insert("type", "enum");
                    setting.insert("default", pSettingsDetails->settingsValue);

                    QJsonObject options;
                    for (int i = 0; i < pSettingsDetails->settingsListExclusivePrompt.size(); i++)
                        options.insert(pSettingsDetails->settingsListExclusiveValue[i],
                                       pSettingsDetails->settingsListExclusivePrompt[i]);
                    setting.insert("options", options);
                } break;

                case LAYER_SETTINGS_INCLUSIVE_LIST: {
                    setting.insert("type", "multi_enum");
                    QJsonObject options;
                    for (int i = 0; i < pSettingsDetails->settingsListInclusivePrompt.size(); i++)
                        options.insert(pSettingsDetails->settingsListInclusiveValue[i],
                                       pSettingsDetails->settingsListInclusivePrompt[i]);
                    setting.insert("options", options);

                    QJsonArray defaults;
                    if (!pSettingsDetails->settingsValue.isEmpty()) {
                        QStringList list = pSettingsDetails->settingsValue.split(",");
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

            jsonSettings.insert(pSettingsDetails->settingsName, setting);
        }

        layerList.insert(pLayer->name, jsonSettings);
    }

    //////////////////////////////////////////////////////////
    // Assemble the json
    QJsonObject root;
    QJsonObject json_profile;
    json_profile.insert("blacklisted_layers", blackList);
    json_profile.insert("description", pProfile->description);
    json_profile.insert("preset", pProfile->preset_index);
    json_profile.insert("editor_state", pProfile->settingTreeState.data());
    json_profile.insert("layer_options", layerList);
    root.insert(pProfile->name, json_profile);
    QJsonDocument doc(root);

    ///////////////////////////////////////////////////////////
    // Write it out - file name is same as name. If it's been
    // changed, this corrects the behavior.
    QString pathToProfile = configuration_path_;
    pathToProfile += "/";
    pathToProfile += pProfile->name;
    pathToProfile += QString(".json");

    QFile jsonFile(pathToProfile);
    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox alert;
        alert.setText("Could not save configuration file!");
        alert.setWindowTitle(pProfile->name);
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
        return false;
    }

    jsonFile.write(doc.toJson());
    jsonFile.close();
    return true;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::CreateEmptyProfile
/// \return
/// Create an empty profile definition that contains all available layers.
/// All settings are the default, and the layer order is just the order at
/// which they have come.
Configuration *Configurator::CreateEmptyConfiguration() {
    Configuration *pNewProfile = new Configuration();

    LayerFile *pTempLayer;
    int nRank = 0;

    // Add layers
    for (int i = 0; i < available_Layers.size(); i++) {
        pTempLayer = new LayerFile();
        available_Layers[i]->CopyLayer(pTempLayer);
        pTempLayer->nRank = nRank++;
        pNewProfile->layers.push_back(pTempLayer);
    }

    // Now grab settings defaults
    for (int i = 0; i < pNewProfile->layers.size(); i++) LoadDefaultSettings(pNewProfile->layers[i]);

    return pNewProfile;
}

//////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::LoadDefaultSettings
/// \param pBlankLayer
/// Load the default settings into an empty layer file container
void Configurator::LoadDefaultSettings(LayerFile *pBlankLayer) {
    const LayerSettingsDefaults *pDefaults = FindLayerSettings(pBlankLayer->name);

    if (pDefaults == nullptr)  // Did we find any?
        return;

    // Create and pop them in....
    for (int s = 0; s < pDefaults->default_settings.size(); s++) {
        LayerSettings *pSetting = new LayerSettings();
        *pSetting = *pDefaults->default_settings[s];
        pBlankLayer->layerSettings.push_back(pSetting);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Set this as the current override profile. The profile definition passed in
// is used to construct the override and settings files.
// Passing in nullptr IS valid, and will clear the current profile
void Configurator::SetActiveConfiguration(Configuration *pProfile) {
    active_configuration_ = pProfile;
    QString profileName;
    QSettings settings;

    // Clear the profile if null
    if (pProfile == nullptr) {
        // Delete a bunch of stuff
        remove(override_settings_path.toUtf8().constData());
        remove(override_json_path.toUtf8().constData());

        // On Windows only, we need clear these values from the registry
        // This works without any Win32 specific functions for the registry
#ifdef _WIN32
        RemoveRegistryEntriesForLayers(override_json_path, override_settings_path);  // Clear out the registry settings
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
    for (int iLayer = 0; iLayer < pProfile->layers.size(); iLayer++) {
        LayerFile *pLayer = pProfile->layers[iLayer];
        stream << endl;
        stream << "# " << pLayer->name << endl;

        QString shortLayerName = pLayer->name;
        shortLayerName.remove("VK_LAYER_");
        QString lcLayerName = shortLayerName.toLower();

        for (int iSetting = 0; iSetting < pLayer->layerSettings.size(); iSetting++) {
            LayerSettings *pSetting = pLayer->layerSettings[iSetting];
            stream << lcLayerName << "." << pSetting->settingsName << " = " << pSetting->settingsValue << endl;

            // Temporary hack due to a gfxrecontruct bug for 2020 July SDK only. Remove after that release.
            if (lcLayerName == QString("lunarg_gfxreconstruct"))
                stream << "lunarg_gfxrecon"
                       << "." << pSetting->settingsName << " = " << pSetting->settingsValue << endl;
        }
    }
    file.close();

    ////////////////////////
    // VkLayer_override.json

    ///////////////////////////////////////////////////////////////////////
    // The paths are tricky... if no custom layers are being used, then
    // this is left empty. If any custom paths are used, then we need to make
    // sure if some of the standard layers are also selected that they
    // can also be found. So, we add their paths as well to the list
    QJsonArray json_paths;

    // See if any of the included layers are custom?
    bool bHaveCustom = false;
    for (int i = 0; i < pProfile->layers.size(); i++)
        if (pProfile->layers[i]->layerType == LAYER_TYPE_CUSTOM) bHaveCustom = true;

    // Only if we have custom paths...
    if (bHaveCustom) {
        // Don't use the additional search paths list, only use the paths
        // used by the layers themsevles. All the paths are included so
        // we can use standard SDK layers and custom layers at the same time.
        // The order of the search paths should match the order of the layers
        // in the profile.
        for (int i = 0; i < pProfile->layers.size(); i++) {
            // Extract just the path
            QFileInfo file(pProfile->layers[i]->qsLayerPath);
            QString qsPath = QDir().toNativeSeparators(file.absolutePath());

            // Okay, add to the list
            json_paths.append(qsPath);
        }
    }

    QJsonArray json_layers;
    for (int i = 0; i < pProfile->layers.size(); i++) json_layers.append(pProfile->layers[i]->name);

    QJsonArray json_blacklist;
    for (int i = 0; i < pProfile->excluded_layers.size(); i++) json_blacklist.append(pProfile->excluded_layers[i]);

    // Only supply this list if an app list is specified
    bool bHasAppList = false;
    QJsonArray json_applist;
    if (this->override_application_list_only) {
        for (int i = 0; i < overridden_application_list.size(); i++) {
            if (overridden_application_list[i]->override_layers) {
                json_applist.append(QDir::toNativeSeparators(overridden_application_list[i]->executable_path));
            } else {
                bHasAppList = true;
            }
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
    layer.insert("blacklisted_layers", json_blacklist);
    layer.insert("disable_environment", disable);

    // This has to contain something, or it will apply globally!
    if (!bHasAppList == false && this->override_application_list_only) json_applist.append("");

    layer.insert("app_keys", json_applist);

    QJsonObject root;
    root.insert("file_format_version", QJsonValue(QString("1.1.2")));
    root.insert("layer", layer);
    QJsonDocument doc(root);

    QFile jsonFile(override_json_path);
    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) return;  // TBD, should we report an error
    jsonFile.write(doc.toJson());
    jsonFile.close();

    // On Windows only, we need to write these values to the registry
#ifdef _WIN32
    AddRegistryEntriesForLayers(override_json_path, override_settings_path);
#endif
}

///////////////////////////////////////////////////////////////
/// Make a temporary copy of this configuration and activate it.
/// Any layer output settings need to be set to stderr
void Configurator::PushConfiguration(Configuration *pNew) {
    // Copy the working profile
    saved_configuration = active_configuration_;
    Configuration *pCopy = pNew->DuplicateProfile();
    pCopy->CollapseProfile();

    for (int iLayer = 0; iLayer < pCopy->layers.size(); iLayer++)  // For each layer
        for (int iSetting = 0; iSetting < pCopy->layers[iLayer]->layerSettings.size(); iSetting++) {
            // Change to stdout if not already so it will get captured.
            if (pCopy->layers[iLayer]->layerSettings[iSetting]->settingsName == QString("log_filename"))
                pCopy->layers[iLayer]->layerSettings[iSetting]->settingsValue = QString("stdout");

            // API Dump also has this setting
            if (pCopy->layers[iLayer]->layerSettings[iSetting]->settingsName == QString("file"))
                pCopy->layers[iLayer]->layerSettings[iSetting]->settingsValue = QString("false");
        }

    SetActiveConfiguration(pCopy);
}

/////////////////////////////////////////////////////////////
/// Restore the original working configuration
void Configurator::PopConfiguration() {
    delete GetActiveConfiguration();
    SetActiveConfiguration(saved_configuration);
    saved_configuration = nullptr;
}

void Configurator::ImportConfiguration(const QString& qsFullPathToSource) {
    QFile input(qsFullPathToSource);
    QString qsFullDestName = configuration_path_ + "/";
    qsFullDestName += QFileInfo(qsFullPathToSource).fileName();

    if (!input.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("File Error");
        msg.setText("Cannot access the profile");
        msg.exec();
        return;
    }

    QFile output(qsFullDestName);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("File Error");
        msg.setText("Cannot create the destination file.");
        msg.exec();
        return;
    }

    QTextStream in(&input);
    QTextStream out(&output);

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.contains("layer_path")) continue;

        out << line << "\n";
    }

    output.close();
    input.close();
    LoadAllConfigurations();
}

void Configurator::ExportConfiguration(const QString& qsFullPathToSource, const QString& qsFullPathToDest) const {
    QFile input(qsFullPathToSource);
    if (!input.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("File Error");
        msg.setText("Cannot access the configuration file.");
        msg.exec();
        return;
    }

    QFile output(qsFullPathToDest);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("File Error");
        msg.setText("Cannot create the destination file.");
        msg.exec();
        return;
    }

    QTextStream in(&input);
    QTextStream out(&output);

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.contains("layer_path")) continue;

        out << line << "\n";
    }

    output.close();
    input.close();
}
