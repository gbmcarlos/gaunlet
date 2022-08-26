#pragma once

#include "gaunlet/core/application/LayerStack.h"
#include "gaunlet/core/application/TimeStep.h"
#include "gaunlet/core/window/Window.h"
#include "gaunlet/core/event/Event.h"

#include "gaunlet/pch.h"

namespace gaunlet::Core {

    class Application {
    public:
        explicit Application(const std::string& name);

        virtual void onReady();
        virtual void onEvent(Event& event);
        virtual void onUpdate(TimeStep timeStep);
        virtual void onGuiRender();

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);
        void popLayer(Layer* layer);
        void popOverlay(Layer* overlay);

        inline bool isRunning() const {return m_running; }
        inline const Ref<Window>& getWindow() {return m_window; }

    protected:
        Ref<Window> m_window;
        bool m_running;

    private:
        LayerStack m_layerStack;

    };

}