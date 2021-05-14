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
#include <QSettings>
#include <QMessageBox>

#include <cassert>

static const char *TOOLTIP_ORDER =
    "Layers are executed between the Vulkan application and driver in the specific order represented here";

SettingsTreeManager::SettingsTreeManager() : tree(nullptr) {}

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
        item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
        this->tree->addTopLevelItem(item);
    } else {
        const std::size_t overridden_layer_count = CountOverriddenLayers(configuration->parameters);

        if (overridden_layer_count > 1) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, "Vulkan Applications");
            item->setToolTip(0, TOOLTIP_ORDER);
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
            layer_item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
            if (layer != nullptr) layer_item->setToolTip(0, layer->description.c_str());

            this->tree->addTopLevelItem(layer_item);
            layer_item->setExpanded(true);

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
                layer_item->addChild(presets_item);
                WidgetPreset *presets_combobox = new WidgetPreset(this->tree, presets_item, *layer, parameter);
                this->connect(presets_combobox, SIGNAL(itemChanged()), this, SLOT(OnPresetChanged()));
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
            item->setToolTip(0, TOOLTIP_ORDER);
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
            excluded_layers->setToolTip(0, "The following layers won't be executed.");
            excluded_layers->setFont(0, font_section);
            excluded_layers->setSizeHint(0, QSize(0, ITEM_HEIGHT));
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

    this->connect(this->tree, SIGNAL(expanded(const QModelIndex)), this, SLOT(OnExpandedChanged(const QModelIndex)));
    this->connect(this->tree, SIGNAL(collapsed(const QModelIndex)), this, SLOT(OnCollapsedChanged(const QModelIndex)));

    this->tree->resizeColumnToContents(0);

    if (!configuration->setting_tree_state.isEmpty()) {
        this->SetTreeState(configuration->setting_tree_state, 0, this->tree->invisibleRootItem());
    } else {
        assert(1);
    }

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

    this->tree->clear();
    this->tree = nullptr;
}

void SettingsTreeManager::OnExpandedChanged(const QModelIndex &index) {
    if (this->tree == nullptr)  // Was not initialized
        return;

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.configurations.GetActiveConfiguration();
    configuration->setting_tree_state.clear();
    GetTreeState(configuration->setting_tree_state, this->tree->invisibleRootItem());

    return;
}

void SettingsTreeManager::OnCollapsedChanged(const QModelIndex &index) {
    if (this->tree == nullptr)  // Was not initialized
        return;

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.configurations.GetActiveConfiguration();
    configuration->setting_tree_state.clear();
    GetTreeState(configuration->setting_tree_state, this->tree->invisibleRootItem());

    return;
}

static bool IsBuiltinValidation(const std::string &key) {
    static const char *VALIDATION_KEYS[] = {"disables",         "enables",        "gpuav_buffer_oob",
                                            "printf_to_stdout", "printf_verbose", "printf_buffer_size"};

    for (std::size_t i = 0, n = countof(VALIDATION_KEYS); i < n; ++i) {
        if (key == VALIDATION_KEYS[i]) return true;
    }

    return false;
}

