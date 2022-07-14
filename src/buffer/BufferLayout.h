#pragma once

#include <string>
#include <exception>
#include <vector>

#include <GL/glew.h>

namespace engine {

    enum class LayoutElementType {
        Bool,
        Int,
        Float
    };

    struct BufferLayoutElement {

        std::string name;
        unsigned int count;
        LayoutElementType type;
        unsigned int size;
        unsigned int offset;
        bool normalized;

        static GLenum layoutElementTypeToGLType(LayoutElementType type) {

            switch (type) {
                case LayoutElementType::Bool:   return GL_BOOL;
                case LayoutElementType::Int:   return GL_INT;
                case LayoutElementType::Float:   return GL_FLOAT;
            }

            throw std::runtime_error("Unknown layout element type");

        }

        BufferLayoutElement(std::string name, unsigned int count, LayoutElementType type, bool normalized = false);

    };

    class BufferLayout {

    private:
        std::vector<BufferLayoutElement> elements;
        unsigned int stride;
    public:
        BufferLayout(const std::initializer_list<BufferLayoutElement>& elements);
        void calculateStrideAndOffsets();
        inline std::vector<BufferLayoutElement> getElements() {return elements;}
        inline unsigned int getStride() {return stride;}

    };

}