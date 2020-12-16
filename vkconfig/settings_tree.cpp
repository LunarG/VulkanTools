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
#include "settings_tree.h"

#include "../vkconfig_core/version.h"
#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/util.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QApplication>

#include <cassert>

SettingsTreeManager::SettingsTreeManager()
    : _settings_tree(nullptr),
      _validation_tree_item(nullptr),
      _validation_log_file_item(nullptr),
      _validation_log_file_widget(nullptr),
      _validation_areas(nullptr) {}

void SettingsTreeManager::CreateGUI(QTreeWidget *build_tree) {
    assert(build_tree);

    // Do this first to make absolutely sure if thee is an old configuration still active
    // it's state gets saved.
    CleanupGUI();

    _settings_tree = build_tree;
    auto configuration = Configurator::Get().GetActiveConfiguration();

    build_tree->blockSignals(true);
    build_tree->clear();

    if (configuration->parameters.empty()) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, "No overridden or excluded layer");
        build_tree->addTopLevelItem(item);
    } else {
        // There will be one top level item for each layer
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            Parameter &parameter = configuration->parameters[i];
            if (parameter.state != LAYER_STATE_OVERRIDDEN) continue;

            const std::vector<Layer> &available_layers = Configurator::Get().layers.available_layers;
            const std::vector<Layer>::const_iterator layer = FindItByKey(available_layers, parameter.key.c_str());

            QTreeWidgetItem *layer_item = new QTreeWidgetItem();
            layer_item->setText(0, (parameter.key + (layer != available_layers.end() ? "" : " (Missing)")).c_str());
            layer_item->setToolTip(0, layer->description);
            _settings_tree->addTopLevelItem(layer_item);

            if (layer == available_layers.end()) continue;

            // Handle the case were we get off easy. No settings.
            if (parameter.settings.empty()) {
                QTreeWidgetItem *layer_child_item = new QTreeWidgetItem();
                layer_child_item->setText(0, "No User Settings");
                layer_item->addChild(layer_child_item);
                continue;
            }

            if (!layer->presets.empty()) {
                QTreeWidgetItem *presets_item = new QTreeWidgetItem();
                PresetWidget *presets_combobox = new PresetWidget(presets_item, *layer, parameter);
                connect(presets_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnPresetChanged(int)));
                layer_item->addChild(presets_item);
                _settings_tree->setItemWidget(presets_item, 0, presets_combobox);
                _presets_comboboxes.push_back(presets_combobox);
            }

            if (parameter.key == "VK_LAYER_KHRONOS_validation") {
                BuildValidationTree(layer_item, parameter);
            } else {
                BuildGenericTree(layer_item, parameter);
            }
        }

        // The last item is just the excluded layers
        QTreeWidgetItem *excluded_layers = new QTreeWidgetItem();
        excluded_layers->setText(0, "Excluded Layers:");
        build_tree->addTopLevelItem(excluded_layers);

        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            Parameter &parameter = configuration->parameters[i];
            if (parameter.state != LAYER_STATE_EXCLUDED) continue;

            const std::vector<Layer>::const_iterator layer =
                FindItByKey(Configurator::Get().layers.available_layers, parameter.key.c_str());

            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(
                0, (parameter.key + (layer != Configurator::Get().layers.available_layers.end() ? "" : " (Missing)")).c_str());
            child->setToolTip(0, layer->description);
            excluded_layers->addChild(child);
        }

        // None excluded layer were found
        if (excluded_layers->childCount() == 0) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, "None");
            excluded_layers->addChild(child);
        }
    }

    // Everyone is expanded.
    build_tree->resizeColumnToContents(0);
    SetTreeState(configuration->setting_tree_state, 0, _settings_tree->invisibleRootItem());
    build_tree->blockSignals(false);
}

void SettingsTreeManager::CleanupGUI() {
    if (_settings_tree == nullptr)  // Was not initialized
        return;

    Configurator &configurator = Configurator::Get();

    std::vector<Configuration>::iterator configuration = configurator.GetActiveConfiguration();
    if (configuration == configurator.available_configurations.end()) return;

    // Get the state of the last tree, and save it!
    configuration->setting_tree_state.clear();
    GetTreeState(configuration->setting_tree_state, _settings_tree->invisibleRootItem());

    if (_validation_areas) {
        delete _validation_areas;
        _validation_areas = nullptr;
    }

    for (std::size_t i = 0; i < _compound_widgets.size(); i++) _settings_tree->setItemWidget(_compound_widgets[i], 1, nullptr);

    _compound_widgets.clear();
    _presets_comboboxes.clear();

    _settings_tree->clear();
    _settings_tree = nullptr;
    _validation_tree_item = nullptr;
    _validation_debug_action = nullptr;
    _validation_log_file_widget = nullptr;
    _validation_log_file_item = nullptr;
}

