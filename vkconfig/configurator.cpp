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

Application::Application(const QString &executable_full_path, const QString &arguments)
    : executable_path(QDir::toNativeSeparators(executable_full_path)),
      working_folder(QDir::toNativeSeparators(QFileInfo(executable_full_path).path())),
      arguments(arguments),
      log_file(
          QDir::toNativeSeparators(QDir::homePath() + QDir::separator() + QFileInfo(executable_full_path).baseName() + ".txt")),
      override_layers(true) {}

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
    {"Validation - Shader Printf", "VK_LAYER_KHRONOS_validation", Version("1.1.126"), "Debug Printf", ValidationPresetShaderPrintf},
    {"Validation - Reduced-Overhead", "VK_LAYER_KHRONOS_validation", Version("1.0.0"), "Reduced-Overhead",
     ValidationPresetReducedOverhead},
    {"Validation - Best Practices", "VK_LAYER_KHRONOS_validation", Version("1.1.126"), "Best Practices",
     ValidationPresetBestPractices},
    {"Validation - Synchronization (Alpha)", "VK_LAYER_KHRONOS_validation", Version("1.2.147"), "Synchronization (Alpha)",
     ValidationPresetSynchronization},
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
#endif

Configurator &Configurator::Get() {
    static Configurator configurator;
    return configurator;
}

Configurator::Configurator()
    : _active_configuration(nullptr),
// Hack for GitHub C.I.
#if PLATFORM_WINDOWS && (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
      _running_as_administrator(IsUserAnAdmin())
#else
      _running_as_administrator(false)
