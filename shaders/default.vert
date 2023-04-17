#version 330 core

layout (location = 0) in vec3 position;
out vec4 vertexColor;
uniform float xoffset;
uniform float yoffset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
  vec2 offset = vec2(xoffset, yoffset);
  gl_Position = projection * view * model * vec4(position, 1.0);
  vertexColor = vec4(abs(position).xy + offset, abs(position).z, 1.0);
}
