#pragma once

#include "../../core/render/RenderCommand.h"
#include <string>

namespace engine {

    class Texture {

    public:
        virtual void bind(unsigned int slot) = 0;
        inline unsigned int getRendererId() {return m_rendererId; }

    protected:
        unsigned int m_rendererId = 0;

    };

}