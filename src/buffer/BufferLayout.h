#pragma once

#include <string>
#include <exception>
#include <vector>

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

        BufferLayoutElement(const std::string& name, unsigned int count, LayoutElementType type, bool normalized = false);

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