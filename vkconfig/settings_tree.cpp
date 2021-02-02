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

#include "widget_setting_int.h"
#include "widget_setting_int_range.h"
#include "widget_setting_bool.h"
#include "widget_setting_enum.h"
#include "widget_setting_string.h"
#include "widget_setting_flags.h"
#include "widget_setting_filesystem.h"
#include "widget_setting_vuid_filter.h"
#include "widget_setting_vuid_search.h"

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
      _validation_log_file_item(nullptr),
      _validation_log_file_widget(nullptr),
      _validation_areas(nullptr) {}

void SettingsTreeManager::CreateGUI(QTreeWidget *build_tree) {
    assert(build_tree);

    // Do this first to make absolutely sure if these is an old configuration still active it's state gets saved.
    CleanupGUI();

    Configurator &configurator = Configurator::Get();

    _settings_tree = build_tree;
    Configuration *configuration = configurator.configurations.GetActiveConfiguration();
    assert(configuration != nullptr);

    build_tree->blockSignals(true);
    build_tree->clear();

    if (!configuration->HasOverride()) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, "No overridden or excluded layer");
        build_tree->addTopLevelItem(item);
    } else {
        // There will be one top level item for each layer
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            Parameter &parameter = configuration->parameters[i];
            if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) continue;

            if (parameter.state != LAYER_STATE_OVERRIDDEN) continue;

            const std::vector<Layer> &available_layers = configurator.layers.available_layers;
            const Layer *layer = FindByKey(available_layers, parameter.key.c_str());

            QTreeWidgetItem *layer_item = new QTreeWidgetItem();
            std::string layer_text = parameter.key;
            if (layer == nullptr) {
                layer_text += " (Missing)";
            } else if (layer->status != STATUS_STABLE) {
                layer_text += std::string(" (") + GetToken(layer->status) + ")";
            }

            layer_item->setText(0, layer_text.c_str());
            if (layer != nullptr) layer_item->setToolTip(0, layer->description.c_str());
            _settings_tree->addTopLevelItem(layer_item);

            if (layer == nullptr) continue;

            // Handle the case were we get off easy. No settings.
            if (parameter.settings.Empty()) {
                QTreeWidgetItem *layer_child_item = new QTreeWidgetItem();
                layer_child_item->setText(0, "No User Settings");
                layer_item->addChild(layer_child_item);
                continue;
            }

            if (!layer->presets.empty()) {
                QTreeWidgetItem *presets_item = new QTreeWidgetItem();
                WidgetPreset *presets_combobox = new WidgetPreset(presets_item, *layer, parameter);
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
            if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) continue;

            if (parameter.state != LAYER_STATE_EXCLUDED) continue;

            const Layer *layer = FindByKey(configurator.layers.available_layers, parameter.key.c_str());
            if (layer == nullptr) continue;  // Do not display missing excluded layers

            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, parameter.key.c_str());
            if (layer != nullptr) {
                child->setToolTip(0, layer->description.c_str());
            }
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

    Configuration *configuration = configurator.configurations.GetActiveConfiguration();
    if (configuration == nullptr) return;

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
    _validation_debug_action = nullptr;
    _validation_log_file_widget = nullptr;
    _validation_log_file_item = nullptr;
}

