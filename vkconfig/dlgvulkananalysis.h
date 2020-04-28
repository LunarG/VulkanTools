#ifndef DLGVULKANANALYSIS_H
#define DLGVULKANANALYSIS_H

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
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QTableWidget>

namespace Ui {
class dlgVulkanAnalysis;
}

class dlgVulkanAnalysis : public QDialog
{
    Q_OBJECT

public:
    explicit dlgVulkanAnalysis(QWidget *parent = nullptr);
    ~dlgVulkanAnalysis();

    void RunTool(void);

private:
    void loadTable(QJsonObject& jsonParent, QTableWidget* pTable);

    Ui::dlgVulkanAnalysis *ui;
};

#endif // DLGVULKANANALYSIS_H
