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

#include "environment.h"
#include "setting.h"
#include "util.h"

#include "type_platform.h"
#include "type_layers_mode.h"
#include "type_log.h"

#include <QMessageBox>
#include <QCheckBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <cassert>
#include <iostream>

Environment::Environment() {}

Environment::~Environment() {}

void Environment::Reset() { this->loader_message_types_flags = ::GetLogFlags(qgetenv("VK_LOADER_DEBUG").toStdString()); }

std::string GetPath() {
    static const char* TABLE[] = {
        "/AppData/Local/LunarG",  // ENVIRONMENT_WIN32
        "/.local/share/vulkan"    // ENVIRONMENT_UNIX
    };
    static_assert(std::size(TABLE) == ENVIRONMENT_COUNT, "The tranlation table size doesn't match the enum number of elements");

    const std::string home = QDir().homePath().toStdString();
    return home + TABLE[VKC_ENV];
}

bool Environment::Load(const QJsonObject& json_root_object) {
    const Version file_format_version = Version(json_root_object.value("file_format_version").toString().toStdString());
    if (file_format_version > Version::VKCONFIG) {
        return false;  // Vulkan Configurator needs to be updated
    }

    // interface json object
    const QJsonObject& json_interface_object = json_root_object.value("interface").toObject();
    this->active_tab = GetTabType(json_interface_object.value("active_tab").toString().toStdString().c_str());
    this->has_crashed = json_interface_object.value("has_crashed").toBool();

    // diagnostic json object
    const QJsonObject& json_diagnostic_object = json_root_object.value("diagnostic").toObject();
    const QJsonArray& json_loader_messages_array = json_diagnostic_object.value("loader_messages").toArray();
    this->loader_message_types_flags = 0;
    for (int i = 0, n = json_loader_messages_array.size(); i < n; ++i) {
        const std::string& token = json_loader_messages_array[i].toString().toStdString();
        this->loader_message_types_flags |= GetLogBit(token.c_str());
    }

    // preferences json object
    const QJsonObject& json_preferences_object = json_root_object.value("preferences").toObject();
    this->home_sdk_path = json_preferences_object.value("VK_HOME").toString().toStdString();
    if (this->home_sdk_path.Empty()) {
        this->home_sdk_path = ::Get(Path::HOME);
    }

    const QJsonArray& json_hide_message_boxes_array = json_preferences_object.value("hide_message_boxes").toArray();
    this->hide_message_boxes_flags = 0;
    for (int i = 0, n = json_hide_message_boxes_array.size(); i < n; ++i) {
        const std::string& token = json_hide_message_boxes_array[i].toString().toStdString();
        this->hide_message_boxes_flags |= GetLogBit(token.c_str());
    }

    return true;
}

bool Environment::Save(QJsonObject& json_root_object) const {
    json_root_object.insert("file_format_version", Version::VKCONFIG.str().c_str());

    QJsonObject json_interface_object;
    json_interface_object.insert("active_tab", GetToken(this->active_tab));
    json_interface_object.insert("has_crashed", this->has_crashed);
    json_root_object.insert("interface", json_interface_object);

    QJsonObject json_diagnostic_object;

    QJsonArray json_loader_messages_array;
    for (int i = LOG_FIRST, n = LOG_COUNT; i < n; ++i) {
        LogType type = static_cast<LogType>(i);
        if (this->loader_message_types_flags & (1 << i)) {
            json_loader_messages_array.append(GetToken(type));
        }
    }
    json_diagnostic_object.insert("loader_messages", json_loader_messages_array);

    json_root_object.insert("diagnostic", json_diagnostic_object);

    QJsonObject json_preferences_object;
    json_preferences_object.insert("VK_HOME", this->home_sdk_path.RelativePath().c_str());

    QJsonArray json_hide_message_boxes_array;
    for (int i = LOG_FIRST, n = LOG_COUNT; i < n; ++i) {
        LogType type = static_cast<LogType>(i);
        if (this->hide_message_boxes_flags & (1 << i)) {
            json_hide_message_boxes_array.append(GetToken(type));
        }
    }
    json_preferences_object.insert("hide_message_boxes", json_hide_message_boxes_array);

    json_root_object.insert("preferences", json_preferences_object);

    return true;
}
