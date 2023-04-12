#include <GLFW/glfw3.h>

namespace window_callbacks {

void error(int error, const char *description);

void key(GLFWwindow *window, int key, int scancode, int action, int mods);

void framebufferSize(GLFWwindow* window, int width, int height);

void setGLFWcallbacks(GLFWwindow *window);

} /* namespace window_callbacks */
