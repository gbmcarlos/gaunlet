#include <Graphics.h>

#include <memory>

class RenderTriangleApplication : public engine::Application {

public:
    explicit RenderTriangleApplication(const std::string& name) : engine::Application(name){}

    std::shared_ptr<engine::VertexArray> m_vertexArray;
    std::shared_ptr<engine::VertexBuffer> m_vertexBuffer;
    std::shared_ptr<engine::IndexBuffer> m_indexBuffer;
    engine::ShaderLibrary m_shaderLibrary;

    float color[4] = {
        1.0f, 1.0f, 1.0f, 1.0f
    };

    void onReady() override {

        float vertices[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.0f, 0.5f
        };

        engine::BufferLayout layout = {
            {"a_position", 2, engine::VertexBufferLayoutElementType::Float}
        };

        // Create the vertex buffer, which contains the actual data
        m_vertexBuffer = std::make_shared<engine::VertexBuffer>(layout, &vertices, sizeof(vertices));

        unsigned int indices[] = {
            0, 1, 2
        };

        // Create an index buffer, which specifies how to use the vertices to draw triangles
        m_indexBuffer = std::make_shared<engine::IndexBuffer>(indices, 3);

        // Create a vertex array, and bind the vertex buffer and the index buffer into it
        m_vertexArray = std::make_shared<engine::VertexArray>();
        m_vertexArray->addBuffer(m_vertexBuffer, m_indexBuffer);

        std::map<engine::ShaderType, std::string> shaderSource {
            {engine::ShaderType::Vertex, "res/shaders/vertex-position.glsl"},
            {engine::ShaderType::Fragment, "res/shaders/fragment-color.glsl"}
        };
        auto shader = m_shaderLibrary.load("main", shaderSource);
        shader->setUniform4f("u_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        shader->bind();

    }

    void onUpdate(engine::TimeStep timeStep) override {

        m_vertexArray->bind();
        m_indexBuffer->bind();

        m_shaderLibrary.get("main")->setUniform4f("u_color", glm::vec4(color[0], color[1], color[2], color[3]));

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        engine::RenderCommand::drawIndexedTriangles(m_vertexArray->getIndexBuffer()->getCount());

    }

    void onGuiRender() override {

        ImGui::Begin("Controls");
        ImGui::ColorPicker4("Color", color);
        ImGui::End();

    }

};

int main() {

    RenderTriangleApplication app("Render Triangle");
    engine::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}