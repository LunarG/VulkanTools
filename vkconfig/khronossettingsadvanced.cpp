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
 * Khronos layer advanced settings dialog.
 *
 */

#include "khronossettingsadvanced.h"
#include "ui_khronossettingsadvanced.h"


struct TREE_SETTING { QString prompt;
         QString token;
         QTreeWidgetItem *pItem;
       };



static TREE_SETTING coreChecks[7] = { { "Image Layout Validation", "VALIDATION_CHECK_DISABLE_IMAGE_LAYOUT_VALIDATION", nullptr},
                       {"Command Buffer State", "VALIDATION_CHECK_DISABLE_COMMAND_BUFFER_STATE", nullptr},
                       {"Object in Use", "VALIDATION_CHECK_DISABLE_OBJECT_IN_USE", nullptr},
                       {"Query Validation", "VALIDATION_CHECK_DISABLE_QUERY_VALIDATION", nullptr},
                       {"Idle Descriptor Set", "VALIDATION_CHECK_DISABLE_IDLE_DESCRIPTOR_SET", nullptr},
                       {"Shader Validation Checks", "VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT", nullptr},
                       {"Push Constant Range", "VALIDATION_CHECK_DISABLE_PUSH_CONSTANT_RANGE", nullptr}};


static TREE_SETTING miscDisables[4] = { { "Thread Safety Checks", "VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT", nullptr},
                               {"Handle Wrapping", "VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT", nullptr},
                               {"Object Lifetime Validation", "VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT", nullptr},
                               {"Stateless Parameter Checks", "VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT", nullptr}};


static TREE_SETTING bestPractices[2] = { {  "Best Practices Warning Checks", "VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT", nullptr},
                               { "ARM Specific Validation", "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM", nullptr}};



