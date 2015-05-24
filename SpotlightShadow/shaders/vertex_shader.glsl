#version 430 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texCoords;

uniform mat4 model;
uniform mat4 viewProjection;
uniform mat3 normalMatrix;
uniform mat4 lightSpaceMatrix;
uniform mat4 projectorMatrix;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;
out vec4 projTexCoords;
out vec4 fragPosinLightSpace;

subroutine void renderPassType();
subroutine uniform renderPassType renderPass;

layout(index = 1)
subroutine (renderPassType)
void recordDepth()
{
  gl_Position = lightSpaceMatrix * model * vec4(vertex_position, 1.0f);
}

layout(index = 2)
subroutine (renderPassType)
void normalRender()
{
  gl_Position = viewProjection * model * vec4(vertex_position, 1.0);
  normal = normalMatrix * vertex_normal;
  fragPos = vec3(model * vec4(vertex_position, 1.0));
  texCoords = vertex_texCoords;
  fragPosinLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
  projTexCoords = projectorMatrix * vec4(fragPos, 1.0);
}

void main()
{
  renderPass();
}
