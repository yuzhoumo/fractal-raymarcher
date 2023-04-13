#include <string>
#include <glad/glad.h>

#define LOADER_FRAG_SHADER "FRAGMENT"
#define LOADER_VERT_SHADER "VERTEX"
#define LOADER_PROGRAM "PROGRAM"

class Loader {
public:
  static GLuint createShaderProgram(const std::string& vertexShaderPath,
                                    const std::string& fragmentShaderPath);

  static GLuint compileShader(const std::string& shaderPath,
                              const std::string& shaderType);
private:
  static int checkCompileErrors(GLuint shader, const std::string& type);
};
