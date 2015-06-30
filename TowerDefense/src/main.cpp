#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>
#include <memory>
#include <iostream>

#include "GLFPSCamera.h"
#include "GLModel.h"
#include "GLPointLight.h"
#include "GLPlane.h"
#include "GLBoundingBox.h"
#include "GLCurves.h"
#include "GLCurvePath.h"
#include "Tower.h"
#include "Enemy.h"
#include <Windows.h>
#include <algorithm>
#include "GridPlane.h"

//constants
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

GLFWwindow *window;

GLTools::GLFPSCamera camera(glm::vec3(0.0f, 1.0f, -5.0f));
GLTools::GLPointLight pointLight(glm::vec3(0.0f, 10.0f, 0.0f));

glm::vec3 target(5.0f, 5.0f, 5.0f);
glm::vec3 targetDir(0.01f, 0.0f, 1.0f);

bool keys[1024];

// for mouse
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;
bool firstMouse = true;
bool enabled = false;

auto maxTower = 5;
auto actualTower = 0;
std::list<Tower> towers;
std::unique_ptr<Enemy> targetShip;
std::unique_ptr<GLTools::GLBoundingBox> boundingBox;
std::unique_ptr<GLTools::GLPlane> floorPlane;
std::unique_ptr<GridPlane> gridPlane;
std::shared_ptr<GLTools::GLModel> base;
std::shared_ptr<GLTools::GLModel> cannon;
std::shared_ptr<GLTools::GLModel> missile;
std::vector<glm::vec3> closeSquares;
auto forbiddenPlace = false;

GLTools::GLCurvePath path;
bool exploding = false;
float timeToExplode = 0.0f;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;

  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_C && action == GLFW_PRESS)
  {
    for (auto& tower : towers)
    {
      tower.clearTarget();
    }
  }

  if (key == GLFW_KEY_V && action == GLFW_PRESS)
  {
    for (auto& tower : towers)
    {
      tower.setTarget(&target);
    }
  }

  if (key == GLFW_KEY_B && action == GLFW_PRESS)
    enabled = true;

  if (key == GLFW_KEY_N && action == GLFW_PRESS)
  {
    exploding = true;
    PlaySound("sfx/explosion.WAV", NULL, SND_ASYNC);
    timeToExplode = glfwGetTime();
  }

}

void moveCamera()
{
  if (keys[GLFW_KEY_W])
    camera.move(GLTools::GLFPSCamera::Direction::FORWARD);

  if (keys[GLFW_KEY_S])
    camera.move(GLTools::GLFPSCamera::Direction::BACKWARD);

  if (keys[GLFW_KEY_A])
    camera.move(GLTools::GLFPSCamera::Direction::LEFT);

  if (keys[GLFW_KEY_D])
    camera.move(GLTools::GLFPSCamera::Direction::RIGHT);

  if (keys[GLFW_KEY_SPACE])
    camera.move(GLTools::GLFPSCamera::Direction::UP);

  if (keys[GLFW_KEY_LEFT_CONTROL])
    camera.move(GLTools::GLFPSCamera::Direction::DOWN);
}

void moveTarget()
{
  if (keys[GLFW_KEY_UP])
  {
    target += glm::vec3(0.0f, 0.2f, 0.0f);
    targetShip->setPosition(target);
  }

  if (keys[GLFW_KEY_DOWN])
  {
    target -= glm::vec3(0.0f, 0.2f, 0.0f);
    targetShip->setPosition(target);
  }

  if (keys[GLFW_KEY_LEFT])
  {
    target += glm::vec3(0.2f, 0.0f, 0.0f);
    targetShip->setPosition(target);
  }

  if (keys[GLFW_KEY_RIGHT])
  {
    target -= glm::vec3(0.2f, 0.0f, 0.0f);
    targetShip->setPosition(target);
  }

  if (keys[GLFW_KEY_PAGE_UP])
  {
    target += glm::vec3(0.0f, 0.0f, 0.2f);
    targetShip->setPosition(target);
  }

  if (keys[GLFW_KEY_PAGE_DOWN])
  {
    target -= glm::vec3(0.0f, 0.0f, 0.2f);
    targetShip->setPosition(target);
  }

  if (keys[GLFW_KEY_I])
  {
    targetDir = rotateX(targetDir, glm::radians(-1.0f));
    targetShip->setDirection(targetDir);
  }

  if (keys[GLFW_KEY_K])
  {
    targetDir = rotateX(targetDir, glm::radians(1.0f));
    targetShip->setDirection(targetDir);
  }

  if (keys[GLFW_KEY_J])
  {
    targetDir = rotateY(targetDir, glm::radians(-1.0f));
    targetShip->setDirection(targetDir);
  }

  if (keys[GLFW_KEY_L])
  {
    targetDir = rotateY(targetDir, glm::radians(1.0f));
    targetShip->setDirection(targetDir);
  }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  auto xoffset = xpos - lastX;
  auto yoffset = ypos - lastY;
  lastX = xpos;
  lastY = ypos;
  camera.rotate(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mod)
{
  /*Curve */
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE
      && actualTower < maxTower)
  {
    if (forbiddenPlace)
    {
      PlaySound("sfx/error.WAV", NULL, SND_ASYNC);
    }
    else
    {
      if (actualTower + 1 != maxTower)
        towers.emplace_back(base.get(), cannon.get(), missile.get());

      actualTower++;
    }
  }
}

