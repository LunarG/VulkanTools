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

#include "main_reset.h"

#include "../vkconfig_core/configurator.h"

#include <cassert>

static int RunReset(int argc, char* argv[], bool hard) {
    Configurator& configurator = Configurator::Get();
    configurator.Init();
    configurator.Reset(hard);

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
