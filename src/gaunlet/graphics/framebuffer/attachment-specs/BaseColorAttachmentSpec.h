#pragma once

#include "gaunlet/graphics/framebuffer/ColorAttachmentSpec.h"

namespace gaunlet::Graphics {

    class BaseColorAttachmentSpec : public ColorAttachmentSpec {

    public:

        BaseColorAttachmentSpec(Core::TextureInternalFormat internalFormat, Core::TextureExternalFormat externalFormat, Core::PrimitiveDataType dataType, Channels channels, int clearValue)
            : m_internalFormat(internalFormat), m_externalFormat(externalFormat), m_dataType(dataType), m_channels(channels), m_clearValueInt(clearValue) {}

        BaseColorAttachmentSpec(Core::TextureInternalFormat internalFormat, Core::TextureExternalFormat externalFormat, Core::PrimitiveDataType dataType, Channels channels, float clearValue)
            : m_internalFormat(internalFormat), m_externalFormat(externalFormat), m_dataType(dataType), m_channels(channels), m_clearValueFloat(clearValue) {}

        BaseColorAttachmentSpec(Core::TextureInternalFormat internalFormat, Core::TextureExternalFormat externalFormat, Core::PrimitiveDataType dataType, Channels channels, glm::vec2 clearValue)
            : m_internalFormat(internalFormat), m_externalFormat(externalFormat), m_dataType(dataType), m_channels(channels), m_clearValueVec2(clearValue) {}

        BaseColorAttachmentSpec(Core::TextureInternalFormat internalFormat, Core::TextureExternalFormat externalFormat, Core::PrimitiveDataType dataType, Channels channels, glm::vec3 clearValue)
            : m_internalFormat(internalFormat), m_externalFormat(externalFormat), m_dataType(dataType), m_channels(channels), m_clearValueVec3(clearValue) {}


        BaseColorAttachmentSpec(Core::TextureInternalFormat internalFormat, Core::TextureExternalFormat externalFormat, Core::PrimitiveDataType dataType, Channels channels, glm::vec4 clearValue)
            : m_internalFormat(internalFormat), m_externalFormat(externalFormat), m_dataType(dataType), m_channels(channels), m_clearValueVec4(clearValue) {}

        Core::PrimitiveDataType getClearDataType() override {
            return m_dataType;
        }

        void* getClearValue() override {

            switch (m_channels) {
                case Channels::CHANNELS_1:
                    switch (m_dataType) {
                        case Core::PrimitiveDataType::Int:   return &m_clearValueInt;
                        case Core::PrimitiveDataType::Float: return &m_clearValueFloat;
                        default: break;
                    }
                case Channels::CHANNELS_2: return &m_clearValueVec2;
                case Channels::CHANNELS_3: return &m_clearValueVec3;
                case Channels::CHANNELS_4: return &m_clearValueVec4;
            }

            return nullptr;

        }

        std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat, Core::PrimitiveDataType> getTextureParameters() override {
            return {m_internalFormat, m_externalFormat, m_dataType};
        }

        std::tuple<Core::TextureExternalFormat, Core::PrimitiveDataType> getReadIntParameters() override {
            if (m_channels != Channels::CHANNELS_1) {
                throw std::runtime_error("Color attachment doesn't support reading INT data");
            }
            return getReadParameters();
        };

        std::tuple<Core::TextureExternalFormat, Core::PrimitiveDataType> getReadFloatParameters() override {
            if (m_channels != Channels::CHANNELS_1) {
                throw std::runtime_error("Color attachment doesn't support reading FLOAT data");
            }
            return getReadParameters();
        };

        std::tuple<Core::TextureExternalFormat, Core::PrimitiveDataType> getReadVec2Parameters() override {
            if (m_channels != Channels::CHANNELS_2) {
                throw std::runtime_error("Color attachment doesn't support reading VEC2 data");
            }
            return getReadParameters();
        };

        std::tuple<Core::TextureExternalFormat, Core::PrimitiveDataType> getReadVec3Parameters() override {
            if (m_channels != Channels::CHANNELS_3) {
                throw std::runtime_error("Color attachment doesn't support reading VEC3 data");
            }
            return getReadParameters();
        };

        std::tuple<Core::TextureExternalFormat, Core::PrimitiveDataType> getReadVec4Parameters() override {
            if (m_channels != Channels::CHANNELS_4) {
                throw std::runtime_error("Color attachment doesn't support reading VEC4 data");
            }
            return getReadParameters();
        };

    protected:

        std::tuple<Core::TextureExternalFormat, Core::PrimitiveDataType> getReadParameters() {
            return {m_externalFormat, m_dataType};
        }

    private:

        Core::TextureInternalFormat m_internalFormat;
        Core::TextureExternalFormat m_externalFormat;
        Core::PrimitiveDataType m_dataType;
        Channels m_channels;
        int m_clearValueInt;
        float m_clearValueFloat;
        glm::vec2 m_clearValueVec2;
        glm::vec3 m_clearValueVec3;
        glm::vec4 m_clearValueVec4;

    };

}