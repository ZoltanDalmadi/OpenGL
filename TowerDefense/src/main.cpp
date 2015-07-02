#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/intersect.hpp>
#include <memory>
#include <iostream>

#include "GLFPSCamera.h"
#include "GLModel.h"
#include "GLPointLight.h"
#include "GLCurvePath.h"
#include "GLBoundingBox.h"
#include "Tower.h"
#include "Enemy.h"
#include "Grid.h"
#include <GLSkyBox.h>
#include <Windows.h>

//constants
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;
const GLuint MAX_TOWERS = 5;
const float ENEMY_FREQUENCY = 0.005f;

float enemyCounter = 0.0f;

GLFWwindow *window;

GLTools::GLFPSCamera camera(glm::vec3(0.0f, 1.0f, -5.0f));
GLTools::GLPointLight pointLight(glm::vec3(0.0f, 10.0f, 0.0f));

glm::vec3 target(5.0f, 5.0f, 5.0f);
glm::vec3 targetDir(0.01f, 0.0f, 1.0f);

std::list<Tower> towers;
std::list<Enemy> enemies;

std::vector<glm::vec3> lockedSquares;
bool keys[1024];

// for mouse
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;
bool firstMouse = true;

std::unique_ptr<Grid> grid;
GLTools::GLCurvePath enemyPath;

std::unique_ptr<Tower> dummyTower;

// MODELS
std::unique_ptr<GLTools::GLModel> base;
std::unique_ptr<GLTools::GLModel> cannon;
std::unique_ptr<GLTools::GLModel> missile;
std::unique_ptr<GLTools::GLModel> enemy;

std::unique_ptr<GLTools::GLBoundingBox> boundingBox;
std::unique_ptr<GLTools::GLSkyBox> skyBox;
std::vector<const char *> skyBoxFaces;
std::unique_ptr<GLTools::GLTexture> cubemapTexture;

bool drawBoundingBox = false;
bool towerPlacementMode = false;
bool cannotPlaceHere = false;
glm::vec3 placeHere;

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

  if (key == GLFW_KEY_E && action == GLFW_PRESS)
    addNewEnemy(enemy.get());

  if (key == GLFW_KEY_B && action == GLFW_PRESS)
    drawBoundingBox = !drawBoundingBox;

  if (key == GLFW_KEY_T && action == GLFW_PRESS)
    towerPlacementMode = !towerPlacementMode;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mod)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
  {
    if (towerPlacementMode)
    {
      if (towers.size() < MAX_TOWERS)
      {
        if (cannotPlaceHere)
          PlaySound("sfx/error.wav", nullptr, SND_ASYNC);
        else
          towers.emplace_back(base.get(), cannon.get(), missile.get(), placeHere);
      }
    }
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
                  GLTools::GLShaderProgram& pathProgram,
                  GLTools::GLShaderProgram& gridProgram,
                  GLTools::GLShaderProgram& skyBoxProgram)
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
     "shaders/path_vertex_shader.glsl");
  std::cout << pathVertexShader->log() << std::endl;

  auto pathGeometryShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::GEOMETRY_SHADER,
     "shaders/path_geometry_shader.glsl");
  std::cout << pathGeometryShader->log() << std::endl;

  auto pathFragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/path_fragment_shader.glsl");
  std::cout << pathFragmentShader->log() << std::endl;

  pathProgram.create();
  pathProgram.addShader(pathVertexShader);
  pathProgram.addShader(pathGeometryShader);
  pathProgram.addShader(pathFragmentShader);
  pathProgram.link();
  std::cout << pathProgram.log() << std::endl;

  auto gridVertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER,
     "shaders/grid_vertex_shader.glsl");
  std::cout << gridVertexShader->log() << std::endl;

  auto gridFragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/grid_fragment_shader.glsl");
  std::cout << gridFragmentShader->log() << std::endl;

  gridProgram.create();
  gridProgram.addShader(gridVertexShader);
  gridProgram.addShader(gridFragmentShader);
  gridProgram.link();
  std::cout << gridProgram.log() << std::endl;

  auto skyBoxVertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER,
     "shaders/skybox_vertex_shader.glsl");
  std::cout << skyBoxVertexShader->log() << std::endl;

  auto skyBoxFragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/skybox_fragment_shader.glsl");
  std::cout << skyBoxFragmentShader->log() << std::endl;

  skyBoxProgram.create();
  skyBoxProgram.addShader(skyBoxVertexShader);
  skyBoxProgram.addShader(skyBoxFragmentShader);
  skyBoxProgram.link();
  std::cout << skyBoxProgram.log() << std::endl;
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
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(2);
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