#endif
{
    _available_Layers.reserve(10);

// Where is stuff
#if PLATFORM_WINDOWS
    QDir home = QDir::home();
    QString main_path = home.path() + QString("/AppData/Local/");
    home.setPath(main_path);
    if (!home.cd("LunarG")) {
        home.mkpath("LunarG");
        home.cd("LunarG");
    }

    if (!home.cd("vkconfig")) {
        home.mkpath("vkconfig");
    }

    if (!home.cd("override")) {
        home.mkpath("override");
    }

    path.SetPath(PATH_CONFIGURATION, main_path + "LunarG/vkconfig");
    path.SetPath(PATH_OVERRIDE_LAYERS, main_path + "LunarG/vkconfig/override");
    path.SetPath(PATH_OVERRIDE_SETTINGS, main_path + "LunarG/vkconfig/override");
#elif PLATFORM_LINUX || PLATFORM_MACOS
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

    if (!home.cd("implicit_layer.d")) {
        home.mkpath("implicit_layer.d");
        home.cd("implicit_layer.d");
    }

    home.cd("..");
    if (!home.cd("settings.d")) {
        home.mkpath("settings.d");
        home.cd("settings.d");
    }

    home.cd("..");
    if (!home.cd("lunarg-vkconfig")) {
        home.mkpath("lunarg-vkconfig");
        home.cd("lunarg-vkconfig");
    }

    home = QDir::home();
    QString main_path = home.path() + QString("/.local/share/vulkan/");
    path.SetPath(PATH_CONFIGURATION, main_path + "lunarg-vkconfig/");
    path.SetPath(PATH_OVERRIDE_LAYERS, main_path + "implicit_layer.d");
    path.SetPath(PATH_OVERRIDE_SETTINGS, main_path + "settings.d");
#else
#error "Unknown platform"
#endif

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
    LoadOverriddenApplicationList();
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
        alert.setText("Could not initialize Vulkan Configurator.");
        alert.setWindowTitle(VKCONFIG_NAME);
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

void Configurator::SelectLaunchApplication(int application_index) {
    if (application_index < 0) return;

    environment.Set(ACTIVE_EXECUTABLE, _overridden_applications[application_index]->executable_path);
}

int Configurator::GetLaunchApplicationIndex() const {
    for (int i = 0; i < _overridden_applications.size(); i++) {
        if (_overridden_applications[i]->executable_path == environment.Get(ACTIVE_EXECUTABLE)) {
            return i;
        }
    }

    return 0;  // Not found, but the list is present, so return the first item.
}

static QString GetDefaultExecutablePath(const QString &executable_name) {
#if defined(__APPLE__)
    static const char *DEFAULT_PATH = "/../..";
#elif defined(__linux__) || defined(_WIN32)
    static const char *DEFAULT_PATH = "";
#else
#error "Unknown platform"
#endif

    // Using relative path to vkconfig
    {
        const QString search_path = QString("../bin") + DEFAULT_PATH + executable_name;
        QFileInfo file_info(search_path);
        if (file_info.exists())  // Couldn't find vkcube
            return file_info.filePath();
    }

    // Using VULKAN_SDK environement variable
    {
        const QString search_path = QString(qgetenv("VULKAN_SDK")) + "/bin" + DEFAULT_PATH + executable_name;
        QFileInfo file_info(search_path);
        if (file_info.exists())  // Couldn't find vkcube
            return file_info.absoluteFilePath();
    }

    return "";
}

/////////////////////////////////////////////////////////////////////////////
// Search for vkcube and add it to the app list.
void Configurator::UpdateDefaultApplications(const bool add_default_applications) {
#ifdef _WIN32
    static const char *SUFFIX = ".exe";
#elif defined(__APPLE__)
    static const char *SUFFIX = ".app";
#elif defined(__linux__)
    static const char *SUFFIX = "";
#else
#error "Unknown platform"
#endif

    struct Default {
        QString name;
        QString arguments;
    };

    static const Default defaults[] = {{QDir::toNativeSeparators("/vkcubepp"), "--suppress_popups"},
                                       {QDir::toNativeSeparators("/vkcube"), "--suppress_popups"}};

    for (std::size_t name_index = 0, name_count = countof(defaults); name_index < name_count; ++name_index) {
        bool add_default_application = add_default_applications;

        // This should only be called on first run, but make sure it's not already there.
        for (int i = 0; i < _overridden_applications.size(); ++i) {
            const Application &application = *_overridden_applications[i];

            if (!application.executable_path.endsWith(defaults[name_index].name + SUFFIX)) continue;

            // We found vkcube in the list, but does it exist?
            const QFileInfo file_info(application.executable_path);
            if (file_info.exists()) continue;

            add_default_application = true;
            _overridden_applications.remove(i);
        }

        if (add_default_application) {
            const QString executable_path = GetDefaultExecutablePath(defaults[name_index].name + SUFFIX);
            if (executable_path.isEmpty()) continue;  // application could not be found..

            Application application(executable_path, "--suppress_popups");

            // On all operating systems, but Windows we keep running into problems with this ending up
            // somewhere the user isn't allowed to create and write files. For consistncy sake, the log
            // initially will be set to the users home folder across all OS's. This is highly visible
            // in the application launcher and should not present a usability issue. The developer can
            // easily change this later to anywhere they like.
            application.log_file = path.GetPath(PATH_HOME) + defaults[name_index].name + ".txt";

            _overridden_applications.push_back(new Application(application));
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// Load the custom application list. This is maintained as a json database
/// file.
void Configurator::LoadOverriddenApplicationList() {
    /////////////////////////////////////////////////////////////
    // Now, use the list. If the file doesn't exist, this is not an error
    QString data;
    QString application_list_json = path.GetFullPath(FILENAME_APPLIST);
    QFile file(application_list_json);
    if (file.open(QFile::ReadOnly)) {
        data = file.readAll();
        file.close();
    }

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

                _overridden_applications.push_back(application);
            }
        }

    UpdateDefaultApplications(environment.first_run || _overridden_applications.isEmpty());
}

//////////////////////////////////////////////////////////////////////////
/// Save the custom application list in a .json file
void Configurator::SaveOverriddenApplicationList() {
    QJsonObject root;

    for (int i = 0; i < _overridden_applications.size(); i++) {
        // Build an array of appnames with associated data
        QJsonObject application_object;
        application_object.insert("app_path", _overridden_applications[i]->executable_path);
        application_object.insert("app_folder", _overridden_applications[i]->working_folder);
        application_object.insert("exclude_override", !_overridden_applications[i]->override_layers);
        application_object.insert("log_file", _overridden_applications[i]->log_file);

        // Ground work for mulitiple sets of command line arguments
        QJsonArray argsArray;
        argsArray.append(QJsonValue(_overridden_applications[i]->arguments));  // [J] PROBABLY

        application_object.insert("command_lines", argsArray);
        root.insert(QFileInfo(_overridden_applications[i]->executable_path).fileName(), application_object);
    }

    QString app_list_json = path.GetFullPath(FILENAME_APPLIST);
    QFile file(app_list_json);
    file.open(QFile::WriteOnly);
    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();
}

bool Configurator::HasActiveOverriddenApplications() const {
    for (int i = 0, n = _overridden_applications.size(); i < n; i++) {
        if (_overridden_applications[i]->override_layers) return true;
    }

    return false;
}

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
    for (std::size_t i = 0, n = countof(szSearchPaths); i < n; i++) LoadLayersFromPath(szSearchPaths[i], _available_Layers);

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
const Layer *Configurator::FindLayerNamed(QString layer_name) {
    assert(!layer_name.isEmpty());

    for (int i = 0; i < _available_Layers.size(); ++i) {
        const Layer *layer = _available_Layers[i];

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

    Layer *temp_layer;
    int rank = 0;

    // Add layers
    for (int i = 0, n = _available_Layers.size(); i < n; i++) {
        temp_layer = new Layer(*_available_Layers[i]);
        //_available_Layers[i]->CopyLayer(temp_layer);
        temp_layer->_rank = rank++;
        new_configuration->_layers.push_back(temp_layer);
    }

    // Now grab settings defaults
    for (int i = 0, n = new_configuration->_layers.size(); i < n; i++) LoadDefaultSettings(new_configuration->_layers[i]);

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

///////////////////////////////////////////////////////////////////////////////
// Set this as the current override profile. The profile definition passed in
// is used to construct the override and settings files.
// Passing in nullptr IS valid, and will clear the current profile
void Configurator::SetActiveConfiguration(Configuration *configuration) {
    _active_configuration = configuration;
    QSettings settings;

    const QString override_settings_path = path.GetFullPath(PATH_OVERRIDE_SETTINGS);
    const QString override_layers_path = path.GetFullPath(PATH_OVERRIDE_LAYERS);

    // Clear the profile if null
    if (configuration == nullptr) {
        environment.Set(ACTIVE_CONFIGURATION, "");

        // Delete a bunch of stuff
        remove(override_settings_path.toUtf8().constData());
        remove(override_layers_path.toUtf8().constData());

        // On Windows only, we need clear these values from the registry
        // This works without any Win32 specific functions for the registry
#if PLATFORM_WINDOWS
        RemoveRegistryEntriesForLayers(override_layers_path, override_settings_path);  // Clear out the registry settings
#endif
        return;
    }

    /////////////////////////////////////////////
    // Now the fun starts, we need to write out the json file
    // that describes the layers being employed and the settings file

    // Save this as the last active profile (and we do NOT want to clear it when
    // no profile is made active.
    environment.Set(ACTIVE_CONFIGURATION, _active_configuration->_name);

    /////////////////////////
    // vk_layer_settings.txt
    QFile file(override_settings_path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);

    // Loop through all the layers
    for (int layer_index = 0, layer_count = configuration->_layers.size(); layer_index < layer_count; layer_index++) {
        Layer *layer = configuration->_layers[layer_index];
        stream << "\n";
        stream << "# " << layer->_name << "\n";

        QString short_layer_name = layer->_name;
        short_layer_name.remove("VK_LAYER_");
        QString lc_layer_name = short_layer_name.toLower();

        for (std::size_t setting_index = 0, setting_count = layer->_layer_settings.size(); setting_index < setting_count;
             setting_index++) {
            const LayerSetting &layer_setting = layer->_layer_settings[setting_index];
            stream << lc_layer_name << "." << layer_setting.name << " = " << layer_setting.value << "\n";

            // Temporary hack due to a gfxrecontruct bug for 2020 July SDK only. Remove after that release.
            if (lc_layer_name == "lunarg_gfxreconstruct")
                stream << "lunarg_gfxrecon"
                       << "." << layer_setting.name << " = " << layer_setting.value << "\n";
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

    for (int i = 0; i < configuration->_layers.size(); i++) {
        // Extract just the path
        QFileInfo file(configuration->_layers[i]->_layer_path);
        QString qsPath = QDir().toNativeSeparators(file.absolutePath());

        // Make sure the path is not already in the list
        if (layer_override_paths.contains(qsPath)) continue;

        // Okay, add to the list
        layer_override_paths << qsPath;
    }

    QJsonArray json_paths;
    for (int i = 0; i < layer_override_paths.count(); i++) json_paths.append(QDir::toNativeSeparators(layer_override_paths[i]));

    QJsonArray json_layers;
    for (int i = 0; i < configuration->_layers.size(); i++) json_layers.append(configuration->_layers[i]->_name);

    QJsonArray json_excluded_layer_list;
    for (int i = 0; i < configuration->_excluded_layers.size(); i++)
        json_excluded_layer_list.append(configuration->_excluded_layers[i]);

    // Only supply this list if an app list is specified
    QJsonArray json_applist;
    for (int i = 0, n = _overridden_applications.size(); i < n; i++) {
        if (_overridden_applications[i]->override_layers) {
            json_applist.append(QDir::toNativeSeparators(_overridden_applications[i]->executable_path));
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
    if (environment.UseApplicationListOverrideMode()) {
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

bool Configurator::IsValid(const Configuration &configuration) const {
    if (configuration._excluded_layers.empty() && configuration._layers.empty()) {
        return false;
    }

    for (int i = 0, n = configuration._layers.size(); i < n; ++i) {
        if (!IsLayerAvailable(configuration._layers[i]->_name)) return false;
    }

    for (int i = 0, n = configuration._excluded_layers.size(); i < n; ++i) {
        if (!IsLayerAvailable(configuration._excluded_layers[i])) return false;
    }

    return true;
}
