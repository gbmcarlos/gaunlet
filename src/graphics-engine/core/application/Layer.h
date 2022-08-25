#pragma once

#include "graphics-engine/core/application/TimeStep.h"
#include "graphics-engine/core/event/Event.h"

namespace engine::Core {

    class Layer {

    public:
        ~Layer() = default;
        virtual void onUpdate(TimeStep timeStep);
        virtual void onGuiRender();
        virtual void onEvent(Event& event);

    };

}