add_executable(example-2-render-triangle
        ${PROJECT_SOURCE_DIR}/examples/2-render-triangle/main.cpp
    )
find_package(graphics-engine REQUIRED)
target_link_libraries(example-2-render-triangle PUBLIC graphics-engine::graphics-engine)
