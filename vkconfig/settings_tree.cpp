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

#include "widget_setting.h"
#include "widget_setting_int.h"
#include "widget_setting_float.h"
#include "widget_setting_frames.h"
#include "widget_setting_bool.h"
#include "widget_setting_enum.h"
#include "widget_setting_string.h"
#include "widget_setting_flags.h"
#include "widget_setting_filesystem.h"
#include "widget_setting_list_element.h"
#include "widget_setting_list.h"

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
    : tree(nullptr), _validation_log_file_item(nullptr), _validation_log_file_widget(nullptr) {}

void SettingsTreeManager::CreateGUI(QTreeWidget *build_tree) {
    assert(build_tree);

    // Do this first to make absolutely sure if these is an old configuration still active it's state gets saved.
    CleanupGUI();

    Configurator &configurator = Configurator::Get();

    this->tree = build_tree;
    Configuration *configuration = configurator.configurations.GetActiveConfiguration();
    assert(configuration != nullptr);

    this->tree->blockSignals(true);
    this->tree->clear();

    QFont font_layer = this->tree->font();
    font_layer.setBold(true);

    QFont font_section = this->tree->font();
    font_section.setItalic(true);

    if (!configuration->HasOverride()) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, "No overridden or excluded layer");
        item->setFont(0, font_section);
        this->tree->addTopLevelItem(item);
    } else {
        const std::size_t overridden_layer_count = CountOverriddenLayers(configuration->parameters);

        if (overridden_layer_count > 1) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, "Vulkan Applications");
            item->setTextAlignment(0, Qt::AlignCenter);
            item->setFont(0, font_section);
            item->setDisabled(true);
            this->tree->addTopLevelItem(item);
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
            this->tree->addTopLevelItem(layer_item);

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
                this->tree->setItemWidget(presets_item, 0, presets_combobox);
                presets.push_back(presets_combobox);
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
            this->tree->addTopLevelItem(item);
        }

        const std::size_t excluded_layer_count =
            CountExcludedLayers(configuration->parameters, configurator.layers.available_layers);

        if (excluded_layer_count > 0) {
            // The last item is just the excluded layers
            QTreeWidgetItem *excluded_layers = new QTreeWidgetItem();
            excluded_layers->setText(0, "Excluded Layers:");
            excluded_layers->setFont(0, font_section);
            this->tree->addTopLevelItem(excluded_layers);

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
    this->tree->resizeColumnToContents(0);
    SetTreeState(configuration->setting_tree_state, 0, this->tree->invisibleRootItem());
    this->tree->blockSignals(false);
}

void SettingsTreeManager::CleanupGUI() {
    if (this->tree == nullptr)  // Was not initialized
        return;

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.configurations.GetActiveConfiguration();
    if (configuration == nullptr) return;

    configuration->setting_tree_state.clear();
    GetTreeState(configuration->setting_tree_state, this->tree->invisibleRootItem());

    this->validation.reset();

    presets.clear();

    this->tree->clear();
    this->tree = nullptr;
    _validation_debug_action = nullptr;
    _validation_log_file_widget = nullptr;
    _validation_log_file_item = nullptr;
}

