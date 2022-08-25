#pragma once

#include "graphics-engine/core/application/Application.h"

#include "graphics-engine/pch.h"

namespace engine::Core {

    class RunLoop {

    public:
        RunLoop(Application& application);
        void run();

    private:
        Application& m_application;

    };

}