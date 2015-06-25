#version 430 core

layout (location = 0) in vec3 vertex_position;
out vec3 ControlPoints;

void main()
{
  gl_Position = vec4(vertex_position.x, vertex_position.y, 0.0f, 1.0);
}
