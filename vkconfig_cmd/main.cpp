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

#include <QApplication>

#include "main_reset.h"
#include "main_layers.h"
#include "main_doc.h"

#include "../vkconfig_core/path.h"
#include "../vkconfig_core/version.h"
#include "../vkconfig_core/configurator_signal.h"

int main(int argc, char* argv[]) {
    QCoreApplication::setOrganizationName("LunarG");
    QCoreApplication::setOrganizationDomain("lunarg.com");

    // This is used by QSettings for .ini, registry, and .plist files.
    // It needs to not have spaces in it, and by default is the same as
    // the executable name. If we rename the executable at a later date,
    // keeping this as 'vkconfig' will ensure that it picks up the
    // settings from the previous version (assuming that's ever an issue)
    QCoreApplication::setApplicationName(VKCONFIG_SHORT_NAME);

    QApplication app(argc, argv);

    const CommandLine command_line(argc, argv);

    if (command_line.error != ERROR_NONE) {
        command_line.log();
        command_line.usage();
        return -1;
    }

    InitSignals();

    switch (command_line.command) {
        case COMMAND_SHOW_USAGE: {
            command_line.usage();
            return 0;
        }
        case COMMAND_VERSION: {
            command_line.version();
            return 0;
        }
        case COMMAND_LAYERS: {
            return run_layers(command_line);
        }
        case COMMAND_RESET: {
            return run_reset(argc, argv, command_line);
        }
        case COMMAND_DOC: {
            return run_doc(command_line);
        }
        default: {
            assert(0);
            return -1;
        }
    }
}
