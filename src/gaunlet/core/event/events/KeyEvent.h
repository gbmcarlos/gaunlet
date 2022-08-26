#pragma once

#include "gaunlet/core/event/Event.h"

namespace gaunlet::Core {

    class KeyPressEvent : public Event {

    public:
        explicit KeyPressEvent(int key) : m_key(key) {}
        inline EventCategory getCategory() const override {return EventCategory::Keyboard;}

        EVENT_TYPE(KeyPress)

    private:
        int m_key;

    public:
        inline int getKey() const {return m_key;}
    };

    class KeyReleaseEvent : public Event {

    public:
        explicit KeyReleaseEvent(int key) : m_key(key) {}
        inline EventCategory getCategory() const override {return EventCategory::Keyboard;}
        EVENT_TYPE(KeyRelease)

    private:
        int m_key;

    public:
        inline int getKey() const {return m_key;}
    };

    class KeyRepeatEvent : public Event {

    public:
        explicit KeyRepeatEvent(int key) : m_key(key) {}
        inline EventCategory getCategory() const override {return EventCategory::Keyboard;}
        EVENT_TYPE(KeyRepeat)

    private:
        int m_key;

    public:
        inline int getKey() const {return m_key;}
    };

}