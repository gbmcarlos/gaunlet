#include "../include/Graphics.h"

class RenderTriangleApplication : public gaunlet::Core::Application {

public:
    explicit RenderTriangleApplication(const std::string& name) : gaunlet::Core::Application(name){}

    gaunlet::Core::Ref<gaunlet::Graphics::VertexArray> m_vertexArray;
    gaunlet::Core::Ref<gaunlet::Graphics::VertexBuffer> m_vertexBuffer;
    gaunlet::Core::Ref<gaunlet::Graphics::IndexBuffer> m_indexBuffer;
    gaunlet::Graphics::ShaderLibrary m_shaderLibrary;

    float color[4] = {
        1.0f, 1.0f, 1.0f, 1.0f
    };

    void onReady() override {

        float vertices[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.0f, 0.5f
        };

        gaunlet::Graphics::BufferLayout layout = {
            {"a_position", 2, gaunlet::Core::PrimitiveDataType::Float}
        };

        // Create the vertex buffer, which contains the actual data
        m_vertexBuffer = gaunlet::Core::CreateRef<gaunlet::Graphics::VertexBuffer>(&vertices, sizeof(vertices));

        unsigned int indices[] = {
            0, 1, 2
        };

        // Create an index buffer, which specifies how to use the vertices to draw triangles
        m_indexBuffer = gaunlet::Core::CreateRef<gaunlet::Graphics::IndexBuffer>(indices, 3);

        // Create a vertex array, and bind the vertex buffer and the index buffer into it
        m_vertexArray = gaunlet::Core::CreateRef<gaunlet::Graphics::VertexArray>();
        m_vertexArray->addBuffer(layout);

        std::map<gaunlet::Core::ShaderType, std::string> shaderSource {
            {gaunlet::Core::ShaderType::Vertex, "res/shaders/vertex-position.glsl"},
            {gaunlet::Core::ShaderType::Fragment, "res/shaders/fragment-color.glsl"}
        };
        auto shader = m_shaderLibrary.load("main", shaderSource);
        shader->setUniform4f("u_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {

        auto& shader = m_shaderLibrary.get("main");

        shader->setUniform4f("u_color", glm::vec4(color[0], color[1], color[2], color[3]));

        gaunlet::Core::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        gaunlet::Core::RenderCommand::drawIndexedTriangles(
            m_vertexBuffer->getRendererId(),
            m_indexBuffer->getRendererId(),
            m_vertexArray->getRendererId(),
            shader->getRendererId(),
            m_indexBuffer->getCount()
        );

    }

    void onGuiRender() override {

        ImGui::Begin("Controls");
        ImGui::ColorPicker4("Color", color);
        ImGui::End();

    }

};

int main() {

    RenderTriangleApplication app("Render Triangle");
    gaunlet::Core::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}