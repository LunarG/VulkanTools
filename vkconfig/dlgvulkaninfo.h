#ifndef DLGVULKANINFO_H
#define DLGVULKANINFO_H

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

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class dlgVulkanInfo;
}

class dlgVulkanInfo : public QDialog
{
    Q_OBJECT

public:
    explicit dlgVulkanInfo(QWidget *parent = nullptr);
    ~dlgVulkanInfo();

    void BuildExtensions(QJsonValue& jsonValue, QTreeWidgetItem *pRoot);
    void BuildLayers(QJsonValue& jsonValue, QTreeWidgetItem *pRoot);
    void BuildSurfaces(QJsonValue& jsonValue, QTreeWidgetItem *pRoot);
    void BuildGroups(QJsonValue& jsonValue, QTreeWidgetItem *pRoot);
    void BuildDevices(QJsonValue& jsonValue, QTreeWidgetItem *pRoot);
    void TraverseGenericProperties(QJsonValue& parentJson, QTreeWidgetItem *pParentTreeItem);


private:
    Ui::dlgVulkanInfo *ui;
};

#endif // DLGVULKANINFO_H
