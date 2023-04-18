#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "include/callbacks.hpp"
#include "include/display.hpp"

namespace callbacks {

void error(int errorcode, const char *description) {
  std::cerr << description << std::endl;
}

void key(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursorPos(GLFWwindow* window, double xpos, double ypos) {
  Display& display = Display::getInstance();

  float x = static_cast<float>(xpos);
  float y = static_cast<float>(ypos);

  /* to prevent camera view jerk on first mouse entry, set previous mouse
   * coordinates to the current mouse position */
  if (display.is_first_mouse_input) {
    display.is_first_mouse_input = false;
    display.prev_mouse_x = x;
    display.prev_mouse_y = y;
  }

  /* dy reversed since y is bottom-to-top */
  float dx = x - display.prev_mouse_x;
  float dy = display.prev_mouse_y - y;

  display.prev_mouse_x = x;
  display.prev_mouse_y = y;

  display.camera.updateView(dx, dy);
}

void scroll(GLFWwindow* window, double xoffset, double yoffset) {
  Display& display = Display::getInstance();
  display.camera.updateFOV(yoffset);
  display.updateProjectionMatrix();
}

void framebufferSize(GLFWwindow* window, int width, int height) {
  Display& display = Display::getInstance();
  glViewport(0, 0, width, height);
  display.window_width = width;
  display.window_height = height;
  display.updateProjectionMatrix();
}

} /* namespace callbacks */
