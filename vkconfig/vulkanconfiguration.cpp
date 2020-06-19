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
 * The vkConfig2 program monitors and adjusts the Vulkan configuration
 * environment. These settings are wrapped in this class, which serves
 * as the "model" of the system.
 *
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 *  * Author: Lenny Komow <lenny@lunarg.com> (Win32 device registry trapsing).
 */

#include <QDir>
#include <QSettings>
#include <QTextStream>
#include <QLibrary>
#include <QMessageBox>

#include <vulkan/vulkan.h>

#include <profiledef.h>
#include "vulkanconfiguration.h"

//////////////////////////////////////////////////////////////////////////////
// Constructor does all the work. Abstracts away instances where we might
// be searching a disk path, or a registry path.
// TBD, does this really need it's own file/module?
CPathFinder::CPathFinder(const QString &qsPath, bool bForceFileSystem) {
    if (!bForceFileSystem) {
        QSettings files(qsPath, QSettings::NativeFormat);
        fileList = files.allKeys();
    } else {
        QDir dir(qsPath);
        QFileInfoList fileInfoList = dir.entryInfoList(QStringList() << "*.json", QDir::Files);

        for (int iFile = 0; iFile < fileInfoList.size(); iFile++) fileList << fileInfoList[iFile].filePath();
    }
}

//////////////////////////////////////////////////////////////////////////////
// These are the built-in configurations that are pulled in from the resource
// file.
int CVulkanConfiguration::nNumCannedProfiles = 8;
int CVulkanConfiguration::nNumKhronosPreConfigs = 5;
const char *CVulkanConfiguration::szCannedProfiles[8] = {
    "Validation - Standard",
    "Validation - Best Practices",
    "Validation - GPU-Assisted",
    "Validation - Shader Printf",
    "Validation - Reduced-Overhead",
    "API dump",
    "Frame Capture - First two frames",
    "Frame Capture - Range F10 to start and to stop"
};

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

// Single pointer to singleton configuration object
CVulkanConfiguration *CVulkanConfiguration::pMe = nullptr;

CVulkanConfiguration::CVulkanConfiguration() {
    allLayers.reserve(10);
    pActiveProfile = nullptr;
    pSavedProfile = nullptr;
    bHasOldLoader = false;
    bFirstRun = true;

    // Where is stuff
#ifdef _WIN32
    // Assemble the path name where the overide .json file goes
    QDir tempPath = QDir::temp();
    if (!tempPath.cd("VulkanLayerManager")) {
        tempPath.mkpath("VulkanLayerManager");
        tempPath.cd("VulkanLayerManager");
    }
    qsOverrideJsonPath = QDir::toNativeSeparators(tempPath.absoluteFilePath("VkLayer_override.json"));
    qsOverrideSettingsPath = QDir::toNativeSeparators(tempPath.absoluteFilePath("vk_layer_settings.txt"));

    QDir home = QDir::home();
    qsProfileFilesPath = home.path() + QString("/AppData/Local/");
    if (!home.cd("LunarG")) {
        home.mkpath("LunarG");
        home.cd("LunarG");
    }

    if (!home.cd("vkconfig")) home.mkpath("vkconfig");

    qsProfileFilesPath += "LunarG/vkconfig";
    qsProfileFilesPath = QDir::toNativeSeparators(qsProfileFilesPath);
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
        dlg.setText("Warning: Could not find a vulkan loader!");
        dlg.setIcon(QMessageBox::Warning);
        dlg.exec();
    } else {
        // Now is a good time to see if we have the old loader
        PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
        vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)library.resolve("vkEnumerateInstanceVersion");
        if (VK_SUCCESS == vkEnumerateInstanceVersion(&vulkanInstanceVersion)) {
            if (vulkanInstanceVersion < 4202633) {
                bHasOldLoader = true;
            }
        }
    }

    // Load simple app settings, the additional search paths, and the
    // override app list.
    LoadAppSettings();
    LoadAdditionalSearchPaths();
    LoadAppList();
    LoadDefaultLayerSettings();  // findAllInstalledLayers uses the results of this.
    FindAllInstalledLayers();
    LoadAllProfiles();

    // This will reset or clear the current profile if the files have been
    // manually manipulated
    SetCurrentActiveProfile(pActiveProfile);
}

