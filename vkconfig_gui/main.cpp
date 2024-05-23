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
#include "vulkan_util.h"

#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/application_singleton.h"
#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/configurator_signal.h"

#include <QApplication>

#include <cassert>

int main(int argc, char* argv[]) {
    InitSignals();

    const VulkanSystemInfo& vulkan_info = BuildVulkanSystemInfo();

    QCoreApplication::setOrganizationName("LunarG");
    QCoreApplication::setOrganizationDomain("lunarg.com");

    // This is used by QSettings for .ini, registry, and .plist files.
    // It needs to not have spaces in it, and by default is the same as
    // the executable name. If we rename the executable at a later date,
    // keeping this as 'vkconfig' will ensure that it picks up the
    // settings from the previous version (assuming that's ever an issue)
    QCoreApplication::setApplicationName(VKCONFIG_SHORT_NAME);

    // Older Qt versions do not have this. Dynamically check the version
    // of Qt since it's just an enumerant. Versions 5.6.0 and later have
    // high dpi support. We really don't need to check the 5, but for
    // the sake of completeness and mabye compatibility with qt 6.
    // Also ignoring the trailing point releases
    const char* version = qVersion();
    int version_major, version_minor;
    sscanf(version, "%d.%d", &version_major, &version_minor);
    if (version_major >= 5 && version_minor >= 6) {
        // Qt::AA_EnableHighDpiScaling = 20  from qnamespace.h in Qt 5.6 or later
        QCoreApplication::setAttribute((Qt::ApplicationAttribute)20);
    }

    QApplication app(argc, argv);

    // This has to go after the construction of QApplication in
    // order to use a QMessageBox and avoid some QThread warnings.
    ApplicationSingleton singleton("vkconfig_single_instance");
    while (!singleton.IsFirstInstance()) {
        if (Alert::ConfiguratorSingleton() == QMessageBox::Cancel) {
            return -1;
        }
    }

    if (vulkan_info.loaderVersion == 0) {
        Alert::LoaderFailure();
        return -1;
    }

    if (Version(vulkan_info.loaderVersion) < Version("1.3.261")) {
        Alert::LoaderIncompatibleVersions(Version(vulkan_info.loaderVersion));
        return -1;
    }

    // We simply cannot run without any layers
    Configurator& configurator = Configurator::Get();
    if (!configurator.Init()) {
        return -1;
    }

    // The main GUI is driven here
    MainWindow main_window;
    main_window.show();

    return app.exec();
}
