/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/version.h"
#include "../vkconfig_core/util.h"
#include "../vkconfig_core/type_platform.h"
#include "../vkconfig_core/type_hide_message.h"

#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QApplication>

#include <cassert>

static const char *TOOLTIP_ORDER =
    "Layers are executed between the Vulkan application and driver in the specific order represented here";

SettingsTreeManager::SettingsTreeManager() : tree(nullptr), parameter(nullptr) {}

void SettingsTreeManager::CreateGUI(std::shared_ptr<Ui::MainWindow> ui) {
    assert(ui.get() != nullptr);

    // Do this first to make absolutely sure if these is an old configuration still active it's state gets saved.
    this->CleanupGUI();

    Configurator &configurator = Configurator::Get();

    this->tree = ui->configurations_settings;

    Configuration *configuration = configurator.GetActiveConfiguration();
    this->parameter = configuration != nullptr ? configuration->GetActiveParameter() : nullptr;
    const bool no_selected_layer = configuration != nullptr ? configuration->selected_layer_name.empty() : false;

    // Group box things
    const Layer *layer = this->parameter ? configurator.layers.Find(parameter->key, parameter->api_version) : nullptr;

    if (no_selected_layer) {
        ui->configurations_group_box_settings->setTitle("Select a layer to display settings");
        ui->configurations_group_box_settings->setCheckable(false);
        ui->configurations_presets->setVisible(false);
        return;
    } else if (!configurator.HasActiveSettings()) {
        ui->configurations_group_box_settings->setTitle("No Layer Settings");
        ui->configurations_group_box_settings->setCheckable(false);
        ui->configurations_presets->setVisible(false);
        return;
    } else {
        ui->configurations_group_box_settings->setTitle(format("%s:", parameter->key.c_str()).c_str());
        ui->configurations_group_box_settings->setCheckable(configurator.advanced);
        ui->configurations_group_box_settings->setChecked(parameter->override_settings);
        ui->configurations_presets->setVisible(!layer->presets.empty());
    }

    // Check layer version and switch if that doesn't match
    if (layer->api_version != this->parameter->api_version) {
        configuration->SwitchLayerVersion(configurator.layers, layer->key, layer->api_version);
    }

    // preset combobox
    {
        ui->configurations_presets->blockSignals(true);
        ui->configurations_presets->clear();
        preset_labels.clear();
        if (!layer->presets.empty()) {
            ui->configurations_presets->addItem(Layer::NO_PRESET);
            preset_labels.push_back(Layer::NO_PRESET);

            for (std::size_t i = 0, n = layer->presets.size(); i < n; ++i) {
                const LayerPreset &layer_preset = layer->presets[i];

                if (!IsPlatformSupported(layer_preset.platform_flags)) {
                    continue;
                }
                if (layer_preset.view == SETTING_VIEW_HIDDEN) {
                    continue;
                }

                ui->configurations_presets->addItem((layer_preset.label + " Preset").c_str());
                preset_labels.push_back(layer_preset.label);
            }

            this->connect(ui->configurations_presets, SIGNAL(currentIndexChanged(int)), this, SLOT(OnPresetChanged(int)));
            ui->configurations_presets->setVisible(true);
        } else {
            ui->configurations_presets->setVisible(false);
        }
        ui->configurations_presets->blockSignals(false);
    }

    // settings tree
    {
        this->tree->blockSignals(true);
        this->tree->clear();

        this->BuildTree();

        this->connect(this->tree, SIGNAL(expanded(const QModelIndex)), this, SLOT(OnExpandedChanged(const QModelIndex)));
        this->connect(this->tree, SIGNAL(collapsed(const QModelIndex)), this, SLOT(OnExpandedChanged(const QModelIndex)));

        this->tree->resizeColumnToContents(0);

        this->tree->blockSignals(false);
    }
}

