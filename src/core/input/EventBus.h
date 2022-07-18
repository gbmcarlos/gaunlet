#pragma once

#include <functional>

#include "events/KeyPressEvent.h"
#include "events/KeyRepeatEvent.h"
#include "events/KeyReleaseEvent.h"

#include "events/WindowResizeEvent.h"

namespace engine {

    class EventBus {

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
        void setKeyboardEventCallback(const std::function<void(Event&)>& callback);

    };

}