/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "../vkconfig_core/parameter.h"
#include "../vkconfig_core/layer.h"

#include "ui_mainwindow.h"

#include <QListWidgetItem>
#include <QLabel>
#include <QComboBox>
#include <QResizeEvent>

#include <memory>

class TabConfigurations;

class ConfigurationLayerWidget : public QLabel {
    Q_OBJECT

   public:
    ConfigurationLayerWidget(TabConfigurations *tab, const std::vector<const Layer *> &layers, const Parameter &parameter,
                             bool advanced_view);

   protected:
    bool eventFilter(QObject *target, QEvent *event);
    void resizeEvent(QResizeEvent *event) override;

   public Q_SLOTS:
    void on_layer_version_currentIndexChanged(int index);
    void on_layer_state_currentIndexChanged(int index);

   private:
    TabConfigurations *tab;
    std::string layer_name;

    QComboBox *layer_version = nullptr;
    QComboBox *layer_state = nullptr;
};
