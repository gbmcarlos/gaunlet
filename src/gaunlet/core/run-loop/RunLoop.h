#pragma once

#include "gaunlet/core/application/Application.h"

#include "gaunlet/pch.h"

namespace engine::Core {

    class RunLoop {

    public:
        RunLoop(Application& application);
        void run();

    private:
        Application& m_application;

    };

}