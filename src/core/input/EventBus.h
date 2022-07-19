#pragma once

#include "Event.h"

#include <functional>

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

        void publishEvent(Event& event);
        void setEventCallback(const std::function<void(Event&)>& callback);

    };

}