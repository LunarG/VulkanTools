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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "foldersettingwidget.h"

FolderSettingWidget::FolderSettingWidget(QTreeWidgetItem* item, LayerSettings* layer_settings) : QWidget(nullptr) {
    _layer_settings = layer_settings;

    item->setText(0, layer_settings->settings_prompt);
    item->setToolTip(0, layer_settings->settings_desc);

    _line_edit = new QLineEdit(this);
    _line_edit->setText(_layer_settings->settings_value);
    _line_edit->show();

    _push_button = new QPushButton(this);
    _push_button->setText("...");
    _push_button->show();

    connect(_push_button, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));
    connect(_line_edit, SIGNAL(textEdited(const QString&)), this, SLOT(textFieldChanged(const QString&)));
}

void FolderSettingWidget::resizeEvent(QResizeEvent* event) {
    if (_line_edit == nullptr) return;

    QSize parentSize = event->size();

    // Button takes up the last 32 pixels
    QRect buttonRect = QRect(parentSize.width() - 32, 0, 32, parentSize.height());
    QRect editRect = QRect(0, 0, parentSize.width() - 32, parentSize.height());
    _line_edit->setGeometry(editRect);
    _push_button->setGeometry(buttonRect);
}

void FolderSettingWidget::browseButtonClicked(void) {
    QString file = QFileDialog::getExistingDirectory(_push_button, tr("Select Folder"), ".");

    if (!file.isEmpty()) {
        file = QDir::toNativeSeparators(file);
        _layer_settings->settings_value = file;
        _line_edit->setText(file);
        emit itemChanged();
    }
}

void FolderSettingWidget::textFieldChanged(const QString& newText) {
    _layer_settings->settings_value = newText;
    emit itemChanged();
}
