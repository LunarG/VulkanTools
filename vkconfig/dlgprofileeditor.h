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
 * - Richard S. Wright Jr.
 * - Christophe Riccio
 */

#pragma once

#include "widget_tree_friendly_combobox.h"

#include "../vkconfig_core/configuration.h"

#include <QDialog>
#include <QTreeWidgetItem>
#include <QResizeEvent>
#include <QShowEvent>

namespace Ui {
class dlgProfileEditor;
}

class dlgProfileEditor : public QDialog {
    Q_OBJECT

   public:
    explicit dlgProfileEditor(QWidget *parent, Configuration *configuration);
    ~dlgProfileEditor();

    // Load all layers into the list box
    void LoadLayerDisplay(int selection = -1);

    QString GetConfigurationName() const;

   private:
    Ui::dlgProfileEditor *ui;

    Configuration *_configuration;

    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *) override;

    void AddMissingLayers(Configuration *configuration);

    void PopulateCustomTree();

   public Q_SLOTS:
    virtual void accept() override;

    void currentLayerChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void customTreeItemActivated(QTreeWidgetItem *item, int column);

    void on_pushButtonResetLayers_clicked();
    void on_pushButtonAddLayers_clicked();
    void on_pushButtonRemoveLayers_clicked();

    void on_toolButtonUp_clicked();
    void on_toolButtonDown_clicked();

    void layerUseChanged(QTreeWidgetItem *item, int selection);

   private:
    dlgProfileEditor(const dlgProfileEditor &) = delete;
    dlgProfileEditor &operator=(const dlgProfileEditor &) = delete;
};
