#pragma once

#include "Types.h"

namespace Crochet
{
    typedef struct CrEngineCreateInfo
    {
        CrStructureType     sType           = CR_STRUCTURE_TYPE_NONE;

        CrWindowBackend     windowBackend   = CR_WINDOW_BACKEND_NONE;
        CrWindowClientAPI   windowClientAPI = CR_WINDOW_CLIENT_API_NONE;
        CrRendererAPI       rendererAPI     = CR_RENDERER_API_NONE;

        const CrChar*       title;
        CrUint32            windowWidth;
        CrUint32            windowHeight;

    } CrEngineCreateInfo;

    // forward declaring some classes
    class CrPlatform;
    class CrWindow;
    //class CrRenderer; => TO BE IMPLEMENTED

    class CrEngine
    {
        public:

            CrEngine() = default;

            ~CrEngine() = default;

            CrResult Initialize(const CrEngineCreateInfo* pCreateInfo);

            CrResult Shutdown();

            CrPlatform* GetPlatform() const;
            CrWindow* GetWindow() const;
            //CrRenderer* GetRenderer() const;

            
        private:

            CrPlatform*     mPlatform   = nullptr;
            CrWindow*       mWindow     = nullptr;
            //CrRenderer*     mRenderer   = nullptr;

    };

    CrResult crCreateEngine(const CrEngineCreateInfo* pCreateInfo, CrEngine** ppEngine);

    CrResult crDestroyEngine(CrEngine** ppEngine);
}