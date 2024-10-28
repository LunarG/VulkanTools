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

ConfigurationLayerWidget::ConfigurationLayerWidget(TabConfigurations *tab, const Parameter &parameter)
    : tab(tab), layer_name(parameter.key) {
    const Configurator &configurator = Configurator::Get();
    const Layer *layer = configurator.layers.Find(parameter.key, parameter.api_version);

    if (IsVisibleLayer(parameter.control)) {
        this->setEnabled(layer != nullptr && parameter.enabled);

        this->layer_state = new QComboBox(this);
        this->layer_state->setSizeAdjustPolicy(QComboBox::AdjustToContents);
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
        this->layer_state->setCurrentIndex(parameter.control);
        this->layer_state->setToolTip(GetDescription(parameter.control));
        this->layer_state->setEnabled(layer != nullptr);
        this->connect(this->layer_state, SIGNAL(currentIndexChanged(int)), this, SLOT(on_layer_state_currentIndexChanged(int)));
    }

    std::string decorated_name = parameter.key;

    if (layer != nullptr) {
        decorated_name += format(" - %s", layer->api_version.str().c_str());

        if (layer->is_32bits) {
            decorated_name += " (32 bits)";
        }

        if (layer->status != STATUS_STABLE) {
            decorated_name += format(" (%s)", GetToken(layer->status));
        }
    }
    /*
    if (layer_versions.empty()) {
        // A layers configuration may have excluded layer that are misssing because they are not available on this platform
        // We simply hide these layers to avoid confusing the Vulkan developers
        if (parameter.control == LAYER_CONTROL_OFF) {
            return;
        }
    } else {
        assert(layer != nullptr);

        if (layer->status != STATUS_STABLE) {
            decorated_name += format(" (%s)", GetToken(layer->status));
        }
    }
    */
    this->setText(decorated_name.c_str());
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
        this->layer_state->adjustSize();

        const int width_state = this->layer_state->width();

        const QRect state_button_rect = QRect(size.width() - width_state, 0, width_state, size.height());
        this->layer_state->setGeometry(state_button_rect);
    }
}

void ConfigurationLayerWidget::on_layer_state_currentIndexChanged(int index) {
    assert(index >= 0);
    const std::string &text = this->layer_state->itemText(index).toStdString();

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    Parameter *parameter = configuration->Find(this->layer_name);
    if (parameter != nullptr) {
        parameter->control = GetLayerControl(text.c_str());
        this->layer_state->setToolTip(GetDescription(parameter->control));

        configurator.Override(OVERRIDE_AREA_ALL);
    }

    this->tab->UpdateUI_Settings(UPDATE_REFRESH_UI);
}
