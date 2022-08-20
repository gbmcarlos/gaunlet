add_executable(example-3-rendering-2d
        ${PROJECT_SOURCE_DIR}/examples/3-rendering-2d/main.cpp
    )

#find_package(GraphicsEngine REQUIRED)
target_link_libraries(example-3-rendering-2d GraphicsEngine::graphics-engine)