CVulkanConfiguration::~CVulkanConfiguration() {
    ClearLayerLists();
    qDeleteAll(profileList.begin(), profileList.end());
    profileList.clear();
}

void CVulkanConfiguration::ClearLayerLists(void) {
    qDeleteAll(allLayers.begin(), allLayers.end());
    allLayers.clear();
}

#ifdef _WIN32
///////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::LoadDeviceRegistry
/// \param id
/// \param entry
/// \param layerList
/// \param type
/// Look for device specific layers
void CVulkanConfiguration::LoadDeviceRegistry(DEVINST id, const QString &entry, QVector<CLayerFile *> &layerList, TLayerType type) {
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
            CLayerFile *pLayerFile = new CLayerFile();
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
void CVulkanConfiguration::LoadRegistryLayers(const QString &path, QVector<CLayerFile *> &layerList, TLayerType type) {
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
void CVulkanConfiguration::AddRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile) {
    // Layer override json file
    HKEY key;
    REGSAM access = KEY_WRITE;
    LSTATUS err = RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers"), 0, NULL,
                                 REG_OPTION_NON_VOLATILE, access, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) return;

    QString file_path;
    DWORD value_count;
    DWORD value = 0;
    RegQueryInfoKey(key, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, NULL, NULL, NULL);
    RegSetValueExW(key, (LPCWSTR)qsJSONFile.utf16(), 0, REG_DWORD, (BYTE *)&value, sizeof(value));
    RegCloseKey(key);

    // Layer settings file
    err = RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Khronos\\Vulkan\\Settings"), 0, NULL, REG_OPTION_NON_VOLATILE, access,
                         NULL, &key, NULL);
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
void CVulkanConfiguration::RemoveRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile) {
    // Layer override json file
    HKEY key;
    REGSAM access = KEY_WRITE;
    LSTATUS err = RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers"), 0, NULL,
                                 REG_OPTION_NON_VOLATILE, access, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) return;

    RegDeleteValueW(key, (LPCWSTR)qsJSONFile.utf16());
    RegCloseKey(key);

    // Layer settings file
    err = RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Khronos\\Vulkan\\Settings"), 0, NULL, REG_OPTION_NON_VOLATILE, access,
                         NULL, &key, NULL);
    if (err != ERROR_SUCCESS) return;

    RegDeleteValueW(key, (LPCWSTR)qsSettingsFile.utf16());
    RegCloseKey(key);
}

#endif

///////////////////////////////////////////////////////////////////////////////
/// This is for the local application settings, not the system Vulkan settings
void CVulkanConfiguration::LoadAppSettings(void) {
    // Load the launch app name from the last session
    QSettings settings;
    qsLaunchApplicationWPath = settings.value(VKCONFIG_KEY_LAUNCHAPP).toString();
    qsLaunchApplicationArgs = settings.value(VKCONFIG_KEY_LAUNCHAPP_ARGS).toString();
    qsLaunchApplicationWorkingDir = settings.value(VKCONFIG_KEY_LAUNCHAPP_CWD).toString();
    qsLogFileWPath = settings.value(VKCONFIG_KEY_LOGFILE, QString("vkconfig_log.txt")).toString();
    bOverrideActive = settings.value(VKCONFIG_KEY_OVERRIDE_ACTIVE, true).toBool();
    bApplyOnlyToList = settings.value(VKCONFIG_KEY_APPLY_ONLY_TO_LIST).toBool();
    bKeepActiveOnExit = settings.value(VKCONFIG_KEY_KEEP_ACTIVE_ON_EXIT).toBool();
}

///////////////////////////////////////////////////////////////////////////////
/// This is for the local application settings, not the system Vulkan settings
void CVulkanConfiguration::SaveAppSettings(void) {
    QSettings settings;
    settings.setValue(VKCONFIG_KEY_LAUNCHAPP, qsLaunchApplicationWPath);
    settings.setValue(VKCONFIG_KEY_LAUNCHAPP_ARGS, qsLaunchApplicationArgs);
    settings.setValue(VKCONFIG_KEY_LAUNCHAPP_CWD, qsLaunchApplicationWorkingDir);
    settings.setValue(VKCONFIG_KEY_LOGFILE, qsLogFileWPath);
    settings.setValue(VKCONFIG_KEY_OVERRIDE_ACTIVE, bOverrideActive);
    settings.setValue(VKCONFIG_KEY_APPLY_ONLY_TO_LIST, bApplyOnlyToList);
    settings.setValue(VKCONFIG_KEY_KEEP_ACTIVE_ON_EXIT, bKeepActiveOnExit);
}

