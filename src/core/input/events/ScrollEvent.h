#pragma once

#include "../Event.h"

namespace engine::Core {

    class ScrollEvent : public Event {

    public:
        ScrollEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}
        EventCategory getCategory() const override {return EventCategory::Scroll;}
        EVENT_TYPE(Scroll)
        inline float getXOffset() const {return m_xOffset;}
        inline float getYOffset() const {return m_yOffset;}

    private:
        float m_xOffset;
        float m_yOffset;

    };

}