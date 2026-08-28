#pragma once

#include "Types.h"

namespace Crochet
{
    typedef struct CrPlatformCreateInfo
    {
        CrStructureType     sType           = CR_STRUCTURE_TYPE_NONE;

        CrWindowBackend     windowBackend   = CR_WINDOW_BACKEND_NONE;

    } CrPlatformCreateInfo;

    class CrPlatform
    {
        public:

            virtual ~CrPlatform() = default;

            virtual CrResult Initialize(const CrPlatformCreateInfo* pCreateInfo) = 0;

            virtual CrResult Shutdown() = 0;
    };

    CrResult crCreatePlatform(const CrPlatformCreateInfo* pCreateInfo, CrPlatform** ppPlatform);
    
    CrResult crDestroyPlatform(CrPlatform** ppPlatform);
}