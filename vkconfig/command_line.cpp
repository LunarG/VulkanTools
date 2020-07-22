/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "command_line.h"
#include "vku.h"
#include <string>
#include <cstdio>

CommandLine::CommandLine(int argc, char* argv[]) : mode(_mode), _mode(ModeExecute) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            _mode = ModeShowUsage;
            return;
        } else if (arg == "--unit-test") {
            _mode = ModeRunTest;
            return;
        }
    }
}

void CommandLine::usage() const {
    printf("Vulkan Configurator %s\n", vku::Version::header_version.str().c_str());
    printf("\n");
    printf("Usage\n");
    printf("\tvkconfig [options]\n");
    printf("\n");
    printf("Options\n");
    printf("\t--help                      = Print usage information and exit.\n");
    printf("\t--unit-test                 = Run unit tests and exit.\n");
    printf("\t                              Return the number of errors.\n");
}
