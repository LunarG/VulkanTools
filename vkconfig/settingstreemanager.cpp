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
 * Authors:
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "configurator.h"
#include "settingstreemanager.h"
#include "util.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QApplication>

SettingsTreeManager::SettingsTreeManager()
    : configuration_settings_tree_(nullptr),
      configuration_(nullptr),
      validation_presets_combo_box_(nullptr),
      validation_layer_file_(nullptr),
      validation_tree_item_(nullptr),
      validation_settings_(nullptr),
      validation_file_item_(nullptr),
      validation_log_file_widget_(nullptr),
      validation_log_file_item_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::CreateGUI(QTreeWidget *build_tree, Configuration *configuration) {
    // Do this first to make absolutely sure if thee is an old profile still active
    // it's state gets saved.
    CleanupGUI();

    configuration_settings_tree_ = build_tree;
    configuration_ = configuration;

    build_tree->clear();

    // There will be one top level item for each layer
    for (int layer_index = 0; layer_index < configuration_->layers.size(); layer_index++) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, configuration->layers[layer_index]->name);
        configuration_settings_tree_->addTopLevelItem(item);
        layer_items_.push_back(item);

        // Handle the case were we get off easy. No settings.
        if (configuration_->layers[layer_index]->layer_settings.size() == 0) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, "No User Settings");
            item->addChild(child);
            continue;
        }

        // There are settings.
        // Okay kid, this is where it gets complicated...
        // Is this Khronos? Khronos is special...
        if (configuration->layers[layer_index]->name == QString("VK_LAYER_KHRONOS_validation")) {
            validation_layer_file_ = configuration->layers[layer_index];
            validation_tree_item_ = item;
            BuildKhronosTree();
            continue;
        }

        // Generic is the only one left
        BuildGenericTree(item, configuration->layers[layer_index]);
    }

    ///////////////////////////////////////////////////////////////////
    // The last item is just the excluded layers
    if (!configuration->excluded_layers.isEmpty()) {
        QTreeWidgetItem *excluded_layers = new QTreeWidgetItem();
        excluded_layers->setText(0, "Excluded Layers");
        build_tree->addTopLevelItem(excluded_layers);
        for (int i = 0; i < configuration->excluded_layers.size(); i++) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, configuration->excluded_layers[i]);
            excluded_layers->addChild(child);
        }
    }

    // Walk the tree, and restore the expanded state of all the items
    SetTreeState(configuration_->setting_tree_state, 0, configuration_settings_tree_->invisibleRootItem());

    // Everyone is expanded.
    build_tree->resizeColumnToContents(0);
    //    build_tree->scrollToItem(build_tree->topLevelItem(0), QAbstractItemView::PositionAtTop);
}

