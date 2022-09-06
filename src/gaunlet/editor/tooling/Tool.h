#pragma once

#include "gaunlet/core/application/TimeStep.h"
#include "gaunlet/core/event/Event.h"

namespace gaunlet::Editor {

    class Workspace;

    class Tool {

        friend class Workspace;

    public:

        virtual const char* getName() = 0;
        virtual void start() {}
        virtual void stop() {}
        virtual bool onEvent(Core::Event& event) {return true; }
        virtual void onUpdate(Core::TimeStep timeStep) {}
        virtual void onGuiRender() {}

        inline const char* getId() {return m_id; }

    protected:
        Workspace* getWorkspace() {return m_workspace; }

    private:
        Workspace* m_workspace = nullptr;
        const char* m_id = nullptr;

    };

}