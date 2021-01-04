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
 * - Lenny Komow  <lenny@lunarg.com>
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "widget_filesystem_setting.h"

#include "../vkconfig_core/path.h"

#include <cassert>

// This can be used to specify a 'load' file or a 'save' file. Save is true by default
FileSystemSettingWidget::FileSystemSettingWidget(QTreeWidgetItem* item, const LayerSettingMeta& layer_setting_meta,
                                                 LayerSettingData& layer_setting_data)
    : QWidget(nullptr),
      _mode(GetMode(layer_setting_meta.type)),
      layer_setting_meta(layer_setting_meta),
      layer_setting_data(layer_setting_data),
      _line_edit(nullptr),
      _push_button(nullptr) {
    assert(item);
    assert(&layer_setting_meta);
    assert(&layer_setting_data);

    item->setText(0, layer_setting_meta.label);
    item->setToolTip(0, layer_setting_meta.description);

    _line_edit = new QLineEdit(this);
    _line_edit->setText(ReplaceBuiltInVariable(layer_setting_data.value).c_str());
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
            file = QFileDialog::getOpenFileName(_push_button, "Select file", _line_edit->text());
            break;
        case MODE_SAVE_FILE:
            file = QFileDialog::getSaveFileName(_push_button, "Select File", _line_edit->text());
            break;
        case MODE_SAVE_FOLDER:
            file = QFileDialog::getExistingDirectory(_push_button, "Select Folder", _line_edit->text());
            break;
        default:
            assert(0);
            break;
    }

    if (!file.isEmpty()) {
        file = ConvertNativeSeparators(file.toStdString()).c_str();
        _line_edit->setText(file);
        layer_setting_data.value = file.toStdString();
        emit itemChanged();
    }
}

void FileSystemSettingWidget::textFieldChanged(const QString& new_text) {
    layer_setting_data.value = new_text.toStdString();
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
