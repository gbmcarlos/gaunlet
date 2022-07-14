#include <Core.h>

#include <array>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class PlayableQuadApplication : public engine::Application {

private:

    std::unique_ptr<engine::VertexArray> vertexArray;
    std::unique_ptr<engine::VertexBuffer> vertexBuffer;
    std::unique_ptr<engine::IndexBuffer> indexBuffer;
    std::unique_ptr<engine::Shader> shader;

    // Quad properties
    glm::vec4 quadColor = {1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec2 quad1Position = {
            300.0f, 300.0f
    };
    float quadSize = 100;
    glm::vec2 quad1Speed = {0.0f, 0.0f};

    // Camera properties
    glm::vec2 cameraPosition = {0.0f, 0.0f};

    // mvp
    glm::mat4 projection;
    glm::mat4 view;

    struct Vertex {
        glm::vec2 position;
    };

    std::array<Vertex, 4> getQuad(float positionX, float positionY, float size) {

        Vertex vertex0 = {
            {positionX, positionY}
        };

        Vertex vertex1 = {
            {positionX + size, positionY}
        };

        Vertex vertex2 = {
            {positionX + size, positionY + size}
        };

        Vertex vertex3 = {
            {positionX, positionY + size}
        };

        return {vertex0, vertex1, vertex2, vertex3};

    }

public:

    PlayableQuadApplication(int windowWidth, int windowHeight) {
        projection = glm::ortho(0.0f, (float) windowWidth, 0.0f, (float) windowHeight);
        view = glm::translate(glm::mat4(1.0f), glm::vec3(cameraPosition.x, cameraPosition.y, 0));
    }

    void onReady() override {

        // Create a vertex array, which will bind together the vertex buffer and the layout
        vertexArray = std::make_unique<engine::VertexArray>();

        engine::BufferLayout layout = {
            {"a_position", 2, engine::LayoutElementType::Float}
        };

        // Create the vertex buffer, which will contain the actual data (2 quads)
        vertexBuffer = std::make_unique<engine::VertexBuffer>(layout, sizeof(Vertex) * 4 * 2);

        // Bind the vertex buffer and the layout into the vertex array
        vertexArray->addBuffer(*vertexBuffer);

        unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0,

                4, 5, 6,
                6, 7, 4
        };

        // Create an index buffer, which specifies how to use the vertices to draw triangles
        indexBuffer = std::make_unique<engine::IndexBuffer>(indices, 12);

        vertexArray->bind();

        shader = std::make_unique<engine::Shader>();
        shader->attach(GL_VERTEX_SHADER, "res/shaders/vertex-position.glsl");
        shader->attach(GL_FRAGMENT_SHADER, "res/shaders/fragment-color.glsl");
        shader->compile();
        shader->bind();
        shader->setUniform4f("u_color", glm::vec4(quadColor.x, quadColor.y, quadColor.z, quadColor.w));

    }

    void onUpdate() override {

        quad1Position.x += quad1Speed.x;
        quad1Position.y += quad1Speed.y;

        // Prepare the quad1, positioned statically
        auto quad1 = getQuad(quad1Position.x, quad1Position.y, quadSize);
        auto quad2 = getQuad(100.0f, 100.0f, quadSize);
        Vertex vertices[8];
        memcpy(vertices, quad1.data(), quad1.size() * sizeof(Vertex));
        memcpy(vertices + quad1.size(), quad2.data(), quad2.size() * sizeof(Vertex));

        // Submit the quad1's data
        vertexBuffer->setData(vertices, sizeof(quad1) * 2);

        shader->setUniformMat4f("u_mvp", projection * view);

    }

    void onRender() override {

        engine::Renderer::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

    }

    void onGuiRender() override {}

    void onEvent(engine::Event& event) override {

        if (event.getType() == engine::EventType::KeyPress) {
            engine::KeyPressEvent derivedEvent = dynamic_cast<engine::KeyPressEvent&>(event);
            if (derivedEvent.getKey() == GLFW_KEY_RIGHT && quad1Speed.x == 0) {
                quad1Speed.x = 10;
                return;
            } else if (derivedEvent.getKey() == GLFW_KEY_LEFT && quad1Speed.x == 0) {
                quad1Speed.x = -10;
                return;
            }
        }

        if (event.getType() == engine::EventType::KeyRelease) {
            engine::KeyReleaseEvent derivedEvent = dynamic_cast<engine::KeyReleaseEvent&>(event);
            if (derivedEvent.getKey() == GLFW_KEY_RIGHT || derivedEvent.getKey() == GLFW_KEY_LEFT) {
                quad1Speed.x = 0;
                return;
            }
        }

        if (event.getType() == engine::EventType::KeyPress) {
            engine::KeyPressEvent derivedEvent = dynamic_cast<engine::KeyPressEvent&>(event);
            if (derivedEvent.getKey() == GLFW_KEY_UP && quad1Speed.y == 0) {
                quad1Speed.y = 10;
                return;
            } else if (derivedEvent.getKey() == GLFW_KEY_DOWN && quad1Speed.y == 0) {
                quad1Speed.y = -10;
                return;
            }
        }

        if (event.getType() == engine::EventType::KeyRelease) {
            engine::KeyReleaseEvent derivedEvent = dynamic_cast<engine::KeyReleaseEvent&>(event);
            if (derivedEvent.getKey() == GLFW_KEY_UP || derivedEvent.getKey() == GLFW_KEY_DOWN) {
                quad1Speed.y = 0;
                return;
            }
        }

    }
};

int main() {

    engine::Window window("Playable Quad");
    engine::RunLoop runLoop(window);

    PlayableQuadApplication app(window.getWidth(), window.getHeight());

    runLoop.run(&app);

    return 0;

}