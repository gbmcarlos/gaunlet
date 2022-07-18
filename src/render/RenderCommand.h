#pragma once

#include "opengl/OpenGLRenderAPI.h"
#include "../buffer/VertexArray.h"

#include <glm/glm.hpp>
#include <memory>

namespace engine {

    class RenderCommand {

    public:
        RenderCommand() = delete;
        RenderCommand(RenderCommand const&) = delete;
        void operator=(RenderCommand const&)  = delete;

    private:
        inline static OpenGLRenderApi& getApi() {return OpenGLRenderApi::getInstance();}

    public:
        static void init();
        static void clear(const glm::vec4& color);

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

        static unsigned int createShaderProgram();
        static unsigned int compileShader(unsigned int type, const std::string& source);
        static void attachShader(unsigned id, unsigned int shaderId);
        static void compileShaderProgram(unsigned int id);
        static void bindShader(unsigned int id);
        static void deleteShader(unsigned int id);

        static int getUniformLocation(unsigned int id, const std::string& name);
        static void setUniform3f(int location, const glm::vec3& value);
        static void setUniform4f(int location, const glm::vec4& value);
        static void setUniformMat3f(int location, const glm::mat3& value);
        static void setUniformMat4f(int location, const glm::mat4& value);

        static unsigned int sizeOfLayoutElementType(LayoutElementType type);
        static void addVertexArrayAttribute(unsigned int index, int count, LayoutElementType type, bool normalized, int stride, int offset);

        static void drawIndexedTriangles(const std::shared_ptr<VertexArray>& vertexArray);

    };

}