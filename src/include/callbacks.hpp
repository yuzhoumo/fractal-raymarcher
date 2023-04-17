#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include "camera.hpp"
#include <GLFW/glfw3.h>

namespace callbacks {

void error(int errorcode, const char *description);
void key(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorPos(GLFWwindow* window, double xpos, double ypos);
void scroll(GLFWwindow* window, double xoffset, double yoffset);
void framebufferSize(GLFWwindow* window, int width, int height);

} /* namespace callbacks */

#endif /* ifndef CALLBACKS_HPP */
