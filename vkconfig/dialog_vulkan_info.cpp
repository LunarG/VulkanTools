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

#include "dialog_vulkan_info.h"

#include "../vkconfig_core/util.h"
#include "../vkconfig_core/platform.h"

#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QMessageBox>
#include <QStringList>

#include <cstdio>
#include <cstdlib>
#include <cassert>

VulkanInfoDialog::VulkanInfoDialog(QWidget *parent) : QDialog(parent), ui(new Ui::dialog_vulkan_info) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    Run();
}

void VulkanInfoDialog::Run() {
    static const char *VULKAN_INFO_PATH[] = {
        "vulkaninfoSDK",              // PLATFORM_WINDOWS
        "vulkaninfo",                 // PLATFORM_LINUX
        "/usr/local/bin/vulkaninfo",  // PLATFORM_MACOS
        "N/A",                        // PLATFORM_ANDROID
    };
    static_assert(countof(VULKAN_INFO_PATH) == PLATFORM_COUNT,
                  "The tranlation table size doesn't match the enum number of elements");

    ui->treeWidget->clear();

    QProcess *vulkan_info = new QProcess(this);
    vulkan_info->setProgram(VULKAN_INFO_PATH[VKC_PLATFORM]);

    QString filePath = QDir::temp().path();

    QStringList args;
    args << "--vkconfig_output";
    args << filePath;

    // Wait... make sure we don't pick up the old one!
    filePath += "/vulkaninfo.json";
    remove(filePath.toUtf8().constData());

    // Lock and load...
    vulkan_info->setArguments(args);
    vulkan_info->start();
    vulkan_info->waitForFinished();

    // Check for the output file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msgBox;
        msgBox.setText("Error running vulkaninfo. Is your SDK up to date and installed properly?");
        msgBox.exec();
        return;
    }

    QString jsonText = file.readAll();
    file.close();

    //////////////////////////////////////////////////////
    // Convert the text to a JSON document & validate it
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonText.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Cannot parse vulkaninfo output.");
        msgBox.setText(parseError.errorString());
        msgBox.exec();
        return;
    }

    if (jsonDoc.isNull() || jsonDoc.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText(tr("Json document is empty!"));
        msgBox.exec();
        return;
    }

    /////////////////////////////////////////////////////////
    // Get the instance version and set that to the header
    QJsonObject jsonTopObject = jsonDoc.object();
    QJsonValue instance = jsonTopObject.value("Vulkan Instance Version");
    QString output = "Vulkan Instance Version: " + instance.toString();

    QTreeWidgetItem *header = ui->treeWidget->headerItem();
    header->setText(0, output);

    ////////////////////////////////////////////////////////////
    // Setp through each major section and parse.
    // All of these are the top layer nodes on the tree.
    QJsonValue rootObject = jsonTopObject.value("Instance Extensions");
    QTreeWidgetItem *parent_node = new QTreeWidgetItem();
    parent_node->setText(0, "Instance Extensions");
    ui->treeWidget->addTopLevelItem(parent_node);
    BuildExtensions(rootObject, parent_node);

    rootObject = jsonTopObject.value("Layer Properties");
    parent_node = new QTreeWidgetItem();
    BuildLayers(rootObject, parent_node);

    rootObject = jsonTopObject.value("Presentable Surfaces");
    parent_node = new QTreeWidgetItem();
    BuildSurfaces(rootObject, parent_node);

    rootObject = jsonTopObject.value("Device Groups");
    parent_node = new QTreeWidgetItem();
    BuildGroups(rootObject, parent_node);

    rootObject = jsonTopObject.value("Device Properties and Extensions");
    parent_node = new QTreeWidgetItem();
    parent_node->setText(0, "Device Properties and Extensions");
    BuildDevices(rootObject, parent_node);

    show();
}

