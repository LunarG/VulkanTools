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

SettingsTreeManager::SettingsTreeManager(std::shared_ptr<Ui::MainWindow> ui) : ui(ui) {
    assert(ui.get() != nullptr);

    this->layer_version = new LayerVersionComboBox(this->ui->configurations_group_box_settings);
    this->connect(this->layer_version, SIGNAL(itemChanged()), this, SLOT(OnLayerVersionChanged()));
    this->ui->configurations_group_box_settings->installEventFilter(this->layer_version);
}

void SettingsTreeManager::CreateGUI() {
    // Do this first to make absolutely sure if these is an old configuration still active it's state gets saved.
    this->CleanupGUI();

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    Parameter *parameter = configuration != nullptr ? configuration->GetActiveParameter() : nullptr;
    const bool no_selected_layer = configuration != nullptr ? configuration->selected_layer_name.empty() : false;

    if (parameter != nullptr) {
        if (parameter->manifest.Empty()) {
            configuration->SwitchLayerLatest(configurator.layers, configuration->GetActiveParameter()->key);
        } else {
            configuration->SwitchLayerVersion(configurator.layers, configuration->GetActiveParameter()->key, parameter->manifest);
        }
    }

    // Group box things
    const Layer *layer = parameter != nullptr ? configurator.layers.FindFromManifest(parameter->manifest) : nullptr;

    if (no_selected_layer) {
        this->ui->configurations_group_box_settings->setTitle("Select a layer to display settings");
        this->ui->configurations_group_box_settings->setCheckable(false);
        this->ui->configurations_presets->setVisible(false);
        this->layer_version->setVisible(false);
        return;
    } else if (!configurator.HasActiveSettings()) {
        this->ui->configurations_group_box_settings->setTitle("No Layer Settings");
        this->ui->configurations_group_box_settings->setCheckable(false);
        this->ui->configurations_presets->setVisible(false);
        this->layer_version->setVisible(false);
        return;
    } else {
        std::string title = parameter->key;
        title.erase(0, strlen("VK_LAYER_"));

        this->ui->configurations_group_box_settings->setTitle(format("%s:", title.c_str()).c_str());
        this->ui->configurations_group_box_settings->setCheckable(configurator.advanced);
        this->ui->configurations_group_box_settings->setChecked(parameter->override_settings);
        this->ui->configurations_presets->setVisible(!layer->presets.empty());
    }

    const std::vector<Path> &layer_versions = configurator.layers.GatherManifests(parameter->key);
    this->layer_version->setVisible(!layer_versions.empty());
    if (!layer_versions.empty()) {
        this->layer_version->Init(*parameter, layer_versions);
    }

    // preset combobox
    {
        this->ui->configurations_presets->blockSignals(true);
        this->ui->configurations_presets->clear();
        preset_labels.clear();
        if (!layer->presets.empty()) {
            this->ui->configurations_presets->addItem(Layer::NO_PRESET);
            preset_labels.push_back(Layer::NO_PRESET);

            for (std::size_t i = 0, n = layer->presets.size(); i < n; ++i) {
                const LayerPreset &layer_preset = layer->presets[i];

                if (!IsPlatformSupported(layer_preset.platform_flags)) {
                    continue;
                }
                if (layer_preset.view == SETTING_VIEW_HIDDEN) {
                    continue;
                }

                this->ui->configurations_presets->addItem((layer_preset.label + " Preset").c_str());
                preset_labels.push_back(layer_preset.label);
            }

            this->connect(this->ui->configurations_presets, SIGNAL(currentIndexChanged(int)), this, SLOT(OnPresetChanged(int)));
            this->ui->configurations_presets->setVisible(true);
        } else {
            this->ui->configurations_presets->setVisible(false);
        }
        this->ui->configurations_presets->blockSignals(false);
    }

    // settings tree
    {
        this->ui->configurations_settings->blockSignals(true);
        this->ui->configurations_settings->clear();

        this->BuildTree();

        this->connect(this->ui->configurations_settings, SIGNAL(expanded(const QModelIndex)), this,
                      SLOT(OnExpandedChanged(const QModelIndex)));
        this->connect(this->ui->configurations_settings, SIGNAL(collapsed(const QModelIndex)), this,
                      SLOT(OnExpandedChanged(const QModelIndex)));

        this->ui->configurations_settings->resizeColumnToContents(0);

        this->ui->configurations_settings->blockSignals(false);
    }
}

void SettingsTreeManager::CleanupGUI() {
    Configurator &configurator = Configurator::Get();
    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        Parameter *parameter = configuration->GetActiveParameter();
        if (parameter != nullptr) {
            parameter->setting_tree_state.clear();
            this->GetTreeState(parameter->setting_tree_state, this->ui->configurations_settings->invisibleRootItem());
        }
    }

    this->ui->configurations_settings->clear();
}

