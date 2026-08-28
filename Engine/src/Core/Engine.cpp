#include <Crochet/Core/Engine.h>

#include <Crochet/Platform/Platform.h>
#include <Crochet/Core/Window.h>

namespace Crochet
{
    CrResult crCreateEngine(const CrEngineCreateInfo* pCreateInfo, CrEngine** ppEngine)
    {
        if(!pCreateInfo || !ppEngine)
        {
            return CR_ERROR_INVALID_ARGUMENT;
        }

        CrResult result;
        *ppEngine = nullptr;

        CrEngine* pEngine = new CrEngine();
        result = pEngine->Initialize(pCreateInfo);
        if(result != CR_SUCCESS)
        {
            delete pEngine;

            return result;
        }

        *ppEngine = pEngine;

        return CR_SUCCESS;
    }

    CrResult crDestroyEngine(CrEngine** ppEngine)
    {
        if(!ppEngine || !*ppEngine)
        {
            return CR_ERROR_INVALID_ARGUMENT;
        }

        CrResult result = (*ppEngine)->Shutdown();
        if(result != CR_SUCCESS)
        {
            return result;
        }

        delete *ppEngine;
        *ppEngine = nullptr;

        return CR_SUCCESS;
    }

    CrResult CrEngine::Initialize(const CrEngineCreateInfo* pCreateInfo)
    {
        CrResult result;

        CrPlatform* pPlatform = nullptr;
        CrWindow* pWindow = nullptr;
        
        CrPlatformCreateInfo platformCreateInfo = {};
        platformCreateInfo.sType = CR_STRUCTURE_TYPE_PLATFORM_CREATE_INFO;
        platformCreateInfo.windowBackend = pCreateInfo->windowBackend;

        result = crCreatePlatform(&platformCreateInfo, &pPlatform);
        if(result != CR_SUCCESS)
        {
            return result;
        }

        mPlatform = pPlatform;

        CrWindowCreateInfo windowCreateInfo = {};
        windowCreateInfo.sType = CR_STRUCTURE_TYPE_WINDOW_CREATE_INFO;
        windowCreateInfo.title = pCreateInfo->title;
        windowCreateInfo.width = pCreateInfo->windowWidth;
        windowCreateInfo.height = pCreateInfo->windowHeight;
        windowCreateInfo.clientAPI = pCreateInfo->windowClientAPI;
        windowCreateInfo.backend = pCreateInfo->windowBackend;

        result = crCreateWindow(&windowCreateInfo, &pWindow);
        if(result != CR_SUCCESS)
        {
            return result;
        }

        mWindow = pWindow;

        return CR_SUCCESS;
    }

    CrResult CrEngine::Shutdown()
    {
        CrResult result;

        if(mWindow)
        {
            result = crDestroyWindow(&mWindow);
            if(result != CR_SUCCESS)
            {
                return result;
            }
        }

        if(mPlatform)
        {
            result = crDestroyPlatform(&mPlatform);
            if(result != CR_SUCCESS)
            {
                return result;
            }
        }

        return CR_SUCCESS;
    }

    CrPlatform* CrEngine::GetPlatform() const { return mPlatform; }
    CrWindow*   CrEngine::GetWindow() const { return mWindow; }
    //CrRenderer* CrEngine::GetRenderer() const { return mRenderer; }
}