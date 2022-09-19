#pragma once

#include "gaunlet/graphics/renderer/Batch.h"
#include "gaunlet/graphics/shader/Shader.h"

#include "gaunlet/core/render/RenderCommand.h"
#include "gaunlet/graphics/texture/TextureImage2D.h"
#include "gaunlet/graphics/renderer/DirectRenderer.h"

namespace gaunlet::Graphics {

    template<typename T>
    class BatchedRenderer {

    public:

        explicit BatchedRenderer(const BatchParameters& batchParameters);

        bool submitIndexedTriangles(std::vector<gaunlet::Graphics::Vertex>& vertices, std::vector<unsigned int>& indices, const Core::Ref<Graphics::Texture>& texture, T& propertySet);
        void flush(const Core::Ref<Graphics::Shader>& shader, RenderMode mode);

        inline unsigned int getMaxTextures() {return m_batch.getMaxTextures(); }
        inline const std::vector<T>& getPropertySets() {return m_batch.getPropertySets(); }

    private:

        Core::Ref<Graphics::Texture> m_whiteTexture = nullptr;
        Batch<T> m_batch;

        void loadWhiteTexture();

    };

    template<typename T>
    BatchedRenderer<T>::BatchedRenderer(const BatchParameters& batchParameters) : m_batch(batchParameters) {
        loadWhiteTexture();
    }

    template<typename T>
    bool BatchedRenderer<T>::submitIndexedTriangles(std::vector<gaunlet::Graphics::Vertex>& vertices, std::vector<unsigned int>& indices, const Core::Ref<Graphics::Texture>& texture, T& propertySet) {

        // Check if we need to flush before rendering the current model
        if (m_batch.shouldFlush(vertices, indices, texture)) {
            return false;
        }

        // Submit the vertices, indices, texture and entity properties to the batch, for storage till we flush
        m_batch.submit(
            vertices,
            indices,
            texture,
            propertySet
        );

        return true;

    }

    template<typename T>
    void BatchedRenderer<T>::flush(const Core::Ref<Graphics::Shader> &shader, RenderMode mode) {

        auto[vertices, indices, textures] = m_batch.get();

        if (!vertices.empty()) {

            // Bind all the textures
            for (unsigned int i = 0; i < textures.size(); i++) {
                textures[i]->activate(i);
            }

            DirectRenderer::renderIndexedVertices(
                vertices, indices, shader, mode
            );

        }

        m_batch.clear();
        m_batch.addTexture(m_whiteTexture);

    }

    template<typename T>
    void BatchedRenderer<T>::loadWhiteTexture() {

        // Create a 1x1 white texture, to be used as default
        unsigned int whiteTextureData = 0xffffffff;
        Core::Ref<Graphics::Texture> whiteTexture = Core::CreateRef<Graphics::TextureImage2D>(Core::TextureDataFormat::RGBA, Core::TextureDataFormat::RGBA, Core::PrimitiveDataType::UByte, 1, 1, &whiteTextureData);
        m_whiteTexture = whiteTexture;

        // Add it to the batch
        m_batch.addTexture(whiteTexture);

    }

}