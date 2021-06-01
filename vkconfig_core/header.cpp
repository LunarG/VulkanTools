/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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
#include "util.h"
#include "json.h"

#include <cassert>

SettingView GetSettingView(const char* token) {
    assert(token != nullptr);
    assert(std::strcmp(token, "") != 0);

    for (int i = SETTING_VIEW_FIRST; i <= SETTING_VIEW_LAST; ++i) {
        const SettingView state = static_cast<SettingView>(i);
        if (ToUpperCase(token) == GetToken(state)) return state;
    }

    assert(0);  // Unknown token
    return static_cast<SettingView>(-1);
}

const char* GetToken(SettingView state) {
    assert(state >= SETTING_VIEW_FIRST && state <= SETTING_VIEW_LAST);

    static const char* table[] = {
        "STANDARD",  // SETTING_VIEW_STANDARD
        "ADVANCED",  // SETTING_VIEW_ADVANCED
        "HIDDEN",    // SETTING_VIEW_HIDDEN
    };
    static_assert(countof(table) == SETTING_VIEW_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[state];
}

void LoadMetaHeader(Header& header, const QJsonObject& json_object) {
    header.label = ReadStringValue(json_object, "label");
    header.description = ReadStringValue(json_object, "description");
    if (json_object.value("url") != QJsonValue::Undefined) {
        header.url = ReadStringValue(json_object, "url");
    }
    if (json_object.value("status") != QJsonValue::Undefined) {
        header.status = GetStatusType(ReadStringValue(json_object, "status").c_str());
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
