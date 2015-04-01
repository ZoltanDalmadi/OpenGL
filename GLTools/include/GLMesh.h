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

class GLMesh
{
public:
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  GLMesh();
  GLMesh(std::vector<Vertex>& v, std::vector<GLuint>& i);

  virtual ~GLMesh();

  void initialize();

  GLVertexArrayObject m_VAO;
  GLBufferObject m_VBO;
  GLBufferObject m_EBO;
};

}
