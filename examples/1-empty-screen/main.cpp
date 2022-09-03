#include "../include/Core.h"

int main() {

    auto window = gaunlet::Core::CreateRef<gaunlet::Core::Window>("Empty Screen");
    gaunlet::Core::RunLoop runLoop(window);
    gaunlet::Core::Application app;

    runLoop.run(app);

    return 0;

}