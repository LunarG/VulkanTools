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

#include "widget_setting_filesystem.h"

#include "../vkconfig_core/path.h"

#include <cassert>

#include <QFileDialog>

static const int MIN_FIELD_WIDTH = 64;
static const int MIN_BUTTON_SIZE = 24;
static const int ITEM_OFFSET = 48;

WidgetSettingFilesystem::WidgetSettingFilesystem(QTreeWidget* tree, QTreeWidgetItem* parent,
                                                 const SettingMetaFilesystem& setting_meta, SettingDataString& setting_data)
    : WidgetSetting(tree, parent, setting_meta),
      setting_meta(setting_meta),
      setting_data(setting_data),
      button(new QPushButton(this)),
      title_field(new QLineEdit(this)),
      child_field(new QLineEdit(this)),
      child_item(new QTreeWidgetItem()) {
    assert(&setting_data);

    this->child_item->setSizeHint(0, QSize(0, 24));
    this->item->addChild(this->child_item);
    this->item->setExpanded(!this->setting_data.collapsed);

    this->button->setText("...");
    this->button->show();

    this->title_field->setText(ReplaceBuiltInVariable(setting_data.value).c_str());
    this->title_field->setToolTip(this->title_field->text());
    this->setting_data.collapsed ? this->title_field->show() : this->title_field->hide();
    this->tree->setItemWidget(this->item, 0, this);

    this->child_field->setText(ReplaceBuiltInVariable(setting_data.value).c_str());
    this->child_field->setToolTip(this->child_field->text());
    this->child_field->show();
    this->tree->setItemWidget(this->child_item, 0, this->child_field);

    this->connect(this->button, SIGNAL(clicked()), this, SLOT(OnButtonClicked()));
    this->connect(this->title_field, SIGNAL(textEdited(const QString&)), this, SLOT(OnTextEdited(const QString&)));
    this->connect(this->child_field, SIGNAL(textEdited(const QString&)), this, SLOT(OnTextEdited(const QString&)));
    this->connect(this->item->treeWidget(), SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(OnItemExpanded(QTreeWidgetItem*)));
    this->connect(this->item->treeWidget(), SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(OnItemCollapsed(QTreeWidgetItem*)));
}

void WidgetSettingFilesystem::Enable(bool enable) {
    ::EnableItem(this->item, enable);
    ::EnableItem(this->child_item, enable);
    this->button->setEnabled(enable);
    this->title_field->setEnabled(enable);
    this->child_field->setEnabled(enable);
}

void WidgetSettingFilesystem::resizeEvent(QResizeEvent* event) {
    const QSize parent_size = event->size();

    const QFontMetrics fm = this->title_field->fontMetrics();
    const int width = std::max(fm.horizontalAdvance(this->title_field->text() + "00") + ITEM_OFFSET, MIN_FIELD_WIDTH);

    const QRect edit_rect = QRect(width, 0, parent_size.width() - MIN_BUTTON_SIZE - width, parent_size.height());
    this->title_field->setGeometry(edit_rect);

    const QRect button_rect = QRect(parent_size.width() - MIN_BUTTON_SIZE, 0, MIN_BUTTON_SIZE, parent_size.height());
    this->button->setGeometry(button_rect);
}

void WidgetSettingFilesystem::OnButtonClicked() {
    std::string file;
    const char* filter = this->setting_meta.filter.c_str();
    const char* path = this->setting_data.value.c_str();

    switch (this->setting_meta.type) {
        case SETTING_LOAD_FILE:
            file = QFileDialog::getOpenFileName(this->button, "Select file", path, filter).toStdString();
            break;
        case SETTING_SAVE_FILE:
            file = QFileDialog::getSaveFileName(this->button, "Select File", path, filter).toStdString();
            break;
        case SETTING_SAVE_FOLDER:
            file = QFileDialog::getExistingDirectory(this->button, "Select Folder", path).toStdString();
            break;
        default:
            assert(0);
            break;
    }

    if (!file.empty()) {
        file = ConvertNativeSeparators(file);
        this->title_field->setText(file.c_str());
        this->child_field->setText(file.c_str());
        this->setting_data.value = file;
        emit itemChanged();
    }
}

void WidgetSettingFilesystem::OnTextEdited(const QString& value) {
    this->setting_data.value = value.toStdString();
    emit itemChanged();
}

void WidgetSettingFilesystem::OnItemExpanded(QTreeWidgetItem* expanded_item) {
    if (this->item != expanded_item) return;

    this->setting_data.collapsed = false;
    this->title_field->hide();
    return;
}

void WidgetSettingFilesystem::OnItemCollapsed(QTreeWidgetItem* expanded_item) {
    if (this->item != expanded_item) return;

    this->setting_data.collapsed = true;
    this->title_field->show();
    return;
}
