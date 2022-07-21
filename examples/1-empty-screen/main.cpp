#include "Core.h"

int main() {

    engine::Application app("Empty Screen");
    engine::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}