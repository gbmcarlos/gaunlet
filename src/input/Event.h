#pragma once

namespace engine {

    enum class EventCategory {
        Window, Keyboard, Mouse, Scroll, Cursor
    };

    enum class EventType {
        WindowClose, WindowResize,
        KeyPress, KeyRepeat, KeyRelease,
        MouseRightClick, MouseLeftClick, MouseWheelClick,
        Scroll,
        CursorMove
    };

    class Event {

    public:
        virtual EventCategory getCategory() = 0;
        virtual EventType getType() = 0;

    };
}