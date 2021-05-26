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

#include "alert.h"

#include <QMessageBox>

void Alert::ConfigurationRenamingFailed() {
    QMessageBox alert;
    alert.setWindowTitle("Renaming of the layers configuration failed...");
    alert.setText("There is already a configuration with the same name.");
    alert.setInformativeText("Use a different name for the configuration.");
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

void Alert::ConfigurationNameEmpty() {
    QMessageBox alert;
    alert.setWindowTitle("Renaming of the layers configuration failed...");
    alert.setText("The configuration name is empty.");
    alert.setInformativeText("The configuration name is required.");
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

void Alert::ConfigurationNameInvalid() {
    QMessageBox alert;
    alert.setWindowTitle("Invalid name for a configuration...");
    alert.setText("The configuration name is used to build a filename.");
    alert.setInformativeText("The name can't contain any of the following characters: \\ / : * \" < > |.");
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

static std::string BuildPropertiesLog(const Layer& layer) {
    std::string description;
    if (!layer.description.empty()) {
        description += layer.description + "\n";
    }
    description += "API Version: " + layer.api_version.str() + " - Implementation Version: " + layer.implementation_version + "\n";
    if (layer.platforms != 0) {
        description += "Supported Platforms: ";

        const std::vector<std::string>& platforms = GetPlatformTokens(layer.platforms);
        for (std::size_t i = 0, n = platforms.size(); i < n; ++i) {
            description += platforms[i];
            if (i < n - 1) {
                description += ", ";
            }
        }

        description += "\n";
    }

    description += "\n";
    description += layer.manifest_path + "\n";
    description += format("- %s Layers Path \n", GetLayerTypeLabel(layer.type));
    description += "- File Format: " + layer.file_format_version.str() + "\n";
    description += "- Layer Binary Path:\n    " + layer.binary_path + "\n";
    description += "\n";
    description +=
        format("Total Settings Count: %d - Total Presets Count: %d", CountSettings(layer.settings), layer.presets.size());
    return description;
}

void Alert::LayerProperties(const Layer* layer) {
    assert(layer != nullptr);

    std::string title = layer->key;
    if (layer->status != STATUS_STABLE) {
        title += format(" (%s)", GetToken(layer->status));
    }

    std::string text;
    if (!layer->introduction.empty()) {
        text += layer->introduction + "\n\n";
    }
    text += BuildPropertiesLog(*layer);

    QMessageBox alert;
    alert.setWindowTitle(title.c_str());
    alert.setText(text.c_str());
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Information);
    alert.exec();
}
