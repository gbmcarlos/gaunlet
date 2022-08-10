#pragma once

#include "TimeStep.h"
#include "../input/Event.h"

namespace engine {

    class Layer {

    public:
        ~Layer() = default;
        virtual void onUpdate(TimeStep timeStep);
        virtual void onGuiRender();
        virtual void onEvent(Event& event);

    };

}