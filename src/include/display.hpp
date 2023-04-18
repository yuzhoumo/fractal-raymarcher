#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <glm/gtc/matrix_transform.hpp>

#include "constants.hpp"
#include "camera.hpp"

#include <GLFW/glfw3.h>

/* Display is a singleton class used to store and update data required by glfw
 * callback functions. */

class Display {
public:
  static Display& getInstance() {
    static Display instance;
    return instance;
  }

  /* update camera based on current display values and user input */
  static inline void updateCamera(GLFWwindow* window) {
    Display& display = Display::getInstance();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      display.camera.updatePosition(FORWARD, display.delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      display.camera.updatePosition(BACKWARD, display.delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      display.camera.updatePosition(LEFT, display.delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      display.camera.updatePosition(RIGHT, display.delta_time);
  }

  /* update projection matrix using current display values */
  static inline void updateProjectionMatrix() {
    Display& display = Display::getInstance();
    display.projection_matrix = glm::perspective(
                  glm::radians(display.camera.getFOV()),
                  display.window_width / display.window_height, 0.1f, 100.0f);
  }

  /* update time values given the current time */
  static inline void updateTime(float curr_time) {
    Display& display = Display::getInstance();
    display.delta_time = curr_time - display.prev_time;
    display.prev_time = display.time;
    display.time = curr_time;
  }

  Display(const Display&) = delete;
  void operator=(const Display&) = delete;

  Camera camera;
  glm::mat4 projection_matrix;

  /* track height and width of the window */
  float window_width = WINDOW_DEFAULT_WIDTH;
  float window_height = WINDOW_DEFAULT_HEIGHT;

  /* track if it's the first mouse input */
  bool is_first_mouse_input = true;

  /* previous mouse coordinates, default to center of window */
  float prev_mouse_x = WINDOW_DEFAULT_WIDTH / 2.0f;
  float prev_mouse_y = WINDOW_DEFAULT_HEIGHT / 2.0f;

  float time = 0.0f;        /* time at the current frame */
  float prev_time = 0.0f;   /* time at the previous frame */
  float delta_time = 0.0f;  /* time between previous and current frame*/

private:
  Display() {}
};

#endif /* ifndef DISPLAY_HPP */
