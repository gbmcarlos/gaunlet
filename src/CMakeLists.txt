add_library(graphics-engine
        ${PROJECT_SOURCE_DIR}/src/window/Window.cpp
        ${PROJECT_SOURCE_DIR}/src/render/opengl/OpenGLRenderAPI.cpp
        ${PROJECT_SOURCE_DIR}/src/render/imgui/ImGuiRenderApi.cpp
        ${PROJECT_SOURCE_DIR}/src/render/RenderCommand.cpp ${PROJECT_SOURCE_DIR}/src/render/Renderer.cpp
        ${PROJECT_SOURCE_DIR}/src/run-loop/RunLoop.cpp
        ${PROJECT_SOURCE_DIR}/src/input/EventBus.cpp
        ${PROJECT_SOURCE_DIR}/src/buffer/VertexBuffer.cpp ${PROJECT_SOURCE_DIR}/src/buffer/BufferLayout.cpp ${PROJECT_SOURCE_DIR}/src/buffer/IndexBuffer.cpp ${PROJECT_SOURCE_DIR}/src/buffer/VertexArray.cpp
        ${PROJECT_SOURCE_DIR}/src/shader/Shader.cpp
        ${PROJECT_SOURCE_DIR}/src/camera/OrthographicCamera.cpp
        ${PROJECT_SOURCE_DIR}/src/mesh/2d/samples/TriangleMesh.cpp ${PROJECT_SOURCE_DIR}/src/mesh/2d/samples/SquareMesh.cpp

        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_glfw.cpp ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.cpp
    )

set_target_properties(graphics-engine PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/src"
    )

target_link_libraries(graphics-engine ${CONAN_LIBS})
