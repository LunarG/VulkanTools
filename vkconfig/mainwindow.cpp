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

#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QFrame>
#include <unistd.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgabout.h"
#include "dlglayeroutput.h"
#include "dlgvulkananalysis.h"
#include "dlgvulkaninfo.h"
#include "dlgcreateprofile.h"
#include "dlgcreateassociation.h"
#include "dlghistory.h"
#include "dlgprofileeditor.h"
#include "dlgcustompaths.h"


// This is the number of canned profiles. It is used to sepearate
// the canned profiles from the user profiles in the list box, there
// is even a "fake" profile used as a spacer in the vulkanconfiguration
// object at this position. If we add more than 5 canned profiles,
// this must be changed.
#define CANNED_PROFILE_COUNT    5


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    {
    ui->setupUi(this);

    // This loads all the layer information and current settings.
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();
    pVulkanConfig->loadAppSettings();
    if(!pVulkanConfig->loadSavedProfiles())
        pVulkanConfig->loadDefaultProfiles();
    pVulkanConfig->loadAppList();

    pSettingsEditor = new CSettingsEditor();

    // Load list of profiles
    ui->listWidgetProfiles->blockSignals(true);
    for(int i = 0; i < pVulkanConfig->profileList.size(); i++) {
        if(i == CANNED_PROFILE_COUNT) {
            QFrame *line;
            line = new QFrame(ui->listWidgetProfiles);
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            line->setFrameRect(QRect(90, 8, 10, 20));

            QListWidgetItem *pItem = new QListWidgetItem(pVulkanConfig->profileList[CANNED_PROFILE_COUNT]->profileName, ui->listWidgetProfiles);
            //pItem->setFlags(pItem->flags() & ~Qt::ItemIsSelectable);
            pItem->setFlags(pItem->flags() & ~Qt::ItemIsEnabled);

            ui->listWidgetProfiles->addItem(pItem);
            ui->listWidgetProfiles->setItemWidget(pItem, line);
            }
        else {
            ui->listWidgetProfiles->addItem(pVulkanConfig->profileList[i]->profileName);
            ui->listWidgetProfiles->item(i)->setCheckState(Qt::Unchecked);
            }
        }
    ui->listWidgetProfiles->blockSignals(false);

 //   ui->listWidgetProfiles->setStyleSheet( "QListWidget::item[separator=""true""] { border-bottom: 1px solid black; }" );


    //    ui->listWidgetProfiles->setProperty("separator", true);

   // ui->listWidgetProfiles->item(0)->setCheckState(Qt::Checked);

    connect(ui->listWidgetProfiles, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(profileItemChanged(QListWidgetItem*)));
    connect(ui->listWidgetProfiles, SIGNAL(currentRowChanged(int)), this, SLOT(currentProfileRowChanged(int)));
    connect(ui->listWidgetProfiles, SIGNAL(itemSelectionChanged()), this, SLOT(selectedProfileChanged()));

    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(fileExit(bool)));
    connect(ui->actionHistory, SIGNAL(triggered(bool)), this, SLOT(fileHistory(bool)));

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));

    connect(ui->actionCustom_Layer_Paths, SIGNAL(triggered(bool)), this, SLOT(toolsSetCustomPaths(bool)));

    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionSet_Test_Application, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanTestApp(bool)));
//    connect(ui->layerTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(layerTree_itemClicked(QTreeWidgetItem*, int)));


    ///////////////////////////////////////////////
    // Final check - if there are no apps, disable the profiles list
    if(pVulkanConfig->appList.length() == 0) {
        this->setWindowTitle("Vulkan Configurator (Warning, no app list specified)");
        ui->pushButtonAppList->setStyleSheet("QPushButton { color: red;}");
        }
    else {
        this->setWindowTitle("Vulkan Configurator");
        ui->pushButtonAppList->setStyleSheet("QPushButton { color: black;}");
        }
    }

MainWindow::~MainWindow()
    {
    delete ui;
    }


void MainWindow::checkAppListState(void)
    {
    // Final check - if there are no apps, disable the profiles list
    if(pVulkanConfig->appList.length() == 0) {
        this->setWindowTitle("Vulkan Configurator (Warning, no app list specified)");
        ui->pushButtonAppList->setStyleSheet("QPushButton { color: red;}");
        }
    else {
        this->setWindowTitle("Vulkan Configurator");
        ui->pushButtonAppList->setStyleSheet("QPushButton { color: black;}");
        }
    }

/////////////////////////////////////////////////////
// Unused flag, just display the about Qt dialog
void MainWindow::aboutVkConfig(bool bChecked)
{
    (void)bChecked;
    dlgAbout dlg(this);
    dlg.exec();

}


