#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 fragPosition;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  fragPosition = vec3(model * vec4(vertexPosition, 1.0f));
  normal = mat3(transpose(inverse(model))) * vertexNormal;
  gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
}
