#pragma once

#include "../Event.h"

namespace engine {

    class KeyReleaseEvent : public Event {

    public:
        KeyReleaseEvent(int key) : m_key(key) {}
        EventCategory getCategory() const override {return EventCategory::Keyboard;}
        EventType getType() const override {return EventType::KeyRelease;}

    private:
        int m_key;

    public:
        inline int getKey() const {return m_key;}
    };
}
