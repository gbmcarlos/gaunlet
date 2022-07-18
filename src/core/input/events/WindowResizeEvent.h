#pragma once

#include "../Event.h"

namespace engine {

    class WindowResizeEvent : public Event {

    public:
        WindowResizeEvent(int width, int height) : m_width(width), m_height(height) {}
        EventCategory getCategory() const override {return EventCategory::Window;}
        EventType getType() const override {return EventType::WindowResize;}

    private:
        float m_width;
        float m_height;

    public:
        inline float getWidth() const {return m_width;};
        inline float getHeight() const {return m_height;};
    };

}
