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

#include "main_reset.h"

#include "configurator.h"

#include <cassert>

#include <QApplication>

static int RunReset(int argc, char* argv[], bool hard) {
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

    Configurator::Get().ResetToDefault(hard);

    return 0;
}

int run_reset(int argc, char* argv[], const CommandLine& command_line) {
    assert(command_line.command == COMMAND_RESET);
    assert(command_line.error == ERROR_NONE);

    switch (command_line.command_reset_arg) {
        case COMMAND_RESET_SOFT: {
            return RunReset(argc, argv, false);
        }
        case COMMAND_RESET_HARD: {
            return RunReset(argc, argv, true);
        }
        default: {
            assert(0);
            return -1;
        }
    }
}
