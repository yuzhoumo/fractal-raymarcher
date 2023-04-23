#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "include/display.hpp"
#include "include/callbacks.hpp"
#include "include/constants.hpp"
#include "include/shader.hpp"

/* initialize GLAD and GLFW, then create window object and set callbacks */
void createGLcontexts(GLFWwindow*& window) {
  if (!glfwInit()) {
    std::cerr << "ERROR::GLFW_INITIALIZATION_FAILURE" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* set error callback before window creation */ 
  glfwSetErrorCallback(callbacks::error);

  window = glfwCreateWindow(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT,
                            WINDOW_TITLE, nullptr, nullptr);
  if (nullptr == window) {
    std::cerr << "ERROR::GLFW_WINDOW_CREATION_FAILURE" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, callbacks::framebufferSize);
  glfwSetKeyCallback(window, callbacks::key);
  glfwSetCursorPosCallback(window, callbacks::cursorPos);
  glfwSetScrollCallback(window, callbacks::scroll);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "ERROR::GLAD_INITIALIZATION_FAILURE" << std::endl;
    exit(EXIT_FAILURE);
  }

  glViewport(0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
  glEnable(GL_DEPTH_TEST); /* enable z-buffer */
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

  /* populate display singleton before glfw window creation */
  Display& display = Display::getInstance();
  display.camera = Camera{};
  display.updateProjectionMatrix();

  GLFWwindow* window;
  createGLcontexts(window);

  /* compile shaders */
  std::filesystem::path vert_shader_path(argv[1]);
  if (!vert_shader_path.is_absolute())
    vert_shader_path = std::filesystem::absolute(vert_shader_path);

  std::filesystem::path frag_shader_path(argv[2]);
  if (!frag_shader_path.is_absolute())
    frag_shader_path = std::filesystem::absolute(frag_shader_path);

  Shader shader = Shader{vert_shader_path.string(),
                         frag_shader_path.string()};
  if (0 == shader.getId()) {
    std::cerr << "ERROR::SHADER_CREATION_FAILURE" << std::endl;
    cleanup(window, EXIT_FAILURE);
  }

  /* load fullscreen quad */

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(FS_QUAD), FS_QUAD, GL_STATIC_DRAW);

  glVertexAttribPointer(FS_QUAD_VERTEX_ATTRIB_PARAMS);
  glEnableVertexAttribArray(FS_QUAD_VERTEX_ATTRIB_INDEX);

  /* main render loop */
  while(!glfwWindowShouldClose(window)) {
    Display::updateTime(glfwGetTime());
    Display::updateCamera(window);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
      shader = Shader{vert_shader_path.string(),
                      frag_shader_path.string()};

    /* clear frame buffer */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* activate shader and set uniforms */
    shader.use();
    shader.setBool("u_supersample", display.supersample);
    shader.setFloat3("u_camera_pos", display.camera.getPosition());
    shader.setFloat3("u_camera_front", display.camera.getFront());
    shader.setFloat3("u_camera_up", display.camera.getUp());
    shader.setFloat3("u_camera_right", display.camera.getRight());
    shader.setFloat("u_fov", display.camera.getFOV());
    shader.setFloat("u_time", display.time);
    shader.setFloat2("u_resolution", glm::vec2(display.window_width,
                                               display.window_height));
    /* draw frame */
    glBindVertexArray(VAO);
    glDrawArrays(FS_QUAD_DRAW_ARRAY_PARAMS);
    glfwSwapBuffers(window);

    /* poll for window events */
    glfwPollEvents();
  }

  cleanup(window, EXIT_SUCCESS);
}
