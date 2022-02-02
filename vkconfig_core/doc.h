/*
 * Copyright (c) 2020-2022 Valve Corporation
 * Copyright (c) 2020-2022 LunarG, Inc.
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
 * - David Pinedo <david@lunarg.com>
 */

#include "layer.h"
#include "environment.h"
#include "configuration.h"

void ExportHtmlDoc(const Layer& layer, const std::string& path);

void ExportMarkdownDoc(const Layer& layer, const std::string& path);

void ExportSettingsDoc(const std::vector<Layer>& available_layers,
                       const Configuration& configuration, const std::string& path);