void SettingsTreeManager::BuildValidationTree(QTreeWidgetItem *parent, Parameter &parameter) {
    Configurator &configurator = Configurator::Get();
    std::vector<Layer> &available_layers = configurator.layers.available_layers;
    Layer *validation_layer = FindByKey(available_layers, "VK_LAYER_KHRONOS_validation");

    QTreeWidgetItem *validation_areas_item = new QTreeWidgetItem();
    validation_areas_item->setText(0, "Validation Areas");
    parent->addChild(validation_areas_item);

    // This just finds the enables and disables
    _validation_areas = new SettingsValidationAreas(_settings_tree, validation_areas_item, validation_layer->_api_version,
                                                    validation_layer->settings, parameter.settings);

    // Get the Debug Action and log file settings (and they must exist)
    assert(validation_layer->settings.Get("debug_action"));
    assert(validation_layer->settings.Get("debug_action")->GetType() == SETTING_FLAGS);
    assert(parameter.settings.Get("debug_action"));
    assert(parameter.settings.Get("debug_action")->GetType() == SETTING_FLAGS);

    const SettingMetaFlags *debug_action_meta =
        static_cast<const SettingMetaFlags *>(validation_layer->settings.Get("debug_action"));
    SettingDataFlags *debug_action_data = static_cast<SettingDataFlags *>(parameter.settings.Get("debug_action"));

    assert(validation_layer->settings.Get("log_filename"));
    assert(validation_layer->settings.Get("log_filename")->GetType() == SETTING_SAVE_FILE);
    assert(parameter.settings.Get("log_filename"));
    assert(parameter.settings.Get("log_filename")->GetType() == SETTING_SAVE_FILE);

    const SettingMetaFileSave *log_file_meta =
        static_cast<const SettingMetaFileSave *>(validation_layer->settings.Get("log_filename"));
    SettingDataFileSave *log_file_data = static_cast<SettingDataFileSave *>(parameter.settings.Get("log_filename"));

    // The debug action set of settings has it's own branch
    QTreeWidgetItem *debug_action_branch = new QTreeWidgetItem();
    debug_action_branch->setText(0, debug_action_meta->label.c_str());
    parent->addChild(debug_action_branch);

    // Each debug action has it's own checkbox
    for (std::size_t i = 0, n = debug_action_meta->enum_values.size(); i < n; ++i) {
        // Debug output is only for Windows
        if (VKC_PLATFORM != VKC_PLATFORM_WINDOWS && debug_action_meta->enum_values[i].key == "VK_DBG_LAYER_ACTION_DEBUG_OUTPUT")
            continue;

        QTreeWidgetItem *child = new QTreeWidgetItem();
        WidgetSettingFlags *this_control =
            new WidgetSettingFlags(*debug_action_meta, *debug_action_data, debug_action_meta->enum_values[i].key);
        this_control->setText(debug_action_meta->enum_values[i].label.c_str());
        this_control->setToolTip(debug_action_meta->enum_values[i].description.c_str());
        debug_action_branch->addChild(child);
        _settings_tree->setItemWidget(child, 0, this_control);
        this_control->setFont(_settings_tree->font());
        connect(this_control, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

        // The log message action also has a child; the log file selection setting/widget
        // Note, this is usually last, but I'll check for it any way in case other new items are added
        if (debug_action_meta->enum_values[i].key == "VK_DBG_LAYER_ACTION_LOG_MSG") {  // log action?
            _validation_debug_action = this_control;
            _validation_log_file_item = new QTreeWidgetItem();
            child->addChild(_validation_log_file_item);
            _validation_log_file_widget = new WidgetSettingFilesystem(_validation_log_file_item, *log_file_meta, *log_file_data);
            _validation_log_file_item->setSizeHint(0, QSize(0, 28));
            _settings_tree->setItemWidget(_validation_log_file_item, 0, _validation_log_file_widget);
            _compound_widgets.push_back(child);

            connect(_validation_log_file_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            connect(_validation_debug_action, SIGNAL(stateChanged(int)), this, SLOT(khronosDebugChanged(int)));

            // Capture initial state, which reflects enabled/disabled
            _validation_log_file_widget->setDisabled(!_validation_debug_action->isChecked());
        }
    }

    if (validation_layer->settings.Get("report_flags") != nullptr) {
        const SettingMetaFlags &setting_meta =
            static_cast<const SettingMetaFlags &>(*validation_layer->settings.Get("report_flags"));
        SettingDataFlags &setting_data =
            static_cast<SettingDataFlags &>(*parameter.settings.Create(setting_meta.GetKey(), setting_meta.GetType()));

        QTreeWidgetItem *sub_category = new QTreeWidgetItem;
        sub_category->setText(0, setting_meta.label.c_str());
        sub_category->setToolTip(0, setting_meta.description.c_str());
        parent->addChild(sub_category);

        for (std::size_t i = 0, n = setting_meta.enum_values.size(); i < n; ++i) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            WidgetSettingFlags *control =
                new WidgetSettingFlags(setting_meta, setting_data, setting_meta.enum_values[i].key.c_str());
            control->setText(setting_meta.enum_values[i].label.c_str());
            control->setToolTip(setting_meta.enum_values[i].description.c_str());
            sub_category->addChild(child);
            _settings_tree->setItemWidget(child, 0, control);
            control->setFont(_settings_tree->font());
            connect(control, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        }
    }

    if (validation_layer->settings.Get("duplicate_message_limit") != nullptr) {
        const SettingMetaInt &setting_meta =
            static_cast<const SettingMetaInt &>(*validation_layer->settings.Get("duplicate_message_limit"));
        SettingDataInt &setting_data =
            static_cast<SettingDataInt &>(*parameter.settings.Create(setting_meta.GetKey(), setting_meta.GetType()));

        QTreeWidgetItem *setting_item = new QTreeWidgetItem();
        WidgetSettingInt *widget = new WidgetSettingInt(setting_item, setting_meta, setting_data);
        widget->setToolTip(setting_meta.description.c_str());
        parent->addChild(setting_item);
        QTreeWidgetItem *place_holder = new QTreeWidgetItem();
        setting_item->addChild(place_holder);
        _settings_tree->setItemWidget(place_holder, 0, widget);
        connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
    }

    if (validation_layer->settings.Get("message_id_filter") != nullptr) {
        const SettingMetaVUIDFilter &setting_meta =
            static_cast<const SettingMetaVUIDFilter &>(*validation_layer->settings.Get("message_id_filter"));
        SettingDataVUIDFilter &setting_data =
            static_cast<SettingDataVUIDFilter &>(*parameter.settings.Create(setting_meta.GetKey(), setting_meta.GetType()));

        QTreeWidgetItem *mute_message_item = new QTreeWidgetItem;
        mute_message_item->setText(0, setting_meta.label.c_str());
        mute_message_item->setToolTip(0, setting_meta.description.c_str());
        parent->addChild(mute_message_item);

        WidgetSettingVUIDSearch *vuid_search_widget = new WidgetSettingVUIDSearch(setting_meta.list, setting_data.value);

        QTreeWidgetItem *next_line = new QTreeWidgetItem();
        next_line->setSizeHint(0, QSize(0, 28));
        mute_message_item->addChild(next_line);
        _settings_tree->setItemWidget(next_line, 0, vuid_search_widget);
        _compound_widgets.push_back(next_line);

        QTreeWidgetItem *list_item = new QTreeWidgetItem();
        mute_message_item->addChild(list_item);
        list_item->setSizeHint(0, QSize(0, 200));
        WidgetSettingVUIDFilter *mute_message_widget = new WidgetSettingVUIDFilter(setting_meta, setting_data);
        _compound_widgets.push_back(list_item);
        _settings_tree->setItemWidget(list_item, 0, mute_message_widget);

        connect(vuid_search_widget, SIGNAL(itemSelected(const QString &)), mute_message_widget, SLOT(addItem(const QString &)));
        connect(vuid_search_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        connect(mute_message_widget, SIGNAL(itemRemoved(const QString &)), vuid_search_widget,
                SLOT(addToSearchList(const QString &)));
        connect(mute_message_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()), Qt::QueuedConnection);
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

    const SettingMetaSet &layer_setting_metas = FindByKey(available_layers, parameter.key.c_str())->settings;

    for (std::size_t setting_index = 0, n = layer_setting_metas.data.size(); setting_index < n; ++setting_index) {
        const SettingMeta &setting_meta = *layer_setting_metas.data[setting_index];
        if (!(setting_meta.platform_flags & (1 << VKC_PLATFORM))) continue;

        SettingData *setting_data = parameter.settings.Get(setting_meta.GetKey());
        assert(setting_data);

        QTreeWidgetItem *setting_item = new QTreeWidgetItem();
        parent->addChild(setting_item);

        switch (setting_meta.GetType()) {
            case SETTING_BOOL:
            case SETTING_BOOL_NUMERIC_DEPRECATED: {
                const SettingMetaBool &setting_meta_src = static_cast<const SettingMetaBool &>(setting_meta);
                SettingDataBool &setting_data_src = static_cast<SettingDataBool &>(*setting_data);

                WidgetSettingBool *widget = new WidgetSettingBool(setting_meta_src, setting_data_src);
                _settings_tree->setItemWidget(setting_item, 0, widget);
                widget->setFont(_settings_tree->font());
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_INT: {
                const SettingMetaInt &setting_meta_src = static_cast<const SettingMetaInt &>(setting_meta);
                SettingDataInt &setting_data_src = static_cast<SettingDataInt &>(*setting_data);

                WidgetSettingInt *widget = new WidgetSettingInt(setting_item, setting_meta_src, setting_data_src);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                setting_item->addChild(place_holder);
                _settings_tree->setItemWidget(place_holder, 0, widget);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_SAVE_FILE:
            case SETTING_LOAD_FILE:
            case SETTING_SAVE_FOLDER: {
                const SettingMetaFilesystem &setting_meta_src = static_cast<const SettingMetaFilesystem &>(setting_meta);
                SettingDataFilesystem &setting_data_src = static_cast<SettingDataFilesystem &>(*setting_data);

                WidgetSettingFilesystem *widget = new WidgetSettingFilesystem(setting_item, setting_meta_src, setting_data_src);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                place_holder->setSizeHint(0, QSize(0, 28));
                setting_item->addChild(place_holder);
                _settings_tree->setItemWidget(place_holder, 0, widget);
                _compound_widgets.push_back(place_holder);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_ENUM: {
                const SettingMetaEnum &setting_meta_src = static_cast<const SettingMetaEnum &>(setting_meta);
                SettingDataEnum &setting_data_src = static_cast<SettingDataEnum &>(*setting_data);

                setting_item->setText(0, setting_meta.label.c_str());
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                setting_item->addChild(place_holder);

                WidgetSettingEnum *enum_widget = new WidgetSettingEnum(setting_item, setting_meta_src, setting_data_src);
                _settings_tree->setItemWidget(place_holder, 0, enum_widget);
                connect(enum_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_INT_RANGE: {
                const SettingMetaIntRange &setting_meta_src = static_cast<const SettingMetaIntRange &>(setting_meta);
                SettingDataIntRange &setting_data_src = static_cast<SettingDataIntRange &>(*setting_data);

                WidgetSettingIntRange *widget = new WidgetSettingIntRange(setting_item, setting_meta_src, setting_data_src);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                setting_item->addChild(place_holder);
                _settings_tree->setItemWidget(place_holder, 0, widget);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_STRING: {
                const SettingMetaString &setting_meta_src = static_cast<const SettingMetaString &>(setting_meta);
                SettingDataString &setting_data_src = static_cast<SettingDataString &>(*setting_data);

                WidgetSettingString *widget = new WidgetSettingString(setting_item, setting_meta_src, setting_data_src);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                setting_item->addChild(place_holder);
                _settings_tree->setItemWidget(place_holder, 0, widget);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            default: {
                setting_item->setText(0, "Unknown setting");
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
    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);
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
    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);
}
