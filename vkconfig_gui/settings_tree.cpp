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

SettingsTreeManager::SettingsTreeManager() : launched_application(false), tree(nullptr) {}

void SettingsTreeManager::CreateGUI(QTreeWidget *build_tree) {
    assert(build_tree);

    // Do this first to make absolutely sure if these is an old configuration still active it's state gets saved.
    CleanupGUI();

    Configurator &configurator = Configurator::Get();

    this->tree = build_tree;

    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration == nullptr || configurator.environment.selected_layer_name.empty()) {
        return;
    }

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
        item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
        this->tree->addTopLevelItem(item);
    } else {
        const std::size_t overridden_layer_count = CountOverriddenLayers(configuration->parameters);

        // There will be one top level item for each layer
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            Parameter &parameter = configuration->parameters[i];

            if (configurator.environment.selected_layer_name.find(parameter.key) == std::string::npos) {
                continue;
            }

            if (!IsPlatformSupported(parameter.platform_flags)) {
                continue;
            }

            if (parameter.control == LAYER_CONTROL_APPLICATIONS_API || parameter.control == LAYER_CONTROL_APPLICATIONS_ENV) {
                continue;
            }

            if (parameter.settings.empty()) {
                continue;
            }

            const std::vector<Layer> &selected_layers = configurator.layers.selected_layers;
            const Layer *layer = FindByKey(selected_layers, parameter.key.c_str());

            QTreeWidgetItem *layer_item = new QTreeWidgetItem();
            this->tree->addTopLevelItem(layer_item);

            std::string layer_text = parameter.key;
            if (layer == nullptr) {
                layer_text += " (Missing)";
            } else if (layer->status != STATUS_STABLE) {
                layer_text += std::string(" (") + GetToken(layer->status) + ")";
            }

            layer_item->setToolTip(0, parameter.key.c_str());  // Hack for the context menu to find the layer
            layer_item->setText(0, layer_text.c_str());
            layer_item->setFont(0, font_layer);
            layer_item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
            if (layer != nullptr) {
                layer_item->setToolTip(0, layer->description.c_str());
            }
            layer_item->setExpanded(parameter.control == LAYER_CONTROL_ON);

            if (layer == nullptr) {
                continue;
            }

            if (!layer->presets.empty()) {
                QTreeWidgetItem *presets_item = new QTreeWidgetItem();
                this->tree->addTopLevelItem(presets_item);
                WidgetPreset *presets_combobox = new WidgetPreset(this->tree, presets_item, *layer, parameter);
                this->connect(presets_combobox, SIGNAL(itemChanged()), this, SLOT(OnPresetChanged()));
            }

            BuildGenericTree(parameter);
        }
    }

    this->connect(this->tree, SIGNAL(expanded(const QModelIndex)), this, SLOT(OnExpandedChanged(const QModelIndex)));
    this->connect(this->tree, SIGNAL(collapsed(const QModelIndex)), this, SLOT(OnCollapsedChanged(const QModelIndex)));

    this->tree->resizeColumnToContents(0);

    if (!configuration->setting_tree_state.isEmpty()) {
        this->SetTreeState(configuration->setting_tree_state, 0, this->tree->invisibleRootItem());
    }

    this->tree->blockSignals(false);
}

void SettingsTreeManager::CleanupGUI() {
    // Was not initialized
    if (this->tree == nullptr) {
        return;
    }

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        configuration->setting_tree_state.clear();
        GetTreeState(configuration->setting_tree_state, this->tree->invisibleRootItem());
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
        configuration->setting_tree_state.clear();
        GetTreeState(configuration->setting_tree_state, this->tree->invisibleRootItem());
    }
}

void SettingsTreeManager::OnCollapsedChanged(const QModelIndex &index) {
    (void)index;

    // Was not initialized
    if (this->tree == nullptr) {
        return;
    }

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        configuration->setting_tree_state.clear();
        GetTreeState(configuration->setting_tree_state, this->tree->invisibleRootItem());
    }
}

static bool IsBuiltinValidationSetting(const Parameter &parameter, const std::string &key) {
    if (parameter.key != "VK_LAYER_KHRONOS_validation") {
        return false;
    }

    std::vector<std::string> keys;
    keys.push_back("enables");
    keys.push_back("disables");

    if (parameter.api_version.GetPatch() < 242) {
        keys.push_back("printf_to_stdout");
        keys.push_back("printf_verbose");
        keys.push_back("printf_buffer_size");
        keys.push_back("gpuav_buffer_oob");
        keys.push_back("warn_on_robust_oob");
        keys.push_back("validate_draw_indirect");
        keys.push_back("vma_linear_output");
        keys.push_back("fine_grained_locking");
    }

    return IsStringFound(keys, key);
}

