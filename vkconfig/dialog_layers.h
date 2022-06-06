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

#pragma once

#include "widget_tree_friendly_combobox.h"

#include "../vkconfig_core/configuration.h"

#include "ui_dialog_layers.h"

#include <cassert>
#include <memory>

class TreeWidgetItemParameter : public QTreeWidgetItem {
   public:
    TreeWidgetItemParameter(const char *layer_name) : layer_name(layer_name) { assert(layer_name != nullptr); }

    std::string layer_name;
};

class LayersDialog : public QDialog {
    Q_OBJECT

   public:
    explicit LayersDialog(QWidget *parent, const Configuration &configuration);
    ~LayersDialog();

   public Q_SLOTS:
    void accept() override;
    void reject() override;

    void currentLayerChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_button_properties_clicked();
    void on_button_doc_clicked();
    void on_button_website_clicked();
    void on_button_reset_clicked();
    void on_pushButtonUp_clicked();
    void on_pushButtonDown_clicked();
    void on_pushButtonAdd_clicked();
    void on_pushButtonRemove_clicked();
    void on_layerTreePath_itemSelectionChanged();

    void OnLayerTreeSortedClicked(QTreeWidgetItem *item, int column);

    void layerUseChanged(QTreeWidgetItem *item, int selection);

   private:
    LayersDialog(const LayersDialog &) = delete;
    LayersDialog &operator=(const LayersDialog &) = delete;

    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *) override;

    void UpdateUI();

    void UpdateButtons();

    void LoadAvailableLayersUI();
    void LoadSortedLayersUI();
    void LoadUserDefinedPaths();
    void Reload();
    void Reinit();
    void AddLayerItem(const Parameter &parameter);
    void BuildParameters();
    void OverrideAllExplicitLayers();
    void OverrideOrder(const std::string layer_name, const TreeWidgetItemParameter *below, const TreeWidgetItemParameter *above);

    Configuration configuration;
    std::string selected_available_layer_name;
    std::string selected_sorted_layer_name;

    std::unique_ptr<Ui::dialog_layers> ui;
};
