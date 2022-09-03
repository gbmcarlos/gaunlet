#pragma once

#include "gaunlet/core/application/TimeStep.h"
#include "gaunlet/core/window/Window.h"
#include "gaunlet/core/event/Event.h"

#include "gaunlet/pch.h"

namespace gaunlet::Core {

    class Application {
    public:
        explicit Application();

        virtual void onReady() {}
        virtual void onEvent(Event& event) {}
        virtual void onUpdate(TimeStep timeStep) {}
        virtual void onGuiRender() {}

        inline bool isRunning() const {return m_running; }

    protected:
        void handleEvent(Event& event);
        bool m_running = true;

    };

}