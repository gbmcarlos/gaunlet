#pragma once

#include "gaunlet/core/application/TimeStep.h"
#include "gaunlet/core/event/Event.h"

namespace gaunlet::Core {

    class Layer {

    public:
        ~Layer() = default;
        virtual void onUpdate(TimeStep timeStep);
        virtual void onGuiRender();
        virtual void onEvent(Event& event);

    };

}