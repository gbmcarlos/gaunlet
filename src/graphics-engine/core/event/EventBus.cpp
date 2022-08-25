#include "graphics-engine/core/event/EventBus.h"

#include "graphics-engine/core/event/Event.h"

namespace engine::Core {

    void EventBus::publishEvent(Event& event) {
        if (eventCallback) {
            eventCallback(event);
        }
    }

    void EventBus::setEventCallback(const std::function<void(Event&)>& callback) {
        eventCallback = callback;
    }

}