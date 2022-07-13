#pragma once

#include "../Event.h"

namespace engine {

    class KeyReleaseEvent : public Event {

    public:
        KeyReleaseEvent(int key) : key(key) {}
        EventCategory getCategory() override {return EventCategory::Keyboard;}
        EventType getType() override {return EventType::KeyRelease;}

    private:
        int key;

    public:
        inline int getKey() {return key;}
    };
}
