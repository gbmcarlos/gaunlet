#include <iostream>

#include "Core.h"

class EmptyScreenApplication : public engine::Application {
    void onReady() override {}

    void onUpdate() override {}

    void onRender() override {}

    void onGuiRender() override {}

    virtual void onEvent(engine::Event& event) override {
        std::cout << "Event" << std::endl;
    }
};

int main() {

    engine::Window window("Empty Screen");
    engine::RunLoop runLoop(window);

    EmptyScreenApplication app;

    runLoop.run(&app);

    return 0;

}