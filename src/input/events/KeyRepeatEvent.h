#pragma once

#include <GLFW/glfw3.h>

#include "../Event.h"

namespace engine {

    class KeyRepeatEvent : public Event {

    public:
        KeyRepeatEvent(int key) : key(key) {}
        EventCategory getCategory() override {return EventCategory::Keyboard;}
        EventType getType() override {return EventType::KeyRepeat;}

    private:
        int key;

    public:
        inline int getKey() {return key;}
    };

}
