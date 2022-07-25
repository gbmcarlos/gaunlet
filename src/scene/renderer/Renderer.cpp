#include "Renderer.h"

#include <map>

namespace engine {

    Renderer::RendererStorage* Renderer::m_rendererStorage = new RendererStorage;

    void Renderer::init() {
        loadDefaultShaders();
        loadDefaultWhiteTexture();
    }

    void Renderer::beginScene(glm::mat4 viewProjectionMatrix) {
        m_rendererStorage->m_viewProjectionMatrix = viewProjectionMatrix;
    }

    void Renderer::endScene() {

        // Flush the polygons, if any
        if (m_rendererStorage->m_polygonVertices.size() > 0) {
            flushPolygons();
        }

        // Flush the circles, if any
        if (m_rendererStorage->m_circleVertices.size() > 0) {
            flushCircles();
        }

        // Reset the view*projection matrix to the default
        m_rendererStorage->m_viewProjectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    }

    void Renderer::submit(const PolygonComponent& polygonComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent) {

        // Get the vertices and indices to be added
        auto vertices = polygonComponent.m_mesh.getVertices();
        auto indices = polygonComponent.m_mesh.getIndices();

        // Check if we need to flush before rendering the current polygon
        if (shouldFlushPolygon(vertices, indices, materialComponent.m_texture)) {
            flushPolygons();
        }

        // If no texture is specified, we use texture number 1, which is the default white texture
        int textureIndex = 0.0f;
        if (materialComponent.m_texture) {

            // Find the texture in the list of existing textures
            auto iterator = std::find(m_rendererStorage->m_polygonTextures.begin(), m_rendererStorage->m_polygonTextures.end(), materialComponent.m_texture);

            // If it isn't there, we need to add it
            if (iterator == m_rendererStorage->m_polygonTextures.end()) {
                textureIndex = m_rendererStorage->m_polygonTextures.size();
                m_rendererStorage->m_polygonTextures.push_back(materialComponent.m_texture);
            } else { // Otherwise, retrieve the index of the existing one
                textureIndex = (int) std::distance(m_rendererStorage->m_polygonTextures.begin(), iterator);
            }

        }

        // Transform the vertices, according to the polygon's transform and material components
        for (auto& vertex : vertices) {
            vertex.m_position = transformComponent.getTransformationMatrix() * vertex.m_position;
            vertex.m_textureIndex = textureIndex;
            vertex.m_color = materialComponent.m_color;
        }

        // Transform the indices, offset them by the current list of vertices in the batch
        for (auto& index : indices) {
            index += m_rendererStorage->m_polygonVertices.size();
        }

        // Store the vertices and indices in the batch
        m_rendererStorage->m_polygonVertices.insert(m_rendererStorage->m_polygonVertices.end(), vertices.begin(), vertices.end());
        m_rendererStorage->m_polygonIndices.insert(m_rendererStorage->m_polygonIndices.end(), indices.begin(), indices.end());

    }

    void Renderer::submit(const CircleComponent& circleComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent) {

        // Get the vertices and indices to be added
        auto vertices = circleComponent.m_mesh.getVertices();
        auto indices = circleComponent.m_mesh.getIndices();

        // Check if we need to flush before rendering the current circle
        if (shouldFlushCircles(vertices, materialComponent.m_texture)) {
            flushCircles();
        }

        // If no texture is specified, we use texture number 1, which is the default white texture
        int textureIndex = 0.0f;
        if (materialComponent.m_texture) {

            // Find the texture in the list of existing textures
            auto iterator = std::find(m_rendererStorage->m_circleTextures.begin(), m_rendererStorage->m_circleTextures.end(), materialComponent.m_texture);

            // If it isn't there, we need to add it
            if (iterator == m_rendererStorage->m_circleTextures.end()) {
                textureIndex = m_rendererStorage->m_circleTextures.size();
                m_rendererStorage->m_circleTextures.push_back(materialComponent.m_texture);
            } else { // Otherwise, retrieve the index of the existing one
                textureIndex = (int) std::distance(m_rendererStorage->m_circleTextures.begin(), iterator);
            }

        }

        // Transform the vertices, according to the circle's transform, material, and circle components
        for (auto& vertex : vertices) {
            vertex.m_position = transformComponent.getTransformationMatrix() * vertex.m_position;
            vertex.m_thickness = circleComponent.m_thickness;
            vertex.m_fade = circleComponent.m_fade;
            vertex.m_textureIndex = textureIndex;
            vertex.m_color = materialComponent.m_color;
        }

        // Transform the indices, offset them by the current list of vertices in the batch
        for (auto& index : indices) {
            index += m_rendererStorage->m_circleVertices.size();
        }

        // Store the vertices and indices in the batch
        m_rendererStorage->m_circleVertices.insert(m_rendererStorage->m_circleVertices.end(), vertices.begin(), vertices.end());
        m_rendererStorage->m_circleIndices.insert(m_rendererStorage->m_circleIndices.end(), indices.begin(), indices.end());

    }

