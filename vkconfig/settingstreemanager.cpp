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

#include "settingstreemanager.h"
#include "boolsettingwidget.h"
#include "enumsettingwidget.h"
#include "stringsettingwidget.h"
#include "filenamesettingwidget.h"
#include "foldersettingwidget.h"

CSettingsTreeManager::CSettingsTreeManager()
    {
    pEditorTree = nullptr;
    pProfile = nullptr;
    pKhronosPresets = nullptr;
    }

////////////////////////////////////////////////////////////////////////////////////
void CSettingsTreeManager::CreateGUI(QTreeWidget *pBuildTree, CProfileDef *pProfileDef)
    {
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
            BuildKhronosTree(pLayerItem, pProfileDef->layers[iLayer]);
            continue;
            }

        // Generic is the only one left
        BuildGenericTree(pLayerItem, pProfileDef->layers[iLayer]);
        }


    ///////////////////////////////////////////////////////////////////
    // The last item is just the blacklisted layers
    if(pProfileDef->blacklistedLayers.isEmpty())
        return;

    QTreeWidgetItem* pBlackList = new QTreeWidgetItem();
    pBlackList->setText(0, "Disabled Layers");
    pBuildTree->addTopLevelItem(pBlackList);
    for(int i = 0; i < pProfileDef->blacklistedLayers.size(); i++) {
        QTreeWidgetItem *pChild = new QTreeWidgetItem();
        pChild->setText(0, pProfileDef->blacklistedLayers[i]);
        pBlackList->addChild(pChild);
        }

//    pBuildTree->setColumnWidth(0, 400);
    pBuildTree->resizeColumnToContents(0);
//    pBuildTree->resizeColumnToContents(1);
    }


void CSettingsTreeManager::BuildKhronosTree(QTreeWidgetItem* pParent, CLayerFile *pKhronosLayer)
    {
    QTreeWidgetItem* pItem = new QTreeWidgetItem();
    pItem->setText(0, "Validation Preset");
    pKhronosPresets = new QComboBox();
    pKhronosPresets->addItem("Standard");
    pKhronosPresets->addItem("Shader-Based");
    pKhronosPresets->addItem("Low-Overhead");
    pKhronosPresets->addItem("Best Practices");
    pKhronosPresets->addItem("User Defined");
    pParent->addChild(pItem);
    pEditorTree->setItemWidget(pItem, 1, pKhronosPresets);

    for(int iSetting = 0; iSetting < pKhronosLayer->layerSettings.size(); iSetting++) {
        QTreeWidgetItem *pChild = new QTreeWidgetItem();

        // Combobox - enum - just one thing
        if(pKhronosLayer->layerSettings[iSetting]->settingsType == LAYER_SETTINGS_EXCLUSIVE_LIST) {
            CEnumSettingWidget *pEnumWidget = new CEnumSettingWidget(pChild, pKhronosLayer->layerSettings[iSetting]);
            pParent->addChild(pChild);
            pEditorTree->setItemWidget(pChild, 1, pEnumWidget);
            }

        // Select a file?
        if(pKhronosLayer->layerSettings[iSetting]->settingsType == LAYER_SETTINGS_FILE) {
            CFilenameSettingWidget* pWidget = new CFilenameSettingWidget(pChild, pKhronosLayer->layerSettings[iSetting]);
            pParent->addChild(pChild);
            pEditorTree->setItemWidget(pChild, 1, pWidget);
            fileWidgets.push_back(pChild);
            continue;
            }


        // TBD - just add description
        pChild->setText(0, pKhronosLayer->layerSettings[iSetting]->settingsPrompt);
        pChild->setToolTip(0, pKhronosLayer->layerSettings[iSetting]->settingsDesc);
        pParent->addChild(pChild);
        }

    pParent->addChild(pItem);
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
void CSettingsTreeManager::CleanupGUI(void)
    {
    if(pEditorTree == nullptr)
        return;

    for(int i = 0; i < fileWidgets.size(); i++)
        pEditorTree->setItemWidget(fileWidgets[i], 1, nullptr);
    fileWidgets.clear();

    pEditorTree->clear();
    pEditorTree = nullptr;
    pProfile = nullptr;
    pKhronosPresets = nullptr;
    }