void SettingsTreeManager::OnExpandedChanged(const QModelIndex &index) {
    (void)index;

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            Parameter &parameter = configuration->parameters[i];

            if (parameter.key != configuration->selected_layer_name) {
                continue;
            }

            parameter.setting_tree_state.clear();
            this->GetTreeState(parameter.setting_tree_state, this->ui->configurations_settings->invisibleRootItem());
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
    Parameter *parameter = configurator.GetActiveParameter();
    assert(parameter != nullptr);

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    if (parent == nullptr) {
        this->ui->configurations_settings->addTopLevelItem(item);
    } else {
        parent->addChild(item);
    }

    switch (meta_object.type) {
        case SETTING_GROUP: {
            item->setText(0, meta_object.label.c_str());
            item->setToolTip(0, meta_object.description.c_str());
            item->setFont(0, this->ui->configurations_settings->font());
            item->setExpanded(meta_object.expanded);
        } break;
        case SETTING_BOOL:
        case SETTING_BOOL_NUMERIC_DEPRECATED: {
            const SettingMetaBool &meta = static_cast<const SettingMetaBool &>(meta_object);

            WidgetSettingBool *widget = new WidgetSettingBool(this->ui->configurations_settings, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_INT: {
            const SettingMetaInt &meta = static_cast<const SettingMetaInt &>(meta_object);

            WidgetSettingInt *widget = new WidgetSettingInt(this->ui->configurations_settings, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FLOAT: {
            const SettingMetaFloat &meta = static_cast<const SettingMetaFloat &>(meta_object);

            WidgetSettingFloat *widget = new WidgetSettingFloat(this->ui->configurations_settings, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FRAMES: {
            const SettingMetaFrames &meta = static_cast<const SettingMetaFrames &>(meta_object);

            WidgetSettingFrames *widget =
                new WidgetSettingFrames(this->ui->configurations_settings, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_SAVE_FILE:
        case SETTING_LOAD_FILE:
        case SETTING_SAVE_FOLDER:
        case SETTING_LOAD_FOLDER: {
            const SettingMetaFilesystem &meta = static_cast<const SettingMetaFilesystem &>(meta_object);

            WidgetSettingFilesystem *widget =
                new WidgetSettingFilesystem(this->ui->configurations_settings, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_ENUM: {
            const SettingMetaEnum &meta = static_cast<const SettingMetaEnum &>(meta_object);

            WidgetSettingEnum *enum_widget =
                new WidgetSettingEnum(this->ui->configurations_settings, item, meta, parameter->settings);
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

                WidgetSettingFlag *widget =
                    new WidgetSettingFlag(this->ui->configurations_settings, child, meta, parameter->settings, value.key.c_str());
                this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

                for (std::size_t j = 0, o = value.settings.size(); j < o; ++j) {
                    this->BuildTreeItem(child, *value.settings[j]);
                }
            }
        } break;

        case SETTING_STRING: {
            const SettingMetaString &meta = static_cast<const SettingMetaString &>(meta_object);

            WidgetSettingString *widget =
                new WidgetSettingString(this->ui->configurations_settings, item, meta, parameter->settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_LIST: {
            const SettingMetaList &meta = static_cast<const SettingMetaList &>(meta_object);

            WidgetSettingList *widget = new WidgetSettingList(this->ui->configurations_settings, item, meta, parameter->settings);
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
    Configurator &configurator = Configurator::Get();
    Parameter *parameter = configurator.GetActiveParameter();
    assert(parameter != nullptr);

    const Layer *layer = configurator.layers.Find(parameter->key, parameter->api_version);
    assert(layer != nullptr);

    for (std::size_t i = 0, n = layer->settings.size(); i < n; ++i) {
        this->BuildTreeItem(nullptr, *layer->settings[i]);
    }

    if (!parameter->setting_tree_state.isEmpty()) {
        this->SetTreeState(parameter->setting_tree_state, 0, this->ui->configurations_settings->invisibleRootItem());
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

void SettingsTreeManager::OnLayerVersionChanged() {
    this->CreateGUI();

    Configurator &configurator = Configurator::Get();
    configurator.Override(OVERRIDE_AREA_ALL);

    emit signalLayerVersionChanged();
}

void SettingsTreeManager::OnPresetChanged(int combox_preset_index) {
    assert(combox_preset_index >= 0 && static_cast<std::size_t>(combox_preset_index) < preset_labels.size());
    const std::string &preset_label = preset_labels[combox_preset_index];

    if (preset_label == Layer::NO_PRESET) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    Parameter *parameter = configurator.GetActiveParameter();
    assert(parameter != nullptr);

    const Layer *layer = configurator.layers.Find(parameter->key.c_str(), parameter->api_version);

    const LayerPreset *preset = GetPreset(layer->presets, preset_label.c_str());
    assert(preset != nullptr);
    parameter->ApplyPresetSettings(*preset);

    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void SettingsTreeManager::OnSettingChanged() { this->Refresh(REFRESH_ENABLE_ONLY); }

void SettingsTreeManager::Refresh(RefreshAreas refresh_areas) {
    Configurator &configurator = Configurator::Get();

    this->ui->configurations_settings->blockSignals(true);

    QTreeWidgetItem *root_item = this->ui->configurations_settings->invisibleRootItem();
    for (int i = 0, n = root_item->childCount(); i < n; ++i) {
        this->RefreshItem(refresh_areas, root_item->child(i));
    }

    this->ui->configurations_settings->blockSignals(false);

    if (!(configurator.Get(HIDE_MESSAGE_NEED_APPLICATION_RESTART))) {
        configurator.Set(HIDE_MESSAGE_NEED_APPLICATION_RESTART);

        Alert::ConfiguratorRestart();
    }

    // Refresh layer configuration
    configurator.Override(OVERRIDE_AREA_LAYERS_SETTINGS_BIT);
}

void SettingsTreeManager::RefreshItem(RefreshAreas refresh_areas, QTreeWidgetItem *parent) {
    QWidget *widget = this->ui->configurations_settings->itemWidget(parent, 0);
    if (widget != nullptr) {
        WidgetSettingBase *widget_base = dynamic_cast<WidgetSettingBase *>(widget);
        if (widget_base != nullptr) widget_base->Refresh(refresh_areas);
    }

    for (int i = 0, n = parent->childCount(); i < n; ++i) {
        QTreeWidgetItem *child = parent->child(i);
        if (child == nullptr) {
            continue;
        }

        this->RefreshItem(refresh_areas, child);
    }
}
