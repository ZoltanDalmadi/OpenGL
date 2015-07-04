#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <memory>
#include <iostream>

#include "GLFPSCamera.h"
#include "GLModel.h"
#include "GLPointLight.h"
#include "GLPlane.h"
#include "GLBoundingBox.h"
#include "Tower.h"
#include "Enemy.h"
#include "GLTexture.h"

#define PI 3.141592653589793
#define TWOPI 6.2831853071795862

GLTools::GLVertexArrayObject fire_VAO;
GLuint nParticles = 800;

//constants
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

GLFWwindow *window;

GLTools::GLFPSCamera camera(glm::vec3(0.0f, 1.0f, -5.0f));
GLTools::GLPointLight pointLight(glm::vec3(0.0f, 10.0f, 0.0f));

glm::vec3 target(5.0f, 5.0f, 5.0f);
glm::vec3 targetDir(0.0f, 0.0f, 1.0f);

bool keys[1024];

// for mouse
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;
bool firstMouse = true;

std::unique_ptr<Tower> tower;
std::unique_ptr<Enemy> targetShip;
std::unique_ptr<GLTools::GLBoundingBox> boundingBox;
std::unique_ptr<GLTools::GLPlane> floorPlane;

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
		tower->clearTarget();

	if (key == GLFW_KEY_V && action == GLFW_PRESS)
		tower->setTarget(&target);
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

void setupShaders2(GLTools::GLShaderProgram& shaderProgram)
{
	auto vertexShader =
		std::make_shared<GLTools::GLShader>
		(GLTools::GLShader::shaderType::VERTEX_SHADER,
		"shaders/fire_vertex_shader.glsl");
	std::cout << vertexShader->log() << std::endl;

	auto fragmentShader =
		std::make_shared<GLTools::GLShader>
		(GLTools::GLShader::shaderType::FRAGMENT_SHADER,
		"shaders/fire_fragment_shader.glsl");
	std::cout << fragmentShader->log() << std::endl;

	shaderProgram.create();
	shaderProgram.addShader(vertexShader);
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(10.0f);

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

void renderScene(const GLTools::GLShaderProgram& shaderProgram)
{
	targetShip->draw(shaderProgram);
	auto aabb = targetShip->calculate_AABB();
	auto bounding = calcBoundingBox(aabb);
	boundingBox->draw(shaderProgram, bounding.first, bounding.second);

	auto model = rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f,
		0.0f, 0.0f));
	auto normalMatrix = glm::mat3(model);
	shaderProgram.setUniformValue("model", model);
	shaderProgram.setUniformValue("normalMatrix", normalMatrix);
	floorPlane->draw(shaderProgram);

	tower->draw(shaderProgram, glfwGetTime());
}

float randFloat() {
	return ((float)rand() / RAND_MAX);
}

void initBuffer()
{
	fire_VAO.create();
	GLTools::GLBufferObject initVel;
	GLTools::GLBufferObject startTime;
	initVel.create();
	startTime.create();

	fire_VAO.bind();

	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, (float)PI / 10.0f, randFloat());
		phi = glm::mix(0.0f, (float)TWOPI, randFloat());

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);
		//v += glm::vec3(0.0f, 1.0f, 0.0f);

		velocity = glm::mix(1.25f, 1.5f, randFloat());

		v = glm::normalize(-v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}

	initVel.bind();
	initVel.upload(data, nParticles * sizeof(glm::vec3));

	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}

	startTime.bind();
	startTime.upload(data, nParticles * sizeof(float));

	fire_VAO.setAttributeArray(0, 3, sizeof(float));
	fire_VAO.setAttributeArray(1, 1, sizeof(float));

	fire_VAO.unbind();
}

int main()
{
	init();
	initBuffer();

	auto shaderProgram = std::make_unique<GLTools::GLShaderProgram>();
	setupShaders(*shaderProgram);

	auto shaderProgram2 = std::make_unique<GLTools::GLShaderProgram>();
	setupShaders2(*shaderProgram2);

	shaderProgram->use();

	auto defaultMaterial = std::make_unique<GLTools::GLMaterial>();

	auto base = std::make_unique<GLTools::GLModel>("models/turret_base.obj");
	auto cannon = std::make_unique<GLTools::GLModel>("models/turret_cannon.obj");
	auto missile = std::make_unique<GLTools::GLModel>("models/missile.obj");
	auto enemy = std::make_unique<GLTools::GLModel>("models/enemyship.obj");

	glActiveTexture(GL_TEXTURE0);
	auto fireTexture = std::make_unique<GLTools::GLTexture>("textures/fire.png");

	base->m_materials[0] = *defaultMaterial;
	cannon->m_materials[0] = *defaultMaterial;

	targetShip = std::make_unique<Enemy>(enemy.get(), target, targetDir);

	boundingBox = std::make_unique<GLTools::GLBoundingBox>();
	boundingBox->initialize();

	floorPlane = std::make_unique<GLTools::GLPlane>(100.0f, 100.0f);
	floorPlane->initialize();
	floorPlane->m_material = defaultMaterial.get();

	tower = std::make_unique<Tower>(base.get(), cannon.get(), missile.get());
	tower->setPosition(glm::vec3(10.0f, 0.0f, 1.0f));

	pointLight.setEnergy(2.0f);

	auto projection =
		glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 50.0f);

	shaderProgram2->use();

	shaderProgram2->setUniformValue("ParticleLifetime", 1003.5f);
	shaderProgram2->setUniformValue("Gravity", glm::vec3(0.0f, 0.0f, 0.0f));

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

		fireTexture->bind(0);

		for (auto& missile : tower->m_missiles){
			shaderProgram->use();
			missile.draw(*shaderProgram);

			shaderProgram2->use();
			shaderProgram2->setUniformValue("Time", (float)glfwGetTime());
			shaderProgram2->setUniformValue("MVP", projection * camera.m_viewMatrix *missile.m_modelMatrix);

			fire_VAO.bind();
			glDrawArrays(GL_POINTS, 0, nParticles);
			fire_VAO.unbind();
		}
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
