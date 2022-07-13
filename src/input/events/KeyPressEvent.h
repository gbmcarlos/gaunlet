#pragma once

#include "../Event.h"

namespace engine {

    class KeyPressEvent : public Event {

    public:
        KeyPressEvent(int key) : key(key) {}
        EventCategory getCategory() override {return EventCategory::Keyboard;}
        EventType getType() override {return EventType::KeyPress;}

    private:
        int key{};

    public:
        inline int getKey() {return key;}
    };

}