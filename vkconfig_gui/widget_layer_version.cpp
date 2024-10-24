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

#include "widget_layer_version.h"

#include "../vkconfig_core/configurator.h"

LayerVersionComboBox::LayerVersionComboBox(QWidget *parent) : QComboBox(parent), parent(parent) {
    this->setObjectName(QString::fromUtf8("layer_version_combobox"));
    this->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    this->setMinimumHeight(24);
    this->adjustSize();
    this->connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(on_layer_version_combobox_currentIndexChanged(int)));
}

void LayerVersionComboBox::Init(const Parameter &parameter, const std::vector<Version> &layer_versions) {
    const Configurator &configurator = Configurator::Get();
    const Layer *layer_select = configurator.layers.Find(parameter.key, parameter.api_version);
    const Layer *layer_latest = configurator.layers.Find(parameter.key, Version::LATEST);

    this->blockSignals(true);

    this->clear();
    this->addItem("Latest");
    this->setItemData(0, layer_latest->manifest_path.AbsolutePath().c_str(), Qt::ToolTipRole);

    int version_index = 0;
    for (std::size_t i = 0, n = layer_versions.size(); i < n; ++i) {
        if (layer_versions[i] == parameter.api_version) {
            version_index = this->count();
        }

        const Layer *layer_version = configurator.layers.Find(parameter.key, layer_versions[i]);

        const int current_index = this->count();

        this->addItem(layer_versions[i].str().c_str());
        this->setItemData(current_index, layer_version->manifest_path.AbsolutePath().c_str(), Qt::ToolTipRole);
    }

    this->setCurrentIndex(version_index);

    this->blockSignals(false);

    if (layer_select != nullptr) {
        this->setToolTip(layer_select->manifest_path.AbsolutePath().c_str());
        this->setToolTip(layer_select->description.c_str());
    }

    this->adjustSize();
}

void LayerVersionComboBox::on_layer_version_combobox_currentIndexChanged(int index) {
    assert(index >= 0);
    const std::string &text = this->itemText(index).toStdString();
    Version version = text == "Latest" ? Version::LATEST : Version(text.c_str());
    assert(version != Version::NONE);

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    configuration->SwitchLayerVersion(configurator.layers, configuration->GetActiveParameter()->key, version);

    const Layer *layer = configurator.layers.Find(configuration->GetActiveParameter()->key, version);
    assert(layer != nullptr);
    this->setToolTip(layer->manifest_path.AbsolutePath().c_str());

    emit itemChanged();
}

bool LayerVersionComboBox::eventFilter(QObject *o, QEvent *e) {
    if (e->type() == QEvent::Resize) {
        const QRect enabled_button_rect = QRect(this->parent->width() - this->width() - 6, 0, this->width(), 24);
        this->setGeometry(enabled_button_rect);
    }
    return false;
}
