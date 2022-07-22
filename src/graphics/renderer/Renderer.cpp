#include "Renderer.h"

#include <map>

namespace engine {

    Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData{OrthographicCamera::getDefaultViewProjectionMatrix()};

    Renderer::RendererStorage* Renderer::m_rendererStorage = new RendererStorage;

    ShaderLibrary Renderer::m_shaderLibrary = ShaderLibrary();

    void Renderer::init() {
        loadDefaultShaders();
        loadDefaultWhiteTexture();
    }

    void Renderer::beginScene(const std::shared_ptr<OrthographicCamera>& orthographicCamera) {
        m_sceneData->m_viewProjectionMatrix = orthographicCamera->getViewProjectionMatrix();
    }

    void Renderer::endScene() {
        m_sceneData = new SceneData{OrthographicCamera::getDefaultViewProjectionMatrix()};
    }

    void Renderer::submit(const Mesh& mesh, const glm::mat4& transform, glm::vec4 color) {

    }

    void Renderer::submit(const Mesh& mesh, const glm::mat4& transform, glm::vec4 color, const std::shared_ptr<Texture>& texture) {

        auto vertices = mesh.getVertices();
        auto indices = mesh.getFaceIndices();

        if (shouldFlush(vertices, indices, texture)) {
            flush();
        }

        int textureIndex = 0.0f;
        if (texture) {

            // Find the texture in the list of existing textures
            auto iterator = std::find(m_rendererStorage->m_textures.begin(), m_rendererStorage->m_textures.end(), texture);

            // If it isn't there, we need to add it
            if (iterator == m_rendererStorage->m_textures.end()) {
                textureIndex = m_rendererStorage->m_textures.size();
                m_rendererStorage->m_textures.push_back(texture);
            } else { // Otherwise, retrieve the index of the existing one
                textureIndex = (int) std::distance(m_rendererStorage->m_textures.begin(), iterator);
            }

        }

        // Transform the vertices
        for (auto& vertex : vertices) {
            vertex.m_position = transform * vertex.m_position;
            vertex.m_textureIndex = textureIndex;
            vertex.m_color = color;
        }

        // Transform the indices
        for (auto& index : indices) {
            index += m_rendererStorage->m_vertices.size();
        }

        m_rendererStorage->m_vertices.insert(m_rendererStorage->m_vertices.end(), vertices.begin(), vertices.end());
        m_rendererStorage->m_indices.insert(m_rendererStorage->m_indices.end(), indices.begin(), indices.end());

    }

    bool Renderer::shouldFlush(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture>& texture) {

        if (m_rendererStorage->m_vertices.size() + vertices.size() > m_rendererStorage->m_maxVertices) {
            return true;
        }

        if (m_rendererStorage->m_indices.size() + indices.size() > m_rendererStorage->m_maxIndices) {
            return true;
        }

        // We only need to check the textures if this is a new one
        if (texture && std::find(m_rendererStorage->m_textures.begin(), m_rendererStorage->m_textures.end(), texture) == m_rendererStorage->m_textures.end()) {

            if (m_rendererStorage->m_textures.size() +1 > m_rendererStorage->m_maxTextures) {
                return true;
            }

        }

        return false;

    }

    void Renderer::flush() {

        static engine::BufferLayout layout = {
                {"a_position", 4, engine::VertexBufferLayoutElementType::Float},
                {"a_textureCoordinates", 2, engine::VertexBufferLayoutElementType::Float},
                {"a_textureIndex", 1, engine::VertexBufferLayoutElementType::Int},
                {"a_color", 4, engine::VertexBufferLayoutElementType::Float},
        };

        // Create the vertex and index buffers
        std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(layout, (const void*) m_rendererStorage->m_vertices.data(), sizeof(Vertex) * m_rendererStorage->m_vertices.size());
        std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>((unsigned int*) m_rendererStorage->m_indices.data(), m_rendererStorage->m_indices.size());

        // Bind them together into a vertex array
        std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
        vertexArray->addBuffer(vertexBuffer, indexBuffer);

        // Bind all the textures
        for (unsigned int i = 0; i < m_rendererStorage->m_textures.size(); i++) {
            m_rendererStorage->m_textures[i]->bind(i);
        }

        submitTriangles(m_shaderLibrary.get("main"), vertexArray);

        m_rendererStorage->m_vertices.clear();
        m_rendererStorage->m_indices.clear();
        m_rendererStorage->m_textures.clear();
        m_rendererStorage->m_textures.push_back(m_rendererStorage->m_whiteTexture);

    }

    void Renderer::submitTriangles(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {

        shader->bind();
        shader->setUniformMat4f("u_viewProjection", m_sceneData->m_viewProjectionMatrix);

        vertexArray->bind();
        vertexArray->getIndexBuffer()->bind();

        RenderCommand::drawIndexedTriangles(vertexArray->getIndexBuffer()->getCount());

    }

    void Renderer::loadDefaultShaders() {

        std::map<engine::ShaderType, std::string> shaderSource {
            {engine::ShaderType::Vertex, ASSETS_PATH"/shaders/2d/polygon-vertex-shader.glsl"},
            {engine::ShaderType::Fragment, ASSETS_PATH"/shaders/2d/polygon-fragment-shader.glsl"}
        };
        auto shader = m_shaderLibrary.load("main", shaderSource);

        for (int i = 0; i < m_rendererStorage->m_maxTextures; i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            shader->setUniform1i(textureName, i);
        }


    }

    void Renderer::loadDefaultWhiteTexture() {

        unsigned int whiteTextureData = 0xffffff;
        std::shared_ptr<engine::Texture> whiteTexture = std::make_shared<engine::Texture>(1, 1, &whiteTextureData);
        m_rendererStorage->m_whiteTexture = whiteTexture;
        m_rendererStorage->m_textures.push_back(whiteTexture);

    }

}