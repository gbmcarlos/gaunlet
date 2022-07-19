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
        WindowClose, WindowResize,
        KeyPress, KeyRepeat, KeyRelease,
        MouseButtonPress, MouseButtonRelease,
        CursorMove,
        Scroll
    };

    class Event {

        friend class EventDispatcher;

    public:
        virtual EventCategory getCategory() const = 0;
        virtual EventType getType() const = 0;
        virtual std::string getName() = 0;

        inline bool isHandled() const {return m_handled; }

    private:
        bool m_handled = false;

    };

    class EventDispatcher {

        template<typename T>
        using callbackFunction = std::function<bool(T&)>;

    public:
        EventDispatcher(Event& event) : m_event(event) {}

    private:
        Event& m_event;

    public:

        template<typename T>
        void dispatch(callbackFunction<T> eventCallbackFunction) {

            if (m_event.getType() == T::getStaticType()) {
                m_event.m_handled = eventCallbackFunction(*(T*)& m_event);
            }

        }

    };
}