void SettingsTreeManager::BuildTreeItem(QTreeWidgetItem *parent, Parameter &parameter, const SettingMeta &meta_object) {
    if (IsBuiltinValidationSetting(parameter, meta_object.key)) {
        return;
    }

    if (!IsPlatformSupported(meta_object.platform_flags)) {
        return;
    }

    if (meta_object.view == SETTING_VIEW_HIDDEN) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    if (meta_object.view == SETTING_VIEW_ADVANCED && !configurator.GetActiveConfiguration()->view_advanced_settings) {
        return;
    }

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

            WidgetSettingBool *widget = new WidgetSettingBool(tree, item, meta, parameter.settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_INT: {
            const SettingMetaInt &meta = static_cast<const SettingMetaInt &>(meta_object);

            WidgetSettingInt *widget = new WidgetSettingInt(tree, item, meta, parameter.settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FLOAT: {
            const SettingMetaFloat &meta = static_cast<const SettingMetaFloat &>(meta_object);

            WidgetSettingFloat *widget = new WidgetSettingFloat(tree, item, meta, parameter.settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FRAMES: {
            const SettingMetaFrames &meta = static_cast<const SettingMetaFrames &>(meta_object);

            WidgetSettingFrames *widget = new WidgetSettingFrames(tree, item, meta, parameter.settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_SAVE_FILE:
        case SETTING_LOAD_FILE:
        case SETTING_SAVE_FOLDER:
        case SETTING_LOAD_FOLDER: {
            const SettingMetaFilesystem &meta = static_cast<const SettingMetaFilesystem &>(meta_object);

            WidgetSettingFilesystem *widget = new WidgetSettingFilesystem(tree, item, meta, parameter.settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_ENUM: {
            const SettingMetaEnum &meta = static_cast<const SettingMetaEnum &>(meta_object);

            WidgetSettingEnum *enum_widget = new WidgetSettingEnum(tree, item, meta, parameter.settings);
            this->connect(enum_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

            for (std::size_t i = 0, n = meta.enum_values.size(); i < n; ++i) {
                const SettingEnumValue &value = meta.enum_values[i];

                if (!IsPlatformSupported(value.platform_flags)) continue;
                if (value.view == SETTING_VIEW_HIDDEN) continue;

                for (std::size_t i = 0, n = value.settings.size(); i < n; ++i) {
                    this->BuildTreeItem(item, parameter, *value.settings[i]);
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

                WidgetSettingFlag *widget = new WidgetSettingFlag(tree, child, meta, parameter.settings, value.key.c_str());
                this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

                for (std::size_t j = 0, o = value.settings.size(); j < o; ++j) {
                    this->BuildTreeItem(child, parameter, *value.settings[j]);
                }
            }
        } break;

        case SETTING_STRING: {
            const SettingMetaString &meta = static_cast<const SettingMetaString &>(meta_object);

            WidgetSettingString *widget = new WidgetSettingString(tree, item, meta, parameter.settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_LIST: {
            const SettingMetaList &meta = static_cast<const SettingMetaList &>(meta_object);

            WidgetSettingList *widget = new WidgetSettingList(tree, item, meta, parameter.settings);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        default: {
            item->setText(0, "Unknown setting");
            assert(0);  // Unknown setting
        } break;
    }

    for (std::size_t i = 0, n = meta_object.children.size(); i < n; ++i) {
        this->BuildTreeItem(item, parameter, *meta_object.children[i]);
    }
}

void SettingsTreeManager::BuildGenericTree(Parameter &parameter) {
    std::vector<Layer> &available_layers = Configurator::Get().layers.selected_layers;

    const SettingMetaSet &settings = FindByKey(available_layers, parameter.key.c_str())->settings;
    for (std::size_t i = 0, n = settings.size(); i < n; ++i) {
        this->BuildTreeItem(nullptr, parameter, *settings[i]);
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

void SettingsTreeManager::OnPresetChanged() { this->Refresh(REFRESH_ENABLE_AND_STATE); }

void SettingsTreeManager::OnSettingChanged() { this->Refresh(REFRESH_ENABLE_ONLY); }

void SettingsTreeManager::Refresh(RefreshAreas refresh_areas) {
    Configurator &configurator = Configurator::Get();

    this->tree->blockSignals(true);

    QTreeWidgetItem *root_item = this->tree->invisibleRootItem();
    for (int i = 0, n = root_item->childCount(); i < n; ++i) {
        this->RefreshItem(refresh_areas, root_item->child(i));
    }

    this->tree->blockSignals(false);

    if (this->launched_application) {
        if (!(configurator.environment.hide_message_boxes_flags & GetBit(HIDE_MESSAGE_NEED_APPLICATION_RESTART))) {
            configurator.environment.hide_message_boxes_flags |= GetBit(HIDE_MESSAGE_NEED_APPLICATION_RESTART);

            Alert::ConfiguratorRestart();
        }
    }

    // Refresh layer configuration
    configurator.Override();
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