//////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::BuildKhronosTree() {
    validation_preset_item_ = new QTreeWidgetItem();
    validation_preset_item_->setText(0, "Validation Preset");
    QTreeWidgetItem *next_line = new QTreeWidgetItem();

    validation_presets_combo_box_ = new QComboBox();
    validation_presets_combo_box_->blockSignals(true);
    for (int i = ValidationPresetFirst; i <= ValidationPresetLast; ++i)
        validation_presets_combo_box_->addItem(Configurator::Get().GetValidationPresetLabel(static_cast<ValidationPreset>(i)));

    validation_presets_combo_box_->setCurrentIndex(configuration_->preset);

    connect(validation_presets_combo_box_, SIGNAL(currentIndexChanged(int)), this, SLOT(khronosPresetChanged(int)));
    validation_tree_item_->addChild(validation_preset_item_);
    validation_preset_item_->addChild(next_line);
    configuration_settings_tree_->setItemWidget(next_line, 0, validation_presets_combo_box_);

    QTreeWidgetItem *validation_settings_item = new QTreeWidgetItem();
    validation_settings_item->setText(0, "Individual Settings");
    validation_preset_item_->addChild(validation_settings_item);

    // This just finds the enables and disables
    validation_settings_ =
        new KhronosSettingsAdvanced(configuration_settings_tree_, validation_settings_item, validation_layer_file_->layer_settings);

    // Look for the Debug Action and log file settings
    LayerSettings *debug_action = nullptr;
    LayerSettings *log_file = nullptr;
    for (int i = 0; i < validation_layer_file_->layer_settings.size(); i++) {
        if (validation_layer_file_->layer_settings[i]->settings_name == QString("debug_action"))
            debug_action = validation_layer_file_->layer_settings[i];

        if (validation_layer_file_->layer_settings[i]->settings_name == QString("log_filename"))
            log_file = validation_layer_file_->layer_settings[i];
    }

    Q_ASSERT(log_file != nullptr);
    Q_ASSERT(debug_action != nullptr);

    // Set them up
    QTreeWidgetItem *debug_action_item = new QTreeWidgetItem();
    validation_debug_action_ = new EnumSettingWidget(debug_action_item, debug_action);
    validation_tree_item_->addChild(debug_action_item);
    next_line = new QTreeWidgetItem();
    debug_action_item->addChild(next_line);
    configuration_settings_tree_->setItemWidget(next_line, 0, validation_debug_action_);

    validation_log_file_item_ = new QTreeWidgetItem();
    next_line = new QTreeWidgetItem();
    validation_log_file_widget_ = new FilenameSettingWidget(validation_log_file_item_, log_file);
    debug_action_item->addChild(validation_log_file_item_);
    validation_log_file_item_->addChild(next_line);
    configuration_settings_tree_->setItemWidget(next_line, 0, validation_log_file_widget_);
    next_line->setSizeHint(0, QSize(0, 28));
    compound_widgets_.push_back(next_line);
    validation_file_item_ = validation_log_file_item_;
    connect(validation_debug_action_, SIGNAL(currentIndexChanged(int)), this, SLOT(khronosDebugChanged(int)));
    if (validation_debug_action_->currentText() != QString("Log Message")) {
        validation_log_file_item_->setDisabled(true);
        validation_log_file_widget_->setDisabled(true);
    }

    // This is looking for the report flags
    for (int setting_index = 0; setting_index < validation_layer_file_->layer_settings.size(); setting_index++) {
        // Multi-enum - report flags only
        if (validation_layer_file_->layer_settings[setting_index]->settings_type == LAYER_SETTINGS_INCLUSIVE_LIST &&
            validation_layer_file_->layer_settings[setting_index]->settings_name == QString("report_flags")) {
            QTreeWidgetItem *sub_category = new QTreeWidgetItem;
            sub_category->setText(0, validation_layer_file_->layer_settings[setting_index]->settings_prompt);
            sub_category->setToolTip(0, validation_layer_file_->layer_settings[setting_index]->settings_desc);
            validation_tree_item_->addChild(sub_category);

            for (int i = 0; i < validation_layer_file_->layer_settings[setting_index]->settings_list_inclusive_value.size(); i++) {
                QTreeWidgetItem *child = new QTreeWidgetItem();
                MultiEnumSetting *control =
                    new MultiEnumSetting(validation_layer_file_->layer_settings[setting_index],
                                         validation_layer_file_->layer_settings[setting_index]->settings_list_inclusive_value[i]);
                control->setText(validation_layer_file_->layer_settings[setting_index]->settings_list_inclusive_prompt[i]);
                sub_category->addChild(child);
                configuration_settings_tree_->setItemWidget(child, 0, control);
                control->setFont(configuration_settings_tree_->font());
                connect(control, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            }

            continue;
        }
    }

    //////////////////////////////// VUID message filtering
    for (int settings_index = 0; settings_index < validation_layer_file_->layer_settings.size(); settings_index++) {
        if (validation_layer_file_->layer_settings[settings_index]->settings_type == LAYER_SETTINGS_VUID_FILTER) {
            QTreeWidgetItem *mute_message_item = new QTreeWidgetItem;

            mute_message_item->setText(0, "Mute Message VUIDs");
            validation_tree_item_->addChild(mute_message_item);

            vuid_search_widget_ = new VUIDSearchWidget();
            next_line = new QTreeWidgetItem();
            next_line->setSizeHint(0, QSize(0, 28));
            mute_message_item->addChild(next_line);
            configuration_settings_tree_->setItemWidget(next_line, 0, vuid_search_widget_);
            compound_widgets_.push_back(next_line);

            QTreeWidgetItem *pListItem = new QTreeWidgetItem();
            mute_message_item->addChild(pListItem);
            pListItem->setSizeHint(0, QSize(0, 200));
            mute_message_widget_ = new MuteMessageWidget(validation_layer_file_->layer_settings[settings_index]);
            compound_widgets_.push_back(pListItem);
            configuration_settings_tree_->setItemWidget(pListItem, 0, mute_message_widget_);

            connect(vuid_search_widget_, SIGNAL(itemSelected(const QString &)), mute_message_widget_,
                    SLOT(addItem(const QString &)));
            connect(vuid_search_widget_, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            connect(mute_message_widget_, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }
    }

    // This really does go way down here.
    connect(validation_settings_, SIGNAL(settingChanged()), this, SLOT(khronosPresetEdited()));
    validation_presets_combo_box_->blockSignals(false);

    //////// Add the preset item
    validation_tree_item_->addChild(validation_preset_item_);
}

/////////////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::khronosDebugChanged(int index) {
    (void)index;
    bool enabled = (validation_debug_action_->currentText() != QString("Log Message"));
    configuration_settings_tree_->blockSignals(true);
    validation_log_file_item_->setDisabled(enabled);
    validation_log_file_widget_->setDisabled(enabled);
    configuration_settings_tree_->blockSignals(false);
    profileEdited();
}

void SettingsTreeManager::BuildGenericTree(QTreeWidgetItem *parent, LayerFile *layer_file) {
    for (int layer_settings_index = 0, n = layer_file->layer_settings.size(); layer_settings_index < n; layer_settings_index++) {
        QTreeWidgetItem *setting_item = new QTreeWidgetItem();

        // True false?
        if (layer_file->layer_settings[layer_settings_index]->settings_type == LAYER_SETTINGS_BOOL) {
            BoolSettingWidget *widget = new BoolSettingWidget(layer_file->layer_settings[layer_settings_index]);
            parent->addChild(setting_item);
            configuration_settings_tree_->setItemWidget(setting_item, 0, widget);
            widget->setFont(configuration_settings_tree_->font());
            connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // True false? (with numeric output instead of text)
        if (layer_file->layer_settings[layer_settings_index]->settings_type == LAYER_SETTINGS_BOOL_NUMERIC) {
            BoolSettingWidget *widget = new BoolSettingWidget(layer_file->layer_settings[layer_settings_index], true);
            parent->addChild(setting_item);
            configuration_settings_tree_->setItemWidget(setting_item, 0, widget);
            widget->setFont(configuration_settings_tree_->font());
            connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Combobox - enum - just one thing
        if (layer_file->layer_settings[layer_settings_index]->settings_type == LAYER_SETTINGS_EXCLUSIVE_LIST) {
            parent->addChild(setting_item);
            setting_item->setText(0, layer_file->layer_settings[layer_settings_index]->settings_prompt);
            QTreeWidgetItem *place_holder = new QTreeWidgetItem();
            setting_item->addChild(place_holder);

            EnumSettingWidget *enum_widget = new EnumSettingWidget(setting_item, layer_file->layer_settings[layer_settings_index]);
            configuration_settings_tree_->setItemWidget(place_holder, 0, enum_widget);
            connect(enum_widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Raw text field?
        if (layer_file->layer_settings[layer_settings_index]->settings_type == LAYER_SETTINGS_STRING) {
            StringSettingWidget *widget = new StringSettingWidget(setting_item, layer_file->layer_settings[layer_settings_index]);
            parent->addChild(setting_item);
            QTreeWidgetItem *place_holder = new QTreeWidgetItem();
            setting_item->addChild(place_holder);
            configuration_settings_tree_->setItemWidget(place_holder, 0, widget);
            connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Select a file?
        if (layer_file->layer_settings[layer_settings_index]->settings_type == LAYER_SETTINGS_FILE) {
            FilenameSettingWidget *widget =
                new FilenameSettingWidget(setting_item, layer_file->layer_settings[layer_settings_index]);
            parent->addChild(setting_item);
            QTreeWidgetItem *place_holder = new QTreeWidgetItem();
            place_holder->setSizeHint(0, QSize(0, 28));
            setting_item->addChild(place_holder);
            configuration_settings_tree_->setItemWidget(place_holder, 0, widget);
            compound_widgets_.push_back(place_holder);
            connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Save to folder?
        if (layer_file->layer_settings[layer_settings_index]->settings_type == LAYER_SETTINGS_SAVE_FOLDER) {
            FolderSettingWidget *widget = new FolderSettingWidget(setting_item, layer_file->layer_settings[layer_settings_index]);
            parent->addChild(setting_item);
            QTreeWidgetItem *place_holder = new QTreeWidgetItem();
            place_holder->setSizeHint(0, QSize(0, 28));
            setting_item->addChild(place_holder);
            configuration_settings_tree_->setItemWidget(place_holder, 0, widget);
            compound_widgets_.push_back(place_holder);
            connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        ///////////////////////////////////////////////////////////////////////////
        // Undefined... at least gracefuly display what the setting is
        setting_item->setText(0, layer_file->layer_settings[layer_settings_index]->settings_prompt);
        setting_item->setToolTip(0, layer_file->layer_settings[layer_settings_index]->settings_desc);
        parent->addChild(setting_item);
    }
}

////////////////////////////////////////////////////////////////////////////////////
/// The user has selected a preset for this layer
void SettingsTreeManager::khronosPresetChanged(int preset_index) {
    const ValidationPreset preset = static_cast<ValidationPreset>(preset_index);

    Configurator &configuration = Configurator::Get();

    configuration.CheckApplicationRestart();

    // We really just don't care
    if (preset == ValidationPresetUserDefined) return;

    // The easiest way to do this is to create a new profile, and copy the layer over
    const QString preset_file = QString(":/resourcefiles/") + configuration.GetValidationPresetName(preset) + ".json";
    Configuration *preset_configuration = configuration.LoadConfiguration(preset_file);
    if (preset_configuration == nullptr) return;

    // Copy it all into the real layer and delete it
    // Find the KhronosLaer
    int validation_layer_index = -1;
    for (int i = 0; i < configuration_->layers.size(); i++)
        if (configuration_->layers[i] == validation_layer_file_) {
            validation_layer_index = i;
            break;
        }

    Q_ASSERT(validation_layer_index != -1);

    // Reset just specific layer settings
    for (int i = 0; i < configuration_->layers[validation_layer_index]->layer_settings.size(); i++) {
        if (validation_layer_file_->layer_settings[i]->settings_name == QString("disables") ||
            validation_layer_file_->layer_settings[i]->settings_name == QString("enables"))
            validation_layer_file_->layer_settings[i]->settings_value =
                preset_configuration->layers[0]->layer_settings[i]->settings_value;
    }

    delete preset_configuration;  // Delete the pattern
    configuration_->preset = preset;

    // Now we need to reload the Khronos tree item.
    configuration_settings_tree_->blockSignals(true);
    configuration_settings_tree_->setItemWidget(validation_file_item_, 1, nullptr);
    delete validation_settings_;

    QByteArray saved_state;
    QTreeWidgetItem *validation_preset_parent = validation_preset_item_->parent();
    GetTreeState(saved_state, validation_preset_parent);

    for (int i = 0, n = validation_tree_item_->childCount(); i < n; i++) {
        validation_tree_item_->takeChild(0);
    }

    BuildKhronosTree();
    SetTreeState(saved_state, 0, validation_preset_parent);
    configuration_settings_tree_->blockSignals(false);
    profileEdited();
}

///////////////////////////////////////////////////////////////////////
// Any edit to these settings means we are not user defined
// (and that we need to save the settings)
void SettingsTreeManager::khronosPresetEdited() {
    validation_presets_combo_box_->blockSignals(true);
    validation_presets_combo_box_->setCurrentIndex(ValidationPresetUserDefined);
    configuration_->preset = ValidationPresetUserDefined;
    validation_presets_combo_box_->blockSignals(false);
    profileEdited();
}

/////////////////////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::GetTreeState(QByteArray &byte_array, QTreeWidgetItem *top_item) {
    if (top_item->isExpanded())
        byte_array.push_back('1');
    else
        byte_array.push_back('0');

    for (int i = 0; i < top_item->childCount(); i++) {
        GetTreeState(byte_array, top_item->child(i));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
int SettingsTreeManager::SetTreeState(QByteArray &byte_array, int index, QTreeWidgetItem *top_item) {
    // We very well could run out, on initial run, expand everything
    if (index > byte_array.length())
        top_item->setExpanded(true);
    else {
        top_item->setExpanded(byte_array[index++] == '1');
    }

    // Walk the children
    if (top_item->childCount() != 0) {
        for (int i = 0; i < top_item->childCount(); i++) {
            index = SetTreeState(byte_array, index, top_item->child(i));
        }
    }

    return index;
}

////////////////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::CleanupGUI() {
    if (configuration_settings_tree_ == nullptr || configuration_ == nullptr) return;

    // Get the state of the last tree, and save it!
    configuration_->setting_tree_state.clear();
    GetTreeState(configuration_->setting_tree_state, configuration_settings_tree_->invisibleRootItem());
    Configurator::Get().SaveConfiguration(configuration_);

    // If a Khronos layer is present, it needs cleanup up from custom controls before
    // it's cleared or deleted.
    if (validation_layer_file_) configuration_settings_tree_->setItemWidget(validation_file_item_, 1, nullptr);

    validation_file_item_ = nullptr;

    if (validation_settings_) delete validation_settings_;

    for (int i = 0; i < compound_widgets_.size(); i++)
        configuration_settings_tree_->setItemWidget(compound_widgets_[i], 1, nullptr);

    compound_widgets_.clear();

    configuration_settings_tree_->clear();
    configuration_settings_tree_ = nullptr;
    configuration_ = nullptr;
    validation_presets_combo_box_ = nullptr;
    validation_layer_file_ = nullptr;
    validation_tree_item_ = nullptr;
    validation_debug_action_ = nullptr;
    validation_preset_item_ = nullptr;
    validation_settings_ = nullptr;
    validation_log_file_widget_ = nullptr;
    validation_log_file_item_ = nullptr;
}

/////////////////////////////////////////////////////////////
// The profile has been edited and should be saved
void SettingsTreeManager::profileEdited() {
    // Resave this profile
    Configurator &configuration = Configurator::Get();
    configuration.SaveConfiguration(configuration_);
    configuration.CheckApplicationRestart();

    // If this profile is active, we need to reset the override files too
    // Just resetting with the same parent pointer will do the trick
    if (configuration_ == configuration.GetActiveConfiguration()) configuration.SetActiveConfiguration(configuration_);
}
