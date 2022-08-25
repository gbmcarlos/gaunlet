add_executable(example-2-render-triangle
        ${PROJECT_SOURCE_DIR}/examples/2-render-triangle/main.cpp
    )
target_link_libraries(example-2-render-triangle PUBLIC graphics-engine)
