#include "EventBus.h"

#include "Event.h"

#include <utility>
#include <iostream>

namespace engine {

    void EventBus::dispatchRawEvent(Event& event) {
        if (eventCallback) {
            eventCallback(event);
        }
    }

    void EventBus::setKeyboardEventCallback(std::function<void(Event&)> callback) {
        eventCallback = std::move(callback);
    }

}