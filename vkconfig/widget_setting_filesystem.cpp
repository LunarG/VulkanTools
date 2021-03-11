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
 * - Lenny Komow  <lenny@lunarg.com>
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "widget_setting_filesystem.h"

#include "../vkconfig_core/path.h"

#include <cassert>

static const int MIN_BUTTON_SIZE = 24;

WidgetSettingFilesystem::WidgetSettingFilesystem(QTreeWidgetItem* item, const SettingMetaFilesystem& setting_meta,
                                                 SettingDataString& setting_data)
    : QWidget(nullptr), setting_meta(setting_meta), setting_data(setting_data), _line_edit(nullptr), _push_button(nullptr) {
    assert(item);
    assert(&setting_meta);
    assert(&setting_data);

    item->setText(0, setting_meta.label.c_str());
    item->setToolTip(0, setting_meta.description.c_str());

    this->_line_edit = new QLineEdit(this);
    this->_line_edit->setText(ReplaceBuiltInVariable(setting_data.value).c_str());
    this->_line_edit->show();

    this->_push_button = new QPushButton(this);
    this->_push_button->setText("...");
    this->_push_button->show();

    connect(this->_push_button, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));
    connect(this->_line_edit, SIGNAL(textEdited(const QString&)), this, SLOT(textFieldChanged(const QString&)));
}

void WidgetSettingFilesystem::resizeEvent(QResizeEvent* event) {
    if (this->_line_edit == nullptr) return;

    const QSize parent_size = event->size();

    // Button takes up the last 32 pixels
    const QRect edit_rect = QRect(0, 0, parent_size.width() - MIN_BUTTON_SIZE, parent_size.height());
    this->_line_edit->setGeometry(edit_rect);

    const QRect button_rect = QRect(parent_size.width() - MIN_BUTTON_SIZE, 0, MIN_BUTTON_SIZE, parent_size.height());
    this->_push_button->setGeometry(button_rect);
}

void WidgetSettingFilesystem::browseButtonClicked() {
    std::string file;

    switch (this->setting_meta.type) {
        case SETTING_LOAD_FILE:
            file = QFileDialog::getOpenFileName(this->_push_button, "Select file", this->_line_edit->text(),
                                                this->setting_meta.filter.c_str())
                       .toStdString();
            break;
        case SETTING_SAVE_FILE:
            file = QFileDialog::getSaveFileName(this->_push_button, "Select File", this->_line_edit->text(),
                                                this->setting_meta.filter.c_str())
                       .toStdString();
            break;
        case SETTING_SAVE_FOLDER:
            file = QFileDialog::getExistingDirectory(this->_push_button, "Select Folder", this->_line_edit->text()).toStdString();
            break;
        default:
            assert(0);
            break;
    }

    if (!file.empty()) {
        file = ConvertNativeSeparators(file);
        _line_edit->setText(file.c_str());
        this->setting_data.value = file;
        emit itemChanged();
    }
}

void WidgetSettingFilesystem::textFieldChanged(const QString& new_text) {
    this->setting_data.value = new_text.toStdString();
    emit itemChanged();
}
