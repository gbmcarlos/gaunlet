#pragma once

namespace engine {

    class LayoutLayer;

    class DockedNode {

        friend LayoutLayer;

    public:

        inline bool isHovered() const {return m_isHovered; }
        inline unsigned int getMousePositionX() const {return m_mousePositionX; }
        inline unsigned int getMousePositionY() const {return m_mousePositionY; }
        inline unsigned int getNodeWidth() const {return m_nodeWidth; }
        inline unsigned int getNodeHeight() const {return m_nodeHeight; }
        inline float getNodeAspectRatio() const {return ((float) m_nodeWidth) / ((float) m_nodeHeight); }

    private:
        unsigned int m_nodeWidth = 0, m_nodeHeight = 0;
        bool m_isHovered = false;
        unsigned int m_mousePositionX = 0, m_mousePositionY = 0;

    };

    class GuiDockedNode : public DockedNode {

    public:
        virtual void onGuiRender() = 0;

    };

    class RenderDockedNode : public DockedNode  {

    public:
        virtual void onEvent() = 0;

    };

}