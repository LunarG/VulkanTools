/*
 * Copyright (c) 2018 Valve Corporation
 * Copyright (c) 2018 LunarG, Inc.
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
 * Author: Lenny Komow <lenny@lunarg.com>
 */

#include "layer_manifest.h"

#include <QJsonArray>
#include <QJsonDocument>

#if defined(_WIN32)
#define WIN_BUFFER_SIZE 1024
#endif

QString LayerManifest::PrettyName() const {
    QStringList segments = name.split("_");
    if (segments.count() <= 3 || segments[0] != "VK" || segments[1] != "LAYER") {
        return name;
    }

    for (int i = 0; i < 2; ++i) {
        segments.removeFirst();
    }

    static const QHash<QString, QString> LAYER_AUTHORS = LoadLayerAuthors(":/layermgr/layer_info.json");
    if (LAYER_AUTHORS.contains(segments[0])) {
        segments[0] = LAYER_AUTHORS[segments[0]];
    }
    segments[0].append(":");

    for (int i = 0; i < segments.count(); ++i) {
        segments[i].replace(0, 1, segments[i][0].toUpper());
    }
    return segments.join(" ");
}

QList<LayerManifest> LayerManifest::LoadDirectory(const QDir &directory, LayerType type, bool recursive) {
    QList<LayerManifest> manifests;
    if (!directory.exists()) {
        return manifests;
    }

    QList<QString> filters = {"*.json"};
    for (auto file_info : directory.entryInfoList(filters, QDir::Files | QDir::Readable)) {
        LoadLayerFile(file_info.absoluteFilePath(), type, &manifests);
    }
    if (recursive) {
        for (auto dir_info : directory.entryInfoList(filters, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Readable)) {
            manifests.append(LoadDirectory(QDir(dir_info.absoluteFilePath()), type, true));
        }
    }

    return manifests;
}

