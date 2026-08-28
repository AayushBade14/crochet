#pragma once

#include <cstdint>

namespace Crochet
{
    //-----------------------------------------------
    // ENGINE-TYPES
    //-----------------------------------------------

    typedef uint32_t    CrUint32;
    typedef uint64_t    CrUint64;

    typedef int         CrInt;

    typedef int32_t     CrInt32;
    typedef int64_t     CrInt64;

    typedef bool        CrBool;

    typedef char        CrChar;

    //-----------------------------------------------
    // STRUCTURE-TYPE-ENUMS
    //-----------------------------------------------

    typedef enum CrStructureType
    {
        CR_STRUCTURE_TYPE_NONE                          = 0,

        //--------------------------
        // Engine
        //--------------------------
        CR_STRUCTURE_TYPE_ENGINE_CREATE_INFO,

        //--------------------------
        // Platform
        //--------------------------
        CR_STRUCTURE_TYPE_PLATFORM_CREATE_INFO,

        //--------------------------
        // Window
        //--------------------------
        CR_STRUCTURE_TYPE_WINDOW_CREATE_INFO,
        CR_STRUCTURE_TYPE_WINDOW_STATE,

        //--------------------------
        // Events
        //--------------------------
        CR_STRUCTURE_TYPE_EVENT_WINDOW_CLOSE,
        CR_STRUCTURE_TYPE_EVENT_WINDOW_RESIZE,


    } CrStructureType;

    //-----------------------------------------------
    // WINDOW-SPECIFIC-ENUMS
    //-----------------------------------------------

    typedef enum CrWindowBackend
    {
        CR_WINDOW_BACKEND_NONE                      = 0,

        CR_WINDOW_BACKEND_GLFW,
        CR_WINDOW_BACKEND_WIN32,
        CR_WINDOW_BACKEND_XLIB

    } CrWindowBackend;

    // Client API will be used in the CrWindowCreateInfo struct for the windowing system to
    // create relevant window context for the client API
    
    typedef enum CrWindowClientAPI
    {
        CR_WINDOW_CLIENT_API_NONE                   = 0,

        CR_WINDOW_CLIENT_API_OPENGL,

    } CrClientAPI;

    //------------------------------------------------
    // RENDERER-BACKEND-ENUMS
    //------------------------------------------------

    typedef enum CrRendererAPI
    {
        CR_RENDERER_API_NONE                        = 0,

        CR_RENDERER_API_OPENGL,
        CR_RENDERER_API_VULKAN,
        CR_RENDERER_API_DIRECT3D11,
        CR_RENDERER_API_DIRECT3D12

    } CrRendererAPI;

    //------------------------------------------------
    // RESULT-SPECIFIC-ENUMS
    //------------------------------------------------

    typedef enum CrResult
    {
        CR_SUCCESS                                          = 0,

        // general errors
        CR_ERROR_UNKNOWN                                    = -1,
        CR_ERROR_INVALID_ARGUMENT                           = -2,
        CR_ERROR_INITIALIZATION_FAILED                      = -3,
        CR_ERROR_DESTRUCTION_FAILED                         = -4,
        CR_ERROR_UNSUPPORTED                                = -5,
        
        // window specific errors
        CR_ERROR_WINDOW_CREATION_FAILED                     = -6,

    } CrResult;
}