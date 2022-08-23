#pragma once

#include "graphics/texture/TextureLibrary.h"
#include "scene/entity/GraphicsComponents.h"

namespace engine::Scene {

    struct PolygonEntityProperties {

        PolygonEntityProperties(int entityId, const glm::mat4& transform, glm::vec4 color)
            : m_entityId(entityId), m_transform(transform), m_color(color), m_textureIndex(0) {}

        // The order of these properties is optimized to minimize required padding when using this data in a uniform buffer
        glm::mat4 m_transform;
        glm::vec4 m_color;
        unsigned int m_textureIndex;
        int m_entityId; glm::vec2 pad2 = {};

    };

    struct CircleEntityProperties {

        CircleEntityProperties(int entityId, const glm::mat4& transform, glm::vec4 color, float thickness, float fade)
            : m_entityId(entityId), m_transform(transform), m_color(color), m_textureIndex(0), m_thickness(thickness), m_fade(fade) {}

        // The order of these properties is optimized to minimize required padding when using this data in a uniform buffer
        glm::mat4 m_transform;
        glm::vec4 m_color;
        unsigned int m_textureIndex;
        int m_entityId;
        float m_thickness;
        float m_fade;

    };

    template<typename T>
    class Batch {

    public:

        void submit(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const Core::Ref<Graphics::Texture>& texture, T& entityProperties) {

            if (texture != nullptr) {
                // Once added the texture, we know the actual index for this entity
                entityProperties.m_textureIndex = (int) m_textureLibrary.addOrGet(texture);
            }

            // Add the entity index to each vertex
            for (auto& vertex : vertices) {
                vertex.m_entityIndex = (unsigned int) m_entityProperties.size();
            }

            // Store the entity properties
            m_entityProperties.push_back(entityProperties);

            // Transform the indices, offset them by the current list of vertices in the batch
            for (auto& index : indices) {
                index += m_vertices.size(); // Yes, offset the indices by the vertices. NOT the indices by the indices. I did that once and I was debugging for hours
            }

            // Store the vertices and indices in the batch
            m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
            m_indices.insert(m_indices.end(), indices.begin(), indices.end());

        }

        bool shouldFlush(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Core::Ref<Graphics::Texture>& texture) {

            // If rendering the current polygon would pass over the limit of vertices, flush
            if (m_vertices.size() + vertices.size() > m_maxVertices) {
                return true;
            }

            // If rendering the current polygon would pass over the limit of indices, flush
            if (m_indices.size() + indices.size() > m_maxIndices) {
                return true;
            }

            // If a texture is being submitted, and it doesn't exist in the library yet, check if it would be over the limit
            if (texture != nullptr && !m_textureLibrary.exists(texture)) {

                // If adding this texture would be over the limit, flush
                if (m_textureLibrary.count() + 1 > m_maxTextures) {
                    return true;
                }

            }

            // If rendering the current polygon would pass over the limit of entity properties, flush
            if (m_entityProperties.size() + 1 > m_maxEntityProperties) {
                return true;
            }

            return false;

        }

        inline void addTexture(const Core::Ref<Graphics::Texture>& texture) {m_textureLibrary.add(texture); }
        inline const std::vector<Vertex>& getVertices() {return m_vertices; }
        inline const std::vector<unsigned int>& getIndices() {return m_indices; }
        inline const std::vector<Core::Ref<Graphics::Texture>>& getTextures() {return m_textureLibrary.getAll(); }
        inline unsigned int getMaxTextures() {return m_maxTextures; }
        inline const std::vector<T>& getEntityProperties() {return m_entityProperties; }

        inline void clear() {
            m_vertices.clear();
            m_indices.clear();
            m_textureLibrary.clear();
            m_entityProperties.clear();
        }

    protected:
        static const unsigned int m_maxVertices = 100000;
        std::vector<Vertex> m_vertices = {};

        static const unsigned int m_maxIndices = m_maxVertices * 6;
        std::vector<unsigned int> m_indices = {};

        static const unsigned int m_maxTextures = 10;
        Graphics::TextureLibrary m_textureLibrary;

        static const unsigned int m_maxEntityProperties = 100;
        std::vector<T> m_entityProperties = {};

    };

}