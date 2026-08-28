#include <Crochet/Platform/Platform.h>

#include <Crochet/Platform/GLFW/GLFWPlatform.h>

namespace Crochet
{
    CrResult crCreatePlatform(const CrPlatformCreateInfo* pCreateInfo, CrPlatform** ppPlatform)
    {
        if(!pCreateInfo || !ppPlatform)
        {
            return CR_ERROR_INVALID_ARGUMENT;
        }

        *ppPlatform = nullptr;

        switch(pCreateInfo->windowBackend)
        {
            case CR_WINDOW_BACKEND_GLFW:
            {
                CrGLFWPlatform* pPlatform = new CrGLFWPlatform();

                CrResult result = pPlatform->Initialize(pCreateInfo);
                if(result != CR_SUCCESS)
                {
                    delete pPlatform;

                    return result;
                }

                *ppPlatform = pPlatform;

                return CR_SUCCESS;
            }

            default:
            {
                return CR_ERROR_UNSUPPORTED;
            }
        }
    }

    CrResult crDestroyPlatform(CrPlatform** ppPlatform)
    {
        if(!ppPlatform || !*ppPlatform)
        {
            return CR_ERROR_INVALID_ARGUMENT;
        }

        CrResult result = (*ppPlatform)->Shutdown();
        if(result != CR_SUCCESS)
        {
            return result;
        }

        delete *ppPlatform;

        *ppPlatform = nullptr;

        return CR_SUCCESS;
    }
}