add_executable(example-render-triangle
        ${PROJECT_SOURCE_DIR}/examples/render-triangle/main.cpp
    )
target_link_libraries(example-render-triangle graphics-engine)
