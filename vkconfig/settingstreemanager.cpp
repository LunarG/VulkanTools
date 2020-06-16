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
 * This class takes a pointer to a treewidget and a profile
 * and creates a gui for displaying and editing those settings.
 *
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

#include "vulkanconfiguration.h"
#include "settingstreemanager.h"


///////////////////////////////////////////////////////
// These correlate with CVulkanConfiguration::szCannedProfiles -1
// Zero must be user defined if we want to be able to add more later.
// -1 could be used, but then we need special handling for the combobox,
// so no matter what, it has to be handled somewhere.
#define KHRONOS_PRESET_USER_DEFINED     0
#define KHRONOS_PRESET_STANDARD         1
#define KHRONOS_PRESET_BEST_PRACTICES   2
#define KHRONOS_PRESET_GPU_ASSIST       3
#define KHRONOS_PRESET_SHADER_PRINTF    4
#define KHRONOS_PRESET_LOW_OVERHEAD     5

CSettingsTreeManager::CSettingsTreeManager()
    {
    pEditorTree = nullptr;
    pProfile = nullptr;
    pKhronosPresets = nullptr;
    pKhronosLayer = nullptr;
    pKhronosTree = nullptr;
    pAdvancedKhronosEditor = nullptr;
    pKhronosFileItem = nullptr;
    pKhronosLogFileWidget = nullptr;
    pKhronosLogFileItem = nullptr;
    }

////////////////////////////////////////////////////////////////////////////////////
void CSettingsTreeManager::CreateGUI(QTreeWidget *pBuildTree, CProfileDef *pProfileDef)
    {
    // Do this first to make absolutely sure if thee is an old profile still active
    // it's state gets saved.
    CleanupGUI();

    pEditorTree = pBuildTree;
    pProfile = pProfileDef;

    pBuildTree->clear();

    // There will be one top level item for each layer
    for(int iLayer = 0; iLayer < pProfile->layers.size(); iLayer++) {
        QTreeWidgetItem *pLayerItem = new QTreeWidgetItem();
        pLayerItem->setText(0, pProfileDef->layers[iLayer]->name);
        pEditorTree->addTopLevelItem(pLayerItem);
        layerItems.push_back(pLayerItem);

        // Handle the case were we get off easy. No settings.
        if(pProfile->layers[iLayer]->layerSettings.size() == 0) {
            QTreeWidgetItem *pChild = new QTreeWidgetItem();
            pChild->setText(0, "No User Settings");
            pLayerItem->addChild(pChild);
            continue;
            }

        // There are settings.
        // Okay kid, this is where it gets complicated...
        // Is this Khronos? Khronos is special...
        if(pProfileDef->layers[iLayer]->name == QString("VK_LAYER_KHRONOS_validation")) {
            pKhronosLayer = pProfileDef->layers[iLayer];
            pKhronosTree = pLayerItem;
            BuildKhronosTree();
            continue;
            }

        // Generic is the only one left
        BuildGenericTree(pLayerItem, pProfileDef->layers[iLayer]);
        }


    ///////////////////////////////////////////////////////////////////
    // The last item is just the blacklisted layers
    if(!pProfileDef->blacklistedLayers.isEmpty()) {
        QTreeWidgetItem* pBlackList = new QTreeWidgetItem();
        pBlackList->setText(0, "Disabled Layers");
        pBuildTree->addTopLevelItem(pBlackList);
        for(int i = 0; i < pProfileDef->blacklistedLayers.size(); i++) {
            QTreeWidgetItem *pChild = new QTreeWidgetItem();
            pChild->setText(0, pProfileDef->blacklistedLayers[i]);
            pBlackList->addChild(pChild);
            }
        }

    // Walk the tree, and restore the expanded state of all the items
    SetTreeState(pProfile->settingTreeState, 0, pEditorTree->invisibleRootItem());
    pBuildTree->resizeColumnToContents(0);
    }

