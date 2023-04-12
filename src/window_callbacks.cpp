#include <iostream>
#include <glad/glad.h>

#include "window_callbacks.h"

namespace window_callbacks {

void error(int error, const char *description) {
  fputs(description, stderr);
}

void key(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebufferSize(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void setGLFWcallbacks(GLFWwindow *window) {
  glfwSetErrorCallback(window_callbacks::error);
  glfwSetKeyCallback(window, window_callbacks::key);
  glfwSetFramebufferSizeCallback(window, window_callbacks::framebufferSize);
}

} /* namespace window_callbacks */