    bool Renderer::shouldFlushPolygon(const std::vector<PolygonVertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture>& texture) {

        // If rendering the current polygon would pass over the limit of vertices, flush
        if (m_rendererStorage->m_polygonVertices.size() + vertices.size() > m_rendererStorage->m_maxPolygonVertices) {
            return true;
        }

        // If rendering the current polygon would pass over the limit of indices, flush
        if (m_rendererStorage->m_polygonIndices.size() + indices.size() > m_rendererStorage->m_maxPolygonIndices) {
            return true;
        }

        // If we're adding a texture, and it would pass over the limit of textures, flush
        if (texture && std::find(m_rendererStorage->m_polygonTextures.begin(), m_rendererStorage->m_polygonTextures.end(), texture) == m_rendererStorage->m_polygonTextures.end()) {

            if (m_rendererStorage->m_polygonTextures.size() + 1 > m_rendererStorage->m_maxPolygonTextures) {
                return true;
            }

        }

        return false;

    }

    bool Renderer::shouldFlushCircles(const std::vector<CircleVertex>& vertices, const std::shared_ptr<Texture>& texture) {

        // If rendering the current circle would pass over the limit of vertices, flush
        if (m_rendererStorage->m_circleVertices.size() + vertices.size() > m_rendererStorage->m_maxCircleVertices) {
            return true;
        }

        // No need to check for indices limit, because a circle always has 4 vertices

        // If we're adding a texture, and it would pass over the limit of textures, flush
        if (texture && std::find(m_rendererStorage->m_circleTextures.begin(), m_rendererStorage->m_circleTextures.end(), texture) == m_rendererStorage->m_circleTextures.end()) {

            if (m_rendererStorage->m_circleTextures.size() + 1 > m_rendererStorage->m_maxCircleTextures) {
                return true;
            }

        }

        return false;

    }

    void Renderer::flushPolygons() {

        // Create a layout, based on the structure of PolygonVertex
        static engine::BufferLayout layout = {
                {"a_position", 4, engine::VertexBufferLayoutElementType::Float},
                {"a_textureCoordinates", 2, engine::VertexBufferLayoutElementType::Float},
                {"a_textureIndex", 1, engine::VertexBufferLayoutElementType::Int},
                {"a_color", 4, engine::VertexBufferLayoutElementType::Float},
        };

        // Create the vertex and index buffers
        std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(layout, (const void*) m_rendererStorage->m_polygonVertices.data(), sizeof(PolygonVertex) * m_rendererStorage->m_polygonVertices.size());
        std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>((unsigned int*) m_rendererStorage->m_polygonIndices.data(), m_rendererStorage->m_polygonIndices.size());

        // Bind them together into a vertex array
        std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
        vertexArray->addBuffer(vertexBuffer, indexBuffer);

        // Bind all the textures
        for (unsigned int i = 0; i < m_rendererStorage->m_polygonTextures.size(); i++) {
            m_rendererStorage->m_polygonTextures[i]->bind(i);
        }

        // Submit the vertex array, to be rendered by the polygon shader
        submitTriangles(m_rendererStorage->m_shaderLibrary.get("polygon-shader"), vertexArray);

        // Clear the batch vectors (vertices, indices, and textures), and add the white texture back
        m_rendererStorage->m_polygonVertices.clear();
        m_rendererStorage->m_polygonIndices.clear();
        m_rendererStorage->m_polygonTextures.clear();
        m_rendererStorage->m_polygonTextures.push_back(m_rendererStorage->m_whiteTexture);

    }

