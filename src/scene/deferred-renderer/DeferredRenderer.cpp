#include "scene/deferred-renderer/DeferredRenderer.h"

#include "core/render/RenderCommand.h"
#include "graphics/texture/TextureImage2D.h"
#include "scene/forward-renderer/ForwardRenderer.h"

namespace engine::Scene {

    DeferredRenderer::RendererStorage* DeferredRenderer::m_rendererStorage = new RendererStorage;

    void DeferredRenderer::init() {
        loadDefaultShaders();
        loadDefaultWhiteTexture();
    }

    void DeferredRenderer::beginScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const Core::Ref<Graphics::Framebuffer>& framebuffer, const DirectionalLightComponent& directionalLight) {

        SceneProperties sceneProperties(
            viewMatrix, projectionMatrix,
            directionalLight.m_color, directionalLight.m_position,
            directionalLight.m_ambientIntensity, directionalLight.m_diffuseIntensity
        );

        std::vector<glm::mat4> sceneData;
        sceneData.emplace_back(viewMatrix);
        sceneData.emplace_back(projectionMatrix);

        m_rendererStorage->m_scenePropertiesUniformBuffer->setData(
            (const void*) &sceneProperties,
            sizeof(SceneProperties)
        );

        if (framebuffer != nullptr) {
            framebuffer->clear();
        } else {
            Core::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        }

        m_rendererStorage->m_framebuffer = framebuffer;
    }

    void DeferredRenderer::endScene() {

        // Flush the polygons, if any
        if (!m_rendererStorage->m_polygonBatch.getVertices().empty()) {
            flushPolygons();
        }

        // Flush the circles, if any
        if (!m_rendererStorage->m_circleBatch.getVertices().empty()) {
            flushCircles();
        }

    }

    void DeferredRenderer::submit(int entityId, const ModelComponent& polygonComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent) {

        std::vector<Vertex> vertices = {};
        std::vector<unsigned int> indices = {};

        // Get the vertices and indices of all the meshes of the model
        for (auto& mesh : polygonComponent.m_model.getMeshes()) {
            vertices.insert(vertices.end(), mesh.getVertices().begin(), mesh.getVertices().end());
            indices.insert(indices.end(), mesh.getIndices().begin(), mesh.getIndices().end());
        }

        // Check if we need to flush before rendering the current model
        if (m_rendererStorage->m_polygonBatch.shouldFlush(vertices, indices, materialComponent.m_texture)) {
            flushPolygons();
        }

        PolygonEntityProperties entityProperties(
            entityId,
            transformComponent.getTransformationMatrix(),
            materialComponent.m_color
        );

        // Submit the vertices, indices, texture and entity properties to the batch, for storage till we flush
        m_rendererStorage->m_polygonBatch.submit(
            vertices,
            indices,
            materialComponent.m_texture,
            entityProperties
        );

    }

    void DeferredRenderer::submit(int entityId, const CircleComponent& circleComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent) {

        // Get the vertices and indices to be added
        auto vertices = circleComponent.m_mesh.getVertices();
        auto indices = circleComponent.m_mesh.getIndices();

        // Check if we need to flush before rendering the current model
        if (m_rendererStorage->m_circleBatch.shouldFlush(vertices, indices, materialComponent.m_texture)) {
            flushCircles();
        }

        CircleEntityProperties entityProperties(
            entityId,
            transformComponent.getTransformationMatrix(),
            materialComponent.m_color,
            circleComponent.m_thickness,
            circleComponent.m_fade
        );

        // Submit the vertices, indices, texture and entity properties to the batch, for storage till we flush
        m_rendererStorage->m_circleBatch.submit(
            vertices,
            indices,
            materialComponent.m_texture,
            entityProperties
        );

    }

    // Gather the polygon data back from the batch, submit it for drawing, and clear the batch
    void DeferredRenderer::flushPolygons() {

        auto& vertices = m_rendererStorage->m_polygonBatch.getVertices();
        auto& indices = m_rendererStorage->m_polygonBatch.getIndices();
        auto& textures = m_rendererStorage->m_polygonBatch.getTextures();
        auto& entityProperties = m_rendererStorage->m_polygonBatch.getEntityProperties();

        // Submit the entity properties to the uniform buffer
        m_rendererStorage->m_polygonEntityPropertiesUniformBuffer->setData(
            (const void*) entityProperties.data(),
            sizeof(PolygonEntityProperties) * entityProperties.size()
        );

        // Render
        ForwardRenderer::renderMesh(
            vertices, indices, textures, m_rendererStorage->m_shaderLibrary.get("polygon-shader"),
            m_rendererStorage->m_framebuffer
        );

        // Clear the batch (and add the white texture back)
        m_rendererStorage->m_polygonBatch.clear();
        m_rendererStorage->m_polygonBatch.addTexture(m_rendererStorage->m_whiteTexture);

    }

    // Gather the circle data back from the batch, submit it for drawing, and clear the batch
    void DeferredRenderer::flushCircles() {

        auto& vertices = m_rendererStorage->m_circleBatch.getVertices();
        auto& indices = m_rendererStorage->m_circleBatch.getIndices();
        auto& textures = m_rendererStorage->m_circleBatch.getTextures();
        auto& entityProperties = m_rendererStorage->m_circleBatch.getEntityProperties();

        // Submit the entity properties to the uniform buffer
        m_rendererStorage->m_circleEntityPropertiesUniformBuffer->setData(
            (const void*) entityProperties.data(),
            sizeof(CircleEntityProperties) * entityProperties.size()
        );

        // Render
        ForwardRenderer::renderMesh(
            vertices, indices, textures, m_rendererStorage->m_shaderLibrary.get("circle-shader"), m_rendererStorage->m_framebuffer
        );

        // Clear the batch (and add the white texture back)
        m_rendererStorage->m_circleBatch.clear();
        m_rendererStorage->m_circleBatch.addTexture(m_rendererStorage->m_whiteTexture);

    }

    // Create the shaders and set the texture slots
    void DeferredRenderer::loadDefaultShaders() {

        GE_PROFILE_FUNCTION;

        m_rendererStorage->m_scenePropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "ScenePropertiesBlock",
            0,
            sizeof (SceneProperties)
        );

        // Create the polygon shader
        std::map<Core::ShaderType, std::string> polygonShaderSource {
            {Core::ShaderType::Vertex, ASSETS_PATH"/shaders/2d/polygon-vertex-position-2d.glsl"},
            {Core::ShaderType::Fragment, ASSETS_PATH"/shaders/2d/polygon-color-texture-2d.glsl"}
        };
        auto polygonShader = m_rendererStorage->m_shaderLibrary.load("polygon-shader", polygonShaderSource);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_rendererStorage->m_polygonBatch.getMaxTextures(); i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            polygonShader->setUniform1i(textureName, i);
        }

        m_rendererStorage->m_polygonEntityPropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "EntityPropertiesBlock",
            1,
            sizeof (PolygonEntityProperties) * 100
        );

        // Link the SceneMatrices and the EntityProperties uniform buffers
        polygonShader->linkUniformBuffer(m_rendererStorage->m_scenePropertiesUniformBuffer);
        polygonShader->linkUniformBuffer(m_rendererStorage->m_polygonEntityPropertiesUniformBuffer);

        // Create the circle shader
        std::map<Core::ShaderType, std::string> circleShaderSource {
            {Core::ShaderType::Vertex, ASSETS_PATH"/shaders/2d/circle-vertex-position-2d.glsl"},
            {Core::ShaderType::Fragment, ASSETS_PATH"/shaders/2d/circle-color-texture-2d.glsl"}
        };
        auto circleShader = m_rendererStorage->m_shaderLibrary.load("circle-shader", circleShaderSource);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_rendererStorage->m_circleBatch.getMaxTextures(); i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            circleShader->setUniform1i(textureName, i);
        }

        m_rendererStorage->m_circleEntityPropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "EntityPropertiesBlock",
            2,
            sizeof (CircleEntityProperties) * 100
        );

        // Link the SceneMatrices and the EntityProperties uniform buffers
        circleShader->linkUniformBuffer(m_rendererStorage->m_scenePropertiesUniformBuffer);
        circleShader->linkUniformBuffer(m_rendererStorage->m_circleEntityPropertiesUniformBuffer);

    }

    void DeferredRenderer::loadDefaultWhiteTexture() {

        GE_PROFILE_FUNCTION;

        // Create a 1x1 white texture, to be used as default
        unsigned int whiteTextureData = 0xffffffff;
        Core::Ref<Graphics::Texture> whiteTexture = Core::CreateRef<Graphics::TextureImage2D>(Core::TextureDataFormat::RGBA, Core::TextureDataFormat::RGBA, 1, 1, &whiteTextureData);
        m_rendererStorage->m_whiteTexture = whiteTexture;

        // Add it to the polygon and circle batches
        m_rendererStorage->m_polygonBatch.addTexture(whiteTexture);
        m_rendererStorage->m_circleBatch.addTexture(whiteTexture);

    }

}