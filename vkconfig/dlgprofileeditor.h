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
 * Authors:
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include <QDialog>
#include <QTreeWidgetItem>
#include <QResizeEvent>
#include <QShowEvent>

#include "treefriendlycombobox.h"
#include "configuration.h"

namespace Ui {
class dlgProfileEditor;
}

class dlgProfileEditor : public QDialog {
    Q_OBJECT

   public:
    explicit dlgProfileEditor(QWidget *parent, Configuration *pProfileToEdit);
    ~dlgProfileEditor();

    // Load all layers into the list box
    void LoadLayerDisplay(int nSelection = -1);

   private:
    Ui::dlgProfileEditor *ui_;

    Configuration *configuration_;

    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *) override;

    void AddMissingLayers(Configuration *pProfile);

    void PopulateCustomTree();

   public Q_SLOTS:
    virtual void accept() override;

    void currentLayerChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious);

    void customTreeItemActivated(QTreeWidgetItem *pItem, int nColumn);

    void on_pushButtonResetLayers_clicked();
    void on_pushButtonAddLayers_clicked();
    void on_pushButtonRemoveLayers_clicked();

    void on_toolButtonUp_clicked();
    void on_toolButtonDown_clicked();

    void layerUseChanged(QTreeWidgetItem *pItem, int nSelection);
};
