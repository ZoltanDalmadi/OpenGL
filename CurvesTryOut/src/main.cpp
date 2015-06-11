#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//A görbe kipróbálása, illettve tárgyak görbe mentén lévõ mozgatásának kipróbálására készült project

// CONSTANTS --------------------------------------------------------------
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;


// GLOBAL VARIABLES -------------------------------------------------------
GLFWwindow * window;

void init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Curves", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(){

	init();

	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
