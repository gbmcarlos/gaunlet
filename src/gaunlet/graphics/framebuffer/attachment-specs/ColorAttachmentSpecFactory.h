#pragma once

#include "gaunlet/graphics/framebuffer/ColorAttachmentSpec.h"
#include "gaunlet/graphics/framebuffer/attachment-specs/BaseColorAttachmentSpec.h"

namespace gaunlet::Graphics {

    class ColorAttachmentSpecFactory {

    public:

        template<typename T>
        static Core::Ref<ColorAttachmentSpec> create(ColorAttachmentSpec::Channels channels, ColorAttachmentSpec::Type type, ColorAttachmentSpec::Size size, T clearColor) {

            auto [internalFormat, externalFormat, dataType] = resolve(channels, type, size);

            auto attachmentSpec = Core::CreateRef<BaseColorAttachmentSpec>(
                internalFormat,
                externalFormat,
                dataType,
                channels,
                clearColor
            );

            return attachmentSpec;

        }

    protected:

        static std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat, Core::PrimitiveDataType> resolve(ColorAttachmentSpec::Channels channels, ColorAttachmentSpec::Type type, ColorAttachmentSpec::Size size) {

            switch (type) {
                case ColorAttachmentSpec::Type::TYPE_UNI: return resolveUNI(channels, size);
                case ColorAttachmentSpec::Type::TYPE_SNI: return resolveSNI(channels, size);
                case ColorAttachmentSpec::Type::TYPE_SI: return resolveSI(channels, size);
                case ColorAttachmentSpec::Type::TYPE_UI: return resolveUI(channels, size);
                case ColorAttachmentSpec::Type::TYPE_F: return resolveF(channels, size);
            }

        }

        static std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat, Core::PrimitiveDataType> resolveUNI(ColorAttachmentSpec::Channels channels, ColorAttachmentSpec::Size size) {

            Core::TextureExternalFormat externalFormat;
            auto dataType = Core::PrimitiveDataType::Float;

            if (channels == ColorAttachmentSpec::Channels::CHANNELS_1) {
                externalFormat = Core::TextureExternalFormat::Red;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::Red_8_UNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::Red_16_UNI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_2) {
                externalFormat = Core::TextureExternalFormat::RG;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RG_8_UNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RG_16_UNI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_3) {
                externalFormat = Core::TextureExternalFormat::RGB;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_4:
                        return {Core::TextureInternalFormat::RGB_4_UNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_5:
                        return {Core::TextureInternalFormat::RGB_5_UNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RGB_8_UNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_10:
                        return {Core::TextureInternalFormat::RGB_10_UNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_12:
                        return {Core::TextureInternalFormat::RGB_12_UNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RGB_16_UNI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_4) {
                externalFormat = Core::TextureExternalFormat::RGBA;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_2:
                        return {Core::TextureInternalFormat::RGBA_2_UNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_4:
                        return {Core::TextureInternalFormat::RGBA_4_UNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RGBA_8_UNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_12:
                        return {Core::TextureInternalFormat::RGBA_12_UNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RGBA_16_UNI, externalFormat, dataType};
                    default: break;
                }
            }

            throw std::runtime_error("Invalid UNI attachment spec parameters");

        }

        static std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat, Core::PrimitiveDataType> resolveSNI(ColorAttachmentSpec::Channels channels, ColorAttachmentSpec::Size size) {

            Core::TextureExternalFormat externalFormat;
            auto dataType = Core::PrimitiveDataType::Float;

            if (channels == ColorAttachmentSpec::Channels::CHANNELS_1) {
                externalFormat = Core::TextureExternalFormat::Red;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::Red_8_SNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::Red_16_SNI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_2) {
                externalFormat = Core::TextureExternalFormat::RG;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RG_8_SNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RG_16_SNI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_3) {
                externalFormat = Core::TextureExternalFormat::RGB;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RGB_8_SNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RGB_16_SNI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_4) {
                externalFormat = Core::TextureExternalFormat::RGBA;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RGBA_8_SNI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RGBA_16_SNI, externalFormat, dataType};
                    default: break;
                }
            }

            throw std::runtime_error("Invalid SNI attachment spec parameters");

        }

        static std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat, Core::PrimitiveDataType> resolveUI(ColorAttachmentSpec::Channels channels, ColorAttachmentSpec::Size size) {

            Core::TextureExternalFormat externalFormat;
            auto dataType = Core::PrimitiveDataType::Int;

            if (channels == ColorAttachmentSpec::Channels::CHANNELS_1) {
                externalFormat = Core::TextureExternalFormat::RedInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::Red_8_UI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::Red_16_UI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::Red_32_UI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_2) {
                externalFormat = Core::TextureExternalFormat::RGInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RG_8_UI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RG_16_UI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::RG_32_UI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_3) {
                externalFormat = Core::TextureExternalFormat::RGBInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RGB_8_UI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RGB_16_UI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::RGB_32_UI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_4) {
                externalFormat = Core::TextureExternalFormat::RGBAInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RGBA_8_UI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RGBA_16_UI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::RGBA_32_UI, externalFormat, dataType};
                    default: break;
                }
            }

            throw std::runtime_error("Invalid UI attachment spec parameters");

        }

        static std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat, Core::PrimitiveDataType> resolveSI(ColorAttachmentSpec::Channels channels, ColorAttachmentSpec::Size size) {

            Core::TextureExternalFormat externalFormat;
            auto dataType = Core::PrimitiveDataType::Int;

            if (channels == ColorAttachmentSpec::Channels::CHANNELS_1) {
                externalFormat = Core::TextureExternalFormat::RedInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::Red_8_SI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::Red_16_SI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::Red_32_SI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_2) {
                externalFormat = Core::TextureExternalFormat::RGInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RG_8_SI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RG_16_SI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::RG_32_SI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_3) {
                externalFormat = Core::TextureExternalFormat::RGBInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RGB_8_SI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RGB_16_SI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::RGB_32_SI, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_4) {
                externalFormat = Core::TextureExternalFormat::RGBAInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_8:
                        return {Core::TextureInternalFormat::RGBA_8_SI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RGBA_16_SI, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::RGBA_32_SI, externalFormat, dataType};
                    default: break;
                }
            }

            throw std::runtime_error("Invalid SI attachment spec parameters");

        }

        static std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat, Core::PrimitiveDataType> resolveF(ColorAttachmentSpec::Channels channels, ColorAttachmentSpec::Size size) {

            Core::TextureExternalFormat externalFormat;
            auto dataType = Core::PrimitiveDataType::Float;

            if (channels == ColorAttachmentSpec::Channels::CHANNELS_1) {
                externalFormat = Core::TextureExternalFormat::RedInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::Red_16_F, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::Red_32_F, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_2) {
                externalFormat = Core::TextureExternalFormat::RGInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RG_16_F, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::RG_32_F, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_3) {
                externalFormat = Core::TextureExternalFormat::RGBInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RGB_16_F, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::RGB_32_F, externalFormat, dataType};
                    default: break;
                }
            } else if (channels == ColorAttachmentSpec::Channels::CHANNELS_4) {
                externalFormat = Core::TextureExternalFormat::RGBAInteger;
                switch (size) {
                    case ColorAttachmentSpec::Size::SIZE_16:
                        return {Core::TextureInternalFormat::RGBA_16_F, externalFormat, dataType};
                    case ColorAttachmentSpec::Size::SIZE_32:
                        return {Core::TextureInternalFormat::RGBA_32_F, externalFormat, dataType};
                    default: break;
                }
            }

            throw std::runtime_error("Invalid F attachment spec parameters");

        }

    };

}