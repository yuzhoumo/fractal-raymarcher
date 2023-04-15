#include <cstdlib>
#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "include/window_callbacks.hpp"
#include "include/constants.hpp"
#include "include/shader.hpp"

/* initialize GLAD and GLFW, then create window object */
void createGLcontexts(GLFWwindow*& window) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE,
                               nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

/* clean up objects and exit */
void cleanup(GLFWwindow*& window, int exit_code) {
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

int main() {
  GLFWwindow* window;
  glfwSetErrorCallback(window_callbacks::error);

  createGLcontexts(window);
  glfwSetFramebufferSizeCallback(window, window_callbacks::framebufferSize);
  glfwSetKeyCallback(window, window_callbacks::key);

  /* compile shaders */
  const Shader shader = Shader{"../shaders/default.vert",
                               "../shaders/raymarch.frag"};
  if (0 == shader.getId()) {
    std::cerr << "ERROR::SHADER_CREATION_FAILURE" << std::endl;
    cleanup(window, EXIT_FAILURE);
  }

  // FIXME: test code
  float tri[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
  };

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // FIXME

  /* main render loop */
  while(!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // FIXME: test code
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shader.getId(), "ourColor");

    shader.use();
    shader.setFloat("xoffset", cos(timeValue) / 2.0);
    shader.setFloat("yoffset", sin(timeValue) / 2.0);
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    // FIXME

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cleanup(window, EXIT_SUCCESS);
}
