#pragma once

#include <vector>

#include <GL/glew.h>

namespace engine {

    struct VertexBufferLayoutElement {
        unsigned int type;
        unsigned int count;
        bool normalized;

        static unsigned int getSizeOfType(unsigned int type) {

            switch (type) {
                case GL_FLOAT: return 4;
            }

            return 0;

        }

    };

    class VertexBufferLayout {
    private:
    public:
        VertexBufferLayout();
        ~VertexBufferLayout();
        std::vector<VertexBufferLayoutElement> elements;
        unsigned int stride;
        void pushFloat(unsigned int count);
    };

}