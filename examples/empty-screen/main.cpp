
#include "Core.h"

class RenderTriangleApplication : public engine::Application {
    void onReady() override {}

    void onUpdate() override {}

    void onRender() override {}

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