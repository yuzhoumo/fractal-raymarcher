#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "include/display.hpp"
#include "include/callbacks.hpp"
#include "include/constants.hpp"
#include "include/shader.hpp"

/* initialize GLAD and GLFW, then create window object */
void createGLcontexts(GLFWwindow*& window) {
  if (!glfwInit()) {
    std::cerr << "ERROR::GLFW_INITIALIZATION_FAILURE" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT,
                            WINDOW_TITLE, nullptr, nullptr);
  if (nullptr == window) {
    std::cerr << "ERROR::GLFW_WINDOW_CREATION_FAILURE" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "ERROR::GLAD_INITIALIZATION_FAILURE" << std::endl;
    exit(EXIT_FAILURE);
  }

  glViewport(0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
}

/* clean up objects and exit */
void cleanup(GLFWwindow*& window, int exit_code) {
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

int main(const int argc, const char* argv[]) {
  if (3 != argc) {
    std::cerr << "ERROR::INCORRECT_ARG_COUNT: " << argc << ", expected 3."
      << std::endl;
    exit(EXIT_FAILURE);
  }

  /* setup window and callbacks */
  GLFWwindow* window;
  Display& display = Display::getInstance();
  display.camera = Camera{};
  display.updateProjectionMatrix();

  glfwSetErrorCallback(callbacks::error);
  createGLcontexts(window);
  glfwSetFramebufferSizeCallback(window, callbacks::framebufferSize);
  glfwSetKeyCallback(window, callbacks::key);
  glfwSetCursorPosCallback(window, callbacks::cursorPos);
  glfwSetScrollCallback(window, callbacks::scroll);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glEnable(GL_DEPTH_TEST); /* enable z-buffer */

  /* compile shaders */
  std::filesystem::path vert_shader_path(argv[1]);
  if (!vert_shader_path.is_absolute())
    vert_shader_path = std::filesystem::absolute(vert_shader_path);

  std::filesystem::path frag_shader_path(argv[2]);
  if (!frag_shader_path.is_absolute())
    frag_shader_path = std::filesystem::absolute(frag_shader_path);

  const Shader shader = Shader{vert_shader_path.string(),
                               frag_shader_path.string()};
  if (0 == shader.getId()) {
    std::cerr << "ERROR::SHADER_CREATION_FAILURE" << std::endl;
    cleanup(window, EXIT_FAILURE);
  }

  // <<< TEST CODE: set up cubs in vertex buffer
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // <<< TEST CODE

  /* main render loop */
  while(!glfwWindowShouldClose(window)) {
    Display::updateTime(glfwGetTime());
    Display::updateCamera(window);

    /* clear frame buffer */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* activate shader and set matrices */
    shader.use();
    shader.setMat4("projection", display.projection_matrix);
    shader.setMat4("view", display.camera.getViewMatrix());

    // >>> TEST CODE 
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
      // calculate model matrix for each
      glm::mat4 model = glm::mat4(1.0f); // identity matrix
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    float green_value = (sin(display.time) / 2.0f) + 0.5f;
    shader.setFloat4("ourColor", glm::vec4(0.0f, green_value, 0.0f, 1.0f));
    shader.setFloat("xoffset", cos(display.time) / 2.0);
    shader.setFloat("yoffset", sin(display.time) / 2.0);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    // <<< TEST CODE

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cleanup(window, EXIT_SUCCESS);
}

