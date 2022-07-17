#pragma once

#include "../run-loop/TimeStep.h"
#include "../input/Event.h"

namespace engine {

    class Application {
    public:
        virtual void onReady() = 0;
        virtual void onUpdate(TimeStep timeStep) = 0;
        virtual void onGuiRender() = 0;
        virtual void onEvent(const engine::Event& event) = 0;
    };

}