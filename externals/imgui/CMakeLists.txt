cmake_minimum_required(VERSION 3.10)
project(imgui LANGUAGES CXX)

set(IMGUI_SOURCES
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_impl_win32.cpp
    imgui_impl_dx12.cpp
)

set(IMGUI_HEADERS
    imconfig.h
    imgui.h
    imgui_internal.h
    imgui_impl_win32.h
    imgui_impl_dx12.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

target_include_directories(imgui PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)