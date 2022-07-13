add_executable(example-empty-screen ${PROJECT_SOURCE_DIR}/examples/empty-screen/main.cpp)
set_target_properties(example-empty-screen PROPERTIES LINKER_LANGUAGE CXX)
include_directories(${PROJECT_SOURCE_DIR}/src)