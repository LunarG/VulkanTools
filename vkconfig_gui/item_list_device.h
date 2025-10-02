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

#pragma once

#include "../vkconfig_core/util.h"
#include "../vkconfig_core/vulkan_util.h"

#include <QListWidgetItem>

#include <string>

class ListItemDevice : public QListWidgetItem {
   public:
    ListItemDevice(const DeviceInfo &info, const std::string &version)
        : QListWidgetItem(format("%s (%s)", info.deviceName.c_str(), version.c_str()).c_str()), info(info) {}

    DeviceInfo info;

   private:
    ListItemDevice(const ListItemDevice &) = delete;
    ListItemDevice &operator=(const ListItemDevice &) = delete;
};
