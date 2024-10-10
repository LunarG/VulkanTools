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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "tab.h"
#include "settings_tree.h"

#include <QAbstractItemView>

class ListWidgetItemParameter : public QListWidgetItem {
   public:
    ListWidgetItemParameter(const char *layer_name) : widget(nullptr), layer_name(layer_name) { assert(layer_name != nullptr); }

    std::string layer_name;
    QWidget *widget;

   private:
    ListWidgetItemParameter(const ListWidgetItemParameter &) = delete;
    ListWidgetItemParameter &operator=(const ListWidgetItemParameter &) = delete;
};

/// This just allows me to associate a specific profile definition
/// with a list widget item.
class ConfigurationListItem : public QListWidgetItem {
   public:
    ConfigurationListItem(const char *configuration_name) : configuration_name(configuration_name) {
        assert(configuration_name != nullptr);
    }

    std::string configuration_name;

   private:
    ConfigurationListItem(const ConfigurationListItem &) = delete;
    ConfigurationListItem &operator=(const ConfigurationListItem &) = delete;
};

struct TabConfigurations : public Tab {
    Q_OBJECT

   public:
    TabConfigurations(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui);
    virtual ~TabConfigurations();

    virtual void UpdateUI(UpdateUIMode ui_update_mode) override;
    virtual void CleanUI() override;
    virtual bool EventFilter(QObject *target, QEvent *event) override;

    void OnSelectConfiguration(int currentRow);
    void OnRenameConfiguration(QListWidgetItem *item);
    void OnSelectLayer(int currentRow);
    void OnCheckedLoaderMessageTypes(bool checked);

    void UpdateUI_Configurations(UpdateUIMode ui_update_mode);
    void UpdateUI_Applications(UpdateUIMode ui_update_mode);
    void UpdateUI_LoaderMessages();
    void UpdateUI_Layers(UpdateUIMode ui_update_mode);
    void UpdateUI_Settings(UpdateUIMode ui_update_mode);

   public Q_SLOTS:
    void on_check_box_per_application_toggled(bool checked);
    void on_combo_box_mode_currentIndexChanged(int index);
    void on_combo_box_applications_currentIndexChanged(int index);

    void on_combo_box_layers_view_currentIndexChanged(int index);

    void on_configuration_loader_errors_checkBox_toggled(bool checked);
    void on_configuration_loader_warns_checkBox_toggled(bool checked);
    void on_configuration_loader_infos_checkBox_toggled(bool checked);
    void on_configuration_loader_debug_checkBox_toggled(bool checked);
    void on_configuration_loader_layers_checkBox_toggled(bool checked);
    void on_configuration_loader_drivers_checkBox_toggled(bool checked);

    void on_configurations_list_itemChanged(QListWidgetItem *item);
    void on_configurations_list_currentRowChanged(int currentRow);
    void on_configurations_layers_list_currentRowChanged(int currentRow);

   private:
    SettingsTreeManager _settings_tree_manager;

    void OnContextMenuNewClicked(ConfigurationListItem *item);
    void OnContextMenuImportClicked(ConfigurationListItem *item);
    void OnContextMenuRenameClicked(ConfigurationListItem *item);
    void OnContextMenuDuplicateClicked(ConfigurationListItem *item);
    void OnContextMenuDeleteClicked(ConfigurationListItem *item);
    void OnContextMenuResetClicked(ConfigurationListItem *item);
    void OnContextMenuReloadClicked(ConfigurationListItem *item);
    void OnContextMenuExportConfigsClicked(ConfigurationListItem *item);
    void OnContextMenuExportSettingsClicked(ConfigurationListItem *item);
};
