add_executable(example-4-viewports
        ${PROJECT_SOURCE_DIR}/examples/4-viewports/main.cpp
    )

target_link_libraries(example-4-viewports GraphicsEngine::graphics-engine)
