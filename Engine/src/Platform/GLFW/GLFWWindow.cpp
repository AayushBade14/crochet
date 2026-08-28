#include <Crochet/Platform/GLFW/GLFWWindow.h>

namespace Crochet
{
    CrResult CrGLFWWindow::Initialize(const CrWindowCreateInfo* pCreateInfo)
    {
        if(!pCreateInfo)
        {
            return CR_ERROR_INVALID_ARGUMENT;
        }

        glfwDefaultWindowHints(); // sets the window hints back to default
                                  // useful when creating multiple windows

        switch(pCreateInfo->clientAPI)
        {
            case CR_WINDOW_CLIENT_API_OPENGL:
            {
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

                break;
            }

            case CR_WINDOW_CLIENT_API_NONE:
            {
                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            
                break;
            }

            default:
            {
                return CR_ERROR_UNSUPPORTED;
            }
        }

        mWindow = glfwCreateWindow(pCreateInfo->width, 
                                        pCreateInfo->height, 
                                        pCreateInfo->title, 
                                        nullptr, 
                                        nullptr);
        if(!mWindow)
        {
            return CR_ERROR_WINDOW_CREATION_FAILED;
        }

        CrInt width;
        CrInt height;
        glfwGetFramebufferSize(mWindow, &width, &height);

        mState.sType                        = CR_STRUCTURE_TYPE_WINDOW_STATE;
        mState.width                        = pCreateInfo->width;
        mState.height                       = pCreateInfo->height;
        mState.shouldClose                  = false;
        mState.framebufferWidth             = static_cast<CrUint32>(width);
        mState.framebufferHeight            = static_cast<CrUint32>(height);

        return CR_SUCCESS;
    }

    CrResult CrGLFWWindow::Shutdown()
    {
        if(mWindow)
        {
            glfwDestroyWindow(mWindow);
            
            mWindow = nullptr;
        }

        return CR_SUCCESS;
    }

    void CrGLFWWindow::PollEvents()
    {
        glfwPollEvents();
    }

    void CrGLFWWindow::SwapBuffers()
    {
        glfwSwapBuffers(mWindow);
    }

    CrUint32 CrGLFWWindow::GetWidth() const { return mState.width; }
    CrUint32 CrGLFWWindow::GetHeight() const { return mState.height; }

    CrUint32 CrGLFWWindow::GetFramebufferWidth() const { return mState.framebufferWidth; }
    CrUint32 CrGLFWWindow::GetFramebufferHeight() const { return mState.framebufferHeight; }

    CrWindowState CrGLFWWindow::GetWindowState() const { return mState; }

    void* CrGLFWWindow::GetNativeHandle() const { return reinterpret_cast<void*>(mWindow); }
}