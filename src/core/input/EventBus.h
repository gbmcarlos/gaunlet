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

        void dispatchRawEvent(Event& event);
        void setKeyboardEventCallback(const std::function<void(Event&)>& callback);

    };

    class EventDispatcher {

        template<typename T>
        using callbackFunction = std::function<void(T&)>;

    public:
        EventDispatcher(const Event& event) : m_event(event) {}

    private:
        const Event& m_event;

    public:

        template<typename T>
        void dispatch(callbackFunction<T> eventCallbackFunction) {

            if (m_event.getType() == T::getStaticType()) {
                eventCallbackFunction(*(T*)& m_event);
            }

        }


    };

}