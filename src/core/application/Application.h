#pragma once

#include "LayerStack.h"
#include "TimeStep.h"
#include "../window/Window.h"
#include "../input/Event.h"

#include <string>

namespace engine {

    class Application {
    public:
        explicit Application(const std::string& name);

        virtual void onReady();
        virtual void onEvent(Event& event);
        virtual void onUpdate(TimeStep timeStep);
        virtual void onGuiRender();

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);

        inline bool isRunning() const {return m_running; }
        inline const std::unique_ptr<Window>& getWindow() {return m_window; }

    protected:
        std::unique_ptr<Window> m_window;
        bool m_running;

    private:
        LayerStack m_layerStack;

    };

}