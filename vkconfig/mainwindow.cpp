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




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    {
    ui->setupUi(this);

    // This loads all the layer information and current settings.
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();
    pVulkanConfig->loadAppSettings();
    pVulkanConfig->loadProfiles();

    // Load list of profiles
    ui->listWidgetProfiles->addItem("** No Layer Profile Active **");
    for(int i = 0; i < pVulkanConfig->profileList.size(); i++)
        ui->listWidgetProfiles->addItem(pVulkanConfig->profileList[i]->profileName);


    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(fileExit(bool)));
    connect(ui->actionHistory, SIGNAL(triggered(bool)), this, SLOT(fileHistory(bool)));

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));

    connect(ui->actionCustom_Layer_Paths, SIGNAL(triggered(bool)), this, SLOT(toolsSetCustomPaths(bool)));

    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionSet_Test_Application, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanTestApp(bool)));
//    connect(ui->layerTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(layerTree_itemClicked(QTreeWidgetItem*, int)));

//    for(int i = 0; i < pVulkanConfig->profileList.size(); i++)
//        ui->listWidgetStacks->addItem(pVulkanConfig->profileList[i]->profileName);

    /*
    ui->listWidgetStacks->item(pVulkanConfig->nActiveProfile)->setBackground(Qt::green);

    */
    }

MainWindow::~MainWindow()
    {
    delete ui;
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
/// Edit the list of apps tied to this profile
void MainWindow::on_pushButtonAppList_clicked(void)
    {
    dlgCreateAssociation dlg(this, ui->listWidgetProfiles->currentRow()-1);
    dlg.exec();
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButtonEditProfile_clicked
/// Just resave the list anytime we go into the editor
void MainWindow::on_pushButtonEditProfile_clicked(void)
    {
    dlgProfileEditor dlg(this);
    dlg.exec();

    pVulkanConfig->saveProfiles();
    }






//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_pushButtonActivateProfile_clicked()
    {
//    if(ui->listWidgetStacks->count() == 0)
//        return;

//    QMessageBox msg;
//    msg.setInformativeText(tr("Set this profile as the default for all Vulkan Applications?"));
//    msg.setText(tr("Set default profile"));
//    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//    msg.setDefaultButton(QMessageBox::Yes);
//    if(msg.exec() == QMessageBox::No)
//        return;


//    pVulkanConfig->nActiveProfile = ui->listWidgetStacks->currentRow();
//    ui->listWidgetStacks->item(pVulkanConfig->nActiveProfile)->setBackground(Qt::white);
//    pVulkanConfig->nActiveProfile = ui->listWidgetStacks->currentRow();
//    ui->listWidgetStacks->item(pVulkanConfig->nActiveProfile)->setBackground(Qt::green);
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


//////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::layerTree_itemClicked
/// \param item
/// \param column
/// Something was enabled/disabled/etc.
void MainWindow::layerTree_itemClicked(QTreeWidgetItem *item, int column)
    {
    if(column == 1) { // Something made active
//        QTreeWidgetItemWithLayer* pLayerRep = static_cast<QTreeWidgetItemWithLayer*>(item);
//        pLayerRep->pLayerRepBackPointer->bUse = (Qt::Checked == item->checkState(column));
//        printf("checked = %d\n",pLayerRep->pLayerRepBackPointer->bUse );




        return;
        }


    if(column == 3) { // Something disabled



        }
    }

