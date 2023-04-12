cmake_minimum_required(VERSION 3.0)
project(fmarch VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(GLAD_INCLUDE_DIRS ${VENDOR_DIR}/glad/include/)
set(GLFW_INCLUDE_DIRS ${VENDOR_DIR}/glfw/include/)
set(GLM_INCLUDE_DIRS ${VENDOR_DIR}/glm/)

# require opengl
find_package(OpenGL REQUIRED)

# check if nix-store is available
find_program(NIX_STORE_EXECUTABLE NAMES nix-store)

if(NIX_STORE_EXECUTABLE)
  add_subdirectory(${VENDOR_DIR}/glad/)
  set(ENV{LD_LIBRARY_PATH}
    "/run/opengl-driver/lib:/run/opengl-driver-32/lib")

  # use nix-provided packages if building for nixos
  find_package(glfw3 REQUIRED)
  find_package(glm REQUIRED)
else()
  # only build glfw3 library
  option(GLFW_BUILD_DOCS OFF)
  option(GLFW_BUILD_EXAMPLES OFF)
  option(GLFW_BUILD_TESTS OFF)
  option(GLFW_INSTALL OFF)

  add_subdirectory(${VENDOR_DIR}/glad/)
  add_subdirectory(${VENDOR_DIR}/glfw/)
  add_subdirectory(${VENDOR_DIR}/glm/)

  include_directories(
    ${GLAD_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
  )
endif()

# project source
file(GLOB_RECURSE SOURCE_FILES ${SRC_DIR}/*.c ${SRC_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} glad glfw)