// SHADERS --------------------------------------------------------------------
void setupShaders(GLTools::GLShaderProgram& shaderProgram)
{
  auto vertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER,
     "shaders/vertex_shader.glsl");
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/fragment_shader.glsl");
  std::cout << fragmentShader->log() << std::endl;

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;
}

void setupShaders1(GLTools::GLShaderProgram& shaderProgram)
{
  auto vertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER,
     "shaders/curves_vertex_shader.glsl");
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/curves_fragment_shader.glsl");
  std::cout << fragmentShader->log() << std::endl;

  auto geometryShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::GEOMETRY_SHADER,
     "shaders/curves_geometry_shader.glsl");
  std::cout << geometryShader->log() << std::endl;

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(geometryShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;
}

void setupShaders2(GLTools::GLShaderProgram& shaderProgram)
{
  auto vertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER,
     "shaders/exploding_vertex_shader.glsl");
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/exploding_fragment_shader.glsl");
  std::cout << fragmentShader->log() << std::endl;

  auto geometryShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::GEOMETRY_SHADER,
     "shaders/exploding_geometry_shader.glsl");
  std::cout << geometryShader->log() << std::endl;

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(geometryShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;
}

void init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  window = glfwCreateWindow(WIDTH, HEIGHT, "TowerDefense", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonOffset(1, 0);
  glLineWidth(2);
  glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

  std::array<glm::vec3, 4> temp = { glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(10.0f, 0.0f, 30.0f),
                                    glm::vec3(17.0f, 0.0f, -34.0f),
                                    glm::vec3(40.0f, 0.0f, 40.0f)
                                  };
  GLTools::GLCurves curve(temp);
  std::array<glm::vec3, 2> temp1 = { glm::vec3(45.0f, 0.0f, -45.0f),
                                     glm::vec3(50.0f, 0.0f, 50.0f)
                                   };
  GLTools::GLCurves curve1(curve, temp1);
  curve.initialize();
  curve1.initialize();
  path.m_data.push_back(curve);
  path.m_data.push_back(curve1);
}

std::pair<glm::vec3, glm::vec3> calcBoundingBox
(const std::pair<glm::vec3, glm::vec3>& input)
{
  auto& minX = input.first.x;
  auto& minY = input.first.y;
  auto& minZ = input.first.z;
  auto& maxX = input.second.x;
  auto& maxY = input.second.y;
  auto& maxZ = input.second.z;

  auto center =
    glm::vec3((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f);
  auto size = glm::vec3(maxX - minX, maxY - minY, maxZ - minZ);
  return std::make_pair(center, size);
}

bool contains(const std::vector<glm::vec3>& vec, const glm::vec3& point)
{
  for (auto& v : vec)
  {
    if (v == point)
    {
      return true;
    }
  }

  return false;
}

void renderScene(const GLTools::GLShaderProgram& shaderProgram)
{

  shaderProgram.use();
  auto model = rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f,
                      0.0f, 0.0f));
  auto normalMatrix = glm::mat3(model);
  shaderProgram.setUniformValue("model", model);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  gridPlane->draw();
  shaderProgram.setUniformValue("lines", true);
  gridPlane->drawLines();
  shaderProgram.setUniformValue("lines", false);

  float d = 0.0f;
  bool intersect = glm::intersectRayPlane(camera.m_position, -camera.m_front,
                                          glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), d);

  auto temp = -camera.m_front * d;
  temp.y = 0.0f;

  auto towerSize = towers.size();

  if (actualTower < maxTower)
  {
    glm::vec3 temp2;
    auto found = gridPlane->getCenter(temp, temp2);
    towers.back().setPosition(temp2);

    /*Itt a pirosítást végzem a towereken, ahol nem szabad lerakni.*/
    forbiddenPlace = false;

    if (!found
        || std::find(closeSquares.begin(), closeSquares.end(),
                     temp2) != closeSquares.end())
    {
      forbiddenPlace = true;
      shaderProgram.setUniformValue("forbiddenTower", true);
    }

    /*itt vége.*/
    shaderProgram.setUniformValue("transparent", true);
    towers.back().draw(shaderProgram, glfwGetTime());
    shaderProgram.setUniformValue("transparent", false);
    shaderProgram.setUniformValue("forbiddenTower", false);
    towerSize -= 1;
  }

  for (auto tower : towers)
  {
    tower.draw(shaderProgram, glfwGetTime());
  }

}


