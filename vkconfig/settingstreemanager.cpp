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

#include "../vkconfig_core/version.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QApplication>

SettingsTreeManager::SettingsTreeManager()
    : _configuration_settings_tree(nullptr),
      _configuration(nullptr),
      _validation_presets_combo_box(nullptr),
      _validation_layer_file(nullptr),
      _validation_tree_item(nullptr),
      _validation_file_item(nullptr),
      _validation_log_file_item(nullptr),
      _validation_log_file_widget(nullptr),
      _validation_settings(nullptr) {}

////////////////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::CreateGUI(QTreeWidget *build_tree, Configuration *configuration) {
    // Do this first to make absolutely sure if thee is an old profile still active
    // it's state gets saved.
    CleanupGUI();

    _configuration_settings_tree = build_tree;
    _configuration = configuration;

    build_tree->blockSignals(true);
    build_tree->clear();

    // There will be one top level item for each layer
    for (int layer_index = 0; layer_index < _configuration->_layers.size(); layer_index++) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, configuration->_layers[layer_index]->_name);
        _configuration_settings_tree->addTopLevelItem(item);
        _layer_items.push_back(item);

        // Handle the case were we get off easy. No settings.
        if (_configuration->_layers[layer_index]->_layer_settings.size() == 0) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, "No User Settings");
            item->addChild(child);
            continue;
        }

        // There are settings.
        // Okay kid, this is where it gets complicated...
        // Is this Khronos? Khronos is special...
        if (configuration->_layers[layer_index]->_name == QString("VK_LAYER_KHRONOS_validation")) {
            _validation_layer_file = configuration->_layers[layer_index];
            _validation_tree_item = item;
            BuildKhronosTree();
            continue;
        }

        // Generic is the only one left
        BuildGenericTree(item, configuration->_layers[layer_index]);
    }

    ///////////////////////////////////////////////////////////////////
    // The last item is just the excluded layers
    if (!configuration->_excluded_layers.isEmpty()) {
        QTreeWidgetItem *excluded_layers = new QTreeWidgetItem();
        excluded_layers->setText(0, "Excluded Layers");
        build_tree->addTopLevelItem(excluded_layers);
        for (int i = 0; i < configuration->_excluded_layers.size(); i++) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, configuration->_excluded_layers[i]);
            excluded_layers->addChild(child);
        }
    }

    // Everyone is expanded.
    build_tree->resizeColumnToContents(0);
    SetTreeState(_configuration->_setting_tree_state, 0, _configuration_settings_tree->invisibleRootItem());
    build_tree->blockSignals(false);
}

