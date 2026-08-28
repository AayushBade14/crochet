#pragma once

#include "Types.h"

namespace Crochet
{
    // This structure is required while creation of a window
    typedef struct CrWindowCreateInfo
    {
        CrStructureType sType       = CR_STRUCTURE_TYPE_NONE;

        CrWindowBackend backend     = CR_WINDOW_BACKEND_NONE;
        CrWindowClientAPI     clientAPI   = CR_CLIENT_API_NONE;

        const CrChar*   title;
        CrUint32        width;
        CrUint32        height;

    } CrWindowCreateInfo;

    // This structure is the state for the window object
    typedef struct CrWindowState
    {
        CrStructureType sType = CR_STRUCTURE_TYPE_NONE;

        CrUint32        width;
        CrUint32        height;

        CrUint32        framebufferWidth;
        CrUint32        framebufferHeight;

        CrBool          shouldClose;

    } CrWindowState;

    // This is an interface for the CrWindow
    class CrWindow
    {
        public:

        virtual ~CrWindow() = default;

        virtual CrResult Initialize(const CrWindowCreateInfo* pCreateInfo) = 0;

        virtual CrResult Shutdown() = 0;
        
        virtual void PollEvents() = 0;
        virtual void SwapBuffers() = 0;

        virtual CrBool ShouldClose() const = 0;

        virtual CrUint32 GetWidth() const = 0;
        virtual CrUint32 GetHeight() const = 0;

        virtual CrUint32 GetFramebufferWidth() const = 0;
        virtual CrUint32 GetFramebufferHeight() const = 0;

        virtual CrWindowState GetWindowState() const = 0;

        virtual void* GetNativeHandle() const = 0;
    };

    // method for creating the CrWindow
    // Implicitly calls the CrWindowFactory
    CrResult crCreateWindow(const CrWindowCreateInfo* createInfo, CrWindow** ppWindow);

    // method for destroying the CrWindow
    CrResult crDestroyWindow(CrWindow** ppWindow);
}