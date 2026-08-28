#pragma once

#include <Crochet/Platform/Platform.h>

namespace Crochet
{
    class CrGLFWPlatform : public CrPlatform
    {
        public:

            CrResult Initialize(const CrPlatformCreateInfo* pCreateInfo) override;

            CrResult Shutdown() override;
    };
}