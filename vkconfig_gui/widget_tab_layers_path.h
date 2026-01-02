/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/type_layers_paths.h"

#include <QCheckBox>
#include <QPushButton>
#include <QResizeEvent>

class LayersPathWidget : public QCheckBox {
    Q_OBJECT

   public:
    LayersPathWidget(const LayersPathInfo &path_info, LayersPaths layers_path, bool removabled);

   protected:
    void resizeEvent(QResizeEvent *event) override;

   public Q_SLOTS:
    void on_buttom_remove_clicked(bool checked = false);
    void on_toggled(bool checked);

   Q_SIGNALS:
    void itemChanged();
    void itemToggled();

   public:
    LayersPaths layers_path;
    LayersPathInfo path_info;
    QPushButton *buttom_remove = nullptr;
};
