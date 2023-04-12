cmake_minimum_required(VERSION 3.20)
project(fmarch VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# glad
add_subdirectory(${VENDOR_DIR}/glad)
set(GLAD_INCLUDE_DIRS ${VENDOR_DIR}/glad/include)

# glfw3
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)

add_subdirectory(${VENDOR_DIR}/glfw)
set(GLFW_INCLUDE_DIRS ${VENDOR_DIR}/glfw/include/)

# opengl
find_package(OpenGL REQUIRED)

# learnopengl
file(GLOB_RECURSE HEADER_FILES ${SRC_DIR}/*.h ${SRC_DIR}/*.hpp)
file(GLOB_RECURSE SOURCE_FILES ${SRC_DIR}/*.c ${SRC_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

include_directories(
  ${GLAD_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
  glad
  glfw
)
