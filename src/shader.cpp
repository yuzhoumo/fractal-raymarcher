#include <iostream>
#include <fstream>
#include <sstream>

#include "include/shader.hpp"

#define SHADER_TYPE_VERT_SHADER 0
#define SHADER_TYPE_FRAG_SHADER 1
#define SHADER_TYPE_PROGRAM 2

Shader::Shader(const std::string& vertexShaderPath,
               const std::string& fragmentShaderPath) {
  _id = buildProgram(vertexShaderPath, fragmentShaderPath);
}

void Shader::use() const {
  glUseProgram(_id);
}

unsigned int Shader::getId() const {
  return _id;
}

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(_id, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(_id, name.c_str()), value); 
}

GLuint Shader::buildProgram(const std::string& vertexShaderPath,
                            const std::string& fragmentShaderPath) {
  /* compile shaders */
  GLuint vertexShader, fragmentShader;
  vertexShader = compile(vertexShaderPath, SHADER_TYPE_VERT_SHADER);

  if (0 == vertexShader) {
    std::cerr << "ERROR::VERTEX_SHADER_COMPILATION_FAILURE" << std::endl;
    return 0;
  }

  fragmentShader = compile(fragmentShaderPath, SHADER_TYPE_FRAG_SHADER);
  if (0 == fragmentShader) {
    std::cerr << "ERROR::FRAGMENT_SHADER_COMPILATION_FAILURE" << std::endl;
    return 0;
  }

  /* create shader program and link fragment shader */
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  if (-1 == checkCompileErrors(shaderProgram, SHADER_TYPE_PROGRAM))
    return 0;

  /* clean up shader objects */
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

GLuint Shader::compile(const std::string& shaderPath,
                       const int shaderType) {
  /* create shader */
  GLuint shader;
  if (SHADER_TYPE_FRAG_SHADER == shaderType) {
    shader = glCreateShader(GL_FRAGMENT_SHADER);
  } else if (SHADER_TYPE_VERT_SHADER == shaderType) {
    shader = glCreateShader(GL_VERTEX_SHADER);
  } else {
    std::cerr << "ERROR::SHADER_TYPE_ERROR Invalid shader type: " <<
      shaderType << std::endl;
    return 0;
  }

  /* read shader source code from path */
  std::string shaderCode;
  std::ifstream shaderFile;
  shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

  try {
    std::stringstream stream;
    shaderFile.open(shaderPath);
    stream << shaderFile.rdbuf();
    shaderFile.close();
    shaderCode = stream.str();
  } catch(std::ifstream::failure e) {
    std::cerr << "ERROR::SHADER::FILE_READ_FAILURE" << std::endl;
  }

  /* compile fragment shader */
  const char* shaderCodeCstr = shaderCode.c_str();
  glShaderSource(shader, 1, &shaderCodeCstr, nullptr);
  glCompileShader(shader);
  if (-1 == checkCompileErrors(shader, SHADER_TYPE_FRAG_SHADER)) 
    return 0;

  return shader;
}

int Shader::checkCompileErrors(GLuint shader, const int type) {
  GLint success; GLchar infoLog[1024];
  std::string errMsg;

  if (SHADER_TYPE_PROGRAM == type) {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    errMsg = "ERROR::SHADER_PROGRAM_LINKING_ERROR";
  } else {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    errMsg = "ERROR::SHADER_COMPILATION_ERROR";
  }

  if (success) return 0;

  glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
  std::cerr << infoLog << std::endl;
  return -1;
}
