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

#include <QApplication>
#include <QProcess>

#include "main_reset.h"
#include "main_layers.h"
#include "main_loader.h"
#include "main_doc.h"
#include "main_settings.h"

#include "../vkconfig_core/path.h"
#include "../vkconfig_core/version.h"
#include "../vkconfig_core/application_singleton.h"
#include "../vkconfig_core/configurator_signal.h"
#include "../vkconfig_core/executable_manager.h"
#include "../vkconfig_core/vulkan_util.h"

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
    QCoreApplication::setOrganizationName("LunarG");
    QCoreApplication::setOrganizationDomain("lunarg.com");

    // This is used by QSettings for .ini, registry, and .plist files.
    // It needs to not have spaces in it, and by default is the same as
    // the executable name. If we rename the executable at a later date,
    // keeping this as 'vkconfig' will ensure that it picks up the
    // settings from the previous version (assuming that's ever an issue)
    QCoreApplication::setApplicationName(VKCONFIG_SHORT_NAME);

    qInstallMessageHandler(log_handler);

    QApplication app(argc, argv);

    // This has to go after the construction of QApplication in
    // order to use a QMessageBox and avoid some QThread warnings.
    ApplicationSingleton singleton("vkconfig_single_instance");
    if (singleton.IsLocked()) {
        fprintf(stderr, "%s: [ERROR] %s GUI is running which is incompatible with %s command line.", VKCONFIG_SHORT_NAME,
                VKCONFIG_NAME, VKCONFIG_NAME);
        return -1;
    }

    const VulkanSystemInfo& vulkan_info = ::BuildVulkanSystemInfo();

    if (vulkan_info.loaderVersion == Version::NONE) {
        fprintf(stderr, "%s: [ERROR] Could not find a Vulkan Loader.", VKCONFIG_SHORT_NAME);
        return -1;
    }

    if (vulkan_info.loaderVersion < Version::REQUIRED_LOADER_VERSION) {
        fprintf(stderr,
                "%s: [ERROR] The system has Vulkan Loader version %s but version %s is required. Please update the Vulkan Runtime "
                "at https://vulkan.lunarg.com/sdk/home",
                VKCONFIG_SHORT_NAME, vulkan_info.loaderVersion.str().c_str(), Version::REQUIRED_LOADER_VERSION.str().c_str());
        return -1;
    }

    if (vulkan_info.physicalDevices.empty()) {
        fprintf(stderr, "%s: [ERROR] Cannot find any Vulkan Physical Devices.", VKCONFIG_SHORT_NAME);
        return -1;
    }

    const CommandLine command_line(argc, argv);

    if (command_line.error != ERROR_NONE) {
        command_line.log();
        command_line.usage();
        return -1;
    }

    ::InitSignals();

    switch (command_line.command) {
        case COMMAND_SHOW_USAGE: {
            command_line.usage();
            return 0;
        }
        case COMMAND_VERSION: {
            command_line.version();
            return 0;
        }
        case COMMAND_GUI: {
            singleton.Release();

            const DefaultExecutable defaults_executable{::GetExecutable(EXECUTABLE_VKCONFIG_GUI), "vkconfig-gui"};
            const Executable executable(defaults_executable);
            const ExecutableOptions* options = executable.GetActiveOptions();

            QProcess* process = new QProcess(&app);

            process->setProgram(executable.path.AbsolutePath().c_str());
            process->setWorkingDirectory(options->working_folder.AbsolutePath().c_str());
            bool result = process->startDetached(nullptr);

            return result ? 0 : 1;
        }
        case COMMAND_LAYERS: {
            return run_layers(command_line);
        }
        case COMMAND_LOADER: {
            return run_loader(command_line);
        }
        case COMMAND_RESET: {
            return run_reset(argc, argv, command_line);
        }
        case COMMAND_DOC: {
            return run_doc(command_line);
        }
        case COMMAND_SETTINGS: {
            return run_generate_settings(command_line);
        }
        default: {
            assert(0);
            return -1;
        }
    }
}
