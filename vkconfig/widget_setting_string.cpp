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

#include <cassert>

static const int MIN_FIELD_WIDTH = 64;
static const int ITEM_OFFSET = 48;

WidgetSettingString::WidgetSettingString(QTreeWidget* tree, QTreeWidgetItem* parent, const SettingMetaString& setting_meta,
                                         SettingDataString& setting_data)
    : WidgetSetting(tree, parent, setting_meta),
      setting_meta(setting_meta),
      setting_data(setting_data),
      title_field(new QLineEdit(this)),
      child_field(new QLineEdit(this)),
      child_item(new QTreeWidgetItem()) {
    assert(&setting_data);

    this->child_item->setSizeHint(0, QSize(0, 24));
    this->item->addChild(this->child_item);
    this->item->setExpanded(!this->setting_data.collapsed);

    this->title_field->setText(setting_data.value.c_str());
    this->title_field->setToolTip(this->title_field->text());
    this->setting_data.collapsed ? this->title_field->show() : this->title_field->hide();
    this->tree->setItemWidget(this->item, 0, this);

    this->child_field->setText(setting_data.value.c_str());
    this->child_field->setToolTip(this->title_field->text());
    this->child_field->show();
    this->tree->setItemWidget(this->child_item, 0, this->child_field);

    this->connect(this->title_field, SIGNAL(textEdited(const QString&)), this, SLOT(OnTextEdited(const QString&)));
    this->connect(this->child_field, SIGNAL(textEdited(const QString&)), this, SLOT(OnTextEdited(const QString&)));
    this->connect(this->item->treeWidget(), SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(OnItemExpanded(QTreeWidgetItem*)));
    this->connect(this->item->treeWidget(), SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(OnItemCollapsed(QTreeWidgetItem*)));
}

void WidgetSettingString::Resize() {
    const QFontMetrics fm = this->title_field->fontMetrics();
    const int width = std::max(fm.horizontalAdvance(this->item->text(0) + "00") + ITEM_OFFSET, MIN_FIELD_WIDTH);

    const QRect button_rect = QRect(128, 0, this->resize.width() - 128, this->resize.height());
    this->title_field->setGeometry(button_rect);
}

void WidgetSettingString::resizeEvent(QResizeEvent* event) {
    this->resize = event->size();
    this->Resize();
}

void WidgetSettingString::OnTextEdited(const QString& value) {
    this->setting_data.value = value.toStdString();

    this->Resize();

    emit itemChanged();
}

void WidgetSettingString::OnItemExpanded(QTreeWidgetItem* expanded_item) {
    if (this->item != expanded_item) return;

    this->setting_data.collapsed = false;
    this->title_field->hide();
    return;
}

void WidgetSettingString::OnItemCollapsed(QTreeWidgetItem* expanded_item) {
    if (this->item != expanded_item) return;

    this->setting_data.collapsed = true;
    this->title_field->show();
    return;
}
