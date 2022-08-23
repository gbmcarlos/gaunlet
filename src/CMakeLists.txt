# Create the target and set properties
add_library(graphics-engine
#        Core
        core/event/EventBus.cpp
        core/window/Window.cpp
        core/application/Layer.cpp
        core/application/LayerStack.cpp
        core/application/Application.cpp core/application/Application.h
        core/render/RenderApi.cpp
        core/render/opengl/OpenGLRenderAPI.cpp
        core/render/RenderCommand.cpp
        core/imgui/ImGuiRenderApi.cpp
        core/run-loop/RunLoop.cpp

#        Graphics
        graphics/vertex-buffer/BufferLayout.cpp
        graphics/vertex-buffer/VertexBuffer.cpp
        graphics/index-buffer/IndexBuffer.cpp
        graphics/vertex-array/VertexArray.cpp
        graphics/uniform-buffer/UniformBuffer.cpp
        graphics/shader/Shader.cpp
        graphics/shader/ShaderLibrary.cpp
        graphics/texture/TextureImage2D.cpp
        graphics/texture/TextureLibrary.cpp
        graphics/framebuffer/Framebuffer.cpp

#        Scene
        scene/model/Model.cpp
        scene/forward-renderer/ForwardRenderer.cpp
        scene/deferred-renderer/DeferredRenderer.cpp
        scene/camera/Camera.cpp
        scene/camera/OrthographicCamera.cpp
        scene/camera/PerspectiveCamera.cpp
        scene/physics/PhysicsWorld.cpp
        scene/Scene.cpp

#        Layout
        layout/docking/DockingLayout.cpp
        layout/application/LayoutLayer.cpp

#        Dear ImGui
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_glfw.cpp ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.cpp
)

target_precompile_headers(
        graphics-engine
        PRIVATE pch.h
)

add_library(GraphicsEngine::graphics-engine ALIAS graphics-engine)

target_include_directories(graphics-engine
        PUBLIC
            $<INSTALL_INTERFACE:include>
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_include_directories(graphics-engine PUBLIC ../include)

# Link the dependencies
target_link_libraries(graphics-engine PRIVATE ${CONAN_LIBS})

target_compile_definitions(graphics-engine
        PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

#install(TARGETS graphics-engine DESTINATION ${CMAKE_INSTALL_LIBDIR})
#install(FILES include/graphics-engine/Core.h DESTINATION include)