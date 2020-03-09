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
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#include <QLabel>

#include "csettingseditor.h"

CSettingsEditor::CSettingsEditor()
    {

    }


///////////////////////////////////////////////////////////////////////////////////////////////
// Creates controls and sets up any signals
void CSettingsEditor::CreateGUI(QWidget *pDestination, QVector<TLayerSettings *>& layerSettings)
    {
    int nRowHeight = 20;
    int nCurrRow = 50;
    int nLeftColumn = 26;

    for(int i = 0; i < layerSettings.size(); i++) {
        QLabel *pLabel = new QLabel(pDestination);
        pLabel->setText("Dynamic GUI...\n");
        pLabel->setGeometry(nLeftColumn, nCurrRow, 200, 24);
        nCurrRow += nRowHeight;
        pLabel->show();
        prompts.push_back(pLabel);

        }




    }

///////////////////////////////////////////////////////////////////////////////////////////////
void CSettingsEditor::CleanupGUI(void)
    {
    for(int i = 0; i < inputControls.size(); i++) {
        delete inputControls[i];
        delete prompts[i];
        }

    inputControls.clear();
    prompts.clear();
    }

