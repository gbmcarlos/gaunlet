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

        std::string m_name;
        unsigned int m_count;
        LayoutElementType m_type;
        unsigned int m_size;
        unsigned int m_offset;
        bool m_normalized;

        static GLenum layoutElementTypeToGLType(LayoutElementType type) {

            switch (type) {
                case LayoutElementType::Bool:       return GL_BOOL;
                case LayoutElementType::Int:        return GL_INT;
                case LayoutElementType::Float:      return GL_FLOAT;
            }

            throw std::runtime_error("Unknown m_layout element type");

        }

        BufferLayoutElement(std::string name, unsigned int count, LayoutElementType type, bool normalized = false);

    };

    class BufferLayout {

    private:
        std::vector<BufferLayoutElement> m_elements;
        unsigned int m_stride;
    public:
        BufferLayout(const std::initializer_list<BufferLayoutElement>& elements);
        void calculateStrideAndOffsets();
        inline std::vector<BufferLayoutElement>& getElements() {return m_elements;}
        inline unsigned int getStride() {return m_stride;}

    };

}