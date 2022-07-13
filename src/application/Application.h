#pragma once

#include "../input/Event.h"

namespace engine {

    class Application {
    public:
        virtual void onReady() = 0;
        virtual void onUpdate() = 0;
        virtual void onRender() = 0;
        virtual void onGuiRender() = 0;
        virtual void onEvent(engine::Event& event) = 0;
    };

}