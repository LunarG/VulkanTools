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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "widget_setting_string.h"
#include "widget_setting.h"

#include <cassert>

static const int MIN_FIELD_WIDTH = 120;

WidgetSettingString::WidgetSettingString(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaString& meta,
                                         SettingDataSet& data_set)
    : WidgetSettingBase(tree, item), meta(meta), data_set(data_set), field(new QLineEdit(this)) {
    assert(&meta);

    this->field->setText(this->data().value.c_str());
    this->field->setFont(tree->font());
    this->field->setToolTip(this->field->text());
    this->field->show();

    this->connect(this->field, SIGNAL(textEdited(const QString&)), this, SLOT(OnTextEdited(const QString&)));

    this->item->setText(0, GetLabel(this->meta).c_str());
    this->item->setFont(0, this->tree->font());
    this->item->setToolTip(0, meta.description.c_str());
    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    this->item->setExpanded(this->meta.expanded);
    this->tree->setItemWidget(this->item, 0, this);

    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void WidgetSettingString::Refresh(RefreshAreas refresh_areas) {
    const SettingDependenceMode enabled = ::CheckDependence(this->meta, data_set);

    this->item->setHidden(enabled == SETTING_DEPENDENCE_HIDE);
    this->item->setDisabled(enabled != SETTING_DEPENDENCE_ENABLE);
    this->field->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);
    this->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);

    if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
        if (::CheckSettingOverridden(this->meta)) {
            this->DisplayOverride(this->field, this->meta);
        }

        this->field->setText(this->data().value.c_str());
    }
}

void WidgetSettingString::Resize() {
    const QFontMetrics fm = this->field->fontMetrics();
    const int width = std::max(HorizontalAdvance(fm, this->field->text() + "00"), MIN_FIELD_WIDTH);

    const QRect field_rect = QRect(this->resize.width() - width, 0, width, this->resize.height());
    this->field->setGeometry(field_rect);
}

void WidgetSettingString::resizeEvent(QResizeEvent* event) {
    this->resize = event->size();
    this->Resize();
}

void WidgetSettingString::OnTextEdited(const QString& value) {
    this->data().value = value.toStdString();
    this->field->setToolTip(this->field->text());

    emit itemChanged();
}

SettingDataString& WidgetSettingString::data() {
    SettingDataString* data = FindSetting<SettingDataString>(this->data_set, this->meta.key.c_str());
    assert(data != nullptr);
    return *data;
}
