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
#include "../vkconfig_core/application_singleton.h"
#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/configurator_signal.h"

#include <QApplication>
#include <QGuiApplication>
#include <QMessageBox>

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
#else
    (void)type;
    (void)context;
    (void)msg;
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    qputenv("QT_QPA_PLATFORMTHEME", "");

    qInstallMessageHandler(log_handler);

    QApplication app(argc, argv);

    //if (VKC_ENV == VKC_ENV_WIN32) {
    //    app.setStyle("fusion");
    //}

    // This has to go after the construction of QApplication in
    // order to use a QMessageBox and avoid some QThread warnings.
    ApplicationSingleton singleton("vkconfig_single_instance");
    while (singleton.IsLocked()) {
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
        int button = alert.exec();

        if (button == QMessageBox::Cancel) {
            return -1;
        }
    }

    Configurator& configurator = Configurator::Get();
    configurator.Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    configurator.vulkan_system_info = BuildVulkanSystemInfo();

    if (configurator.vulkan_system_info.loaderVersion == Version::NONE) {
        QMessageBox alert;
        alert.QDialog::setWindowTitle("Vulkan Configurator failed to start...");
        alert.setText("Could not find a Vulkan Loader. Please install the Vulkan SDK.");
        alert.setInformativeText("<a href=\"https://vulkan.lunarg.com/sdk/home\">https://vulkan.lunarg.com/sdk/home</a>");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
        return -1;
    }

    if (configurator.vulkan_system_info.loaderVersion < Version::REQUIRED_LOADER_VERSION) {
        QMessageBox alert;
        alert.setWindowTitle("Vulkan Configurator failed to start...");
        alert.setText(
            format("The system has Vulkan Loader version % s but version %s is required. Please update the Vulkan Runtime.",
                   configurator.vulkan_system_info.loaderVersion.str().c_str(), Version::REQUIRED_LOADER_VERSION.str().c_str())
                .c_str());
        alert.setInformativeText("<a href=\"https://vulkan.lunarg.com/sdk/home\">https://vulkan.lunarg.com/sdk/home</a>");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
        return -1;
    }

    if (configurator.vulkan_system_info.physicalDevices.empty()) {
        QMessageBox alert;
        alert.setWindowTitle("Vulkan Configurator failed to start...");
        alert.setText("Cannot find any Vulkan Physical Devices.");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
        return -1;
    }

    const bool init = configurator.Init(CONFIGURATOR_MODE_GUI);
    int result = 0;

    if (init) {
        // The main GUI is driven here
        MainWindow main_window(app);
        main_window.show();

        result = app.exec();
    }

    configurator.Surrender(OVERRIDE_AREA_ALL);

    if (configurator.reset_hard) {
        configurator.Release();

        singleton.Release();

        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    } else {
        configurator.Release();
    }

    return result;
}
