#include <glm/gtc/constants.hpp>
#include "GLSphere.h"

GLTools::GLSphere::GLSphere(float radius, int slices, int stacks)
{
  float theta, phi;
  float thetaFac = glm::two_pi<float>() / slices;
  float phiFac = glm::pi<float>() / stacks;
  float nx, ny, nz, s, t;

  Vertex vert;

  for (int i = 0; i <= slices; i++)
  {
    theta = i * thetaFac;
    s = (float) i / slices;

    for (int j = 0; j <= stacks; j++)
    {
      phi = j * phiFac;
      t = (float) j / stacks;
      nx = std::sin(phi) * std::cos(theta);
      ny = std::sin(phi) * std::sin(theta);
      nz = std::cos(phi);

      vert.position = glm::vec3(radius * nx, radius * ny, radius * nz);
      vert.normal = glm::vec3(nx, ny, nz);
      vert.texCoords = glm::vec2(s, t);

      m_vertices.push_back(vert);
    }
  }

  for (int i = 0; i < slices; i++)
  {
    GLuint stackStart = i * (stacks + 1);
    GLuint nextStackStart = (i + 1) * (stacks + 1);

    for (int j = 0; j < stacks; j++)
    {
      if (j == 0)
      {
        m_indices.push_back(stackStart);
        m_indices.push_back(stackStart + 1);
        m_indices.push_back(nextStackStart + 1);
      }
      else if (j == stacks - 1)
      {
        m_indices.push_back(stackStart + j);
        m_indices.push_back(stackStart + j + 1);
        m_indices.push_back(nextStackStart + j);
      }
      else
      {
        m_indices.push_back(stackStart + j);
        m_indices.push_back(stackStart + j + 1);
        m_indices.push_back(nextStackStart + j + 1);
        m_indices.push_back(nextStackStart + j);
        m_indices.push_back(stackStart + j);
        m_indices.push_back(nextStackStart + j + 1);
      }
    }
  }
}

GLTools::GLSphere::~GLSphere()
{}
