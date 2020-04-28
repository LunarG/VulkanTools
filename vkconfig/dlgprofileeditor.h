#ifndef DLGPROFILEEDITOR_H
#define DLGPROFILEEDITOR_H
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
#include <QResizeEvent>
#include <QShowEvent>

#include "profiledef.h"
#include <vulkanconfiguration.h>
#include <settingseditor.h>


namespace Ui {
class dlgProfileEditor;
}

class dlgProfileEditor : public QDialog
{
    Q_OBJECT

public:
    explicit dlgProfileEditor(QWidget *parent, CProfileDef* pProfileToEdit);
    ~dlgProfileEditor();

    // Load all layers into the list box
    void LoadLayerDisplay(int nSelection = -1);



private:
    Ui::dlgProfileEditor *ui;

    CVulkanConfiguration*    pVulkanConfig;
    CSettingsEditor          settingsEditor;

    CProfileDef             *pThisProfile;

    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *) override;

    void addMissingLayers(CProfileDef *pProfile);

public Q_SLOTS:
    virtual void accept() override;

    void currentLayerChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious);

    void on_pushButtonResetLayers_clicked();
    void on_pushButtonLaunchTest_clicked();
    void on_pushButtonAddLayers_clicked();

    void on_toolButtonUp_clicked();
    void on_toolButtonDown_clicked();

    void layerUseChanged(int nSelection);

    void profileNameChanged(const QString& qsProfileName);

};

#endif // DLGPROFILEEDITOR_H
