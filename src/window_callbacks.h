#include <GLFW/glfw3.h>

namespace window_callbacks {

/* handles window errors */
void error(int error, const char *description);

/* hanldes kayboard inputs */
void key(GLFWwindow *window, int key, int scancode, int action, int mods);

/* handles window resize */
void framebufferSize(GLFWwindow* window, int width, int height);

/* sets callback functions for the given window object */
void setGLFWcallbacks(GLFWwindow *window);

} /* namespace window_callbacks */
