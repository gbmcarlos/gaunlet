#pragma once

#include <functional>
#include "events/KeyPressEvent.h"
#include "events/KeyRepeatEvent.h"
#include "events/KeyReleaseEvent.h"

#include <GLFW/glfw3.h>

namespace engine {

    class EventBus {
    private:

        EventBus() {}
    public:
        static EventBus& getInstance(){
            static EventBus instance;
            return instance;
        }
        EventBus(EventBus const&) = delete;
        void operator=(EventBus const&)  = delete;
    private:
        std::function<void(Event&)> eventCallback;
    public:
        void dispatchRawEvent(Event& event);
        void setKeyboardEventCallback(std::function<void(Event&)> callback);

    };

}