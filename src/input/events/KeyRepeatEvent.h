#pragma once

#include <GLFW/glfw3.h>

#include "../Event.h"

namespace engine {

    class KeyRepeatEvent : public Event {

    public:
        KeyRepeatEvent(int key) : m_key(key) {}
        EventCategory getCategory() const override {return EventCategory::Keyboard;}
        EventType getType() const override {return EventType::KeyRepeat;}

    private:
        int m_key;

    public:
        inline int getKey() const {return m_key;}
    };

}
