#include "SceneManager.h"
#include "Scene.h"
#include "SampleScene.h"
//Static members
GLFWwindow* SceneManager::window;
int SceneManager::windowWidth;
int SceneManager::windowHeight;
float SceneManager::currTime;
float SceneManager::prevTime;
Scene* SceneManager::currScene = NULL;

int SceneManager::createWindow(const char* title, int window_width, int window_height) {

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	windowWidth = window_width;
	windowHeight = window_height;
	
	// Create the GLFW window
	window = glfwCreateWindow(windowWidth, windowHeight, title, NULL, NULL);


	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return 0;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

	//setup callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, resize_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, mouse_wheel_callback);

	return 1;
}

void SceneManager::initObjects() {

	//init statics for classes which need them
	Material::initStatics();
	Asteroid::initStatics();
	
	prevTime = (float)glfwGetTime();
	
	//create scene
	currScene = new SampleScene();
	currScene->initObjects();

	// Call the resize callback to make sure things get drawn immediately
	resize_callback(window, windowWidth, windowHeight);
	
}
void SceneManager::dispose() {

	currScene->dispose();

	Asteroid::cleanUpStatics();
	Material::cleanUpStatics();

	glfwDestroyWindow(window);
}

void SceneManager::update() {
	currTime = (float)glfwGetTime();
	float deltaTime = currTime - prevTime;
	prevTime = currTime;
	

	currScene->update(deltaTime);
}
void SceneManager::draw() {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	currScene->draw();
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}


//callbacks
void SceneManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

	}
	currScene->key_event(key, scancode, action, mods);
}
void SceneManager::resize_callback(GLFWwindow* window, int width, int height) {
	
	SceneManager::windowWidth = width;
	SceneManager::windowHeight = height;

	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);
	

	currScene->resize_event(width, height);

}
void SceneManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	currScene->mouse_button_event(button, action, mods);
}
void SceneManager::cursor_position_callback(GLFWwindow * window, double xpos, double ypos) {
	currScene->cursor_position_event(xpos, ypos);
}
void SceneManager::mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset) {
	currScene->mouse_wheel_event(xoffset, yoffset);
}

bool SceneManager::isWindowOpen() {
	return !glfwWindowShouldClose(window);
}