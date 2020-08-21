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

#include "../vkconfig_core/layer.h"

#include <QWidget>
#include <QTreeWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QFileDialog>

class FileSystemSettingWidget : public QWidget {
    Q_OBJECT
   public:
    explicit FileSystemSettingWidget(QTreeWidgetItem *item, LayerSetting &layer_setting, SettingType setting_type);

   private:
    virtual void resizeEvent(QResizeEvent *event) override;

    LayerSetting &_layer_setting;
    QLineEdit *_line_edit;
    QPushButton *_push_button;

   public Q_SLOTS:
    void browseButtonClicked();
    void textFieldChanged(const QString &newText);

   Q_SIGNALS:
    void itemChanged();

   private:
    enum Mode { MODE_OPEN_FILE, MODE_SAVE_FILE, MODE_SAVE_FOLDER };
    const Mode _mode;
    Mode GetMode(SettingType type) const;
};
