add_executable(example-6-viewport-layout
        ${PROJECT_SOURCE_DIR}/examples/6-viewport-layout/main.cpp
    )
find_package(graphics-engine REQUIRED)
target_link_libraries(example-6-viewport-layout PUBLIC graphics-engine::graphics-engine)
