#include "gaunlet/core/event/EventBus.h"

#include "gaunlet/core/event/Event.h"

namespace gaunlet::Core {

    void EventBus::publishEvent(Event& event) {
        if (m_eventCallback) {
            m_eventCallback(event);
        }
    }

    void EventBus::setEventCallback(const std::function<void(Event&)>& callback) {
        m_eventCallback = callback;
    }

}