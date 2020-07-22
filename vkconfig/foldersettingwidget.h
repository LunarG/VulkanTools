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

#include "layerfile.h"

#include <QWidget>
#include <QTreeWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QFileDialog>

class FolderSettingWidget : public QWidget {
    Q_OBJECT
   public:
    explicit FolderSettingWidget(QTreeWidgetItem *item, LayerSettings *layer_settings);

    virtual void resizeEvent(QResizeEvent *event) override;

   private:
    LayerSettings *_layer_settings;
    QLineEdit *_line_edit;
    QPushButton *_push_button;

   public Q_SLOTS:
    void browseButtonClicked();
    void textFieldChanged(const QString &new_text);

   Q_SIGNALS:
    void itemChanged();
};
