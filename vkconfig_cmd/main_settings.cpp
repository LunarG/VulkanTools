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

#include "main_settings.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/generate_settings_files.h"

int run_generate_settings(const CommandLine& command_line) {
    assert(command_line.command == COMMAND_SETTINGS);
    assert(command_line.error == ERROR_NONE);

    ConfiguratorGuard configurator_guard(command_line.dry_run ? CONFIGURATOR_MODE_DRY : CONFIGURATOR_MODE_CMD);

    return generate_settings(configurator_guard.Get(), command_line);
}
