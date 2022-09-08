#include "gaunlet/scene/deferred-renderer/DeferredRenderer.h"

#include "gaunlet/core/render/RenderCommand.h"
#include "gaunlet/graphics/texture/TextureImage2D.h"

namespace gaunlet::Scene {

    DeferredRenderer::RendererStorage* DeferredRenderer::m_rendererStorage = new RendererStorage;

    void DeferredRenderer::init() {
        loadShaders();
        loadDefaultWhiteTexture();
    }

    void DeferredRenderer::beginScene(RenderMode renderMode, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const Core::Ref<DirectionalLightComponent>& directionalLight, const Core::Ref<Graphics::TextureCubeMap> skyboxCubeMap) {

        m_rendererStorage->m_renderMode = renderMode;

        SceneProperties sceneProperties(
            viewMatrix, projectionMatrix,
            directionalLight->m_color, directionalLight->m_direction,
            directionalLight->m_ambientIntensity, directionalLight->m_diffuseIntensity
        );

        std::vector<glm::mat4> sceneData;
        sceneData.emplace_back(viewMatrix);
        sceneData.emplace_back(projectionMatrix);

        m_rendererStorage->m_scenePropertiesUniformBuffer->setData(
            (const void*) &sceneProperties,
            sizeof(SceneProperties)
        );

        m_rendererStorage->m_skyboxCubeMap = skyboxCubeMap;

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

        if (m_rendererStorage->m_skyboxCubeMap != nullptr) {
            flushSkybox();
        }

    }

    void DeferredRenderer::submit(int entityId, const ModelComponent& polygonComponent, const glm::mat4& transformationMatrix, const MaterialComponent& materialComponent) {

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
            transformationMatrix,
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

    void DeferredRenderer::submit(int entityId, const CircleComponent& circleComponent, const glm::mat4& transformationMatrix, const MaterialComponent& materialComponent) {

        // Get the vertices and indices to be added
        auto vertices = circleComponent.m_mesh.getVertices();
        auto indices = circleComponent.m_mesh.getIndices();

        // Check if we need to flush before rendering the current model
        if (m_rendererStorage->m_circleBatch.shouldFlush(vertices, indices, materialComponent.m_texture)) {
            flushCircles();
        }

        CircleEntityProperties entityProperties(
            entityId,
            transformationMatrix,
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

        Core::RenderCommand::setDepthFunction(Core::DepthFunction::Less);

        // Render
        render(
            vertices, indices, textures, shader
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

        Core::RenderCommand::setDepthFunction(Core::DepthFunction::Less);

        // Render
        render(
            vertices, indices, textures, m_rendererStorage->m_shaderLibrary.get("circle-shader")
        );

        // Clear the batch (and add the white texture back)
        m_rendererStorage->m_circleBatch.clear();
        m_rendererStorage->m_circleBatch.addTexture(m_rendererStorage->m_whiteTexture);

    }

    void DeferredRenderer::flushSkybox() {

        CubeModel cube;

        std::vector<Core::Ref<Graphics::Texture>> textures = {
            m_rendererStorage->m_skyboxCubeMap
        };

        Core::RenderCommand::setDepthFunction(Core::DepthFunction::LessOrEqual);

        // Render
        render(
            cube.getMeshes()[0].getVertices(), cube.getMeshes()[0].getIndices(), textures, m_rendererStorage->m_shaderLibrary.get("skybox-shader")
        );

    }

    void DeferredRenderer::render(const std::vector<gaunlet::Graphics::Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Core::Ref<Graphics::Texture>>& textures, const Core::Ref<Graphics::Shader>& shader) {

        // Create a layout, based on the structure of PolygonVertex
        static gaunlet::Graphics::BufferLayout vertexLayout = gaunlet::Graphics::Vertex::getBufferLayout();

        // Create the vertex and index buffers
        gaunlet::Graphics::VertexBuffer vertexBuffer((const void*) vertices.data(), sizeof(gaunlet::Graphics::Vertex) * vertices.size());
        gaunlet::Graphics::IndexBuffer indexBuffer((unsigned int*) indices.data(), indices.size());

        // Bind them together into a vertex array
        gaunlet::Graphics::VertexArray vertexArray;
        vertexArray.addBuffer(vertexLayout);

        // Bind all the textures
        for (unsigned int i = 0; i < textures.size(); i++) {
            textures[i]->activate(i);
        }

        // Render the polygons as triangles
        Core::RenderCommand::drawIndexedTriangles(
            vertexBuffer.getRendererId(),
            indexBuffer.getRendererId(),
            vertexArray.getRendererId(),
            shader->getRendererId(),
            indexBuffer.getCount()
        );

    }

    // Create the shaders and set the texture slots
    void DeferredRenderer::loadShaders() {

        GL_PROFILE_FUNCTION;

        // Create the Uniform Buffer for the Scene Properties, which will be linked to every shader
        m_rendererStorage->m_scenePropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "ScenePropertiesBlock",
            0,
            sizeof (SceneProperties)
        );

        loadPolygonShaders();
        loadCircleShaders();
        loadSkyboxShader();

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

    void DeferredRenderer::loadSkyboxShader() {

        std::map<Core::ShaderType, std::string> skyboxShaderSource {
            {Core::ShaderType::Vertex, ASSETS_PATH"/shaders/skybox/vertex.glsl"},
            {Core::ShaderType::Fragment, ASSETS_PATH"/shaders/skybox/fragment.glsl"}
        };
        auto skyboxShader = m_rendererStorage->m_shaderLibrary.load("skybox-shader", skyboxShaderSource);

        // Set a single "skybox" texture
        skyboxShader->setUniform1i("skybox", 0);

        // Link  uniform buffers
        skyboxShader->linkUniformBuffer(m_rendererStorage->m_scenePropertiesUniformBuffer);

    }

    void DeferredRenderer::loadDefaultWhiteTexture() {

        GL_PROFILE_FUNCTION;

        // Create a 1x1 white texture, to be used as default
        unsigned int whiteTextureData = 0xffffffff;
        Core::Ref<Graphics::Texture> whiteTexture = Core::CreateRef<Graphics::TextureImage2D>(Core::TextureDataFormat::RGBA, Core::TextureDataFormat::RGBA, 1, 1, &whiteTextureData);
        m_rendererStorage->m_whiteTexture = whiteTexture;

        // Add it to the polygon and circle batches
        m_rendererStorage->m_polygonBatch.addTexture(whiteTexture);
        m_rendererStorage->m_circleBatch.addTexture(whiteTexture);

    }

}