void SettingsTreeManager::CleanupGUI() {
    // Was not initialized
    if (this->tree == nullptr) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        Parameter *parameter = configuration->GetActiveParameter();
        if (parameter != nullptr) {
            parameter->setting_tree_state.clear();
            this->GetTreeState(parameter->setting_tree_state, this->tree->invisibleRootItem());
        }
    }

    this->tree->clear();
    this->tree = nullptr;
}

void SettingsTreeManager::OnExpandedChanged(const QModelIndex &index) {
    (void)index;

    // Was not initialized
    if (this->tree == nullptr) {
        return;
    }

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            Parameter &parameter = configuration->parameters[i];

            if (parameter.key != configuration->selected_layer_name) {
                continue;
            }

            parameter.setting_tree_state.clear();
            GetTreeState(parameter.setting_tree_state, this->tree->invisibleRootItem());
        }
    }
}

void SettingsTreeManager::BuildTreeItem(QTreeWidgetItem *parent, const SettingMeta &meta_object) {
    if (!IsPlatformSupported(meta_object.platform_flags)) {
        return;
    }

    if (meta_object.view == SETTING_VIEW_HIDDEN) {
        return;
    }

    Configurator &configurator = Configurator::Get();

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    if (parent == nullptr) {
        this->tree->addTopLevelItem(item);
    } else {
        parent->addChild(item);
    }

    switch (meta_object.type) {
        case SETTING_GROUP: {
            item->setText(0, meta_object.label.c_str());
            item->setToolTip(0, meta_object.description.c_str());
            item->setFont(0, tree->font());
            item->setExpanded(meta_object.expanded);
        } break;
        case SETTING_BOOL:
        case SETTING_BOOL_NUMERIC_DEPRECATED: {
            const SettingMetaBool &meta = static_cast<const SettingMetaBool &>(meta_object);

            WidgetSettingBool *widget = new WidgetSettingBool(tree, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_INT: {
            const SettingMetaInt &meta = static_cast<const SettingMetaInt &>(meta_object);

            WidgetSettingInt *widget = new WidgetSettingInt(tree, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FLOAT: {
            const SettingMetaFloat &meta = static_cast<const SettingMetaFloat &>(meta_object);

            WidgetSettingFloat *widget = new WidgetSettingFloat(tree, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FRAMES: {
            const SettingMetaFrames &meta = static_cast<const SettingMetaFrames &>(meta_object);

            WidgetSettingFrames *widget = new WidgetSettingFrames(tree, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_SAVE_FILE:
        case SETTING_LOAD_FILE:
        case SETTING_SAVE_FOLDER:
        case SETTING_LOAD_FOLDER: {
            const SettingMetaFilesystem &meta = static_cast<const SettingMetaFilesystem &>(meta_object);

            WidgetSettingFilesystem *widget = new WidgetSettingFilesystem(tree, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_ENUM: {
            const SettingMetaEnum &meta = static_cast<const SettingMetaEnum &>(meta_object);

            WidgetSettingEnum *enum_widget = new WidgetSettingEnum(tree, item, meta, parameter->settings);
            this->connect(enum_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

            for (std::size_t i = 0, n = meta.enum_values.size(); i < n; ++i) {
                const SettingEnumValue &value = meta.enum_values[i];

                if (!IsPlatformSupported(value.platform_flags)) continue;
                if (value.view == SETTING_VIEW_HIDDEN) continue;

                for (std::size_t i = 0, n = value.settings.size(); i < n; ++i) {
                    this->BuildTreeItem(item, *value.settings[i]);
                }
            }
        } break;

        case SETTING_FLAGS: {
            const SettingMetaFlags &meta = static_cast<const SettingMetaFlags &>(meta_object);

            item->setText(0, meta.label.c_str());
            item->setToolTip(0, meta.description.c_str());
            item->setExpanded(meta.expanded);

            for (std::size_t i = 0, n = meta.enum_values.size(); i < n; ++i) {
                const SettingEnumValue &value = meta.enum_values[i];

                if (!IsPlatformSupported(value.platform_flags)) continue;
                if (value.view == SETTING_VIEW_HIDDEN) continue;

                QTreeWidgetItem *child = new QTreeWidgetItem();
                item->addChild(child);

                WidgetSettingFlag *widget = new WidgetSettingFlag(tree, child, meta, parameter->settings, value.key.c_str());
                this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

                for (std::size_t j = 0, o = value.settings.size(); j < o; ++j) {
                    this->BuildTreeItem(child, *value.settings[j]);
                }
            }
        } break;

        case SETTING_STRING: {
            const SettingMetaString &meta = static_cast<const SettingMetaString &>(meta_object);

            WidgetSettingString *widget = new WidgetSettingString(tree, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_LIST: {
            const SettingMetaList &meta = static_cast<const SettingMetaList &>(meta_object);

            WidgetSettingList *widget = new WidgetSettingList(tree, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        default: {
            item->setText(0, "Unknown setting");
            assert(0);  // Unknown setting
        } break;
    }

    for (std::size_t i = 0, n = meta_object.children.size(); i < n; ++i) {
        this->BuildTreeItem(item, *meta_object.children[i]);
    }
}

void SettingsTreeManager::BuildTree() {
    assert(this->parameter != nullptr);

    Configurator &configurator = Configurator::Get();
    const Layer *layer = configurator.layers.Find(parameter->key, parameter->api_version);
    assert(layer != nullptr);

    for (std::size_t i = 0, n = layer->settings.size(); i < n; ++i) {
        this->BuildTreeItem(nullptr, *layer->settings[i]);
    }

    if (!this->parameter->setting_tree_state.isEmpty()) {
        this->SetTreeState(parameter->setting_tree_state, 0, this->tree->invisibleRootItem());
    }
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

void SettingsTreeManager::OnPresetChanged(int combox_preset_index) {
    assert(combox_preset_index >= 0 && static_cast<std::size_t>(combox_preset_index) < preset_labels.size());
    const std::string &preset_label = preset_labels[combox_preset_index];

    if (preset_label == Layer::NO_PRESET) return;

    Configurator &configurator = Configurator::Get();
    const Layer *layer = configurator.layers.Find(this->parameter->key.c_str(), this->parameter->api_version);

    const LayerPreset *preset = GetPreset(layer->presets, preset_label.c_str());
    assert(preset != nullptr);
    this->parameter->ApplyPresetSettings(*preset);

    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void SettingsTreeManager::OnSettingChanged() { this->Refresh(REFRESH_ENABLE_ONLY); }

void SettingsTreeManager::Refresh(RefreshAreas refresh_areas) {
    Configurator &configurator = Configurator::Get();

    this->tree->blockSignals(true);

    QTreeWidgetItem *root_item = this->tree->invisibleRootItem();
    for (int i = 0, n = root_item->childCount(); i < n; ++i) {
        this->RefreshItem(refresh_areas, root_item->child(i));
    }

    this->tree->blockSignals(false);

    if (!(configurator.Get(HIDE_MESSAGE_NEED_APPLICATION_RESTART))) {
        configurator.Set(HIDE_MESSAGE_NEED_APPLICATION_RESTART);

        Alert::ConfiguratorRestart();
    }

    // Refresh layer configuration
    configurator.Override(OVERRIDE_AREA_LAYERS_SETTINGS_BIT);
}

void SettingsTreeManager::RefreshItem(RefreshAreas refresh_areas, QTreeWidgetItem *parent) {
    QWidget *widget = this->tree->itemWidget(parent, 0);
    if (widget != nullptr) {
        WidgetSettingBase *widget_base = dynamic_cast<WidgetSettingBase *>(widget);
        if (widget_base != nullptr) widget_base->Refresh(refresh_areas);
    }

    for (int i = 0, n = parent->childCount(); i < n; ++i) {
        QTreeWidgetItem *child = parent->child(i);
        if (child == nullptr) continue;

        this->RefreshItem(refresh_areas, child);
    }
}
