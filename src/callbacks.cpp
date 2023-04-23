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
  /* close window with escape key */
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  /* toggle mouse input with M key */
  if (key == GLFW_KEY_M && action == GLFW_PRESS) {
    Display& display = Display::getInstance();
    glfwSetInputMode(window, GLFW_CURSOR, display.hide_cursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    display.hide_cursor = !display.hide_cursor;
    display.is_first_mouse_input = display.hide_cursor;
  }

  /* toggle supersampling with P key */
  if (key == GLFW_KEY_P && action == GLFW_PRESS) {
    Display& display = Display::getInstance();
    display.supersample = !display.supersample;
  }
}

void cursorPos(GLFWwindow* window, double xpos, double ypos) {
  float x = static_cast<float>(xpos);
  float y = static_cast<float>(ypos);

  Display& display = Display::getInstance();

  if (!display.hide_cursor) return;

  /* to prevent camera jerking on first mouse entry, set previous mouse
   * coordinates to the current mouse position */
  if (display.is_first_mouse_input) {
    display.is_first_mouse_input = false;
    display.updateMousePos(x, y);
  }

  /* dy reversed since mouse y=0 is at top of window */
  float dx = x - display.prev_mouse_x;
  float dy = display.prev_mouse_y - y;

  display.updateMousePos(x, y);
  display.camera.updateView(dx, dy);
}

void scroll(GLFWwindow* window, double xoffset, double yoffset) {
  Display& display = Display::getInstance();
  display.camera.updateFOV(yoffset);
  display.updateProjectionMatrix();
}

void framebufferSize(GLFWwindow* window, int width, int height) {
  Display& display = Display::getInstance();
  display.updateWindowSize(width, height);
  display.updateProjectionMatrix();
  glViewport(0, 0, width, height);
}

} /* namespace callbacks */
