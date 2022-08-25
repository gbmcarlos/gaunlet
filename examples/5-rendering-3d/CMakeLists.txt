add_executable(example-5-rendering-3d
        ${PROJECT_SOURCE_DIR}/examples/5-rendering-3d/main.cpp
    )
find_package(graphics-engine REQUIRED)
target_link_libraries(example-5-rendering-3d PUBLIC graphics-engine::graphics-engine)
