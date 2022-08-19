#include "Renderer.h"

#include "../../graphics/texture/TextureImage2D.h"

namespace engine {

    Renderer::RendererStorage* Renderer::m_rendererStorage = new RendererStorage;

    void Renderer::init() {
        loadDefaultShaders();
        loadDefaultWhiteTexture();
    }

    void Renderer::beginScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

        std::vector<glm::mat4> sceneData;
        sceneData.emplace_back(viewMatrix);
        sceneData.emplace_back(projectionMatrix);

        m_rendererStorage->m_sceneMatricesUniformBuffer->setData(
            (const void*) sceneData.data(),
            sizeof(glm::mat4) * 2
        );
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

    void Renderer::submit(const unsigned int& entityId, const PolygonModelComponent& polygonComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent) {

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

    void Renderer::submit(const unsigned int& entityId, const CircleModelComponent& circleComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent) {

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

        submitPolygons(
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

        submitCircles(
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
    void Renderer::submitPolygons(const std::vector<PolygonVertex>& polygonVertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures, const std::vector<PolygonEntityProperties>& entityProperties, const std::shared_ptr<Shader>& shader) {

        // Create a layout, based on the structure of PolygonVertex
        static engine::BufferLayout polygonVerticesLayout = {
            {"a_position", 4, engine::VertexBufferLayoutElementType::Float},
            {"a_normal", 4, engine::VertexBufferLayoutElementType::Float},
            {"a_textureCoordinates", 2, engine::VertexBufferLayoutElementType::Float},
            {"a_entityIndex", 1, engine::VertexBufferLayoutElementType::UInt},
        };

        // Create the vertex and index buffers
        std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(polygonVerticesLayout, (const void*) polygonVertices.data(), sizeof(PolygonVertex) * polygonVertices.size());
        std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>((unsigned int*) indices.data(), indices.size());

        // Bind them together into a vertex array
        std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
        vertexArray->addBuffer(vertexBuffer, indexBuffer);

        // Bind all the textures
        for (unsigned int i = 0; i < textures.size(); i++) {
            textures[i]->bind(i);
        }

        // Bind the shader and submit the view*projection matrix as a uniform
        shader->bind();
        m_rendererStorage->m_polygonEntityPropertiesUniformBuffer->setData(
            (const void*) entityProperties.data(),
            sizeof(PolygonEntityProperties) * entityProperties.size()
        );

        // Bind the vertex array and the index buffer
        vertexArray->bind();
        vertexArray->getIndexBuffer()->bind();

        // Render the polygons as triangles
        RenderCommand::drawIndexedTriangles(vertexArray->getIndexBuffer()->getCount());

    }

    // Create the actual circle drawing resources (buffers and such), prepare the shader, and make the draw call
    void Renderer::submitCircles(const std::vector<CircleVertex>& circleVertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures, const std::vector<CircleEntityProperties>& entityProperties, const std::shared_ptr<Shader>& shader) {

        // Create a layout, based on the structure of CircleVertex
        static engine::BufferLayout circleVerticesLayout = {
            {"a_position", 4, engine::VertexBufferLayoutElementType::Float},
            {"a_localCoordinates", 2, engine::VertexBufferLayoutElementType::Float},
            {"a_textureCoordinates", 2, engine::VertexBufferLayoutElementType::Float},
            {"a_entityIndex", 1, engine::VertexBufferLayoutElementType::UInt},
        };

        // Create the vertex and index buffers
        std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(circleVerticesLayout, (const void*) circleVertices.data(), sizeof(CircleVertex) * circleVertices.size());
        std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>((unsigned int*) indices.data(), indices.size());

        // Bind them together into a vertex array
        std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
        vertexArray->addBuffer(vertexBuffer, indexBuffer);

        // Bind all the textures
        for (unsigned int i = 0; i < textures.size(); i++) {
            textures[i]->bind(i);
        }

        // Bind the shader and submit the view*projection matrix as a uniform
        shader->bind();
        m_rendererStorage->m_circleEntityPropertiesUniformBuffer->setData(
            (const void*) entityProperties.data(),
            sizeof(CircleEntityProperties) * entityProperties.size()
        );

        // Bind the vertex array and the index buffer
        vertexArray->bind();
        vertexArray->getIndexBuffer()->bind();

        // Render the circles as triangles (the circle shader will do the rest)
        RenderCommand::drawIndexedTriangles(vertexArray->getIndexBuffer()->getCount());

    }

    // Create the shaders and set the texture slots
    void Renderer::loadDefaultShaders() {

        GE_PROFILE_FUNCTION;

        m_rendererStorage->m_sceneMatricesUniformBuffer = std::make_shared<UniformBuffer>(
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

        m_rendererStorage->m_polygonEntityPropertiesUniformBuffer = std::make_shared<UniformBuffer>(
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

        m_rendererStorage->m_circleEntityPropertiesUniformBuffer = std::make_shared<UniformBuffer>(
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
        std::shared_ptr<engine::Texture> whiteTexture = std::make_shared<engine::TextureImage2D>(TextureDataFormat::RGBA, TextureDataFormat::RGBA, 1, 1, &whiteTextureData);
        m_rendererStorage->m_whiteTexture = whiteTexture;

        // Add it to the polygon and circle batches
        m_rendererStorage->m_polygonBatch.addTexture(whiteTexture);
        m_rendererStorage->m_circleBatch.addTexture(whiteTexture);

    }

}