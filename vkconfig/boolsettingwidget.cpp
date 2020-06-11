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

#include "boolsettingwidget.h"

CBoolSettingWidget::CBoolSettingWidget(TLayerSettings *pLayerSetting, bool bNumeric)
    {
    bNumericOutput = bNumeric;
    pSetting = pLayerSetting;
    setText(pLayerSetting->settingsPrompt);
    setToolTip(pLayerSetting->settingsDesc);
    setChecked(pLayerSetting->settingsValue == QString("TRUE"));
    connect(this, SIGNAL(clicked()), this, SLOT(itemToggled()));
    }


void CBoolSettingWidget::itemToggled(void)
    {
    if(bNumericOutput) {
        if(isChecked())
            pSetting->settingsValue = QString("1");
        else
            pSetting->settingsValue = QString("0");
        }
    else {
        if(isChecked())
            pSetting->settingsValue = QString("TRUE");
        else
            pSetting->settingsValue = QString("FALSE");
        }
    }
