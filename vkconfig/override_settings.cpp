/*
 * Copyright (c) 2018-2019 Valve Corporation
 * Copyright (c) 2018-2019 LunarG, Inc.
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

#include "override_settings.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QTextStream>

// This is only needed to keep the manifest version up to date. Other than that, it shouldn't be needed
#include <vulkan/vulkan.h>

#if defined(_WIN32)
#include <windows.h>

#define WIN_BUFFER_SIZE 1024
#endif

OverrideSettings::OverrideSettings()
{
    // Load the override layer (if found)
    QFile layer_file(LayerFile(false));
    if (layer_file.exists() && layer_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString data = layer_file.readAll();
        layer_file.close();

        QJsonDocument document = QJsonDocument::fromJson(data.toLocal8Bit());
        if (document.isObject()) {
            QJsonValue layer = document.object().value("layer");
            if (layer.isObject()) {
                QJsonValue components = layer.toObject().value("component_layers");
                if (components.isArray()) {
                    for (const QJsonValue &component : components.toArray()) {
                        if (!component.isString()) {
                            continue;
                        }
                        enabled_layers.append(component.toString());
                    }
                }
                QJsonValue blacklist = layer.toObject().value("blacklisted_layers");
                if (blacklist.isArray()) {
                    for (const QJsonValue &disable : blacklist.toArray()) {
                        if (!disable.isString()) {
                            continue;
                        }
                        disabled_layers.append(disable.toString());
                    }
                }
            }
        }
    }

    // Load the settings file (if found)
    QFile settings_file(LayerSettingsFile(false));
    if (settings_file.exists() && settings_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString data = settings_file.readAll();
        settings_file.close();

        for (const QString& line : data.split(QRegularExpression("\n|\r\n|\r"))) {
            if (QRegularExpression("\\s*#").match(line).hasMatch()) {
                continue;
            }

            QList<QString> sections = line.split(".");
            if (sections.size() != 2) {
                continue;
            }
            QString layer_abbrev = sections[0];
            QList<QString> subsections = sections[1].split("=");
            if (subsections.size() != 2) {
                continue;
            }
            QString setting_name = subsections[0];
            QString setting_value = subsections[1];

            QStringList layer_sections = layer_abbrev.split("_");
            layer_sections[0] = layer_sections[0].toUpper();
            layer_sections.prepend("LAYER");
            layer_sections.prepend("VK");
            QString layer_name = layer_sections.join("_");

            layer_settings[layer_name][setting_name] = setting_value;
        }
    }
}

void OverrideSettings::ClearLayers()
{
    QString layer_path = LayerFile(false);
#if defined(_WIN32)
    HKEY key;
    LSTATUS err = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers", REG_OPTION_NON_VOLATILE, KEY_WRITE, &key);
    if (err == ERROR_SUCCESS) {
        QByteArray path_bytes = layer_path.toLocal8Bit();
        const char* dummy = path_bytes.data();
        RegDeleteValue(key, path_bytes.data());
        RegCloseKey(key);
    }
#endif

    QFile file(layer_path);
    if (file.exists()) {
        file.remove();
    }
    enabled_layers.clear();
    disabled_layers.clear();
}

void OverrideSettings::ClearSettings()
{
    QString settings_path = LayerSettingsFile(false);
#if defined(_WIN32)
    HKEY key;
    LSTATUS err = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Khronos\\Vulkan\\Settings", REG_OPTION_NON_VOLATILE, KEY_WRITE, &key);
    if (err == ERROR_SUCCESS) {
        QByteArray path_bytes = settings_path.toLocal8Bit();
        const char *dummy = path_bytes.data();
        RegDeleteValue(key, path_bytes.data());
        RegCloseKey(key);
    }
#endif

    QFile file(settings_path);
    if (file.exists()) {
        file.remove();
    }
    layer_settings.clear();
}

QList<QString> OverrideSettings::DisabledLayers() const { return disabled_layers; }

QList<QString> OverrideSettings::EnabledLayers() const { return enabled_layers; }

QHash<QString, QHash<QString, QString>> OverrideSettings::LayerSettings() const
{
    return layer_settings;
}

bool OverrideSettings::SaveLayers(const QList<QPair<QString, LayerType>> &paths, const QList<LayerManifest> &enabled_layers,
                                  const QList<LayerManifest> &disabled_layers, int expiration) {
    this->enabled_layers.clear();
    for (const LayerManifest &manifest : enabled_layers) {
        this->enabled_layers.append(manifest.name);
    }

    QDateTime now = QDateTime::currentDateTime();
    now = now.addSecs(expiration);

    QJsonArray json_paths;
    for (const QPair<QString, LayerType> &pair : paths) {
        json_paths.append(pair.first);
    }
    QJsonArray json_layers;
    for (const LayerManifest &manifest : enabled_layers) {
        json_layers.append(manifest.name);
    }
    QJsonArray json_blacklist;
    for (const LayerManifest &manifest : disabled_layers) {
        json_blacklist.append(manifest.name);
    }
    QJsonObject disable;
    disable.insert("DISABLE_VK_LAYER_LUNARG_override", QString("1"));

    QJsonObject layer;
    layer.insert("name", QString("VK_LAYER_LUNARG_override"));
    layer.insert("type", QString("GLOBAL"));
    layer.insert("api_version", "1.1." + QString::number(VK_HEADER_VERSION));
    layer.insert("implementation_version", QString("1"));
    layer.insert("description", QString("LunarG Override Layer"));
    if (expiration >= 0) {
        layer.insert("expiration_date", now.toString("yyyy-MM-dd-hh-mm"));
    }
    layer.insert("override_paths", json_paths);
    layer.insert("component_layers", json_layers);
    layer.insert("blacklisted_layers", json_blacklist);
    layer.insert("disable_environment", disable);

    QJsonObject root;
    root.insert("file_format_version", QJsonValue(QString("1.1.2")));
    root.insert("layer", layer);
    QJsonDocument doc(root);

    QFile layer_file(LayerFile(true));
    if (!layer_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return true;
    }
    layer_file.write(doc.toJson());
    layer_file.close();

    return false;
}

bool OverrideSettings::SaveSettings(const QHash<QString, QHash<QString, LayerValue>> &settings)
{
    layer_settings.clear();
    for (const QString &layer : settings.keys()) {
        QHash<QString, QString> options;
        for (const QString &option : settings[layer].keys()) {
            QStringList values = settings[layer][option].values.values();
            options.insert(option, values.join(","));
        }
        layer_settings.insert(layer, options);
    }

    QFile settings_file(LayerSettingsFile(true));
    if (!settings_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return true;
    }

    QTextStream stream(&settings_file);
    for (const QString &layer : settings.keys()) {
        if (!layer.startsWith("VK_LAYER_")) {
            continue;
        }
        QString short_layer = layer.right(layer.size() - 9).toLower();

        stream << "\n# " << layer << "\n";
        for (const QString &option : settings[layer].keys()) {
            QStringList values = settings[layer][option].values.values();
            QString string_value = values.join(",");
            stream << short_layer << "." << option << " = " << string_value << "\n";
        }
    }
    settings_file.close();

    return false;
}

QString OverrideSettings::LayerFile(bool create_path) const
{
#if defined(_WIN32)
    HKEY key;
    REGSAM access = KEY_READ | (create_path ? KEY_WRITE : 0);
    LSTATUS err = RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers", 0, NULL, REG_OPTION_NON_VOLATILE, access, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) {
        return "";
    }

    QString file_path;
    DWORD value_count;
    RegQueryInfoKey(key, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, NULL, NULL, NULL);
    for (DWORD i = 0; i < value_count; ++i) {
        TCHAR buffer[WIN_BUFFER_SIZE];
        DWORD buff_size = WIN_BUFFER_SIZE;
        DWORD type, value, value_size = sizeof(value);
        RegEnumValue(key, i, buffer, &buff_size, NULL, &type, (BYTE*) &value, &value_size);

        if (type == REG_DWORD && value == 0) {
            file_path = buffer;
            break;
        }
    }

    if (file_path.isEmpty() && create_path) {
        QDir dir = QDir::temp();
        if (!dir.cd("VulkanLayerManager")) {
            dir.mkpath("VulkanLayerManager");
            dir.cd("VulkanLayerManager");
        }
        file_path = QDir::toNativeSeparators(dir.absoluteFilePath("VkLayer_override.json"));
        QByteArray file_path_bytes = file_path.toLocal8Bit();
        DWORD value = 0;
        RegSetValueEx(key, file_path_bytes.data(), 0, REG_DWORD, (BYTE*) &value, sizeof(value));
    }

    RegCloseKey(key);
    return file_path;
#else
    QDir dir = QDir::home();
    if (!dir.cd(".local/share/vulkan/implicit_layer.d")) {
        dir.mkpath(".local/share/vulkan/implicit_layer.d");
        dir.cd(".local/share/vulkan/implicit_layer.d");
    }
    return dir.absoluteFilePath("VkLayer_override.json");
#endif
}

QString OverrideSettings::LayerSettingsFile(bool create_path) const
{
#if defined(_WIN32)
    HKEY key;
    REGSAM access = KEY_READ | (create_path ? KEY_WRITE : 0);
    LSTATUS err = RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Khronos\\Vulkan\\Settings", 0, NULL, REG_OPTION_NON_VOLATILE,
                                 access, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) {
        return "";
    }

    QString file_path;
    DWORD value_count;
    RegQueryInfoKey(key, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, NULL, NULL, NULL);
    for (DWORD i = 0; i < value_count; ++i) {
        TCHAR buffer[WIN_BUFFER_SIZE];
        DWORD buff_size = WIN_BUFFER_SIZE;
        DWORD type, value, value_size = sizeof(value);
        RegEnumValue(key, i, buffer, &buff_size, NULL, &type, (BYTE *)&value, &value_size);

        if (type == REG_DWORD && value == 0) {
            file_path = buffer;
            break;
        }
    }

    if (file_path.isEmpty() && create_path) {
        QDir dir = QDir::temp();
        if (!dir.cd("VulkanLayerManager")) {
            dir.mkpath("VulkanLayerManager");
            dir.cd("VulkanLayerManager");
        }
        QString file_path = QDir::toNativeSeparators(dir.absoluteFilePath("vk_layer_settings.txt"));
        QByteArray file_path_bytes = file_path.toLocal8Bit();
        DWORD value = 0;
        RegSetValueEx(key, file_path_bytes.data(), 0, REG_DWORD, (BYTE *)&value, sizeof(value));
    }

    RegCloseKey(key);
    return file_path;
#else
    QDir dir = QDir::home();
    if (!dir.cd(".local/share/vulkan/settings.d")) {
            dir.mkpath(".local/share/vulkan/settings.d");
            dir.cd(".local/share/vulkan/settings.d");
        }
    return dir.absoluteFilePath("vk_layer_settings.txt");
#endif
}
