add_executable(example-3-basic-rendering
        ${PROJECT_SOURCE_DIR}/examples/3-basic-rendering/main.cpp
    )

#find_package(GraphicsEngine REQUIRED)
target_link_libraries(example-3-basic-rendering GraphicsEngine::graphics-engine)
