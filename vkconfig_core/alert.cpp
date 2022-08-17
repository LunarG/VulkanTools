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
#include <QCheckBox>

void Alert::LoaderFailure() {
    QMessageBox alert;
    alert.QDialog::setWindowTitle("Vulkan Development Status failure...");
    alert.setText("Could not find a Vulkan Loader.");
    alert.setIcon(QMessageBox::Critical);
    alert.exec();
}

void Alert::InstanceFailure() {
    QMessageBox alert;
    alert.QDialog::setWindowTitle("Vulkan Development Status failure...");
    alert.setText("Cannot find a compatible Vulkan installable client driver (ICD).");
    alert.setIcon(QMessageBox::Critical);
    alert.exec();
}

void Alert::PhysicalDeviceFailure() {
    QMessageBox alert;
    alert.setWindowTitle("Vulkan Development Status failure...");
    alert.setText("Cannot find any Vulkan Physical Devices.");
    alert.setIcon(QMessageBox::Critical);
    alert.exec();
}

void Alert::ApplicationListUnsupported(const char* message) {
    QMessageBox alert;
    alert.QDialog::setWindowTitle("Layers override of a selected list of Vulkan Applications is not available");
    alert.setTextFormat(Qt::RichText);
    alert.setText(message);
    alert.setInformativeText(
        "In order to apply layers override to only a selected list of Vulkan applications, get the latest Vulkan Runtime from "
        "<a href='https://vulkan.lunarg.com/sdk/home'>HERE.</a> to use this feature or update your Vulkan drivers");
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

void Alert::ApplicationListEmpty() {
    QMessageBox alert;
    alert.setIcon(QMessageBox::Warning);
    alert.QDialog::setWindowTitle("Vulkan Layers overriding will apply globally.");
    alert.setText("The application list to override is empty. Restricting layers overriding to the selected list is disabled.");
    alert.setInformativeText("As a result, Vulkan Layers overriding will apply globally, to all Vulkan applications.");
    alert.exec();
}

void Alert::LayerInvalid(const char* path, const char* message) {
    const std::string text = format("%s is not a valid layer manifest. \n\n", path) + message;

    QMessageBox alert;
    alert.QDialog::setWindowTitle("Failed to load a layer manifest...");
    alert.setText(text.c_str());
    alert.setInformativeText("The layer is being ignored.");
    alert.setIcon(QMessageBox::Critical);
    alert.exec();
}

QMessageBox::Button Alert::LayerImplicitExcluded(const char* layer_name) {
    const char* text = "%s was excluded but it is an implicit layer. This may cause undefined behavior, including crashes.";

    QMessageBox alert;
    alert.QDialog::setWindowTitle("Implicit layer excluded...");
    alert.setText(format(text, layer_name).c_str());
    alert.setInformativeText("Do you want to continue?");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setIcon(QMessageBox::Warning);
    return static_cast<QMessageBox::Button>(alert.exec());
}

QMessageBox::Button Alert::LayerProfiles() {
    QMessageBox alert;
    alert.QDialog::setWindowTitle("Overridding or excluding ALL explicit layers is recommended");
    alert.setText(
        "VK_LAYER_KHRONOS_profiles requires being executed close to the Vulkan drivers. However, "
        "application-controlled layers are executed after Vulkan Configurator overridden layers.");
    alert.setInformativeText("Do you want to override ALL explicit layers too?");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setIcon(QMessageBox::Warning);
    return static_cast<QMessageBox::Button>(alert.exec());
}

QMessageBox::Button Alert::ConfiguratorSingleton() {
    QMessageBox alert;
    alert.QDialog::setWindowTitle(format("Cannot start a new instance of %s", VKCONFIG_NAME).c_str());
    alert.setIcon(QMessageBox::Critical);
    alert.setDefaultButton(QMessageBox::Cancel);
    alert.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    alert.setText(format("Another instance of %s is currently running. Please close it to continue.", VKCONFIG_NAME).c_str());
    alert.setInformativeText(format("Press OK to continue launching the new instance of %s when the other instance is "
                                    "stopped.\nPress CANCEL to stop the launch of a new %s instance.",
                                    VKCONFIG_NAME, VKCONFIG_NAME)
                                 .c_str());

    return static_cast<QMessageBox::Button>(alert.exec());
}

void Alert::ConfiguratorRestart() {
    const char* text =
        "Vulkan Layers are fully configured when creating a Vulkan Instance which typically happens at Vulkan Application start.";

    QMessageBox alert;
    alert.QDialog::setWindowTitle("Any change requires Vulkan Applications restart");
    alert.setText(text);
    alert.setInformativeText("For changes to take effect, running Vulkan Applications should be restarted.");
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

QMessageBox::Button Alert::ConfiguratorResetAll() {
    QMessageBox alert;
    alert.QDialog::setWindowTitle("Restoring and Resetting all Layers Configurations to default");
    alert.setText(
        "You are about to delete all the user-defined configurations and resetting all default configurations to their default "
        "state.");
    alert.setInformativeText("Are you sure you want to continue?");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::Yes);
    alert.setIcon(QMessageBox::Warning);

    return static_cast<QMessageBox::Button>(alert.exec());
}

QMessageBox::Button Alert::ConfiguratorReloadDefault() {
    QMessageBox alert;
    alert.QDialog::setWindowTitle("Reloading Missing Default Configurations...");
    alert.setText("Are you sure you want to reload the default configurations?");
    alert.setInformativeText("Add missing default configurations. Existing configurations are preserved.");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::No);
    alert.setIcon(QMessageBox::Warning);

    return static_cast<QMessageBox::Button>(alert.exec());
}

