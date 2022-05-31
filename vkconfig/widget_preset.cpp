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

#include "widget_preset.h"

#include <cassert>

WidgetPreset::WidgetPreset(QTreeWidget* tree, QTreeWidgetItem* item, const Layer& layer, Parameter& parameter)
    : WidgetSettingBase(tree, item), layer(layer), parameter(parameter), field(new ComboBox(this)) {
    assert(&layer);
    assert(&parameter);

    this->field->setFocusPolicy(Qt::StrongFocus);
    this->field->addItem(Layer::NO_PRESET);

    preset_labels.push_back(Layer::NO_PRESET);

    for (std::size_t i = 0, n = layer.presets.size(); i < n; ++i) {
        const LayerPreset& layer_preset = layer.presets[i];

        if (!IsPlatformSupported(layer_preset.platform_flags)) continue;
        if (layer_preset.view == SETTING_VIEW_HIDDEN) continue;

        this->field->addItem((layer_preset.label + " Preset").c_str());
        preset_labels.push_back(layer_preset.label);
    }

    // 'Refresh' need to be called before 'connect' to avoid triggering 'currentIndexChanged' in an infinite loop
    this->Refresh(REFRESH_ENABLE_AND_STATE);
    this->connect(this->field, SIGNAL(currentIndexChanged(int)), this, SLOT(OnPresetChanged(int)));

    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    this->tree->setItemWidget(this->item, 0, this);
}

void WidgetPreset::Refresh(RefreshAreas refresh_areas) {
    (void)refresh_areas;

    const std::string& preset_label = layer.FindPresetLabel(parameter.settings);

    this->field->blockSignals(true);
    this->field->setCurrentIndex(GetComboBoxIndex(preset_label.c_str()));
    this->field->blockSignals(false);

    if (preset_label != Layer::NO_PRESET) {
        const LayerPreset* preset = GetPreset(layer.presets, preset_label.c_str());
        assert(preset != nullptr);
        this->setToolTip(preset->description.c_str());
    }
}

void WidgetPreset::resizeEvent(QResizeEvent* event) {
    const QRect button_rect = QRect(0, 0, event->size().width(), event->size().height());
    this->field->setGeometry(button_rect);
}

int WidgetPreset::GetComboBoxIndex(const char* preset_label) const {
    for (std::size_t i = 0, n = preset_labels.size(); i < n; ++i) {
        if (preset_labels[i] == preset_label) return static_cast<int>(i);
    }

    assert(0);
    return -1;
}

void WidgetPreset::OnPresetChanged(int combox_preset_index) {
    assert(combox_preset_index >= 0 && static_cast<std::size_t>(combox_preset_index) < preset_labels.size());
    const std::string& preset_label = preset_labels[combox_preset_index];

    if (preset_label == Layer::NO_PRESET) return;

    const LayerPreset* preset = GetPreset(layer.presets, preset_label.c_str());
    assert(preset != nullptr);
    parameter.ApplyPresetSettings(*preset);

    emit itemChanged();
}