//////////////////////////////////////////////////////////////////////////
void CSettingsTreeManager::BuildKhronosTree(void)
    {
    pKhronosPresetItem = new QTreeWidgetItem();
    pKhronosPresetItem->setText(0, "Validation Preset");
    pKhronosPresets = new QComboBox();
    pKhronosPresets->addItem("User Defined");
    pKhronosPresets->addItem("Standard");
    pKhronosPresets->addItem("Best Practices");
    pKhronosPresets->addItem("GPU Assisted");
    pKhronosPresets->addItem("Shader Printf");
    pKhronosPresets->addItem("Reduced-Overhead");

    pKhronosPresets->setCurrentIndex(pProfile->nPresetIndex);

    connect(pKhronosPresets, SIGNAL(currentIndexChanged(int)), this, SLOT(khronosPresetChanged(int)));
    pKhronosTree->addChild(pKhronosPresetItem);
    pEditorTree->setItemWidget(pKhronosPresetItem, 1, pKhronosPresets);

    // This just finds the enables and disables
    pAdvancedKhronosEditor = new KhronosSettingsAdvanced(pEditorTree, pKhronosPresetItem, pKhronosLayer->layerSettings);
    connect(pAdvancedKhronosEditor, SIGNAL(settingChanged()), this, SLOT(khronosPresetEdited()));

    // Look for the Debug Action and log file settings
    TLayerSettings *pDebugAction = nullptr;
    TLayerSettings *pLogFile = nullptr;
    for(int i = 0; i < pKhronosLayer->layerSettings.size(); i++) {
        if(pKhronosLayer->layerSettings[i]->settingsName == QString("debug_action"))
            pDebugAction = pKhronosLayer->layerSettings[i];

        if(pKhronosLayer->layerSettings[i]->settingsName == QString("log_filename"))
            pLogFile = pKhronosLayer->layerSettings[i];
        }

    Q_ASSERT(pLogFile != nullptr);
    Q_ASSERT(pDebugAction != nullptr);

    // Set them up
    QTreeWidgetItem *pDebugActionItem = new QTreeWidgetItem();
    pKhronosDebugAction = new CEnumSettingWidget(pDebugActionItem, pDebugAction);
    pKhronosTree->addChild(pDebugActionItem);
    pEditorTree->setItemWidget(pDebugActionItem, 1, pKhronosDebugAction);

    pKhronosLogFileItem = new QTreeWidgetItem();
    pKhronosLogFileWidget = new CFilenameSettingWidget(pKhronosLogFileItem, pLogFile);
    pDebugActionItem->addChild(pKhronosLogFileItem);
    pEditorTree->setItemWidget(pKhronosLogFileItem, 1, pKhronosLogFileWidget);
    pKhronosFileItem = pKhronosLogFileItem;
    connect(pKhronosDebugAction, SIGNAL(currentIndexChanged(int)), this, SLOT(khronosDebugChanged(int)));
    if(pKhronosDebugAction->currentText() != QString("Log Message")) {
        pKhronosLogFileItem->setDisabled(true);
        pKhronosLogFileWidget->setDisabled(true);
        }

    // This is looking for the report flags
    for(int iSetting = 0; iSetting < pKhronosLayer->layerSettings.size(); iSetting++) {
        // Multi-enum - report flags only
        if(pKhronosLayer->layerSettings[iSetting]->settingsType == LAYER_SETTINGS_INCLUSIVE_LIST &&
            pKhronosLayer->layerSettings[iSetting]->settingsName == QString("report_flags")) {
            QTreeWidgetItem *pSubCategory = new QTreeWidgetItem;
            pSubCategory->setText(0, pKhronosLayer->layerSettings[iSetting]->settingsPrompt);
            pSubCategory->setToolTip(0, pKhronosLayer->layerSettings[iSetting]->settingsDesc);
            pKhronosTree->addChild(pSubCategory);

            for(int i = 0; i < pKhronosLayer->layerSettings[iSetting]->settingsListInclusiveValue.size(); i++) {
                QTreeWidgetItem *pChild = new QTreeWidgetItem();
                CMultiEnumSetting *pControl = new CMultiEnumSetting(pKhronosLayer->layerSettings[iSetting], pKhronosLayer->layerSettings[iSetting]->settingsListInclusiveValue[i]);
                pControl->setText(pKhronosLayer->layerSettings[iSetting]->settingsListInclusivePrompt[i]);
                pSubCategory->addChild(pChild);
                pEditorTree->setItemWidget(pChild, 0, pControl);
                }

            continue;
            }
        }

    pKhronosTree->addChild(pKhronosPresetItem);
    }