#if defined(_WIN32)
QList<LayerManifest> LayerManifest::LoadRegistry(const QString &path, LayerType type) {
    QList<LayerManifest> manifests;

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

    if (!path.contains("...")) {
        HKEY key;
        QByteArray key_bytes = path.section('\\', 1).toLocal8Bit();
        LSTATUS err = RegCreateKeyEx(root, key_bytes.data(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, NULL);
        if (err != ERROR_SUCCESS) {
            return manifests;
        }

        DWORD value_count;
        RegQueryInfoKey(key, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, NULL, NULL, NULL);
        for (DWORD i = 0; i < value_count; ++i) {
            TCHAR file_path[WIN_BUFFER_SIZE];
            DWORD buff_size = WIN_BUFFER_SIZE;
            RegEnumValue(key, i, file_path, &buff_size, NULL, NULL, NULL, NULL);

            LoadLayerFile(file_path, type, &manifests);
        }

        RegCloseKey(key);
    } else {
        static const char* const DISPLAY_GUID = "{4d36e968-e325-11ce-bfc1-08002be10318}";
        static const char *const SOFTWARE_COMPONENT_GUID = "{5c4c3332-344d-483c-8739-259e934c9cc8}";
        static const ULONG FLAGS = CM_GETIDLIST_FILTER_CLASS | CM_GETIDLIST_FILTER_PRESENT;

        ULONG device_names_size;
        char *device_names = nullptr;
        do {
            CM_Get_Device_ID_List_Size(&device_names_size, DISPLAY_GUID, FLAGS);
            if (device_names != nullptr) {
                delete[] device_names;
            }
            device_names = new char[device_names_size];
        }  while (CM_Get_Device_ID_List(DISPLAY_GUID, device_names, device_names_size, FLAGS) == CR_BUFFER_SMALL);

        if (device_names != nullptr) {
            QString entry;
            LayerType type;
            if (path.endsWith("VulkanExplicitLayers")) {
                entry = "VulkanExplicitLayers";
                type = LayerType::Explicit;
            } else if (path.endsWith("VulkanImplicitLayers")) {
                entry = "VulkanImplicitLayers";
                type = LayerType::Implicit;
            }

            for (char *device_name = device_names; device_name[0] != '\0'; device_name += strlen(device_name) + 1) {
                DEVINST device_id;
                if (CM_Locate_DevNode(&device_id, device_name, CM_LOCATE_DEVNODE_NORMAL) != CR_SUCCESS) {
                    continue;
                }
                manifests += LoadDeviceRegistry(device_id, entry, type);

                DEVINST child_id;
                if (CM_Get_Child(&child_id, device_id, 0) != CR_SUCCESS) {
                    continue;
                }
                do {
                    char child_buffer[MAX_DEVICE_ID_LEN];
                    CM_Get_Device_ID(child_id, child_buffer, MAX_DEVICE_ID_LEN, 0);

                    char child_guid[MAX_GUID_STRING_LEN + 2];
                    ULONG child_guid_size = sizeof(child_guid);
                    if (CM_Get_DevNode_Registry_Property(child_id, CM_DRP_CLASSGUID, nullptr, &child_guid, &child_guid_size, 0) != CR_SUCCESS) {
                        continue;
                    }
                    if (strcmp(child_guid, SOFTWARE_COMPONENT_GUID) == 0) {
                        manifests += LoadDeviceRegistry(child_id, entry, type);
                        break;
                     }
                } while(CM_Get_Sibling(&child_id, child_id, 0) == CR_SUCCESS);
            }
        }

        if(device_names != nullptr) {
            delete[] device_names;
        }
    }
    
    return manifests;
}
#endif

QHash<QString, QString> LayerManifest::LoadLayerAuthors(const QString &file_path) {
    QFile file(file_path);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(data.toLocal8Bit());
    if (!document.isObject()) {
        return QHash<QString, QString>();
    }
    QJsonObject root = document.object();
    if (!root.value("layer_authors").isObject()) {
        return QHash<QString, QString>();
    }
    QJsonObject author_object = root.value("layer_authors").toObject();

    QHash<QString, QString> authors;
    for (QString &key : author_object.keys()) {
        if (author_object[key].isObject() && author_object[key].toObject()["name"].isString()) {
            authors[key] = author_object[key].toObject()["name"].toString();
        }
    }
    return authors;
}

void LayerManifest::LoadLayerFile(const QString &file_path, LayerType type, QList<LayerManifest> *manifest_list) {
    QFile file(file_path);
    QFileInfo file_info(file);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(data.toLocal8Bit());
    if (!document.isObject()) {
        return;
    }
    QJsonObject root = document.object();

    if (root.value("layer").isObject()) {
        LoadLayerObject(root.value("layer").toObject(), type, file_info, manifest_list);
    }

    if (root.value("layers").isArray()) {
        QJsonArray layer_array = root.value("layers").toArray();
        for (auto layer : layer_array) {
            if (layer.isObject()) {
                LoadLayerObject(layer.toObject(), type, file_info, manifest_list);
            }
        }
    }
}

void LayerManifest::LoadLayerObject(const QJsonObject &layer_object, LayerType type, const QFileInfo &file,
                                    QList<LayerManifest> *manifest_list) {
    QJsonValue name = layer_object.value("name");
    QJsonValue description = layer_object.value("description");
    QJsonValue library_path = layer_object.value("library_path");
    QJsonValue component_layers = layer_object.value("component_layers");
    if (name.isString() && description.isString() && (library_path.isString() || component_layers.isArray())) {
        QString lib_path = library_path.toString();
        QFileInfo library(file.dir(), lib_path);
        if (!(lib_path.startsWith("./") || lib_path.startsWith(".\\")) || library.exists() || component_layers.isArray()) {
            LayerManifest layer_manifest;
            layer_manifest.file_path = file.absoluteFilePath();
            layer_manifest.name = layer_object.value("name").toString();
            layer_manifest.description = layer_object.value("description").toString();
            layer_manifest.type = type;
            if (layer_manifest.name != "VK_LAYER_LUNARG_override") {
                manifest_list->append(layer_manifest);
            }
        }
    }
}

#if defined(_WIN32)
QList<LayerManifest> LayerManifest::LoadDeviceRegistry(DEVINST id, const QString& entry, LayerType type) {
    QList<LayerManifest> manifests;

    HKEY key;
    if(CM_Open_DevNode_Key(id, KEY_QUERY_VALUE, 0, RegDisposition_OpenExisting, &key, CM_REGISTRY_SOFTWARE) != CR_SUCCESS) {
        return manifests;
    }

    QByteArray entry_bytes = entry.toLocal8Bit();
    DWORD path_size;
    if (RegQueryValueEx(key, entry_bytes.data(), nullptr, nullptr, nullptr, &path_size) != ERROR_SUCCESS) {
        RegCloseKey(key);
        return manifests;
    }
    DWORD data_type;
    char *path = new char[path_size];
    if (RegQueryValueEx(key, entry_bytes.data(), nullptr, &data_type, (LPBYTE)path, &path_size) != ERROR_SUCCESS) {
        delete[] path;
        RegCloseKey(key);
        return manifests;
    }

    if (data_type == REG_SZ || data_type == REG_MULTI_SZ) {
        for (char* curr_filename = path; curr_filename[0] != '\0'; curr_filename += strlen(curr_filename) + 1) {
            LoadLayerFile(curr_filename, type, &manifests);

            if (data_type == REG_SZ) {
                break;
            }
        }
    }

    delete[] path;
    RegCloseKey(key);
    return manifests;
}
#endif

QList<LayerOption> LayerOption::LoadOptions(const LayerManifest &manifest) {
    static const QJsonObject OPTIONS_JSON = LoadOptionJson(":/layermgr/layer_info.json");
    if (OPTIONS_JSON.empty()) {
        return QList<LayerOption>();
    }

    if (!OPTIONS_JSON.value(manifest.name).isObject()) {
        return QList<LayerOption>();
    }

    QList<LayerOption> options;
    const QJsonObject layer_info = OPTIONS_JSON.value(manifest.name).toObject();
    for (const QString &key : layer_info.keys()) {
        if (!layer_info.value(key).isObject()) {
            continue;
        }
        QJsonObject option_info = layer_info.value(key).toObject();

        LayerOption option;
        option.layer_name = manifest.name;
        option.name = key;

        if (option_info.value("name").isString()) {
            option.pretty_name = option_info.value("name").toString();
        } else {
            option.pretty_name = key;
        }

        if (option_info.value("description").isString()) {
            option.description = option_info.value("description").toString();
        }

        if (!option_info.value("type").isString()) {
            continue;
        }
        QString option_type = option_info.value("type").toString();
        QJsonValue default_value = option_info.value("default");

        if (option_type == "bool") {
            option.type = LayerOptionType::Bool;
            if (!default_value.isBool()) {
                continue;
            }
            option.default_values.insert(default_value.toBool() ? "TRUE" : "FALSE");

        } else if (option_type == "enum" || option_type == "multi_enum") {
            QJsonValue enum_options = option_info.value("options");
            if (enum_options.isArray()) {
                for (const QJsonValue &item : enum_options.toArray()) {
                    if (!item.isString()) {
                        continue;
                    }
                    option.enum_options.insert(item.toString(), item.toString());
                }
            } else if (enum_options.isObject()) {
                for (QString &key : enum_options.toObject().keys()) {
                    if (!enum_options.toObject().value(key).isString()) {
                        continue;
                    }
                    option.enum_options.insert(key, enum_options.toObject().value(key).toString());
                }
            } else {
                continue;
            }

            if (option_type == "enum") {
                option.type = LayerOptionType::Enum;
                if (!default_value.isString() || !option.enum_options.contains(default_value.toString())) {
                    continue;
                }
                option.default_values.insert(default_value.toString());
            } else {
                option.type = LayerOptionType::MultiEnum;
                if (!default_value.isArray()) {
                    continue;
                }
                for (const QJsonValue &item : default_value.toArray()) {
                    if (!item.isString() || !option.enum_options.contains(item.toString())) {
                        continue;
                    }
                    option.default_values.insert(item.toString());
                }
            }

        } else if (option_type == "open_file") {
            option.type = LayerOptionType::OpenFile;
            if (!default_value.isString()) {
                continue;
            }
            option.default_values.insert(default_value.toString());

        } else if (option_type == "save_file") {
            option.type = LayerOptionType::SaveFile;
            if (!default_value.isString()) {
                continue;
            }
            option.default_values.insert(default_value.toString());

        } else if (option_type == "string") {
            option.type = LayerOptionType::String;
            if (!default_value.isString()) {
                continue;
            }
            option.default_values.insert(default_value.toString());

        } else {
            continue;
        }

        options.append(option);
    }

    return options;
}

QJsonObject LayerOption::LoadOptionJson(const QString &file_path) {
    QFile file(file_path);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(data.toLocal8Bit());
    if (!document.isObject()) {
        return QJsonObject();
    }
    QJsonObject root = document.object();
    if (!root.value("layer_options").isObject()) {
        return QJsonObject();
    }
    return root.value("layer_options").toObject();
}
