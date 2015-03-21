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
glm::mat4 projection;

struct Vertex
{
  glm::vec3 position;
  glm::vec3 color;
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
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// SHADERS --------------------------------------------------------------------
void setupShaders(GLShaderProgram& shaderProgram)
{
  GLShader vertexShader(GLShader::shaderType::VERTEX_SHADER);
  vertexShader.loadSource("vertex_shader.glsl");
  vertexShader.compile();

  GLShader fragmentShader(GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader.loadSource("fragment_shader.glsl");
  fragmentShader.compile();

  // Shader program
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
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
  setupShaders(shaderProgram);

  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile("model.obj", aiProcess_Triangulate);

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
    vert.color = vec3(0.0f);
    vertices.push_back(vert);
  }

  for (size_t i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];

    for (GLuint j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  // VAO, VBO
  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<GLvoid *>(0));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<GLvoid *>(offsetof(Vertex, color)));
  glBindVertexArray(0);

  projection = glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 100.0f);

  GLint mLoc = glGetUniformLocation(shaderProgram.ID(), "model");
  GLint vLoc = glGetUniformLocation(shaderProgram.ID(), "view");
  GLint pLoc = glGetUniformLocation(shaderProgram.ID(), "projection");

  // Program loop -------------------------------------------------------------
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render
    shaderProgram.use();

    glBindVertexArray(VAO);

    glDrawElements(GL_POINTS, static_cast<GLsizei>(indices.size()),
                   GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glfwTerminate();

  return 0;
}
