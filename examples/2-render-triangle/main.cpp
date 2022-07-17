#include <Core.h>
#include <imgui.h>

#include <memory>

class RenderTriangleApplication : public engine::Application {

    std::shared_ptr<engine::VertexArray> vertexArray;
    std::shared_ptr<engine::VertexBuffer> vertexBuffer;
    std::shared_ptr<engine::IndexBuffer> indexBuffer;
    std::shared_ptr<engine::Shader> shader;

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
            {"a_position", 2, engine::LayoutElementType::Float}
        };

        // Create the vertex buffer, which contains the actual data
        vertexBuffer = std::make_shared<engine::VertexBuffer>(layout, &vertices, sizeof(vertices));

        unsigned int indices[] = {
                0, 1, 2
        };

        // Create an index buffer, which specifies how to use the vertices to draw triangles
        indexBuffer = std::make_shared<engine::IndexBuffer>(indices, 3);

        // Create a vertex array, and bind the vertex buffer and the index buffer into it
        vertexArray = std::make_shared<engine::VertexArray>();
        vertexArray->addBuffer(vertexBuffer, indexBuffer);

        shader = std::make_shared<engine::Shader>();
        shader->attach(GL_VERTEX_SHADER, "res/shaders/vertex-position.glsl");
        shader->attach(GL_FRAGMENT_SHADER, "res/shaders/fragment-color.glsl");
        shader->compile();
        shader->bind();

    }

    void onUpdate(engine::TimeStep timeStep) override {

        shader->setUniform4f("u_color", glm::vec4(color[0], color[1], color[2], color[3]));

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        engine::Renderer::submit(shader, vertexArray);

    }

    void onGuiRender() override {

        ImGui::Begin("Controls");
        ImGui::ColorPicker4("Color", color);
        ImGui::End();

    }

    virtual void onEvent(const engine::Event& event) override {}
};

int main() {

    engine::Window window("Render Triangle");
    engine::RunLoop runLoop(window);

    RenderTriangleApplication app;

    runLoop.run(app);

    return 0;

}