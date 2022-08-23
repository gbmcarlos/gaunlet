#pragma once

#include "core/application/TimeStep.h"
#include "core/event/Event.h"

namespace engine::Core {

    class Layer {

    public:
        ~Layer() = default;
        virtual void onUpdate(TimeStep timeStep);
        virtual void onGuiRender();
        virtual void onEvent(Event& event);

    };

}