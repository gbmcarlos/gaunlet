add_library(graphics-engine
        ${PROJECT_SOURCE_DIR}/src/window/Window.h ${PROJECT_SOURCE_DIR}/src/window/Window.cpp
        ${PROJECT_SOURCE_DIR}/src/render/opengl/OpenGLRenderAPI.h ${PROJECT_SOURCE_DIR}/src/render/opengl/OpenGLRenderAPI.cpp ${PROJECT_SOURCE_DIR}/src/render/opengl/opengl_utils.h
        ${PROJECT_SOURCE_DIR}/src/render/imgui/ImGuiRenderApi.h ${PROJECT_SOURCE_DIR}/src/render/imgui/ImGuiRenderApi.cpp
        ${PROJECT_SOURCE_DIR}/src/render/Renderer.h ${PROJECT_SOURCE_DIR}/src/render/Renderer.cpp
        ${PROJECT_SOURCE_DIR}/src/application/Application.h
        ${PROJECT_SOURCE_DIR}/src/run-loop/run_loop_utils.h ${PROJECT_SOURCE_DIR}/src/run-loop/RunLoop.h ${PROJECT_SOURCE_DIR}/src/run-loop/RunLoop.cpp
        ${PROJECT_SOURCE_DIR}/src/input/Input.h ${PROJECT_SOURCE_DIR}/src/input/Event.h ${PROJECT_SOURCE_DIR}/src/input/EventBus.h ${PROJECT_SOURCE_DIR}/src/input/EventBus.cpp
        ${PROJECT_SOURCE_DIR}/src/input/events/KeyPressEvent.h ${PROJECT_SOURCE_DIR}/src/input/events/KeyRepeatEvent.h ${PROJECT_SOURCE_DIR}/src/input/events/KeyRepeatEvent.h
        ${PROJECT_SOURCE_DIR}/src/buffer/VertexBuffer.h ${PROJECT_SOURCE_DIR}/src/buffer/VertexBuffer.cpp ${PROJECT_SOURCE_DIR}/src/buffer/VertexBufferLayout.h ${PROJECT_SOURCE_DIR}/src/buffer/VertexBufferLayout.cpp ${PROJECT_SOURCE_DIR}/src/buffer/IndexBuffer.h ${PROJECT_SOURCE_DIR}/src/buffer/IndexBuffer.cpp ${PROJECT_SOURCE_DIR}/src/buffer/VertexArray.h ${PROJECT_SOURCE_DIR}/src/buffer/VertexArray.cpp
        ${PROJECT_SOURCE_DIR}/src/shader/Shader.h ${PROJECT_SOURCE_DIR}/src/shader/Shader.cpp

        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_glfw.h ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_glfw.cpp ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.h ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.cpp
    )

set_target_properties(graphics-engine PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/src"
    )

target_link_libraries(graphics-engine ${CONAN_LIBS})
