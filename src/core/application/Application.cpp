#include "core/application/Application.h"

#include "core/event/EventBus.h"
#include "core/utils.h"

namespace engine::Core {

    Application::Application(const std::string& name)
        : m_window(std::make_unique<Window>(name)), m_running(true) {
        EventBus::getInstance().setEventCallback(GE_BIND_CALLBACK_FN(Application::onEvent));
    }

    void Application::onReady() {}

    void Application::onEvent(Event &event) {

        if (event.getType() == EventType::WindowClose) {
            m_running = false;
        }

        for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {

            (*--it)->onEvent(event);
            if (event.isHandled()) {
                break;
            }

        }

    }

    void Application::onUpdate(TimeStep timeStep) {

        GE_PROFILE_FUNCTION;

        for (auto layer : m_layerStack) {
            layer->onUpdate(timeStep);
        }

    }

    void Application::onGuiRender() {

        GE_PROFILE_FUNCTION;

        for (auto layer : m_layerStack) {
            layer->onGuiRender();
        }

    }

    void Application::pushLayer(Layer* layer) {
        m_layerStack.pushLayer(layer);
    }

    void Application::pushOverlay(Layer* overlay) {
        m_layerStack.pushOverlay(overlay);
    }

    void Application::popLayer(Layer* layer) {
        m_layerStack.popLayer(layer);
    }

    void Application::popOverlay(Layer* overlay) {
        m_layerStack.popOverlay(overlay);
    }

}