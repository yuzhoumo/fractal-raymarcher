#include <iostream>
#include <fstream>
#include <sstream>

#include "loader.h"

GLuint Loader::createShaderProgram(const std::string& vertexShaderPath,
                                   const std::string& fragmentShaderPath) {
  /* compile shaders */
  GLuint vertexShader, fragmentShader;
  vertexShader = compileShader(vertexShaderPath, LOADER_VERT_SHADER);

  if (0 == vertexShader) {
    std::cerr << "Failed to compile vertex shader" << std::endl;
    return 0;
  }

  fragmentShader = compileShader(fragmentShaderPath, LOADER_FRAG_SHADER);
  if (0 == fragmentShader) {
    std::cerr << "Failed to compile fragment shader" << std::endl;
    return 0;
  }

  /* create shader program and link fragment shader */
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  if (-1 == checkCompileErrors(shaderProgram, LOADER_PROGRAM))
    return 0;

  /* clean up shader objects */
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

GLuint Loader::compileShader(const std::string& shaderPath,
                             const std::string& shaderType) {
  /* create shader */
  GLuint shader;
  if (LOADER_FRAG_SHADER == shaderType) {
    shader = glCreateShader(GL_FRAGMENT_SHADER);
  } else if (LOADER_VERT_SHADER == shaderType) {
    shader = glCreateShader(GL_VERTEX_SHADER);
  } else {
    std::cerr << "Invalid shader type: " << shaderType << std::endl;
    return 0;
  }

  /* read source code from path */
  std::stringstream stream;
  std::ifstream shaderFile(shaderPath, std::ios::in);
  if (shaderFile.is_open()) {
    stream << shaderFile.rdbuf();
    shaderFile.close();
  } else {
    std::cerr << "Failed to open fragment shader file: " <<
      shaderPath << std::endl;
    return 0;
  }

  const std::string code = stream.str();

  /* compile fragment shader */
  const char* codeCstr = code.c_str();
  glShaderSource(shader, 1, &codeCstr, NULL);
  glCompileShader(shader);
  if (-1 == checkCompileErrors(shader, LOADER_FRAG_SHADER)) 
    return 0;

  return shader;
}

int Loader::checkCompileErrors(GLuint shader, const std::string& type) {
  GLint success; GLchar infoLog[1024];
  bool isProgram = (type == "PROGRAM");
  std::string errMsg;

  if (isProgram) {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    errMsg = "ERROR::SHADER::PROGRAM::LINKING_FAILED";
  } else {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    errMsg = "ERROR::SHADER::FRAGMENT";
  }

  if (success) return 0;

  glGetProgramInfoLog(shader, 1024, NULL, infoLog);
  std::cerr << infoLog << std::endl;
  return -1;
}
