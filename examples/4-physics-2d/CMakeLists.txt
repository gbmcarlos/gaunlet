add_executable(example-4-physics-2d
        ${PROJECT_SOURCE_DIR}/examples/4-physics-2d/main.cpp
    )
find_package(graphics-engine REQUIRED)
target_link_libraries(example-4-physics-2d PUBLIC graphics-engine::graphics-engine)
