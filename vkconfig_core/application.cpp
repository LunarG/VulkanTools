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

#include "application.h"

#include <QDir>
#include <QFileInfo>

Application::Application(const std::string& name, const std::string& executable_full_path, const std::string& arguments)
    : app_name(name),
      executable_path(executable_full_path),
      working_folder(QFileInfo(executable_full_path.c_str()).path().toStdString()),
      arguments(arguments),
      log_file((QString(GetPath(BUILTIN_PATH_LOCAL).c_str()) + GetNativeSeparator() +
                QFileInfo(executable_full_path.c_str()).baseName() + ".txt")
                   .toStdString()
                   .c_str()),
      override_layers(true) {}
