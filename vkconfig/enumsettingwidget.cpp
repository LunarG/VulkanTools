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

#include "enumsettingwidget.h"

CEnumSettingWidget::CEnumSettingWidget(QTreeWidgetItem* pTreeItem, TLayerSettings *pLayerSetting)
    {
    pSetting = pLayerSetting;
    pTreeItem->setText(0, pLayerSetting->settingsPrompt);
    pTreeItem->setToolTip(0, pLayerSetting->settingsDesc);

    int nCurrSel = 0;
    for(int i = 0; i < pLayerSetting->settingsListExclusivePrompt.size(); i++) {
        this->addItem(pLayerSetting->settingsListExclusivePrompt[i]);
        if(pLayerSetting->settingsListExclusiveValue[i] == pLayerSetting->settingsValue)
            nCurrSel = i;
        }

    setCurrentIndex(nCurrSel);

    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
    }


void CEnumSettingWidget::indexChanged(int nIndex)
    {
    pSetting->settingsValue = pSetting->settingsListExclusiveValue[nIndex];
    }
