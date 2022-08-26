# Create the target and set properties
add_library(graphics-engine STATIC
#        Core
        graphics-engine/core/event/EventBus.cpp
        graphics-engine/core/window/Window.cpp
        graphics-engine/core/application/Layer.cpp
        graphics-engine/core/application/LayerStack.cpp
        graphics-engine/core/application/Application.cpp
        graphics-engine/core/render/RenderApi.cpp
        graphics-engine/core/render/opengl/OpenGLRenderAPI.cpp
        graphics-engine/core/render/RenderCommand.cpp
        graphics-engine/core/imgui/ImGuiRenderApi.cpp
        graphics-engine/core/run-loop/RunLoop.cpp

#        Graphics
        graphics-engine/graphics/vertex-buffer/BufferLayout.cpp
        graphics-engine/graphics/vertex-buffer/VertexBuffer.cpp
        graphics-engine/graphics/index-buffer/IndexBuffer.cpp
        graphics-engine/graphics/vertex-array/VertexArray.cpp
        graphics-engine/graphics/uniform-buffer/UniformBuffer.cpp
        graphics-engine/graphics/shader/Shader.cpp
        graphics-engine/graphics/shader/ShaderLibrary.cpp
        graphics-engine/graphics/texture/TextureImage2D.cpp
        graphics-engine/graphics/texture/TextureLibrary.cpp
        graphics-engine/graphics/framebuffer/Framebuffer.cpp
        graphics-engine/graphics/forward-renderer/ForwardRenderer.cpp

#        Scene
        graphics-engine/scene/model/Model.cpp
        graphics-engine/scene/deferred-renderer/DeferredRenderer.cpp
        graphics-engine/scene/camera/Camera.cpp
        graphics-engine/scene/camera/OrthographicCamera.cpp
        graphics-engine/scene/camera/PerspectiveCamera.cpp
        graphics-engine/scene/physics/PhysicsWorld.cpp
        graphics-engine/scene/Scene.cpp

#        Layout
        graphics-engine/layout/docking/DockingLayout.cpp
        graphics-engine/layout/application/LayoutLayer.cpp

#        Dear ImGui
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_glfw.cpp ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.cpp
)

# Link the dependencies
target_link_directories(graphics-engine PUBLIC ${CONAN_LIB_DIRS}) # Tell the compiler where to find the libraries
target_link_libraries(graphics-engine PUBLIC ${CONAN_LIBS}) # Tell the compiler to link the libraries
target_include_directories(graphics-engine PUBLIC ${CONAN_INCLUDE_DIRS}) # Tell the compiler where to find the includes (header files)

target_precompile_headers(
        graphics-engine
        PRIVATE graphics-engine/pch.h
)

target_compile_definitions(graphics-engine
        PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/graphics-engine/assets"
)

target_include_directories(graphics-engine
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# INSTALLATION

# Install the library
install(
        TARGETS graphics-engine # copy the library itself
        EXPORT graphics-engine # add to an export group
        LIBRARY
            DESTINATION lib
)

# Install the target (so it can be imported and linked by other cmake projects
install(
        EXPORT graphics-engine
        NAMESPACE graphics-engine::
        FILE graphics-engine-config.cmake
        DESTINATION lib/cmake/graphics-engine
)

# Install all the public headers (exclude the opengl implementation)
install(
        DIRECTORY graphics-engine/ DESTINATION include/graphics-engine
        FILES_MATCHING PATTERN "*.h"
        PATTERN "opengl" EXCLUDE
)

# Install the assets
install(
        DIRECTORY graphics-engine/assets DESTINATION include/graphics-engine
        DIRECTORY_PERMISSIONS
            OWNER_READ OWNER_EXECUTE OWNER_WRITE
            GROUP_READ GROUP_EXECUTE GROUP_WRITE
            WORLD_READ WORLD_EXECUTE
)

# Install the composite headers
install(
        DIRECTORY ../include/ DESTINATION include/graphics-engine
        FILES_MATCHING PATTERN "*.h"
)