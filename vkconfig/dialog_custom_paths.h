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

class CustomPathsDialog : public QDialog {
    Q_OBJECT

   public:
    explicit CustomPathsDialog(QWidget *parent = nullptr);
    ~CustomPathsDialog();

   public Q_SLOTS:
    void on_pushButtonAdd_clicked();
    void on_pushButtonRemove_clicked();
    void on_treeWidget_itemSelectionChanged();

   private:
    CustomPathsDialog(const CustomPathsDialog &) = delete;
    CustomPathsDialog &operator=(const CustomPathsDialog &) = delete;

    void RepopulateTree();

    std::unique_ptr<Ui::dialog_custom_paths> ui;
};
