#pragma once

#include "../pch.h"

namespace engine::Graphics {

    class UniformBuffer {

    private:
        unsigned int m_rendererId;
        bool m_dynamic;
        std::string m_name;
        unsigned int m_bindingPoint;
    public:
        UniformBuffer(std::string name, unsigned int bindingPoint, const void* data, unsigned int size);
        UniformBuffer(std::string  name, unsigned int bindingPoint, unsigned int size);
        ~UniformBuffer();
        void setData(const void* data, unsigned int size);

        inline const std::string& getName() {return m_name; }
        inline unsigned int getBindingPoint() {return m_bindingPoint; }

    };

}