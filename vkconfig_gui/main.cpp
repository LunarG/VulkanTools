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

#include "mainwindow.h"

#include "../vkconfig_core/vulkan_util.h"
#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/application_singleton.h"
#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/configurator_signal.h"

#include <QApplication>

#include <cassert>

QtMessageHandler originalHandler = nullptr;

void log_handler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
#if _DEBUG
    QString message = qFormatLogMessage(type, context, msg);
    std::string printable_message = qPrintable(message);
    switch (type) {
        default:
            fprintf(stdout, "%s\n", printable_message.c_str());
            fflush(stdout);
            break;
        case QtCriticalMsg:
        case QtFatalMsg:
            fprintf(stderr, "%s\n", printable_message.c_str());
            fflush(stderr);
            break;
    }

    if (originalHandler) {
        (*originalHandler)(type, context, msg);
    }
#endif  //_DEBUG
}

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

    QtMessageHandler originalHandler = qInstallMessageHandler(log_handler);

    QApplication app(argc, argv);
    app.setStyle("Fusion");

    // This has to go after the construction of QApplication in
    // order to use a QMessageBox and avoid some QThread warnings.
    ApplicationSingleton singleton("vkconfig_single_instance");
    while (singleton.IsLocked()) {
        if (Alert::StartSingleton() == QMessageBox::Cancel) {
            return -1;
        }
    }

    Configurator& configurator = Configurator::Get();
    configurator.Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    configurator.vulkan_system_info = BuildVulkanSystemInfo();

    if (configurator.vulkan_system_info.loaderVersion == Version::NONE) {
        Alert::StartLoaderFailure();
        return -1;
    }

    if (configurator.vulkan_system_info.loaderVersion < Version::REQUIRED_LOADER_VERSION) {
        Alert::StartLoaderIncompatibleVersions(configurator.vulkan_system_info.loaderVersion, Version::REQUIRED_LOADER_VERSION);
        return -1;
    }

    if (configurator.vulkan_system_info.physicalDevices.empty()) {
        Alert::StartPhysicalDeviceFailure();
        return -1;
    }

    const bool init = configurator.Init();
    int result = 0;

    if (init) {
        // The main GUI is driven here
        MainWindow main_window;
        main_window.show();

        result = app.exec();
    }

    configurator.Surrender(OVERRIDE_AREA_ALL);

    if (configurator.reset_hard) {
        singleton.Release();

        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }

    return result;
}
