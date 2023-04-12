#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>

class Loader {
public:
  static GLuint loadFragmentShader(const std::string& shaderPath) {
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    /* read source code from path */
    std::string fragmentCode;
    std::ifstream fragmentFile(shaderPath, std::ios::in);
    if (fragmentFile.is_open()) {
      std::stringstream stream;
      stream << fragmentFile.rdbuf();
      fragmentCode = stream.str();
      fragmentFile.close();
    } else {
      std::cerr << "Failed to open fragment shader file: " <<
        shaderPath << std::endl;
      return 0;
    }

    /* compile fragment shader */
    const char* fragmentCodeCstr = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fragmentCodeCstr, NULL);
    glCompileShader(fragmentShader);
    if (EXIT_FAILURE == checkCompileErrors(fragmentShader, "FRAGMENT"))
      return 0;

    /* create shader program and link fragment shader */
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    if (EXIT_FAILURE == checkCompileErrors(shaderProgram, "PROGRAM"))
      return 0;

    glDeleteShader(fragmentShader);
    return shaderProgram;
  }

private:
  static int checkCompileErrors(GLuint shader, const std::string& type) {
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

    if (success) return EXIT_SUCCESS;

    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
    std::cerr << infoLog << std::endl;
    return EXIT_FAILURE;
  }
};
