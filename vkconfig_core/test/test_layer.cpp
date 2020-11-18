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

#include <gtest/gtest.h>

#include "../layer.h"

bool operator==(const Layer& a, const Layer& b) {
    if (a.file_format_version != b.file_format_version)
        return false;
    else if (a.key != b.key)
        return false;
    else if (a._type != b._type)
        return false;
    else if (a._library_path != b._library_path)
        return false;
    else if (a._api_version != b._api_version)
        return false;
    else if (a._implementation_version != b._implementation_version)
        return false;
    else if (a.description != b.description)
        return false;
    else if (a._layer_path != b._layer_path)
        return false;
    else if (a._layer_type != b._layer_type)
        return false;

    return true;
}

bool operator!=(const Layer& a, const Layer& b) { return !(a == b); }
