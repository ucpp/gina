cmake_minimum_required(VERSION 3.10)
project(gina_animation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(EXTERNALS_PATH ${CMAKE_SOURCE_DIR}/externals)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DEMO  "Build demo"  ON)

add_subdirectory(${EXTERNALS_PATH}/imgui)
add_subdirectory(${EXTERNALS_PATH}/assimp)

add_subdirectory(engine)

if(BUILD_DEMO)
    add_subdirectory(demo)
endif()

if(BUILD_TESTS)

endif()
