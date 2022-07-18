add_library(graphics-engine
#        Core
        core/input/EventBus.cpp
        core/window/Window.cpp
        core/render/RenderApi.cpp
        core/render/opengl/OpenGLRenderAPI.cpp
        core/render/RenderCommand.cpp
        core/imgui/ImGuiRenderApi.cpp
        core/run-loop/RunLoop.cpp

#        Graphics
        graphics/renderer/Renderer.cpp
        graphics/buffer/VertexBuffer.cpp graphics/buffer/BufferLayout.cpp graphics/buffer/IndexBuffer.cpp graphics/buffer/VertexArray.cpp
        graphics/shader/Shader.cpp
        graphics/camera/OrthographicCamera.cpp

        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_glfw.cpp ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.cpp
    )

set_target_properties(graphics-engine PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/src"
    )

target_link_libraries(graphics-engine ${CONAN_LIBS})