std::pair<glm::vec3, glm::vec3> calcBoundingBox
(const glm::vec3& minPoint, const glm::vec3& maxPoint)
{
  auto& minX = minPoint.x;
  auto& minY = minPoint.y;
  auto& minZ = minPoint.z;
  auto& maxX = maxPoint.x;
  auto& maxY = maxPoint.y;
  auto& maxZ = maxPoint.z;

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
  if (towerPlacementMode && towers.size() < MAX_TOWERS)
  {
    auto d = 0.0f;

    if (intersectRayPlane
        (camera.m_position, -camera.m_front, glm::vec3(0.0f),
         glm::vec3(0.0f, 1.0f, 0.0f), d))
    {
      auto intersectionPoint = -camera.m_front * d;
      intersectionPoint.y = 0.0f;

      placeHere = grid->getCenter(intersectionPoint);

      cannotPlaceHere = false;

      if (find(lockedSquares.begin(), lockedSquares.end(), placeHere) !=
          lockedSquares.end())
      {
        cannotPlaceHere = true;
        shaderProgram.setUniformValue("invalidPlacement", true);
      }

      shaderProgram.setUniformValue("placementMode", true);
      dummyTower->setPosition(placeHere);
      dummyTower->draw(shaderProgram);
      shaderProgram.setUniformValue("placementMode", false);
      shaderProgram.setUniformValue("invalidPlacement", false);
    }
  }
  else
    towerPlacementMode = false;

  if (!enemies.empty())
  {
    for (auto& enemy : enemies)
    {
      auto vectors = enemyPath.getPositionAndTangent(enemy.m_progress);
      enemy.m_position = vectors.first;
      enemy.m_direction = normalize(vectors.second);
      enemy.draw(shaderProgram);

      if (drawBoundingBox)
      {
        auto bb = calcBoundingBox(enemy.m_minPoint, enemy.m_maxPoint);
        boundingBox->draw(shaderProgram, bb.first,
                          bb.second, enemy.m_modelMatrix);
      }
    }
  }

  for (auto& tower : towers)
    tower.draw(shaderProgram, glfwGetTime());
}

void renderSkyBox(const GLTools::GLShaderProgram& shaderProgram,
                  const glm::mat4& projMatrix)
{
  glDepthMask(GL_FALSE);
  shaderProgram.use();
  auto skyBoxView = glm::mat4(glm::mat3(camera.m_viewMatrix));
  shaderProgram.setUniformValue("view", skyBoxView);
  shaderProgram.setUniformValue("projection", projMatrix);
  cubemapTexture->bind();
  skyBox->draw();
  cubemapTexture->unbind();
  glDepthMask(GL_TRUE);
}

void setupSkyBox()
{
  //cubemap textures
  skyBoxFaces.push_back("textures/nebula_right.png");
  skyBoxFaces.push_back("textures/nebula_left.png");
  skyBoxFaces.push_back("textures/nebula_top.png");
  skyBoxFaces.push_back("textures/nebula_bottom.png");
  skyBoxFaces.push_back("textures/nebula_back.png");
  skyBoxFaces.push_back("textures/nebula_front.png");

  skyBox = std::make_unique<GLTools::GLSkyBox>();
  skyBox->initialize();

  cubemapTexture = std::make_unique<GLTools::GLTexture>(skyBoxFaces.data());
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

void initGrid()
{
  grid = std::make_unique<Grid>(50.0f, 50.0f, 5.0f);
  grid->initialize();
  grid->color = glm::vec3(0.0f, 1.0f, 1.0f);
  grid->alpha = 0.1;

  auto OneOverDetail = 1.0f / float(100.0f - 1.0f);

  for (auto i = 0; i < 100; i++)
  {
    auto t = i * OneOverDetail;
    glm::vec3 point;

    if (grid->getCenter(enemyPath.getPositionAndTangent(t).first, point))
    {
      if (find(lockedSquares.begin(), lockedSquares.end(), point)
          == lockedSquares.end())
        lockedSquares.push_back(point);
    }
  }
}

int main()
{
  init();
  setupSkyBox();
  setupEnemyPath();
  initGrid();

  auto shaderProgram = std::make_unique<GLTools::GLShaderProgram>();
  auto pathProgram = std::make_unique<GLTools::GLShaderProgram>();
  auto gridProgram = std::make_unique<GLTools::GLShaderProgram>();
  auto skyBoxProgram = std::make_unique<GLTools::GLShaderProgram>();
  setupShaders(*shaderProgram, *pathProgram, *gridProgram, *skyBoxProgram);

  auto defaultMaterial = std::make_unique<GLTools::GLMaterial>();

  base = std::make_unique<GLTools::GLModel>("models/turret_base.obj");
  cannon = std::make_unique<GLTools::GLModel>("models/turret_cannon.obj");
  missile = std::make_unique<GLTools::GLModel>("models/missile.obj");
  enemy = std::make_unique<GLTools::GLModel>("models/enemyship.obj");

  boundingBox = std::make_unique<GLTools::GLBoundingBox>();
  boundingBox->initialize();

  base->m_materials[0] = *defaultMaterial;
  cannon->m_materials[0] = *defaultMaterial;

  dummyTower = std::make_unique<Tower>(base.get(), cannon.get(), missile.get());

  pointLight.setEnergy(2.0f);

  auto projection =
    glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 100.0f);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    moveCamera();

    renderSkyBox(*skyBoxProgram, projection);

    auto VP = projection * camera.m_viewMatrix;

    pathProgram->use();
    pathProgram->setUniformValue("VP", VP);
    enemyPath.draw();

    gridProgram->use();
    gridProgram->setUniformValue("viewProjection", VP);
    grid->draw(*gridProgram);

    shaderProgram->use();
    shaderProgram->setUniformValue("viewProjection", VP);
    shaderProgram->setUniformValue("camPos", camera.m_position);
    pointLight.setShaderUniform(*shaderProgram);

    renderScene(*shaderProgram);

    glfwSwapBuffers(window);

    if (!enemies.empty())
    {
      checkHitsAndCleanupMissiles();
      cleanupEnemies();
    }

    if (towers.size() == MAX_TOWERS)
      enemyCounter += ENEMY_FREQUENCY;

    if (enemyCounter >= 1.0f)
    {
      addNewEnemy(enemy.get());
      enemyCounter = 0.0f;
    }
  }

  glfwTerminate();

  return 0;
}