void CSettingsTreeManager::khronosDebugChanged(int nIndex)
    {
    (void)nIndex;
    bool bEnable = (pKhronosDebugAction->currentText() != QString("Log Message"));
    pKhronosLogFileItem->setDisabled(bEnable);
    pKhronosLogFileWidget->setDisabled(bEnable);
    }

void CSettingsTreeManager::BuildGenericTree(QTreeWidgetItem* pParent, CLayerFile *pLayer)
    {
    for(int iSetting = 0; iSetting < pLayer->layerSettings.size(); iSetting++) {
        QTreeWidgetItem *pSettingItem = new QTreeWidgetItem();

        // True false?
        if(pLayer->layerSettings[iSetting]->settingsType == LAYER_SETTINGS_BOOL) {
            CBoolSettingWidget *pBoolWidget = new CBoolSettingWidget(pLayer->layerSettings[iSetting]);
            pParent->addChild(pSettingItem);
            pEditorTree->setItemWidget(pSettingItem, 0, pBoolWidget);
            continue;
            }

        // True false? (with numeric output instead of text)
        if(pLayer->layerSettings[iSetting]->settingsType == LAYER_SETTINGS_BOOL_NUMERIC) {
            CBoolSettingWidget *pBoolWidget = new CBoolSettingWidget(pLayer->layerSettings[iSetting], true);
            pParent->addChild(pSettingItem);
            pEditorTree->setItemWidget(pSettingItem, 0, pBoolWidget);
            continue;
            }

        // Combobox - enum - just one thing
        if(pLayer->layerSettings[iSetting]->settingsType == LAYER_SETTINGS_EXCLUSIVE_LIST) {
            CEnumSettingWidget *pEnumWidget = new CEnumSettingWidget(pSettingItem, pLayer->layerSettings[iSetting]);
            pParent->addChild(pSettingItem);
            pEditorTree->setItemWidget(pSettingItem, 1, pEnumWidget);
            continue;
            }

        // Raw text field?
        if(pLayer->layerSettings[iSetting]->settingsType == LAYER_SETTINGS_STRING) {
            CStringSettingWidget *pStringWidget = new CStringSettingWidget(pSettingItem, pLayer->layerSettings[iSetting]);
            pParent->addChild(pSettingItem);
            pEditorTree->setItemWidget(pSettingItem, 1, pStringWidget);
            continue;
            }

        // Select a file?
        if(pLayer->layerSettings[iSetting]->settingsType == LAYER_SETTINGS_FILE) {
            CFilenameSettingWidget* pWidget = new CFilenameSettingWidget(pSettingItem, pLayer->layerSettings[iSetting]);
            pParent->addChild(pSettingItem);
            pEditorTree->setItemWidget(pSettingItem, 1, pWidget);
            fileWidgets.push_back(pSettingItem);
            continue;
            }


        // Save to folder?
        if(pLayer->layerSettings[iSetting]->settingsType == LAYER_SETTINGS_SAVE_FOLDER) {
            CFolderSettingWidget* pWidget = new CFolderSettingWidget(pSettingItem, pLayer->layerSettings[iSetting]);
            pParent->addChild(pSettingItem);
            pEditorTree->setItemWidget(pSettingItem, 1, pWidget);
            fileWidgets.push_back(pSettingItem);
            continue;
            }

        // Undefined... at least gracefuly display what the setting is
        pSettingItem->setText(0, pLayer->layerSettings[iSetting]->settingsPrompt);
        pSettingItem->setToolTip(0, pLayer->layerSettings[iSetting]->settingsDesc);
        pParent->addChild(pSettingItem);
        }
    }