/// Many large sections are generic enough to simply parse and construct a tree,
/// without the need for any special formatting or extra text that is not in the
/// json file.
void VulkanInfoDialog::TraverseGenericProperties(QJsonValue &parentJson, QTreeWidgetItem *pParentTreeItem) {
    QJsonObject parentObject = parentJson.toObject();
    int listSize = parentObject.size();
    QStringList fields = parentObject.keys();

    for (int field = 0; field < listSize; field++) {
        QJsonValue fieldValue = parentObject.value(fields[field]);

        if (!fieldValue.isArray()) {
            // Is it a single child or does it have children? If it has children, recurse
            QJsonObject childObject = fieldValue.toObject();
            if (childObject.size() > 0) {
                QTreeWidgetItem *pNewChild = new QTreeWidgetItem();
                pNewChild->setText(0, fields[field]);
                pParentTreeItem->addChild(pNewChild);
                TraverseGenericProperties(fieldValue, pNewChild);
                continue;
            }

            QTreeWidgetItem *pItem = new QTreeWidgetItem();
            pItem->setText(0, QString().asprintf("%s = %s", fields[field].toUtf8().constData(),
                                                 fieldValue.toVariant().toString().toUtf8().constData()));
            pParentTreeItem->addChild(pItem);
        } else {
            // Add array list
            QJsonArray jsonArray = fieldValue.toArray();
            QTreeWidgetItem *pArrayParent = new QTreeWidgetItem();
            pArrayParent->setText(0, QString().asprintf("%s: count = %d", fields[field].toUtf8().constData(), jsonArray.size()));
            pParentTreeItem->addChild(pArrayParent);

            // The array is just a list of values. No children.
            for (int i = 0; i < jsonArray.size(); i++) {
                QTreeWidgetItem *pChild = new QTreeWidgetItem();
                // This looks weird... but integer fields will not conver to strings directly. However, if I
                // make them a variant first, then they do. I'd call this a Qt bug, but the word around is
                // a better choice than to wait for Qt to fix it and then have to require that version of Qt
                // to build this tool... RSW
                pChild->setText(0, jsonArray[i].toVariant().toString().toUtf8().constData());
                pArrayParent->addChild(pChild);
            }
        }
    }
}

/// Populate the a subtree with extension names. Extensions also report their
/// spec version, so some extra text is needed, and thus the need for a special
/// function as opposed to just calling TraverseGenericProperties()
void VulkanInfoDialog::BuildExtensions(QJsonValue &jsonValue, QTreeWidgetItem *pRoot) {
    QString output;
    QJsonObject extensionObject = jsonValue.toObject();
    int nObjectSize = extensionObject.size();

    // Add all the extensions and thier version
    QStringList keys = extensionObject.keys();

    for (int i = 0; i < nObjectSize; i++) {
        QJsonValue default_value = extensionObject.value(keys[i]);
        QJsonObject object = default_value.toObject();
        QJsonValue specValue = object.value("specVersion");
        output = keys[i];
        output += " : extension revision ";
        output += specValue.toVariant().toString();

        QTreeWidgetItem *pSubItem = new QTreeWidgetItem();
        pSubItem->setText(0, output);
        pRoot->addChild(pSubItem);
    }
}

