#include "UniformBuffer.h"

#include <utility>

#include "../../core/render/RenderCommand.h"

namespace engine {

    UniformBuffer::UniformBuffer(std::string name, unsigned int bindingPoint, const void *data, unsigned int size)
        : m_dynamic(false), m_name(std::move(name)), m_bindingPoint(bindingPoint) {
        RenderCommand::createUniformBuffer(m_rendererId, data, size);
        RenderCommand::bindUniformBufferToBindingPoint(m_rendererId, bindingPoint, size);
    }

    UniformBuffer::UniformBuffer(std::string  name, unsigned int bindingPoint, unsigned int size)
        : m_dynamic(true), m_name(std::move(name)), m_bindingPoint(bindingPoint) {
        RenderCommand::createUniformBuffer(m_rendererId, size);
        RenderCommand::bindUniformBufferToBindingPoint(m_rendererId, bindingPoint, size);
    }

    UniformBuffer::~UniformBuffer() {
        RenderCommand::deleteUniformBuffer(m_rendererId);
    }

    void UniformBuffer::setData(const void *data, unsigned int size) {

        if (!m_dynamic) {
            throw std::runtime_error("Can't set data for non-dynamic uniform buffer");
        }

        RenderCommand::submitUniformBufferData(m_rendererId, data, size);
    }

}