/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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
#include "widget_setting_list.h"
#include "widget_setting_search.h"

#include "../vkconfig_core/version.h"
#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/util.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QApplication>

#include <cassert>

static const int ITEM_HEIGHT = 24;

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

    _settings_tree->blockSignals(true);
    _settings_tree->clear();

    QFont font_layer = _settings_tree->font();
    font_layer.setBold(true);

    QFont font_section = _settings_tree->font();
    font_section.setItalic(true);

    if (!configuration->HasOverride()) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, "No overridden or excluded layer");
        item->setFont(0, font_section);
        _settings_tree->addTopLevelItem(item);
    } else {
        const std::size_t overridden_layer_count = CountOverriddenLayers(configuration->parameters);

        if (overridden_layer_count > 1) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, "Vulkan Applications");
            item->setTextAlignment(0, Qt::AlignCenter);
            item->setFont(0, font_section);
            item->setDisabled(true);
            _settings_tree->addTopLevelItem(item);
        }

        // There will be one top level item for each layer
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            Parameter &parameter = configuration->parameters[i];
            if (!IsPlatformSupported(parameter.platform_flags)) continue;

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
            layer_item->setFont(0, font_layer);
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

        if (overridden_layer_count > 1) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, "Vulkan Drivers");
            item->setTextAlignment(0, Qt::AlignCenter);
            item->setFont(0, font_section);
            item->setDisabled(true);
            _settings_tree->addTopLevelItem(item);
        }

        const std::size_t excluded_layer_count =
            CountExcludedLayers(configuration->parameters, configurator.layers.available_layers);

        if (excluded_layer_count > 0) {
            // The last item is just the excluded layers
            QTreeWidgetItem *excluded_layers = new QTreeWidgetItem();
            excluded_layers->setText(0, "Excluded Layers:");
            excluded_layers->setFont(0, font_section);
            _settings_tree->addTopLevelItem(excluded_layers);

            for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
                Parameter &parameter = configuration->parameters[i];
                if (!IsPlatformSupported(parameter.platform_flags)) continue;

                if (parameter.state != LAYER_STATE_EXCLUDED) continue;

                const Layer *layer = FindByKey(configurator.layers.available_layers, parameter.key.c_str());
                if (layer == nullptr) continue;  // Do not display missing excluded layers

                QTreeWidgetItem *layer_item = new QTreeWidgetItem();
                layer_item->setText(0, parameter.key.c_str());
                layer_item->setFont(0, font_layer);
                layer_item->setToolTip(0, layer->description.c_str());
                excluded_layers->addChild(layer_item);
            }

            // None excluded layer were found
            if (excluded_layers->childCount() == 0) {
                QTreeWidgetItem *child = new QTreeWidgetItem();
                child->setText(0, "None");
                excluded_layers->addChild(child);
            }
        }
    }

    // Everyone is expanded.
    _settings_tree->resizeColumnToContents(0);
    SetTreeState(configuration->setting_tree_state, 0, _settings_tree->invisibleRootItem());
    _settings_tree->blockSignals(false);
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
    validation_areas_item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    parent->addChild(validation_areas_item);

    // This just finds the enables and disables
    _validation_areas = new SettingsValidationAreas(_settings_tree, validation_areas_item, validation_layer->_api_version,
                                                    validation_layer->settings, parameter.settings);

    // Get the Debug Action and log file settings (and they must exist)
    const SettingMetaFlags *meta_debug = validation_layer->settings.Get<SettingMetaFlags>("debug_action");
    assert(meta_debug != nullptr);
    SettingDataFlags *data_debug = parameter.settings.Get<SettingDataFlags>("debug_action");
    assert(data_debug != nullptr);

    const SettingMetaFileSave *meta_log_file = validation_layer->settings.Get<SettingMetaFileSave>("log_filename");
    assert(meta_log_file != nullptr);
    SettingDataFileSave *data_log_file = parameter.settings.Get<SettingDataFileSave>("log_filename");
    assert(data_log_file != nullptr);

    // The debug action set of settings has it's own branch
    QTreeWidgetItem *debug_action_branch = new QTreeWidgetItem();
    debug_action_branch->setText(0, meta_debug->label.c_str());
    parent->addChild(debug_action_branch);

    // Each debug action has it's own checkbox
    for (std::size_t i = 0, n = meta_debug->enum_values.size(); i < n; ++i) {
        if (!IsPlatformSupported(meta_debug->enum_values[i].platform_flags)) continue;

        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setSizeHint(0, QSize(0, ITEM_HEIGHT));
        WidgetSettingFlag *widget = new WidgetSettingFlag(*meta_debug, *data_debug, meta_debug->enum_values[i].key);
        debug_action_branch->addChild(child);
        _settings_tree->setItemWidget(child, 0, widget);
        widget->setFont(_settings_tree->font());
        connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

        // The log message action also has a child; the log file selection setting/widget
        // Note, this is usually last, but I'll check for it any way in case other new items are added
        if (meta_debug->enum_values[i].key == "VK_DBG_LAYER_ACTION_LOG_MSG") {  // log action?
            _validation_debug_action = widget;
            _validation_log_file_item = new QTreeWidgetItem();
            child->addChild(_validation_log_file_item);
            _validation_log_file_widget = new WidgetSettingFilesystem(_validation_log_file_item, *meta_log_file, *data_log_file);
            _validation_log_file_item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
            _settings_tree->setItemWidget(_validation_log_file_item, 0, _validation_log_file_widget);

            connect(_validation_log_file_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            connect(_validation_debug_action, SIGNAL(stateChanged(int)), this, SLOT(khronosDebugChanged(int)));

            // Capture initial state, which reflects enabled/disabled
            _validation_log_file_widget->setDisabled(!_validation_debug_action->isChecked());
        }
    }

    const SettingMetaFlags *meta_report = validation_layer->settings.Get<SettingMetaFlags>("report_flags");
    if (meta_report != nullptr) {
        const SettingMetaFlags &meta = *meta_report;
        SettingDataFlags &data = *parameter.settings.Get<SettingDataFlags>(meta.key.c_str());

        QTreeWidgetItem *sub_category = new QTreeWidgetItem;
        sub_category->setText(0, meta.label.c_str());
        sub_category->setToolTip(0, meta.description.c_str());
        sub_category->setSizeHint(0, QSize(0, ITEM_HEIGHT));
        parent->addChild(sub_category);

        for (std::size_t i = 0, n = meta.enum_values.size(); i < n; ++i) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setSizeHint(0, QSize(0, ITEM_HEIGHT));
            WidgetSettingFlag *widget = new WidgetSettingFlag(meta, data, meta.enum_values[i].key.c_str());
            sub_category->addChild(child);
            _settings_tree->setItemWidget(child, 0, widget);
            widget->setFont(_settings_tree->font());
            connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        }
    }

    const SettingMetaInt *meta_duplicate = validation_layer->settings.Get<SettingMetaInt>("duplicate_message_limit");
    if (meta_duplicate != nullptr) {
        const SettingMetaInt &meta = *meta_duplicate;
        SettingDataInt &data = *parameter.settings.Get<SettingDataInt>(meta.key.c_str());

        QTreeWidgetItem *setting_item = new QTreeWidgetItem();
        parent->addChild(setting_item);
        WidgetSettingInt *widget = new WidgetSettingInt(_settings_tree, setting_item, meta, data);
        connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
    }

    const SettingMetaList *meta_filter = validation_layer->settings.Get<SettingMetaList>("message_id_filter");
    if (meta_filter != nullptr) {
        const SettingMetaList &meta = *meta_filter;
        static_cast<const SettingMetaList &>(*validation_layer->settings.Get("message_id_filter"));
        SettingDataList &data = *parameter.settings.Get<SettingDataList>(meta.key.c_str());

        QTreeWidgetItem *mute_message_item = new QTreeWidgetItem;
        mute_message_item->setText(0, meta.label.c_str());
        mute_message_item->setToolTip(0, meta.description.c_str());
        parent->addChild(mute_message_item);

        WidgetSettingSearch *widget_search = new WidgetSettingSearch(meta.list, data.value);

        QTreeWidgetItem *next_line = new QTreeWidgetItem();
        next_line->setSizeHint(0, QSize(0, ITEM_HEIGHT));
        mute_message_item->addChild(next_line);
        _settings_tree->setItemWidget(next_line, 0, widget_search);

        QTreeWidgetItem *list_item = new QTreeWidgetItem();
        mute_message_item->addChild(list_item);
        list_item->setSizeHint(0, QSize(0, 200));
        WidgetSettingList *widget_list = new WidgetSettingList(meta, data);
        _settings_tree->setItemWidget(list_item, 0, widget_list);

        connect(widget_search, SIGNAL(itemSelected(const QString &)), widget_list, SLOT(addItem(const QString &)));
        connect(widget_search, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        connect(widget_list, SIGNAL(itemRemoved(const QString &)), widget_search, SLOT(addToSearchList(const QString &)));
        connect(widget_list, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()), Qt::QueuedConnection);
    }

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

    for (std::size_t setting_index = 0, n = layer_setting_metas.Size(); setting_index < n; ++setting_index) {
        if (!IsPlatformSupported(layer_setting_metas[setting_index].platform_flags)) continue;

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
        parent->addChild(item);

        switch (layer_setting_metas[setting_index].type) {
            case SETTING_BOOL:
            case SETTING_BOOL_NUMERIC_DEPRECATED: {
                const SettingMetaBool &meta = static_cast<const SettingMetaBool &>(layer_setting_metas[setting_index]);
                SettingDataBool &data = *parameter.settings.Get<SettingDataBool>(meta.key.c_str());

                WidgetSettingBool *widget = new WidgetSettingBool(_settings_tree, item, meta, data);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_INT: {
                const SettingMetaInt &meta = static_cast<const SettingMetaInt &>(layer_setting_metas[setting_index]);
                SettingDataInt &data = *parameter.settings.Get<SettingDataInt>(meta.key.c_str());

                WidgetSettingInt *widget = new WidgetSettingInt(_settings_tree, item, meta, data);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_SAVE_FILE:
            case SETTING_LOAD_FILE:
            case SETTING_SAVE_FOLDER: {
                const SettingMetaFilesystem &meta = static_cast<const SettingMetaFilesystem &>(layer_setting_metas[setting_index]);
                SettingDataString &data = *parameter.settings.Get<SettingDataString>(meta.key.c_str());

                WidgetSettingFilesystem *widget = new WidgetSettingFilesystem(item, meta, data);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                place_holder->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                item->addChild(place_holder);
                _settings_tree->setItemWidget(place_holder, 0, widget);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_ENUM: {
                const SettingMetaEnum &meta = static_cast<const SettingMetaEnum &>(layer_setting_metas[setting_index]);
                SettingDataEnum &data = *parameter.settings.Get<SettingDataEnum>(meta.key.c_str());

                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                item->setText(0, meta.label.c_str());
                item->setToolTip(0, meta.description.c_str());
                item->addChild(place_holder);

                WidgetSettingEnum *enum_widget = new WidgetSettingEnum(item, meta, data);
                _settings_tree->setItemWidget(place_holder, 0, enum_widget);
                connect(enum_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_FLAGS: {
                const SettingMetaFlags &meta = static_cast<const SettingMetaFlags &>(layer_setting_metas[setting_index]);
                SettingDataFlags &data = *parameter.settings.Get<SettingDataFlags>(meta.key.c_str());

                item->setText(0, meta.label.c_str());
                item->setToolTip(0, meta.description.c_str());

                for (std::size_t i = 0, n = meta.enum_values.size(); i < n; ++i) {
                    WidgetSettingFlag *widget = new WidgetSettingFlag(meta, data, meta.enum_values[i].key.c_str());
                    QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                    item->addChild(place_holder);
                    _settings_tree->setItemWidget(place_holder, 0, widget);
                    widget->setFont(_settings_tree->font());
                    connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
                }
            } break;

            case SETTING_INT_RANGES: {
                const SettingMetaIntRanges &meta = static_cast<const SettingMetaIntRanges &>(layer_setting_metas[setting_index]);
                SettingDataIntRanges &data = *parameter.settings.Get<SettingDataIntRanges>(meta.key.c_str());

                WidgetSettingIntRange *widget = new WidgetSettingIntRange(item, meta, data);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                item->addChild(place_holder);
                _settings_tree->setItemWidget(place_holder, 0, widget);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_STRING: {
                const SettingMetaString &meta = static_cast<const SettingMetaString &>(layer_setting_metas[setting_index]);
                SettingDataString &data = *parameter.settings.Get<SettingDataString>(meta.key.c_str());

                WidgetSettingString *widget = new WidgetSettingString(item, meta, data);
                QTreeWidgetItem *place_holder = new QTreeWidgetItem();
                item->addChild(place_holder);
                _settings_tree->setItemWidget(place_holder, 0, widget);
                connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            } break;

            case SETTING_LIST: {
                const SettingMetaList &meta = static_cast<const SettingMetaList &>(layer_setting_metas[setting_index]);
                SettingDataList &data = *parameter.settings.Get<SettingDataList>(meta.key.c_str());

                QTreeWidgetItem *mute_message_item = new QTreeWidgetItem;
                mute_message_item->setText(0, meta.label.c_str());
                mute_message_item->setToolTip(0, meta.description.c_str());
                parent->addChild(mute_message_item);

                WidgetSettingSearch *widget_search = new WidgetSettingSearch(meta.list, data.value);

                QTreeWidgetItem *next_line = new QTreeWidgetItem();
                next_line->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                mute_message_item->addChild(next_line);
                _settings_tree->setItemWidget(next_line, 0, widget_search);

                QTreeWidgetItem *list_item = new QTreeWidgetItem();
                mute_message_item->addChild(list_item);
                list_item->setSizeHint(0, QSize(0, 200));
                WidgetSettingList *widget_list = new WidgetSettingList(meta, data);
                _settings_tree->setItemWidget(list_item, 0, widget_list);

                connect(widget_search, SIGNAL(itemSelected(const QString &)), widget_list, SLOT(addItem(const QString &)));
                connect(widget_search, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
                connect(widget_list, SIGNAL(itemRemoved(const QString &)), widget_search, SLOT(addToSearchList(const QString &)));
                connect(widget_list, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()), Qt::QueuedConnection);
            } break;

            default: {
                item->setText(0, "Unknown setting");
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