QMessageBox::Button Alert::ConfiguratorCrashed() {
    QMessageBox alert;
    alert.QDialog::setWindowTitle(format("%s crashed during last run...", VKCONFIG_NAME).c_str());
    alert.setText("Do you want to reset to default resolve the issue?");
    alert.setInformativeText("All layers configurations will be lost...");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::No);
    alert.setIcon(QMessageBox::Critical);
    return static_cast<QMessageBox::Button>(alert.exec());
}

void Alert::ConfigurationRenamingFailed() {
    QMessageBox alert;
    alert.QDialog::setWindowTitle("Renaming of the layers configuration failed...");
    alert.setText("There is already a configuration with the same name.");
    alert.setInformativeText("Use a different name for the configuration.");
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

void Alert::ConfigurationNameEmpty() {
    QMessageBox alert;
    alert.QDialog::setWindowTitle("Renaming of the layers configuration failed...");
    alert.setText("The configuration name is empty.");
    alert.setInformativeText("The configuration name is required.");
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

void Alert::ConfigurationNameInvalid() {
    QMessageBox alert;
    alert.QDialog::setWindowTitle("Invalid name for a configuration...");
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
    alert.QDialog::setWindowTitle(title.c_str());
    alert.setText(text.c_str());
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Information);
    alert.exec();
}

QMessageBox::Button Alert::LayerIncompatibleVersions(const char* message, const Version& loader_version) {
    QMessageBox alert;
    alert.setWindowTitle("Incompatible layers versions");
    alert.setText(format("The system has Vulkan Loader %s. The Vulkan Loader 1.3.211 and older requires that the layers use the "
                         "same Vulkan Headers minor version.",
                         loader_version.str().c_str())
                      .c_str());
    alert.setInformativeText(message);
    alert.setIcon(QMessageBox::Critical);
    return static_cast<QMessageBox::Button>(alert.exec());
}

void Alert::LogFileFailed() {
    QMessageBox alert;
    alert.setText("Cannot open log file");
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

void Alert::FileNotFound(const char* message) {
    QMessageBox alert;
    alert.setText(format("%s file not found.", message).c_str());
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

void Alert::FileNotJson(const char* message) {
    QMessageBox alert;
    alert.setText(format("%s is not a JSON file.", message).c_str());
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

void Alert::FileNotProfile(const char* message) {
    QMessageBox alert;
    alert.setText(format("%s is not a JSON profile file.", message).c_str());
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}
