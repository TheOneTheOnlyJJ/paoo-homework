cmake_minimum_required(VERSION 3.2)

project(paoo_homework)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inc)
