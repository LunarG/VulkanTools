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

#include "configurator.h"
#include "settingstreemanager.h"

///////////////////////////////////////////////////////
// These correlate with CVulkanConfiguration::szCannedProfiles -1
// Zero must be user defined if we want to be able to add more later.
// -1 could be used, but then we need special handling for the combobox,
// so no matter what, it has to be handled somewhere.
#define KHRONOS_PRESET_USER_DEFINED 0
#define KHRONOS_PRESET_STANDARD 1
#define KHRONOS_PRESET_BEST_PRACTICES 2
#define KHRONOS_PRESET_GPU_ASSIST 3
#define KHRONOS_PRESET_SHADER_PRINTF 4
#define KHRONOS_PRESET_LOW_OVERHEAD 5

SettingsTreeManager::SettingsTreeManager() {
    configuration_settings_tree_ = nullptr;
    configuration_ = nullptr;
    pKhronosPresets = nullptr;
    pKhronosLayer = nullptr;
    pKhronosTree = nullptr;
    pAdvancedKhronosEditor = nullptr;
    pKhronosFileItem = nullptr;
    pKhronosLogFileWidget = nullptr;
    pKhronosLogFileItem = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::CreateGUI(QTreeWidget *pBuildTree, Configuration *pProfileDef) {
    // Do this first to make absolutely sure if thee is an old profile still active
    // it's state gets saved.
    CleanupGUI();

    configuration_settings_tree_ = pBuildTree;
    configuration_ = pProfileDef;

    pBuildTree->clear();

    // There will be one top level item for each layer
    for (int iLayer = 0; iLayer < configuration_->layers.size(); iLayer++) {
        QTreeWidgetItem *pLayerItem = new QTreeWidgetItem();
        pLayerItem->setText(0, pProfileDef->layers[iLayer]->name);
        configuration_settings_tree_->addTopLevelItem(pLayerItem);
        layer_items_.push_back(pLayerItem);

        // Handle the case were we get off easy. No settings.
        if (configuration_->layers[iLayer]->layer_settings.size() == 0) {
            QTreeWidgetItem *pChild = new QTreeWidgetItem();
            pChild->setText(0, "No User Settings");
            pLayerItem->addChild(pChild);
            continue;
        }

        // There are settings.
        // Okay kid, this is where it gets complicated...
        // Is this Khronos? Khronos is special...
        if (pProfileDef->layers[iLayer]->name == QString("VK_LAYER_KHRONOS_validation")) {
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
    if (!pProfileDef->excluded_layers.isEmpty()) {
        QTreeWidgetItem *pBlackList = new QTreeWidgetItem();
        pBlackList->setText(0, "Excluded Layers");
        pBuildTree->addTopLevelItem(pBlackList);
        for (int i = 0; i < pProfileDef->excluded_layers.size(); i++) {
            QTreeWidgetItem *pChild = new QTreeWidgetItem();
            pChild->setText(0, pProfileDef->excluded_layers[i]);
            pBlackList->addChild(pChild);
        }
    }

    // Walk the tree, and restore the expanded state of all the items
    SetTreeState(configuration_->setting_tree_state, 0, configuration_settings_tree_->invisibleRootItem());

    // Everyone is expanded.
    pBuildTree->resizeColumnToContents(0);
}

//////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::BuildKhronosTree(void) {
    pKhronosPresetItem = new QTreeWidgetItem();
    pKhronosPresetItem->setText(0, "Validation Preset");
    QTreeWidgetItem *pNextLine = new QTreeWidgetItem();

    pKhronosPresets = new QComboBox();
    pKhronosPresets->blockSignals(true);
    pKhronosPresets->addItem("User Defined");
    pKhronosPresets->addItem("Standard");
    pKhronosPresets->addItem("Best Practices");
    pKhronosPresets->addItem("GPU-Assisted");
    pKhronosPresets->addItem("Shader Printf");
    pKhronosPresets->addItem("Reduced-Overhead");

    pKhronosPresets->setCurrentIndex(configuration_->preset_index);

    connect(pKhronosPresets, SIGNAL(currentIndexChanged(int)), this, SLOT(khronosPresetChanged(int)));
    pKhronosTree->addChild(pKhronosPresetItem);
    pKhronosPresetItem->addChild(pNextLine);
    configuration_settings_tree_->setItemWidget(pNextLine, 0, pKhronosPresets);

    QTreeWidgetItem *pKhronosSettingsItem = new QTreeWidgetItem();
    pKhronosSettingsItem->setText(0, "Individual Settings");
    pKhronosPresetItem->addChild(pKhronosSettingsItem);

    // This just finds the enables and disables
    pAdvancedKhronosEditor =
        new KhronosSettingsAdvanced(configuration_settings_tree_, pKhronosSettingsItem, pKhronosLayer->layer_settings);

    // Look for the Debug Action and log file settings
    LayerSettings *pDebugAction = nullptr;
    LayerSettings *pLogFile = nullptr;
    for (int i = 0; i < pKhronosLayer->layer_settings.size(); i++) {
        if (pKhronosLayer->layer_settings[i]->settings_name == QString("debug_action"))
            pDebugAction = pKhronosLayer->layer_settings[i];

        if (pKhronosLayer->layer_settings[i]->settings_name == QString("log_filename")) pLogFile = pKhronosLayer->layer_settings[i];
    }

    Q_ASSERT(pLogFile != nullptr);
    Q_ASSERT(pDebugAction != nullptr);

    // Set them up
    QTreeWidgetItem *pDebugActionItem = new QTreeWidgetItem();
    pKhronosDebugAction = new EnumSettingWidget(pDebugActionItem, pDebugAction);
    pKhronosTree->addChild(pDebugActionItem);
    pNextLine = new QTreeWidgetItem();
    pDebugActionItem->addChild(pNextLine);
    configuration_settings_tree_->setItemWidget(pNextLine, 0, pKhronosDebugAction);

    pKhronosLogFileItem = new QTreeWidgetItem();
    pNextLine = new QTreeWidgetItem();
    pKhronosLogFileWidget = new FilenameSettingWidget(pKhronosLogFileItem, pLogFile);
    pDebugActionItem->addChild(pKhronosLogFileItem);
    pKhronosLogFileItem->addChild(pNextLine);
    configuration_settings_tree_->setItemWidget(pNextLine, 0, pKhronosLogFileWidget);
    pNextLine->setSizeHint(0, QSize(0, 28));
    compound_widgets_.push_back(pNextLine);
    pKhronosFileItem = pKhronosLogFileItem;
    connect(pKhronosDebugAction, SIGNAL(currentIndexChanged(int)), this, SLOT(khronosDebugChanged(int)));
    if (pKhronosDebugAction->currentText() != QString("Log Message")) {
        pKhronosLogFileItem->setDisabled(true);
        pKhronosLogFileWidget->setDisabled(true);
    }

    // This is looking for the report flags
    for (int setting_index = 0; setting_index < pKhronosLayer->layer_settings.size(); setting_index++) {
        // Multi-enum - report flags only
        if (pKhronosLayer->layer_settings[setting_index]->settings_type == LAYER_SETTINGS_INCLUSIVE_LIST &&
            pKhronosLayer->layer_settings[setting_index]->settings_name == QString("report_flags")) {
            QTreeWidgetItem *pSubCategory = new QTreeWidgetItem;
            pSubCategory->setText(0, pKhronosLayer->layer_settings[setting_index]->settings_prompt);
            pSubCategory->setToolTip(0, pKhronosLayer->layer_settings[setting_index]->settings_desc);
            pKhronosTree->addChild(pSubCategory);

            for (int i = 0; i < pKhronosLayer->layer_settings[setting_index]->settings_list_inclusive_value.size(); i++) {
                QTreeWidgetItem *pChild = new QTreeWidgetItem();
                MultiEnumSetting *pControl =
                    new MultiEnumSetting(pKhronosLayer->layer_settings[setting_index],
                                         pKhronosLayer->layer_settings[setting_index]->settings_list_inclusive_value[i]);
                pControl->setText(pKhronosLayer->layer_settings[setting_index]->settings_list_inclusive_prompt[i]);
                pSubCategory->addChild(pChild);
                configuration_settings_tree_->setItemWidget(pChild, 0, pControl);
                pControl->setFont(configuration_settings_tree_->font());
                connect(pControl, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            }

            continue;
        }
    }

    //////////////////////////////// VUID message filtering
    for (int iSetting = 0; iSetting < pKhronosLayer->layer_settings.size(); iSetting++) {
        if (pKhronosLayer->layer_settings[iSetting]->settings_type == LAYER_SETTINGS_VUID_FILTER) {
            QTreeWidgetItem *pMuteMessageItem = new QTreeWidgetItem;

            pMuteMessageItem->setText(0, "Mute Message VUIDs");
            pKhronosTree->addChild(pMuteMessageItem);

            pMuteMessageSearchItem = new QTreeWidgetItem();
            pMuteMessageSearchItem->setText(0, "Search for:");
            pMuteMessageItem->addChild(pMuteMessageSearchItem);
            pVUIDSearchWidget = new VUIDSearchWidget();
            pNextLine = new QTreeWidgetItem();
            pNextLine->setSizeHint(0, QSize(0, 28));
            pMuteMessageItem->addChild(pNextLine);
            configuration_settings_tree_->setItemWidget(pNextLine, 0, pVUIDSearchWidget);
            compound_widgets_.push_back(pNextLine);

            QTreeWidgetItem *pListItem = new QTreeWidgetItem();
            pMuteMessageItem->addChild(pListItem);
            pListItem->setSizeHint(0, QSize(350, 200));
            pMuteMessageWidget = new MuteMessageWidget(pKhronosLayer->layer_settings[iSetting]);
            compound_widgets_.push_back(pListItem);
            configuration_settings_tree_->setItemWidget(pListItem, 0, pMuteMessageWidget);

            connect(pVUIDSearchWidget, SIGNAL(itemSelected(QString &)), pMuteMessageWidget, SLOT(addItem(QString &)));
            connect(pMuteMessageWidget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }
    }

    // This really does go way down here.
    connect(pAdvancedKhronosEditor, SIGNAL(settingChanged()), this, SLOT(khronosPresetEdited()));
    pKhronosPresets->blockSignals(false);

    //////// Add the preset item
    pKhronosTree->addChild(pKhronosPresetItem);
}

/////////////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::khronosDebugChanged(int nIndex) {
    (void)nIndex;
    bool bEnable = (pKhronosDebugAction->currentText() != QString("Log Message"));
    configuration_settings_tree_->blockSignals(true);
    pKhronosLogFileItem->setDisabled(bEnable);
    pKhronosLogFileWidget->setDisabled(bEnable);
    configuration_settings_tree_->blockSignals(false);
    profileEdited();
}

void SettingsTreeManager::BuildGenericTree(QTreeWidgetItem *pParent, LayerFile *pLayer) {
    for (int iSetting = 0; iSetting < pLayer->layer_settings.size(); iSetting++) {
        QTreeWidgetItem *pSettingItem = new QTreeWidgetItem();

        // True false?
        if (pLayer->layer_settings[iSetting]->settings_type == LAYER_SETTINGS_BOOL) {
            BoolSettingWidget *pBoolWidget = new BoolSettingWidget(pLayer->layer_settings[iSetting]);
            pParent->addChild(pSettingItem);
            configuration_settings_tree_->setItemWidget(pSettingItem, 0, pBoolWidget);
            pBoolWidget->setFont(configuration_settings_tree_->font());
            connect(pBoolWidget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // True false? (with numeric output instead of text)
        if (pLayer->layer_settings[iSetting]->settings_type == LAYER_SETTINGS_BOOL_NUMERIC) {
            BoolSettingWidget *pBoolWidget = new BoolSettingWidget(pLayer->layer_settings[iSetting], true);
            pParent->addChild(pSettingItem);
            configuration_settings_tree_->setItemWidget(pSettingItem, 0, pBoolWidget);
            pBoolWidget->setFont(configuration_settings_tree_->font());
            connect(pBoolWidget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Combobox - enum - just one thing
        if (pLayer->layer_settings[iSetting]->settings_type == LAYER_SETTINGS_EXCLUSIVE_LIST) {
            pParent->addChild(pSettingItem);
            pSettingItem->setText(0, pLayer->layer_settings[iSetting]->settings_prompt);
            QTreeWidgetItem *pPlaceHolder = new QTreeWidgetItem();
            pSettingItem->addChild(pPlaceHolder);

            EnumSettingWidget *pEnumWidget = new EnumSettingWidget(pSettingItem, pLayer->layer_settings[iSetting]);
            configuration_settings_tree_->setItemWidget(pPlaceHolder, 0, pEnumWidget);
            connect(pEnumWidget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Raw text field?
        if (pLayer->layer_settings[iSetting]->settings_type == LAYER_SETTINGS_STRING) {
            StringSettingWidget *pStringWidget = new StringSettingWidget(pSettingItem, pLayer->layer_settings[iSetting]);
            pParent->addChild(pSettingItem);
            QTreeWidgetItem *pPlaceHolder = new QTreeWidgetItem();
            pSettingItem->addChild(pPlaceHolder);
            configuration_settings_tree_->setItemWidget(pPlaceHolder, 0, pStringWidget);
            connect(pStringWidget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Select a file?
        if (pLayer->layer_settings[iSetting]->settings_type == LAYER_SETTINGS_FILE) {
            FilenameSettingWidget *pWidget = new FilenameSettingWidget(pSettingItem, pLayer->layer_settings[iSetting]);
            pParent->addChild(pSettingItem);
            QTreeWidgetItem *pPlaceHolder = new QTreeWidgetItem();
            pPlaceHolder->setSizeHint(0, QSize(0, 28));
            pSettingItem->addChild(pPlaceHolder);
            configuration_settings_tree_->setItemWidget(pPlaceHolder, 0, pWidget);
            compound_widgets_.push_back(pPlaceHolder);
            connect(pWidget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Save to folder?
        if (pLayer->layer_settings[iSetting]->settings_type == LAYER_SETTINGS_SAVE_FOLDER) {
            FolderSettingWidget *pWidget = new FolderSettingWidget(pSettingItem, pLayer->layer_settings[iSetting]);
            pParent->addChild(pSettingItem);
            QTreeWidgetItem *pPlaceHolder = new QTreeWidgetItem();
            pPlaceHolder->setSizeHint(0, QSize(0, 28));
            pSettingItem->addChild(pPlaceHolder);
            configuration_settings_tree_->setItemWidget(pPlaceHolder, 0, pWidget);
            compound_widgets_.push_back(pPlaceHolder);
            connect(pWidget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        ///////////////////////////////////////////////////////////////////////////
        // Undefined... at least gracefuly display what the setting is
        pSettingItem->setText(0, pLayer->layer_settings[iSetting]->settings_prompt);
        pSettingItem->setToolTip(0, pLayer->layer_settings[iSetting]->settings_desc);
        pParent->addChild(pSettingItem);
    }
}

////////////////////////////////////////////////////////////////////////////////////
/// The user has selected a preset for this layer
void SettingsTreeManager::khronosPresetChanged(int nIndex) {
    Configurator &configuration = Configurator::Get();

    configuration.CheckApplicationRestart();

    // We really just don't care (and the value is zero)
    if (nIndex == KHRONOS_PRESET_USER_DEFINED) return;

    // The easiest way to do this is to create a new profile, and copy the layer over
    QString preDefined = ":/resourcefiles/";
    preDefined += Configurator::default_configurations[nIndex - 1];
    preDefined += ".json";
    Configuration *pPatternProfile = configuration.LoadConfiguration(preDefined);
    if (pPatternProfile == nullptr) return;

    // Copy it all into the real layer and delete it
    // Find the KhronosLaer
    int nKhronosLayer = -1;
    for (int i = 0; i < configuration_->layers.size(); i++)
        if (configuration_->layers[i] == pKhronosLayer) {
            nKhronosLayer = i;
            break;
        }

    Q_ASSERT(nKhronosLayer != -1);

    // Reset just specific layer settings
    for (int i = 0; i < configuration_->layers[nKhronosLayer]->layer_settings.size(); i++) {
        if (pKhronosLayer->layer_settings[i]->settings_name == QString("disables") ||
            pKhronosLayer->layer_settings[i]->settings_name == QString("enables"))
            pKhronosLayer->layer_settings[i]->settings_value = pPatternProfile->layers[0]->layer_settings[i]->settings_value;
    }

    delete pPatternProfile;  // Delete the pattern
    configuration_->preset_index = nIndex;

    // Now we need to reload the Khronos tree item.
    configuration_settings_tree_->blockSignals(true);
    configuration_settings_tree_->setItemWidget(pKhronosFileItem, 1, nullptr);
    delete pAdvancedKhronosEditor;

    QByteArray savedState;
    QTreeWidgetItem *pKhronosParent = pKhronosPresetItem->parent();
    GetTreeState(savedState, pKhronosParent);

    int nChildren = pKhronosTree->childCount();
    for (int i = 0; i < nChildren; i++) pKhronosTree->takeChild(0);

    BuildKhronosTree();
    SetTreeState(savedState, 0, pKhronosParent);
    configuration_settings_tree_->blockSignals(false);
    profileEdited();
}

///////////////////////////////////////////////////////////////////////
// Any edit to these settings means we are not user defined
// (and that we need to save the settings)
void SettingsTreeManager::khronosPresetEdited(void) {
    pKhronosPresets->blockSignals(true);
    pKhronosPresets->setCurrentIndex(KHRONOS_PRESET_USER_DEFINED);
    configuration_->preset_index = KHRONOS_PRESET_USER_DEFINED;
    pKhronosPresets->blockSignals(false);
    profileEdited();
}

/////////////////////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::GetTreeState(QByteArray &byteArray, QTreeWidgetItem *pTopItem) {
    if (pTopItem->isExpanded())
        byteArray.push_back('1');
    else
        byteArray.push_back('0');

    for (int i = 0; i < pTopItem->childCount(); i++) {
        QTreeWidgetItem *pChild = pTopItem->child(i);
        GetTreeState(byteArray, pChild);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
int SettingsTreeManager::SetTreeState(QByteArray &byteArray, int nIndex, QTreeWidgetItem *pTopItem) {
    // We very well could run out, on initial run, expand everything
    if (nIndex > byteArray.length())
        pTopItem->setExpanded(true);
    else {
        if (byteArray[nIndex++] == '1')
            pTopItem->setExpanded(true);
        else
            pTopItem->setExpanded(false);
    }

    // Walk the children
    if (pTopItem->childCount() != 0)
        for (int i = 0; i < pTopItem->childCount(); i++) nIndex = SetTreeState(byteArray, nIndex, pTopItem->child(i));

    return nIndex;
}

////////////////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::CleanupGUI(void) {
    if (configuration_settings_tree_ == nullptr || configuration_ == nullptr) return;

    // Get the state of the last tree, and save it!
    configuration_->setting_tree_state.clear();
    GetTreeState(configuration_->setting_tree_state, configuration_settings_tree_->invisibleRootItem());
    Configurator::Get().SaveConfiguration(configuration_);

    // If a Khronos layer is present, it needs cleanup up from custom controls before
    // it's cleared or deleted.
    if (pKhronosLayer) {
        configuration_settings_tree_->setItemWidget(pKhronosFileItem, 1, nullptr);
        configuration_settings_tree_->setItemWidget(pMuteMessageSearchItem, 1, nullptr);
    }

    pKhronosFileItem = nullptr;

    if (pAdvancedKhronosEditor) delete pAdvancedKhronosEditor;

    for (int i = 0; i < compound_widgets_.size(); i++)
        configuration_settings_tree_->setItemWidget(compound_widgets_[i], 1, nullptr);

    compound_widgets_.clear();

    configuration_settings_tree_->clear();
    configuration_settings_tree_ = nullptr;
    configuration_ = nullptr;
    pKhronosPresets = nullptr;
    pKhronosLayer = nullptr;
    pKhronosTree = nullptr;
    pKhronosDebugAction = nullptr;
    pKhronosPresetItem = nullptr;
    pAdvancedKhronosEditor = nullptr;
    pKhronosLogFileWidget = nullptr;
    pKhronosLogFileItem = nullptr;
}

/////////////////////////////////////////////////////////////
// The profile has been edited and should be saved
void SettingsTreeManager::profileEdited(void) {
    // Resave this profile
    Configurator &configuration = Configurator::Get();
    configuration.SaveConfiguration(configuration_);
    configuration.CheckApplicationRestart();

    // If this profile is active, we need to reset the override files too
    // Just resetting with the same parent pointer will do the trick
    if (configuration_ == configuration.GetActiveConfiguration()) configuration.SetActiveConfiguration(configuration_);
}
