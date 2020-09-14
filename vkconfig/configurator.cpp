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

#include "../vkconfig_core/version.h"
#include "../vkconfig_core/util.h"
#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/vulkan.h"

#include <Qt>
#include <QDir>
#include <QSettings>
#include <QLibrary>
#include <QTextStream>
#include <QMessageBox>
#include <QCheckBox>
#include <QJsonArray>

#if PLATFORM_WINDOWS
#include <shlobj.h>
#endif

#include <cassert>

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
    {"Validation - Reduced-Overhead", "VK_LAYER_KHRONOS_validation", Version("1.0.0"), "Reduced-Overhead",
     ValidationPresetReducedOverhead},
    {"Validation - Best Practices", "VK_LAYER_KHRONOS_validation", Version("1.1.126"), "Best Practices",
     ValidationPresetBestPractices},
    {"Validation - Synchronization (Alpha)", "VK_LAYER_KHRONOS_validation", Version("1.2.147"), "Synchronization (Alpha)",
     ValidationPresetSynchronization},
#if HAS_SHADER_BASED
    {"Validation - GPU-Assisted", "VK_LAYER_KHRONOS_validation", Version("1.1.126"), "GPU-Assisted", ValidationPresetGPUAssisted},
    {"Validation - Shader Printf", "VK_LAYER_KHRONOS_validation", Version("1.1.126"), "Debug Printf", ValidationPresetShaderPrintf},
#endif
#if !PLATFORM_MACOS
    {"Frame Capture - First two frames", "VK_LAYER_LUNARG_gfxreconstruct", Version("1.2.147"), "", ValidationPresetNone},
    {"Frame Capture - Range (F10 to start and to stop)", "VK_LAYER_LUNARG_gfxreconstruct", Version("1.2.147"), "",
     ValidationPresetNone},
#endif
    {"API dump", "VK_LAYER_LUNARG_api_dump", Version("1.1.126"), "", ValidationPresetNone}};

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
#if PLATFORM_WINDOWS
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
#endif

Configurator &Configurator::Get() {
    static Configurator configurator;
    return configurator;
}

Configurator::Configurator()
    : _active_configuration(nullptr),
      environment(path),
// Hack for GitHub C.I.
#if PLATFORM_WINDOWS && (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
      _running_as_administrator(IsUserAnAdmin())
#else
      _running_as_administrator(false)
#endif
{
    _available_Layers.reserve(10);

    // See if the VK_LAYER_PATH environment variable is set. If so, parse it and
    // assemble a list of paths that take precidence for layer discovery.
    QString layer_path = qgetenv("VK_LAYER_PATH");
    if (!layer_path.isEmpty()) {
#if PLATFORM_WINDOWS
        VK_LAYER_PATH = layer_path.split(";");  // Windows uses ; as seperator
#elif PLATFORM_LINUX || PLATFORM_MACOS
        VK_LAYER_PATH = layer_path.split(":");  // Linux/macOS uses : as seperator
#else
#error "Unknown platform"
#endif
    }
}

