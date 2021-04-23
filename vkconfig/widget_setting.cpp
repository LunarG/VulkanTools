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

#include "widget_setting.h"

WidgetSettingBase::WidgetSettingBase(QTreeWidget* tree, QTreeWidgetItem* item) : tree(tree), item(item) {
    assert(tree != nullptr);
    assert(item != nullptr);
}

bool IsSupported(const SettingMeta* meta) {
    if (meta == nullptr) return false;

    if (meta->view == SETTING_VIEW_HIDDEN) return false;

    if (!IsPlatformSupported(meta->platform_flags)) return false;

    return true;
}

bool IsSupported(const SettingEnumValue* value) {
    if (value == nullptr) return false;

    if (value->view == SETTING_VIEW_HIDDEN) return false;

    if (!IsPlatformSupported(value->platform_flags)) return false;

    return true;
}

int HorizontalAdvance(const QFontMetrics& fm, const QString& string) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    return fm.horizontalAdvance(string);
#else
    return fm.width(string);
#endif
}
