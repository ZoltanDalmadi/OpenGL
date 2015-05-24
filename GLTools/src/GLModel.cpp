#include "GLModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GLTools::GLModel::GLModel()
{}

GLTools::GLModel::GLModel(const std::string& path)
{
  loadModel(path);
}

GLTools::GLModel::~GLModel()
{}

void GLTools::GLModel::loadModel(const std::string& path)
{
  Assimp::Importer importer;
  auto scene
    = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

  for (size_t i = 0; i < scene->mNumMaterials; i++)
    m_materials.emplace_back(loadMaterial(scene->mMaterials[i]));

  for (size_t i = 0; i < scene->mNumMeshes; i++)
  {
    auto activeAiMesh = scene->mMeshes[i];
    m_meshes.emplace_back(loadMesh(activeAiMesh));
    m_meshes.back().m_material =
      std::make_shared<GLMaterial>(m_materials[activeAiMesh->mMaterialIndex]);
    m_meshes.back().setMaterialTextureIndices(1, 2);
    m_meshes.back().initialize();
  }
}

void GLTools::GLModel::draw(const GLShaderProgram& shaderProgram)
{
  for (auto& mesh : m_meshes)
    mesh.draw(shaderProgram);
}

GLTools::GLMesh GLTools::GLModel::loadMesh(const aiMesh *mesh)
{
  GLMesh result;

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

    if (mesh->mTextureCoords[0])
    {
      glm::vec2 vector2;
      vector2.x = mesh->mTextureCoords[0][i].x;
      vector2.y = 1.0f - mesh->mTextureCoords[0][i].y;
      vert.texCoords = vector2;
    }

    result.m_vertices.push_back(vert);
  }

  for (size_t i = 0; i < mesh->mNumFaces; i++)
  {
    auto face = mesh->mFaces[i];

    for (GLuint j = 0; j < face.mNumIndices; j++)
      result.m_indices.push_back(face.mIndices[j]);
  }

  return result;
}

GLTools::GLMaterial GLTools::GLModel::loadMaterial(const aiMaterial *mat)
{
  GLMaterial result;

  aiColor3D a_color;
  float a_float;
  aiString a_string;
  mat->Get(AI_MATKEY_COLOR_DIFFUSE, a_color);
  result.m_diffuse = glm::vec3(a_color.r, a_color.g, a_color.b);
  mat->Get(AI_MATKEY_COLOR_SPECULAR, a_color);
  result.m_specular = glm::vec3(a_color.r, a_color.g, a_color.b);
  mat->Get(AI_MATKEY_SHININESS, a_float);
  result.m_shininess = a_float;

  if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
  {
    mat->GetTexture(aiTextureType_DIFFUSE, 0, &a_string);

    for (const auto& material : m_materials)
    {
      if (material.m_diffuseTex)
      {
        if (material.m_diffuseTex->m_loadedTexturePath == a_string.C_Str())
        {
          result.m_diffuseTex = material.m_diffuseTex;
          break;
        }
      }

      if (material.m_specularTex)
      {
        if (material.m_specularTex->m_loadedTexturePath == a_string.C_Str())
        {
          result.m_diffuseTex = material.m_specularTex;
          break;
        }
      }
    }

    if (!result.m_diffuseTex)
      result.m_diffuseTex = std::make_shared<GLTexture>(a_string.C_Str());

    result.m_diffMix = 1.0f;
  }
  else
    result.m_diffMix = 0.0f;

  if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0)
  {
    mat->GetTexture(aiTextureType_SPECULAR, 0, &a_string);

    for (const auto& material : m_materials)
    {
      if (material.m_diffuseTex)
      {
        if (material.m_diffuseTex->m_loadedTexturePath == a_string.C_Str())
        {
          result.m_specularTex = material.m_diffuseTex;
          break;
        }
      }

      if (material.m_specularTex)
      {
        if (material.m_specularTex->m_loadedTexturePath == a_string.C_Str())
        {
          result.m_specularTex = material.m_specularTex;
          break;
        }
      }
    }

    if (!result.m_specularTex)
      result.m_specularTex = std::make_shared<GLTexture>(a_string.C_Str());

    result.m_specMix = 1.0f;
  }
  else
    result.m_specMix = 0.0f;

  return result;
}