////////////////////////////////////////////////////////////////////////////////////
/// The user has selected a preset for this layer
void CSettingsTreeManager::khronosPresetChanged(int nIndex)
    {
    // We really just don't care (and the value is zero)
    if(nIndex == KHRONOS_PRESET_USER_DEFINED)
        return;

    // The easiest way to do this is to create a new profile, and copy the layer over
    QString preDefined = ":/resourcefiles/";
    preDefined += CVulkanConfiguration::szCannedProfiles[nIndex-1];
    preDefined += ".json";
    CProfileDef *pPatternProfile = CVulkanConfiguration::getVulkanConfig()->LoadProfile(preDefined);
    if(pPatternProfile == nullptr)
        return;

    // Copy it all into the real layer and delete it
    // Find the KhronosLaer
    int nKhronosLayer = -1;
    for(int i = 0; i < pProfile->layers.size(); i++)
        if(pProfile->layers[i] == pKhronosLayer) {
            nKhronosLayer = i;
            break;
            }

    Q_ASSERT(nKhronosLayer != -1);

    delete pProfile->layers[nKhronosLayer];             // This is the Khronos layer we are deleting
    pProfile->layers[nKhronosLayer] = new CLayerFile;   // Allocate a new one
    pKhronosLayer = pProfile->layers[nKhronosLayer];    // Update Khronos pointer
    pPatternProfile->layers[0]->CopyLayer(pKhronosLayer);   // Copy pattern into the new layer
    delete pPatternProfile;                                 // Delete the pattern
    pProfile->nPresetIndex = nIndex;

    // Now we need to reload the Khronos tree item.
    pEditorTree->blockSignals(true);
    pEditorTree->setItemWidget(pKhronosFileItem, 1, nullptr);
    delete pAdvancedKhronosEditor;

    QByteArray savedState;
    GetTreeState(savedState, pKhronosPresetItem);

    int nChildren = pKhronosTree->childCount();
    for(int i = 0; i < nChildren; i++)
        pKhronosTree->takeChild(0);

    BuildKhronosTree();
    SetTreeState(savedState, 0, pKhronosPresetItem);
    pEditorTree->blockSignals(false);
    }


void CSettingsTreeManager::khronosPresetEdited(void)
    {
    pKhronosPresets->blockSignals(true);
    pKhronosPresets->setCurrentIndex(KHRONOS_PRESET_USER_DEFINED);
    pProfile->nPresetIndex = KHRONOS_PRESET_USER_DEFINED;
    pKhronosPresets->blockSignals(false);
    }


/////////////////////////////////////////////////////////////////////////////////////////
void CSettingsTreeManager::GetTreeState(QByteArray &byteArray, QTreeWidgetItem *pTopItem)
    {
    if(pTopItem->isExpanded())
        byteArray.push_back('1');
    else
        byteArray.push_back('0');

    for(int i = 0; i < pTopItem->childCount(); i++) {
        QTreeWidgetItem *pChild = pTopItem->child(i);
        GetTreeState(byteArray, pChild);
        }
    }

/////////////////////////////////////////////////////////////////////////////////////////
int CSettingsTreeManager::SetTreeState(QByteArray &byteArray, int nIndex, QTreeWidgetItem *pTopItem)
    {
    // We very well could run out, on initial run, expand everything
    if(nIndex > byteArray.length())
        pTopItem->setExpanded(true);
    else {
        if(byteArray[nIndex++] == '1')
            pTopItem->setExpanded(true);
        }

    // Walk the children
    if(pTopItem->childCount() != 0)
        for(int i = 0; i < pTopItem->childCount(); i++)
            nIndex = SetTreeState(byteArray, nIndex, pTopItem->child(i));

    return nIndex;
    }

////////////////////////////////////////////////////////////////////////////////////
void CSettingsTreeManager::CleanupGUI(void)
    {
    if(pEditorTree == nullptr || pProfile == nullptr)
        return;

    // Get the state of the last tree, and save it!
    pProfile->settingTreeState.clear();
    GetTreeState(pProfile->settingTreeState, pEditorTree->invisibleRootItem());
    CVulkanConfiguration::getVulkanConfig()->SaveProfile(pProfile);

    if(pKhronosFileItem)
        pEditorTree->setItemWidget(pKhronosFileItem, 1, nullptr);

    pKhronosFileItem = nullptr;

    if(pAdvancedKhronosEditor)
        delete pAdvancedKhronosEditor;

    for(int i = 0; i < fileWidgets.size(); i++)
        pEditorTree->setItemWidget(fileWidgets[i], 1, nullptr);
    fileWidgets.clear();

    pEditorTree->clear();
    pEditorTree = nullptr;
    pProfile = nullptr;
    pKhronosPresets = nullptr;
    pKhronosLayer = nullptr;
    pKhronosTree = nullptr;
    pKhronosDebugAction = nullptr;
    pKhronosPresetItem = nullptr;
    pAdvancedKhronosEditor = nullptr;
    pKhronosLogFileWidget = nullptr;
    pKhronosLogFileItem = nullptr;
    }
