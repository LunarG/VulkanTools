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

#include <QVector>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QLabel>
#include <QRadioButton>
#include <QShowEvent>
#include <QResizeEvent>
#include <QProcess>

#include "configurator.h"
#include "settingstreemanager.h"
#include "dlgvulkananalysis.h"
#include "dlgvulkaninfo.h"
#include "dlghelp.h"

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

    dlgVulkanAnalysis *pVKVia;
    dlgVulkanInfo *pVulkanInfo;
    dlgHelp *pDlgHelp;

   private:
    Ui::MainWindow *ui;

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

    void RemoveClicked(ContigurationListItem *pItem);
    void RenameClicked(ContigurationListItem *pItem);
    void NewClicked();
    void DuplicateClicked(ContigurationListItem *pItem);
    void ExportClicked(ContigurationListItem *pItem);
    void ImportClicked(ContigurationListItem *pItem);
    void EditClicked(ContigurationListItem *pItem);
    void RestoreClicked(ContigurationListItem *pItem);
    void EditCustomPathsClicked(ContigurationListItem *pItem);

   public Q_SLOTS:
    void aboutVkConfig(bool bChecked);
    void toolsVulkanInfo(bool bChecked);
    void toolsVulkanInstallation(bool bChecked);
    void toolsSetCustomPaths(bool bChecked);
    void toolsResetToDefault(bool bChecked);

    void helpShowHelp(bool bChecked);

    void addCustomPaths();  // Fired by menu

    void editorExpanded(QTreeWidgetItem *pItem);

    void launchItemExpanded(QTreeWidgetItem *pItem);
    void launchItemCollapsed(QTreeWidgetItem *pItem);
    void launchItemChanged(int nIndex);
    void launchSetLogFile();
    void launchArgsEdited(const QString &newText);
    void on_pushButtonLaunch_clicked(void);
    void on_pushButtonClearLog_clicked(void);

    void on_radioFully_clicked();
    void on_radioOverride_clicked();
    void on_checkBoxApplyList_clicked();
    void on_checkBoxPersistent_clicked();

    void on_pushButtonAppList_clicked();

    void profileItemChanged(QTreeWidgetItem *pItem, int nCol);
    void profileTreeChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious);
    void profileItemClicked(bool bChecked);
    void profileItemExpanded(QTreeWidgetItem *pItem);

    void on_pushButtonEditProfile_clicked();

    void standardOutputAvailable();                                 // stdout output is available
    void errorOutputAvailable();                                    // Layeroutput is available
    void processClosed(int exitCode, QProcess::ExitStatus status);  // app died
};
