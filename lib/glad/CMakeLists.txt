cmake_minimum_required(VERSION 3.22)
project(glad)

set(GLAD_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)

add_library(glad include/glad/glad.h src/glad.c)
include_directories(${GLAD_INCLUDE_DIRS})
target_include_directories(glad PUBLIC include/)
