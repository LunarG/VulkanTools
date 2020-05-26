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

#include "vulkanconfiguration.h"
#include "settingseditor.h"
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
    CSettingsEditor          settingsEditor;

    void CheckAppListState(void);
    void LoadProfileList(void);

    void ChangeActiveProfile(CProfileDef *pNewProfile);

    virtual void closeEvent(QCloseEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

    dlgVulkanAnalysis *pVKVia;
    dlgVulkanInfo     *pVulkanInfo;
    dlgLayerOutput    *pTestEnv;
    dlgHelp           *pDlgHelp;

private:
    Ui::MainWindow *ui;

    // This is the number of canned profiles. It is used to sepearate
    // the canned profiles from the user profiles in the list box
    int CANNED_PROFILE_COUNT;

    CProfileListItem    *pLastSelectedProfileItem;
    CProfileListItem* GetCheckedItem(void);

    void updateGetStartedStatus(QString qsText);
    void updateActivateButtonState(void);

public Q_SLOTS:
    void fileExit(bool bChecked);
    void fileHistory(bool bChecked);

    void aboutVkConfig(bool bChecked);
    void toolsVulkanInfo(bool bChecked);
    void toolsVulkanInstallation(bool bChecked);
    void toolsVulkanTestApp(bool bChecked);
    void toolsVulkanAPIDump(bool bChecked);
    void toolsSetCustomPaths(bool bChecked);
    void helpShowHelp(bool bChecked);

    void selectedProfileChanged(void);

    void addCustomPaths();    // Fired by menu

    void on_radioFully_clicked(void);
    void on_radioOverride_clicked(void);
    void on_checkBoxApplyList_clicked(void);
    void on_checkBoxPersistent_clicked(void);

    void on_pushButtonAppList_clicked(void);

    void profileItemChanged(QTreeWidgetItem *pItem, int nCol);
    void profileItemHighlighted(QTreeWidgetItem *pItem, int nCol);
    void profileTreeChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious);
    void profileItemClicked(bool bChecked);

    void on_pushButtonEdit_clicked(void);
    void on_pushButtonNewProfile_clicked(void);
    void on_pushButtonRemove_clicked(void);
    void on_pushButtonActivate_clicked(void);
    void on_pushButtonLaunch_clicked(void);
};