KhronosSettingsAdvanced::KhronosSettingsAdvanced(QWidget *parent,  QVector<TLayerSettings *>& layerSettings, QString qsText) :
    QWidget(parent),
    ui(new Ui::KhronosSettingsAdvanced)
    {
    (void)qsText; // ???? Sure about this?
    ui->setupUi(this);
    ui->labelHideMessage->setVisible(true);

    // Find the enables
    pEnables = nullptr;
    for(int i = 0; i < layerSettings.size(); i++)
        if(layerSettings[i]->settingsName == QString("enables")) {
            pEnables = layerSettings[i];
            break;
            }
    Q_ASSERT(pEnables != nullptr);

    // Find the disables
    pDisables = nullptr;
    for(int i = 0; i < layerSettings.size(); i++)
        if(layerSettings[i]->settingsName == QString("disables")) {
            pDisables = layerSettings[i];
            break;
        }
    Q_ASSERT(pEnables != nullptr);

    QTreeWidgetItem *pHeader = ui->treeWidget->headerItem();
    pHeader->setText(0, "Validation Settings");

    ///////////////////////////////////////////////////////////////
    /// \brief pCoreChecks
    /// If this is off, everyone below is disabled
    bool bCoreValidation = !pDisables->settingsValue.contains("VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT");
    pCoreChecksParent = new QTreeWidgetItem();
    pCoreChecksParent->setText(0, "Core Validation Checks");
    pCoreChecksParent->setCheckState(0, (bCoreValidation) ? Qt::Checked : Qt::Unchecked);
    ui->treeWidget->addTopLevelItem(pCoreChecksParent);


    QTreeWidgetItem *pChild;
    for(int i = 0; i < 7; i++) {
        pChild = new QTreeWidgetItem();
        pChild->setText(0, coreChecks[i].prompt);
        if(pDisables->settingsValue.contains(coreChecks[i].token))
            pChild->setCheckState(0, Qt::Unchecked);
        else
            pChild->setCheckState(0, Qt::Checked);

        if(!bCoreValidation)
            pChild->setFlags(pChild->flags() & ~Qt::ItemIsEnabled);

        pCoreChecksParent->addChild(pChild);
        coreChecks[i].pItem = pChild;
        }

    //////////////////////////////////////////////////////////////////////

    // Best Practices - one parent/child, but we want to be able
    // to go back to these
    pChild = new QTreeWidgetItem();
    pChild->setText(0, bestPractices[1].prompt);
    if(pEnables->settingsValue.contains(bestPractices[1].token))
        pChild->setCheckState(0, Qt::Checked);
    else
        pChild->setCheckState(0, Qt::Unchecked);


    QTreeWidgetItem *pItem = new QTreeWidgetItem();
    pItem->setText(0, bestPractices[0].prompt);
    if(pEnables->settingsValue.contains(bestPractices[0].token))
        pItem->setCheckState(0, Qt::Checked);
    else {
        pItem->setCheckState(0, Qt::Unchecked);
        pChild->setFlags(pChild->flags() & ~Qt::ItemIsEnabled);
        }

    bestPractices[0].pItem = pItem;
    ui->treeWidget->addTopLevelItem(pItem);

    pItem->addChild(pChild);
    bestPractices[1].pItem = pChild;

    ///////////////////////////////////////////////////////////////
    // Miscellaneous disables
    for(int i = 0; i <4; i++) {
        pItem = new QTreeWidgetItem();
        pItem->setText(0, miscDisables[i].prompt);
        if(pDisables->settingsValue.contains(miscDisables[i].token))
            pItem->setCheckState(0, Qt::Unchecked);
        else
            pItem->setCheckState(0, Qt::Checked);

        ui->treeWidget->addTopLevelItem(pItem);
        miscDisables[i].pItem = pItem;
        }

    ///////////////////////////////////////////////////////////////
    // Now for the GPU specific stuff
    if(pEnables->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT"))
        ui->radioButtonDebug->setChecked(true);
    else
        ui->radioButtonGPU->setChecked(true);   // This way one is the default

    if(pEnables->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT"))
        ui->checkReserve->setChecked(true);

    if(!pEnables->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT") &&
            !pEnables->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT"))
        ui->groupBoxGPU->setChecked(false);

    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemChanged(QTreeWidgetItem*, int)));
    connect(ui->radioButtonGPU, SIGNAL(toggled(bool)), this, SLOT(gpuToggled(bool)));
    connect(ui->radioButtonDebug, SIGNAL(toggled(bool)), this, SLOT(printfToggled(bool)));
    }

KhronosSettingsAdvanced::~KhronosSettingsAdvanced()
    {
    delete ui;
    }


void KhronosSettingsAdvanced::SetEnabled(bool bEnabled)
    {
    ui->labelHideMessage->setVisible(!bEnabled);
    ui->treeWidget->setVisible(bEnabled);
    ui->groupBoxGPU->setVisible(bEnabled);
    }

///////////////////////////////////////////////////////////////////////////////
/// \brief KhronosSettingsAdvanced::itemChanged
/// \param pItem
/// \param nColumn
/// Something was checked or unchecked
void KhronosSettingsAdvanced::itemChanged(QTreeWidgetItem *pItem, int nColumn)
    {
    if(nColumn != 0)
        return;

    // Most toggles we ignore. A few are important.


    // Best Practices?
    if(pItem == bestPractices[0].pItem)
        {
        // If we turned it on, we need to enable AMD
        if(pItem->checkState(0) == Qt::Checked)
            bestPractices[1].pItem->setFlags(bestPractices[1].pItem->flags() | Qt::ItemIsEnabled);
        else {
            bestPractices[1].pItem->setFlags(bestPractices[1].pItem->flags() & ~Qt::ItemIsEnabled);
            bestPractices[1].pItem->setCheckState(0, Qt::Unchecked);
            }
        }

    // Core Validation.
    if(pItem == pCoreChecksParent) {
        // If checked, enable all below it.
        if(pItem->checkState(0) == Qt::Checked) {
            for(int i = 0; i < 7; i++) {
                coreChecks[i].pItem->setFlags(coreChecks[i].pItem->flags() | Qt::ItemIsEnabled);
                coreChecks[i].pItem->setCheckState(0, Qt::Checked);
                }
            } else {     // If unchecked both clear, and disable all below it
                for(int i = 0; i < 7; i++) {
                 coreChecks[i].pItem->setFlags(coreChecks[i].pItem->flags() & ~Qt::ItemIsEnabled);
                 coreChecks[i].pItem->setCheckState(0, Qt::Unchecked);
                 }
             }
        }
    }


void KhronosSettingsAdvanced::gpuToggled(bool toggle)
    {
    if(toggle)
        ui->checkReserve->setEnabled(true);
    }

void KhronosSettingsAdvanced::printfToggled(bool toggle)
    {
    if(toggle)
        ui->checkReserve->setEnabled(false);
    }

///////////////////////////////////////////////////////////////////////////
/// \brief KhronosSettingsAdvanced::CollectSettings
/// \return
/// Collect all the settings
bool KhronosSettingsAdvanced::CollectSettings(void)
    {
    QString enables;
    QString disables;

    // GPU stuff
    if(ui->groupBoxGPU->isChecked()) {
        if(ui->radioButtonGPU->isChecked()) {
            enables ="VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT";

            if(ui->checkReserve->isChecked())
                enables += ",VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT";
            }
         else  // Debug printf must be checked
            enables = "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT";
        }

    // The rest is cake... just reap the controls

    // Best practice enables
    for(int i = 0; i < 2; i++)
        if(bestPractices[i].pItem->checkState(0) == Qt::Checked)
            AddString(enables, bestPractices[i].token);

    ///////////////////////////////////////////////////////
    // Everything else is a disable. Remember, these are backwards
    // because they are exposed to the end user as an enable.
    // If they are NOT checked, we add them to disables
    for(int i = 0; i < 4; i++) {
        if(miscDisables[i].pItem->checkState(0) != Qt::Checked)
            AddString(disables, miscDisables[i].token);
            }

    // Core checks. If unchecked, then individual ones might still be checked
    if(pCoreChecksParent->checkState(0) == Qt::Checked) {
        for(int i = 0; i < 7; i++)
            if(coreChecks[i].pItem->checkState(0) == Qt::Unchecked)
                AddString(disables, coreChecks[i].token);
        }
    else //Not checked, turn them all off
        AddString(disables, "VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT");

    pDisables->settingsValue = disables;
    pEnables->settingsValue = enables;

    return true;
    }

