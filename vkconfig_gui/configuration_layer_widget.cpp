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

#include "configuration_layer_widget.h"
#include "widget_setting.h"

#include "../vkconfig_core/configurator.h"

ConfigurationLayerWidget::ConfigurationLayerWidget(const std::vector<const Layer *> &layers, const Parameter &parameter,
                                                   bool advanced_view)
    : layer_name(parameter.key) {
    // const bool is_implicit_layer = layers.empty() ? false : layers[0]->type == LAYER_TYPE_IMPLICIT;

    Version api_version;

    if (parameter.control != LAYER_CONTROL_APPLICATIONS_API && parameter.control != LAYER_CONTROL_APPLICATIONS_ENV) {
        this->layer_version = new QComboBox(this);
        this->layer_version->setVisible(advanced_view);
        this->layer_version->addItem("Latest");
        int version_index = 0;

        const Layer *selected_layer = nullptr;
        const Layer *latest_layer = nullptr;

        for (std::size_t i = 0, n = layers.size(); i < n; ++i) {
            if (layers[i]->key != parameter.key) {
                continue;
            }

            if (latest_layer == nullptr) {
                latest_layer = layers[i];
            }

            if (layers[i]->api_version == parameter.api_version) {
                selected_layer = layers[i];
                version_index = this->layer_version->count();
            } else if (layers[i]->api_version > latest_layer->api_version) {
                latest_layer = layers[i];
            }

            this->layer_version->addItem(layers[i]->api_version.str().c_str());
        }
        this->layer_version->setCurrentIndex(version_index);
        if (selected_layer != nullptr) {
            api_version = selected_layer->api_version;
            this->layer_version->setToolTip(selected_layer->manifest_path.AbsolutePath().c_str());
        } else if (latest_layer != nullptr) {
            api_version = latest_layer->api_version;
            this->layer_version->setToolTip(latest_layer->manifest_path.AbsolutePath().c_str());
        }

        // this->layer_version->setEnabled(layers.size() > 1);
        this->connect(this->layer_version, SIGNAL(currentIndexChanged(int)), this, SLOT(on_layer_version_currentIndexChanged(int)));
        // this->layer_version->installEventFilter(this);

        this->layer_state = new QComboBox(this);
        for (int i = LAYER_CONTROL_UI_FIRST; i <= LAYER_CONTROL_UI_LAST; ++i) {
            this->layer_state->addItem(GetToken(static_cast<LayerControl>(i)));
        }
        this->layer_state->setEnabled(!layers.empty());
        this->layer_state->setCurrentIndex(parameter.control);
        this->connect(this->layer_state, SIGNAL(currentIndexChanged(int)), this, SLOT(on_layer_state_currentIndexChanged(int)));
        // this->layer_state->installEventFilter(this);
    }

    std::string decorated_name = parameter.key;

    if (layers.empty()) {
        // A layers configuration may have excluded layer that are misssing because they are not available on this platform
        // We simply hide these layers to avoid confusing the Vulkan developers
        if (parameter.control == LAYER_CONTROL_OFF) {
            return;
        }

        if (parameter.control != LAYER_CONTROL_APPLICATIONS_API && parameter.control != LAYER_CONTROL_APPLICATIONS_ENV) {
            decorated_name += " (Missing)";
        }
    } else {
        if (!advanced_view && api_version != Version::VERSION_NULL) {
            decorated_name += format(" - %s", api_version.str().c_str());
        }

        if (layers[0]->status != STATUS_STABLE) {
            decorated_name += format(" (%s)", GetToken(layers[0]->status));
        }

        // if (IsDLL32Bit(layer->manifest_path)) {
        //    decorated_name += " (32-bit)";
        //}
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
    Version version = text == "latest" ? Version::VERSION_NULL : Version(text.c_str());

    Configuration *configuration = Configurator::Get().GetActiveConfiguration();
    Parameter *parameter = configuration->Find(this->layer_name);
    parameter->api_version = version;
}

void ConfigurationLayerWidget::on_layer_state_currentIndexChanged(int index) {
    assert(index >= 0);
    const std::string &text = this->layer_state->itemText(index).toStdString();

    Configuration *configuration = Configurator::Get().GetActiveConfiguration();
    Parameter *parameter = configuration->Find(this->layer_name);
    parameter->control = GetLayerControl(text.c_str());
}