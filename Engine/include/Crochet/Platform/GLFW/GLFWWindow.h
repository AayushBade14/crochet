#pragma once

#include <GLFW/glfw3.h>

#include <Crochet/Core/Window.h>

namespace Crochet
{
    class CrGLFWWindow : public CrWindow
    {
        public:

            CrGLFWWindow() = default;

            ~CrGLFWWindow() override = default;
            
            CrResult Initialize(const CrWindowCreateInfo* pCreateInfo) override;

            CrResult Shutdown() override;

            void PollEvents() override;
            void SwapBuffers() override;

            CrUint32 GetWidth() const override;
            CrUint32 GetHeight() const override;
            
            CrUint32 GetFramebufferWidth() const override;
            CrUint32 GetFramebufferHeight() const override;

            CrWindowState GetWindowState() const override;

            void* GetNativeHandle() const override;

        private:

            GLFWwindow* mWindow = nullptr;
            CrWindowState mState = {};
    };
}