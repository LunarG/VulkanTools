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
#include "widget_setting.h"

#include "../vkconfig_core/path.h"

#include <cassert>

WidgetSettingFilesystem::WidgetSettingFilesystem(QTreeWidget* tree, QTreeWidgetItem* item,
                                                 const SettingMetaFilesystem& setting_meta, SettingDataString& setting_data)
    : tree(tree), item(item), meta(setting_meta), data(setting_data), field(new QLineEdit(this)), button(new QPushButton(this)) {
    assert(item);
    assert(&setting_meta);
    assert(&setting_data);

    item->setText(0, setting_meta.label.c_str());
    item->setToolTip(0, setting_meta.description.c_str());
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));

    this->field->setText(ReplaceBuiltInVariable(setting_data.value).c_str());
    this->field->setToolTip(this->field->text());
    this->field->show();
    this->connect(this->field, SIGNAL(textEdited(const QString&)), this, SLOT(textFieldChanged(const QString&)));

    this->button->setText("...");
    this->button->show();
    this->connect(this->button, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));

    QTreeWidgetItem* child_item = new QTreeWidgetItem();
    child_item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    item->addChild(child_item);

    tree->setItemWidget(item, 0, this);
    tree->setItemWidget(child_item, 0, this->field);
}

void WidgetSettingFilesystem::Enable(bool enabled) {
    this->setEnabled(enabled);
    this->field->setEnabled(enabled);
    this->button->setEnabled(enabled);
}

void WidgetSettingFilesystem::resizeEvent(QResizeEvent* event) {
    const QSize parent_size = event->size();

    const QRect button_rect = QRect(parent_size.width() - MIN_BUTTON_SIZE, 0, MIN_BUTTON_SIZE, parent_size.height());
    this->button->setGeometry(button_rect);
}

void WidgetSettingFilesystem::browseButtonClicked() {
    std::string file;

    const char* filter = this->meta.filter.c_str();
    const char* path = this->data.value.c_str();

    switch (this->meta.type) {
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
        field->setText(file.c_str());
        this->data.value = file;
        emit itemChanged();
    }
}

void WidgetSettingFilesystem::textFieldChanged(const QString& value) {
    this->data.value = value.toStdString();
    this->field->setToolTip(this->field->text());

    emit itemChanged();
}
