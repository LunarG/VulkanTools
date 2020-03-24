#ifndef DLGCREATEASSOCIATION_H
#define DLGCREATEASSOCIATION_H
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
 *
 * A list of applicaitons can be maintained that when enabled
 * the profile will only affect those in the list.
 */
#include <QDialog>
#include <vulkanconfiguration.h>

namespace Ui {
class dlgCreateAssociation;
}

class dlgCreateAssociation : public QDialog
{
    Q_OBJECT

public:
    explicit dlgCreateAssociation(QWidget *parent = nullptr);
    ~dlgCreateAssociation();

private:
    Ui::dlgCreateAssociation *ui;

    CVulkanConfiguration    *pVulkanConfig;
    int nCurrentProfile;

public	Q_SLOTS:
    void on_pushButtonAdd_clicked();        // Pick the application
    void on_pushButtonRemove_clicked();     // Remove
};

#endif // DLGCREATEASSOCIATION_H
