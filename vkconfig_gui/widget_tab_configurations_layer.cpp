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

#include "widget_tab_configurations_layer.h"
#include "widget_setting.h"
#include "tab_configurations.h"

#include "../vkconfig_core/configurator.h"

ConfigurationLayerWidget::ConfigurationLayerWidget(TabConfigurations *tab, const Parameter &parameter,
                                                   const std::vector<Version> &layer_versions, LayersView view)
    : tab(tab), layer_name(parameter.key) {
    // const bool is_implicit_layer = layers.empty() ? false : layers[0]->type == LAYER_TYPE_IMPLICIT;

    const Configurator &configurator = Configurator::Get();
    const Layer *layer = configurator.layers.Find(parameter.key, parameter.api_version);

    if (parameter.control != LAYER_CONTROL_APPLICATIONS_API && parameter.control != LAYER_CONTROL_APPLICATIONS_ENV) {
        this->setEnabled(layer != nullptr);

        this->layer_version = new QComboBox(this);
        this->layer_version->setVisible(view == LAYERS_VIEW_ALL_AVAILABLE);
        this->layer_version->setEnabled(layer != nullptr);
        this->layer_version->addItem("Latest");

        const Layer *layer_latest = configurator.layers.Find(parameter.key, Version::LATEST);
        if (layer_latest != nullptr) {
            this->layer_version->setItemData(0, layer_latest->manifest_path.AbsolutePath().c_str(), Qt::ToolTipRole);
        }

        int version_index = 0;
        for (std::size_t i = 0, n = layer_versions.size(); i < n; ++i) {
            if (layer_versions[i] == parameter.api_version) {
                version_index = this->layer_version->count();
            }

            const Layer *layer_version = configurator.layers.Find(parameter.key, layer_versions[i]);

            const int current_index = this->layer_version->count();

            this->layer_version->addItem(layer_versions[i].str().c_str());
            this->layer_version->setItemData(current_index, layer_version->manifest_path.AbsolutePath().c_str(), Qt::ToolTipRole);
        }

        this->layer_version->setCurrentIndex(version_index);
        if (layer != nullptr) {
            this->layer_version->setToolTip(layer->manifest_path.AbsolutePath().c_str());
            this->setToolTip(layer->description.c_str());
        }

        this->connect(this->layer_version, SIGNAL(currentIndexChanged(int)), this, SLOT(on_layer_version_currentIndexChanged(int)));

        this->layer_state = new QComboBox(this);
        if (layer != nullptr) {
            if (layer->type == LAYER_TYPE_EXPLICIT) {
                for (int i = LAYER_CONTROL_EXPLICIT_FIRST; i <= LAYER_CONTROL_EXPLICIT_LAST; ++i) {
                    this->layer_state->addItem(GetToken(static_cast<LayerControl>(i)));
                    this->layer_state->setItemData(i, GetDescription(static_cast<LayerControl>(i)), Qt::ToolTipRole);
                }
            } else {
                for (int i = LAYER_CONTROL_IMPLICIT_FIRST; i <= LAYER_CONTROL_IMPLICIT_LAST; ++i) {
                    this->layer_state->addItem(GetToken(static_cast<LayerControl>(i)));
                    this->layer_state->setItemData(i, GetDescription(static_cast<LayerControl>(i)), Qt::ToolTipRole);
                }
            }
        }
        this->layer_state->setEnabled(!layer_versions.empty());
        this->layer_state->setCurrentIndex(parameter.control);
        this->layer_state->setToolTip(GetDescription(parameter.control));
        this->layer_state->setEnabled(layer != nullptr);
        this->connect(this->layer_state, SIGNAL(currentIndexChanged(int)), this, SLOT(on_layer_state_currentIndexChanged(int)));
    }

    std::string decorated_name = parameter.key;

    if (layer_versions.empty()) {
        // A layers configuration may have excluded layer that are misssing because they are not available on this platform
        // We simply hide these layers to avoid confusing the Vulkan developers
        if (parameter.control == LAYER_CONTROL_OFF) {
            return;
        }

        if (parameter.control != LAYER_CONTROL_APPLICATIONS_API && parameter.control != LAYER_CONTROL_APPLICATIONS_ENV) {
            decorated_name += " (Missing)";
        }
    } else {
        assert(layer != nullptr);

        if (view == LAYERS_VIEW_OVERRIDDEN_ONLY && parameter.api_version != Version::LATEST) {
            decorated_name += format(" - %s", parameter.api_version.str().c_str());
        }

        if (layer->status != STATUS_STABLE) {
            decorated_name += format(" (%s)", GetToken(layer->status));
        }
    }

    this->setText((" =  " + decorated_name).c_str());
}

bool ConfigurationLayerWidget::eventFilter(QObject *target, QEvent *event) {
    QEvent::Type event_type = event->type();

    if (event_type == QEvent::Wheel) {
        return true;
    }

    if (event_type == QEvent::DragEnter || event_type == QEvent::DragMove || event_type == QEvent::DragLeave ||
        event_type == QEvent::DragEnter) {
        return true;
    }

    return false;
}

void ConfigurationLayerWidget::resizeEvent(QResizeEvent *event) {
    QSize size = event->size();

    if (this->layer_state != nullptr) {
        const QFontMetrics fm = this->layer_state->fontMetrics();
        const int width_state = HorizontalAdvance(fm, "Auto 000");
        const int width_version = HorizontalAdvance(fm, "1.2.199 000");

        const QRect state_button_rect = QRect(size.width() - width_state, 0, width_state, size.height());
        this->layer_state->setGeometry(state_button_rect);

        const QRect version_button_rect = QRect(size.width() - width_state - width_version, 0, width_version, size.height());
        this->layer_version->setGeometry(version_button_rect);
    }
}

void ConfigurationLayerWidget::on_layer_version_currentIndexChanged(int index) {
    assert(index >= 0);
    const std::string &text = this->layer_version->itemText(index).toStdString();
    Version version = text == "latest" ? Version::LATEST : Version(text.c_str());

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    configuration->SwitchLayerVersion(configurator.layers, this->layer_name, version);

    const Layer *layer = configurator.layers.Find(this->layer_name, version);
    if (layer != nullptr) {
        this->layer_version->setToolTip(layer->manifest_path.AbsolutePath().c_str());
        this->setToolTip(layer->description.c_str());
    }

    this->tab->UpdateUI_Settings(UPDATE_REBUILD_UI);
}

void ConfigurationLayerWidget::on_layer_state_currentIndexChanged(int index) {
    assert(index >= 0);
    const std::string &text = this->layer_state->itemText(index).toStdString();

    Configuration *configuration = Configurator::Get().GetActiveConfiguration();
    Parameter *parameter = configuration->Find(this->layer_name);
    parameter->control = GetLayerControl(text.c_str());

    this->layer_state->setToolTip(GetDescription(parameter->control));

    this->tab->UpdateUI_Settings(UPDATE_REFRESH_UI);
}
