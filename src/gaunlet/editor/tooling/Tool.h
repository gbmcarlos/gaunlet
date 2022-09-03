#pragma once

#include "gaunlet/core/application/TimeStep.h"
#include "gaunlet/core/event/Event.h"

namespace gaunlet::Editor {

    class Workspace;

    class Tool {

        friend class Workspace;

        virtual bool onEvent(Core::Event& event) {return true; }
        virtual void onUpdate(Core::TimeStep timeStep) {}

    protected:
        Workspace* getWorkspace() {return m_workspace; }

    private:
        Workspace* m_workspace;

    };

}