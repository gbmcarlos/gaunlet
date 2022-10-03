#pragma once

#include "gaunlet/pch.h"

namespace gaunlet::Core {

    class CoordinatesUtils {

    public:

        // Transform Normalized Device coordinates to UV coordinates
        static float nc2uv(float nc) {
            return (nc / 2.0f) + 0.5f;
        }

        // Transform UV coordinates to Normalized Device
        static float uv2nc(float uv) {
            return (uv - 0.5f) * 2.0f;
        }

        static glm::vec2 nc2uv(glm::vec2 nc) {
            return {
                nc2uv(nc.x),
                nc2uv(nc.y)
            };
        }

        static glm::vec2 uv2nc(glm::vec2 uv) {
            return {
                uv2nc(uv.x),
                uv2nc(uv.y)
            };
        }

        static std::vector<glm::vec2> quadCorners(glm::vec2 origin, float width, float height) {

            float left = origin.x - (width/2.0f);
            float right = origin.x + (width/2.0f);
            float bottom = origin.y - (height/2.0f);
            float top = origin.y + (height/2.0f);

            return {
                uv2nc({left, bottom}),
                uv2nc({right, bottom}),
                uv2nc({right, top}),
                uv2nc({left, top})
            };

        }

        static std::vector<glm::vec2> uvQuad2nc(glm::vec2 uvOrigin, float width, float height) {

            float uvLeft = uvOrigin.x - (width/2.0f);
            float uvRight = uvOrigin.x + (width/2.0f);
            float uvBottom = uvOrigin.y - (height/2.0f);
            float uvTop = uvOrigin.y + (height/2.0f);

            return {
                uv2nc({uvLeft, uvBottom}),
                uv2nc({uvRight, uvBottom}),
                uv2nc({uvRight, uvTop}),
                uv2nc({uvLeft, uvTop})
            };

        }

        static std::vector<glm::vec2> ncQuad2uv(glm::vec2 ncOrigin, float width, float height) {

            float ncLeft = ncOrigin.x - (width/2.0f);
            float ncRight = ncOrigin.x + (width/2.0f);
            float ncBottom = ncOrigin.y - (height/2.0f);
            float ncTop = ncOrigin.y + (height/2.0f);

            return {
                nc2uv({ncLeft, ncBottom}),
                nc2uv({ncRight, ncBottom}),
                nc2uv({ncRight, ncTop}),
                nc2uv({ncLeft, ncTop})
            };

        }

    };

}