int main()
{
  init();

  auto shaderProgram2 = std::make_unique<GLTools::GLShaderProgram>();
  setupShaders2(*shaderProgram2);

  auto shaderProgram1 = std::make_unique<GLTools::GLShaderProgram>();
  setupShaders1(*shaderProgram1);

  auto shaderProgram = std::make_unique<GLTools::GLShaderProgram>();
  setupShaders(*shaderProgram);

  shaderProgram->use();

  auto defaultMaterial = std::make_unique<GLTools::GLMaterial>();

  auto enemy = std::make_unique<GLTools::GLModel>("models/enemyship.obj");

  base = std::make_shared<GLTools::GLModel>("models/turret_base.obj");
  cannon = std::make_shared<GLTools::GLModel>("models/turret_cannon.obj");
  missile = std::make_shared<GLTools::GLModel>("models/missile.obj");

  base->m_materials[0] = *defaultMaterial;
  cannon->m_materials[0] = *defaultMaterial;

  targetShip = std::make_unique<Enemy>(enemy.get(), target, targetDir);

  boundingBox = std::make_unique<GLTools::GLBoundingBox>();
  boundingBox->initialize();

  floorPlane = std::make_unique<GLTools::GLPlane>(100.0f, 100.0f);
  floorPlane->initialize();
  floorPlane->m_material = defaultMaterial.get();

  gridPlane = std::make_unique<GridPlane>(100.0f, 100.0f, 4.0f);
  gridPlane->initialize();
  gridPlane->setMaterial(defaultMaterial.get());

  /*Initialize the squares center which must be empty because the path*/
  auto OneOverDetail = 1.0 / float(100.0 - 1.0);

  for (int i = 0; i < 100; i++)
  {
    auto t = i * OneOverDetail;
    glm::vec3 point;
    auto found = gridPlane->getCenter(path.getPositionAndTangent(t).first, point);

    if (found)
    {
      if (std::find(closeSquares.begin(), closeSquares.end(),
                    point) == closeSquares.end())
      {
        closeSquares.push_back(point);
        std::cout << point.x << " " << point.y << " " << point.z << std::endl;
      }
    }
  }

  /*end initialize*/

  towers.emplace_back(base.get(), cannon.get(), missile.get());

  pointLight.setEnergy(2.0f);

  auto projection =
    glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 500.0f);

  auto t = 0.0f;

  auto explodingSize = 0.0f;

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    moveCamera();
    moveTarget();

    shaderProgram->use();
    shaderProgram->setUniformValue("viewProjection",
                                   projection * camera.m_viewMatrix);
    shaderProgram->setUniformValue("camPos", camera.m_position);
    pointLight.setShaderUniform(*shaderProgram);

    renderScene(*shaderProgram);

    if (!exploding)
    {
      targetShip->draw(*shaderProgram);
      auto aabb = targetShip->calculate_AABB();
      auto bounding = calcBoundingBox(aabb);
      boundingBox->draw(*shaderProgram, bounding.first, bounding.second);
    }
    else
    {
      if (glfwGetTime() - timeToExplode < 2)
      {
        shaderProgram2->use();
        explodingSize += 0.2f;
        shaderProgram2->setUniformValue("projection", projection);
        shaderProgram2->setUniformValue("view", camera.m_viewMatrix);
        shaderProgram2->setUniformValue("model", targetShip->getModelMatrx());
        shaderProgram2->setUniformValue("time", explodingSize);
        targetShip->draw(*shaderProgram2);
      }
    }


    shaderProgram1->use();
    shaderProgram1->setUniformValue("MVP", projection * camera.m_viewMatrix);

    path.draw();
    std::pair<glm::vec3, glm::vec3> temp = path.getPositionAndTangent(t);
    targetShip->setPosition(temp.first);
    target = temp.first;
    auto targetDir1 = glm::normalize(temp.second);
    targetShip->setDirection(targetDir1);

    if (t < 1.0f)
    {
      if (enabled)
        t += 0.001f;
    }
    else
    {
      t = 1.0f;
    }

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
