#include "../include/Core.h"

int main() {

    gaunlet::Core::Application app("Empty Screen");
    gaunlet::Core::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}