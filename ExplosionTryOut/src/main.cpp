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
#include <Windows.h>
#include <SOIL\SOIL.h>

#define PI 3.141592653589793
#define TWOPI 6.2831853071795862
GLuint initVel, startTime, particles;
GLuint nParticles;
float time;

auto shaderProgram = std::make_unique<GLTools::GLShaderProgram>();
GLuint texture;

glm::mat4 projection;

std::unique_ptr<GLTools::GLModel> enemy;
//float time = -1.57;
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
		if (time2 < 1.0)
		time2 += 0.01;
		std::cout << time2 << std::endl;
	}
	if (keys[GLFW_KEY_N])
	{
		if (time2 > 0.0)
		time2 -= 0.01;
		std::cout << time2 << std::endl;
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

void update(float t)
{
	time = t;
}

float randFloat() {
	return ((float)rand() / RAND_MAX);
}

void initBuffers()
{
	nParticles = 8000;

	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, (float)PI / 6.0f, randFloat());
		phi = glm::mix(0.0f, (float)TWOPI, randFloat());

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(1.25f, 1.5f, randFloat());
		v = glm::normalize(v) * velocity;

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPointSize(10.0f);

	initBuffers();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;
	unsigned char *image =
		SOIL_load_image("textures/bluewater.bmp", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	shaderProgram->setUniformValue("ParticleTex", 0);
	shaderProgram->setUniformValue("ParticleLifetime", 3.5f);
	shaderProgram->setUniformValue("Gravity", glm::vec3(0.0f, -0.2f, 0.0f));
}

void renderScene(const GLTools::GLShaderProgram& shaderProgram)
{
	auto model = glm::mat4(1.0f);
	auto normalMatrix = glm::mat3(model);
	shaderProgram.setUniformValue("MVP", projection * camera.m_viewMatrix * model);
	shaderProgram.setUniformValue("camPos", camera.m_position);

	shaderProgram.setUniformValue("Time", time);

	glBindVertexArray(particles);
	glDrawArrays(GL_POINTS, 0, nParticles);
}

int main()
{
	init();

	setupShaders(*shaderProgram);

	shaderProgram->use();

	/*auto defaultMaterial = std::make_unique<GLTools::GLMaterial>();

	enemy = std::make_unique<GLTools::GLModel>("models/enemyship.obj");*/

	//pointLight.setEnergy(2.0f);

	projection =
		glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 500.0f);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		update(glfwGetTime());
		moveCamera();

		renderScene(*shaderProgram);		

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
