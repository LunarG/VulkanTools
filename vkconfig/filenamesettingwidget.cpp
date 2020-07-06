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
 * This class creates an edit control that is paired with a setting
 * in a profile.
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#include "filenamesettingwidget.h"

FilenameSettingWidget::FilenameSettingWidget(QTreeWidgetItem* pItem, LayerSettings* pLayerSetting) : QWidget(nullptr) {
    layer_settings_ = pLayerSetting;

    pItem->setText(0, pLayerSetting->settings_prompt);
    pItem->setToolTip(0, pLayerSetting->settings_desc);

    line_edit_ = new QLineEdit(this);
    line_edit_->setText(layer_settings_->settings_value);
    line_edit_->show();

    push_button_ = new QPushButton(this);
    push_button_->setText("...");
    push_button_->show();

    connect(push_button_, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));
    connect(line_edit_, SIGNAL(textEdited(const QString&)), this, SLOT(textFieldChanged(const QString&)));
}

void FilenameSettingWidget::resizeEvent(QResizeEvent* event) {
    if (line_edit_ == nullptr) return;

    QSize parentSize = event->size();

    // Button takes up the last 32 pixels
    QRect buttonRect = QRect(parentSize.width() - 32, 0, 32, parentSize.height());
    QRect editRect = QRect(0, 0, parentSize.width() - 32, parentSize.height());
    line_edit_->setGeometry(editRect);
    push_button_->setGeometry(buttonRect);
}

void FilenameSettingWidget::browseButtonClicked(void) {
    QString file = QFileDialog::getSaveFileName(push_button_, tr("Select File"), ".");

    if (!file.isEmpty()) {
        file = QDir::toNativeSeparators(file);
        layer_settings_->settings_value = file;
        line_edit_->setText(file);
        emit itemChanged();
    }
}

void FilenameSettingWidget::textFieldChanged(const QString& newText) {
    layer_settings_->settings_value = newText;
    emit itemChanged();
}
