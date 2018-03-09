//#define GLFW_INCLUDE_GLEXT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "SceneManager.h"
GLFWwindow* window;

void error_callback(int error, const char* description)
{
	// Print error
	fputs(description, stderr);
}

void setup_callbacks()
{
	// Set the error callback
	glfwSetErrorCallback(error_callback);
	
}

void setup_glew()
{

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
	}
	fprintf(stdout, "Current GLEW version: %s\n", glewGetString(GLEW_VERSION));
}

void setup_opengl_settings()
{

	setup_glew();
	// Enable depth buffering
	glEnable(GL_DEPTH_TEST);
	// Related to shaders and z value comparisons for the depth buffer
	glDepthFunc(GL_LEQUAL);
	// Set polygon drawing mode to fill front and back of each polygon
	// You can also use the paramter of GL_LINE instead of GL_FILL to see wireframes
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Disable backface culling to render both sides of polygons
	glDisable(GL_CULL_FACE);
	// Set clear color
	glClearColor(0.05f, 0.8f, 0.85f, 1.0f);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	//If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
}

int main(void)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return 0;
	}

	//create window
	if (!SceneManager::createWindow("Alex Lui", 1920, 1080)) {
		exit(-1);
	}

	// Print OpenGL and GLSL versions
	print_versions();
	// Setup OpenGL settings, including lighting, materials, etc.
	setup_opengl_settings();

	SceneManager::initObjects();

	// Loop while GameManager window is open
	while (SceneManager::isWindowOpen())
	{
		
		// Main render draw Rendering of objects is done here.
		SceneManager::draw();

		// Updating objects, etc. can be done here.
		SceneManager::update();
	}

	SceneManager::dispose();

	// Terminate GLFW
	glfwTerminate();

	exit(EXIT_SUCCESS);
}