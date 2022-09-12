#pragma once

#include "gaunlet/graphics/Vertex.h"
#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/graphics/texture/TextureLibrary.h"

namespace gaunlet::Graphics {

    struct BatchParameters {

        BatchParameters()
            : m_maxVertices(100000), m_maxIndices(m_maxVertices * 6), m_maxTextures(10), m_maxPropertySets(100) {}

        BatchParameters(unsigned int maxVertices, unsigned int maxIndices, unsigned int maxTextures, unsigned int maxPropertySets)
            : m_maxVertices(maxVertices), m_maxIndices(maxIndices), m_maxTextures(maxTextures), m_maxPropertySets(maxPropertySets) {}

        unsigned int m_maxVertices;
        unsigned int m_maxIndices;
        unsigned int m_maxTextures;
        unsigned int m_maxPropertySets;

    };

    template<typename T>
    class Batch {

    public:

        Batch() = default;
        explicit Batch(const BatchParameters& parameters);

        void submit(std::vector<Graphics::Vertex>& vertices, std::vector<unsigned int>& indices, const Core::Ref<Graphics::Texture>& texture, T& propertySet);
        bool shouldFlush(const std::vector<Graphics::Vertex>& vertices, const std::vector<unsigned int>& indices, const Core::Ref<Graphics::Texture>& texture);

        std::tuple<
            const std::vector<Graphics::Vertex>&,
            const std::vector<unsigned int>&,
            const std::vector<Core::Ref<Graphics::Texture>>&
        > get() {
            return {m_vertices, m_indices, m_textureLibrary.getAll()};
        }

        inline void addTexture(const Core::Ref<Graphics::Texture>& texture) {m_textureLibrary.add(texture); }
        inline unsigned int getMaxTextures() {return m_parameters.m_maxTextures; }
        inline const std::vector<T>& getPropertySets() {return m_propertySets; }

        void clear() {
            m_vertices.clear();
            m_indices.clear();
            m_textureLibrary.clear();
            m_propertySets.clear();
        }

    protected:

        BatchParameters m_parameters;

        std::vector<Graphics::Vertex> m_vertices = {};
        std::vector<unsigned int> m_indices = {};
        Graphics::TextureLibrary m_textureLibrary;
        std::vector<T> m_propertySets = {};

    };

    template<typename T>
    Batch<T>::Batch(const BatchParameters& parameters) : m_parameters(parameters) {}

    template<typename T>
    void Batch<T>::submit(std::vector<Graphics::Vertex> &vertices, std::vector<unsigned int> &indices, const Core::Ref<Graphics::Texture>& texture, T& propertySet) {

        if (texture != nullptr) {
            // Once added the texture, we know the actual index for this entity
            propertySet.m_textureIndex = (int) m_textureLibrary.addOrGet(texture);
        }

        // Add the entity index to each vertex
        for (auto& vertex : vertices) {
            vertex.m_entityIndex = (unsigned int) m_propertySets.size();
        }

        // Store the entity properties
        m_propertySets.push_back(propertySet);

        // Transform the indices, offset them by the current list of vertices in the batch
        for (auto& index : indices) {
            index += m_vertices.size(); // Yes, offset the indices by the vertices. NOT the indices by the indices. I did that once and I was debugging for hours
        }

        // Store the vertices and indices in the batch
        m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
        m_indices.insert(m_indices.end(), indices.begin(), indices.end());

    }

    template<typename T>
    bool Batch<T>::shouldFlush(const std::vector<Graphics::Vertex> &vertices, const std::vector<unsigned int> &indices, const Core::Ref<Graphics::Texture> &texture) {

        // If rendering the current polygon would pass over the limit of vertices, flush
        if (m_vertices.size() + vertices.size() > m_parameters.m_maxVertices) {
            return true;
        }

        // If rendering the current polygon would pass over the limit of indices, flush
        if (m_indices.size() + indices.size() > m_parameters.m_maxIndices) {
            return true;
        }

        // If a texture is being submitted, and it doesn't exist in the library yet, check if it would be over the limit
        if (texture != nullptr && !m_textureLibrary.exists(texture)) {

            // If adding this texture would be over the limit, flush
            if (m_textureLibrary.count() + 1 > m_parameters.m_maxTextures) {
                return true;
            }

        }

        // If rendering the current polygon would pass over the limit of entity properties, flush
        if (m_propertySets.size() + 1 > m_parameters.m_maxPropertySets) {
            return true;
        }

        return false;

    }

}