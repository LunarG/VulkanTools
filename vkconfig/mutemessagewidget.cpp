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
 * This class creates a list box that displays all the filtered
 * message ids.
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */
#include "mutemessagewidget.h"

CMuteMessageWidget::CMuteMessageWidget(TLayerSettings *pLayerSetting) : QWidget(nullptr) {
    pSetting = pLayerSetting;
    pListWidget = new QListWidget(this);
    pListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pListWidget->show();
    pRemovebutton = new QPushButton(this);
    pRemovebutton->setText("Remove");
    pRemovebutton->show();

    // Load with existing settings
    if (!pSetting->settingsValue.isEmpty()) {
        QStringList list = pSetting->settingsValue.split(",");
        pListWidget->addItems(list);
        pListWidget->setCurrentRow(pListWidget->count() - 1);
    } else
        pRemovebutton->setEnabled(false);

    connect(pRemovebutton, SIGNAL(pressed()), this, SLOT(removePushed()));
}

void CMuteMessageWidget::resizeEvent(QResizeEvent *event) {
    int nButtonHeight = 26;
    QSize parentSize = event->size();
    pListWidget->setGeometry(0, 0, parentSize.width(), parentSize.height() - nButtonHeight);
    pRemovebutton->setGeometry(0, parentSize.height() - nButtonHeight, parentSize.width(), nButtonHeight);
}

void CMuteMessageWidget::addItem(QString &item) {
    pListWidget->addItem(item);
    pListWidget->setCurrentRow(pListWidget->count() - 1);

    // Update Setting
    AddString(pSetting->settingsValue, item);
    pRemovebutton->setEnabled(true);
}

void CMuteMessageWidget::removePushed(void) {
    int nRow = pListWidget->currentRow();
    if (nRow <= 0) return;

    QString itemName = pListWidget->currentItem()->text();
    pListWidget->takeItem(nRow);

    // Update Setting
    RemoveString(pSetting->settingsValue, itemName);
}
