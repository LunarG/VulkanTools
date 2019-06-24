/*
 * Copyright (c) 2015-2017 Valve Corporation
 * Copyright (c) 2015-2017 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Camden Stocker <camden@lunarg.com>
 */

#include <string>
#include <sstream>

class ResultCheck : public layer_factory {
   public:
    // Constructor for interceptor
    ResultCheck(){};

    void PostCallApiFunction(const char *api_name, VkResult result) {
        if ((result < 0) || (result == VK_INCOMPLETE)) {
            std::stringstream message;
            message << "API call " << api_name << " returned with return code " << result << " instead of VK_SUCCESS";
            Warning(message.str());
        }
    }
};

ResultCheck result_check;
