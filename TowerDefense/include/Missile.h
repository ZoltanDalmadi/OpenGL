#pragma once
#include "GLModel.h"
#include <GLFW/glfw3.h>

class Missile
{
public:
	explicit Missile(GLTools::GLModel *model, const glm::vec3& position,
		const glm::vec3& direction);
	virtual ~Missile();

	const glm::vec3& getPosition() const;
	void setPosition(const	glm::vec3& pos);

	const glm::vec3& getDirection() const;
	void setDirection(const glm::vec3& dir);

	float getVelocity() const;
	void setVelocity(float velocity);

	float getMaxVelocity() const;
	void setMaxVelocity(float max_velocity);

	float getAcceleration() const;
	void setAcceleration(float acceleration);

	float getSpin() const;
	void setSpin(float spin);

	float getSpinSpeed() const;
	void setSpinSpeed(float spin_speed);

	void draw(const GLTools::GLShaderProgram& shaderProgram);

	glm::mat4 m_modelMatrix;
	float time;

private:
	void update();
	GLTools::GLModel *m_model;

	// in world space
	glm::vec3 m_position;
	glm::vec3 m_direction;

	
	float m_velocity;
	float m_maxVelocity;
	float m_acceleration;
	float m_spin;
	float m_spinSpeed;
};
