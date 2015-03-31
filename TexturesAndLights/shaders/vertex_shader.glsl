#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 normalVec;
out vec3 fragPos;
out vec2 texCoord;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.0f);
  fragPos = vec3(model * vec4(position, 1.0f));
  normalVec = normalMatrix * normal;
  texCoord = vec2(1.0 - texCoords.x, 1.0 - texCoords.y);
}
