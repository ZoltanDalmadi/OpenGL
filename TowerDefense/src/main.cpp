#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <memory>
#include <iostream>
#include <Windows.h>

#include "GLFPSCamera.h"
#include "GLModel.h"
#include "GLPointLight.h"
#include "GLCurvePath.h"
#include "GLBoundingBox.h"
#include "Tower.h"
#include "Enemy.h"
#include "Grid.h"
#include <GLSkyBox.h>
#include "GLText.h"
#include "Laser.h"

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

std::vector<const char *> skyBoxFaces;

std::vector<glm::vec3> closeSquares;
bool keys[1024];

// for mouse
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;
bool firstMouse = true;

std::vector<glm::vec3> leserPoints;

GLTools::GLCurvePath enemyPath;

std::unique_ptr<Grid> grid;
std::unique_ptr<GLTools::GLModel> enemy;
std::unique_ptr<GLTools::GLBoundingBox> boundingBox;
std::unique_ptr<GLTools::GLSkyBox> skyBox;
std::unique_ptr<GLTools::GLTexture> cubemapTexture;

std::unique_ptr<GLTools::GLModel> base;
std::unique_ptr<GLTools::GLModel> cannon;
std::unique_ptr<GLTools::GLModel> missile;

Laser laserObject;

glm::mat4 projection;

auto maxTower = 5;
auto actualTower = 0;
auto forbiddenPlace = false;
auto inTower = -1;
glm::vec3 target1(5.0f, 5.0f, 5.0f);

bool lezer = false;

bool drawBoundingBox = false;

GLTools::GLText text("fonts/arial.ttf");

auto atert = 0;
auto lelove = 0;
float levelUp = 0.0f;
bool lup = false;

//fire
GLuint initVel, startTime, particles;
GLTools::GLVertexArrayObject fire_VAO;
GLuint nParticles;
std::unique_ptr<GLTools::GLTexture> fireTexture;

void addNewEnemy(GLTools::GLModel *enemyModel)
{
  auto startVectors = enemyPath.getPositionAndTangent(0.0f);
  enemies.emplace_back(enemyModel, startVectors.first, startVectors.second);
}

