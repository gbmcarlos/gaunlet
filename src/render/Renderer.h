#pragma once

#include "memory"

#include <glm/glm.hpp>

#include "opengl/OpenGLRenderAPI.h"

namespace engine {

    class Renderer {

    public:
        Renderer() = delete;
        Renderer(Renderer const&) = delete;
        void operator=(Renderer const&)  = delete;

    public:
        static void init();
        static void clear(glm::vec4 color);

        static void createVertexBuffer(unsigned int& id, unsigned int size);
        static void createVertexBuffer(unsigned int& id, const void *data, unsigned int size);
        static void bindVertexBuffer(unsigned int& id);
        static void submitVertexBufferData(const void *data, unsigned int size);
        static void unbindVertexBuffer();

        static void createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count);
        static void bindIndexBuffer(unsigned int&);
        static void unbindIndexBuffer();

        static void deleteBuffer(unsigned int&);

        static void createVertexArray(unsigned int& id);
        static void bindVertexArray(unsigned int& id);
        static void unbindVertexArray();
        static void deleteVertexArray(unsigned int& id);

        static void addVertexArrayAttribute(unsigned int index, int count, GLenum type, bool normalized, int stride, int offset);

    };

}