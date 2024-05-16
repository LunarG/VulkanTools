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

#include "dialog_vulkan_analysis.h"

#include "../vkconfig_core/platform.h"

#include <stdlib.h>
#include <QProcess>
#include <QDir>
#include <QByteArray>
#include <QTextStream>
#include <QMessageBox>
#include <QJsonDocument>

VulkanAnalysisDialog::VulkanAnalysisDialog(QWidget *parent) : QDialog(parent), ui(new Ui::dialog_vulkan_analysis) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Hide the test widget as no test is actually run.
    // This may be added back again later.
    ui->tabWidget->removeTab(2);

    Run();
}

void VulkanAnalysisDialog::Run() {
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
#if VKC_PLATFORM == VKC_PLATFORM_MACOS
    via->setProgram("/usr/local/bin/vkvia");
#else
    via->setProgram("vkvia");
#endif

    const QString &file_Path = QDir::temp().path() + "/vkvia.json";

    // Remove the old one
    remove(file_Path.toUtf8().constData());

    QStringList args;
    args << "--output_path" << QDir::temp().path();
    args << "--vkconfig_output";
    args << "--disable_cube_tests";
    via->setArguments(args);
    via->start();
    via->waitForFinished();

    QFile file(file_Path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msgBox;
        msgBox.setText(tr("Error running vkvia. Is your SDK up to date and installed properly?"));
        msgBox.exec();
        return;
    }

    QString json_text = file.readAll();
    file.close();

    //////////////////////////////////////////////////////
    // Convert the text to a JSON document & validate it
    QJsonParseError parse_error;
    QJsonDocument json_document = QJsonDocument::fromJson(json_text.toUtf8(), &parse_error);

    if (parse_error.error != QJsonParseError::NoError) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Cannot parse vkvia output.");
        msgBox.setText(parse_error.errorString());
        msgBox.exec();
        return;
    }

    if (json_document.isEmpty() || json_document.isNull()) return;

    /////////////////////////////////////////////////////////
    // Get the instance version and set that to the header
    QString output;
    const QJsonObject &json_object = json_document.object();

    ///////////////////////////////////// System Info
    // Get the extensions object and process it's members
    QJsonValue environmentValue = json_object.value(QString(tr("Environment")));
    QJsonObject environmentObject = environmentValue.toObject();
    LoadTable(environmentObject, ui->envTable);

    QJsonValue hardwareValue = json_object.value(QString(tr("Hardware")));
    QJsonObject hardwareObject = hardwareValue.toObject();
    LoadTable(hardwareObject, ui->hardwareTable);

    QJsonValue executableValue = json_object.value(QString(tr("Executable Info")));
    QJsonObject executableObject = executableValue.toObject();
    LoadTable(executableObject, ui->executableTable);

    QJsonValue vkDriverInfo = json_object.value(QString(tr("Vulkan Driver Info")));
    QJsonObject vkDriverObject = vkDriverInfo.toObject();
    LoadTable(vkDriverObject, ui->vkDriverInfoTable);

    QJsonValue vkRunTimeValue = json_object.value(QString(tr("Vulkan Runtimes")));
    QJsonObject vkRunTimeObject = vkRunTimeValue.toObject();
    LoadTable(vkRunTimeObject, ui->vkRuntimesTable);

    QJsonValue lunarGSDKValue = json_object.value(QString(tr("Vulkan SDKs")));
    QJsonObject lunarGSDKObject = lunarGSDKValue.toObject();
    LoadTable(lunarGSDKObject, ui->lunarGSDKTable);

    QJsonValue vkImplicitValue = json_object.value(QString(tr("Vulkan Implicit Layers")));
    QJsonObject vkImplicitObject = vkImplicitValue.toObject();
    LoadTable(vkImplicitObject, ui->implicitLayersTable);

    QJsonValue vkExplicitValue = json_object.value(QString(tr("Vulkan Explicit Layers")));
    QJsonObject vkExplicitObject = vkExplicitValue.toObject();
    LoadTable(vkExplicitObject, ui->explicitLayersTable);

    QJsonValue vkLayerSettingsValue = json_object.value(QString(tr("Vulkan Layer Settings File")));
    QJsonObject vkLayerSettingsObject = vkLayerSettingsValue.toObject();
    LoadTable(vkLayerSettingsObject, ui->layerSettingsTable);

    /////////////////////////// Vulkan API Calls
    QJsonValue instanceValue = json_object.value(QString(tr("Instance")));
    QJsonObject instanceObject = instanceValue.toObject();
    LoadTable(instanceObject, ui->instanceTable);

    QJsonObject devicesObject = json_object.value(QString(tr("Physical Devices"))).toObject();
    LoadTable(devicesObject, ui->physicalDevicesTable);

    QJsonObject logicalObject = json_object.value(QString(tr("Logical Devices"))).toObject();
    LoadTable(logicalObject, ui->logicalDevicesTable);

    QJsonObject cleanupObject = json_object.value(QString(tr("Cleanup"))).toObject();
    LoadTable(cleanupObject, ui->cleanupTable);

    /////////////////////////////////// External Tests
    QJsonValue cubeValue = json_object.value(QString(tr("Cube"))).toObject();
    QJsonObject cubeObject = cubeValue.toObject();
    if (!cubeObject.isEmpty())
        LoadTable(cubeObject, ui->externalTestsTable);
    else {
        ui->externalTestsTable->setRowCount(1);
        ui->externalTestsTable->setColumnCount(1);
        ui->externalTestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->externalTestsTable->setShowGrid(false);
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(tr("No SDK found by VIA, skipping test section"));
        ui->externalTestsTable->setItem(0, 0, item);
    }

    show();
}

void VulkanAnalysisDialog::LoadTable(QJsonObject &json_parent, QTableWidget *table) {
    // How many items does this object contain?
    int nHowManyRows = json_parent.size();
    table->setRowCount(nHowManyRows);

    // Get the first item in the table, and see how many items it has. This
    // will be the number of columns
    QJsonValue rowValue = json_parent.value("0");
    QJsonObject rowObject = rowValue.toObject();
    int nHowManyCols = rowObject.size();
    table->setColumnCount(nHowManyCols);

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Now just iterate and fill in the cells
    for (int row = 0; row < nHowManyRows; row++) {
        rowValue = json_parent.value(QString().asprintf("%d", row));
        rowObject = rowValue.toObject();

        for (int col = 0; col < nHowManyCols; col++) {
            QJsonValue colValue = rowObject.value(QString().asprintf("%d", col));
            QString text = colValue.toString();
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(text);
            table->setItem(row, col, item);
        }
    }
}
