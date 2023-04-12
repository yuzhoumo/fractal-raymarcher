#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window_callbacks.h"
#include "constants.h"

/* initialize GLAD and GLFW, then create window object */
void createGLcontexts(GLFWwindow **window) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
    /* set compatability mode for macOS */
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE,
                               NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(*window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

/* called in main render loop */
void render(GLFWwindow *window) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_ACCUM_BUFFER_BIT);
}

/* clean up objects and exit */
void cleanup(GLFWwindow *window) {
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

int main() {
  GLFWwindow *window;
  createGLcontexts(&window);
  window_callbacks::setGLFWcallbacks(window);

  /* main render loop */
  while(!glfwWindowShouldClose(window)) {
    render(window);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cleanup(window);
}
