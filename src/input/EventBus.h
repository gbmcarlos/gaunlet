#pragma once

#include <functional>
#include "events/KeyPressEvent.h"
#include "events/KeyRepeatEvent.h"
#include "events/KeyReleaseEvent.h"

#include <GLFW/glfw3.h>

namespace engine {

    class EventBus {

    // Make it a singleton by 1) making the default constructor private, 2) deleting the copy-constructors (keep them public, to make the deletion known)
    private:
        EventBus() {}
    public:
        EventBus(EventBus const&) = delete;
        void operator=(EventBus const&)  = delete;

    private:
        std::function<void(Event&)> eventCallback;

    public:
        static EventBus& getInstance(){
            static EventBus m_instance;
            return m_instance;
        }

        void dispatchRawEvent(Event& event);
        void setKeyboardEventCallback(std::function<void(Event&)> callback);

    };

}