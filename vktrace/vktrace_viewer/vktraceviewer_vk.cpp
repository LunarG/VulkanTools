/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation
 * Copyright (C) 2014-2016 LunarG, Inc.
 * All Rights Reserved.
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
 * Author: Peter Lohrmann <peterl@valvesoftware.com> <plohrmann@gmail.com>
 **************************************************************************/
#include "vktraceviewer_vk_qcontroller.h"
#include "vktraceviewer_controller.h"

extern "C" {
VKTRACER_EXPORT vktraceviewer_QController* VKTRACER_CDECL vtvCreateQController() {
    vktraceviewer_vk_QController* pController = new vktraceviewer_vk_QController();

    return (vktraceviewer_QController*)pController;
}

VKTRACER_EXPORT void VKTRACER_CDECL vtvDeleteQController(vktraceviewer_QController** ppController) {
    if (ppController != NULL && *ppController != NULL) {
        delete *ppController;
        *ppController = NULL;
    }
}
}
