#include "gaunlet/scene/deferred-renderer/DeferredRenderer.h"

#include "gaunlet/core/render/RenderCommand.h"
#include "gaunlet/graphics/texture/TextureImage2D.h"
#include "gaunlet/graphics//forward-renderer/ForwardRenderer.h"

namespace gaunlet::Scene {

    DeferredRenderer::RendererStorage* DeferredRenderer::m_rendererStorage = new RendererStorage;

    void DeferredRenderer::init() {
        loadShaders();
        loadDefaultWhiteTexture();
    }

    void DeferredRenderer::beginScene(RenderMode renderMode, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const Core::Ref<Graphics::Framebuffer>& framebuffer, const DirectionalLightComponent& directionalLight) {

        m_rendererStorage->m_renderMode = renderMode;

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

        // Clear the screen or the framebuffer
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

        // Forget about the framebuffer
        m_rendererStorage->m_framebuffer = nullptr;

    }

    void DeferredRenderer::submit(int entityId, const ModelComponent& polygonComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent) {

        std::vector<Graphics::Vertex> vertices = {};
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

        Core::Ref<Graphics::Shader> shader;

        if (m_rendererStorage->m_renderMode == RenderMode::Faces) {
            shader = m_rendererStorage->m_shaderLibrary.get("polygon-faces-shader");
        } else {
            shader = m_rendererStorage->m_shaderLibrary.get("polygon-wireframe-shader");
        }

        // Render
        Graphics::ForwardRenderer::renderMesh(
            vertices, indices, textures, shader,
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
        Graphics::ForwardRenderer::renderMesh(
            vertices, indices, textures, m_rendererStorage->m_shaderLibrary.get("circle-shader"), m_rendererStorage->m_framebuffer
        );

        // Clear the batch (and add the white texture back)
        m_rendererStorage->m_circleBatch.clear();
        m_rendererStorage->m_circleBatch.addTexture(m_rendererStorage->m_whiteTexture);

    }

    // Create the shaders and set the texture slots
    void DeferredRenderer::loadShaders() {

        GE_PROFILE_FUNCTION;

        // Create the Uniform Buffer for the Scene Properties, which will be linked to every shader
        m_rendererStorage->m_scenePropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "ScenePropertiesBlock",
            0,
            sizeof (SceneProperties)
        );

        loadPolygonShaders();
        loadCircleShaders();

    }

    void DeferredRenderer::loadPolygonShaders() {

        // Create a uniform buffer that will contain the properties of every polygon model, and will be linked to every polygon shader
        m_rendererStorage->m_polygonEntityPropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "EntityPropertiesBlock",
            1,
            sizeof (PolygonEntityProperties) * 100
        );

        // Polygon Faces Shader
        std::map<Core::ShaderType, std::string> polygonFacesShaderSources {
            {Core::ShaderType::Vertex, ASSETS_PATH"/shaders/polygon-faces/vertex.glsl"},
            {Core::ShaderType::Fragment, ASSETS_PATH"/shaders/polygon-faces/fragment.glsl"}
        };
        auto polygonFacesShader = m_rendererStorage->m_shaderLibrary.load("polygon-faces-shader", polygonFacesShaderSources);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_rendererStorage->m_polygonBatch.getMaxTextures(); i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            polygonFacesShader->setUniform1i(textureName, i);
        }

        // Polygon Wireframe Shader
        std::map<Core::ShaderType, std::string> polygonWireframeShaderSources {
            {Core::ShaderType::Vertex, ASSETS_PATH"/shaders/polygon-wireframe/vertex.glsl"},
            {Core::ShaderType::Geometry, ASSETS_PATH"/shaders/polygon-wireframe/geometry.glsl"},
            {Core::ShaderType::Fragment, ASSETS_PATH"/shaders/polygon-wireframe/fragment.glsl"}
        };
        auto polygonWireframeShader = m_rendererStorage->m_shaderLibrary.load("polygon-wireframe-shader", polygonWireframeShaderSources);

        // Link the SceneProperties and EntityProperties uniform buffers to both shader
        polygonFacesShader->linkUniformBuffer(m_rendererStorage->m_polygonEntityPropertiesUniformBuffer);
        polygonFacesShader->linkUniformBuffer(m_rendererStorage->m_scenePropertiesUniformBuffer);
        polygonWireframeShader->linkUniformBuffer(m_rendererStorage->m_polygonEntityPropertiesUniformBuffer);
        polygonWireframeShader->linkUniformBuffer(m_rendererStorage->m_scenePropertiesUniformBuffer);

    }

    void DeferredRenderer::loadCircleShaders() {

        // Create a uniform buffer that will contain the properties of every circle model, and will be linked to every circle shader
        m_rendererStorage->m_circleEntityPropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "EntityPropertiesBlock",
            2,
            sizeof (CircleEntityProperties) * 100
        );

        std::map<Core::ShaderType, std::string> circleShaderSource {
            {Core::ShaderType::Vertex, ASSETS_PATH"/shaders/circle-faces/vertex.glsl"},
            {Core::ShaderType::Fragment, ASSETS_PATH"/shaders/circle-faces/fragment.glsl"}
        };
        auto circleShader = m_rendererStorage->m_shaderLibrary.load("circle-shader", circleShaderSource);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_rendererStorage->m_circleBatch.getMaxTextures(); i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            circleShader->setUniform1i(textureName, i);
        }

        // Link the EntityProperties uniform buffers
        circleShader->linkUniformBuffer(m_rendererStorage->m_circleEntityPropertiesUniformBuffer);
        circleShader->linkUniformBuffer(m_rendererStorage->m_scenePropertiesUniformBuffer);

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