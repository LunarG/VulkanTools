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

#include "mainwindow.h"

#include "../vkconfig_core/vulkan_util.h"
#include "../vkconfig_core/vulkan_util.h"
#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/application_singleton.h"
#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/configurator_signal.h"

#include <QApplication>

#include <cassert>

// TODO, until Loader 284 release...
#if true || VKC_ENV == VKC_ENV_UNIX
static const Version REQUIRED_LOADER_VERSION(1, 3, 261);
#elif VKC_ENV == VKC_ENV_WIN32
static const Version REQUIRED_LOADER_VERSION(1, 3, 284);
#else
#endif

int main(int argc, char* argv[]) {
    InitSignals();

    QCoreApplication::setOrganizationName("LunarG");
    QCoreApplication::setOrganizationDomain("lunarg.com");

    // This is used by QSettings for .ini, registry, and .plist files.
    // It needs to not have spaces in it, and by default is the same as
    // the executable name. If we rename the executable at a later date,
    // keeping this as 'vkconfig' will ensure that it picks up the
    // settings from the previous version (assuming that's ever an issue)
    QCoreApplication::setApplicationName(VKCONFIG_SHORT_NAME);

    // Qt::AA_EnableHighDpiScaling = 20  from qnamespace.h in Qt 5.6 or later
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    // This has to go after the construction of QApplication in
    // order to use a QMessageBox and avoid some QThread warnings.
    ApplicationSingleton singleton("vkconfig_single_instance");
    while (!singleton.IsFirstInstance()) {
        if (Alert::StartSingleton() == QMessageBox::Cancel) {
            return -1;
        }
    }

    Configurator& configurator = Configurator::Get();
    // configurator.Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    const VulkanSystemInfo& vulkan_info = BuildVulkanSystemInfo();

    if (vulkan_info.loaderVersion == Version::NONE) {
        Alert::StartLoaderFailure();
        return -1;
    }

    if (vulkan_info.loaderVersion < REQUIRED_LOADER_VERSION) {
        Alert::StartLoaderIncompatibleVersions(vulkan_info.loaderVersion, REQUIRED_LOADER_VERSION);
        return -1;
    }

    if (vulkan_info.physicalDevices.empty()) {
        Alert::StartPhysicalDeviceFailure();
        return -1;
    }

    if (!configurator.Init()) {
        return -1;
    }

    // The main GUI is driven here
    MainWindow main_window;
    main_window.show();

    return app.exec();
}
