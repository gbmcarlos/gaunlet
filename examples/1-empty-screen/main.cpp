#include <core/Core.h>

class EmptyScreenApplication : public engine::Application {

public:
    explicit EmptyScreenApplication(const std::string& name) : engine::Application(name) {}

};

int main() {

    EmptyScreenApplication app("Empty Screen");
    engine::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}