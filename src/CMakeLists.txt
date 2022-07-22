# Create the target and set properties
add_library(graphics-engine
#        Core
        core/input/EventBus.cpp
        core/window/Window.cpp
        core/application/Layer.cpp
        core/application/LayerStack.cpp
        core/application/Application.cpp
        core/render/RenderApi.cpp
        core/render/opengl/OpenGLRenderAPI.cpp
        core/render/RenderCommand.cpp
        core/imgui/ImGuiRenderApi.cpp
        core/run-loop/RunLoop.cpp

#        Graphics
        graphics/renderer/Renderer.cpp
        graphics/buffer/VertexBuffer.cpp graphics/buffer/BufferLayout.cpp graphics/buffer/IndexBuffer.cpp graphics/buffer/VertexArray.cpp
        graphics/shader/Shader.cpp
        graphics/shader/ShaderLibrary.cpp
        graphics/camera/OrthographicCamera.cpp
        graphics/texture/Texture.cpp

#        Scene
        graphics/scene/Scene.cpp

        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_glfw.cpp ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.cpp
    )

add_library(GraphicsEngine::graphics-engine ALIAS graphics-engine)

target_include_directories(graphics-engine
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Link the dependencies
target_link_libraries(graphics-engine PRIVATE ${CONAN_LIBS})

target_compile_definitions(graphics-engine
        PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

# Installation instructions
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/graphics-engine)

install(TARGETS graphics-engine
        EXPORT graphics-engine-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

#This is required so that the exported target has the name JSONUtils and not jsonutils
set_target_properties(graphics-engine PROPERTIES EXPORT_NAME GraphicsEngine)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT graphics-engine-targets
        FILE GraphicsEngineConfig.cmake
        NAMESPACE GraphicsEngine::
        DESTINATION ${INSTALL_CONFIGDIR}
)
