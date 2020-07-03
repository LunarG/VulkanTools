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
 * The vkConfig2 program monitors and adjusts the Vulkan configuration
 * environment. These settings are wrapped in this class, which serves
 * as the "model" of the system.
 *
 * Authors:
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

class Preferences {
   public:
    static const Preferences& Get();

    // When the user click on the Vulkan Layers Configurations tree,
    // the user has to click on the ratio button is used to activate the configuration
    // and the row is used to edit the configuration.
    // Alternatively, selection and activation is done together wherever we click.
    bool use_separated_select_and_activate;

   private:
    Preferences();
    ~Preferences();
    Preferences(const Preferences&) = delete;
    Preferences& operator=(const Preferences&) = delete;
};
