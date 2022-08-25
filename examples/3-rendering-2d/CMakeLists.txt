add_executable(example-3-rendering-2d
        ${PROJECT_SOURCE_DIR}/examples/3-rendering-2d/main.cpp
    )
target_link_libraries(example-3-rendering-2d PUBLIC graphics-engine)
