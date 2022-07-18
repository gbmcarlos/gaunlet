#pragma once

#include "../Event.h"

namespace engine {

    class KeyPressEvent : public Event {

    public:
        KeyPressEvent(int key) : m_key(key) {}
        EventCategory getCategory() const override {return EventCategory::Keyboard;}

        EVENT_TYPE(KeyPress)

    private:
        int m_key;

    public:
        inline int getKey() const {return m_key;}
    };

    class KeyReleaseEvent : public Event {

    public:
        KeyReleaseEvent(int key) : m_key(key) {}
        EventCategory getCategory() const override {return EventCategory::Keyboard;}
        EVENT_TYPE(KeyRelease)

    private:
        int m_key;

    public:
        inline int getKey() const {return m_key;}
    };

    class KeyRepeatEvent : public Event {

    public:
        KeyRepeatEvent(int key) : m_key(key) {}
        EventCategory getCategory() const override {return EventCategory::Keyboard;}
        EVENT_TYPE(KeyRepeat)

    private:
        int m_key;

    public:
        inline int getKey() const {return m_key;}
    };

}