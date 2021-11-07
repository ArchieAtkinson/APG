cmake_minimum_required(VERSION 3.13...3.16 FATAL_ERROR)

set(CMAKE_GENERATOR Ninja)

project(APG VERSION 0.0.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_options(${PROJECT_NAME} PRIVATE --static)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-DDEBUG>)

target_include_directories(${PROJECT_NAME} PRIVATE inc)

target_include_directories(${PROJECT_NAME} PRIVATE ./external/argparse/include/argparse)

configure_file(${CMAKE_SOURCE_DIR}/configuration/root_dir.h.in ${CMAKE_SOURCE_DIR}/inc/root_dir.h)

add_subdirectory(external/raylib)
target_link_libraries(${PROJECT_NAME} raylib)