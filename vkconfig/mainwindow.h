/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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
#include "settingstreemanager.h"
#include "dlgvulkananalysis.h"
#include "dlgvulkaninfo.h"

#include <QVector>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QLabel>
#include <QRadioButton>
#include <QShowEvent>
#include <QResizeEvent>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

//////////////////////////////////////////////////
/// This just allows me to associate a specific profile definition
/// with a list widget item.
class ConfigurationListItem : public QTreeWidgetItem {
   public:
    ConfigurationListItem(Configuration &configuration) : configuration(configuration) {}
    Configuration &configuration;
    QRadioButton *radio_button;

   private:
    ConfigurationListItem(const ConfigurationListItem &) = delete;
    ConfigurationListItem &operator=(const ConfigurationListItem &) = delete;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void UpdateUI();
    void UpdateConfiguration();

   protected:
    SettingsTreeManager _settings_tree_manager;

    QProcess *_launch_application;  // Keeps track of the monitored app
    QFile _log_file;                // Log file for layer output

    void LoadConfigurationList();
    void SetupLaunchTree();

    virtual void closeEvent(QCloseEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual bool eventFilter(QObject *target, QEvent *event) override;

    dlgVulkanAnalysis *_vk_via;
    dlgVulkanInfo *_vk_info;

   private:
    void Log(const QString &log);

    Ui::MainWindow *ui;
    ConfigurationListItem *_selected_configuration_item;
    ConfigurationListItem *GetCheckedItem();

    QComboBox *_launcher_apps_combo;
    QLineEdit *_launch_arguments;
    QLineEdit *_launcher_working;
    QLineEdit *_launcher_log_file_edit;
    QPushButton *_launcher_apps_browse_button;
    QPushButton *_launcher_working_browse_button;
    QPushButton *_launcher_log_file_browse_button;

    ConfigurationListItem *SaveLastItem();
    bool RestoreLastItem(const char *szOverride = nullptr);
    QString _last_item;

    void RemoveClicked(ConfigurationListItem *item);
    void RenameClicked(ConfigurationListItem *item);
    void EditClicked(ConfigurationListItem *item);
    void NewClicked();
    void DuplicateClicked(ConfigurationListItem *item);
    void ExportClicked(ConfigurationListItem *item);
    void ImportClicked(ConfigurationListItem *item);
    void EditCustomPathsClicked(ConfigurationListItem *item);

   public Q_SLOTS:
    void aboutVkConfig(bool checked);
    void toolsVulkanInfo(bool checked);
    void toolsVulkanInstallation(bool checked);
    void toolsSetCustomPaths(bool checked);
    void toolsResetToDefault(bool checked);

    void helpShowHelp(bool checked);
    void helpShowVulkanSpec(bool checked);
    void helpShowLayerSpec(bool checked);

    void addCustomPaths();

    void editorExpanded(QTreeWidgetItem *item);

    void launchItemExpanded(QTreeWidgetItem *item);
    void launchItemCollapsed(QTreeWidgetItem *item);
    void launchItemChanged(int index);
    void launchSetLogFile();
    void launchSetWorkingFolder();
    void launchChangeLogFile(const QString &new_text);
    void launchChangeWorkingFolder(const QString &new_text);
    void launchArgsEdited(const QString &new_text);

    void on_pushButtonLaunch_clicked();
    void on_pushButtonClearLog_clicked();
    void on_radioFully_clicked();
    void on_radioOverride_clicked();
    void on_checkBoxApplyList_clicked();
    void on_checkBoxPersistent_clicked();
    void on_checkBoxClearOnLaunch_clicked();
    void on_pushButtonAppList_clicked();
    void on_pushButtonEditProfile_clicked();

    void profileItemChanged(QTreeWidgetItem *item, int column);
    void profileTreeChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void profileItemClicked(bool checked);
    void profileItemExpanded(QTreeWidgetItem *item);

    void OnConfigurationTreeClicked(QTreeWidgetItem *item, int column);
    void OnConfigurationSettingsTreeClicked(QTreeWidgetItem *item, int column);

    void standardOutputAvailable();                                 // stdout output is available
    void errorOutputAvailable();                                    // Layeroutput is available
    void processClosed(int exitCode, QProcess::ExitStatus status);  // app died

   private:
    MainWindow(const MainWindow &) = delete;
    MainWindow &operator=(const MainWindow &) = delete;
};