void MainWindow::toolsVulkanInfo(bool bChecked)
{
    (void)bChecked;
    dlgVulkanInfo dlg(this);
    dlg.exec();
}
void MainWindow::toolsVulkanInstallation(bool bChecked)
{
    (void)bChecked;
    dlgVulkanAnalysis dlg(this);
    dlg.exec();
}


void MainWindow::toolsVulkanTestApp(bool bChecked)
{
    (void)bChecked;
    dlgLayerOutput dlg(this);
    dlg.exec();
}

void MainWindow::on_pushButtonLaunch_clicked()
    {
    dlgLayerOutput dlg(this);
    dlg.bAutoLaunch = true;
    dlg.exec();
    }


void MainWindow::fileExit(bool bChecked)
{
    (void)bChecked;
    close();
}


void MainWindow::fileHistory(bool bChecked)
{
    (void)bChecked;
    dlgHistory dlg(this);
    dlg.exec();
}


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonAppList_clicked
/// Edit the list of apps that can be filtered.
void MainWindow::on_pushButtonAppList_clicked(void)
    {
    dlgCreateAssociation dlg(this);
    dlg.exec();
    pVulkanConfig->saveAppList();
    checkAppListState();
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonEditProfile_clicked
/// Just resave the list anytime we go into the editor
void MainWindow::on_pushButtonEditProfile_clicked(void)
    {
    int nSelection = ui->listWidgetProfiles->currentRow();
    if(nSelection == 0) {
        QMessageBox msg;
        msg.setInformativeText(tr("Select a specific profile to clone."));
        msg.setText(tr("Edit profile"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
        }


    dlgProfileEditor dlg(this, pVulkanConfig->profileList[nSelection]);
    dlg.exec();

    pVulkanConfig->saveProfiles();
    }

////////////////////////////////////////////////////////////////////////////////
// Create a new blank profile
void MainWindow::on_pushButtonNewProfile_clicked(void)
    {
    dlgProfileEditor dlg(this, nullptr);
    dlg.exec();
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonCustomPaths_clicked
/// Allow addition or removal of custom layer paths. Afterwards reset the list
/// of loaded layers, but only if something was changed.
void MainWindow::on_pushButtonCustomPaths_clicked()
    {
    dlgCustomPaths dlg(this);
    dlg.exec();
    }

void MainWindow::toolsSetCustomPaths(bool bChecked)
    {
    (void)bChecked;
    on_pushButtonCustomPaths_clicked();
    }


void MainWindow::profileItemChanged(QListWidgetItem *item)
    {
    // If an item is checked, well, a few things need to happen.
    if(item->checkState() == Qt::Checked)
        {
        // First, can we check things?
        if(pVulkanConfig->appList.length() == 0) { // Big nope
            item->setCheckState(Qt::Unchecked);
            QMessageBox warning;
            warning.setInformativeText(tr("You cannot activate a profile with an empty applicaton list."));
            warning.setText(tr("App list is empty"));
            warning.setStandardButtons(QMessageBox::Ok);
            warning.exec();
            return;
            }

        // Okay, checks are mutually exclusive (but rado buttons are ugly)
        // so only one profile can actually be checked
        for(int i = 0; i < ui->listWidgetProfiles->count(); i++)
            if(item != ui->listWidgetProfiles->item(i) && i != CANNED_PROFILE_COUNT)
                ui->listWidgetProfiles->item(i)->setCheckState(Qt::Unchecked);



        // MakeThisProfileCurrent();
        return;
        }

    // Well then, there is no current profile
    // RemoveCurrentProfile();
    }


///////////////////////////////////////////////////////////////////
void MainWindow::selectedProfileChanged(void)
    {
    ui->pushButtonEditProfile->setEnabled(true);

    int nRow = ui->listWidgetProfiles->currentRow();
    if(nRow == CANNED_PROFILE_COUNT) {
        // This can't be selected
        ui->listWidgetProfiles->setCurrentRow(-1);
        return;
        }

    pVulkanConfig->nActiveProfile = nRow;
    CProfileDef* pProfile = pVulkanConfig->getActiveProfile();
    if(pProfile->readOnly)
        ui->pushButtonEditProfile->setText("Clone");
    else
        ui->pushButtonEditProfile->setText("Edit");
    }


void MainWindow::currentProfileRowChanged(int row)
    {


    // Display editor for profile
   //   pSettingsEditor->CreateGUI(ui->tabWidget, pVulkanConfig->explicitLayers[row]->layerSettings);
 //   ui->tabWidget->update();

    }
