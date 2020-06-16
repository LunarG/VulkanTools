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
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#pragma once
#include <QVector>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QLabel>
#include <QRadioButton>
#include <QShowEvent>
#include <QResizeEvent>

#include "vulkanconfiguration.h"
#include "settingstreemanager.h"
#include "dlgvulkananalysis.h"
#include "dlgvulkaninfo.h"
#include "dlglayeroutput.h"
#include "dlghelp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//////////////////////////////////////////////////
/// \brief The CProfileListItem class
/// This just allows me to associate a specific profile definition
/// with a list widget item.
class CProfileListItem : public QTreeWidgetItem
{
public:
    CProfileDef*    pProfilePointer;
    QRadioButton*   pRadioButton;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    CVulkanConfiguration*    pVulkanConfig;
    CSettingsTreeManager     settingsTreeManager;

    QProcess *pVulkanApp;       // Keeps track of the monitored app
    QFile    *pLogFile;          // Log file for layer output

    void LoadProfileList(void);
    void SetupLaunchTree(void);

    void ChangeActiveProfile(CProfileDef *pNewProfile);

    virtual void closeEvent(QCloseEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

    virtual bool eventFilter(QObject *target, QEvent *event) override;

    dlgVulkanAnalysis *pVKVia;
    dlgVulkanInfo     *pVulkanInfo;
    dlgLayerOutput    *pTestEnv;
    dlgHelp           *pDlgHelp;

private:
    Ui::MainWindow *ui;

    CProfileListItem    *pLastSelectedProfileItem;
    CProfileListItem* GetCheckedItem(void);

    QComboBox*          pLaunchAppsCombo;
    QLineEdit*          pLaunchArguments;
    QLineEdit*          pLaunchWorkingFolder;
    QLineEdit*          pLaunchLogFile;
    QPushButton*        pLuanchAppBrowseButton;
    QPushButton*        pLaunchWorkingFolderButton;
    QPushButton*        pLaunchLogFilebutton;

    void ResetLaunchOptions(void);

    void RemoveClicked(CProfileListItem *pItem);
    void RenameClicked(CProfileListItem *pItem);
    void NewClicked(void);
    void DuplicateClicked(CProfileListItem *pItem);
    void ExportClicked(CProfileListItem *pItem);
    void ImportClicked(CProfileListItem *pItem);
    void EditClicked(CProfileListItem *pItem);

public Q_SLOTS:
    void aboutVkConfig(bool bChecked);
    void toolsVulkanInfo(bool bChecked);
    void toolsVulkanInstallation(bool bChecked);
    void toolsVulkanTestApp(bool bChecked);
    void toolsVulkanAPIDump(bool bChecked);
    void toolsSetCustomPaths(bool bChecked);
    void toolsResetDefaultProfiles(bool bChecked);

    void helpShowHelp(bool bChecked);

    void addCustomPaths();    // Fired by menu

    void editorExpanded(QTreeWidgetItem *pItem);



    void launchItemExpanded(QTreeWidgetItem* pItem);
    void launchItemCollapsed(QTreeWidgetItem* pItem);
    void launchItemChanged(int nIndex);
    void launchAddProgram(void);
    void launchSetLogFile(void);
    void launchArgsEdited(const QString& newText);
    void on_pushButtonLaunch_clicked(void);
    void on_pushButtonClearLog_clicked(void);
    void on_pushButtonOpenLog_clicked(void);

    void on_radioFully_clicked(void);
    void on_radioOverride_clicked(void);
    void on_checkBoxApplyList_clicked(void);
    void on_checkBoxPersistent_clicked(void);

    void on_pushButtonAppList_clicked(void);

    void profileItemChanged(QTreeWidgetItem *pItem, int nCol);
    void profileTreeChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious);
    void profileItemClicked(bool bChecked);
    void profileItemExpanded(QTreeWidgetItem *pItem);


    void on_pushButtonEditProfile_clicked(void);

    void standardOutputAvailable(void);         // stdout output is available
    void errorOutputAvailable(void);            // Layeroutput is available
    void processClosed(int exitCode, QProcess::ExitStatus status);  // app died


};

