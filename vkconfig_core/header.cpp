/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include "header.h"
#include "json.h"

void LoadMetaHeader(Header& header, const QJsonObject& json_object) {
    header.label = ReadStringValue(json_object, "label");
    header.description = ReadStringValue(json_object, "description");
    if (json_object.value("detailed") != QJsonValue::Undefined) {
        header.detailed = ReadStringValue(json_object, "detailed");
    }
    if (json_object.value("url") != QJsonValue::Undefined) {
        header.url = Path(ReadStringValue(json_object, "url"), false);
    }
    if (json_object.value("status") != QJsonValue::Undefined) {
        header.status = GetStatusType(ReadStringValue(json_object, "status").c_str());
    }
    if (json_object.value("deprecated_by_key") != QJsonValue::Undefined) {
        header.deprecated_by_key = ReadStringValue(json_object, "deprecated_by_key");
    }
    if (json_object.value("view") != QJsonValue::Undefined) {
        header.view = GetSettingView(ReadStringValue(json_object, "view").c_str());
    }
    if (json_object.value("platforms") != QJsonValue::Undefined) {
        header.platform_flags = GetPlatformFlags(ReadStringArray(json_object, "platforms"));
    }
    if (json_object.value("expanded") != QJsonValue::Undefined) {
        header.expanded = ReadBoolValue(json_object, "expanded");
    }
}
