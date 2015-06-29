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

std::unique_ptr<GLTools::GLModel> enemy;
float time = -1.57;
float explosionTime = 0.0f;
bool explosion = false;

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

void key_callback(GLFWwindow *window, int key, int scancode, int action,
	int mods)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		explosion = true;
		time = glfwGetTime();
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
	/*if (keys[GLFW_KEY_M])
	{
		time += 0.01;
		std::cout << time << std::endl;
	}
	if (keys[GLFW_KEY_N])
	{
		time -= 0.01;
	}*/
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

	auto geomtryShader =
		std::make_shared<GLTools::GLShader>
		(GLTools::GLShader::shaderType::GEOMETRY_SHADER,
		"shaders/geometry_shader.glsl");
	std::cout << geomtryShader->log() << std::endl;

	auto fragmentShader =
		std::make_shared<GLTools::GLShader>
		(GLTools::GLShader::shaderType::FRAGMENT_SHADER,
		"shaders/fragment_shader.glsl");
	std::cout << fragmentShader->log() << std::endl;

	shaderProgram.create();
	shaderProgram.addShader(vertexShader);
	shaderProgram.addShader(geomtryShader);
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
	glEnable(GL_CULL_FACE);
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
	auto model = glm::mat4(1.0f);
	auto normalMatrix = glm::mat3(model);
	shaderProgram.setUniformValue("model", model);
	shaderProgram.setUniformValue("normalMatrix", normalMatrix);

	if (glfwGetTime() - time < 3 && explosion)
	{
		explosionTime += 0.2;
		shaderProgram.setUniformValue("time", explosionTime);
	}

	enemy->draw(shaderProgram);
}

int main()
{
	init();

	auto shaderProgram = std::make_unique<GLTools::GLShaderProgram>();
	setupShaders(*shaderProgram);

	shaderProgram->use();

	auto defaultMaterial = std::make_unique<GLTools::GLMaterial>();

	enemy = std::make_unique<GLTools::GLModel>("models/enemyship.obj");

	pointLight.setEnergy(2.0f);

	auto projection =
		glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 500.0f);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		moveCamera();

		shaderProgram->setUniformValue("viewProjection",
			projection * camera.m_viewMatrix);
		shaderProgram->setUniformValue("camPos", camera.m_position);
		pointLight.setShaderUniform(*shaderProgram);

		renderScene(*shaderProgram);		

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
