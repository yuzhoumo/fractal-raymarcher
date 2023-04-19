#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

/* enum for denoting types during shader compilation and error checking */
enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER, SHADER_PROGRAM };

class Shader {
public:

  /* constructor: reads shader sources and builds */
  Shader(const std::string& vertex_shader_path,
         const std::string& fragment_shader_path);

  /* activate shader */
  void use() const;

  /* get opengl identifier */
  unsigned int getId() const;

  /* utility functions for setting uniforms */
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setFloat2(const std::string &name, glm::vec2 values) const;
  void setFloat3(const std::string &name, glm::vec3 values) const;
  void setFloat4(const std::string &name, glm::vec4 values) const;
  void setMat4(const std::string &name, glm::mat4 value) const;

private:
  /* opengl shader identifier */
  unsigned int _id;

  /* create shader program from paths of the fragment shader and vertex
   * shader source GLSL files */
  static GLuint _buildProgram(const std::string& vertex_shader_path,
                              const std::string& fragment_shader_path);

  /* compile shader of type LOADER_TYPE_VERT_SHADER or LOADER_TYPE_FRAG_SHADER
   * given the path to the shader source GLSL file */
  static GLuint _compile(const std::string& shader_path, const ShaderType type);

  /* private method for checking shader compilation errors */
  static int _checkCompileErrors(const GLuint shader, const ShaderType type);
};

#endif /* ifndef SHADER_H */