///////////////////////////////////////////////////////////////////////////////////
/// A good rule of C++ is to not put things in the constructor that can fail, or
/// that might require recursion. This initializes
///
bool Configurator::Init() {
    // Load simple app settings, the additional search paths, and the
    // override app list.
    LoadDefaultLayerSettings();  // findAllInstalledLayers uses the results of this.
    LoadAllInstalledLayers();

    // If no layers are found, give the user another chance to add some custom paths
    if (_available_Layers.empty()) {
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

    if (_available_Layers.empty()) {
        QMessageBox alert;
        alert.setWindowTitle(VKCONFIG_NAME);
        alert.setText("Could not initialize Vulkan Configurator.");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        return false;
    }

    LoadAllConfigurations();

    // This will reset or clear the current profile if the files have been
    // manually manipulated
    SetActiveConfiguration(_active_configuration);

    return true;
}

Configurator::~Configurator() {
    ClearLayerLists();
    qDeleteAll(_available_configurations.begin(), _available_configurations.end());
    _available_configurations.clear();
}

bool Configurator::HasLayers() const { return !_available_Layers.empty(); }

void Configurator::ClearLayerLists() {
    qDeleteAll(_available_Layers.begin(), _available_Layers.end());
    _available_Layers.clear();
}

#if PLATFORM_WINDOWS
///////////////////////////////////////////////////////////////////////
/// Look for device specific layers
void Configurator::LoadDeviceRegistry(DEVINST id, const QString &entry, QVector<Layer *> &layerList, LayerType type) {
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
            Layer *pLayerFile = new Layer();
            if (pLayerFile->Load(QString::fromWCharArray(curr_filename), type)) layerList.push_back(pLayerFile);
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
void Configurator::LoadRegistryLayers(const QString &path, QVector<Layer *> &layerList, LayerType type) {
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
    HKEY userKey = (_running_as_administrator) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;

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
    HKEY userKey = (_running_as_administrator) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;

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
#endif  // PLATFORM_WINDOWS

bool Configurator::SupportApplicationList(bool quiet, Version *return_loader_version) const {
    // Check loader version
    const Version version = GetVulkanLoaderVersion();
    assert(version != Version::VERSION_NULL);

    if (return_loader_version) {
        *return_loader_version = version;
    }

    // This is the minimum version that supports the application list
    if (version < Version("1.2.141") && !quiet) {
        const QString message = QString().asprintf(
            "The detected Vulkan Loader version is %s but version 1.2.141 or newer is required in order to apply layers "
            "override to only a selected list of Vulkan applications.\n\n<br><br>"
            "Get the latest Vulkan Runtime from <a href='https://vulkan.lunarg.com/sdk/home'>HERE.</a> to use this feature.",
            version.str().c_str());

        QMessageBox alert(NULL);
        alert.setWindowTitle("Layers override of a selected list of Vulkan Applications is not available");
        alert.setText(message);
        alert.setTextFormat(Qt::RichText);
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
        return false;
    }

    return true;
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
        for (int i = 0; i < lp; i++) LoadLayersFromPath(VK_LAYER_PATH[i], _available_Layers);

    // SECOND: Any custom paths? Search for those too
    const QStringList &custom_layers_paths = environment.GetCustomLayerPaths();
    for (int i = 0; i < custom_layers_paths.size(); i++) {
        LoadLayersFromPath(custom_layers_paths[i], _available_Layers);
    }

    // THIRD: Standard layer paths, in standard locations. The above has always taken precedence.
    for (std::size_t i = 0, n = countof(szSearchPaths); i < n; i++) {
        LoadLayersFromPath(szSearchPaths[i], _available_Layers);
    }

    // FOURTH: Finally, see if thee is anyting in the VULKAN_SDK path that wasn't already found elsewhere
    QString vulkanSDK = qgetenv("VULKAN_SDK");
    if (!vulkanSDK.isEmpty()) {
        vulkanSDK += "/etc/vulkan/explicit_layer.d";
        LoadLayersFromPath(vulkanSDK, _available_Layers);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Search a folder and load up all the layers found there. This does NOT
/// load the default settings for each layer. This is just a master list of
/// layers found. Do NOT load duplicate layer names. The type of layer (explicit or implicit) is
/// determined from the path name.
void Configurator::LoadLayersFromPath(const QString &path, QVector<Layer *> &layer_list) {
    // On Windows custom files are in the file system. On non Windows all layers are
    // searched this way
    LayerType type = LAYER_TYPE_CUSTOM;
    if (path.contains("explicit", Qt::CaseInsensitive)) type = LAYER_TYPE_EXPLICIT;

    if (path.contains("implicit", Qt::CaseInsensitive)) type = LAYER_TYPE_IMPLICIT;

#if PLATFORM_WINDOWS
    if (path.contains("...")) {
        LoadRegistryLayers(path, layer_list, type);
        return;
    }

    PathFinder file_list(path, (type == LAYER_TYPE_CUSTOM));
#elif PLATFORM_MACOS || PLATFORM_LINUX
    // On Linux/Mac, we also need the home folder
    QString search_path = path;
    if (path[0] == '.') {
        search_path = QDir().homePath();
        search_path += "/";
        search_path += path;
    }

    PathFinder file_list(search_path, true);
#else
#error Unknown platform
#endif
    if (file_list.FileCount() == 0) return;

    // We have a list of layer files. Add to the list as long as the layer name has
    // not already been added.
    for (int file_index = 0; file_index < file_list.FileCount(); file_index++) {
        Layer *layer_file = new Layer();
        if (layer_file->Load(file_list.GetFileName(file_index), type)) {
            // Do not load VK_LAYER_LUNARG_override
            for (int i = 0; i < layer_list.size(); i++)
                if (QString("VK_LAYER_LUNARG_override") == layer_file->_name) {
                    delete layer_file;
                    layer_file = nullptr;
                    break;
                }

            if (layer_file == nullptr) continue;

            // Make sure this layer name has not already been added
            if (FindLayerNamed(layer_file->_name)) {
                delete layer_file;
                continue;
            }

            // Good to go, add the layer
            layer_list.push_back(layer_file);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// Populate a tree widget with the custom layer paths and the layers that
// are being used in them.
void Configurator::BuildCustomLayerTree(QTreeWidget *tree_widget) {
    // Populate the tree
    tree_widget->clear();

    // Building the list is not obvious. Each custom path may have multiple layers and there
    // could be duplicates, which are not allowed. The layer paths are traversed in order, and
    // layers are used on a first occurance basis. So we can't just show the layers that are
    // present in the folder (because they may not be used). We have to list the custom layer paths
    // and then look for layers that are already loaded that are from that path.
    const QStringList &custom_layer_paths = environment.GetCustomLayerPaths();

    for (int custom_path_index = 0, n = custom_layer_paths.size(); custom_path_index < n; ++custom_path_index) {
        // Custom path is the parent tree item
        const QString &custom_path = QDir::toNativeSeparators(custom_layer_paths[custom_path_index]);

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, custom_path);
        tree_widget->addTopLevelItem(item);

        // Look for layers that are loaded that are also from this folder
        for (int i = 0, n = _available_Layers.size(); i < n; i++) {
            Layer *candidate = _available_Layers[i];

            QFileInfo fileInfo = candidate->_layer_path;
            QString path = QDir::toNativeSeparators(fileInfo.path());
            if (path != custom_path) continue;

            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, candidate->_name);
            item->addChild(child);
        }
        item->setExpanded(true);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Find the settings for this named layer. If none found, return nullptr
const LayerSettingsDefaults *Configurator::FindLayerSettings(const QString &layer_name) const {
    for (int i = 0; i < _default_layers_settings.size(); i++)
        if (layer_name == _default_layers_settings[i]->layer_name) return _default_layers_settings[i];

    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////
/// Search the list of loaded profiles and return a pointer
/// Note that this function is case insensitive since names are derived from file names
Configuration *Configurator::FindConfiguration(const QString &configuration_name) const {
    for (int i = 0, n = _available_configurations.size(); i < n; i++)
        if (configuration_name.compare(_available_configurations[i]->_name, Qt::CaseInsensitive) == 0)
            return _available_configurations[i];
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// Load all the  profiles. If the canned profiles don't exist,
/// they are created from the embedded json files
void Configurator::LoadAllConfigurations() {
    // This might be called to refresh the list...
    qDeleteAll(_available_configurations.begin(), _available_configurations.end());
    _available_configurations.clear();
    _active_configuration = nullptr;

    // //////////////////////////////////////////////////////////////////////////
    // If this is the first time, we need to create the initial set of
    // configuration files.
    if (environment.first_run) {
        // Delete all the *.json files in the storage folder
        QDir dir(path.GetPath(PATH_CONFIGURATION));
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

            Configuration configuration;
            const bool result = configuration.Load(file, _available_Layers);
            if (result) {
                const bool result = configuration.Save(path.GetFullPath(PATH_CONFIGURATION, configuration._name));
                assert(result);
            }
        }

        environment.first_run = false;
    }

    // Get a list of all files that end in .json in the folder where
    // we store them. TBD... don't hard code this here.
    QDir dir(path.GetPath(PATH_CONFIGURATION));
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.json");
    QFileInfoList configuration_files = dir.entryInfoList();

    // Loop through all the configurations found and load them
    for (int i = 0, n = configuration_files.size(); i < n; i++) {
        QFileInfo info = configuration_files.at(i);
        if (info.absoluteFilePath().contains("applist.json")) continue;

        Configuration *configuration = new Configuration;
        const bool result = configuration->Load(info.absoluteFilePath(), _available_Layers);
        if (result) {
            _available_configurations.push_back(configuration);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Which of these profiles is currently active?
    _active_configuration = nullptr;
    QSettings settings;
    const QString &active_configuration_name = environment.Get(ACTIVE_CONFIGURATION);
    for (int i = 0; i < _available_configurations.size(); i++) {
        if (_available_configurations[i]->_name == active_configuration_name) {
            _active_configuration = _available_configurations[i];
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

        ::LoadSettings(layerObject, settings_defaults->default_settings);

        // Add to my list of layer settings
        _default_layers_settings.push_back(settings_defaults);
    }
}

bool Configurator::IsLayerAvailable(const QString &layer_name) const {
    assert(!layer_name.isEmpty());

    for (int i = 0, n = _available_Layers.size(); i < n; ++i) {
        const Layer &layer = *_available_Layers[i];

        if (layer_name != layer._name) continue;

        assert(layer.IsValid());
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////
/// To do a full match, not only the layer name, but the layer path/location
/// must also be a match. It IS possible to have two layers with the same name
/// as long as they are in different locations.
Layer *Configurator::FindLayerNamed(QString layer_name) {
    assert(!layer_name.isEmpty());

    for (int i = 0; i < _available_Layers.size(); ++i) {
        Layer *layer = _available_Layers[i];

        if (!(layer_name == layer->_name)) continue;
        return layer;
    }

    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// Create an empty profile definition that contains all available layers.
/// All settings are the default, and the layer order is just the order at
/// which they have come.
Configuration *Configurator::CreateEmptyConfiguration() {
    Configuration *new_configuration = new Configuration();

    std::size_t named_new_count = 0;
    for (int i = 0, n = _available_configurations.size(); i < n; i++) {
        if (_available_configurations[i]->_name.startsWith("New Configuration")) ++named_new_count;
    }

    if (named_new_count > 0) {
        new_configuration->_name += format(" (%d)", named_new_count + 1).c_str();
    }

    return new_configuration;
}

//////////////////////////////////////////////////////////////////////////////
/// Load the default settings into an empty layer file container
void Configurator::LoadDefaultSettings(Layer *blank_layer) {
    assert(blank_layer);

    const LayerSettingsDefaults *layer_settings_defaults = FindLayerSettings(blank_layer->_name);

    if (layer_settings_defaults == nullptr)  // Did we find any?
        return;

    // Create and pop them in....
    for (int s = 0; s < layer_settings_defaults->default_settings.size(); s++) {
        const LayerSetting layer_setting = layer_settings_defaults->default_settings[s];
        assert(layer_setting == layer_settings_defaults->default_settings[s]);
        blank_layer->_layer_settings.push_back(layer_setting);
    }
}

void Configurator::SetActiveConfiguration(const QString &configuration_name) {
    assert(!configuration_name.isEmpty());

    Configuration *configuration = FindConfiguration(configuration_name);
    assert(configuration);

    SetActiveConfiguration(configuration);
}

///////////////////////////////////////////////////////////////////////////////
// Set this as the current override profile. The profile definition passed in
// is used to construct the override and settings files.
// Passing in nullptr IS valid, and will clear the current profile
void Configurator::SetActiveConfiguration(Configuration *active_configuration) {
    _active_configuration = active_configuration;
    QSettings settings;

    const QString override_settings_path = path.GetFullPath(PATH_OVERRIDE_SETTINGS);
    const QString override_layers_path = path.GetFullPath(PATH_OVERRIDE_LAYERS);

    bool need_remove_of_configuration_files = false;
    if (_active_configuration) {
        assert(!_active_configuration->_name.isEmpty());
        environment.Set(ACTIVE_CONFIGURATION, _active_configuration->_name);
        need_remove_of_configuration_files = _active_configuration->IsEmpty();
    } else {
        environment.Set(ACTIVE_CONFIGURATION, "");
        need_remove_of_configuration_files = true;
    }

    if (need_remove_of_configuration_files) {
        remove(override_settings_path.toUtf8().constData());
        remove(override_layers_path.toUtf8().constData());

        // On Windows only, we need clear these values from the registry
        // This works without any Win32 specific functions for the registry
#if PLATFORM_WINDOWS
        RemoveRegistryEntriesForLayers(override_layers_path, override_settings_path);  // Clear out the registry settings
#endif
        return;
    }

    /////////////////////////
    // vk_layer_settings.txt
    QFile file(override_settings_path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);

    // Loop through all the layers
    for (std::size_t j = 0, n = _active_configuration->parameters.size(); j < n; ++j) {
        const Parameter &parameter = _active_configuration->parameters[j];

        OutputDebugString("SetActiveConfiguration\n");
        OutputDebugString(_active_configuration->_name.toUtf8().constData());
        OutputDebugString("\n");
        OutputDebugString(parameter.name.toUtf8().constData());
        OutputDebugString("\n");

        const Layer *layer = FindLayerNamed(parameter.name);
        if (layer == nullptr) continue;

        if (parameter.state != LAYER_STATE_OVERRIDDEN) continue;

        stream << "\n";
        stream << "# " << layer->_name << "\n";

        QString short_layer_name = layer->_name;
        short_layer_name.remove("VK_LAYER_");
        QString lc_layer_name = short_layer_name.toLower();

        for (std::size_t i = 0, m = layer->_layer_settings.size(); i < m; ++i) {
            const LayerSetting &setting = layer->_layer_settings[i];

            if (layer->_name == "lunarg_gfxreconstruct" && layer->_api_version < Version("1.2.148")) {
                stream << "lunarg_gfxrecon"
                       << "." << setting.name << " = " << setting.value << "\n";
            } else {
                stream << lc_layer_name << "." << setting.name << " = " << setting.value << "\n";
            }
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
    for (std::size_t i = 0, n = _active_configuration->parameters.size(); i < n; ++i) {
        const Parameter &parameter = _active_configuration->parameters[i];

        if (parameter.state != LAYER_STATE_OVERRIDDEN) continue;

        const Layer *layer = FindLayerNamed(parameter.name);

        if (!layer) continue;

        // Extract just the path
        const QFileInfo file(layer->_layer_path);
        const QString absolute_path = QDir().toNativeSeparators(file.absolutePath());

        // Make sure the path is not already in the list
        if (layer_override_paths.contains(absolute_path)) continue;

        // Okay, add to the list
        layer_override_paths << absolute_path;
    }

    QJsonArray json_paths;
    for (int i = 0, n = layer_override_paths.count(); i < n; i++) {
        json_paths.append(QDir::toNativeSeparators(layer_override_paths[i]));
    }

    QJsonArray json_overridden_layers;
    QJsonArray json_excluded_layers;
    for (std::size_t i = 0, n = _active_configuration->parameters.size(); i < n; ++i) {
        const Parameter &parameter = _active_configuration->parameters[i];
        if (parameter.state == LAYER_STATE_OVERRIDDEN)
            json_overridden_layers.append(parameter.name);
        else if (parameter.state == LAYER_STATE_EXCLUDED)
            json_excluded_layers.append(parameter.name);
    }

    // Only supply this list if an app list is specified
    const std::vector<Application> &applications = environment.GetApplications();
    QJsonArray json_applist;
    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        if (applications[i].override_layers) {
            json_applist.append(QDir::toNativeSeparators(applications[i].executable_path));
        }
    }

    QJsonObject disable;
    disable.insert("DISABLE_VK_LAYER_LUNARG_override", "1");

    QJsonObject layer;
    layer.insert("name", "VK_LAYER_LUNARG_override");
    layer.insert("type", "GLOBAL");
    layer.insert("api_version", "1.2." + QString::number(VK_HEADER_VERSION));
    layer.insert("implementation_version", "1");
    layer.insert("description", "LunarG Override Layer");
    layer.insert("override_paths", json_paths);
    layer.insert("component_layers", json_overridden_layers);
    layer.insert("blacklisted_layers", json_excluded_layers);
    layer.insert("disable_environment", disable);

    // This has to contain something, or it will apply globally!
    if (environment.UseApplicationListOverrideMode()) {
        layer.insert("app_keys", json_applist);
    }

    QJsonObject root;
    root.insert("file_format_version", QJsonValue("1.1.2"));
    root.insert("layer", layer);
    QJsonDocument doc(root);

    QFile jsonFile(override_layers_path);
    const bool result = jsonFile.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result);
    jsonFile.write(doc.toJson());
    jsonFile.close();

    // On Windows only, we need to write these values to the registry
#if PLATFORM_WINDOWS
    AddRegistryEntriesForLayers(override_layers_path, override_settings_path);
#endif
}

void Configurator::ImportConfiguration(const QString &full_import_path) {
    assert(!full_import_path.isEmpty());

    Configuration configuration;

    if (!configuration.Load(full_import_path, _available_Layers)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Import of Layers Configuration error");
        msg.setText("Cannot access the source configuration file.");
        msg.setInformativeText(full_import_path);
        msg.exec();
        return;
    }

    configuration._name += " (Imported)";

    if (!configuration.Save(path.GetFullPath(PATH_CONFIGURATION, configuration._name))) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Import of Layers Configuration error");
        msg.setText("Cannot create the destination configuration file.");
        msg.setInformativeText(format("%s.json", configuration._name.toUtf8().constData()).c_str());
        msg.exec();
        return;
    }

    LoadAllConfigurations();
}

void Configurator::ExportConfiguration(const QString &source_file, const QString &full_export_path) {
    assert(!source_file.isEmpty());
    assert(!full_export_path.isEmpty());

    Configuration configuration;

    const QString source_path = path.GetFullPath(PATH_CONFIGURATION, source_file);

    if (!configuration.Load(source_path, _available_Layers)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Export of Layers Configuration error");
        msg.setText("Cannot access the source configuration file.");
        msg.setInformativeText(source_path);
        msg.exec();
        return;
    }

    if (!configuration.Save(full_export_path)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Export of Layers Configuration error");
        msg.setText("Cannot create the destination configuration file.");
        msg.setInformativeText(full_export_path);
        msg.exec();
        return;
    }
}

bool Configurator::HasMissingLayers(const Configuration &configuration) const {
    assert(&configuration);

    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        const Parameter &parameter = configuration.parameters[i];

        if (!IsLayerAvailable(parameter.name)) return false;
    }

    return true;
}
