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
 *
 * Khronos layer specific common settings dialog.
 *
 */

#include <QFileDialog>
#include "khronossettings.h"
#include "ui_khronossettings.h"

/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief KhronosSettings::KhronosSettings
/// \param parent       - Scroll area on the front dialog
/// \param pSettings    - Only one set of settings is passed in
/// \param qsText       - Descriptive text of the profiles
KhronosSettings::KhronosSettings(QWidget *parent,  QVector<TLayerSettings *>& layerSettings, QString qsText) :
    QWidget(parent),
    ui(new Ui::KhronosSettings)
    {
    ui->setupUi(this);

    // Populate the controls appropriately
    ui->labelText->setText(qsText);

    // Look for Debug action
    for(int i = 0; i < layerSettings.size(); i++) {
        if(layerSettings[i]->settingsName == QString("debug_action")) { // Found
            linkedSetting.push_back(layerSettings[i]);

            int nFoundDefault = 0;
            ui->comboAction->addItem("<None>");
            for(int j = 0; j < layerSettings[i]->settingsListExclusiveValue.size(); j++) {
                ui->comboAction->addItem(layerSettings[i]->settingsListExclusivePrompt[j]);
                if(layerSettings[i]->settingsValue == layerSettings[i]->settingsListExclusiveValue[j])
                    nFoundDefault = j+1;
                }

            ui->comboAction->setCurrentIndex(nFoundDefault);
            break;
            }
        }


    // Look for the log file name
    for(int i = 0; i < layerSettings.size(); i++) {
        if(layerSettings[i]->settingsName == QString("log_filename")) {
            ui->lineEditLogFile->setText(layerSettings[i]->settingsValue);
            break;
            }
        }

    // GPU specific?


    // Messages


    // ARM specific



    ui->lineEditLogFile->setText("stdout");

    }

KhronosSettings::~KhronosSettings()
{
    delete ui;
}


////////////////////////////////////////////////////////////////
// Browse/set a log file name (usually stdout)
void KhronosSettings::on_pushButtonBrowse_clicked(void)
    {
    QString file = QFileDialog::getSaveFileName(this,
        tr("Select File"),
        ".");

    if(!file.isEmpty())
        ui->lineEditLogFile->setText(file);
    }
