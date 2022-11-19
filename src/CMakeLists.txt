# Create the target and set properties
add_library(gaunlet STATIC
#        Core
        gaunlet/core/event/EventBus.cpp
        gaunlet/core/window/Window.cpp
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
        gaunlet/graphics/texture/Image.cpp
        gaunlet/graphics/texture/TextureImage2D.cpp
        gaunlet/graphics/texture/TextureCubeMap.cpp
        gaunlet/graphics/texture/TextureLibrary.cpp
        gaunlet/graphics/framebuffer/Framebuffer.cpp
        gaunlet/graphics/render-pass/SimplerRenderPass.cpp

#        Scene
        gaunlet/scene/model/Model.cpp
        gaunlet/scene/camera/Camera.cpp
        gaunlet/scene/camera/OrthographicCamera.cpp
        gaunlet/scene/camera/PerspectiveCamera.cpp
        gaunlet/scene/entity/Entity.cpp
        gaunlet/scene/ray-casting/RayCasting.cpp

#        Layout
        gaunlet/editor/docking/DockingLayout.cpp
        gaunlet/editor/workspace/Workspace.cpp
        gaunlet/editor/panel/RenderPanel.cpp
        gaunlet/editor/gizmos/TranslationGizmo.cpp

#        Prefab
        gaunlet/prefab/render-pipelines/basic-3d-render-pipeline/Basic3DRenderPipeline.cpp
        gaunlet/prefab/render-pipelines/basic-editor-render-pipeline/BasicEditorRenderPipeline.cpp
        gaunlet/prefab/editor-tools/first-person-camera-controller/FirstPersonCameraController.cpp

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
        PUBLIC
            GL_PREFABS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/gaunlet/prefab"
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
        FILES_MATCHING
            PATTERN "*.h"
            PATTERN "*.glsl"
            PATTERN "*.obj"
        PATTERN "opengl" EXCLUDE
)

# Install the composite headers
install(
        DIRECTORY ../include/ DESTINATION include/gaunlet
        FILES_MATCHING PATTERN "*.h"
)