#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include "include/shader.hpp"

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


void Shader::setFloat4(const std::string &name, glm::vec4 values) const {
  glUniform4f(glGetUniformLocation(_id, name.c_str()),
              values.x, values.y, values.z, values.w);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const {
  glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}

GLuint Shader::buildProgram(const std::string& vertexShaderPath,
                            const std::string& fragmentShaderPath) {
  /* compile shaders */
  GLuint vertexShader, fragmentShader;
  vertexShader = compile(vertexShaderPath, ShaderType::VERTEX_SHADER);

  if (0 == vertexShader) {
    std::cerr << "ERROR::VERTEX_SHADER_COMPILATION_FAILURE" << std::endl;
    return 0;
  }

  fragmentShader = compile(fragmentShaderPath, ShaderType::FRAGMENT_SHADER);
  if (0 == fragmentShader) {
    std::cerr << "ERROR::FRAGMENT_SHADER_COMPILATION_FAILURE" << std::endl;
    return 0;
  }

  /* create shader program and link fragment shader */
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  if (-1 == checkCompileErrors(shaderProgram, ShaderType::SHADER_PROGRAM))
    return 0;

  /* clean up shader objects */
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

GLuint Shader::compile(const std::string& shaderPath,
                       const ShaderType type) {
  /* create shader */
  GLuint shader;
  if (ShaderType::FRAGMENT_SHADER == type) {
    shader = glCreateShader(GL_FRAGMENT_SHADER);
  } else if (ShaderType::VERTEX_SHADER == type) {
    shader = glCreateShader(GL_VERTEX_SHADER);
  } else {
    std::cerr << "ERROR::SHADER_TYPE_ERROR Invalid shader type: " <<
      type << std::endl;
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
  if (-1 == checkCompileErrors(shader, type)) 
    return 0;

  return shader;
}

int Shader::checkCompileErrors(const GLuint shader, const ShaderType type) {
  GLint success; GLchar infoLog[1024];
  std::string errMsg;

  if (ShaderType::SHADER_PROGRAM == type) {
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
