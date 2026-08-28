#include <Crochet/Platform/GLFW/GLFWPlatform.h>

#include <GLFW/glfw3.h>

namespace Crochet
{
    CrResult CrGLFWPlatform::Initialize(const CrPlatformCreateInfo* pCreateInfo)
    {
        if(!glfwInit())
        {
            return CR_ERROR_INITIALIZATION_FAILED;
        }

        return CR_SUCCESS;
    }

    CrResult CrGLFWPlatform::Shutdown()
    {
        glfwTerminate();

        return CR_SUCCESS;
    }
}