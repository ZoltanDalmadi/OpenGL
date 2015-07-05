#include <glm/gtc/matrix_transform.hpp>
#include "Tower.h"
#include <iostream>

#define Y_AXIS glm::vec3(0.0f, 1.0f, 0.0f)
#define Z_AXIS glm::vec3(0.0f, 0.0f, 1.0f)

Tower::Tower(GLTools::GLModel *base, GLTools::GLModel *cannon,
             GLTools::GLModel *missile)
  : m_base(base),
    m_cannon(cannon),
    m_missile(missile),
    m_baseAngle(0.0f),
    m_cannonAngle(0.0f)
{}

Tower::~Tower()
{}

const glm::vec3& Tower::getPosition() const
{
  return m_position;
}

void Tower::setPosition(const glm::vec3& pos)
{
  m_position = pos;
  m_modelMatrix = translate(glm::mat4(), m_position);
  m_cannonMatrix = translate(m_modelMatrix, m_offset);
}

const glm::vec3& Tower::getOffset() const
{
  return m_offset;
}

void Tower::setOffset(const glm::vec3& offset)
{
  m_offset = offset;
}

float Tower::getCannonLength() const
{
  return m_cannonLength;
}

void Tower::setCannonLength(float cannon_length)
{
  m_cannonLength = cannon_length;
}

void Tower::setTarget(glm::vec3 *target)
{
  m_target = target;
}

void Tower::clearTarget()
{
  m_target = nullptr;
  m_modelMatrix = translate(glm::mat4(), m_position);
  m_cannonMatrix = translate(m_modelMatrix, m_offset);
}

float Tower::getRange() const
{
  return m_range;
}

void Tower::setRange(float range)
{
  m_range = range;
}

float Tower::getDamage() const
{
  return m_damage;
}

void Tower::setDamage(float damage)
{
  m_damage = damage;
}

void Tower::shoot(const glm::vec3& pos, const glm::vec3& dir)
{
  m_missiles.emplace_back(m_missile, pos, dir);
}

void Tower::draw(const GLTools::GLShaderProgram& shaderProgram, double time,
	bool inTower, const GLTools::GLShaderProgram& fireProgram, GLuint nParticles,
	GLuint initVel, GLuint particles)
{
  this->update(time, inTower);

  shaderProgram.setUniformValue("model", m_modelMatrix);
  auto normalMatrix = glm::mat3(m_modelMatrix);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  m_base->draw(shaderProgram);

  shaderProgram.setUniformValue("model", m_cannonMatrix);
  normalMatrix = glm::mat3(m_cannonMatrix);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  m_cannon->draw(shaderProgram);

  int size = nParticles * 3 * sizeof(float);
  for (auto& missile : m_missiles)
  {
	  shaderProgram.use();
	  missile.draw(shaderProgram);

	  fireProgram.use();
	  float Time = glfwGetTime() - missile.time;
	  fireProgram.setUniformValue("Time", Time);
	  glm::mat4 model;
	  model *= missile.m_modelMatrix;
	  model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
	  model = glm::rotate(model, glm::radians(180.0f), Y_AXIS);
	  fireProgram.setUniformValue("model", model);

	  glm::vec3 v(0.0f, 0.0f, 0.0f);
	  float velocity, theta, phi;
	  GLfloat *data = new GLfloat[nParticles * 3];
	  for (unsigned int i = 0; i < nParticles; i++) {

		  theta = glm::mix(0.0f, (float)PI / 20.0f, randFloat());
		  phi = glm::mix(0.0f, (float)TWOPI * 2, randFloat());

		  v.x = cosf(theta);
		  v.y = sinf(theta) * cosf(phi);
		  v.z = sinf(theta) * sinf(phi);

		  velocity = glm::mix(0.25f, 0.5f, randFloat());
		  v = glm::normalize(v) * velocity;

		  data[3 * i] = v.x;
		  data[3 * i + 1] = v.y;
		  data[3 * i + 2] = v.z;
	  }

	  glBindBuffer(GL_ARRAY_BUFFER, initVel);
	  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	  glBindVertexArray(particles);
	  glDrawArrays(GL_POINTS, 0, nParticles);
	  shaderProgram.use();
  }
}

void Tower::update(double time, bool inTower)
{
  if (!m_target)
    return;

  auto v = *m_target - m_position;
  m_baseAngle = -glm::atan(v.z, v.x);

  m_modelMatrix = translate(glm::mat4(), m_position);
  m_modelMatrix = rotate(m_modelMatrix, m_baseAngle, Y_AXIS);

  auto u = glm::vec3(glm::vec4(v - m_offset, 1.0f) * m_modelMatrix);
  m_cannonAngle =
    glm::clamp(glm::atan(u.y, u.x), glm::radians(-5.0f), glm::radians(49.0f));

  m_cannonMatrix = translate(m_modelMatrix, m_offset);
  m_cannonMatrix = rotate(m_cannonMatrix, m_cannonAngle, Z_AXIS);

  if (!inTower)
  {
    m_deltaTime += time - m_lastShotTime;

    if (m_deltaTime >= m_coolDown)
    {
      auto missilePos =
        glm::vec3(m_cannonMatrix * glm::vec4(m_cannonLength, 0.0f, 0.0f, 1.0f));
      auto missileDir = normalize(*m_target - missilePos);
      shoot(missilePos, missileDir);
      m_deltaTime = 0.0f;
      m_lastShotTime = time;
    }

  }
}

float Tower::randFloat() {
	return ((float)rand() / RAND_MAX);
}