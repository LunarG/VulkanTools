/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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
 * - Lenny Komow
 * - Richard S. Wright Jr.
 * - Christophe Riccio
 */

#include "widget_filesystem_setting.h"

#include <cassert>

////////////////////////////////////////////////////////////////////////////
// This can be used to specify a 'load' file or a 'save' file. Save is true by default
FileSystemSettingWidget::FileSystemSettingWidget(QTreeWidgetItem* item, LayerSetting& layer_setting, SettingType setting_type)
    : QWidget(nullptr), _layer_setting(layer_setting), _mode(GetMode(setting_type)) {
    assert(item);
    assert(&_layer_setting);
    assert(setting_type >= SETTING_FIRST && setting_type <= SETTING_LAST);

    item->setText(0, layer_setting.label);
    item->setToolTip(0, layer_setting.description);

    _line_edit = new QLineEdit(this);
    _line_edit->setText(_layer_setting.value);
    _line_edit->show();

    _push_button = new QPushButton(this);
    _push_button->setText("...");
    _push_button->show();

    connect(_push_button, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));
    connect(_line_edit, SIGNAL(textEdited(const QString&)), this, SLOT(textFieldChanged(const QString&)));
}

void FileSystemSettingWidget::resizeEvent(QResizeEvent* event) {
    if (_line_edit == nullptr) return;

    const QSize parent_size = event->size();

    // Button takes up the last 32 pixels
    const QRect edit_rect = QRect(0, 0, parent_size.width() - 32, parent_size.height());
    _line_edit->setGeometry(edit_rect);

    const QRect button_rect = QRect(parent_size.width() - 32, 0, 32, parent_size.height());
    _push_button->setGeometry(button_rect);
}

void FileSystemSettingWidget::browseButtonClicked() {
    QString file;

    switch (_mode) {
        case MODE_OPEN_FILE:
            file = QFileDialog::getOpenFileName(_push_button, "Select file", ".");
            break;
        case MODE_SAVE_FILE:
            file = QFileDialog::getSaveFileName(_push_button, "Select File", ".");
            break;
        case MODE_SAVE_FOLDER:
            file = QFileDialog::getExistingDirectory(_push_button, "Select Folder", ".");
            break;
        default:
            assert(0);
            break;
    }

    if (!file.isEmpty()) {
        file = QDir::toNativeSeparators(file);
        _layer_setting.value = file;
        _line_edit->setText(file);
        emit itemChanged();
    }
}

void FileSystemSettingWidget::textFieldChanged(const QString& new_text) {
    _layer_setting.value = new_text;
    emit itemChanged();
}

FileSystemSettingWidget::Mode FileSystemSettingWidget::GetMode(SettingType type) const {
    switch (type) {
        case SETTING_SAVE_FILE:
            return MODE_SAVE_FILE;
        case SETTING_LOAD_FILE:
            return MODE_OPEN_FILE;
        case SETTING_SAVE_FOLDER:
            return MODE_SAVE_FOLDER;
        default:
            assert(0);
            return static_cast<Mode>(-1);
    }
}
