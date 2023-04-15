#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>


class Shader {
public:

  /* constructor: reads shader sources and builds */
  Shader(const std::string& vertexShaderPath,
         const std::string& fragmentShaderPath);

  /* activate shader */
  void use() const;

  /* get opengl identifier */
  unsigned int getId() const;

  /* utility functions for setting uniforms */
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;

private:
  unsigned int _id; /* opengl shader identifier */

  /* create shader program from paths of the fragment shader and vertex
   * shader source GLSL files */
  static GLuint buildProgram(const std::string& vertexShaderPath,
                             const std::string& fragmentShaderPath);

  /* compile shader of type LOADER_TYPE_VERT_SHADER or LOADER_TYPE_FRAG_SHADER
   * given the path to the shader source GLSL file */
  static GLuint compile(const std::string& shaderPath, const int shaderType);

  /* private method for checking shader compilation errors */
  static int checkCompileErrors(GLuint shader, const int type);
};

#endif /* ifndef SHADER_H */
