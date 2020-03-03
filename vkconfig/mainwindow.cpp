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
#include "dlghistory.h"
#include "dlgcustompaths.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // This loads all the layer information and current settings.
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();
    pVulkanConfig->LoadAppSettings();
    pVulkanConfig->LoadLayerConfiguration();

    loadAllFoundLayers(pVulkanConfig->explicitLayers);
    loadAllFoundLayers(pVulkanConfig->implicitLayers);


    ui->statusbar->showMessage("No layers active", 1000000);

    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(fileExit(bool)));
    connect(ui->actionHistory, SIGNAL(triggered(bool)), this, SLOT(fileHistory(bool)));

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));
    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionSet_Test_Application, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanTestApp(bool)));


    for(int i = 0; i < pVulkanConfig->profileList.size(); i++)
        ui->listWidgetStacks->addItem(pVulkanConfig->profileList[i]->profileName);

    /*
    ui->listWidgetStacks->item(pVulkanConfig->nActiveProfile)->setBackground(Qt::green);

    QTreeWidgetItem *pItem = new QTreeWidgetItem();
    pItem->setText(0, "Standard Validation");
    ui->treeWidgetStacks->addTopLevelItem(pItem);

    QTreeWidgetItem *pChild = new QTreeWidgetItem();
    pChild->setText(0, "LunarG: Standard Validation");
    pItem->addChild(pChild);
    pChild = new QTreeWidgetItem();
    pChild->setText(0, "Khronos: Validation");
    pItem->addChild(pChild);

    ui->listWidgetStackApps->addItem("vkcube - API Usage Validation");
    ui->listWidgetStackApps->addItem("Lunartic - Synchronization and Best Practices");
    ui->listWidgetStackApps->addItem("Doom 3 - Performance Tuning");

    pItem = new QTreeWidgetItem();
    pItem->setText(0, "Best Practices Suite");
    ui->treeWidgetStacks->addTopLevelItem(pItem);

    pChild = new QTreeWidgetItem();
    pChild->setText(0, "MoltenVK - Molten BP");
    pItem->addChild(pChild);
    pChild = new QTreeWidgetItem();
    pChild->setText(0, "Khronos: Best Practices");
    pItem->addChild(pChild);

    pItem = new QTreeWidgetItem();
    pItem->setText(0, "LunarG: Standard Validation");
    ui->treeWidgetStacks->addTopLevelItem(pItem);

    pChild = new QTreeWidgetItem();
    pChild->setText(0, "LunarG: Standard Validation");
    pItem->addChild(pChild);
    */

}

