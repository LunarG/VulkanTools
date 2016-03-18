#include "glvdebug_controller_factory.h"
#include "vktrace_platform.h"

glvdebug_controller_factory::glvdebug_controller_factory()
{
}

glvdebug_controller_factory::~glvdebug_controller_factory()
{
}

glvdebug_QController *glvdebug_controller_factory::Load(const char* filename)
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

    glvdebug_QController* pController = NULL;
    funcptr_glvdebug_CreateGlvdebugQController CreateGlvdebugQController = (funcptr_glvdebug_CreateGlvdebugQController)vktrace_platform_get_library_entrypoint(pLibrary, "CreateGlvdebugQController");
    funcptr_glvdebug_DeleteGlvdebugQController DeleteGlvdebugQController = (funcptr_glvdebug_DeleteGlvdebugQController)vktrace_platform_get_library_entrypoint(pLibrary, "DeleteGlvdebugQController");
    if (CreateGlvdebugQController == NULL)
    {
        vktrace_LogError("Controller '%s' is missing entrypoint 'CreateGlvdebugQController'.\n", filename);
    }
    if (DeleteGlvdebugQController == NULL)
    {
        vktrace_LogError("Controller '%s' is missing entrypoint 'DeleteGlvdebugQController'.\n", filename);
    }

    if (CreateGlvdebugQController != NULL &&
        DeleteGlvdebugQController != NULL)
    {
        pController = CreateGlvdebugQController();
    }

    if (pController != NULL)
    {
        m_controllerToLibraryMap[pController] = pLibrary;
    }

    return pController;
}

void glvdebug_controller_factory::Unload(glvdebug_QController** ppController)
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
        funcptr_glvdebug_DeleteGlvdebugQController DeleteGlvdebugQController = (funcptr_glvdebug_DeleteGlvdebugQController)vktrace_platform_get_library_entrypoint(pLibrary, "DeleteGlvdebugQController");
        if (DeleteGlvdebugQController != NULL)
        {
            DeleteGlvdebugQController(*ppController);
            *ppController = NULL;
        }

        vktrace_platform_close_library(pLibrary);
    }
}