static bool IsBuiltinValidation(const std::string &key) {
    static const char *VALIDATION_KEYS[] = {"disables",         "enables",        "gpuav_buffer_oob",
                                            "printf_to_stdout", "printf_verbose", "printf_buffer_size",
                                            "debug_action",     "log_filename"};

    for (std::size_t i = 0, n = countof(VALIDATION_KEYS); i < n; ++i) {
        if (key == VALIDATION_KEYS[i]) return true;
    }

    return false;
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
    this->validation.reset(new SettingsValidationAreas(this->tree, validation_areas_item, validation_layer->_api_version,
                                                       validation_layer->settings, parameter.settings));
    this->connect(this->validation.get(), SIGNAL(settingChanged()), this, SLOT(OnSettingChanged()));

    // Debug area
    const SettingMetaFlags *meta_debug = validation_layer->settings.Get<SettingMetaFlags>("debug_action");
    if (meta_debug != nullptr) {
        // The debug action set of settings has it's own branch
        QTreeWidgetItem *debug_action_branch = new QTreeWidgetItem();
        debug_action_branch->setText(0, meta_debug->label.c_str());
        parent->addChild(debug_action_branch);

        SettingDataFlags *data_debug = parameter.settings.Get<SettingDataFlags>("debug_action");

        // Each debug action has it's own checkbox
        for (std::size_t i = 0, n = meta_debug->enum_values.size(); i < n; ++i) {
            if (!IsPlatformSupported(meta_debug->enum_values[i].platform_flags)) continue;
            if (meta_debug->enum_values[i].view == SETTING_VIEW_HIDDEN) continue;

            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setSizeHint(0, QSize(0, ITEM_HEIGHT));
            WidgetSettingFlag *widget = new WidgetSettingFlag(tree, *meta_debug, *data_debug, meta_debug->enum_values[i].key);
            debug_action_branch->addChild(child);
            tree->setItemWidget(child, 0, widget);
            connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

            // The log message action also has a child; the log file selection setting/widget
            // Note, this is usually last, but I'll check for it any way in case other new items are added
            if (meta_debug->enum_values[i].key == "VK_DBG_LAYER_ACTION_LOG_MSG") {  // log action?
                const SettingMetaFileSave *meta_log_file = validation_layer->settings.Get<SettingMetaFileSave>("log_filename");
                SettingDataFileSave *data_log_file = parameter.settings.Get<SettingDataFileSave>("log_filename");

                _validation_debug_action = widget;
                _validation_log_file_item = new QTreeWidgetItem();
                child->addChild(_validation_log_file_item);
                _validation_log_file_widget =
                    new WidgetSettingFilesystem(tree, _validation_log_file_item, *meta_log_file, *data_log_file);
                _validation_log_file_item->setSizeHint(0, QSize(0, ITEM_HEIGHT));

                connect(_validation_log_file_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
                connect(_validation_debug_action, SIGNAL(stateChanged(int)), this, SLOT(OnDebugChanged(int)));

                // Capture initial state, which reflects enabled/disabled
                _validation_log_file_widget->setDisabled(!_validation_debug_action->isChecked());
            }
        }
    }

    const SettingMetaSet &layer_setting_metas = FindByKey(available_layers, parameter.key.c_str())->settings;
    for (std::size_t setting_index = 0, n = layer_setting_metas.Size(); setting_index < n; ++setting_index) {
        const SettingMeta &setting_meta = layer_setting_metas[setting_index];
        if (IsBuiltinValidation(setting_meta.key)) continue;

        this->BuildTreeItem(parent, layer_setting_metas, parameter.settings, setting_meta);
    }
}

void SettingsTreeManager::OnDebugChanged(int index) {
    (void)index;
    bool enabled = !(_validation_debug_action->isChecked());
    this->tree->blockSignals(true);
    this->_validation_log_file_item->setDisabled(enabled);
    this->_validation_log_file_widget->setDisabled(enabled);
    this->tree->blockSignals(false);
    this->OnSettingChanged();
}

void SettingsTreeManager::BuildTreeItem(QTreeWidgetItem *parent, const SettingMetaSet &meta_set, SettingDataSet &data_set,
                                        const SettingMeta &meta_object) {
    if (!IsPlatformSupported(meta_object.platform_flags)) return;
    if (meta_object.view == SETTING_VIEW_HIDDEN) return;

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    parent->addChild(item);

    switch (meta_object.type) {
        case SETTING_BOOL:
        case SETTING_BOOL_NUMERIC_DEPRECATED: {
            const SettingMetaBool &meta = static_cast<const SettingMetaBool &>(meta_object);
            SettingDataBool &data = *data_set.Get<SettingDataBool>(meta.key.c_str());

            WidgetSettingBool *widget = new WidgetSettingBool(tree, item, meta, data);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_INT: {
            const SettingMetaInt &meta = static_cast<const SettingMetaInt &>(meta_object);
            SettingDataInt &data = *data_set.Get<SettingDataInt>(meta.key.c_str());

            WidgetSettingInt *widget = new WidgetSettingInt(tree, item, meta, data);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FLOAT: {
            const SettingMetaFloat &meta = static_cast<const SettingMetaFloat &>(meta_object);
            SettingDataFloat &data = *data_set.Get<SettingDataFloat>(meta.key.c_str());

            WidgetSettingFloat *widget = new WidgetSettingFloat(tree, item, meta, data);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FRAMES: {
            const SettingMetaFrames &meta = static_cast<const SettingMetaFrames &>(meta_object);
            SettingDataFrames &data = *data_set.Get<SettingDataFrames>(meta.key.c_str());

            WidgetSettingFrames *widget = new WidgetSettingFrames(tree, item, meta, data);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_SAVE_FILE:
        case SETTING_LOAD_FILE:
        case SETTING_SAVE_FOLDER: {
            const SettingMetaFilesystem &meta = static_cast<const SettingMetaFilesystem &>(meta_object);
            SettingDataString &data = *data_set.Get<SettingDataString>(meta.key.c_str());

            WidgetSettingFilesystem *widget = new WidgetSettingFilesystem(tree, item, meta, data);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_ENUM: {
            const SettingMetaEnum &meta = static_cast<const SettingMetaEnum &>(meta_object);
            SettingDataEnum &data = *data_set.Get<SettingDataEnum>(meta.key.c_str());

            WidgetSettingEnum *enum_widget = new WidgetSettingEnum(tree, item, meta, data);
            this->connect(enum_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FLAGS: {
            const SettingMetaFlags &meta = static_cast<const SettingMetaFlags &>(meta_object);
            SettingDataFlags &data = *data_set.Get<SettingDataFlags>(meta.key.c_str());

            item->setText(0, meta.label.c_str());
            item->setToolTip(0, meta.description.c_str());

            for (std::size_t i = 0, n = meta.enum_values.size(); i < n; ++i) {
                if (!IsPlatformSupported(meta.enum_values[i].platform_flags)) continue;
                if (meta.enum_values[i].view == SETTING_VIEW_HIDDEN) continue;

                WidgetSettingFlag *widget = new WidgetSettingFlag(tree, meta, data, meta.enum_values[i].key.c_str());
                QTreeWidgetItem *child = new QTreeWidgetItem();
                item->addChild(child);

                this->tree->setItemWidget(child, 0, widget);
                this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
            }
        } break;

        case SETTING_STRING: {
            const SettingMetaString &meta = static_cast<const SettingMetaString &>(meta_object);
            SettingDataString &data = *data_set.Get<SettingDataString>(meta.key.c_str());

            WidgetSettingString *widget = new WidgetSettingString(tree, item, meta, data);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_LIST: {
            const SettingMetaList &meta = static_cast<const SettingMetaList &>(meta_object);
            SettingDataList &data = *data_set.Get<SettingDataList>(meta.key.c_str());

            WidgetSettingList *widget = new WidgetSettingList(tree, item, meta, data);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        default: {
            item->setText(0, "Unknown setting");
            assert(0);  // Unknown setting
        } break;
    }

    for (std::size_t i = 0, n = meta_object.children.Size(); i < n; ++i) {
        this->BuildTreeItem(item, meta_object.children, data_set, meta_object.children[i]);
    }
}

void SettingsTreeManager::BuildGenericTree(QTreeWidgetItem *parent, Parameter &parameter) {
    std::vector<Layer> &available_layers = Configurator::Get().layers.available_layers;

    const SettingMetaSet &layer_setting_metas = FindByKey(available_layers, parameter.key.c_str())->settings;

    for (std::size_t setting_index = 0, n = layer_setting_metas.Size(); setting_index < n; ++setting_index) {
        this->BuildTreeItem(parent, layer_setting_metas, parameter.settings, layer_setting_metas[setting_index]);
    }
}

void SettingsTreeManager::OnPresetChanged(int combox_preset_index) {
    (void)combox_preset_index;

    CreateGUI(tree);

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
    for (std::size_t i = 0, n = presets.size(); i < n; ++i) {
        presets[i]->UpdateCurrentIndex();
    }

    Configurator &configurator = Configurator::Get();
    configurator.environment.Notify(NOTIFICATION_RESTART);
    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);
}
