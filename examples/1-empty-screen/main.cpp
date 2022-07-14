#include <Core.h>

class RenderTriangleApplication : public engine::Application {
    void onReady() override {}

    void onUpdate(engine::TimeStep timeStep) override {}

    void onGuiRender() override {}

    virtual void onEvent(engine::Event& event) override {}
};

int main() {

    engine::Window window("Empty Screen");
    engine::RunLoop runLoop(window);

    RenderTriangleApplication app;

    runLoop.run(&app);

    return 0;

}