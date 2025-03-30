cmake_minimum_required(VERSION 3.10)

project(gina VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE)

set(ENGINE_HEADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/public)
set(ENGINE_SOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/private)

file(GLOB_RECURSE ENGINE_HEADERS ${ENGINE_HEADERS_PATH}/*.h)
file(GLOB_RECURSE ENGINE_SOURCES ${ENGINE_SOURCES_PATH}/*.cpp)

add_library(${PROJECT_NAME} STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})

target_link_libraries(
    ${PROJECT_NAME} PUBLIC  
    imgui  
    assimp  
    dxtex  
    d3d12  
    dxgi  
    dxguid  
    d3dcompiler  
    ${EXTERNALS_PATH}/pix/bin/WinPixEventRuntime.lib  
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC  
    ${ENGINE_HEADERS_PATH}  
    ${EXTERNALS_PATH}/imgui/  
    ${EXTERNALS_PATH}/json/  
    ${EXTERNALS_PATH}/dxtex/DirectXTex/  
    ${EXTERNALS_PATH}/pix/include/WinPixEventRuntime/
    ${EXTERNALS_PATH}/dx/  
)