project(graphics-engine)
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 14)

add_library(graphics-engine ${PROJECT_SOURCE_DIR}/src/application/Application.h)
set_target_properties(graphics-engine PROPERTIES LINKER_LANGUAGE CXX)

#add_subdirectory(examples/empty-screen)

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()
#target_link_libraries(graphics-engine ${CONAN_LIBS})

#include_directories(vendor)

#set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE INTERNAL "" FORCE)
