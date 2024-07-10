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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/parameter.h"

#include "widget_setting.h"
#include "combo_box.h"

#include <QResizeEvent>

#include <vector>

class WidgetPreset : public WidgetSettingBase {
    Q_OBJECT

   public:
    explicit WidgetPreset(QTreeWidget* tree, QTreeWidgetItem* item, const Layer& layer, Parameter& parameter);

    void Refresh(RefreshAreas refresh_areas) override;

   public Q_SLOTS:
    void OnPresetChanged(int combox_preset_index);

   Q_SIGNALS:
    void itemChanged();

   protected:
    void resizeEvent(QResizeEvent* event) override;

   private:
    int GetComboBoxIndex(const char* preset_label) const;

    std::vector<std::string> preset_labels;  // The preset in the combobox
    const Layer& layer;
    Parameter& parameter;
    ComboBox* field;
};
