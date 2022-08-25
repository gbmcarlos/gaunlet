add_executable(example-1-empty-screen
        ${PROJECT_SOURCE_DIR}/examples/1-empty-screen/main.cpp
    )
find_package(graphics-engine REQUIRED)
target_link_libraries(example-1-empty-screen PUBLIC graphics-engine::graphics-engine)
