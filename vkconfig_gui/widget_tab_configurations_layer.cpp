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

#if VKC_ENV == VKC_ENV_UNIX
#include <unistd.h>

#elif VKC_ENV == VKC_ENV_WIN32
#include <windows.h>

// From Stack Overflow.
#define MKPTR(p1, p2) ((DWORD_PTR)(p1) + (DWORD_PTR)(p2))

typedef enum _pe_architecture {
    PE_ARCHITECTURE_UNKNOWN = 0x0000,
    PE_ARCHITECTURE_ANYCPU = 0x0001,
    PE_ARCHITECTURE_X86 = 0x010B,
    PE_ARCHITECTURE_x64 = 0x020B
} PE_ARCHITECTURE;

static PE_ARCHITECTURE GetImageArchitecture(void *pImageBase) {
    // Parse and validate the DOS header
    IMAGE_DOS_HEADER *pDosHd = (IMAGE_DOS_HEADER *)pImageBase;
    if (IsBadReadPtr(pDosHd, sizeof(pDosHd->e_magic)) || pDosHd->e_magic != IMAGE_DOS_SIGNATURE) return PE_ARCHITECTURE_UNKNOWN;

    // Parse and validate the NT header
    IMAGE_NT_HEADERS *pNtHd = (IMAGE_NT_HEADERS *)MKPTR(pDosHd, pDosHd->e_lfanew);
    if (IsBadReadPtr(pNtHd, sizeof(pNtHd->Signature)) || pNtHd->Signature != IMAGE_NT_SIGNATURE) return PE_ARCHITECTURE_UNKNOWN;

    // First, naive, check based on the 'Magic' number in the Optional Header.
    PE_ARCHITECTURE architecture = (PE_ARCHITECTURE)pNtHd->OptionalHeader.Magic;

    return architecture;
}

// End Stack Overflow
#endif  // VKC_ENV

/// Utility function to see if the file is 32-bit
static bool IsDLL32Bit(const std::string full_path) {
#if VKC_ENV == VKC_ENV_WIN32
    if (full_path.empty()) {
        return false;
    }

    QFile file(full_path.c_str());
    if (!file.open(QIODevice::ReadOnly)) {
        return false;  // punt...
    }

    // Not gonna lie, just guessed 1024 and it was enough.
    // This is the minimum page size on any OS (I might be wrong,
    // it could be 4096), so going lower is not really helpful.
    // Changed to 4096 on a whim; looking for crashing bug I can't debug
    void *header = file.map(0, 4096, QFileDevice::MapPrivateOption);

    // Another punt as we may not be able to map the file
    if (header == nullptr) {
        return false;
    }

    PE_ARCHITECTURE arch = GetImageArchitecture(header);

    file.unmap((uchar *)header);
    file.close();

    return (PE_ARCHITECTURE_X86 == arch);
#else
    (void)full_path;
    return false;
#endif  // VKC_ENV
}

ConfigurationLayerWidget::ConfigurationLayerWidget(TabConfigurations *tab, const Parameter &parameter,
                                                   const std::vector<Version> &layer_versions, bool advanced_view)
    : tab(tab), layer_name(parameter.key) {
    // const bool is_implicit_layer = layers.empty() ? false : layers[0]->type == LAYER_TYPE_IMPLICIT;

    const Configurator &configurator = Configurator::Get();
    const Layer *layer = configurator.layers.Find(parameter.key, parameter.api_version);

    if (parameter.control != LAYER_CONTROL_APPLICATIONS_API && parameter.control != LAYER_CONTROL_APPLICATIONS_ENV) {
        this->setEnabled(layer != nullptr);

        this->layer_version = new QComboBox(this);
        this->layer_version->setVisible(advanced_view);
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

        // this->layer_version->setEnabled(layers.size() > 1);
        this->connect(this->layer_version, SIGNAL(currentIndexChanged(int)), this, SLOT(on_layer_version_currentIndexChanged(int)));
        // this->layer_version->installEventFilter(this);

        this->layer_state = new QComboBox(this);
        if (layer != nullptr) {
            if (layer->type == LAYER_TYPE_EXPLICIT) {
                for (int i = LAYER_CONTROL_EXPLICIT_FIRST; i <= LAYER_CONTROL_EXPLICIT_LAST; ++i) {
                    this->layer_state->addItem(GetToken(static_cast<LayerControl>(i)));
                }
            } else {
                for (int i = LAYER_CONTROL_IMPLICIT_FIRST; i <= LAYER_CONTROL_IMPLICIT_LAST; ++i) {
                    this->layer_state->addItem(GetToken(static_cast<LayerControl>(i)));
                }
            }
        }
        this->layer_state->setEnabled(!layer_versions.empty());
        this->layer_state->setCurrentIndex(parameter.control);
        this->layer_state->setToolTip(GetDescription(parameter.control));
        this->layer_state->setEnabled(layer != nullptr);
        this->connect(this->layer_state, SIGNAL(currentIndexChanged(int)), this, SLOT(on_layer_state_currentIndexChanged(int)));
        // this->layer_state->installEventFilter(this);
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

        if (!advanced_view && parameter.api_version != Version::LATEST) {
            decorated_name += format(" - %s", parameter.api_version.str().c_str());
        }

        if (layer->status != STATUS_STABLE) {
            decorated_name += format(" (%s)", GetToken(layer->status));
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