void clearPreviousTarget()
{
  if (inTower > -1)
  {
    auto front = towers.begin();

    std::advance(front, inTower);

    front->clearTarget();
  }
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

  if (key == GLFW_KEY_R && action == GLFW_PRESS)
  {
    clearPreviousTarget();
    inTower = -1;
  }

  if (key == GLFW_KEY_T && action == GLFW_PRESS)
  {
    clearPreviousTarget();

    if (inTower == towers.size() - 1)
      inTower = -1;

    inTower++;

    auto front = towers.begin();

    std::advance(front, inTower);

    camera.m_position = front->getPosition() + glm::vec3(0.0f, 2.0f, 0.0f);
    front->setTarget(&target1);
  }

}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mod)
{
  /*Curve */
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
  {
    if (actualTower < maxTower)
    {
      if (forbiddenPlace)
      {
        PlaySound("sfx/error.WAV", NULL, SND_ASYNC);
      }
      else
      {
        if (actualTower + 1 != maxTower)
        {
          closeSquares.emplace_back(towers.back().getPosition());
          towers.emplace_back(base.get(), cannon.get(), missile.get());
        }

        actualTower++;
      }
    }
    else if (inTower > -1)
    {
      auto actual = towers.begin();

      std::advance(actual, inTower);

      auto towerPos = actual->getPosition();
      actual->shoot(towerPos,
                    glm::normalize(target1 - towerPos));
    }
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
  {
    lezer = true;

    auto actual = towers.begin();

    std::advance(actual, inTower);

    laserObject.m_data[0] = actual->getPosition();
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
  {
    lezer = false;
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
                  GLTools::GLShaderProgram& skyBoxProgram,
                  GLTools::GLShaderProgram& textProgram,
				  GLTools::GLShaderProgram& fireProgram)
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

  auto textVertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER,
     "shaders/text_vertex_shader.glsl");
  std::cout << textVertexShader->log() << std::endl;

  auto textFragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/text_fragment_shader.glsl");
  std::cout << textFragmentShader->log() << std::endl;

  textProgram.create();
  textProgram.addShader(textVertexShader);
  textProgram.addShader(textFragmentShader);
  textProgram.link();
  std::cout << textProgram.log() << std::endl;

  auto fireVertexShader =
	  std::make_shared<GLTools::GLShader>
	  (GLTools::GLShader::shaderType::VERTEX_SHADER,
	  "shaders/fire_vertex_shader.glsl");
  std::cout << fireVertexShader->log() << std::endl;

  auto fireFragmentShader =
	  std::make_shared<GLTools::GLShader>
	  (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
	  "shaders/fire_fragment_shader.glsl");
  std::cout << fireFragmentShader->log() << std::endl;

  fireProgram.create();
  fireProgram.addShader(fireVertexShader);
  fireProgram.addShader(fireFragmentShader);
  fireProgram.link();
  std::cout << fireProgram.log() << std::endl;
}

void init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  window = glfwCreateWindow(WIDTH, HEIGHT, "TowerDefense", nullptr
                            /*glfwGetPrimaryMonitor()*/, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetMouseButtonCallback(window, mouse_button_callback);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glPolygonOffset(1, 0);
  glLineWidth(2);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
          {
            it = missiles.erase(it);
          }
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
    if (it->m_progress >= 1.0f)
      atert++;

    if (it->isDestroyed())
      lelove++;

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

void renderScene(const GLTools::GLShaderProgram& shaderProgram,
	GLTools::GLShaderProgram& textProgram, GLTools::GLShaderProgram& fireProgram)
{
  if (!enemies.empty())
  {
    for (auto& enemy : enemies)
    {
      auto vectors = enemyPath.getPositionAndTangent(enemy.m_progress);
      enemy.m_position = vectors.first;
      enemy.m_direction = normalize(vectors.second);
      enemy.draw(shaderProgram);
      auto targetVector = enemy.m_position - camera.m_position;
      targetVector.y = 0.0f;
      auto angle = glm::acos(glm::dot(glm::vec3(0.0f, 0.0f, -1.0f),
                                      glm::normalize(targetVector)));

      auto model1 = glm::mat4(1.0f);
      model1 *= glm::translate(enemy.m_position);
      model1 *= glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));

      textProgram.use();
      textProgram.setUniformValue("projection",
                                  projection * camera.m_viewMatrix *
                                  model1/*projection2*/);
      text.color = glm::vec3(0.5, 0.8f, 0.2f);
      std::string percent = std::to_string((int)enemy.m_hitPoints) + "%";
      text.draw(textProgram, percent, -1.0f, 1.1f, 0.005f);

      shaderProgram.use();

      if (drawBoundingBox)
      {
        auto bb = calcBoundingBox(enemy.m_minPoint, enemy.m_maxPoint);
        boundingBox->draw(shaderProgram, bb.first,
                          bb.second, enemy.m_modelMatrix);
      }
    }
  }

  float d = 0.0f;
  bool intersect = glm::intersectRayPlane(camera.m_position, -camera.m_front,
                                          glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), d);

  auto temp = -camera.m_front * d;
  temp.y = 0.0f;

  if (actualTower < maxTower)
  {
    glm::vec3 temp2;
    auto found = grid->getCenter(temp, temp2);
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
	towers.back().draw(shaderProgram, glfwGetTime(), false, fireProgram, nParticles, initVel, particles);
    shaderProgram.setUniformValue("transparent", false);
    shaderProgram.setUniformValue("forbiddenTower", false);
  }

  int ciklusi = 0;

  for (auto& tower : towers)
  {
	  fireTexture->bind(0);
    if (ciklusi == inTower)
      tower.draw(shaderProgram, glfwGetTime(), true, fireProgram, nParticles, initVel, particles);
    else
		tower.draw(shaderProgram, glfwGetTime(), false, fireProgram, nParticles, initVel, particles);

    ciklusi++;
  }
}

