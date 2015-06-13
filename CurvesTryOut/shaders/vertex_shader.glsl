#version 430 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 normal;

void main()
{
  gl_Position = projection * view * model * vec4(vertex_position, 1.0);
  normal = normalMatrix * vertex_normal;
}
