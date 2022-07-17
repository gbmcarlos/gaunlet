#pragma once

#include "../Event.h"

namespace engine {

    class KeyPressEvent : public Event {

    public:
        KeyPressEvent(int key) : m_key(key) {}
        EventCategory getCategory() const override {return EventCategory::Keyboard;}
        EventType getType() const override {return EventType::KeyPress;}

    private:
        int m_key;

    public:
        inline int getKey() const {return m_key;}
    };

}