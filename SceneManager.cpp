#include <iostream>
#include "SceneManager.h"
#include "Scene.h"
#include "SampleScene.h"
#include "ShadowMap.h"

//Basic Data
GLFWwindow* SceneManager::window;
int SceneManager::windowWidth;
int SceneManager::windowHeight;
float SceneManager::currTime;
float SceneManager::prevTime;
float SceneManager::deltaTime;
Scene* SceneManager::currScene = NULL;


//Gaussian Blur Data
GLuint SceneManager::blurShaderProgram;
GLuint SceneManager::frameBufferID;
Texture SceneManager::frameTexture;
GLuint SceneManager::renderBufferID;
GLenum SceneManager::drawBuffers[1];
GLuint SceneManager::VAO_ScreenQuad;
GLuint SceneManager::VBO_SceenQuadPositions;

GLfloat SceneManager::screenQuadVertexPositions[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,
};
GLuint SceneManager::EB0_ScreenQuad;
GLuint SceneManager::meshIndices[6] = {0,1,2,3,4,5};



//don't call any gl functions here, only glfw
int SceneManager::createWindow(const char* title, int window_width, int window_height) {

	// 8x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 8);

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
	ShadowMap::initStatics();

	prevTime = (float)glfwGetTime();
	
	//Init Gaussian Blur buffers and shader
	initFrameBufferObjects();

	//create scene
	currScene = new SampleScene();
	currScene->init();


	// Call the resize callback to make sure things get drawn immediately
	resize_callback(window, windowWidth, windowHeight);

}
void SceneManager::dispose() {

	currScene->dispose();

	frameTexture.disposeCurrentTexture();
	glDeleteBuffers(1, &renderBufferID);
	glDeleteBuffers(1, &frameBufferID);
	glDeleteBuffers(1, &VBO_SceenQuadPositions);
	glDeleteBuffers(1, &EB0_ScreenQuad);
	glDeleteVertexArrays(1, &VAO_ScreenQuad);
	glDeleteProgram(blurShaderProgram);

	ShadowMap::cleanUpStatics();
	Material::cleanUpStatics();

	glfwDestroyWindow(window);
}

void SceneManager::update() {
	currTime = (float)glfwGetTime();
	deltaTime = currTime - prevTime;
	prevTime = currTime;
	

	currScene->update();
}
void SceneManager::draw() {

	//calc shadow maps before doing any drawing
	currScene->calcShadowMaps();

	//set display matrix for window screen
	glViewport(0, 0, SceneManager::windowWidth, SceneManager::windowHeight);
	//draw scene to frame buffer's screen texture
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	currScene->draw();

	
	//Draw frame buffer's frame texture to window
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//use the blur shader program
	glUseProgram(blurShaderProgram);	

	//send frame buffer's frame texture to blur shader
	glUniform1i(glGetUniformLocation(blurShaderProgram, "texture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameTexture.getID());
	
	//send current camera's blur radius value to blur shader
	glUniform1f(glGetUniformLocation(blurShaderProgram, "blurRadius"), currScene->getActiveCamera()->getBlurValue());

	//bind and draw screen quad
	glBindVertexArray(VAO_ScreenQuad);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//unbind screen quad
	glBindVertexArray(0);
	

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
	
	resizeFrameBufferObjects();

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

float SceneManager::getDeltaTime(){
	return deltaTime;
}


//PRIVATE HELPERS

void SceneManager::initFrameBufferObjects() {

	//generate since they will be deleted during resize
	glGenFramebuffers(1, &frameBufferID);
	frameTexture.generatePlainTexture();
	glGenRenderbuffers(1, &renderBufferID);


	//Validate FBO
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		exit(1);
	}

	//Done setting up FBO and Frame Texture. Set current frame buffer to standard window
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//VAO
	glGenVertexArrays(1, &VAO_ScreenQuad);
	glBindVertexArray(VAO_ScreenQuad);

	//VBO
	glGenBuffers(1, &VBO_SceenQuadPositions);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_SceenQuadPositions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertexPositions), screenQuadVertexPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//EBO
	glGenBuffers(1, &EB0_ScreenQuad);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB0_ScreenQuad);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLint), meshIndices, GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//set up blur shader
	blurShaderProgram = LoadShaders("../shader_blur.vert", "../shader_blur.frag");

}


void SceneManager::resizeFrameBufferObjects() {
	
	//delete buffers
	frameTexture.disposeCurrentTexture();
	glDeleteBuffers(1, &renderBufferID);
	glDeleteBuffers(1, &frameBufferID);

	//FBO
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	//Frame texture
	frameTexture.generatePlainTexture();
	glBindTexture(GL_TEXTURE_2D, frameTexture.getID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Depth buffer
	glGenRenderbuffers(1, &renderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);
	
	//FBO config
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frameTexture.getID(), 0);
	drawBuffers[0] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	//Validate FBO
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		exit(1);
	}
}