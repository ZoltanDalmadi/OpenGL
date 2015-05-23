#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

#include "GLShaderProgram.h"
#include "GLBufferObject.h"
#include "GLVertexArrayObject.h"
#include "GLMaterial.h"

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
  GLMaterial m_material;

  GLMesh();
  explicit GLMesh(const std::string& path);
  GLMesh(std::vector<Vertex>& v, std::vector<GLuint>& i);

  virtual ~GLMesh();

  virtual void initialize();
  void loadMesh(const std::string& path);
  virtual void draw(const GLShaderProgram& shaderProgram);
  virtual void draw();

  GLVertexArrayObject m_VAO;
  GLBufferObject m_VBO;
  GLBufferObject m_EBO;
};

}
