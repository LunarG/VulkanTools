/*
 * Copyright (c) 2016-2021 Valve Corporation
 * Copyright (c) 2016-2021 LunarG, Inc.
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
 * Author: Mark Young <marky@lunarg.com>
 */

#ifdef VIA_WINDOWS_TARGET
#include "via_system_windows.hpp"
#elif VIA_LINUX_TARGET
#include "via_system_linux.hpp"
#elif VIA_MACOS_TARGET
#include "via_system_macos.hpp"
#elif VIA_BSD_TARGET
#include "via_system_bsd.hpp"
#else
#error("Unsupported target")
#endif

int main(int argc, char** argv) {
    int success = 0;
#ifdef VIA_WINDOWS_TARGET
    ViaSystem* via_system = reinterpret_cast<ViaSystem*>(new ViaSystemWindows());
#elif VIA_LINUX_TARGET
    ViaSystem* via_system = reinterpret_cast<ViaSystem*>(new ViaSystemLinux());
#elif VIA_MACOS_TARGET
    ViaSystem* via_system = reinterpret_cast<ViaSystem*>(new ViaSystemMacOS());
#elif VIA_BSD_TARGET
    ViaSystem* via_system = reinterpret_cast<ViaSystem*>(new ViaSystemBSD());
#endif

    if (!via_system->Init(argc, argv) || !via_system->GenerateInfo()) {
        success = -1;
    }

    delete via_system;
    return success;
}
