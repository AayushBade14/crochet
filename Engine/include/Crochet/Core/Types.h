#pragma once

namespace Crochet
{
    typedef enum CR_StructureType
    {
        CR_STRUCTURE_TYPE_NONE = 0,

        //--------------------------
        // Events
        //--------------------------
        CR_STRUCTURE_TYPE_EVENT_WINDOW_CLOSE,
        CR_STRUCTURE_TYPE_EVENT_WINDOW_RESIZE,

        //--------------------------
        // Input
        //--------------------------
        CR_STRUCTURE_TYPE_EVENT_INPUT_KEY_PRESSED,

        //--------------------------
        // Window
        //--------------------------
        CR_STRUCTURE_TYPE_WINDOW_CREATE_INFO
    } CR_StructureType;
}