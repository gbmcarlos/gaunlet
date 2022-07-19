#pragma once

#include "../application/Application.h"

#include <string>
#include <memory>

namespace engine {

    class RunLoop {

    public:
        RunLoop(Application& application);
        void run();

    private:
        Application& m_application;

    };

}