/////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadAdditionalSearchPaths
/// We may have additional paths where we want to search for layers.
/// Load the list of paths here.
void CVulkanConfiguration::LoadAdditionalSearchPaths(void) {
    QSettings searchPaths;
    additionalSearchPaths = searchPaths.value(VKCONFIG_KEY_CUSTOM_PATHS).toStringList();
}

/////////////////////////////////////////////////////////////////////////////
/// \brief saveAdditionalSearchPaths
/// We may have additional paths where we want to search for layers.
/// Save the list of paths here.
void CVulkanConfiguration::SaveAdditionalSearchPaths(void) {
    QSettings searchPaths;
    searchPaths.setValue(VKCONFIG_KEY_CUSTOM_PATHS, additionalSearchPaths);
}

/////////////////////////////////////////////////////////////////////////////
// Search for vkcube and add it to the app list.
void CVulkanConfiguration::FindVkCube(void) {
    // This should only be called on first run, but make sure it's not already there.
    if (appList.size() != 0)
        for (int i = 0; i < appList.size(); i++)
            if (appList[i]->qsAppNameWithPath.contains("vkcube")) return;

                // One of these must be true, or we just aren't going to compile!
#ifdef _WIN32
    QString appName("vkcube.exe");
#endif

#ifdef __APPLE__
    QString appName("vkcube.app");
#endif

#ifdef __linux__
    QString appName("vkcube");
#endif

    QString searchPath = "./" + appName;
    QFileInfo local(searchPath);
    if (!local.exists()) {
        searchPath = "../bin/" + appName;
        QFileInfo local2(searchPath);
        if (!local2.exists()) return;
        local = local2;
    }

    TAppListEntry *appEntry = new TAppListEntry;
    appEntry->qsWorkingFolder = local.absolutePath();
    appEntry->qsAppNameWithPath = local.absoluteFilePath();
    appEntry->bExcludeFromGlobalList = false;
    appList.push_back(appEntry);
}

///////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadAppList
/// Load the custom application list. This is maintained as a json database
/// file.
void CVulkanConfiguration::LoadAppList(void) {
    /////////////////////////////////////////////////////////////
    // Now, use the list
    QString appListJson = qsProfileFilesPath + "/applist.json";
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

                TAppListEntry *appEntry = new TAppListEntry;
                appEntry->qsWorkingFolder = appObject.value("app_folder").toString();
                appEntry->qsAppNameWithPath = appObject.value("app_path").toString();
                appEntry->bExcludeFromGlobalList = appObject.value("exclude_override").toBool();

                // Arguments are in an array to make room for adding more in a future version
                QJsonArray args = appObject.value("command_lines").toArray();
                appEntry->qsArguments = args[0].toString();

                appList.push_back(appEntry);
            }
        }

    //////////////////////////////////////////////
    // On first run, search for vkcube. Do this after this list
    // is loaded in case it's already there.
    QSettings settings;
    if (settings.value(VKCONFIG_KEY_FIRST_RUN, true).toBool()) FindVkCube();
}

//////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::saveAppList
/// Save the custom applicaiton list in a .json file
void CVulkanConfiguration::SaveAppList(void) {
    QJsonObject root;

    for (int i = 0; i < appList.size(); i++) {
        // Build an array of appnames with associated data
        QJsonObject applicationObject;
        applicationObject.insert("app_path", appList[i]->qsAppNameWithPath);
        applicationObject.insert("app_folder", appList[i]->qsWorkingFolder);
        applicationObject.insert("exclude_override", appList[i]->bExcludeFromGlobalList);

        // Ground work for mulitiple sets of command line arguments
        QJsonArray argsArray;
        argsArray.append(QJsonValue(appList[i]->qsArguments));  // [J] PROBABLY

        applicationObject.insert("command_lines", argsArray);
        root.insert(QFileInfo(appList[i]->qsAppNameWithPath).fileName(), applicationObject);
    }

    QString appListJson = qsProfileFilesPath + "/applist.json";
    QFile file(appListJson);
    file.open(QFile::WriteOnly);
    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();
}

