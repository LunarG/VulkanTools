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

#include "registry.h"

#if VKC_ENV == VKC_ENV_WIN32
#include <windows.h>
#include <winreg.h>
#include <Cfgmgr32.h>
#include <shlobj.h>
#define WIN_BUFFER_SIZE 1024

#include <QSettings>

/// On Windows the overide json file and settings file are not used unless the path to those
/// files are stored in the registry.
void AppendRegistryEntriesForLayers(QString loader_settings_file, QString layers_settings_file) {
    // Layer override json file
    HKEY key;
    const HKEY userKey = IsUserAnAdmin() ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;

    REGSAM access = KEY_WRITE;
    LSTATUS err = RegCreateKeyEx(userKey, TEXT("SOFTWARE\\Khronos\\Vulkan\\LoaderSettings"), 0, NULL, REG_OPTION_NON_VOLATILE,
                                 access, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) return;

    QString file_path;
    DWORD value_count;
    DWORD value = 0;
    RegQueryInfoKey(key, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, NULL, NULL, NULL);
    RegSetValueExW(key, (LPCWSTR)loader_settings_file.utf16(), 0, REG_DWORD, (BYTE *)&value, sizeof(value));
    RegCloseKey(key);

    // Layer settings file
    err = RegCreateKeyEx(userKey, TEXT("SOFTWARE\\Khronos\\Vulkan\\Settings"), 0, NULL, REG_OPTION_NON_VOLATILE, access, NULL, &key,
                         NULL);
    if (err != ERROR_SUCCESS) return;

    RegQueryInfoKeyW(key, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, NULL, NULL, NULL);
    RegSetValueExW(key, (LPCWSTR)layers_settings_file.utf16(), 0, REG_DWORD, (BYTE *)&value, sizeof(value));
    RegCloseKey(key);
}

/// On Windows the overide json file and settings file are not used unless the path to those
/// files are stored in the registry.
void RemoveRegistryEntriesForLayers() {
    // Layer override json file
    HKEY key;
    HKEY userKey = IsUserAnAdmin() ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;

    REGSAM access = KEY_WRITE;
    LSTATUS err =
        RegCreateKeyEx(userKey, TEXT("SOFTWARE\\Khronos\\Vulkan"), 0, NULL, REG_OPTION_NON_VOLATILE, access, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) return;

    err = RegDeleteTreeW(key, (LPCWSTR)QString("LoaderSettings").utf16());
    RegCloseKey(key);

    // Layer settings file
    err = RegCreateKeyEx(userKey, TEXT("SOFTWARE\\Khronos\\Vulkan"), 0, NULL, REG_OPTION_NON_VOLATILE, access, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) return;

    err = RegDeleteTreeW(key, (LPCWSTR)QString("Settings").utf16());
    RegCloseKey(key);
}

/// Look for device specific layers
static void LoadDeviceRegistry(DEVINST id, const QString &entry, std::vector<LayersPathInfo> &layers_paths) {
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
            LayersPathInfo path_info;
            path_info.type = LAYER_TYPE_IMPLICIT;
            path_info.path = QString::fromWCharArray(curr_filename).toStdString();
            layers_paths.push_back(path_info);
            Layer layer;

            if (data_type == REG_SZ) {
                break;
            }
        }
    }

    delete[] path;
    RegCloseKey(key);
}

/// This is for Windows only. It looks for device specific layers in the Windows registry.
std::vector<LayersPathInfo> LoadRegistrySystemLayers(const char *input_path) {
    std::vector<LayersPathInfo> layers_paths;

    QString path(input_path);

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
            // type = LAYER_TYPE_EXPLICIT;
        } else if (path.endsWith("VulkanImplicitLayers")) {
            entry = "VulkanImplicitLayers";
            // type = LAYER_TYPE_IMPLICIT;
        }

        for (wchar_t *device_name = device_names; device_name[0] != '\0'; device_name += wcslen(device_name) + 1) {
            DEVINST device_id;
            if (CM_Locate_DevNodeW(&device_id, device_name, CM_LOCATE_DEVNODE_NORMAL) != CR_SUCCESS) {
                continue;
            }
            LoadDeviceRegistry(device_id, entry, layers_paths);

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
                    LoadDeviceRegistry(child_id, entry, layers_paths);
                    break;
                }
            } while (CM_Get_Sibling(&child_id, child_id, 0) == CR_SUCCESS);
        }
    }

    if (device_names != nullptr) {
        delete[] device_names;
    }

    return layers_paths;
}

std::vector<LayersPathInfo> LoadRegistrySoftwareLayers(const char *path, LayerType type) {
    std::vector<LayersPathInfo> result;
    QSettings settings(path, QSettings::NativeFormat);
    const QStringList &files = settings.allKeys();

    for (int i = 0, n = files.size(); i < n; ++i) {
        Path path(files[i].toStdString());

        LayersPathInfo info;
        info.type = type;
        info.path = path.IsFile() ? path.AbsoluteDir() : path.AbsolutePath();

        if (::Found(result, info.path)) {
            continue;
        }

        result.push_back(info);
    }

    return result;
}

#endif  // VKC_ENV == VKC_ENV_WIN32
