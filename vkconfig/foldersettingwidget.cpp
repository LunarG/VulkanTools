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

#include "foldersettingwidget.h"

CFolderSettingWidget::CFolderSettingWidget(QTreeWidgetItem* pItem, TLayerSettings* pLayerSetting) : QWidget(nullptr)
{
    pSetting = pLayerSetting;

    pItem->setText(0, pLayerSetting->settingsPrompt);
    pItem->setToolTip(0, pLayerSetting->settingsDesc);

    pLineEdit = new QLineEdit(this);
    pLineEdit->setText(pSetting->settingsValue);
    pLineEdit->show();

    pPushButton = new QPushButton(this);
    pPushButton->setText("...");
    pPushButton->show();

    connect(pPushButton, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));
    connect(pLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(textFieldChanged(const QString&)));
    }


 void CFolderSettingWidget::resizeEvent(QResizeEvent *event)
    {
    if(pLineEdit == nullptr)
        return;

    QSize parentSize = event->size();

    // Button takes up the last 32 pixels
    QRect buttonRect = QRect(parentSize.width() - 32, 0, 32, parentSize.height());
    QRect editRect= QRect(0, 0, parentSize.width() - 32, parentSize.height());
    pLineEdit->setGeometry(editRect);
    pPushButton->setGeometry(buttonRect);
    }

void CFolderSettingWidget::browseButtonClicked(void)
    {
    QString file = QFileDialog::getExistingDirectory(pPushButton,
        tr("Select Folder"),
        ".");

    if(!file.isEmpty()) {
        file = QDir::toNativeSeparators(file);
        pSetting->settingsValue = file;
        pLineEdit->setText(file);
        }
    }


void CFolderSettingWidget::textFieldChanged(const QString& newText)
    {
    pSetting->settingsValue = newText;
    }