    void Renderer::flushCircles() {

        // Create a layout, based on the structure of CircleVertex
        static engine::BufferLayout layout = {
            {"a_position", 4, engine::VertexBufferLayoutElementType::Float},
            {"a_localCoordinates", 2, engine::VertexBufferLayoutElementType::Float},
            {"a_thickness", 1, engine::VertexBufferLayoutElementType::Float},
            {"a_fade", 1, engine::VertexBufferLayoutElementType::Float},
            {"a_textureCoordinates", 2, engine::VertexBufferLayoutElementType::Float},
            {"a_textureIndex", 1, engine::VertexBufferLayoutElementType::Int},
            {"a_color", 4, engine::VertexBufferLayoutElementType::Float},
        };

        // Create the vertex and index buffers
        std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(layout, (const void*) m_rendererStorage->m_circleVertices.data(), sizeof(CircleVertex) * m_rendererStorage->m_circleVertices.size());
        std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(m_rendererStorage->m_circleIndices.data(), m_rendererStorage->m_circleIndices.size());

        // Bind them together into a vertex array
        std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
        vertexArray->addBuffer(vertexBuffer, indexBuffer);

        // Bind all the textures
        for (unsigned int i = 0; i < m_rendererStorage->m_circleTextures.size(); i++) {
            m_rendererStorage->m_circleTextures[i]->bind(i);
        }

        // Submit the vertex array, to be rendered by the circle shader
        submitCircles(m_rendererStorage->m_shaderLibrary.get("circle-shader"), vertexArray);

        // Clear the batch vectors (vertices, indices, and textures), and add the white texture back
        m_rendererStorage->m_circleVertices.clear();
        m_rendererStorage->m_circleIndices.clear();
        m_rendererStorage->m_circleTextures.clear();
        m_rendererStorage->m_circleTextures.push_back(m_rendererStorage->m_whiteTexture);

    }

    void Renderer::submitTriangles(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {

        // Bind the shader and submit the view*projection matrix as a uniform
        shader->bind();
        shader->setUniformMat4f("u_viewProjection", m_rendererStorage->m_viewProjectionMatrix);

        // Bind the vertex array and the index buffer
        vertexArray->bind();
        vertexArray->getIndexBuffer()->bind();

        // Render the polygons as triangles
        RenderCommand::drawIndexedTriangles(vertexArray->getIndexBuffer()->getCount());

    }

    void Renderer::submitCircles(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {

        // Bind the shader and submit the view*projection matrix as a uniform
        shader->bind();
        shader->setUniformMat4f("u_viewProjection", m_rendererStorage->m_viewProjectionMatrix);

        // Bind the vertex array and the index buffer
        vertexArray->bind();
        vertexArray->getIndexBuffer()->bind();

        // Render the circles as triangles (the circle shader will do the rest)
        RenderCommand::drawIndexedTriangles(vertexArray->getIndexBuffer()->getCount());

    }

    void Renderer::loadDefaultShaders() {

        // Create the polygon shader
        std::map<engine::ShaderType, std::string> polygonShaderSource {
            {engine::ShaderType::Vertex, ASSETS_PATH"/shaders/2d/polygon-vertex-position-2d.glsl"},
            {engine::ShaderType::Fragment, ASSETS_PATH"/shaders/2d/polygon-color-texture-2d.glsl"}
        };
        auto polygonShader = m_rendererStorage->m_shaderLibrary.load("polygon-shader", polygonShaderSource);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_rendererStorage->m_maxPolygonTextures; i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            polygonShader->setUniform1i(textureName, i);
        }

        // Create the circle shader
        std::map<engine::ShaderType, std::string> circleShaderSource {
            {engine::ShaderType::Vertex, ASSETS_PATH"/shaders/2d/circle-vertex-position-2d.glsl"},
            {engine::ShaderType::Fragment, ASSETS_PATH"/shaders/2d/circle-color-texture-2d.glsl"}
        };
        auto circleShader = m_rendererStorage->m_shaderLibrary.load("circle-shader", circleShaderSource);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_rendererStorage->m_maxCircleTextures; i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            circleShader->setUniform1i(textureName, i);
        }

    }

    void Renderer::loadDefaultWhiteTexture() {

        // Create a 1x1 white texture, to be used as default
        unsigned int whiteTextureData = 0xffffff;
        std::shared_ptr<engine::Texture> whiteTexture = std::make_shared<engine::Texture>(1, 1, &whiteTextureData);
        m_rendererStorage->m_whiteTexture = whiteTexture;

        // Add it to the polygon and circle texture vectors
        m_rendererStorage->m_polygonTextures.push_back(whiteTexture);
        m_rendererStorage->m_circleTextures.push_back(whiteTexture);

    }

}