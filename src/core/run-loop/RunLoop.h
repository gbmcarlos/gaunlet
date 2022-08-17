#pragma once

#include "../application/Application.h"

#include "../pch.h"

namespace engine {

    class RunLoop {

    public:
        RunLoop(Application& application);
        void run();

    private:
        Application& m_application;

    };

}