# Create the target and set properties
add_library(gaunlet STATIC
#        Core
        gaunlet/core/event/EventBus.cpp
        gaunlet/core/window/Window.cpp
        gaunlet/core/application/Layer.cpp
        gaunlet/core/application/LayerStack.cpp
        gaunlet/core/application/Application.cpp
        gaunlet/core/render/RenderApi.cpp
        gaunlet/core/render/opengl/OpenGLRenderAPI.cpp
        gaunlet/core/render/RenderCommand.cpp
        gaunlet/core/imgui/ImGuiRenderApi.cpp
        gaunlet/core/run-loop/RunLoop.cpp

#        Graphics
        gaunlet/graphics/vertex-buffer/BufferLayout.cpp
        gaunlet/graphics/vertex-buffer/VertexBuffer.cpp
        gaunlet/graphics/index-buffer/IndexBuffer.cpp
        gaunlet/graphics/vertex-array/VertexArray.cpp
        gaunlet/graphics/uniform-buffer/UniformBuffer.cpp
        gaunlet/graphics/shader/Shader.cpp
        gaunlet/graphics/shader/ShaderLibrary.cpp
        gaunlet/graphics/texture/TextureImage2D.cpp
        gaunlet/graphics/texture/TextureLibrary.cpp
        gaunlet/graphics/framebuffer/Framebuffer.cpp

#        Scene
        gaunlet/scene/model/Model.cpp
        gaunlet/scene/deferred-renderer/DeferredRenderer.cpp
        gaunlet/scene/camera/Camera.cpp
        gaunlet/scene/camera/OrthographicCamera.cpp
        gaunlet/scene/camera/PerspectiveCamera.cpp
        gaunlet/scene/physics/PhysicsWorld.cpp
        gaunlet/scene/Scene.cpp

#        Layout
        gaunlet/layout/docking/DockingLayout.cpp
        gaunlet/layout/layout-layer/LayoutLayer.cpp
        gaunlet/layout/panel/RenderPanel.cpp

#        Dear ImGui
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_glfw.cpp ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.cpp
)

# Link the dependencies
target_link_directories(gaunlet PUBLIC ${CONAN_LIB_DIRS}) # Tell the compiler where to find the libraries
target_link_libraries(gaunlet PUBLIC ${CONAN_LIBS}) # Tell the compiler to link the libraries
target_include_directories(gaunlet PUBLIC ${CONAN_INCLUDE_DIRS}) # Tell the compiler where to find the includes (header files)

target_precompile_headers(gaunlet
        PRIVATE gaunlet/pch.h
)

target_compile_definitions(gaunlet
        PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/gaunlet/assets"
)

target_include_directories(gaunlet
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# INSTALLATION

# Install the library
install(
        TARGETS gaunlet # copy the library itself
        EXPORT gaunlet # add to an export group
        LIBRARY
            DESTINATION lib
)

# Install the target (so it can be imported and linked by other cmake projects
install(
        EXPORT gaunlet
        NAMESPACE gaunlet::
        FILE gaunlet-config.cmake
        DESTINATION lib/cmake/gaunlet
)

# Install all the public headers (exclude the opengl implementation)
install(
        DIRECTORY gaunlet/ DESTINATION include/gaunlet
        FILES_MATCHING PATTERN "*.h"
        PATTERN "opengl" EXCLUDE
)

# Install the assets
install(
        DIRECTORY gaunlet/assets DESTINATION include/gaunlet
        DIRECTORY_PERMISSIONS
            OWNER_READ OWNER_EXECUTE OWNER_WRITE
            GROUP_READ GROUP_EXECUTE GROUP_WRITE
            WORLD_READ WORLD_EXECUTE
)

# Install the composite headers
install(
        DIRECTORY ../include/ DESTINATION include/gaunlet
        FILES_MATCHING PATTERN "*.h"
)