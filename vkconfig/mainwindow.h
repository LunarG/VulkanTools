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

#pragma once

#include "configurator.h"
#include "settings_tree.h"

#include "ui_mainwindow.h"

#include <QDialog>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QLabel>
#include <QRadioButton>
#include <QShowEvent>
#include <QResizeEvent>
#include <QProcess>

#include <memory>
#include <string>

/// This just allows me to associate a specific profile definition
/// with a list widget item.
class ConfigurationListItem : public QTreeWidgetItem {
   public:
    ConfigurationListItem(const std::string &configuration_name) : configuration_name(configuration_name) {}
    std::string configuration_name;
    QRadioButton *radio_button;

   private:
    ConfigurationListItem(const ConfigurationListItem &) = delete;
    ConfigurationListItem &operator=(const ConfigurationListItem &) = delete;
};

enum Tool { TOOL_VULKAN_INFO, TOOL_VULKAN_INSTALL };

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void UpdateUI();
    void UpdateConfiguration();

   private:
    SettingsTreeManager _settings_tree_manager;

    std::unique_ptr<QProcess> _launch_application;  // Keeps track of the monitored app
    QFile _log_file;                                // Log file for layer output

    void LoadConfigurationList();
    void SetupLauncherTree();

    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *target, QEvent *event) override;

    std::unique_ptr<QDialog> vk_info_dialog;
    std::unique_ptr<QDialog> vk_installation_dialog;

    void Log(const std::string &log);

    ConfigurationListItem *GetCheckedItem();

    QComboBox *_launcher_apps_combo;
    QLineEdit *_launcher_executable;
    QLineEdit *_launcher_arguments;
    QLineEdit *_launcher_working;
    QLineEdit *_launcher_log_file_edit;
    QPushButton *_launcher_apps_browse_button;
    QPushButton *_launcher_executable_browse_button;
    QPushButton *_launcher_working_browse_button;
    QPushButton *_launcher_log_file_browse_button;

    void RemoveClicked(ConfigurationListItem *item);
    void ResetClicked(ConfigurationListItem *item);
    void RenameClicked(ConfigurationListItem *item);
    void EditClicked(ConfigurationListItem *item);
    void NewClicked();
    void DuplicateClicked(ConfigurationListItem *item);
    void ExportClicked(ConfigurationListItem *item);
    void ImportClicked(ConfigurationListItem *item);
    void ReloadDefaultClicked(ConfigurationListItem *item);

   public Q_SLOTS:
    void toolsVulkanInfo(bool checked);
    void toolsVulkanInstallation(bool checked);
    void toolsResetToDefault(bool checked);

    void OnHelpFindLayers(bool checked);
    void OnHelpAbout(bool checked);
    void OnHelpReadme(bool checked);
    void OnHelpChangelog(bool checked);
    void OnHelpVulkanSpec(bool checked);
    void OnHelpLayerSpec(bool checked);
    void OnHelpGPUInfo(bool checked);

    void editorExpanded(QTreeWidgetItem *item);

    void launchItemExpanded(QTreeWidgetItem *item);
    void launchItemCollapsed(QTreeWidgetItem *item);
    void launchItemChanged(int index);
    void launchSetLogFile();
    void launchSetExecutable();
    void launchSetWorkingFolder();
    void launchChangeLogFile(const QString &new_text);
    void launchChangeExecutable(const QString &new_text);
    void launchChangeWorkingFolder(const QString &new_text);
    void launchArgsEdited(const QString &new_text);

    void on_push_button_launcher_clicked();
    void on_push_button_clear_log_clicked();
    void on_radio_fully_clicked();
    void on_radio_override_clicked();
    void on_check_box_apply_list_clicked();
    void on_check_box_persistent_clicked();
    void on_check_box_clear_on_launch_clicked();
    void on_push_button_applications_clicked();
    void on_push_button_edit_clicked();
    void on_push_button_new_clicked();
    void on_push_button_remove_clicked();
    void on_push_button_duplicate_clicked();

    void OnConfigurationItemExpanded(QTreeWidgetItem *item);
    void OnConfigurationItemClicked(bool checked);
    void OnConfigurationTreeChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void OnConfigurationItemChanged(QTreeWidgetItem *item, int column);
    void OnConfigurationTreeClicked(QTreeWidgetItem *item, int column);
    void OnSettingsTreeClicked(QTreeWidgetItem *item, int column);
    void OnLauncherLoaderMessageChanged(int level);

    void standardOutputAvailable();                                 // stdout output is available
    void errorOutputAvailable();                                    // Layeroutput is available
    void processClosed(int exitCode, QProcess::ExitStatus status);  // app died

   private:
    MainWindow(const MainWindow &) = delete;
    MainWindow &operator=(const MainWindow &) = delete;

    void RemoveConfiguration(const std::string &configuration_name);
    bool SelectConfigurationItem(const std::string &configuration_name);
    void ResetLaunchApplication();
    void StartTool(Tool tool);
    QStringList BuildEnvVariables() const;

    std::unique_ptr<Ui::MainWindow> ui;
    bool been_warned_about_old_loader;
};

int run_gui(int argc, char *argv[]);