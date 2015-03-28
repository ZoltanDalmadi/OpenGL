#include "GLMesh.h"

GLTools::GLMesh::GLMesh()
{
}

GLTools::GLMesh::GLMesh
(std::vector<Vertex>& v, std::vector<GLuint>& i, std::vector<Texture>& t)
  : vertices(std::move(v)),
    indices(std::move(i)),
    textures(std::move(t))
{
  initialize();
}

GLTools::GLMesh::~GLMesh()
{}

void GLTools::GLMesh::initialize()
{
  VAO.create();
  VBO.create();
  EBO.create();

  VAO.bind();

  VBO.bind();
  VBO.upload(vertices.data(), vertices.size() * sizeof(Vertex));
  EBO.bind();
  EBO.upload(indices.data(), indices.size() * sizeof(GLuint));

  VAO.setAttributeArray(0, 3, offsetof(Vertex, position), sizeof(Vertex));
  VAO.setAttributeArray(1, 3, offsetof(Vertex, normal), sizeof(Vertex));
  VAO.setAttributeArray(2, 3, offsetof(Vertex, texCoords), sizeof(Vertex));

  VAO.unbind();
}