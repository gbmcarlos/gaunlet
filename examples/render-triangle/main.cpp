#include "Core.h"
#include <imgui.h>

class RenderTriangleApplication : public engine::Application {

    std::unique_ptr<engine::VertexArray> vertexArray;
    std::unique_ptr<engine::VertexBuffer> vertexBuffer;
    std::unique_ptr<engine::IndexBuffer> indexBuffer;
    std::unique_ptr<engine::Shader> shader;

    float color[4] = {
            1.0f, 1.0f, 1.0f, 1.0f
    };

    void onReady() override {

        float vertices[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.0f, 0.5f
        };

        // Create a vertex array, which will bind together the vertex buffer and the layout
        vertexArray = std::make_unique<engine::VertexArray>();

        // Create the vertex buffer, which contains the actual data
        vertexBuffer = std::make_unique<engine::VertexBuffer>(vertices, sizeof(float) * 2 * 4);

        // Create a layout, and specify the structure of the vertex buffer
        engine::VertexBufferLayout vertexBufferLayout;
        vertexBufferLayout.pushFloat(2);

        // Bind the vertex buffer and the layout into the vertex array
        vertexArray->addBuffer(*vertexBuffer, vertexBufferLayout);

        unsigned int indices[] = {
                0, 1, 2
        };

        // Create a index buffer, which specifies how to use the vertices to draw triangles
        indexBuffer = std::make_unique<engine::IndexBuffer>(indices, 6);

        vertexArray->bind();

        shader = std::make_unique<engine::Shader>();
        shader->attach(GL_VERTEX_SHADER, "res/shaders/vertex-position.glsl");
        shader->attach(GL_FRAGMENT_SHADER, "res/shaders/fragment-color.glsl");
        shader->compile();
        shader->bind();

    }

    void onUpdate() override {}

    void onRender() override {

        shader->setUniform4f("u_color", glm::vec4(color[0], color[1], color[2], color[3]));

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

    }

    void onGuiRender() override {

        ImGui::Begin("Controls");
        ImGui::ColorPicker4("Color", color);
        ImGui::End();

    }

    virtual void onEvent(engine::Event& event) override {}
};

int main() {

    engine::Window window("Empty Screen");
    engine::RunLoop runLoop(window);

    RenderTriangleApplication app;

    runLoop.run(&app);

    return 0;

}