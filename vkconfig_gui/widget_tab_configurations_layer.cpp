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

#include "widget_tab_configurations_layer.h"
#include "widget_setting.h"
#include "tab_configurations.h"
#include "widget_resize_button.h"
#include "combo_box.h"
#include "style.h"

#include "../vkconfig_core/configurator.h"

#include <QMessageBox>

static const char *GetDescription(LayerControl control, LayerBuiltin builtin) {
    if (builtin == LAYER_BUILTIN_UNORDERED) {
        static const char *TABLE[] = {
            "Vulkan Layers located by Vulkan Applications using 'VK_ADD_LAYER_PATH' at launch",  // LAYER_CONTROL_AUTO
            "Prevent Vulkan applications from loading additional Vulkan Layers not visible by Vulkan Configurator",  // LAYER_CONTROL_DISCARD
        };
        static_assert(std::size(TABLE) == LAYER_CONTROL_UNORDERED_COUNT);

        return TABLE[control];
    } else {
        static const char
            *
                TABLE[] =
                    {
                        "Explicit layers are disabled by default and implicit layers are enabled by default.",  // LAYER_CONTROL_AUTO
                        "Discard the layer, the Vulkan Loader isn't made awate of this layer but the Vulkan application can locate "
                        "it.",                                           // LAYER_CONTROL_DISCARD
                        "Enable the layer, ensuring its execution.",     // LAYER_CONTROL_ON
                        "Disable the layer, preventing its execution.",  // LAYER_CONTROL_OFF
                    };
        static_assert(std::size(TABLE) == LAYER_CONTROL_COUNT);

        return TABLE[control];
    }
}

static std::string GetActualControlTooltip(const Layer *layer) {
    if (layer->type == LAYER_TYPE_IMPLICIT) {
        if (!layer->disable_env.empty()) {
            if (qEnvironmentVariableIsSet(layer->disable_env.c_str())) {
                return format("'%s' is set", layer->disable_env.c_str());
            }
        }

        if (!layer->enable_env.empty()) {
            const std::string &value = qgetenv(layer->enable_env.c_str()).toStdString();
            if (value == layer->enable_value) {
                return format("'%s' is set to '%s'.", layer->enable_env.c_str(), value.c_str());
            } else {
                return format("Set '%s' to '%s' to enable '%s' by default.", layer->enable_env.c_str(), layer->enable_value.c_str(),
                              layer->key.c_str());
            }
        }

        return format("Set '%s' to disable '%s' by default.", layer->disable_env.c_str(), layer->key.c_str());
    } else {
        return ::GetDescription(LAYER_CONTROL_AUTO, LAYER_BUILTIN_NONE);
    }
}

ConfigurationLayerWidget::ConfigurationLayerWidget(TabConfigurations *tab, const Parameter &parameter)
    : layer_name(parameter.key), tab(tab) {
    const Configurator &configurator = Configurator::Get();
    const Layer *layer = configurator.layers.Find(parameter.key, parameter.api_version);

    this->layer_state = new ComboBox(this);
    this->layer_remove = new ResizeButton(this, 0);
    this->layer_remove->setMaximumWidth(32);
    this->layer_remove->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->layer_remove->adjustSize();
    this->layer_remove->setIcon(::Get(configurator.current_theme_mode, ::ICON_EXIT));
    this->layer_remove->setToolTip("Remove the missing layer");
    this->connect(this->layer_remove, SIGNAL(clicked()), this, SLOT(on_layer_remove_pressed()));

    const int first = parameter.builtin == LAYER_BUILTIN_UNORDERED ? LAYER_CONTROL_UNORDERED_FIRST : LAYER_CONTROL_FIRST;
    const int last = parameter.builtin == LAYER_BUILTIN_UNORDERED ? LAYER_CONTROL_UNORDERED_LAST : LAYER_CONTROL_LAST;
    for (int i = first; i <= last; ++i) {
        const LayerControl layer_control = static_cast<LayerControl>(i);
        std::string label = ::GetLabel(layer_control);

        this->layer_state->addItem(label.c_str());
        this->layer_state->setItemData(i, ::GetDescription(layer_control, parameter.builtin), Qt::ToolTipRole);
    }
    this->layer_state->setCurrentIndex(parameter.control);

    if (parameter.builtin == LAYER_BUILTIN_NONE && parameter.control == LAYER_CONTROL_AUTO && layer != nullptr) {
        std::string message = ::GetToken(layer->type) + std::string(" layer: ") + ::GetLabel(layer->GetActualControl());
        message += format(". %s", ::GetActualControlTooltip(layer).c_str());
        this->layer_state->setToolTip(message.c_str());
    } else {
        this->layer_state->setToolTip(::GetDescription(parameter.control, parameter.builtin));
    }

    this->connect(this->layer_state, SIGNAL(currentIndexChanged(int)), this, SLOT(on_layer_state_currentIndexChanged(int)));

    const bool layer_found = layer != nullptr || parameter.builtin != LAYER_BUILTIN_NONE;
    // this->setEnabled(layer_found);

    std::string decorated_name = parameter.key;

    if (layer != nullptr) {
        decorated_name += format(" - %s", layer->api_version.str().c_str());
        this->layer_remove->setVisible(false);
    } else if (!layer_found) {
        decorated_name += " - Missing";
        this->layer_state->setVisible(false);
        this->layer_state->setToolTip("Remove the layer from the configuration");
    }
    if (parameter.builtin == LAYER_BUILTIN_UNORDERED) {
        this->layer_remove->setVisible(false);
    }

    /*
    if (layer_versions.empty()) {
        // A layers configuration may have excluded layer that are misssing because they are not available on this platform
        // We simply hide these layers to avoid confusing the Vulkan developers
        if (parameter.control == LAYER_CONTROL_OFF) {
            return;
        }
    }
    */
    this->setText(decorated_name.c_str());

    if (parameter.builtin != LAYER_BUILTIN_NONE) {
        this->setToolTip(::GetDescription(parameter.control, parameter.builtin));
    } else if (!parameter.manifest.Empty()) {
        this->setToolTip(parameter.manifest.AbsolutePath().c_str());
    }
}

