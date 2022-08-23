#pragma once

#include "core/event/Event.h"

namespace engine::Core {

    class CursorMoveEvent : public Event {

    public:
        CursorMoveEvent(float xPosition, float yPosition) : m_xPosition(xPosition), m_yPosition(yPosition) {}
        EventCategory getCategory() const override {return EventCategory::Cursor;}
        EVENT_TYPE(CursorMove)
        inline float getXPosition() const {return m_xPosition;}
        inline float getYPosition() const {return m_yPosition;}

    private:
        float m_xPosition;
        float m_yPosition;

    };

}