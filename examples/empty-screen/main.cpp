#include "application/Application.h"
#include "window/Window.h"

class EmptyScreenApplication : engine::Application {
    void onReady() override {}

    void onUpdate() override {}

    void onRender() override {}

    void onGuiRender() override {}
};

int main() {

    engine::Window window("Empty Screen");

    EmptyScreenApplication app;

    return 0;

}