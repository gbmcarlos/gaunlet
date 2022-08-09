#pragma once

#include "../../core/render/RenderCommand.h"
#include <string>

namespace engine {

    class Texture {

    public:
        virtual void bind(unsigned int slot) = 0;
        inline unsigned int getRendererId() {return m_rendererId; }
        inline unsigned int getWidth() {return m_width; }
        inline unsigned int getHeight() {return m_height; }

    protected:
        unsigned int m_rendererId = 0;
        unsigned int m_width = 0;
        unsigned int m_height = 0;

    };

}