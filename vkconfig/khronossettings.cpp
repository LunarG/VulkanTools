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
    bGPU = false;

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
            linkedSetting.push_back(layerSettings[i]);
            ui->lineEditLogFile->setText(layerSettings[i]->settingsValue);
            break;
            }
        }

    // GPU specific? Auto detect it's presense
    for(int i = 0; i < layerSettings.size(); i++) {
        if(layerSettings[i]->settingsName == QString("enables")) {
            linkedSetting.push_back(layerSettings[i]);

            if(!(layerSettings[i]->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT") ||
                 layerSettings[i]->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT"))) {
                ui->groupBoxGPU->setVisible(false);
                bGPU = false;
                break;
                }

            bGPU = true;

            // We are either GPU or debugprintf
            if(layerSettings[i]->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT")) {
                ui->radioButtonDebug->setChecked(true);
                ui->checkReserve->setCheckState(Qt::Unchecked);
                ui->checkReserve->setEnabled(false);
                }
            else { // GPU assisted
                ui->radioButtonGPU->setChecked(true);
                ui->checkReserve->setEnabled(true);
                ui->checkReserve->setChecked(layerSettings[i]->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT"));
                }

            break;
            }
        }


    // Messages
    for(int i = 0; i < layerSettings.size(); i++) {
        if(layerSettings[i]->settingsName == QString("report_flags")) {
            linkedSetting.push_back(layerSettings[i]);
            ui->checkWarning->setChecked(layerSettings[i]->settingsValue.contains("warn"));
            ui->checkInfo->setChecked(layerSettings[i]->settingsValue.contains("info"));
            ui->checkDebug->setChecked(layerSettings[i]->settingsValue.contains("debug"));
            ui->checkError->setChecked(layerSettings[i]->settingsValue.contains("error"));
            ui->checkPerformance->setChecked(layerSettings[i]->settingsValue.contains("perf"));
            break;
            }
        }

    // ARM specific - We need a flag for this
    for(int i = 0; i < layerSettings.size(); i++)
        if(layerSettings[i]->settingsName == QString("enables")) {
            if(!layerSettings[i]->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT")) {
                ui->checkARM->setVisible(false);
                break;
                }

            //linkedSetting.push_back(layerSettings[i]); // This is already in the list
            ui->checkARM->setChecked(layerSettings[i]->settingsValue.contains("VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM"));
            break;
            }
    }

KhronosSettings::~KhronosSettings()
    {
    delete ui;
    }


void KhronosSettings::on_radioButtonDebug_clicked(void)
    {
    ui->checkReserve->setChecked(false);
    ui->checkReserve->setEnabled(false);
    }


void KhronosSettings::on_radioButtonGPU_clicked(void)
    {
    ui->checkReserve->setEnabled(true);
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


////////////////////////////////////////////////////////////////
/// \brief KhronosSettings::CollectSettings
/// Okay, get the settings
bool KhronosSettings::CollectSettings(void)
    {
    // Debug action
    int nSel = ui->comboAction->currentIndex();
    if(nSel == 0)
        linkedSetting[0]->settingsValue = ""; // None
    else
        linkedSetting[0]->settingsValue = linkedSetting[0]->settingsListExclusiveValue[nSel -1];

    // Log file name
    linkedSetting[1]->settingsValue = ui->lineEditLogFile->text();

    // GPU
    if(bGPU) {
        if(ui->radioButtonGPU->isChecked()) {
            AddString(linkedSetting[2]->settingsValue, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT");
            RemoveString(linkedSetting[2]->settingsValue, "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT");
            if(ui->checkReserve)
                AddString(linkedSetting[2]->settingsValue, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");
            else
                RemoveString(linkedSetting[2]->settingsValue, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");
            }
        else {  // Debug Printf
            RemoveString(linkedSetting[2]->settingsValue, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT");
            AddString(linkedSetting[2]->settingsValue, "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT");
            RemoveString(linkedSetting[2]->settingsValue, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");
            }
        }

    // Messages
    QStringList messages;
    if(ui->checkInfo->isChecked())
        messages << "info";

    if(ui->checkError->isChecked())
        messages << "error";

    if(ui->checkWarning->isChecked())
        messages << "warn";

    if(ui->checkDebug->isChecked())
        messages << "debug";

    if(ui->checkPerformance->isChecked())
        messages << "perf";

    linkedSetting[3]->settingsValue = messages.join(",");

    // ARM setting
    // (this is the same setting slot as the GPU stuff)
    if(ui->checkARM->isChecked())
       // Make sure arm is included
        // We can tack it onto the end because we know it also contains VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT
        AddString(linkedSetting[2]->settingsValue, "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM");
    else
        // Make sure arm is not included
        RemoveString(linkedSetting[2]->settingsValue, "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM");

    return true;
    }
