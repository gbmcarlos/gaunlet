#pragma once

#include <string>

#define EVENT_TYPE(type) static EventType getStaticType() {return EventType::type; }\
                         EventType getType() const override {return getStaticType(); }\
                         std::string getName() override {return #type; }

namespace engine {

    enum class EventCategory {
        Window, Keyboard, Mouse, Cursor, Scroll
    };

    enum class EventType {
        WindowResize,
        KeyPress, KeyRepeat, KeyRelease,
        MouseButtonPress, MouseButtonRelease,
        CursorMove,
        Scroll
    };

    class Event {

    public:
        virtual EventCategory getCategory() const = 0;
        virtual EventType getType() const = 0;
        virtual std::string getName() = 0;
    };
}