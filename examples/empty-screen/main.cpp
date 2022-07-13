#include "application/Application.h"

class EmptyScreenApplication : engine::Application {
    void onReady() override {}

    void onUpdate() override {}

    void onRender() override {}

    void onGuiRender() override {}
};

int main() {

    EmptyScreenApplication app;

    return 0;

}