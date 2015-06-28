#version 430 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texCoords;

uniform mat4 model;
uniform mat4 viewProjection;
uniform mat3 normalMatrix;
uniform mat4 MVP;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

void main()
{
  gl_Position = MVP * vec4(vertex_position, 1.0);
  normal = normalMatrix * vertex_normal;
  fragPos = vec3(model * vec4(vertex_position, 1.0));
  texCoords = vertex_texCoords;
}
