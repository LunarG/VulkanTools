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
 * - Richard S. Wright Jr.
 * - Christophe Riccio
 */

#include "configurator.h"
#include "settingstreemanager.h"

#include "../vkconfig_core/version.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QApplication>

#include <cassert>

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

    // Get the Debug Action and log file settings (and they must exist)
    LayerSetting &debug_action = FindSetting(_validation_layer_file->_layer_settings, "debug_action");
    LayerSetting &log_file = FindSetting(_validation_layer_file->_layer_settings, "log_filename");

    // The debug action set of settings has it's own branch
    QTreeWidgetItem *debug_action_branch = new QTreeWidgetItem();
    debug_action_branch->setText(0, "Debug Action");
    _validation_tree_item->addChild(debug_action_branch);

    // Each debug action has it's own checkbox
    for (int i = 0, n = debug_action.inclusive_values.size(); i < n; ++i) {
        QTreeWidgetItem *child = new QTreeWidgetItem();
        MultiEnumSettingWidget *this_control = new MultiEnumSettingWidget(debug_action, debug_action.inclusive_values[i]);
        this_control->setText(debug_action.inclusive_labels[i]);
        debug_action_branch->addChild(child);
        _configuration_settings_tree->setItemWidget(child, 0, this_control);
        this_control->setFont(_configuration_settings_tree->font());
        connect(this_control, SIGNAL(itemChanged()), this, SLOT(profileEdited()));

        // The log message action also has a child; the log file selection setting/widget
        // Note, this is usually last, but I'll check for it any way in case other new items are added
        if (debug_action.inclusive_values[i] == QString("VK_DBG_LAYER_ACTION_LOG_MSG")) {  // log action?
            _validation_debug_action = this_control;
            _validation_log_file_item = new QTreeWidgetItem();
            child->addChild(_validation_log_file_item);
            _validation_log_file_widget = new FileSystemSettingWidget(_validation_log_file_item, log_file, SETTING_SAVE_FILE);
            _configuration_settings_tree->setItemWidget(_validation_log_file_item, 0, _validation_log_file_widget);
            _validation_log_file_item->setSizeHint(0, QSize(0, 28));
            _compound_widgets.push_back(child);

            connect(_validation_log_file_widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            connect(_validation_debug_action, SIGNAL(stateChanged(int)), this, SLOT(khronosDebugChanged(int)));

            // Capture initial state, which reflects enabled/disabled
            _validation_log_file_widget->setDisabled(!_validation_debug_action->isChecked());
        }
    }

    std::vector<LayerSetting> &settings = _validation_layer_file->_layer_settings;

    // This is looking for the report flags
    for (std::size_t setting_index = 0, settings_count = settings.size(); setting_index < settings_count; setting_index++) {
        LayerSetting &layer_setting = settings[setting_index];

        // Multi-enum - report flags only
        if (!(layer_setting.type == SETTING_INCLUSIVE_LIST && layer_setting.name == QString("report_flags"))) continue;

        QTreeWidgetItem *sub_category = new QTreeWidgetItem;
        sub_category->setText(0, layer_setting.label);
        sub_category->setToolTip(0, layer_setting.description);
        _validation_tree_item->addChild(sub_category);

        for (int i = 0, n = layer_setting.inclusive_values.size(); i < n; ++i) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            MultiEnumSettingWidget *control = new MultiEnumSettingWidget(layer_setting, layer_setting.inclusive_values[i]);
            control->setText(layer_setting.inclusive_labels[i]);
            sub_category->addChild(child);
            _configuration_settings_tree->setItemWidget(child, 0, control);
            control->setFont(_configuration_settings_tree->font());
            connect(control, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
        }
    }

    // VUID message filtering
    for (std::size_t setting_index = 0, settings_count = settings.size(); setting_index < settings_count; setting_index++) {
        LayerSetting &layer_setting = settings[setting_index];

        if (layer_setting.type != SETTING_VUID_FILTER) {
            continue;
        }

        QTreeWidgetItem *mute_message_item = new QTreeWidgetItem;

        mute_message_item->setText(0, "Mute Message VUIDs");
        _validation_tree_item->addChild(mute_message_item);

        _vuid_search_widget = new VUIDSearchWidget(layer_setting.value);
        next_line = new QTreeWidgetItem();
        next_line->setSizeHint(0, QSize(0, 28));
        mute_message_item->addChild(next_line);
        _configuration_settings_tree->setItemWidget(next_line, 0, _vuid_search_widget);
        _compound_widgets.push_back(next_line);

        QTreeWidgetItem *pListItem = new QTreeWidgetItem();
        mute_message_item->addChild(pListItem);
        pListItem->setSizeHint(0, QSize(0, 200));
        _mute_message_widget = new MuteMessageWidget(layer_setting);
        _compound_widgets.push_back(pListItem);
        _configuration_settings_tree->setItemWidget(pListItem, 0, _mute_message_widget);

        connect(_vuid_search_widget, SIGNAL(itemSelected(const QString &)), _mute_message_widget, SLOT(addItem(const QString &)));
        connect(_vuid_search_widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
        connect(_mute_message_widget, SIGNAL(itemRemoved(const QString &)), _vuid_search_widget,
                SLOT(addToSearchList(const QString &)));
        connect(_mute_message_widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()), Qt::QueuedConnection);
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
    bool enabled = !(_validation_debug_action->isChecked());
    _configuration_settings_tree->blockSignals(true);
    _validation_log_file_item->setDisabled(enabled);
    _validation_log_file_widget->setDisabled(enabled);
    _configuration_settings_tree->blockSignals(false);
    profileEdited();
}

void SettingsTreeManager::BuildGenericTree(QTreeWidgetItem *parent, Layer *layer) {
    for (std::size_t setting_index = 0, n = layer->_layer_settings.size(); setting_index < n; setting_index++) {
        LayerSetting &setting = layer->_layer_settings[setting_index];

        QTreeWidgetItem *setting_item = new QTreeWidgetItem();

        switch (setting.type) {
            case SETTING_BOOL:          // True false?
            case SETTING_BOOL_NUMERIC:  // True false? (with numeric output instead of text)
            {
                BoolSettingWidget *widget = new BoolSettingWidget(setting, setting.type);
                parent->addChild(setting_item);
                _configuration_settings_tree->setItemWidget(setting_item, 0, widget);
                widget->setFont(_configuration_settings_tree->font());
                connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            } break;

            case SETTING_SAVE_FILE:    // Save a file?
            case SETTING_LOAD_FILE:    // Load a file?
            case SETTING_SAVE_FOLDER:  // Save to folder?
            {
                FileSystemSettingWidget *widget = new FileSystemSettingWidget(setting_item, setting, setting.type);
                parent->addChild(setting_item);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                place_holder->setSizeHint(0, QSize(0, 28));
                setting_item->addChild(place_holder);
                _configuration_settings_tree->setItemWidget(place_holder, 0, widget);
                _compound_widgets.push_back(place_holder);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            } break;

            case SETTING_EXCLUSIVE_LIST:  // Combobox - enum - just one thing
            {
                parent->addChild(setting_item);
                setting_item->setText(0, setting.label);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                setting_item->addChild(place_holder);

                EnumSettingWidget *enum_widget = new EnumSettingWidget(setting_item, setting);
                _configuration_settings_tree->setItemWidget(place_holder, 0, enum_widget);
                connect(enum_widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            } break;

            case SETTING_STRING:  // Raw text field?
            {
                StringSettingWidget *widget = new StringSettingWidget(setting_item, setting);
                parent->addChild(setting_item);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                setting_item->addChild(place_holder);
                _configuration_settings_tree->setItemWidget(place_holder, 0, widget);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(profileEdited()));
            } break;

            default: {
                setting_item->setText(0, setting.label);
                setting_item->setToolTip(0, setting.description);
                parent->addChild(setting_item);
                assert(0);  // Unknown setting
            } break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
/// The user has selected a preset for this layer
void SettingsTreeManager::khronosPresetChanged(int preset_index) {
    const ValidationPreset preset = static_cast<ValidationPreset>(preset_index);

    Configurator &configurator = Configurator::Get();

    configurator.CheckApplicationRestart();

    // We really just don't care
    if (preset == ValidationPresetUserDefined) return;

    // The easiest way to do this is to create a new profile, and copy the layer over
    const QString preset_file = QString(":/resourcefiles/") + configurator.GetValidationPresetName(preset) + ".json";

    Configuration *preset_configuration = new Configuration;
    const bool result = preset_configuration->Load(preset_file);
    assert(result);

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
    for (std::size_t i = 0; i < _configuration->_layers[validation_layer_index]->_layer_settings.size(); i++) {
        if (_validation_layer_file->_layer_settings[i].name == "disables" ||
            _validation_layer_file->_layer_settings[i].name == "enables")
            _validation_layer_file->_layer_settings[i].value = preset_configuration->_layers[0]->_layer_settings[i].value;
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
    const bool result = _configuration->Save();
    assert(result);

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

    const bool result = _configuration->Save();
    assert(result);

    Configurator &configurator = Configurator::Get();
    configurator.CheckApplicationRestart();

    // If this profile is active, we need to reset the override files too
    // Just resetting with the same parent pointer will do the trick
    if (_configuration == configurator.GetActiveConfiguration()) configurator.SetActiveConfiguration(_configuration);
}
