#include <Crochet/Core/Window.h>
#include <Crochet/Platform/GLFW/GLFWWindow.h>

namespace Crochet
{
    CrResult crCreateWindow(const CrWindowCreateInfo* pCreateInfo, CrWindow** ppWindow)
    {
        if(!pCreateInfo || !ppWindow)
        {
            return CR_ERROR_INVALID_ARGUMENT;
        }

        switch(pCreateInfo->backend)
        {
            case CR_WINDOW_BACKEND_GLFW:

        }
    }
}