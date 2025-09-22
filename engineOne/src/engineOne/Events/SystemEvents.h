#pragma once
#include<engineOne/Core/Input/keyCodes.h>


enum class SystemEventType : int
{
    KeyPress,
    KeyRelease,
    WindowResize,
    MouseMove,
    MouseButton,
    MouseScroll,
    WindowClose,
    Count
};



struct KeyEventData
{
    KeyCode code;
    bool pressed;
};

struct WindowResizeEventData
{
    int width, height;
};

struct SystemEvent
{
    SystemEventType type;
    union EventData
    {
        struct
        {

        };
    };
};











