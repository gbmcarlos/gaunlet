#pragma once

#include "gaunlet/core/application/Application.h"

#include "gaunlet/pch.h"

namespace gaunlet::Core {

    class RunLoop {

    public:
        RunLoop(const Core::Ref<Core::Window>& window);
        void run(Application& application);

    private:
        Core::Ref<Core::Window> m_window;

    };

}