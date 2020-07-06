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
#include "dlghelp.h"

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
/// \brief The ContigurationListItem class
/// This just allows me to associate a specific profile definition
/// with a list widget item.
class ContigurationListItem : public QTreeWidgetItem {
   public:
    Configuration *configuration;
    QRadioButton *radio_button;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   protected:
    SettingsTreeManager settings_tree_manager_;

    QProcess *launch_application_;  // Keeps track of the monitored app
    QFile *log_file_;               // Log file for layer output

    void LoadConfigurationList();
    void SetupLaunchTree();

    void ChangeActiveConfiguration(Configuration *configuration);

    virtual void closeEvent(QCloseEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

    virtual bool eventFilter(QObject *target, QEvent *event) override;

    dlgVulkanAnalysis *vk_via_;
    dlgVulkanInfo *vk_info_;
    dlgHelp *help_;

   private:
    Ui::MainWindow *ui_;
    ContigurationListItem *selected_configuration_item_;
    ContigurationListItem *GetCheckedItem();

    QComboBox *pLaunchAppsCombo;
    QLineEdit *pLaunchArguments;
    QLineEdit *pLaunchWorkingFolder;
    QLineEdit *pLaunchLogFileEdit;
    QPushButton *pLuanchAppBrowseButton;
    QPushButton *pLaunchWorkingFolderButton;
    QPushButton *pLaunchLogFilebutton;

    void ResetLaunchOptions();

    void RemoveClicked(ContigurationListItem *item);
    void RenameClicked(ContigurationListItem *item);
    void NewClicked();
    void DuplicateClicked(ContigurationListItem *item);
    void ExportClicked(ContigurationListItem *item);
    void ImportClicked(ContigurationListItem *item);
    void EditCustomPathsClicked(ContigurationListItem *item);

   public Q_SLOTS:
    void aboutVkConfig(bool checked);
    void toolsVulkanInfo(bool checked);
    void toolsVulkanInstallation(bool checked);
    void toolsSetCustomPaths(bool checked);
    void toolsResetToDefault(bool checked);

    void helpShowHelp(bool checked);

    void addCustomPaths();  // Fired by menu

    void editorExpanded(QTreeWidgetItem *item);

    void launchItemExpanded(QTreeWidgetItem *item);
    void launchItemCollapsed(QTreeWidgetItem *item);
    void launchItemChanged(int index);
    void launchSetLogFile();
    void launchArgsEdited(const QString &new_text);
    void on_pushButtonLaunch_clicked();
    void on_pushButtonClearLog_clicked();

    void on_radioFully_clicked();
    void on_radioOverride_clicked();
    void on_checkBoxApplyList_clicked();
    void on_checkBoxPersistent_clicked();

    void on_pushButtonAppList_clicked();

    void profileItemChanged(QTreeWidgetItem *item, int column);
    void profileTreeChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void profileItemClicked(bool checked);
    void profileItemExpanded(QTreeWidgetItem *item);

    void OnConfigurationTreeClicked(QTreeWidgetItem *item, int column);
    void OnConfigurationSettingsTreeClicked(QTreeWidgetItem *item, int column);

    void on_pushButtonEditProfile_clicked();

    void standardOutputAvailable();                                 // stdout output is available
    void errorOutputAvailable();                                    // Layeroutput is available
    void processClosed(int exitCode, QProcess::ExitStatus status);  // app died
};