//////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::BuildKhronosTree() {
    _validation_preset_item = new QTreeWidgetItem();
    _validation_preset_item->setText(0, "Validation Preset");
    QTreeWidgetItem *next_line = new QTreeWidgetItem();

    _validation_presets_combo_box = new QComboBox();
    _validation_presets_combo_box->blockSignals(true);
    for (int i = ValidationPresetFirst; i <= ValidationPresetLast; ++i) {
        QString presetName = Configurator::Get().GetValidationPresetLabel(static_cast<ValidationPreset>(i));

        // There is no preset for a user defined group of settings, so watch for blank.
        if (presetName.isEmpty()) presetName = "User Defined";

        _validation_presets_combo_box->addItem(presetName);
    }

    _validation_presets_combo_box->setCurrentIndex(_configuration->_preset);

    connect(_validation_presets_combo_box, SIGNAL(currentIndexChanged(int)), this, SLOT(khronosPresetChanged(int)));
    _validation_tree_item->addChild(_validation_preset_item);
    _validation_preset_item->addChild(next_line);
    _configuration_settings_tree->setItemWidget(next_line, 0, _validation_presets_combo_box);

    QTreeWidgetItem *_validation_settingsitem = new QTreeWidgetItem();
    _validation_settingsitem->setText(0, "Individual Settings");
    _validation_preset_item->addChild(_validation_settingsitem);

    // This just finds the enables and disables
    _validation_settings = new KhronosSettingsAdvanced(_configuration_settings_tree, _validation_settingsitem,
                                                       _validation_layer_file->_layer_settings);

    // Look for the Debug Action and log file settings
    LayerSetting *debug_action = nullptr;
    LayerSetting *log_file = nullptr;
    for (int i = 0; i < _validation_layer_file->_layer_settings.size(); i++) {
        if (_validation_layer_file->_layer_settings[i]->name == QString("debug_action"))
            debug_action = _validation_layer_file->_layer_settings[i];

        if (_validation_layer_file->_layer_settings[i]->name == QString("log_filename"))
            log_file = _validation_layer_file->_layer_settings[i];
    }

    Q_ASSERT(log_file != nullptr);
    Q_ASSERT(debug_action != nullptr);

    // Set them up
    QTreeWidgetItem *debug_action_item = new QTreeWidgetItem();
    _validation_debug_action = new EnumSettingWidget(debug_action_item, debug_action);
    _validation_tree_item->addChild(debug_action_item);
    next_line = new QTreeWidgetItem();
    debug_action_item->addChild(next_line);
    _configuration_settings_tree->setItemWidget(next_line, 0, _validation_debug_action);

    _validation_log_file_item = new QTreeWidgetItem();
    next_line = new QTreeWidgetItem();
    _validation_log_file_widget = new FilenameSettingWidget(_validation_log_file_item, log_file);
    connect(_validation_log_file_widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
    debug_action_item->addChild(_validation_log_file_item);
    _validation_log_file_item->addChild(next_line);
    _configuration_settings_tree->setItemWidget(next_line, 0, _validation_log_file_widget);
    next_line->setSizeHint(0, QSize(0, 28));
    _compound_widgets.push_back(next_line);
    _validation_file_item = _validation_log_file_item;
    connect(_validation_debug_action, SIGNAL(currentIndexChanged(int)), this, SLOT(khronosDebugChanged(int)));
    if (_validation_debug_action->currentText() != QString("Log Message")) {
        _validation_log_file_item->setDisabled(true);
        _validation_log_file_widget->setDisabled(true);
    }

    // This is looking for the report flags
    for (int setting_index = 0; setting_index < _validation_layer_file->_layer_settings.size(); setting_index++) {
        // Multi-enum - report flags only
        if (_validation_layer_file->_layer_settings[setting_index]->type == SETTING_INCLUSIVE_LIST &&
            _validation_layer_file->_layer_settings[setting_index]->name == QString("report_flags")) {
            QTreeWidgetItem *sub_category = new QTreeWidgetItem;
            sub_category->setText(0, _validation_layer_file->_layer_settings[setting_index]->label);
            sub_category->setToolTip(0, _validation_layer_file->_layer_settings[setting_index]->description);
            _validation_tree_item->addChild(sub_category);

            for (int i = 0; i < _validation_layer_file->_layer_settings[setting_index]->inclusive_values.size(); i++) {
                QTreeWidgetItem *child = new QTreeWidgetItem();
                MultiEnumSetting *control =
                    new MultiEnumSetting(_validation_layer_file->_layer_settings[setting_index],
                                         _validation_layer_file->_layer_settings[setting_index]->inclusive_values[i]);
                control->setText(_validation_layer_file->_layer_settings[setting_index]->inclusive_labels[i]);
                sub_category->addChild(child);
                _configuration_settings_tree->setItemWidget(child, 0, control);
                control->setFont(_configuration_settings_tree->font());
                connect(control, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            }

            continue;
        }
    }

    //////////////////////////////// VUID message filtering
    for (int settings_index = 0; settings_index < _validation_layer_file->_layer_settings.size(); settings_index++) {
        if (_validation_layer_file->_layer_settings[settings_index]->type == SETTING_VUID_FILTER) {
            QTreeWidgetItem *mute_message_item = new QTreeWidgetItem;

            mute_message_item->setText(0, "Mute Message VUIDs");
            _validation_tree_item->addChild(mute_message_item);

            _vuid_search_widget = new VUIDSearchWidget(_validation_layer_file->_layer_settings[settings_index]->value);
            next_line = new QTreeWidgetItem();
            next_line->setSizeHint(0, QSize(0, 28));
            mute_message_item->addChild(next_line);
            _configuration_settings_tree->setItemWidget(next_line, 0, _vuid_search_widget);
            _compound_widgets.push_back(next_line);

            QTreeWidgetItem *pListItem = new QTreeWidgetItem();
            mute_message_item->addChild(pListItem);
            pListItem->setSizeHint(0, QSize(0, 200));
            _mute_message_widget = new MuteMessageWidget(_validation_layer_file->_layer_settings[settings_index]);
            _compound_widgets.push_back(pListItem);
            _configuration_settings_tree->setItemWidget(pListItem, 0, _mute_message_widget);

            connect(_vuid_search_widget, SIGNAL(itemSelected(const QString &)), _mute_message_widget,
                    SLOT(addItem(const QString &)));
            connect(_vuid_search_widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            connect(_mute_message_widget, SIGNAL(itemRemoved(const QString &)), _vuid_search_widget,
                    SLOT(addToSearchList(const QString &)));
            connect(_mute_message_widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()), Qt::QueuedConnection);
            continue;
        }
    }

    // This really does go way down here.
    connect(_validation_settings, SIGNAL(settingChanged()), this, SLOT(khronosPresetEdited()));
    _validation_presets_combo_box->blockSignals(false);

    //////// Add the preset item
    _validation_tree_item->addChild(_validation_preset_item);
}

/////////////////////////////////////////////////////////////////////////////////
void SettingsTreeManager::khronosDebugChanged(int index) {
    (void)index;
    bool enabled = (_validation_debug_action->currentText() != QString("Log Message"));
    _configuration_settings_tree->blockSignals(true);
    _validation_log_file_item->setDisabled(enabled);
    _validation_log_file_widget->setDisabled(enabled);
    _configuration_settings_tree->blockSignals(false);
    profileEdited();
}

void SettingsTreeManager::BuildGenericTree(QTreeWidgetItem *parent, LayerFile *layer_file) {
    for (int layer_settings_index = 0, n = layer_file->_layer_settings.size(); layer_settings_index < n; layer_settings_index++) {
        QTreeWidgetItem *setting_item = new QTreeWidgetItem();

        // True false?
        if (layer_file->_layer_settings[layer_settings_index]->type == SETTING_BOOL) {
            BoolSettingWidget *widget = new BoolSettingWidget(layer_file->_layer_settings[layer_settings_index]);
            parent->addChild(setting_item);
            _configuration_settings_tree->setItemWidget(setting_item, 0, widget);
            widget->setFont(_configuration_settings_tree->font());
            connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // True false? (with numeric output instead of text)
        if (layer_file->_layer_settings[layer_settings_index]->type == SETTING_BOOL_NUMERIC) {
            BoolSettingWidget *widget = new BoolSettingWidget(layer_file->_layer_settings[layer_settings_index], true);
            parent->addChild(setting_item);
            _configuration_settings_tree->setItemWidget(setting_item, 0, widget);
            widget->setFont(_configuration_settings_tree->font());
            connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Combobox - enum - just one thing
        if (layer_file->_layer_settings[layer_settings_index]->type == SETTING_EXCLUSIVE_LIST) {
            parent->addChild(setting_item);
            setting_item->setText(0, layer_file->_layer_settings[layer_settings_index]->label);
            QTreeWidgetItem *place_holder = new QTreeWidgetItem();
            setting_item->addChild(place_holder);

            EnumSettingWidget *enum_widget = new EnumSettingWidget(setting_item, layer_file->_layer_settings[layer_settings_index]);
            _configuration_settings_tree->setItemWidget(place_holder, 0, enum_widget);
            connect(enum_widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Raw text field?
        if (layer_file->_layer_settings[layer_settings_index]->type == SETTING_STRING) {
            StringSettingWidget *widget = new StringSettingWidget(setting_item, layer_file->_layer_settings[layer_settings_index]);
            parent->addChild(setting_item);
            QTreeWidgetItem *place_holder = new QTreeWidgetItem();
            setting_item->addChild(place_holder);
            _configuration_settings_tree->setItemWidget(place_holder, 0, widget);
            connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Select a file?
        if (layer_file->_layer_settings[layer_settings_index]->type == SETTING_SAVE_FILE) {
            FilenameSettingWidget *widget =
                new FilenameSettingWidget(setting_item, layer_file->_layer_settings[layer_settings_index]);
            parent->addChild(setting_item);
            QTreeWidgetItem *place_holder = new QTreeWidgetItem();
            place_holder->setSizeHint(0, QSize(0, 28));
            setting_item->addChild(place_holder);
            _configuration_settings_tree->setItemWidget(place_holder, 0, widget);
            _compound_widgets.push_back(place_holder);
            connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Load a file?
        if (layer_file->_layer_settings[layer_settings_index]->type == SETTING_LOAD_FILE) {
            FilenameSettingWidget *widget =
                new FilenameSettingWidget(setting_item, layer_file->_layer_settings[layer_settings_index], false);
            parent->addChild(setting_item);
            QTreeWidgetItem *place_holder = new QTreeWidgetItem();
            place_holder->setSizeHint(0, QSize(0, 28));
            setting_item->addChild(place_holder);
            _configuration_settings_tree->setItemWidget(place_holder, 0, widget);
            _compound_widgets.push_back(place_holder);
            connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        // Save to folder?
        if (layer_file->_layer_settings[layer_settings_index]->type == SETTING_SAVE_FOLDER) {
            FolderSettingWidget *widget = new FolderSettingWidget(setting_item, layer_file->_layer_settings[layer_settings_index]);
            parent->addChild(setting_item);
            QTreeWidgetItem *place_holder = new QTreeWidgetItem();
            place_holder->setSizeHint(0, QSize(0, 28));
            setting_item->addChild(place_holder);
            _configuration_settings_tree->setItemWidget(place_holder, 0, widget);
            _compound_widgets.push_back(place_holder);
            connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            continue;
        }

        ///////////////////////////////////////////////////////////////////////////
        // Undefined... at least gracefuly display what the setting is
        setting_item->setText(0, layer_file->_layer_settings[layer_settings_index]->label);
        setting_item->setToolTip(0, layer_file->_layer_settings[layer_settings_index]->description);
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
    for (int i = 0; i < _configuration->_layers.size(); i++)
        if (_configuration->_layers[i] == _validation_layer_file) {
            validation_layer_index = i;
            break;
        }

    Q_ASSERT(validation_layer_index != -1);

    // Reset just specific layer settings
    for (int i = 0; i < _configuration->_layers[validation_layer_index]->_layer_settings.size(); i++) {
        if (_validation_layer_file->_layer_settings[i]->name == QString("disables") ||
            _validation_layer_file->_layer_settings[i]->name == QString("enables"))
            _validation_layer_file->_layer_settings[i]->value = preset_configuration->_layers[0]->_layer_settings[i]->value;
    }

    delete preset_configuration;  // Delete the pattern
    _configuration->_preset = preset;

    // Now we need to reload the Khronos tree item.
    _configuration_settings_tree->blockSignals(true);
    _configuration_settings_tree->setItemWidget(_validation_file_item, 1, nullptr);
    delete _validation_settings;

    QByteArray saved_state;
    QTreeWidgetItem *validation_preset_parent = _validation_preset_item->parent();
    GetTreeState(saved_state, validation_preset_parent);

    for (int i = 0, n = _validation_tree_item->childCount(); i < n; i++) {
        _validation_tree_item->takeChild(0);
    }

    BuildKhronosTree();
    SetTreeState(saved_state, 0, validation_preset_parent);
    _configuration_settings_tree->blockSignals(false);
    profileEdited();
}

///////////////////////////////////////////////////////////////////////
// Any edit to these settings means we are not user defined
// (and that we need to save the settings)
void SettingsTreeManager::khronosPresetEdited() {
    _validation_presets_combo_box->blockSignals(true);
    _validation_presets_combo_box->setCurrentIndex(ValidationPresetUserDefined);
    _configuration->_preset = ValidationPresetUserDefined;
    _validation_presets_combo_box->blockSignals(false);
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
    if (_configuration_settings_tree == nullptr || _configuration == nullptr) return;

    // Get the state of the last tree, and save it!
    _configuration->_setting_tree_state.clear();
    GetTreeState(_configuration->_setting_tree_state, _configuration_settings_tree->invisibleRootItem());
    Configurator::Get().SaveConfiguration(_configuration);

    // If a Khronos layer is present, it needs cleanup up from custom controls before
    // it's cleared or deleted.
    if (_validation_layer_file) _configuration_settings_tree->setItemWidget(_validation_file_item, 1, nullptr);

    _validation_file_item = nullptr;

    if (_validation_settings) delete _validation_settings;

    for (int i = 0; i < _compound_widgets.size(); i++)
        _configuration_settings_tree->setItemWidget(_compound_widgets[i], 1, nullptr);

    _compound_widgets.clear();

    _configuration_settings_tree->clear();
    _configuration_settings_tree = nullptr;
    _configuration = nullptr;
    _validation_presets_combo_box = nullptr;
    _validation_layer_file = nullptr;
    _validation_tree_item = nullptr;
    _validation_debug_action = nullptr;
    _validation_preset_item = nullptr;
    _validation_settings = nullptr;
    _validation_log_file_widget = nullptr;
    _validation_log_file_item = nullptr;
}

/////////////////////////////////////////////////////////////
// The profile has been edited and should be saved
void SettingsTreeManager::profileEdited() {
    // Resave this profile
    Configurator &configuration = Configurator::Get();
    configuration.SaveConfiguration(_configuration);
    configuration.CheckApplicationRestart();

    // If this profile is active, we need to reset the override files too
    // Just resetting with the same parent pointer will do the trick
    if (_configuration == configuration.GetActiveConfiguration()) configuration.SetActiveConfiguration(_configuration);
}
