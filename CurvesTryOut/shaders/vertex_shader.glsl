#version 430 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertexColor;

out vec3 color;

void main()
{
  color = vertexColor;
  gl_Position = vec4(vertex_position, 1.0);
}