void initBuffer()
{
	nParticles = 8000;

	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f, 0.0f, 0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	// Attach these to the torus's vertex array
	glGenVertexArrays(1, &particles);
	glBindVertexArray(particles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

}

void renderSkyBox(const GLTools::GLShaderProgram& shaderProgram,
                  const glm::mat4& projMatrix)
{
  glDepthFunc(GL_LEQUAL);
  shaderProgram.use();
  auto skyBoxView = glm::mat4(glm::mat3(camera.m_viewMatrix));
  shaderProgram.setUniformValue("view", skyBoxView);
  shaderProgram.setUniformValue("projection", projMatrix);
  cubemapTexture->bind();
  skyBox->draw();
  cubemapTexture->unbind();
  glDepthFunc(GL_LESS);
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
  grid->alpha = 0.2f;

  auto OneOverDetail = 1.0f / float(100.0f - 1.0f);

  for (auto i = 0; i < 100; i++)
  {
    auto t = i * OneOverDetail;
    glm::vec3 point;

    if (grid->getCenter(enemyPath.getPositionAndTangent(t).first, point))
    {
      if (find(closeSquares.begin(), closeSquares.end(), point)
          == closeSquares.end())
        closeSquares.push_back(point);
    }
  }
}

int main()
{
  init();
  setupSkyBox();
  setupEnemyPath();
  initGrid();
  initBuffer();

  auto shaderProgram = std::make_unique<GLTools::GLShaderProgram>();
  auto pathProgram = std::make_unique<GLTools::GLShaderProgram>();
  auto gridProgram = std::make_unique<GLTools::GLShaderProgram>();
  auto skyBoxProgram = std::make_unique<GLTools::GLShaderProgram>();
  auto textProgram = std::make_unique<GLTools::GLShaderProgram>();
  auto fireProgram = std::make_unique<GLTools::GLShaderProgram>();
  setupShaders(*shaderProgram, *pathProgram, *gridProgram, *skyBoxProgram,
               *textProgram, *fireProgram);

  auto defaultMaterial = std::make_unique<GLTools::GLMaterial>();

  fireTexture = std::make_unique<GLTools::GLTexture>("textures/fire.png");

  enemy = std::make_unique<GLTools::GLModel>("models/enemyship.obj");
  base = std::make_unique<GLTools::GLModel>("models/turret_base.obj");
  cannon = std::make_unique<GLTools::GLModel>("models/turret_cannon.obj");
  missile = std::make_unique<GLTools::GLModel>("models/missile.obj");

  boundingBox = std::make_unique<GLTools::GLBoundingBox>();
  boundingBox->initialize();

  base->m_materials[0] = *defaultMaterial;
  cannon->m_materials[0] = *defaultMaterial;

  towers.emplace_back(base.get(), cannon.get(), missile.get());
  towers.back().setPosition(grid->getCenter(glm::vec3(10.0f, 0.0f, 1.0f)));

  pointLight.setEnergy(2.0f);

  projection =
    glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 100.0f);

  laserObject = Laser();
  laserObject.initialize();

  text.initialize();
  glm::mat4 projection2 = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);

  fireProgram->use();
  fireProgram->setUniformValue("ParticleLifetime", 2.5f);
  fireProgram->setUniformValue("Gravity", glm::vec3(0.0f, 0.0f, 0.0f));
  shaderProgram->use();

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (inTower < 0)
      moveCamera();
    else
    {
      target1 = camera.m_front;
      target1 *= glm::vec3(-100.0f);
    }

    //moveCamera();

    auto VP = projection * camera.m_viewMatrix;

	fireProgram->use();
	fireProgram->setUniformValue("MVP", VP);

    shaderProgram->use();
    shaderProgram->setUniformValue("viewProjection", VP);
    shaderProgram->setUniformValue("camPos", camera.m_position);
    pointLight.setShaderUniform(*shaderProgram);

    renderScene(*shaderProgram, *textProgram, *fireProgram);


    gridProgram->use();
    gridProgram->setUniformValue("viewProjection", VP);
    grid->draw(*gridProgram);

    if (lezer)
    {
      gridProgram->setUniformValue("gridColor", glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
      laserObject.m_data[1] = target1;
      laserObject.commitDataChange();
      laserObject.draw(*gridProgram);
    }

    pathProgram->use();

    pathProgram->setUniformValue("MVP", VP);
    enemyPath.draw();

    renderSkyBox(*skyBoxProgram, projection);


    if (atert >= 3)
    {
      textProgram->use();
      textProgram->setUniformValue("projection", projection2);
      text.color = glm::vec3(1.0, 0.0f, 0.0f);
      text.draw(*textProgram, "You Lose!", (float)WIDTH / 5, (float)HEIGHT / 2 - 10,
                4.0f);
      glfwSwapBuffers(window);
      break;
    }

    if (lelove >= 2)
    {
      levelUp = glfwGetTime();
      lup = true;

      for (auto& enemy : enemies)
      {
        enemy.m_velocity += 0.0005;
      }

      lelove = 0;
    }

    if (lup)
    {
      if (glfwGetTime() - levelUp <= 3.0f)
      {
        textProgram->use();
        textProgram->setUniformValue("projection", projection2);
        text.color = glm::vec3(0.0, 0.0f, 1.0f);
        text.draw(*textProgram, "Level Up!", (float)WIDTH / 5, (float)HEIGHT / 2 - 10,
                  4.0f);
      }
      else
      {
        lup = false;
      }
    }

    glfwSwapBuffers(window);

    if (!enemies.empty())
    {
      checkHitsAndCleanupMissiles();
      cleanupEnemies();
    }
  }

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