MainWindow::~MainWindow()
{
    qDeleteAll(layers.begin(), layers.end());
    layers.clear();
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

void MainWindow::on_pushButtonAddAssociation_clicked()
{
    dlgcreateprofile dlg(this);
    dlg.exec();


}




///////////////////////////////////////////////////////////////////////////////
void MainWindow::loadAllFoundLayers(QVector <CLayerFile*> &layerFile)
    {
    QTreeWidgetItem *pHeader = ui->layerTree->headerItem();

    pHeader->setText(0, "Layer Name");
    pHeader->setText(1, "Active");
    pHeader->setText(2, "Implicit?");
    pHeader->setText(3, "Disabled");
    pHeader->setText(4, "Custom Paths?");


    QString out;
    for(int i = 0; i < layerFile.size(); i++)
       {
       TLayerRepresentations *pLayer = new TLayerRepresentations;
       pLayer->pLayerFileInfo = layerFile[i];
       pLayer->bUse = false;
       pLayer->bDisabled = false;
       pLayer->bExplicit = true;
       pLayer->pTreeItem = new QTreeWidgetItem();
       layers.push_back(pLayer);

       pLayer->pTreeItem->setText(0, layerFile[i]->name);
       pLayer->pTreeItem->setFlags(pLayer->pTreeItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
       pLayer->pTreeItem->setCheckState(1, Qt::Unchecked);

       if(layerFile[i]->layerType == LAYER_TYPE_EXPLICIT)
           pLayer->pTreeItem->setText(2, "No");
       else
        pLayer->pTreeItem->setText(2, "Yes");

       pLayer->pTreeItem->setCheckState(3, Qt::Unchecked);
       ui->layerTree->addTopLevelItem(pLayer->pTreeItem);

       QTreeWidgetItem *pChild = new QTreeWidgetItem();
       pChild->setText(0, layerFile[i]->description);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItem();
       out.sprintf("Path: %s", layerFile[i]->library_path.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItem();
       out.sprintf("API Version: %s", layerFile[i]->api_version.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItem();
       out.sprintf("Implementation Version: %s", layerFile[i]->implementation_version.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);

       pChild = new QTreeWidgetItem();
       out.sprintf("File format: %s", layerFile[i]->file_format_version.toUtf8().constData());
       pChild->setText(0, out);
       pLayer->pTreeItem->addChild(pChild);
       }


    ui->layerTree->resizeColumnToContents(0);
    ui->layerTree->resizeColumnToContents(1);
    ui->layerTree->resizeColumnToContents(2);
    }


void MainWindow::on_checkBoxHideUnused_clicked()
    {



    }


void MainWindow::on_pushButtonActivateProfile_clicked()
    {
    QMessageBox msg;
    msg.setInformativeText(tr("Set this profile as the default for all Vulkan Applications?"));
    msg.setText(tr("Set default profile"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if(msg.exec() == QMessageBox::No)
        return;

    ui->listWidgetStacks->item(pVulkanConfig->nActiveProfile)->setBackground(Qt::white);
    pVulkanConfig->nActiveProfile = ui->listWidgetStacks->currentRow();
    ui->listWidgetStacks->item(pVulkanConfig->nActiveProfile)->setBackground(Qt::green);
    }


void MainWindow::on_pushButtonCustomPaths_clicked()
    {
    dlgCustomPaths dlg(this);
    dlg.exec();


    }



void MainWindow::on_pushButtonCreateProfile_clicked()
    {
    QJsonObject     rootProfiles;
    QJsonObject     profiles;

    QJsonArray array;
    array.push_back(QJsonValue("VK_LAYER_LUNARG_override"));
    array.push_back(QJsonValue("VK_SOMETHING_else"));
    profiles.insert("My Profile", array);

    QJsonArray array2;
    array2.push_back(QJsonValue("VK_another layer"));
    array2.push_back(QJsonValue("VK_unknown_layer"));
    profiles.insert("Another silly name", array2);


    //QString("VK_LAYER_LUNARG_override"));
    //profile.insert("type", QString("GLOBAL"));



    QJsonDocument   jsonDoc(profiles);

/*
    lunarg_api_dump.use_spaces = TRUE
    lunarg_api_dump.no_addr = FALSE
    lunarg_api_dump.name_size = 32
    lunarg_api_dump.show_shader = FALSE
    lunarg_api_dump.detailed = TRUE
    lunarg_api_dump.flush = TRUE
    lunarg_api_dump.file = FALSE
    lunarg_api_dump.output_range = 0-0
    lunarg_api_dump.log_filename = vk_apidump.txt
    lunarg_api_dump.output_format = Text
    lunarg_api_dump.show_timestamp = FALSE
    lunarg_api_dump.show_types = TRUE
    lunarg_api_dump.indent_size = 4
    lunarg_api_dump.type_size = 0
*/

    /*
     * # VK_LAYER_KHRONOS_validation
khronos_validation.debug_action = VK_DBG_LAYER_ACTION_LOG_MSG
khronos_validation.log_filename = stdout
khronos_validation.report_flags = warn,error,perf
khronos_validation.disables =
khronos_validation.enables =
*/


    QFile file("./Test.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(jsonDoc.toJson());
     file.close();
    }