void SettingsTreeManager::BuildValidationTree(QTreeWidgetItem *parent, Parameter &parameter) {
    _validation_tree_item = parent;

    Configurator &configurator = Configurator::Get();
    std::vector<Layer> &available_layers = configurator.layers.available_layers;
    std::vector<Layer>::iterator validation_layer = FindItByKey(available_layers, "VK_LAYER_KHRONOS_validation");

    QTreeWidgetItem *_validation_areas_item = new QTreeWidgetItem();
    _validation_areas_item->setText(0, "Validation Checks");
    _validation_tree_item->addChild(_validation_areas_item);

    // This just finds the enables and disables
    _validation_areas = new SettingsValidationAreas(_settings_tree, _validation_areas_item, parameter.settings);

    // Get the Debug Action and log file settings (and they must exist)
    LayerSettingMeta *debug_action_meta = FindByKey(validation_layer->settings, "debug_action");
    assert(debug_action_meta);
    LayerSettingData *debug_action_data = FindByKey(parameter.settings, "debug_action");
    assert(debug_action_data);

    LayerSettingMeta *log_file_meta = FindByKey(validation_layer->settings, "log_filename");
    assert(log_file_meta);
    LayerSettingData *log_file_data = FindByKey(parameter.settings, "log_filename");
    assert(log_file_data);

    // The debug action set of settings has it's own branch
    QTreeWidgetItem *debug_action_branch = new QTreeWidgetItem();
    debug_action_branch->setText(0, debug_action_meta->label);
    _validation_tree_item->addChild(debug_action_branch);

    // Each debug action has it's own checkbox
    for (int i = 0, n = debug_action_meta->enum_values.size(); i < n; ++i) {
        // Debug output is only for Windows
        if (VKC_PLATFORM != VKC_PLATFORM_WINDOWS && debug_action_meta->enum_values[i] == "VK_DBG_LAYER_ACTION_DEBUG_OUTPUT")
            continue;

        QTreeWidgetItem *child = new QTreeWidgetItem();
        MultiEnumSettingWidget *this_control =
            new MultiEnumSettingWidget(*debug_action_data, debug_action_meta->enum_values[i].toStdString().c_str());
        this_control->setText(debug_action_meta->enum_labels[i]);
        debug_action_branch->addChild(child);
        _settings_tree->setItemWidget(child, 0, this_control);
        this_control->setFont(_settings_tree->font());
        connect(this_control, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

        // The log message action also has a child; the log file selection setting/widget
        // Note, this is usually last, but I'll check for it any way in case other new items are added
        if (debug_action_meta->enum_values[i] == "VK_DBG_LAYER_ACTION_LOG_MSG") {  // log action?
            _validation_debug_action = this_control;
            _validation_log_file_item = new QTreeWidgetItem();
            child->addChild(_validation_log_file_item);
            _validation_log_file_widget = new FileSystemSettingWidget(_validation_log_file_item, *log_file_meta, *log_file_data);
            _validation_log_file_item->setSizeHint(0, QSize(0, 28));
            _settings_tree->setItemWidget(_validation_log_file_item, 0, _validation_log_file_widget);
            _compound_widgets.push_back(child);

            connect(_validation_log_file_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            connect(_validation_debug_action, SIGNAL(stateChanged(int)), this, SLOT(khronosDebugChanged(int)));

            // Capture initial state, which reflects enabled/disabled
            _validation_log_file_widget->setDisabled(!_validation_debug_action->isChecked());
        }
    }

    // This is looking for the report flags
    for (std::size_t setting_index = 0, settings_count = validation_layer->settings.size(); setting_index < settings_count;
         setting_index++) {
        const LayerSettingMeta &layer_setting_meta = validation_layer->settings[setting_index];
        LayerSettingData *layer_setting_data = FindByKey(parameter.settings, layer_setting_meta.key.c_str());
        if (layer_setting_data == nullptr) {
            LayerSettingData new_layer_setting_data;
            new_layer_setting_data.key = layer_setting_meta.key;
            new_layer_setting_data.value = layer_setting_meta.default_value.toStdString();
            parameter.settings.push_back(new_layer_setting_data);

            layer_setting_data = FindByKey(parameter.settings, layer_setting_meta.key.c_str());
        }

        // Multi-enum - report flags only
        if (layer_setting_meta.key == "report_flags") {
            QTreeWidgetItem *sub_category = new QTreeWidgetItem;
            sub_category->setText(0, layer_setting_meta.label);
            sub_category->setToolTip(0, layer_setting_meta.description);
            _validation_tree_item->addChild(sub_category);

            for (int i = 0, n = layer_setting_meta.enum_values.size(); i < n; ++i) {
                QTreeWidgetItem *child = new QTreeWidgetItem();
                MultiEnumSettingWidget *control =
                    new MultiEnumSettingWidget(*layer_setting_data, layer_setting_meta.enum_values[i].toStdString().c_str());
                control->setText(layer_setting_meta.enum_labels[i]);
                sub_category->addChild(child);
                _settings_tree->setItemWidget(child, 0, control);
                control->setFont(_settings_tree->font());
                connect(control, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            }
        } else if (layer_setting_meta.key == "duplicate_message_limit") {
            QTreeWidgetItem *setting_item = new QTreeWidgetItem();
            StringSettingWidget *widget = new StringSettingWidget(setting_item, layer_setting_meta, *layer_setting_data);
            _validation_tree_item->addChild(setting_item);
            QTreeWidgetItem *place_holder = new QTreeWidgetItem();
            setting_item->addChild(place_holder);
            _settings_tree->setItemWidget(place_holder, 0, widget);
            connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        }
    }

    // VUID message filtering
    for (std::size_t setting_index = 0, settings_count = validation_layer->settings.size(); setting_index < settings_count;
         setting_index++) {
        const LayerSettingMeta &layer_setting_meta = validation_layer->settings[setting_index];

        if (layer_setting_meta.type != SETTING_VUID_FILTER) {
            continue;
        }

        QTreeWidgetItem *mute_message_item = new QTreeWidgetItem;

        mute_message_item->setText(0, layer_setting_meta.label);
        _validation_tree_item->addChild(mute_message_item);

        const LayerSettingMeta *layer_setting_vuid = FindByKey(validation_layer->settings, "message_id_filter");
        assert(layer_setting_vuid);

        LayerSettingData *layer_setting_data = FindByKey(parameter.settings, layer_setting_meta.key.c_str());
        assert(layer_setting_data);

        _vuid_search_widget = new VUIDSearchWidget(layer_setting_vuid->enum_values, layer_setting_data->value.c_str());

        QTreeWidgetItem *next_line = new QTreeWidgetItem();
        next_line->setSizeHint(0, QSize(0, 28));
        mute_message_item->addChild(next_line);
        _settings_tree->setItemWidget(next_line, 0, _vuid_search_widget);
        _compound_widgets.push_back(next_line);

        QTreeWidgetItem *list_item = new QTreeWidgetItem();
        mute_message_item->addChild(list_item);
        list_item->setSizeHint(0, QSize(0, 200));
        _mute_message_widget = new MuteMessageWidget(*layer_setting_data);
        _compound_widgets.push_back(list_item);
        _settings_tree->setItemWidget(list_item, 0, _mute_message_widget);

        connect(_vuid_search_widget, SIGNAL(itemSelected(const QString &)), _mute_message_widget, SLOT(addItem(const QString &)));
        connect(_vuid_search_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        connect(_mute_message_widget, SIGNAL(itemRemoved(const QString &)), _vuid_search_widget,
                SLOT(addToSearchList(const QString &)));
        connect(_mute_message_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()), Qt::QueuedConnection);
    }

    // This really does go way down here.
    connect(_validation_areas, SIGNAL(settingChanged()), this, SLOT(OnSettingChanged()));
}

void SettingsTreeManager::khronosDebugChanged(int index) {
    (void)index;
    bool enabled = !(_validation_debug_action->isChecked());
    _settings_tree->blockSignals(true);
    _validation_log_file_item->setDisabled(enabled);
    _validation_log_file_widget->setDisabled(enabled);
    _settings_tree->blockSignals(false);
    OnSettingChanged();
}

void SettingsTreeManager::BuildGenericTree(QTreeWidgetItem *parent, Parameter &parameter) {
    std::vector<Layer> &available_layers = Configurator::Get().layers.available_layers;

    const std::vector<LayerSettingMeta> &layer_setting_metas = FindItByKey(available_layers, parameter.key.c_str())->settings;

    for (std::size_t setting_index = 0, n = layer_setting_metas.size(); setting_index < n; ++setting_index) {
        const LayerSettingMeta &layer_setting_meta = layer_setting_metas[setting_index];
        LayerSettingData *layer_setting_data = FindByKey(parameter.settings, layer_setting_meta.key.c_str());
        assert(layer_setting_data);

        QTreeWidgetItem *setting_item = new QTreeWidgetItem();
        parent->addChild(setting_item);

        switch (layer_setting_meta.type) {
            case SETTING_BOOL:          // True false?
            case SETTING_BOOL_NUMERIC:  // True false? (with numeric output instead of text)
            {
                BoolSettingWidget *widget = new BoolSettingWidget(layer_setting_meta, *layer_setting_data);
                _settings_tree->setItemWidget(setting_item, 0, widget);
                widget->setFont(_settings_tree->font());
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_SAVE_FILE:    // Save a file?
            case SETTING_LOAD_FILE:    // Load a file?
            case SETTING_SAVE_FOLDER:  // Save to folder?
            {
                FileSystemSettingWidget *widget =
                    new FileSystemSettingWidget(setting_item, layer_setting_meta, *layer_setting_data);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                place_holder->setSizeHint(0, QSize(0, 28));
                setting_item->addChild(place_holder);
                _settings_tree->setItemWidget(place_holder, 0, widget);
                _compound_widgets.push_back(place_holder);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_EXCLUSIVE_LIST:  // Combobox - enum - just one thing
            {
                setting_item->setText(0, layer_setting_meta.label);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                setting_item->addChild(place_holder);

                EnumSettingWidget *enum_widget = new EnumSettingWidget(setting_item, layer_setting_meta, *layer_setting_data);
                _settings_tree->setItemWidget(place_holder, 0, enum_widget);
                connect(enum_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_STRING:  // Raw text field?
            {
                StringSettingWidget *widget = new StringSettingWidget(setting_item, layer_setting_meta, *layer_setting_data);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                setting_item->addChild(place_holder);
                _settings_tree->setItemWidget(place_holder, 0, widget);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            default: {
                setting_item->setText(0, layer_setting_meta.label);
                setting_item->setToolTip(0, layer_setting_meta.description);
                assert(0);  // Unknown setting
            } break;
        }
    }
}

void SettingsTreeManager::OnPresetChanged(int combox_preset_index) {
    (void)combox_preset_index;

    CreateGUI(_settings_tree);

    Configurator &configurator = Configurator::Get();
    configurator.environment.Notify(NOTIFICATION_RESTART);
    configurator.RefreshConfiguration();
}

void SettingsTreeManager::GetTreeState(QByteArray &byte_array, QTreeWidgetItem *top_item) {
    byte_array.push_back(top_item->isExpanded() ? '1' : '0');

    for (int i = 0, n = top_item->childCount(); i < n; ++i) {
        GetTreeState(byte_array, top_item->child(i));
    }
}

int SettingsTreeManager::SetTreeState(QByteArray &byte_array, int index, QTreeWidgetItem *top_item) {
    // We very well could run out, on initial run, expand everything
    if (index > byte_array.length())
        top_item->setExpanded(true);
    else {
        top_item->setExpanded(byte_array[index++] == '1');
    }

    // Walk the children
    if (top_item->childCount() != 0) {
        for (int i = 0, n = top_item->childCount(); i < n; ++i) {
            index = SetTreeState(byte_array, index, top_item->child(i));
        }
    }

    return index;
}

// The setting has been edited
void SettingsTreeManager::OnSettingChanged() {
    for (std::size_t i = 0, n = _presets_comboboxes.size(); i < n; ++i) {
        _presets_comboboxes[i]->UpdateCurrentIndex();
    }

    Configurator &configurator = Configurator::Get();
    configurator.environment.Notify(NOTIFICATION_RESTART);
    configurator.RefreshConfiguration();
}
