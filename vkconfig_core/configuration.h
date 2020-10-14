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

#pragma once

#include "parameter.h"

#include <QString>
#include <QStringList>

#include <vector>

class Configuration {
   public:
    Configuration();

    bool Load(const QString& full_path);
    bool Save(const QString& full_path) const;

    QString name;                    // User readable display of the profile name (may contain spaces)
    QString _description;            // A friendly description of what this profile does
    QByteArray _setting_tree_state;  // Recall editor tree state

    std::vector<Parameter> parameters;

    bool IsEmpty() const;
};

QString MakeConfigurationName(const std::vector<Configuration>& configurations, const QString& configuration_name);
