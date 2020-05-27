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


#include "dlgvulkananalysis.h"
#include "ui_dlgvulkananalysis.h"

#include <stdlib.h>
#include <QProcess>
#include <QDir>
#include <QByteArray>
#include <QTextStream>

#include <QMessageBox>

dlgVulkanAnalysis::dlgVulkanAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgVulkanAnalysis)
    {
    ui->setupUi(this);
    }

dlgVulkanAnalysis::~dlgVulkanAnalysis()
    {
    delete ui;
    }


void dlgVulkanAnalysis::RunTool(void)
    {
    ui->envTable->clear();
    ui->cleanupTable->clear();
    ui->hardwareTable->clear();
    ui->instanceTable->clear();
    ui->lunarGSDKTable->clear();
    ui->executableTable->clear();
    ui->vkRuntimesTable->clear();
    ui->externalTestsTable->clear();
    ui->layerSettingsTable->clear();
    ui->explicitLayersTable->clear();
    ui->implicitLayersTable->clear();
    ui->logicalDevicesTable->clear();
    ui->physicalDevicesTable->clear();

    QProcess *via = new QProcess(this);
#ifdef __APPLE__
    via->setProgram("/usr/local/bin/vkvia");
#else
    via->setProgram("vkvia");
#endif    

    char cPath[128];
    printf("%s\n", getenv("PATH"));

    QString filePath = QDir::temp().path() + "/vkvia.json";

    // Remove the old one
    remove(filePath.toUtf8().constData());

    QStringList args;
    args << "--output_path" << QDir::temp().path();
    args << "--vkconfig_output";
    args << "--disable_cube_tests";
    via->setArguments(args);
    via->start();
    via->waitForFinished();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         QMessageBox msgBox;
         msgBox.setText(tr("Error running vkvia. Is your SDK up to date and installed properly?"));
         msgBox.exec();
         return;
        }

     QString jsonText = file.readAll();
     file.close();

     //////////////////////////////////////////////////////
     // Convert the text to a JSON document & validate it
     QJsonDocument jsonDoc;
     QJsonParseError parseError;
     jsonDoc = QJsonDocument::fromJson(jsonText.toUtf8(), &parseError);

     if(parseError.error != QJsonParseError::NoError) {
        QMessageBox msgBox;
        msgBox.setText(parseError.errorString());
        msgBox.exec();
        return;
        }

     if(jsonDoc.isEmpty() || jsonDoc.isNull())
         return;

    /////////////////////////////////////////////////////////
    // Get the instance version and set that to the header
    QString output;
    QJsonObject jsonObject = jsonDoc.object();


    ///////////////////////////////////// System Info
    // Get the extensions object and process it's members
    QJsonValue environmentValue = jsonObject.value(QString(tr("Environment")));
    QJsonObject environmentObject = environmentValue.toObject();
    LoadTable(environmentObject, ui->envTable);

    QJsonValue hardwareValue = jsonObject.value(QString(tr("Hardware")));
    QJsonObject hardwareObject = hardwareValue.toObject();
    LoadTable(hardwareObject, ui->hardwareTable);

    QJsonValue executableValue = jsonObject.value(QString(tr("Executable Info")));
    QJsonObject executableObject = executableValue.toObject();
    LoadTable(executableObject, ui->executableTable);

    QJsonValue vkDriverInfo = jsonObject.value(QString(tr("Vulkan Driver Info")));
    QJsonObject vkDriverObject = vkDriverInfo.toObject();
    LoadTable(vkDriverObject, ui->vkDriverInfoTable);

    QJsonValue vkRunTimeValue = jsonObject.value(QString(tr("Vulkan Runtimes")));
    QJsonObject vkRunTimeObject = vkRunTimeValue.toObject();
    LoadTable(vkRunTimeObject, ui->vkRuntimesTable);

    QJsonValue lunarGSDKValue = jsonObject.value(QString(tr("LunarG Vulkan SDKs")));
    QJsonObject lunarGSDKObject = lunarGSDKValue.toObject();
    LoadTable(lunarGSDKObject, ui->lunarGSDKTable);

    QJsonValue vkImplicitValue = jsonObject.value(QString(tr("Vulkan Implicit Layers")));
    QJsonObject vkImplicitObject = vkImplicitValue.toObject();
    LoadTable(vkImplicitObject, ui->implicitLayersTable);

    QJsonValue vkExplicitValue = jsonObject.value(QString(tr("Vulkan Explicit Layers")));
    QJsonObject vkExplicitObject = vkExplicitValue.toObject();
    LoadTable(vkExplicitObject, ui->explicitLayersTable);

    QJsonValue vkLayerSettingsValue = jsonObject.value(QString(tr("Vulkan Layer Settings File")));
    QJsonObject vkLayerSettingsObject = vkLayerSettingsValue.toObject();
    LoadTable(vkLayerSettingsObject, ui->layerSettingsTable);

    /////////////////////////// Vulkan API Calls
    QJsonValue instanceValue = jsonObject.value(QString(tr("Instance")));
    QJsonObject instanceObject = instanceValue.toObject();
    LoadTable(instanceObject, ui->instanceTable);

    QJsonObject devicesObject = jsonObject.value(QString(tr("Physical Devices"))).toObject();
    LoadTable(devicesObject, ui->physicalDevicesTable);

    QJsonObject logicalObject = jsonObject.value(QString(tr("Logical Devices"))).toObject();
    LoadTable(logicalObject, ui->logicalDevicesTable);

    QJsonObject cleanupObject = jsonObject.value(QString(tr("Cleanup"))).toObject();
    LoadTable(cleanupObject, ui->cleanupTable);

    /////////////////////////////////// External Tests
    QJsonValue cubeValue = jsonObject.value(QString(tr("Cube"))).toObject();
    QJsonObject cubeObject = cubeValue.toObject();
    if(!cubeObject.isEmpty())
        LoadTable(cubeObject, ui->externalTestsTable);
    else
    {
        ui->externalTestsTable->setRowCount(1);
        ui->externalTestsTable->setColumnCount(1);
        ui->externalTestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->externalTestsTable->setShowGrid(false);
        QTableWidgetItem *pItem = new QTableWidgetItem();
        pItem->setText(tr("No SDK found by VIA, skipping test section"));
        ui->externalTestsTable->setItem(0, 0, pItem);
    }

    show();
    }


void dlgVulkanAnalysis::LoadTable(QJsonObject& jsonParent, QTableWidget* pTable)
    {
    // How many items does this object contain?
    int nHowManyRows = jsonParent.size();
    pTable->setRowCount(nHowManyRows);

    // Get the first item in the table, and see how many items it has. This
    // will be the number of columns
    QJsonValue rowValue = jsonParent.value("0");
    QJsonObject rowObject = rowValue.toObject();
    int nHowManyCols = rowObject.size();
    pTable->setColumnCount(nHowManyCols);

    pTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    // Now just iterate and fill in the cells
    for(int row = 0; row < nHowManyRows; row++) {
        rowValue = jsonParent.value(QString().asprintf("%d", row));
        rowObject = rowValue.toObject();

        for(int col = 0; col < nHowManyCols; col++) {
            QJsonValue colValue = rowObject.value(QString().asprintf("%d", col));
            QString text = colValue.toString();
            QTableWidgetItem *pItem = new QTableWidgetItem;
            pItem->setText(text);
            pTable->setItem(row, col, pItem);
        }
    }
}
