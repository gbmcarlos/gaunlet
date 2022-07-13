add_library(graphics-engine
        ${PROJECT_SOURCE_DIR}/src/application/Application.h
        ${PROJECT_SOURCE_DIR}/src/window/Window.cpp ${PROJECT_SOURCE_DIR}/src/window/Window.h
    )

set_target_properties(graphics-engine PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/src"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/src"
    )

target_link_libraries(graphics-engine ${CONAN_LIBS})

include_directories(vendor)