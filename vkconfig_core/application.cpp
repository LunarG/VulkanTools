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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "application.h"

#include <QDir>
#include <QFileInfo>

Application::Application(const QString& executable_full_path, const QString& arguments)
    : executable_path(executable_full_path.toStdString().c_str()),
      working_folder(QFileInfo(executable_full_path).path().toStdString().c_str()),
      arguments(arguments),
      log_file(
          (QDir::homePath() + GetNativeSeparator() + QFileInfo(executable_full_path).baseName() + ".txt").toStdString().c_str()),
      override_layers(true) {}