///////////////////////////////////////////////////////////////////////////////
// Find all installed layers on the system.
void CVulkanConfiguration::FindAllInstalledLayers(void) {
    // This is called initially, but also when custom search paths are set, so
    // we need to clear out the old data and just do a clean refresh
    ClearLayerLists();

    // Standard layer paths
    for (uint32_t i = 0; i < nSearchPaths; i++) {
        TLayerType type = (szSearchPaths[i].contains("implicit", Qt::CaseInsensitive)) ? LAYER_TYPE_IMPLICIT : LAYER_TYPE_EXPLICIT;
        if (type == LAYER_TYPE_IMPLICIT)
            LoadLayersFromPath(szSearchPaths[i], allLayers, type);
        else
            LoadLayersFromPath(szSearchPaths[i], allLayers, type);
    }

    // Any custom paths? All layers from all paths are appended together here
    for (int i = 0; i < additionalSearchPaths.size(); i++)
        LoadLayersFromPath(additionalSearchPaths[i], allLayers, LAYER_TYPE_CUSTOM);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadLayersFromPath
/// \param szPath
/// \param layerList
/// Search a folder and load up all the layers found there. This does NOT
/// load the default settings for each layer. This is just a master list of
/// settings to be copied from, and referenced.
void CVulkanConfiguration::LoadLayersFromPath(const QString &qsPath, QVector<CLayerFile *> &layerList, TLayerType type) {
    // On Windows custom files are in the file system. On non Windows all layers are
    // searched this way
#ifdef _WIN32
    if (qsPath.contains("...")) {
        LoadRegistryLayers(qsPath, layerList, type);
        return;
    }

    CPathFinder fileList(qsPath, (type == LAYER_TYPE_CUSTOM));
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
        CLayerFile *pLayerFile = new CLayerFile();
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
const LayerSettingsDefaults *CVulkanConfiguration::FindSettingsFor(QString layerName) {
    for (int i = 0; i < defaultLayerSettings.size(); i++)
        if (layerName == defaultLayerSettings[i]->layerName) return defaultLayerSettings[i];

    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::FindProfile
/// \param profileName
/// \return
/// Search the list of loaded profiles and return a pointer
CProfileDef *CVulkanConfiguration::FindProfile(QString profileName) {
    for (int i = 0; i < profileList.size(); i++)
        if (profileList[i]->qsProfileName == profileName) return profileList[i];

    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadProfiles
/// Load all the  profiles. If the canned profiles don't exist,
/// they are created from the embedded json files
void CVulkanConfiguration::LoadAllProfiles(void) {
    // This might be called to refresh the list...
    qDeleteAll(profileList.begin(), profileList.end());
    profileList.clear();
    pActiveProfile = nullptr;

    // //////////////////////////////////////////////////////////////////////////
    // If this is the first time, we need to create the initial set of
    // configuration files.
    QSettings settings;
    bFirstRun = settings.value(VKCONFIG_KEY_FIRST_RUN, true).toBool();
    if (bFirstRun) {
        for (int i = 0; i < nNumCannedProfiles; i += 1) {
            // Search the list of loaded profiles
            QString qsFile = ":/resourcefiles/";
            qsFile += szCannedProfiles[i];
            qsFile += ".json";
            CProfileDef *pProfile = LoadProfile(qsFile);
            if (pProfile != nullptr) SaveProfile(pProfile);
        }

        bFirstRun = false;
        settings.setValue(VKCONFIG_KEY_FIRST_RUN, false);
    }

    // Get a list of all files that end in .json in the folder where
    // we store them. TBD... don't hard code this here.
    QDir dir(qsProfileFilesPath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.json");
    QFileInfoList profileFiles = dir.entryInfoList();

    // Loop through all the profiles found and load them
    for (int iProfile = 0; iProfile < profileFiles.size(); iProfile++) {
        QFileInfo info = profileFiles.at(iProfile);
        if (info.absoluteFilePath().contains("applist.json")) continue;

        CProfileDef *pProfile = LoadProfile(info.absoluteFilePath());
        if (pProfile != nullptr) {
            pProfile->qsFileName = info.fileName();  // Easier than parsing it myself ;-)
            profileList.push_back(pProfile);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Which of these profiles is currently active?
    pActiveProfile = nullptr;
    QString qsActiveProfile = settings.value(VKCONFIG_KEY_ACTIVEPROFILE).toString();
    for (int i = 0; i < profileList.size(); i++)
        if (profileList[i]->qsProfileName == qsActiveProfile) {
            pActiveProfile = profileList[i];
            break;
        }
}

//////////////////////////////////////////////////////////////////////////////
// This function loads the (currently four) sets of profile settings into
// the defaults. These are all stored in layer_info.json
// 4/8/2020
void CVulkanConfiguration::LoadDefaultLayerSettings(void) {
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
        pDefaults->layerName = layersWithSettings[i];

        // Save the name of the layer, and by default none are read only
        pDefaults->layerName = layersWithSettings[i];

        // Get the object for just this layer
        QJsonValue layerValue = layersOptionsObject.value(layersWithSettings[i]);
        QJsonObject layerObject = layerValue.toObject();

        CLayerFile::LoadSettingsFromJson(layerObject, pDefaults->defaultSettings);

        // Add to my list of layer settings
        defaultLayerSettings.push_back(pDefaults);
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
const CLayerFile *CVulkanConfiguration::FindLayerNamed(QString qsLayerName, const char *location) {
    // Search just by name
    if (location == nullptr) {
        for (int i = 0; i < allLayers.size(); i++)
            if (qsLayerName == allLayers[i]->name) {
                return allLayers[i];
            }

        return nullptr;  // Not found
    }

    // Match both
    for (int i = 0; i < allLayers.size(); i++)
        if (qsLayerName == allLayers[i]->name && QString(location) == allLayers[i]->qsLayerPath) return allLayers[i];

    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// Load from a .profile file (.json really)
CProfileDef *CVulkanConfiguration::LoadProfile(QString pathToProfile) {
    // Just load the name for now, and if it's read only
    if (pathToProfile.isEmpty()) return nullptr;

    QFile file(pathToProfile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString jsonText = file.readAll();
    file.close();

    // Tease it apart, get the name of the profile
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    jsonDoc = QJsonDocument::fromJson(jsonText.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) return nullptr;

    // Allocate a new profile container
    CProfileDef *pProfile = new CProfileDef();
    pProfile->qsFileName = QFileInfo(pathToProfile).fileName();

    QJsonObject jsonTopObject = jsonDoc.object();
    QStringList key = jsonTopObject.keys();

    // The file name overrides the stored name. Otherwise
    // we can end up with duplicate profiles
    pProfile->qsProfileName = pProfile->qsFileName.left(pProfile->qsFileName.length() - 5);

    QJsonValue profileEntryValue = jsonTopObject.value(key[0]);
    QJsonObject profileEntryObject = profileEntryValue.toObject();

    QJsonValue blackListValue = profileEntryObject.value("blacklisted_layers");
    QJsonArray blackListArray = blackListValue.toArray();
    for (int i = 0; i < blackListArray.size(); i++) pProfile->blacklistedLayers << blackListArray[i].toString();

    QJsonValue presetIndex = profileEntryObject.value("preset");
    pProfile->nPresetIndex = presetIndex.toInt();

    QJsonValue editorState = profileEntryObject.value("editor_state");
    pProfile->settingTreeState = editorState.toVariant().toByteArray();

    QJsonValue description = profileEntryObject.value("description");
    pProfile->qsDescription = description.toString();

    QJsonValue optionsValue = profileEntryObject.value("layer_options");

    QJsonObject layerObjects = optionsValue.toObject();
    QStringList layerList = layerObjects.keys();

    // Build the list of layers with their settings
    for (int iLayer = 0; iLayer < layerList.length(); iLayer++) {
        const CLayerFile *pLayer = nullptr;
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
            if (pLayer == nullptr)  // If not found, we have a layer missing....
                continue;
        } else {
            // We have a layer path, find the exact match. If an exact match doesn't
            // exist, allow just the name to match
            // (this fixes the problem with the API dump tool)
            // WAIT? Why both checking for path then?
            pLayer = FindLayerNamed(layerList[iLayer], layerPath.toUtf8().constData());
            if (pLayer == nullptr) continue;  // We still can't find the layer
        }

        // Make a copy add it to this layer
        CLayerFile *pProfileLayer = new CLayerFile();
        pLayer->CopyLayer(pProfileLayer);
        pProfile->layers.push_back(pProfileLayer);

        QJsonValue layerRank = layerObject.value("layer_rank");
        pProfileLayer->nRank = layerRank.toInt();
        pProfileLayer->bActive = true;  // Always because it's present in the file

        // Load the layer
        CLayerFile::LoadSettingsFromJson(layerObject, pProfileLayer->layerSettings);
    }

    // We need to sort the layers by their rank. The json sorts alphebetically and we
    // need to undo it.... A bubble quick sort is fine, it's a small list
    if (pProfile->layers.size() > 1)
        for (int i = 0; i < pProfile->layers.size() - 1; i++)
            for (int j = i + 1; j < pProfile->layers.size(); j++)
                if (pProfile->layers[i]->nRank > pProfile->layers[j]->nRank) {
                    CLayerFile *pTemp = pProfile->layers[i];
                    pProfile->layers[i] = pProfile->layers[j];
                    pProfile->layers[j] = pTemp;
                }

    return pProfile;
}

/////////////////////////////////////////////////////////////////////////////////////
void CVulkanConfiguration::SaveProfile(CProfileDef *pProfile) {
    // Build the json document
    QJsonArray blackList;
    for (int i = 0; i < pProfile->blacklistedLayers.size(); i++) blackList.append(pProfile->blacklistedLayers[i]);

    QJsonObject layerList;  // This list of layers

    for (int iLayer = 0; iLayer < pProfile->layers.size(); iLayer++) {
        CLayerFile *pLayer = pProfile->layers[iLayer];

        QJsonObject jsonSettings;

        // Rank goes in here with settings
        jsonSettings.insert("layer_rank", pLayer->nRank);

        // We also need the path to the layer
        jsonSettings.insert("layer_path", pLayer->qsLayerPath);

        // Loop through the actual settings
        for (int iSetting = 0; iSetting < pLayer->layerSettings.size(); iSetting++) {
            QJsonObject setting;
            TLayerSettings *pSettingsDetails = pLayer->layerSettings[iSetting];

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
    json_profile.insert("description", pProfile->qsDescription);
    json_profile.insert("preset", pProfile->nPresetIndex);
    json_profile.insert("editor_state", pProfile->settingTreeState.data());
    json_profile.insert("layer_options", layerList);
    root.insert(pProfile->qsProfileName, json_profile);
    QJsonDocument doc(root);

    ///////////////////////////////////////////////////////////
    // Write it out - file name is same as name. If it's been
    // changed, this corrects the behavior.
    QString pathToProfile = qsProfileFilesPath;
    pathToProfile += "/";
    pathToProfile += pProfile->qsProfileName;
    pathToProfile += QString(".json");

    QFile jsonFile(pathToProfile);
    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) return;  // TBD, should we report an error
    jsonFile.write(doc.toJson());
    jsonFile.close();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::CreateEmptyProfile
/// \return
/// Create an empty profile definition that contains all available layers.
/// All settings are the default, and the layer order is just the order at
/// which they have come.
CProfileDef *CVulkanConfiguration::CreateEmptyProfile() {
    CProfileDef *pNewProfile = new CProfileDef();

    CLayerFile *pTempLayer;
    int nRank = 0;

    // Add layers
    for (int i = 0; i < allLayers.size(); i++) {
        pTempLayer = new CLayerFile();
        allLayers[i]->CopyLayer(pTempLayer);
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
void CVulkanConfiguration::LoadDefaultSettings(CLayerFile *pBlankLayer) {
    const LayerSettingsDefaults *pDefaults = FindSettingsFor(pBlankLayer->name);

    if (pDefaults == nullptr)  // Did we find any?
        return;

    // Create and pop them in....
    for (int s = 0; s < pDefaults->defaultSettings.size(); s++) {
        TLayerSettings *pSetting = new TLayerSettings();
        *pSetting = *pDefaults->defaultSettings[s];
        pBlankLayer->layerSettings.push_back(pSetting);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Set this as the current override profile. The profile definition passed in
// is used to construct the override and settings files.
// Passing in nullptr IS valid, and will clear the current profile
void CVulkanConfiguration::SetCurrentActiveProfile(CProfileDef *pProfile) {
    pActiveProfile = pProfile;
    QString profileName;
    QSettings settings;

    // Clear the profile if null
    if (pProfile == nullptr) {
        // Delete a bunch of stuff
        remove(qsOverrideSettingsPath.toUtf8().constData());
        remove(qsOverrideJsonPath.toUtf8().constData());

        // On Windows only, we need clear these values from the registry
        // This works without any Win32 specific functions for the registry
#ifdef _WIN32
        RemoveRegistryEntriesForLayers(qsOverrideJsonPath, qsOverrideSettingsPath);  // Clear out the registry settings
#endif
        return;
    }

    /////////////////////////////////////////////
    // Now the fun starts, we need to write out the json file
    // that describes the layers being employed and the settings file

    // Save this as the last active profile (and we do NOT want to clear it when
    // no profile is made active.
    settings.setValue(VKCONFIG_KEY_ACTIVEPROFILE, pActiveProfile->qsProfileName);

    /////////////////////////
    // vk_layer_settings.txt
    QFile file(qsOverrideSettingsPath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);

    // Loop through all the layers
    for (int iLayer = 0; iLayer < pProfile->layers.size(); iLayer++) {
        CLayerFile *pLayer = pProfile->layers[iLayer];
        stream << endl;
        stream << "# " << pLayer->name << endl;

        QString shortLayerName = pLayer->name;
        shortLayerName.remove("VK_LAYER_");
        QString lcLayerName = shortLayerName.toLower();

        for (int iSetting = 0; iSetting < pLayer->layerSettings.size(); iSetting++) {
            TLayerSettings *pSetting = pLayer->layerSettings[iSetting];
            stream << lcLayerName << "." << pSetting->settingsName << " = " << pSetting->settingsValue << endl;
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
    for (int i = 0; i < pProfile->blacklistedLayers.size(); i++) json_blacklist.append(pProfile->blacklistedLayers[i]);

    // Only supply this list if an app list is specified
    QJsonArray json_applist;
    if (this->bApplyOnlyToList)
        for (int i = 0; i < appList.size(); i++) json_applist.append(QDir::toNativeSeparators(appList[i]->qsAppNameWithPath));

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
    layer.insert("app_keys", json_applist);

    QJsonObject root;
    root.insert("file_format_version", QJsonValue(QString("1.1.2")));
    root.insert("layer", layer);
    QJsonDocument doc(root);

    QFile jsonFile(qsOverrideJsonPath);
    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) return;  // TBD, should we report an error
    jsonFile.write(doc.toJson());
    jsonFile.close();

    // On Windows only, we need to write these values to the registry
#ifdef _WIN32
    // I'm leaving this here as a warning... the Qt wrappers WILL NOT allow the file path seperators in a key
    // so you MUST use the native API to override this policy.
    /*    QSettings registry("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ImplicitLayers", QSettings::NativeFormat);
        registry.setValue(qsOverrideJsonPath, 0);
        QSettings overrideSettings("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\Settings", QSettings::NativeFormat);
        //overrideSettings.setValue(qsOverrideSettingsPath, QVariant()::DWORD(0));
    */
    AddRegistryEntriesForLayers(qsOverrideJsonPath, qsOverrideSettingsPath);
#endif
}

///////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::pushProfile
/// \param pNew
/// Make a temporary copy of this profile and activate it.
/// Any layer output settings need to be set to stderr
void CVulkanConfiguration::pushProfile(CProfileDef *pNew) {
    // Copy the working profile
    pSavedProfile = pActiveProfile;
    CProfileDef *pCopy = pNew->DuplicateProfile();
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

    SetCurrentActiveProfile(pCopy);
}

/////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::popProfile
/// Restore the original working profile
void CVulkanConfiguration::popProfile(void) {
    delete GetCurrentActiveProfile();
    SetCurrentActiveProfile(pSavedProfile);
    pSavedProfile = nullptr;
}
