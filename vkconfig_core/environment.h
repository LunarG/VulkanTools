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

#pragma once

#include "version.h"
#include "type_tab.h"
#include "type_log.h"
#include "path.h"
#include "serialization.h"

#include <QByteArray>

#include <array>
#include <vector>
#include <string>

class Environment : public Serialize {
   public:
    Environment();
    ~Environment();

    bool Load(const QJsonObject& json_root_object) override;
    bool Save(QJsonObject& json_root_object) const override;
    void Reset() override;

    LogFlags GetLoaderMessageFlags() const { return this->loader_message_types_flags; }
    void SetLoaderMessageFlags(LogFlags flags) { this->loader_message_types_flags = flags; }

    bool has_crashed = false;
    int hide_message_boxes_flags = 0;
    TabType active_tab = TAB_CONFIGURATIONS;

   private:
    Environment(const Environment&) = delete;
    Environment& operator=(const Environment&) = delete;

    LogFlags loader_message_types_flags;
    Path home_sdk_path;
};
