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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "widget_preset.h"

#include <cassert>

WidgetPreset::WidgetPreset(QTreeWidgetItem* item, const Layer& layer, Parameter& parameter) : layer(layer), parameter(parameter) {
    assert(item);
    assert(&layer);
    assert(&parameter);

    this->blockSignals(true);
    this->addItem(Layer::NO_PRESET);

    preset_labels.push_back(Layer::NO_PRESET);

    for (std::size_t i = 0, n = layer.presets.size(); i < n; ++i) {
        const LayerPreset& layer_preset = layer.presets[i];

        if (!IsPlatformSupported(layer_preset.platform_flags)) {
            continue;
        }

        this->addItem((layer_preset.label + " Preset").c_str());
        preset_labels.push_back(layer_preset.label);
    }

    this->blockSignals(false);
    this->UpdateCurrentIndex();

    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(OnPresetChanged(int)));
}

void WidgetPreset::UpdateCurrentIndex() {
    const std::string& preset_label = layer.FindPresetLabel(parameter.settings);

    this->blockSignals(true);
    this->setCurrentIndex(GetComboBoxIndex(preset_label.c_str()));
    this->blockSignals(false);

    if (preset_label == Layer::NO_PRESET) return;

    const LayerPreset* preset = GetPreset(layer.presets, preset_label.c_str());
    assert(preset != nullptr);
    this->setToolTip(preset->description.c_str());
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
}
