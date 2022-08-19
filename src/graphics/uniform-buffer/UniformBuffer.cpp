#include "UniformBuffer.h"

#include <utility>

#include "../../core/render/RenderCommand.h"

namespace engine {

    UniformBuffer::UniformBuffer(std::string name, unsigned int bindingPoint, const void *data, unsigned int size)
        : m_dynamic(false), m_name(std::move(name)), m_bindingPoint(bindingPoint) {
        RenderCommand::createUniformBuffer(m_rendererId, data, size);
        RenderCommand::bindUniformBufferToBindingPoint(m_rendererId, bindingPoint, size);
        RenderCommand::unbindUniformBuffer();
    }

    UniformBuffer::UniformBuffer(std::string  name, unsigned int bindingPoint, unsigned int size)
        : m_dynamic(true), m_name(std::move(name)), m_bindingPoint(bindingPoint) {
        RenderCommand::createUniformBuffer(m_rendererId, size);
        RenderCommand::bindUniformBufferToBindingPoint(m_rendererId, bindingPoint, size);
        RenderCommand::unbindUniformBuffer();
    }

    UniformBuffer::~UniformBuffer() {
        RenderCommand::deleteBuffer(m_rendererId);
    }

    void UniformBuffer::setData(const void *data, unsigned int size) {

        if (!m_dynamic) {
            throw std::runtime_error("Can't set data for non-dynamic uniform buffer");
        }

        bind();
        RenderCommand::submitUniformBufferData(data, size);
    }

    void UniformBuffer::bind() {
        RenderCommand::bindUniformBuffer(m_rendererId);
    }
    void UniformBuffer::unbind() {
        RenderCommand::unbindUniformBuffer();
    }

}