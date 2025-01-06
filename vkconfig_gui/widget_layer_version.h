/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include <QComboBox>
#include <QEvent>

class LayerVersionComboBox : public QComboBox {
    Q_OBJECT

   public:
    LayerVersionComboBox(QWidget *parent);

    void Init(const Parameter &parameter, const std::vector<Path> &layer_versions);

    bool eventFilter(QObject *o, QEvent *e) override;

   public Q_SLOTS:
    void on_layer_version_combobox_currentIndexChanged(int index);

   Q_SIGNALS:
    void itemChanged();

   private:
    QWidget *parent = nullptr;
    std::vector<Path> data;
};
