#version 330 core

layout (location = 0) in vec2 position;
out vec4 vertexColor;
uniform float xoffset;
uniform float yoffset;

void main(){
  vec2 offset = vec2(xoffset, yoffset);
  gl_Position = vec4(position + offset, 0.0, 1.0);
  vertexColor = vec4(abs(position) + offset, 0.0, 1.0);
}
