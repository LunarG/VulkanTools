/*
 * Copyright (c) 2026 LunarG, Inc.
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
 */

#include "monitor_settings.h"

#include "monitor_overlay.h"

namespace monitor {

template <typename T>
static void Read(VkuLayerSettingSet settings, const char *key, T &value) {
    if (vkuHasLayerSetting(settings, key)) {
        vkuGetLayerSettingValue(settings, key, value);
    }
}

RawSettings ReadRawSettings(VkuLayerSettingSet settings) {
    RawSettings raw;
    if (settings == VK_NULL_HANDLE) {
        return raw;
    }
    Read(settings, "position", raw.position);
    Read(settings, "scale", raw.scale);
    Read(settings, "update_interval_ms", raw.update_interval_ms);
    Read(settings, "show_fps", raw.show_fps);
    Read(settings, "show_frame_time", raw.show_frame_time);
    Read(settings, "show_frame_number", raw.show_frame_number);
    Read(settings, "log_warnings", raw.log_warnings);
    return raw;
}

}  // namespace monitor
