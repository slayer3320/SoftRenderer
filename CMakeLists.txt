cmake_minimum_required(VERSION 3.10)

project(SoftRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
aux_source_directory(imgui UISOURCES)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${UISOURCES})

target_link_libraries(${PROJECT_NAME} d3d11 dxgi d3dcompiler dwmapi)