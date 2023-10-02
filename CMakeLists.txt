project(gaunlet)
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE INTERNAL "" FORCE)

include(vendor/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${PROJECT_SOURCE_DIR}/vendor)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/examples/1-empty-screen)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/2-render-triangle)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/3-rendering-2d)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/4-rendering-3d)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/5-viewport-layout)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/6-basic-editor)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/7-sphere-rendering)
