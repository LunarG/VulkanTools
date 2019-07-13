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
#include "vktraceviewer_controller_factory.h"
#include "vktrace_platform.h"

vktraceviewer_controller_factory::vktraceviewer_controller_factory() {}

vktraceviewer_controller_factory::~vktraceviewer_controller_factory() {}

vktraceviewer_QController* vktraceviewer_controller_factory::Load(const char* filename) {
    void* pLibrary = vktrace_platform_open_library(filename);
    if (pLibrary == NULL) {
        vktrace_LogError("Failed to load controller '%s'", filename);
#if defined(PLATFORM_LINUX) || defined(PLATFORM_BSD)
        char* error = dlerror();
        vktrace_LogError("Due to: %s", error);
#endif
        return NULL;
    }

    vktraceviewer_QController* pController = NULL;
    funcptr_vktraceviewer_CreateQController CreateQController =
        (funcptr_vktraceviewer_CreateQController)vktrace_platform_get_library_entrypoint(pLibrary, "vtvCreateQController");
    funcptr_vktraceviewer_DeleteQController DeleteQController =
        (funcptr_vktraceviewer_DeleteQController)vktrace_platform_get_library_entrypoint(pLibrary, "vtvDeleteQController");
    if (CreateQController == NULL) {
        vktrace_LogError("Controller '%s' is missing entrypoint 'vtvCreateQController'.\n", filename);
    }
    if (DeleteQController == NULL) {
        vktrace_LogError("Controller '%s' is missing entrypoint 'vtvDeleteQController'.\n", filename);
    }

    if (CreateQController != NULL && DeleteQController != NULL) {
        pController = CreateQController();
    }

    if (pController != NULL) {
        m_controllerToLibraryMap[pController] = pLibrary;
    }

    return pController;
}

void vktraceviewer_controller_factory::Unload(vktraceviewer_QController** ppController) {
    assert(ppController != NULL);
    assert(*ppController != NULL);

    void* pLibrary = m_controllerToLibraryMap[*ppController];
    if (pLibrary == NULL) {
        vktrace_LogError("NULL Library encountered while unloading controller.");
    } else {
        funcptr_vktraceviewer_DeleteQController DeleteQController =
            (funcptr_vktraceviewer_DeleteQController)vktrace_platform_get_library_entrypoint(pLibrary, "vtvDeleteQController");
        if (DeleteQController != NULL) {
            DeleteQController(*ppController);
            *ppController = NULL;
        }

        vktrace_platform_close_library(pLibrary);
    }
}
