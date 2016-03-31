/**************************************************************************
*
* Copyright 2014-2016 Valve Corporation
* Copyright (C) 2014-2016 LunarG, Inc.
* All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* Author: Peter Lohrmann <peterl@valvesoftware.com> <plohrmann@gmail.com>
**************************************************************************/
#include "vktraceviewer_controller_factory.h"
#include "vktrace_platform.h"

vktraceviewer_controller_factory::vktraceviewer_controller_factory()
{
}

vktraceviewer_controller_factory::~vktraceviewer_controller_factory()
{
}

vktraceviewer_QController *vktraceviewer_controller_factory::Load(const char* filename)
{
    void* pLibrary = vktrace_platform_open_library(filename);
    if (pLibrary == NULL)
    {
        vktrace_LogError("Failed to load controller '%s'", filename);
#if defined(PLATFORM_LINUX)
        char* error = dlerror();
        vktrace_LogError("Due to: %s", error);
#endif
        return NULL;
    }

    vktraceviewer_QController* pController = NULL;
    funcptr_vktraceviewer_CreateQController CreateQController = (funcptr_vktraceviewer_CreateQController)vktrace_platform_get_library_entrypoint(pLibrary, "vtvCreateQController");
    funcptr_vktraceviewer_DeleteQController DeleteQController = (funcptr_vktraceviewer_DeleteQController)vktrace_platform_get_library_entrypoint(pLibrary, "vtvDeleteQController");
    if (CreateQController == NULL)
    {
        vktrace_LogError("Controller '%s' is missing entrypoint 'vtvCreateQController'.\n", filename);
    }
    if (DeleteQController == NULL)
    {
        vktrace_LogError("Controller '%s' is missing entrypoint 'vtvDeleteQController'.\n", filename);
    }

    if (CreateQController != NULL &&
        DeleteQController != NULL)
    {
        pController = CreateQController();
    }

    if (pController != NULL)
    {
        m_controllerToLibraryMap[pController] = pLibrary;
    }

    return pController;
}

void vktraceviewer_controller_factory::Unload(vktraceviewer_QController** ppController)
{
    assert(ppController != NULL);
    assert(*ppController != NULL);

    void* pLibrary = m_controllerToLibraryMap[*ppController];
    if (pLibrary == NULL)
    {
        vktrace_LogError("NULL Library encountered while unloading controller.");
    }
    else
    {
        funcptr_vktraceviewer_DeleteQController DeleteQController = (funcptr_vktraceviewer_DeleteQController)vktrace_platform_get_library_entrypoint(pLibrary, "vtvDeleteQController");
        if (DeleteQController != NULL)
        {
            DeleteQController(*ppController);
            *ppController = NULL;
        }

        vktrace_platform_close_library(pLibrary);
    }
}
