#pragma once

#include "../Event.h"

namespace engine {

    class KeyReleaseEvent : public Event {

    public:
        KeyReleaseEvent(int key) : m_key(key) {}
        EventCategory getCategory() override {return EventCategory::Keyboard;}
        EventType getType() override {return EventType::KeyRelease;}

    private:
        int m_key;

    public:
        inline int getKey() {return m_key;}
    };
}
