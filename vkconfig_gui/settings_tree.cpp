/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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
#include "item_tree.h"

#include "widget_setting.h"
#include "widget_setting_group.h"
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
#include "../vkconfig_core/version.h"
#include "../vkconfig_core/util.h"
#include "../vkconfig_core/type_platform.h"
#include "../vkconfig_core/type_hide_message.h"

#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QApplication>

#include <cassert>

SettingsTreeManager::SettingsTreeManager(std::shared_ptr<Ui::MainWindow> ui) : ui(ui) {
    assert(ui.get() != nullptr);

    // this->layer_version = new LayerVersionComboBox(this->ui->configurations_group_box_settings);
    this->connect(this->ui->configurations_versions, SIGNAL(currentIndexChanged(int)), this, SLOT(OnLayerVersionChanged(int)));
    // this->ui->configurations_group_box_settings->installEventFilter(this->layer_version);
}

void SettingsTreeManager::CreateGUI() {
    // Do this first to make absolutely sure if these is an old configuration still active it's state gets saved.
    this->CleanupGUI();

    Configurator &configurator = Configurator::Get();
    if (!configurator.HasActiveParameter() || !configurator.HasActiveSettings()) {
        return;
    }

    Parameter *parameter = configurator.GetActiveParameter();
    if (parameter->builtin == LAYER_BUILTIN_NONE) {
        if (configurator.layers.FindFromManifest(parameter->manifest) == nullptr) {
            Configuration *configuration = configurator.GetActiveConfiguration();
            configuration->SwitchLayerLatest(configurator.layers, parameter->key);
        }
    }

    // this->RefreshVersion();

    const Layer *layer = configurator.layers.FindFromManifest(parameter->manifest);

    std::string title = parameter->key;
    if (layer != nullptr) {
        if (layer->status != STATUS_STABLE) {
            title += format(" (%s)", GetToken(layer->status));
        }
    }
    // title.erase(0, strlen("VK_LAYER_"));

    this->ui->configurations_group_box_settings->blockSignals(true);
    this->ui->configurations_group_box_settings->setTitle(format("%s:", title.c_str()).c_str());
    this->ui->configurations_group_box_settings->setEnabled(configurator.HasEnabledUI(ENABLE_UI_SETTINGS));
    this->ui->configurations_group_box_settings->setCheckable(true);
    this->ui->configurations_group_box_settings->setChecked(parameter->override_settings);
    this->ui->configurations_group_box_settings->blockSignals(false);
    this->ui->configurations_presets->setVisible(!layer->presets.empty());

    const std::vector<Path> &layer_versions = configurator.layers.GatherManifests(parameter->key);
    this->ui->configurations_versions->setEnabled(true);
    this->ui->configurations_versions->setVisible(!layer_versions.empty());
    if (!layer_versions.empty()) {
        const Layer *layer_select = configurator.layers.FindFromManifest(parameter->manifest);
        const Layer *layer_latest = configurator.layers.Find(parameter->key, Version::LATEST);

        this->ui->configurations_versions->blockSignals(true);
        this->ui->configurations_versions->clear();

        this->layer_version_path.clear();

        const std::string &latest_label = "Latest - " + layer_latest->manifest_path.AbsolutePath();
        this->ui->configurations_versions->addItem(latest_label.c_str());
        this->ui->configurations_versions->setItemData(0, layer_latest->manifest_path.AbsolutePath().c_str(), Qt::ToolTipRole);

        this->layer_version_path.push_back(layer_latest->manifest_path);

        int version_index = 0;
        for (std::size_t i = 0, n = layer_versions.size(); i < n; ++i) {
            if (layer_versions[i] == parameter->manifest) {
                version_index = this->ui->configurations_versions->count();
            }

            const Layer *layer_version = configurator.layers.FindFromManifest(layer_versions[i]);

            const int current_index = this->ui->configurations_versions->count();

            const std::string &label = layer_version->api_version.str() + " - " + layer_version->manifest_path.AbsolutePath();

            this->ui->configurations_versions->addItem(label.c_str());
            this->ui->configurations_versions->setItemData(current_index, layer_version->manifest_path.AbsolutePath().c_str(),
                                                           Qt::ToolTipRole);

            this->layer_version_path.push_back(layer_version->manifest_path);
        }

        if (parameter->api_version != Version::LATEST) {
            this->ui->configurations_versions->setCurrentIndex(version_index);
        }

        this->ui->configurations_versions->blockSignals(false);

        if (layer_select != nullptr) {
            this->ui->configurations_versions->setToolTip(layer_select->manifest_path.AbsolutePath().c_str());
        }
    }

    // preset combobox
    {
        this->ui->configurations_presets->blockSignals(true);
        this->ui->configurations_presets->clear();
        this->ui->configurations_presets->setToolTip("Change to apply a layer setting preset");
        if (!layer->presets.empty()) {
            this->ui->configurations_presets->addItem("User-Defined Settings");

            for (std::size_t i = 0, n = layer->presets.size(); i < n; ++i) {
                const LayerPreset &layer_preset = layer->presets[i];

                if (!IsPlatformSupported(layer_preset.platform_flags)) {
                    continue;
                }
                if (configurator.GetUseLayerDevMode()) {
                    if (layer_preset.view == SETTING_VIEW_HIDDEN) {
                        continue;
                    }
                } else {
                    if (layer_preset.view != SETTING_VIEW_STANDARD) {
                        continue;
                    }
                }

                this->ui->configurations_presets->addItem((layer_preset.label + " Preset").c_str());
            }

            this->connect(this->ui->configurations_presets, SIGNAL(currentIndexChanged(int)), this, SLOT(OnPresetChanged(int)));
            this->ui->configurations_presets->setVisible(true);

            this->RefreshPresetLabel();
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

        this->connect(this->ui->configurations_settings, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this,
                      SLOT(on_item_collapsed(QTreeWidgetItem *)));
        this->connect(this->ui->configurations_settings, SIGNAL(itemExpanded(QTreeWidgetItem *)), this,
                      SLOT(on_item_expanded(QTreeWidgetItem *)));

        this->ui->configurations_settings->resizeColumnToContents(0);

        this->ui->configurations_settings->blockSignals(false);
    }
}

void SettingsTreeManager::CleanupGUI() {
    Configurator &configurator = Configurator::Get();

    this->ui->configurations_settings->clear();

    if (!configurator.HasActiveSettings()) {
        this->ui->configurations_group_box_settings->setTitle("No Layer Settings");
        this->ui->configurations_group_box_settings->setCheckable(false);
        this->ui->configurations_presets->setVisible(false);
        this->ui->configurations_versions->setVisible(false);
        return;
    }

    if (!configurator.HasActiveParameter()) {
        this->ui->configurations_group_box_settings->setTitle("Select a layer to display settings");
        this->ui->configurations_group_box_settings->setCheckable(false);
        this->ui->configurations_presets->setVisible(false);
        this->ui->configurations_versions->setVisible(false);
        return;
    }
}

void SettingsTreeManager::SetSettingExpanded(const std::string &key, const std::string &flag, bool expanded) {
    Configurator &configurator = Configurator::Get();
    Parameter *parameter = configurator.GetActiveParameter();
    if (parameter == nullptr) {
        return;
    }

    parameter->SetExpanded(key, flag, expanded);
}

void SettingsTreeManager::on_item_collapsed(QTreeWidgetItem *item) {
    assert(item != nullptr);

    TreeItem *tree_item = static_cast<TreeItem *>(item);
    this->SetSettingExpanded(tree_item->key, tree_item->flag, false);
}

void SettingsTreeManager::on_item_expanded(QTreeWidgetItem *item) {
    assert(item != nullptr);

    TreeItem *tree_item = static_cast<TreeItem *>(item);
    this->SetSettingExpanded(tree_item->key, tree_item->flag, true);
}

void SettingsTreeManager::BuildTreeItem(QTreeWidgetItem *parent, const SettingMeta &meta_object) {
    if (!IsPlatformSupported(meta_object.platform_flags)) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    if (configurator.GetUseLayerDevMode()) {
        if (meta_object.view == SETTING_VIEW_HIDDEN) {
            return;
        }
    } else {
        if (meta_object.view != SETTING_VIEW_STANDARD) {
            return;
        }
    }

    Parameter *parameter = configurator.GetActiveParameter();
    assert(parameter != nullptr);

    QTreeWidgetItem *item = new TreeItem(meta_object.key);
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    if (parent == nullptr) {
        this->ui->configurations_settings->addTopLevelItem(item);
    } else {
        parent->addChild(item);
    }
    item->setExpanded(parameter->GetExpanded(meta_object.key.c_str()));

    const SettingDependenceMode enabled = ::CheckDependence(meta_object, parameter->settings);
    item->setHidden(enabled == SETTING_DEPENDENCE_HIDE);
    item->setDisabled(enabled != SETTING_DEPENDENCE_ENABLE);

    switch (meta_object.type) {
        case SETTING_GROUP: {
            const SettingMetaGroup &meta = static_cast<const SettingMetaGroup &>(meta_object);

            WidgetSettingGroup *widget = new WidgetSettingGroup(this->ui->configurations_settings, item, meta, parameter->settings);
            (void)widget;
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

                if (!IsPlatformSupported(value.platform_flags)) {
                    continue;
                }

                if (configurator.GetUseLayerDevMode()) {
                    if (value.view == SETTING_VIEW_HIDDEN) {
                        return;
                    }
                } else {
                    if (value.view != SETTING_VIEW_STANDARD) {
                        return;
                    }
                }

                for (std::size_t i = 0, n = value.settings.size(); i < n; ++i) {
                    this->BuildTreeItem(item, *value.settings[i]);
                }
            }
        } break;

        case SETTING_FLAGS: {
            const SettingMetaFlags &meta = static_cast<const SettingMetaFlags &>(meta_object);

            item->setText(0, meta.label.c_str());
            item->setToolTip(0, meta.description.c_str());

            for (std::size_t i = 0, n = meta.enum_values.size(); i < n; ++i) {
                const SettingEnumValue &value = meta.enum_values[i];

                if (!IsPlatformSupported(value.platform_flags)) {
                    continue;
                }

                if (configurator.GetUseLayerDevMode()) {
                    if (value.view == SETTING_VIEW_HIDDEN) {
                        return;
                    }
                } else {
                    if (value.view != SETTING_VIEW_STANDARD) {
                        return;
                    }
                }

                QTreeWidgetItem *child = new TreeItem(meta.key, value.key);
                item->addChild(child);
                child->setExpanded(parameter->GetExpanded(meta.key, value.key));

                WidgetSettingFlag *widget =
                    new WidgetSettingFlag(this->ui->configurations_settings, child, meta, parameter->settings, value.key);
                this->connect(widget, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));

                if (value.status == STATUS_DEPRECATED && !value.deprecated_by_key.empty()) {
                    const Layer *layer = configurator.layers.FindFromManifest(parameter->manifest);
                    const SettingMeta *setting_meta = ::FindSetting(layer->settings, value.deprecated_by_key.c_str());
                    child->setToolTip(
                        0,
                        format("Replaced by \"%s\" (%s) setting.", setting_meta->label.c_str(), setting_meta->key.c_str()).c_str());
                } else {
                    child->setToolTip(0, value.description.c_str());
                }

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

    if (meta_object.status == STATUS_DEPRECATED) {
        if (meta_object.deprecated_by_key.empty()) {
            item->setToolTip(0, format("(%s) %s", GetToken(meta_object.status), meta_object.description.c_str()).c_str());
        } else {
            const Layer *layer = configurator.layers.FindFromManifest(parameter->manifest);
            const SettingMeta *setting_meta = ::FindSetting(layer->settings, meta_object.deprecated_by_key.c_str());

            item->setToolTip(
                0, format("Replaced by \"%s\" (%s) setting.", setting_meta->label.c_str(), setting_meta->key.c_str()).c_str());
        }
    }

    for (std::size_t i = 0, n = meta_object.children.size(); i < n; ++i) {
        this->BuildTreeItem(item, *meta_object.children[i]);
    }
}

void SettingsTreeManager::BuildTree() {
    Configurator &configurator = Configurator::Get();
    Parameter *parameter = configurator.GetActiveParameter();
    assert(parameter != nullptr);

    const Layer *layer = configurator.layers.FindFromManifest(parameter->manifest);
    assert(layer != nullptr);

    for (std::size_t i = 0, n = layer->settings.size(); i < n; ++i) {
        this->BuildTreeItem(nullptr, *layer->settings[i]);
    }
}

void SettingsTreeManager::OnLayerVersionChanged(int index) {
    const Path &path = this->layer_version_path[index];

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    Parameter *parameter = configuration->GetActiveParameter();
    if (index == 0) {
        configuration->SwitchLayerLatest(configurator.layers, parameter->key);
    } else {
        configuration->SwitchLayerVersion(configurator.layers, parameter->key, path);
    }

    const Layer *layer = configurator.layers.FindFromManifest(path);
    assert(layer != nullptr);
    this->ui->configurations_versions->setToolTip(layer->manifest_path.AbsolutePath().c_str());

    this->CreateGUI();

    configurator.Override(OVERRIDE_AREA_ALL);

    emit signalLayerVersionChanged();
}

void SettingsTreeManager::OnPresetChanged(int combox_preset_index) {
    const int preset_index = combox_preset_index - 1;

    Configurator &configurator = Configurator::Get();
    Parameter *parameter = configurator.GetActiveParameter();
    assert(parameter != nullptr);

    if (preset_index == Layer::NO_PRESET) {
        return;
    }

    const Layer *layer = configurator.layers.Find(parameter->key.c_str(), parameter->api_version);
    if (layer == nullptr) {
        return;
    }

    if (!(preset_index >= 0 && static_cast<std::size_t>(preset_index) < layer->presets.size())) {
        return;
    }

    const LayerPreset &preset = layer->presets[preset_index];
    parameter->ApplyPresetSettings(preset);

    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void SettingsTreeManager::OnSettingChanged() {
    this->RefreshPresetLabel();
    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void SettingsTreeManager::RefreshPresetLabel() {
    Configurator &configurator = Configurator::Get();
    const Parameter *parameter = configurator.GetActiveParameter();
    const Layer *layer = configurator.layers.FindFromManifest(parameter->manifest);

    const int preset_index = layer->FindPresetIndex(parameter->settings) + 1;
    this->ui->configurations_presets->setCurrentIndex(preset_index);
}

/*
void SettingsTreeManager::RefreshVersion() {
    if (this->layer_version != nullptr) {
        this->layer_version->setEnabled(true);
        QEvent resize_event(QEvent::Resize);
        this->layer_version->eventFilter(nullptr, &resize_event);
    }
}
*/

void SettingsTreeManager::Refresh(RefreshAreas refresh_areas) {
    Configurator &configurator = Configurator::Get();

    this->ui->configurations_settings->blockSignals(true);

    QTreeWidgetItem *root_item = this->ui->configurations_settings->invisibleRootItem();
    for (int i = 0, n = root_item->childCount(); i < n; ++i) {
        this->RefreshItem(refresh_areas, root_item->child(i));
    }

    this->ui->configurations_settings->blockSignals(false);

    // Refresh layer configuration
    configurator.Override(OVERRIDE_AREA_LAYERS_SETTINGS_BIT);
}

void SettingsTreeManager::RefreshItem(RefreshAreas refresh_areas, QTreeWidgetItem *parent) {
    QWidget *widget = this->ui->configurations_settings->itemWidget(parent, 0);
    if (widget != nullptr) {
        WidgetSettingBase *widget_base = dynamic_cast<WidgetSettingBase *>(widget);
        if (widget_base != nullptr) {
            widget_base->Refresh(refresh_areas);
        }
    }

    for (int i = 0, n = parent->childCount(); i < n; ++i) {
        QTreeWidgetItem *child = parent->child(i);
        if (child == nullptr) {
            continue;
        }

        this->RefreshItem(refresh_areas, child);
    }
}
