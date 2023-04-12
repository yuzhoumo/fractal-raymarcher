#include <iostream>
#include <glad/glad.h>

#include "window_callbacks.h"

namespace window_callbacks {

void error(int error, const char *description) {
  std::cout << description << std::endl;
}

void key(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebufferSize(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

} /* namespace window_callbacks */
