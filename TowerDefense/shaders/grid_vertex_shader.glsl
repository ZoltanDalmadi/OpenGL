#version 430 core

layout (location = 0) in vec3 vertex_position;

uniform mat4 viewProjection;

void main()
{
  gl_Position = viewProjection * vec4(vertex_position, 1.0);
}
