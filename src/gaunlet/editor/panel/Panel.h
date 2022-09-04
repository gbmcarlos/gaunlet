#pragma once

#include "gaunlet/core/event/Event.h"

namespace gaunlet::Editor {

    class Workspace;

    class Panel {

        friend Workspace;

    public:

        inline const char* getId() {return m_id; }
        inline bool isHovered() const {return m_isHovered; }
        inline unsigned int getMousePositionX() const {return m_mousePositionX; }
        inline unsigned int getMousePositionY() const {return m_mousePositionY; }
        inline unsigned int getMousePositionYInverted() const {return m_mousePositionYInverted; }
        inline unsigned int getWidth() const {return m_width; }
        inline unsigned int getHeight() const {return m_height; }
        inline float getNodeAspectRatio() const {return ((float) m_width) / ((float) m_height); }

    protected:
        inline Workspace* getWorkspace() const {return m_workspace; }

    private:
        const char* m_id;
        Workspace* m_workspace = nullptr;
        unsigned int m_width = 0, m_height = 0;
        bool m_isHovered = false;
        unsigned int m_mousePositionX = 0, m_mousePositionY = 0, m_mousePositionYInverted = 0;

    };

    class GuiPanel : public Panel {

    public:
        virtual void onGuiRender() = 0;

    };

}