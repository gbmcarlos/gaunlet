project(graphics-engine)
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE INTERNAL "" FORCE)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
include_directories(${PROJECT_SOURCE_DIR}/vendor)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/examples/1-empty-screen)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/2-render-triangle)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/3-basic-rendering)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/4-playable-quad)
