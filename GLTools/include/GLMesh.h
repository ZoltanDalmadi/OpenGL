#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

#include "GLShaderProgram.h"
#include "GLBufferObject.h"
#include "GLVertexArrayObject.h"

namespace GLTools
{

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct Texture
{
  GLint ID;
  enum class TextureType
  {
    DIFFUSE,
    SPECULAR,
    BUMP,
    NORMAL
  };
};

class GLMesh
{
public:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  GLMesh();
  GLMesh(std::vector<Vertex>& v, std::vector<GLuint>& i, std::vector<Texture>& t);

  virtual ~GLMesh();

  void initialize();

private:
  GLVertexArrayObject VAO;
  GLBufferObject VBO;
  GLBufferObject EBO;
};

}
