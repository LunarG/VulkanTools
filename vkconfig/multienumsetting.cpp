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


#include "multienumsetting.h"

CMultiEnumSetting::CMultiEnumSetting(TLayerSettings *pLayerSetting, QString thisSetting)
    {
    pSetting = pLayerSetting;
    mySetting = thisSetting;

    if(pLayerSetting->settingsValue.contains(mySetting))
        this->setChecked(true);

    connect(this, SIGNAL(clicked(bool)), this, SLOT(itemChecked(bool)));
    }


void CMultiEnumSetting::itemChecked(bool bChecked)
    {
    if(bChecked)
        AddString(pSetting->settingsValue, mySetting);
    else
        RemoveString(pSetting->settingsValue, mySetting);

    emit itemChanged();
    }


