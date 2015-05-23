#include "GLMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

GLTools::GLMesh::GLMesh()
{
  m_EBO.setType(GLBufferObject::BufferType::IndexBuffer);
}

GLTools::GLMesh::GLMesh(const std::string& path)
{
  m_EBO.setType(GLBufferObject::BufferType::IndexBuffer);
  loadMesh(path);
}

GLTools::GLMesh::GLMesh(std::vector<Vertex>& v, std::vector<GLuint>& i)
  : m_vertices(move(v)),
    m_indices(move(i))
{
  m_EBO.setType(GLBufferObject::BufferType::IndexBuffer);
}

GLTools::GLMesh::~GLMesh()
{}

void GLTools::GLMesh::initialize()
{
  m_VAO.create();
  m_VBO.create();
  m_EBO.create();

  m_VAO.bind();

  m_VBO.bind();
  m_VBO.upload(m_vertices.data(), m_vertices.size() * sizeof(Vertex));
  m_EBO.bind();
  m_EBO.upload(m_indices.data(), m_indices.size() * sizeof(GLuint));

  m_VAO.setAttributeArray(0, 3, sizeof(Vertex));
  m_VAO.setAttributeArray(1, 3, sizeof(Vertex), offsetof(Vertex, normal));
  m_VAO.setAttributeArray(2, 2, sizeof(Vertex), offsetof(Vertex, texCoords));

  m_VAO.unbind();
}

void GLTools::GLMesh::loadMesh(const std::string& path)
{
  Assimp::Importer importer;
  auto scene
    = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

  auto mesh = scene->mMeshes[0];
  auto mat = scene->mMaterials[mesh->mMaterialIndex];

  for (size_t i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex vert;
    glm::vec3 vector;
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vert.position = vector;
    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vert.normal = vector;

    if (mesh->HasTextureCoords(static_cast<unsigned>(i)))
    {
      glm::vec2 vector2;
      vector2.x = mesh->mTextureCoords[0][i].x;
      vector2.y = mesh->mTextureCoords[0][i].y;
      vert.texCoords = vector2;
    }

    m_vertices.push_back(vert);
  }

  for (size_t i = 0; i < mesh->mNumFaces; i++)
  {
    auto face = mesh->mFaces[i];

    for (GLuint j = 0; j < face.mNumIndices; j++)
      m_indices.push_back(face.mIndices[j]);
  }

  aiColor3D a_color;
  float a_float;
  aiString a_string;

  mat->Get(AI_MATKEY_COLOR_DIFFUSE, a_color);
  m_material.m_diffuse = glm::vec3(a_color.r, a_color.g, a_color.b);
  mat->Get(AI_MATKEY_COLOR_SPECULAR, a_color);
  m_material.m_specular = glm::vec3(a_color.r, a_color.g, a_color.b);
  mat->Get(AI_MATKEY_SHININESS, a_float);
  m_material.m_shininess = a_float;

  if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
  {
    mat->GetTexture(aiTextureType_DIFFUSE, 0, &a_string);
    m_material.m_diffuseTex = std::make_unique<GLTexture>(a_string.C_Str());
    m_material.m_diffMix = 1.0f;
  }
  else
    m_material.m_diffMix = 0.0f;

  if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0)
  {
    mat->GetTexture(aiTextureType_SPECULAR, 0, &a_string);
    std::cout << a_string.C_Str() << std::endl;
    m_material.m_specularTex = std::make_unique<GLTexture>(a_string.C_Str());
    m_material.m_specMix = 1.0f;
  }
  else
    m_material.m_specMix = 0.0f;

  initialize();
}

void GLTools::GLMesh::draw(const GLShaderProgram& shaderProgram)
{
  if (m_material.m_diffuseTex)
    m_material.m_diffuseTex->bind(m_material.m_diffuseTex_index);

  if (m_material.m_specularTex)
    m_material.m_specularTex->bind(m_material.m_specularTex_index);

  m_material.setShaderUniform(shaderProgram);

  draw();
}

void GLTools::GLMesh::draw()
{
  m_VAO.bind();
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
                 GL_UNSIGNED_INT, nullptr);

  m_VAO.unbind();
}