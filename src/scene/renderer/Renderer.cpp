#include "Renderer.h"

#include "../../graphics/texture/TextureImage2D.h"

namespace engine {

    Renderer::RendererStorage* Renderer::m_rendererStorage = new RendererStorage;

    void Renderer::init() {
        loadDefaultShaders();
        loadDefaultWhiteTexture();
    }

    void Renderer::beginScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const Ref<Framebuffer>& framebuffer) {

        std::vector<glm::mat4> sceneData;
        sceneData.emplace_back(viewMatrix);
        sceneData.emplace_back(projectionMatrix);

        m_rendererStorage->m_sceneMatricesUniformBuffer->setData(
            (const void*) sceneData.data(),
            sizeof(glm::mat4) * 2
        );

        if (framebuffer != nullptr) {
            framebuffer->clear();
        } else {
            RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        }

        m_rendererStorage->m_framebuffer = framebuffer;
    }

    void Renderer::endScene() {

        // Flush the polygons, if any
        if (!m_rendererStorage->m_polygonBatch.getVertices().empty()) {
            flushPolygons();
        }

        // Flush the circles, if any
        if (!m_rendererStorage->m_circleBatch.getVertices().empty()) {
            flushCircles();
        }

    }

    void Renderer::submit(int entityId, const PolygonModelComponent& polygonComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent) {

        std::vector<PolygonVertex> vertices = {};
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

    void Renderer::submit(int entityId, const CircleModelComponent& circleComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent) {

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
    void Renderer::flushPolygons() {

        auto& vertices = m_rendererStorage->m_polygonBatch.getVertices();
        auto& indices = m_rendererStorage->m_polygonBatch.getIndices();
        auto& textures = m_rendererStorage->m_polygonBatch.getTextures();
        auto& entityProperties = m_rendererStorage->m_polygonBatch.getEntityProperties();

        renderPolygons(
            vertices,
            indices,
            textures,
            entityProperties,
            m_rendererStorage->m_shaderLibrary.get("polygon-shader")
        );

        m_rendererStorage->m_polygonBatch.clear();
        m_rendererStorage->m_polygonBatch.addTexture(m_rendererStorage->m_whiteTexture);

    }

    // Gather the circle data back from the batch, submit it for drawing, and clear the batch
    void Renderer::flushCircles() {

        auto& vertices = m_rendererStorage->m_circleBatch.getVertices();
        auto& indices = m_rendererStorage->m_circleBatch.getIndices();
        auto& textures = m_rendererStorage->m_circleBatch.getTextures();
        auto& entityProperties = m_rendererStorage->m_circleBatch.getEntityProperties();

        renderCircles(
            vertices,
            indices,
            textures,
            entityProperties,
            m_rendererStorage->m_shaderLibrary.get("circle-shader")
        );

        m_rendererStorage->m_circleBatch.clear();
        m_rendererStorage->m_circleBatch.addTexture(m_rendererStorage->m_whiteTexture);

    }

    // Create the actual polygon drawing resources (buffers and such), prepare the shader, and make the draw call
    void Renderer::renderPolygons(const std::vector<PolygonVertex>& polygonVertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures, const std::vector<PolygonEntityProperties>& entityProperties, const Ref<Shader>& shader) {

        // Create a layout, based on the structure of PolygonVertex
        static engine::BufferLayout polygonVerticesLayout = {
            {"a_position", 4, engine::PrimitiveDataType::Float},
            {"a_normal", 4, engine::PrimitiveDataType::Float},
            {"a_textureCoordinates", 2, engine::PrimitiveDataType::Float},
            {"a_entityIndex", 1, engine::PrimitiveDataType::UInt},
        };

        // Create the vertex and index buffers
        VertexBuffer vertexBuffer(polygonVerticesLayout, (const void*) polygonVertices.data(), sizeof(PolygonVertex) * polygonVertices.size());
        IndexBuffer indexBuffer((unsigned int*) indices.data(), indices.size());

        // Bind them together into a vertex array
        VertexArray vertexArray;
        vertexArray.addBuffer(polygonVerticesLayout);

        // Bind all the textures
        for (unsigned int i = 0; i < textures.size(); i++) {
            textures[i]->activate(i);
        }

        // Bind the shader and submit the view*projection matrix as a uniform
        m_rendererStorage->m_polygonEntityPropertiesUniformBuffer->setData(
            (const void*) entityProperties.data(),
            sizeof(PolygonEntityProperties) * entityProperties.size()
        );

        if (m_rendererStorage->m_framebuffer != nullptr) {
            m_rendererStorage->m_framebuffer->bind();
        }

        // Render the polygons as triangles
        RenderCommand::drawIndexedTriangles(
            vertexBuffer.getRendererId(),
            indexBuffer.getRendererId(),
            vertexArray.getRendererId(),
            shader->getRendererId(),
            indexBuffer.getCount()
        );

        if (m_rendererStorage->m_framebuffer != nullptr) {
            m_rendererStorage->m_framebuffer->unbind();
        }

    }

    // Create the actual circle drawing resources (buffers and such), prepare the shader, and make the draw call
    void Renderer::renderCircles(const std::vector<CircleVertex>& circleVertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures, const std::vector<CircleEntityProperties>& entityProperties, const Ref<Shader>& shader) {

        // Create a layout, based on the structure of CircleVertex
        static engine::BufferLayout circleVerticesLayout = {
            {"a_position", 4, engine::PrimitiveDataType::Float},
            {"a_localCoordinates", 2, engine::PrimitiveDataType::Float},
            {"a_textureCoordinates", 2, engine::PrimitiveDataType::Float},
            {"a_entityIndex", 1, engine::PrimitiveDataType::UInt},
        };

        // Create the vertex and index buffers
        VertexBuffer vertexBuffer(circleVerticesLayout, (const void*) circleVertices.data(), sizeof(CircleVertex) * circleVertices.size());
        IndexBuffer indexBuffer((unsigned int*) indices.data(), indices.size());

        // Bind them together into a vertex array
        VertexArray vertexArray;
        vertexArray.addBuffer(circleVerticesLayout);

        // Bind all the textures
        for (unsigned int i = 0; i < textures.size(); i++) {
            textures[i]->activate(i);
        }

        // Bind the shader and submit the view*projection matrix as a uniform
        m_rendererStorage->m_circleEntityPropertiesUniformBuffer->setData(
            (const void*) entityProperties.data(),
            sizeof(CircleEntityProperties) * entityProperties.size()
        );

        if (m_rendererStorage->m_framebuffer != nullptr) {
            m_rendererStorage->m_framebuffer->bind();
        }

        // Render the circles as triangles (the circle shader will do the rest)
        RenderCommand::drawIndexedTriangles(
            vertexBuffer.getRendererId(),
            indexBuffer.getRendererId(),
            vertexArray.getRendererId(),
            shader->getRendererId(),
            indexBuffer.getCount()
        );

        if (m_rendererStorage->m_framebuffer != nullptr) {
            m_rendererStorage->m_framebuffer->unbind();
        }

    }

    // Create the shaders and set the texture slots
    void Renderer::loadDefaultShaders() {

        GE_PROFILE_FUNCTION;

        m_rendererStorage->m_sceneMatricesUniformBuffer = CreateRef<UniformBuffer>(
            "SceneMatricesBlock",
            0,
            sizeof (glm::mat4) * 2 // We are going to store the view and projection matrices
        );

        // Create the polygon shader
        std::map<engine::ShaderType, std::string> polygonShaderSource {
            {engine::ShaderType::Vertex, ASSETS_PATH"/shaders/2d/polygon-vertex-position-2d.glsl"},
            {engine::ShaderType::Fragment, ASSETS_PATH"/shaders/2d/polygon-color-texture-2d.glsl"}
        };
        auto polygonShader = m_rendererStorage->m_shaderLibrary.load("polygon-shader", polygonShaderSource);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_rendererStorage->m_polygonBatch.getMaxTextures(); i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            polygonShader->setUniform1i(textureName, i);
        }

        m_rendererStorage->m_polygonEntityPropertiesUniformBuffer = CreateRef<UniformBuffer>(
            "EntityPropertiesBlock",
            1,
            sizeof (PolygonEntityProperties) * 100
        );

        // Link the SceneMatrices and the EntityProperties uniform buffers
        polygonShader->linkUniformBuffer(m_rendererStorage->m_sceneMatricesUniformBuffer);
        polygonShader->linkUniformBuffer(m_rendererStorage->m_polygonEntityPropertiesUniformBuffer);

        // Create the circle shader
        std::map<engine::ShaderType, std::string> circleShaderSource {
            {engine::ShaderType::Vertex, ASSETS_PATH"/shaders/2d/circle-vertex-position-2d.glsl"},
            {engine::ShaderType::Fragment, ASSETS_PATH"/shaders/2d/circle-color-texture-2d.glsl"}
        };
        auto circleShader = m_rendererStorage->m_shaderLibrary.load("circle-shader", circleShaderSource);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_rendererStorage->m_circleBatch.getMaxTextures(); i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            circleShader->setUniform1i(textureName, i);
        }

        m_rendererStorage->m_circleEntityPropertiesUniformBuffer = CreateRef<UniformBuffer>(
            "EntityPropertiesBlock",
            2,
            sizeof (CircleEntityProperties) * 100
        );

        // Link the SceneMatrices and the EntityProperties uniform buffers
        circleShader->linkUniformBuffer(m_rendererStorage->m_sceneMatricesUniformBuffer);
        circleShader->linkUniformBuffer(m_rendererStorage->m_circleEntityPropertiesUniformBuffer);

    }

    void Renderer::loadDefaultWhiteTexture() {

        GE_PROFILE_FUNCTION;

        // Create a 1x1 white texture, to be used as default
        unsigned int whiteTextureData = 0xffffffff;
        Ref<engine::Texture> whiteTexture = CreateRef<engine::TextureImage2D>(TextureDataFormat::RGBA, TextureDataFormat::RGBA, 1, 1, &whiteTextureData);
        m_rendererStorage->m_whiteTexture = whiteTexture;

        // Add it to the polygon and circle batches
        m_rendererStorage->m_polygonBatch.addTexture(whiteTexture);
        m_rendererStorage->m_circleBatch.addTexture(whiteTexture);

    }

}