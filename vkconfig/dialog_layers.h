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

#include "widget_tree_friendly_combobox.h"

#include "../vkconfig_core/configuration.h"

#include "ui_dialog_layers.h"

#include <cassert>
#include <memory>

class TreeWidgetItemParameter : public QTreeWidgetItem {
   public:
    TreeWidgetItemParameter(const QString &layer_name) : layer_name(layer_name) { assert(!layer_name.isEmpty()); }

    QString layer_name;
};

class LayersDialog : public QDialog {
    Q_OBJECT

   public:
    explicit LayersDialog(QWidget *parent, const Configuration &configuration);
    ~LayersDialog();

    void LoadAvailableLayersUI();
    void LoadSortedLayersUI();

    QString GetConfigurationName() const;

   public Q_SLOTS:
    virtual void accept() override;

    void currentLayerChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_pushButtonResetLayers_clicked();
    void on_pushButtonCustomLayers_clicked();
    void on_pushButtonUp_clicked();
    void on_pushButtonDown_clicked();

    void OnLayerTreeSortedClicked(QTreeWidgetItem *item, int column);

    void layerUseChanged(QTreeWidgetItem *item, int selection);

   private:
    LayersDialog(const LayersDialog &) = delete;
    LayersDialog &operator=(const LayersDialog &) = delete;

    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *) override;

    void UpdateUI();

    void AddLayerItem(const Parameter &parameter);
    void BuildParameters();
    void OverrideAllExplicitLayers();
    void OverrideOrder(const QString layer_name, const TreeWidgetItemParameter *below, const TreeWidgetItemParameter *above);

    Configuration configuration;
    std::vector<Parameter> parameters;  // List of available layers and configuration layers
    QString selected_available_layer_name;
    QString selected_sorted_layer_name;

    std::unique_ptr<Ui::dialog_layers> ui;
};
