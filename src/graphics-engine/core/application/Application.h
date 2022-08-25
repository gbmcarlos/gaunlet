#pragma once

#include "graphics-engine/core/application/LayerStack.h"
#include "graphics-engine/core/application/TimeStep.h"
#include "graphics-engine/core/window/Window.h"
#include "graphics-engine/core/event/Event.h"

#include "graphics-engine/pch.h"

namespace engine::Core {

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