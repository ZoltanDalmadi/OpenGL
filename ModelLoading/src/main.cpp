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
#include "GLCamera.h"

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

GLFWwindow *window;

glm::mat4 model;
glm::mat4 lightModel;
glm::mat4 projection;

glm::vec3 lightPosition(-1.0f, 1.0f, -2.0f);

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
};

double lastX = WIDTH / 2.0f, lastY = HEIGHT / 2.0f;

GLCamera camera(glm::vec3(0.0f, 0.0f, -5.0f));

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.dolly(-0.1f);

  if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.dolly(0.1f);

  if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.pan(0.1f);

  if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.pan(-0.1f);

  if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.rotate(0.0f, 1.0f);

  if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.rotate(0.0f, -1.0f);

  if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.rotate(-1.0f, 0.0f);

  if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    camera.rotate(1.0f, 0.0f);

  if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    vec3 delta(0.0f, 0.1f, 0.0f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }

  if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    vec3 delta(0.0f, -0.1f, 0.0f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }

  if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    vec3 delta(-0.1f, 0.0f, 0.0f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }

  if (key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    vec3 delta(0.1f, 0.0f, 0.0f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }

  if (key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    vec3 delta(0.0f, 0.0f, 0.1f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }

  if (key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    vec3 delta(0.0f, 0.0f, -0.1f);
    lightPosition += delta;
    lightModel = glm::translate(lightModel, delta);
  }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  double xoffset = xpos - lastX;
  double yoffset = ypos - lastY;
  lastX = xpos;
  lastY = ypos;
  camera.FPSMode(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

void init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

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
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// SHADERS --------------------------------------------------------------------
void setupShaders(GLShaderProgram& shaderProgram, GLShaderProgram& lightShader)
{
  GLShader vertexShader(GLShader::shaderType::VERTEX_SHADER);
  vertexShader.loadSource("vertex_shader.glsl");
  vertexShader.compile();
  std::cout << vertexShader.log() << std::endl;

  GLShader fragmentShader(GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader.loadSource("fragment_shader.glsl");
  fragmentShader.compile();
  std::cout << fragmentShader.log() << std::endl;

  GLShader lightVertex(GLShader::shaderType::VERTEX_SHADER);
  lightVertex.loadSource("vertex_shader_light.glsl");
  lightVertex.compile();
  std::cout << lightVertex.log() << std::endl;

  GLShader lightFragment(GLShader::shaderType::FRAGMENT_SHADER);
  lightFragment.loadSource("fragment_shader_light.glsl");
  lightFragment.compile();
  std::cout << lightFragment.log() << std::endl;

  // Shader program
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;

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
  GLuint VAO, lightVAO, lightVBO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &lightVBO);
  glGenBuffers(1, &EBO);

  // --------------------------------------------------------------------
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
    GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
    vertices.data(), GL_STATIC_DRAW
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
    indices.data(), GL_STATIC_DRAW
  );

  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(0)
  );
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    reinterpret_cast<GLvoid *>(offsetof(Vertex, normal))
  );
  glEnableVertexAttribArray(1);

  // --------------------------------------------------------------------
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(lightPosition),
    &lightPosition, GL_STATIC_DRAW
  );

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, sizeof(lightPosition), reinterpret_cast<GLvoid *>(0)
  );
  glBindVertexArray(0);
  // --------------------------------------------------------------------

  projection = glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 100.0f);

  // Program loop -------------------------------------------------------------
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render
    shaderProgram.use();
    GLint mLoc = glGetUniformLocation(shaderProgram.ID(), "model");
    GLint vLoc = glGetUniformLocation(shaderProgram.ID(), "view");
    GLint pLoc = glGetUniformLocation(shaderProgram.ID(), "projection");
    GLint objectColorLoc = glGetUniformLocation(shaderProgram.ID(), "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shaderProgram.ID(), "lightColor");
    GLint lightPosLoc = glGetUniformLocation(shaderProgram.ID(), "lightPosition");
    GLint cameraPosLoc = glGetUniformLocation(shaderProgram.ID(), "cameraPosition");

    glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.3f);
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
    glUniform3f(lightPosLoc, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform3f(
      cameraPosLoc, camera.getPosition().x,
      camera.getPosition().y, camera.getPosition().z
    );

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                   GL_UNSIGNED_INT, 0);

    lightShaderProgram.use();
    mLoc = glGetUniformLocation(lightShaderProgram.ID(), "model");
    vLoc = glGetUniformLocation(lightShaderProgram.ID(), "view");
    pLoc = glGetUniformLocation(lightShaderProgram.ID(), "projection");
    glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_POINTS, 0, 1);

    glBindVertexArray(0);

    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &lightVBO);
  glDeleteBuffers(1, &EBO);
  glfwTerminate();

  return 0;
}