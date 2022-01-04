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

void WidgetSettingBase::DisplayOverride(QWidget* widget, const SettingMeta& meta) const {
    QCursor cursor = widget->cursor();
    cursor.setShape(Qt::WhatsThisCursor);
    widget->setCursor(cursor);

    QPalette palette;
    palette.setColor(QPalette::Active, QPalette::WindowText, QColor(255, 0, 0));
    palette.setColor(QPalette::Active, QPalette::Text, QColor(255, 0, 0));
    palette.setColor(QPalette::Inactive, QPalette::WindowText, QColor(255, 0, 0));
    palette.setColor(QPalette::Inactive, QPalette::Text, QColor(255, 0, 0));
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(192, 128, 128));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(192, 128, 128));

    widget->setPalette(palette);

    const std::string tip =
        format("Overridden by '%s' environment variable set to: '%s'", meta.env.c_str(), GetSettingOverride(meta).c_str());

    widget->setToolTip(tip.c_str());
}

int HorizontalAdvance(const QFontMetrics& fm, const QString& string) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    return fm.horizontalAdvance(string);
#else
    return fm.width(string);
#endif
}

std::string GetLabel(const SettingMeta& meta) { 
    if (meta.status == STATUS_STABLE) {
        return meta.label;
    } else {
        return meta.label + " (" + GetToken(meta.status) + ")";
    }
}
