#include "SceneManager.h"
#include "Scene.h"
#include "SampleScene.h"

float SceneManager::testFloat[1];
//Static members
GLFWwindow* SceneManager::window;
int SceneManager::windowWidth;
int SceneManager::windowHeight;
float SceneManager::currTime;
float SceneManager::prevTime;
Scene* SceneManager::currScene = NULL;


//FBOs
GLuint SceneManager::blurShader;
GLuint SceneManager::FramebufferName;
GLuint SceneManager::renderedTexture;
GLuint SceneManager::depthrenderbuffer;
GLenum SceneManager::DrawBuffers[1];
GLuint SceneManager::quad_VertexArrayID;
GLuint SceneManager::quad_vertexbuffer;

GLfloat SceneManager::g_quad_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,
};
GLuint SceneManager::quadEBO;
GLuint SceneManager::indices[6] = {0,1,2,3,4,5};



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
	
	prevTime = (float)glfwGetTime();
	
	//create scene
	currScene = new SampleScene();
	currScene->initObjects();

	// Call the resize callback to make sure things get drawn immediately
	resize_callback(window, windowWidth, windowHeight);

	
	//FBOS
	//fbo
	FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	//texture
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	// The depth buffer
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	//fbo config
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	DrawBuffers[0] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	//check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		exit(1);

	//set current frame buffer to standard one
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	//Screen QUAD

	//vao
	glGenVertexArrays(1, &quad_VertexArrayID);
	glBindVertexArray(quad_VertexArrayID);

	//vbo
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	//ebo
	glGenBuffers(1, &quadEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLint), indices, GL_STATIC_DRAW);
	
	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//blur shader
	blurShader = LoadShaders("../shader_blur.vert", "../shader_blur.frag");

	
}
void SceneManager::dispose() {

	currScene->dispose();
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


	//draw scene to frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	currScene->draw();
	
	
	//draw frame buffer to window
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	glUseProgram(blurShader);	
	glUniform1i(glGetUniformLocation(blurShader, "texture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	

	glBindVertexArray(quad_VertexArrayID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

GLuint SceneManager::getBlurShader() {
	return blurShader;
}