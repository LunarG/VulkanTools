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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "tab.h"
#include "settings_tree.h"

#include "item_list.h"

#include <QPushButton>
#include <QAbstractItemView>

class TabConfigurations : public Tab {
    Q_OBJECT

   public:
    TabConfigurations(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui);
    virtual ~TabConfigurations();

    virtual void UpdateUI(UpdateUIMode ui_update_mode) override;
    virtual void CleanUI() override;
    virtual bool EventFilter(QObject *target, QEvent *event) override;

    void OnRenameConfiguration(QListWidgetItem *item);

    void UpdateUI_Configurations(UpdateUIMode ui_update_mode);
    void UpdateUI_Applications(UpdateUIMode ui_update_mode);
    void UpdateUI_Layers(UpdateUIMode ui_update_mode);
    void UpdateUI_Settings(UpdateUIMode ui_update_mode);

   public Q_SLOTS:
    void on_configurations_advanced_toggle_pressed();

    void on_configurations_executable_scope_currentIndexChanged(int index);
    void on_configurations_executable_list_currentIndexChanged(int index);
    void on_configurations_executable_append_pressed();
    void on_configurations_executable_remove_pressed();

    void on_configurations_list_toggled(bool checked);
    void on_configurations_layers_settings_toggled(bool checked);

    void on_configurations_list_itemDoubleClicked(QListWidgetItem *item);
    void on_configurations_list_itemChanged(QListWidgetItem *item);
    void on_configurations_list_currentRowChanged(int currentRow);

    void on_configuration_settings_file_enable_toggled(bool checked);
    void on_configuration_settings_file_search_pressed();
    void on_configuration_settings_file_remove_pressed();
    void on_configuration_settings_file_path_textEdited(const QString &text);

    void on_configurations_layers_list_currentRowChanged(int currentRow);

    void on_configurations_layerVersionChanged();

   private:
    SettingsTreeManager _settings_tree_manager;
    QPushButton *advanced_mode = nullptr;

    void OnContextMenuNewClicked(ListItem *item);
    void OnContextMenuImportClicked(ListItem *item);
    void OnContextMenuRenameClicked(ListItem *item);
    void OnContextMenuDuplicateClicked(ListItem *item);
    void OnContextMenuDeleteClicked(ListItem *item);
    void OnContextMenuResetOneClicked(ListItem *item);
    void OnContextMenuResetAllClicked(ListItem *item);
    void OnContextMenuExportConfigsClicked(ListItem *item);
    void OnContextMenuExportEnvVariablesBashClicked(ListItem *item);
    void OnContextMenuExportEnvVariablesCMDClicked(ListItem *item);
    void OnContextMenuExportExtensionCodeClicked(ListItem *item);
    void OnContextMenuExportSettingsClicked(ListItem *item);

    void UpdatePerExecutableConfigurations();
};
