#pragma once

#include "gaunlet/core/event/Event.h"

namespace engine::Core {

    class MouseButtonPress : public Event {

    public:
        MouseButtonPress(int button) : m_button(button) {}
        EventCategory getCategory() const override {return EventCategory::Mouse;}
        EVENT_TYPE(MouseButtonPress)
        inline int getButton() const {return m_button;}

    private:
        int m_button;

    };

    class MouseButtonRelease : public Event {

    public:
        MouseButtonRelease(int button) : m_button(button) {}
        EventCategory getCategory() const override {return EventCategory::Mouse;}
        EVENT_TYPE(MouseButtonRelease)
        inline int getButton() const {return m_button;}

    private:
        int m_button;

    };

}