#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

#include "GLShaderProgram.h"
#include "GLBufferObject.h"
#include "GLVertexArrayObject.h"
#include "GLCamera.h"

using namespace GLTools;

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

GLFWwindow *window;

glm::mat4 model;
glm::mat4 lightModel;
glm::mat4 projection;

glm::vec3 lightPosition(-1.0f, 1.0f, -2.0f);

GLCamera camera(glm::vec3(0.0f, 0.0f, -5.0f));

double lastX = WIDTH / 2.0f, lastY = HEIGHT / 2.0f;

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
};

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.movePosition(glm::vec3(0.0f, 0.0f, 0.1f));

  if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.movePosition(glm::vec3(0.0f, 0.0f, -0.1f));

  if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.movePosition(glm::vec3(0.1f, 0.0f, 0.0f));

  if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.movePosition(glm::vec3(-0.1f, 0.0f, 0.0f));

  if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.moveTarget(glm::vec3(0.1f, 0.0f, 0.0f));

  if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.moveTarget(glm::vec3(-0.1f, 0.0f, 0.0f));

  if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.moveTarget(glm::vec3(0.0f, 0.1f, 0.0f));

  if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.moveTarget(glm::vec3(0.0f, -0.1f, 0.0f));

  if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    glm::vec3 delta(0.0f, 0.1f, 0.0f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }

  if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    glm::vec3 delta(0.0f, -0.1f, 0.0f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }

  if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    glm::vec3 delta(-0.1f, 0.0f, 0.0f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }

  if (key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    glm::vec3 delta(0.1f, 0.0f, 0.0f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }

  if (key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    glm::vec3 delta(0.0f, 0.0f, 0.1f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }

  if (key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    glm::vec3 delta(0.0f, 0.0f, -0.1f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }
}

void init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  window = glfwCreateWindow(WIDTH, HEIGHT, "ModelLoading", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glPointSize(2);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// SHADERS --------------------------------------------------------------------
void setupShaders(GLShaderProgram& shaderProgram, GLShaderProgram& lightShader)
{
  auto vertexShader =
    std::make_shared<GLShader>(GLShader::shaderType::VERTEX_SHADER);
  vertexShader->loadSource("vertex_shader.glsl");
  vertexShader->compile();
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader =
    std::make_shared<GLShader>(GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader->loadSource("fragment_shader.glsl");
  fragmentShader->compile();
  std::cout << fragmentShader->log() << std::endl;

  auto lightVertex =
    std::make_shared<GLShader>(GLShader::shaderType::VERTEX_SHADER);
  lightVertex->loadSource("vertex_shader_light.glsl");
  lightVertex->compile();
  std::cout << lightVertex->log() << std::endl;

  auto lightFragment =
    std::make_shared<GLShader>(GLShader::shaderType::FRAGMENT_SHADER);
  lightFragment->loadSource("fragment_shader_light.glsl");
  lightFragment->compile();
  std::cout << lightFragment->log() << std::endl;

  // Shader program
  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;

  lightShader.create();
  lightShader.addShader(lightVertex);
  lightShader.addShader(lightFragment);
  lightShader.link();
  std::cout << lightShader.log() << std::endl;
}

// The MAIN function
int main()
{
  init();

  glfwSetKeyCallback(window, key_callback);
  //glfwSetCursorPosCallback(window, mouse_callback);
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // STUFF --------------------------------------------------------------------
  GLShaderProgram shaderProgram;
  GLShaderProgram lightShaderProgram;
  setupShaders(shaderProgram, lightShaderProgram);

  Assimp::Importer importer;
  const aiScene *scene
    = importer.ReadFile("model.obj",
                        aiProcess_Triangulate | aiProcess_GenSmoothNormals);

  aiMesh *mesh = scene->mMeshes[0];

  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

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
    vertices.push_back(vert);
  }

  for (size_t i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];

    for (GLuint j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  // VAO, VBO
  GLVertexArrayObject VAO, lightVAO;
  VAO.create();
  lightVAO.create();

  GLBufferObject VBO, lightVBO;
  GLBufferObject EBO(GLBufferObject::BufferType::IndexBuffer);
  VBO.create();
  lightVBO.create();
  EBO.create();

  // --------------------------------------------------------------------
  VAO.bind();

  VBO.bind();
  VBO.upload(vertices.data(), vertices.size() * sizeof(Vertex));

  EBO.bind();
  EBO.upload(indices.data(), indices.size() * sizeof(GLuint));

  shaderProgram.setAttributeArray(0, 3, offsetof(Vertex, position),
                                  sizeof(Vertex));

  shaderProgram.setAttributeArray(1, 3, offsetof(Vertex, normal),
                                  sizeof(Vertex));

  // --------------------------------------------------------------------
  lightVAO.bind();
  lightVBO.bind();
  lightVBO.upload(&lightPosition, sizeof(lightPosition));
  lightShaderProgram.setAttributeArray(0, 3, 0, sizeof(lightPosition));
  lightVAO.unbind();

  // --------------------------------------------------------------------
  projection = glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 100.0f);

  // Program loop -------------------------------------------------------------
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render
    shaderProgram.use();
    shaderProgram.setUniformValue("model", model);
    shaderProgram.setUniformValue("view", camera.m_viewMatrix);
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("objectColor", 1.0f, 0.5f, 0.3f);
    shaderProgram.setUniformValue("lightColor", 1.0f, 1.0f, 1.0f);
    shaderProgram.setUniformValue("lightPosition", lightPosition);
    shaderProgram.setUniformValue("cameraPosition", camera.m_position);

    VAO.bind();

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                   GL_UNSIGNED_INT, 0);

    lightShaderProgram.use();
    lightShaderProgram.setUniformValue("model", lightModel);
    lightShaderProgram.setUniformValue("view", camera.m_viewMatrix);
    lightShaderProgram.setUniformValue("projection", projection);

    lightVAO.bind();
    glDrawArrays(GL_POINTS, 0, 1);

    lightVAO.unbind();

    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
