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

#include "ui_dialog_custom_paths.h"

#include <memory>
#include <string>
#include <vector>

#include <QCloseEvent>

class UserDefinedPathsDialog : public QDialog {
    Q_OBJECT

   public:
    explicit UserDefinedPathsDialog(QWidget *parent = nullptr);

   public Q_SLOTS:
    void on_pushButtonAdd_clicked();
    void on_pushButtonRemove_clicked();
    void on_treeWidget_itemSelectionChanged();
    void accept() override;
    void reject() override;

   private:
    UserDefinedPathsDialog(const UserDefinedPathsDialog &) = delete;
    UserDefinedPathsDialog &operator=(const UserDefinedPathsDialog &) = delete;

    void RepopulateTree();
    void Reload();
    void SaveLayersPaths(const std::vector<std::string> &layers_paths);

    std::unique_ptr<Ui::dialog_custom_paths> ui;
    std::vector<std::string> layers_paths;
    std::vector<std::string> layers_paths_saved;
};