/// This tree section has some different "kinds" of subtrees (the extensions)
/// and some extra text formatting requirements, so it had to be treated specially.
void VulkanInfoDialog::BuildLayers(QJsonValue &jsonValue, QTreeWidgetItem *root) {
    QJsonObject layersObject = jsonValue.toObject();
    int layersCount = layersObject.size();

    QString output = "Layers : count = ";
    output += QString().asprintf("%d", layersCount);

    root->setText(0, output);
    ui->treeWidget->addTopLevelItem(root);

    // Loop through all the layers
    QStringList layers = layersObject.keys();
    for (int i = 0; i < layersCount; i++) {
        QJsonValue layerTop = layersObject.value(layers[i]);
        QJsonObject layerObject = layerTop.toObject();
        output = layers[i] + " (";
        output += layerObject.value("description").toString();
        output += ")";

        QTreeWidgetItem *layer = new QTreeWidgetItem();
        layer->setText(0, output);

        QJsonValue apiVersion = layerObject.value("version");
        QTreeWidgetItem *api_version_item = new QTreeWidgetItem();
        output = "Vulkan Version : " + apiVersion.toString();
        api_version_item->setText(0, output);
        layer->addChild(api_version_item);

        QVariant implementationVersion = layerObject.value("implementation version").toVariant();
        QTreeWidgetItem *implementation_version_item = new QTreeWidgetItem();
        output = "Implementation Version : " + implementationVersion.toString();
        implementation_version_item->setText(0, output);
        layer->addChild(implementation_version_item);

        // Each layer has extensions
        QJsonValue layerExtensions = layerObject.value("Layer Extensions");
        QJsonObject layerExtensionsObject = layerExtensions.toObject();
        int nExtCount = layerExtensionsObject.size();
        output = QString().asprintf("Layer Extensions: count = %d", nExtCount);
        QTreeWidgetItem *ext_item = new QTreeWidgetItem();
        ext_item->setText(0, output);
        layer->addChild(ext_item);

        BuildExtensions(layerExtensions, ext_item);  // Generic enough

        // Each layer has devices too
        QJsonValue devicesValue = layerObject.value("Devices");
        QJsonObject devicesObject = devicesValue.toObject();
        int devCount = devicesObject.size();
        QTreeWidgetItem *device_item = new QTreeWidgetItem();
        device_item->setText(0, QString().asprintf("Devices: count = %d", devCount));
        layer->addChild(device_item);
        QStringList devicesList = devicesObject.keys();
        for (int dev = 0; dev < devCount; dev++) {
            QJsonValue gpuVal = devicesObject.value(devicesList[dev]);
            QJsonObject gpuObject = gpuVal.toObject();
            QJsonValue gpuIDValue = gpuObject.value("GPU id");
            output = "GPU id : ";
            output += gpuIDValue.toVariant().toString();
            output += " (";
            output += devicesList[dev];
            output += ")";
            QTreeWidgetItem *no_child = new QTreeWidgetItem();
            no_child->setText(0, output);
            device_item->addChild(no_child);

            QJsonValue devExtVal = gpuObject.value("Layer-Device Extensions");
            QJsonObject devExtObj = devExtVal.toObject();
            int extCount = devExtObj.size();
            output = QString().asprintf("Layer-Device Extensions: count = %d", extCount);
            QTreeWidgetItem *pNext = new QTreeWidgetItem();
            pNext->setText(0, output);
            device_item->addChild(pNext);
            BuildExtensions(devExtVal, pNext);  // Generic enough
        }
        root->addChild(layer);
    }
}

/// Nice and well behaved. TraverseGenericProperties will build the whole tree.
void VulkanInfoDialog::BuildSurfaces(QJsonValue &jsonValue, QTreeWidgetItem *pRoot) {
    QJsonObject surfaces = jsonValue.toObject();

    pRoot->setText(0, "Presentable Surfaces");
    ui->treeWidget->addTopLevelItem(pRoot);

    TraverseGenericProperties(jsonValue, pRoot);
}

/// Nice and well behaved. TraverseGenericProperties will build the whole tree.
void VulkanInfoDialog::BuildGroups(QJsonValue &jsonValue, QTreeWidgetItem *pRoot) {
    QJsonObject groupsObject = jsonValue.toObject();
    pRoot->setText(0, "Device Groups");
    ui->treeWidget->addTopLevelItem(pRoot);

    TraverseGenericProperties(jsonValue, pRoot);
}

/// The Device Properties and Extensions tree is mostly pretty well behaved.
/// There is one section that can be handled by the TraverseGenericProperties()
/// function, and just one section that is specifially needing the
/// extensions list parser.
void VulkanInfoDialog::BuildDevices(QJsonValue &jsonValue, QTreeWidgetItem *root) {
    QJsonObject gpuObject = jsonValue.toObject();

    root->setText(0, "Device Properties and Extensions");
    ui->treeWidget->addTopLevelItem(root);

    // For each like GPU0 object
    QStringList gpuList = gpuObject.keys();
    for (int i = 0; i < gpuObject.size(); i++) {
        QTreeWidgetItem *pGPU = new QTreeWidgetItem();
        pGPU->setText(0, gpuList[i]);
        root->addChild(pGPU);

        QJsonValue properties = gpuObject.value(gpuList[i]);
        QJsonObject propertiesObject = properties.toObject();
        QStringList propertyParents = propertiesObject.keys();

        for (int j = 0; j < propertiesObject.size(); j++) {
            QTreeWidgetItem *parent = new QTreeWidgetItem();
            parent->setText(0, propertyParents[j]);
            pGPU->addChild(parent);
            QJsonValue default_value = propertiesObject.value(propertyParents[j]);

            if (propertyParents[j] == "Device Extensions")
                BuildExtensions(default_value, parent);
            else
                TraverseGenericProperties(default_value, parent);
        }
    }
}
