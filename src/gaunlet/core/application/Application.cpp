#include "gaunlet/core/application/Application.h"

#include "gaunlet/core/event/EventBus.h"
#include "gaunlet/core/utils.h"

namespace gaunlet::Core {

    Application::Application() {
        EventBus::getInstance().setEventCallback(GL_BIND_CALLBACK_FN(Application::handleEvent));
    }

    void Application::handleEvent(Event &event) {

        if (event.getType() == EventType::WindowClose) {
            m_running = false;
        }

        onEvent(event);

    }

}