void SettingsTreeManager::BuildValidationTree(QTreeWidgetItem *parent, Parameter &parameter) {
    Configurator &configurator = Configurator::Get();
    std::vector<Layer> &available_layers = configurator.layers.available_layers;
    Layer *validation_layer = FindByKey(available_layers, "VK_LAYER_KHRONOS_validation");
    assert(validation_layer != nullptr);

    QTreeWidgetItem *validation_areas_item = new QTreeWidgetItem();
    parent->addChild(validation_areas_item);

    // This just finds the enables and disables
    this->validation.reset(
        new WidgetSettingValidation(this->tree, validation_areas_item, validation_layer->settings, parameter.settings));
    this->connect(this->validation.get(), SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

    const SettingMetaSet &layer_setting_metas = FindByKey(available_layers, parameter.key.c_str())->settings;
    for (std::size_t setting_index = 0, n = layer_setting_metas.Size(); setting_index < n; ++setting_index) {
        const SettingMeta &setting_meta = layer_setting_metas[setting_index];
        if (IsBuiltinValidation(setting_meta.key)) continue;

        this->BuildTreeItem(parent, layer_setting_metas, parameter.settings, setting_meta);
    }
}

void SettingsTreeManager::BuildTreeItem(QTreeWidgetItem *parent, const SettingMetaSet &meta_set, SettingDataSet &data_set,
                                        const SettingMeta &meta_object) {
    if (!IsPlatformSupported(meta_object.platform_flags)) return;
    if (meta_object.view == SETTING_VIEW_HIDDEN) return;

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    parent->addChild(item);

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

            WidgetSettingBool *widget = new WidgetSettingBool(tree, item, meta, data_set);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_INT: {
            const SettingMetaInt &meta = static_cast<const SettingMetaInt &>(meta_object);

            WidgetSettingInt *widget = new WidgetSettingInt(tree, item, meta, data_set);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FLOAT: {
            const SettingMetaFloat &meta = static_cast<const SettingMetaFloat &>(meta_object);

            WidgetSettingFloat *widget = new WidgetSettingFloat(tree, item, meta, data_set);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FRAMES: {
            const SettingMetaFrames &meta = static_cast<const SettingMetaFrames &>(meta_object);

            WidgetSettingFrames *widget = new WidgetSettingFrames(tree, item, meta, data_set);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_SAVE_FILE:
        case SETTING_LOAD_FILE:
        case SETTING_SAVE_FOLDER: {
            const SettingMetaFilesystem &meta = static_cast<const SettingMetaFilesystem &>(meta_object);

            WidgetSettingFilesystem *widget = new WidgetSettingFilesystem(tree, item, meta, data_set);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_ENUM: {
            const SettingMetaEnum &meta = static_cast<const SettingMetaEnum &>(meta_object);

            WidgetSettingEnum *enum_widget = new WidgetSettingEnum(tree, item, meta, data_set);
            this->connect(enum_widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_FLAGS: {
            const SettingMetaFlags &meta = static_cast<const SettingMetaFlags &>(meta_object);

            item->setText(0, meta.label.c_str());
            item->setToolTip(0, meta.description.c_str());
            item->setExpanded(meta.expanded);

            for (std::size_t i = 0, n = meta.enum_values.size(); i < n; ++i) {
                if (!IsPlatformSupported(meta.enum_values[i].platform_flags)) continue;
                if (meta.enum_values[i].view == SETTING_VIEW_HIDDEN) continue;

                QTreeWidgetItem *child = new QTreeWidgetItem();
                item->addChild(child);

                WidgetSettingFlag *widget = new WidgetSettingFlag(tree, child, meta, data_set, meta.enum_values[i].key.c_str());
                this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

                for (std::size_t j = 0, o = meta.enum_values[i].settings.Size(); j < o; ++j) {
                    this->BuildTreeItem(child, meta.enum_values[i].settings, data_set, meta.enum_values[i].settings[j]);
                }
            }
        } break;

        case SETTING_STRING: {
            const SettingMetaString &meta = static_cast<const SettingMetaString &>(meta_object);

            WidgetSettingString *widget = new WidgetSettingString(tree, item, meta, data_set);
            this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
        } break;

        case SETTING_LIST: {
            const SettingMetaList &meta = static_cast<const SettingMetaList &>(meta_object);

            WidgetSettingList *widget = new WidgetSettingList(tree, item, meta, data_set);
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
    this->tree->blockSignals(true);

    QTreeWidgetItem *root_item = this->tree->invisibleRootItem();
    for (int i = 0, n = root_item->childCount(); i < n; ++i) {
        this->RefreshItem(refresh_areas, root_item->child(i));
    }

    this->tree->blockSignals(false);

    QSettings settings;
    if (!settings.value("vkconfig_restart", false).toBool()) {
        settings.setValue("vkconfig_restart", true);

        QMessageBox alert;
        alert.setText(
            "Vulkan Layers are fully configured when creating a Vulkan Instance which typically happens at Vulkan Application "
            "start.\n\n"
            "For changes to take effect, running Vulkan Applications should be restarted.");
        alert.setWindowTitle("Any change requires Vulkan Applications restart");
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
    }

    // Refresh layer configuration
    Configurator &configurator = Configurator::Get();
    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);
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
