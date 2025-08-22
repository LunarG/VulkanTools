/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include "../configurator.h"

#include <gtest/gtest.h>

#include <vector>
#include <array>

// #include "Validation.hpp"
// LayerSettings layer_settings;

TEST(test_export, extension_code) {
    Configurator& configurator = Configurator::Get();
    configurator.Init(CONFIGURATOR_MODE_NONE);

    configurator.WriteExtensionCode("./VK_EXT_layer_settings.txt");  // .txt to avoid C++ code formating...

    if (VKC_ENV == VKC_ENV_WIN32) {
        QFile file_reference(":/test/export/windows/VK_EXT_layer_settings.txt");
        if (!file_reference.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }

        QString data_reference = file_reference.readAll();
        file_reference.close();

        QFile file_generated("./VK_EXT_layer_settings.txt");
        if (!file_generated.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }

        QString data_generated = file_generated.readAll();
        file_generated.close();

        EXPECT_STREQ(data_reference.toStdString().c_str(), data_generated.toStdString().c_str());
    }
}

TEST(test_export, vk_layer_settings_txt) {
    Configurator& configurator = Configurator::Get();
    configurator.Init(CONFIGURATOR_MODE_NONE);

    configurator.WriteLayersSettings(OVERRIDE_AREA_LAYERS_SETTINGS_BIT, "./vk_layer_settings.txt");

    if (VKC_ENV == VKC_ENV_WIN32) {
        QFile file_reference(":/test/export/windows/vk_layer_settings.txt");
        if (!file_reference.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }

        QString data_reference = file_reference.readAll();
        file_reference.close();

        QFile file_generated("./vk_layer_settings.txt");
        if (!file_generated.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }

        QString data_generated = file_generated.readAll();
        file_generated.close();

        EXPECT_STREQ(data_reference.toStdString().c_str(), data_generated.toStdString().c_str());
    }
}

TEST(test_export, environment_variables_bash) {
    Configurator& configurator = Configurator::Get();
    configurator.Init(CONFIGURATOR_MODE_NONE);

    configurator.Export(EXPORT_ENV_BASH, "./environment_variables.sh");

    if (VKC_ENV == VKC_ENV_WIN32) {
        QFile file_reference(":/test/export/windows/environment_variables.sh");
        if (!file_reference.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }

        QString data_reference = file_reference.readAll();
        file_reference.close();

        QFile file_generated("./environment_variables.sh");
        if (!file_generated.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }

        QString data_generated = file_generated.readAll();
        file_generated.close();

        EXPECT_STREQ(data_reference.toStdString().c_str(), data_generated.toStdString().c_str());
    }
}

TEST(test_export, environment_variables_cmd) {
    Configurator& configurator = Configurator::Get();
    configurator.Init(CONFIGURATOR_MODE_NONE);

    configurator.Export(EXPORT_ENV_CMD, "./environment_variables.bat");

    if (VKC_ENV == VKC_ENV_WIN32) {
        QFile file_reference(":/test/export/windows/environment_variables.bat");
        if (!file_reference.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }

        QString data_reference = file_reference.readAll();
        file_reference.close();

        QFile file_generated("./environment_variables.bat");
        if (!file_generated.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }

        QString data_generated = file_generated.readAll();
        file_generated.close();

        EXPECT_STREQ(data_reference.toStdString().c_str(), data_generated.toStdString().c_str());
    }
}