bool ConfigurationLayerWidget::eventFilter(QObject *target, QEvent *event) {
    (void)target;

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
        if (this->layer_state->isVisible()) {
            this->layer_state->adjustSize();

            const int width_state = this->layer_state->width();
            const int height_state = this->layer_state->height();

            const QRect state_button_rect = QRect(size.width() - width_state, 0, width_state, height_state);
            this->layer_state->setGeometry(state_button_rect);
        }
    }

    if (this->layer_remove != nullptr) {
        if (this->layer_remove->isVisible()) {
            this->layer_remove->adjustSize();

            const int width_state = this->layer_remove->width();
            const int height_state = this->layer_remove->height();

            const QRect state_button_rect = QRect(size.width() - width_state, 0, width_state, height_state);
            this->layer_remove->setGeometry(state_button_rect);
        }
    }
}

void ConfigurationLayerWidget::on_layer_remove_pressed() {
    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    configuration->RemoveParameter(this->layer_name);

    tab->UpdateUI(UPDATE_REBUILD_UI);
}

void ConfigurationLayerWidget::on_layer_state_currentIndexChanged(int index) {
    assert(index >= 0);
    const LayerControl control = static_cast<LayerControl>(index);

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    Parameter *parameter = configuration->Find(this->layer_name);
    if (parameter != nullptr) {
        if (!(configurator.Get(HIDE_MESSAGE_CRITICAL_IMPLICIT_LAYER_OVERRIDE))) {
            if (parameter->type == LAYER_TYPE_IMPLICIT && control != LAYER_CONTROL_AUTO) {
                const char *text =
                    "%s was overridden but it is an implicit layer. This may cause undefined behavior, including crashes.";

                QMessageBox alert;
                alert.setWindowTitle("An Vulkan implicit layer was overridden...");
                alert.setText(format(text, this->layer_name.c_str()).c_str());
                alert.setInformativeText("Do you want to continue?");
                alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                alert.setIcon(QMessageBox::Critical);
                alert.setCheckBox(new QCheckBox("Do not show again."));
                int selection = alert.exec();

                if (alert.checkBox()->isChecked()) {
                    configurator.Set(HIDE_MESSAGE_CRITICAL_IMPLICIT_LAYER_OVERRIDE);
                }

                if (selection == QMessageBox::No) {
                    this->layer_state->setCurrentIndex(LAYER_CONTROL_AUTO);
                    return;
                }
            }
        }

        parameter->control = control;
        this->layer_state->setToolTip(GetDescription(control, parameter->builtin));

        configurator.Override(OVERRIDE_AREA_ALL);
    }

    this->tab->UpdateUI_Settings(UPDATE_REFRESH_UI);
}
