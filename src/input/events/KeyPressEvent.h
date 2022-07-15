#pragma once

#include "../Event.h"

namespace engine {

    class KeyPressEvent : public Event {

    public:
        KeyPressEvent(int key) : m_key(key) {}
        EventCategory getCategory() override {return EventCategory::Keyboard;}
        EventType getType() override {return EventType::KeyPress;}

    private:
        int m_key;

    public:
        inline int getKey() {return m_key;}
    };

}