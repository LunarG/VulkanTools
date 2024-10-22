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
#include "item.h"

#include <QAbstractItemView>

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
    void on_configurations_executable_scope_currentIndexChanged(int index);
    void on_configurations_executable_list_currentIndexChanged(int index);
    void on_configurations_executable_append_pressed();

    void on_configurations_list_toggled(bool checked);
    void on_configurations_layers_ordering_toggled(bool checked);
    void on_configurations_loader_messages_toggled(bool checked);
    void on_configurations_layers_settings_toggled(bool checked);

    void on_configuration_loader_errors_toggled(bool checked);
    void on_configuration_loader_warns_toggled(bool checked);
    void on_configuration_loader_infos_toggled(bool checked);
    void on_configuration_loader_debug_toggled(bool checked);
    void on_configuration_loader_layers_toggled(bool checked);
    void on_configuration_loader_drivers_toggled(bool checked);

    void on_configurations_list_itemChanged(QListWidgetItem *item);
    void on_configurations_list_currentRowChanged(int currentRow);
    void on_configurations_layers_list_currentRowChanged(int currentRow);

   private:
    SettingsTreeManager _settings_tree_manager;

    void OnContextMenuNewClicked(ListItem *item);
    void OnContextMenuImportClicked(ListItem *item);
    void OnContextMenuRenameClicked(ListItem *item);
    void OnContextMenuDuplicateClicked(ListItem *item);
    void OnContextMenuDeleteClicked(ListItem *item);
    void OnContextMenuResetClicked(ListItem *item);
    void OnContextMenuReloadClicked(ListItem *item);
    void OnContextMenuExportConfigsClicked(ListItem *item);
    void OnContextMenuExportSettingsClicked(ListItem *item);

   private:
    void ui_configurations_group_box_list_tooltip();
    void ui_configurations_group_box_settings_tooltip();
};
