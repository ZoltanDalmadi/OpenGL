#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <memory>
#include <iostream>

#include "GLFPSCamera.h"
#include "GLModel.h"
#include "GLPointLight.h"
#include "GLPlane.h"
#include "GLCurvePath.h"
#include "Tower.h"
#include "Enemy.h"

//constants
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

GLFWwindow *window;

GLTools::GLFPSCamera camera(glm::vec3(0.0f, 1.0f, -5.0f));
GLTools::GLPointLight pointLight(glm::vec3(0.0f, 10.0f, 0.0f));

glm::vec3 target(5.0f, 5.0f, 5.0f);
glm::vec3 targetDir(0.01f, 0.0f, 1.0f);

std::list<Tower> towers;
std::list<Enemy> enemies;

bool keys[1024];

// for mouse
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;
bool firstMouse = true;

GLTools::GLCurvePath enemyPath;

std::unique_ptr<Enemy> targetShip;
std::unique_ptr<GLTools::GLPlane> floorPlane;
std::unique_ptr<GLTools::GLModel> enemy;

void addNewEnemy(GLTools::GLModel *enemyModel)
{
  auto startVectors = enemyPath.getPositionAndTangent(0.0f);
  enemies.emplace_back(enemyModel, startVectors.first, startVectors.second);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;

  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  //if (key == GLFW_KEY_C && action == GLFW_PRESS)
  //  clearTowerTargets();

  //if (key == GLFW_KEY_V && action == GLFW_PRESS)
  //  setTowerTargets(target);

  if (key == GLFW_KEY_E && action == GLFW_PRESS)
    addNewEnemy(enemy.get());
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

// SHADERS --------------------------------------------------------------------
void setupShaders(GLTools::GLShaderProgram& shaderProgram,
                  GLTools::GLShaderProgram& pathProgram)
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

  auto pathVertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER,
     "shaders/curves_vertex_shader.glsl");
  std::cout << pathVertexShader->log() << std::endl;

  auto pathGeometryShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::GEOMETRY_SHADER,
     "shaders/curves_geometry_shader.glsl");
  std::cout << pathGeometryShader->log() << std::endl;

  auto pathFragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/curves_fragment_shader.glsl");
  std::cout << pathFragmentShader->log() << std::endl;

  pathProgram.create();
  pathProgram.addShader(pathVertexShader);
  pathProgram.addShader(pathGeometryShader);
  pathProgram.addShader(pathFragmentShader);
  pathProgram.link();
  std::cout << pathProgram.log() << std::endl;
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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1, 0);
  glLineWidth(2);
  glClearColor(0.0f, 0.3f, 0.6f, 1.0f);
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

void scanForTarget(Tower& tower)
{
  for (auto& enemy : enemies)
  {
    auto enemyPos = enemy.m_position;
    auto d = tower.getPosition() - enemyPos;
    auto rangeSquared = tower.getRange() * tower.getRange();

    if (length2(d) <= rangeSquared)
    {
      tower.setTarget(&enemy.m_position);
      break;
    }
  }

}

void checkHitsAndCleanupMissiles()
{
  for (auto& enemy : enemies)
  {
    for (auto& tower : towers)
    {
      if (tower.m_target)
      {
        auto& missiles = tower.m_missiles;

        for (auto it = missiles.begin(); it != missiles.end();)
        {
          if (enemy.isColliding(it->getPosition()))
          {
            it = missiles.erase(it);
            enemy.damage(tower.getDamage());
          }
          else if (length2(it->getPosition()) > 25.0f * 25.0f)
            it = missiles.erase(it);
          else
            ++it;
        }
      }
      else
        scanForTarget(tower);
    }
  }
}

void cleanupEnemies()
{
  for (auto it = enemies.begin(); it != enemies.end();)
  {
    if (it->isDestroyed() || it->m_progress >= 1.0f)
    {
      for (auto& tower : towers)
      {
        if (tower.m_target == &it->m_position)
          tower.clearTarget();
      }

      it = enemies.erase(it);
    }
    else
      ++it;
  }
}

void renderScene(const GLTools::GLShaderProgram& shaderProgram)
{
  if (!enemies.empty())
  {
    for (auto& enemy : enemies)
    {
      auto vectors = enemyPath.getPositionAndTangent(enemy.m_progress);
      enemy.m_position = vectors.first;
      enemy.m_direction = normalize(vectors.second);
      enemy.draw(shaderProgram);
    }
  }

  auto model = rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f,
                      0.0f, 0.0f));
  auto normalMatrix = glm::mat3(model);
  shaderProgram.setUniformValue("model", model);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  floorPlane->draw(shaderProgram);

  for (auto& tower : towers)
    tower.draw(shaderProgram, glfwGetTime());
}

void setupEnemyPath()
{
  std::array<glm::vec3, 4> p1 =
  {
    glm::vec3(25.0f, 0.0f, -25.0f),
    glm::vec3(14.0f, 0.0f, -10.0f),
    glm::vec3(6.0f, 0.0f, 1.0f),
    glm::vec3(-10.0f, 0.0f, -8.0f)
  };

  std::array<glm::vec3, 2> p2 =
  {
    glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(-25.0f, 0.0f, 25.0f)
  };

  GLTools::GLCurves c1(p1);
  GLTools::GLCurves c2(c1, p2);

  c1.initialize();
  c2.initialize();

  enemyPath.m_data.push_back(c1);
  enemyPath.m_data.push_back(c2);
}

int main()
{
  init();
  setupEnemyPath();

  auto shaderProgram = std::make_unique<GLTools::GLShaderProgram>();
  auto pathProgram = std::make_unique<GLTools::GLShaderProgram>();
  setupShaders(*shaderProgram, *pathProgram);

  auto defaultMaterial = std::make_unique<GLTools::GLMaterial>();

  auto base = std::make_unique<GLTools::GLModel>("models/turret_base.obj");
  auto cannon = std::make_unique<GLTools::GLModel>("models/turret_cannon.obj");
  auto missile = std::make_unique<GLTools::GLModel>("models/missile.obj");
  enemy = std::make_unique<GLTools::GLModel>("models/enemyship.obj");

  base->m_materials[0] = *defaultMaterial;
  cannon->m_materials[0] = *defaultMaterial;

  targetShip = std::make_unique<Enemy>(enemy.get(), target, targetDir);

  floorPlane = std::make_unique<GLTools::GLPlane>(50.0f, 50.0f);
  floorPlane->initialize();
  floorPlane->m_material = defaultMaterial.get();

  towers.emplace_back(base.get(), cannon.get(), missile.get());
  towers.back().setPosition(glm::vec3(10.0f, 0.0f, 1.0f));

  towers.emplace_back(base.get(), cannon.get(), missile.get());
  towers.back().setPosition(glm::vec3(-10.0f, 0.0f, 1.0f));

  pointLight.setEnergy(2.0f);

  auto projection =
    glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 100.0f);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    moveCamera();

    auto VP = projection * camera.m_viewMatrix;

    shaderProgram->use();
    shaderProgram->setUniformValue("viewProjection", VP);
    shaderProgram->setUniformValue("camPos", camera.m_position);
    pointLight.setShaderUniform(*shaderProgram);

    renderScene(*shaderProgram);

    pathProgram->use();
    pathProgram->setUniformValue("MVP", VP);
    enemyPath.draw();

    glfwSwapBuffers(window);

    if (!enemies.empty())
    {
      checkHitsAndCleanupMissiles();
      cleanupEnemies();
    }
  }

  glfwTerminate();

  return 0;
}
