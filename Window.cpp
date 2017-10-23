#include "window.h"
const char* window_title = "GLFW Starter Project";
GLint shaderProgram;
GLint directionalLightProgram;
GLint pointLightProgram;
GLint spotLightProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"


// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

bool Window::isRightMouseButtonDown;
bool Window::isLeftMouseButtonDown;
int Window::width;
int Window::height;
int Window::currModel;
int Window::currLight;

glm::vec2 Window::mousePosition;
glm::vec2 Window::lastMousePosition;
glm::vec3 Window::currPoint;
glm::vec3 Window::lastPoint;

OBJObject* Window::models;
Light* Window::sceneLights;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	isLeftMouseButtonDown = false;
	isRightMouseButtonDown = false;
	currModel = Window::BUNNY;
	models = new OBJObject[3]{ OBJObject("bunny.obj") , OBJObject("bear.obj") , OBJObject("dragon.obj") };
	
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	directionalLightProgram = LoadShaders(VERTEX_SHADER_PATH, "../shader_directionalLight.frag");

	//set up lights
	sceneLights = new Light[3];
	sceneLights[Light::DIRECTIONAL].setAsDirectionalLight( glm::vec3(1, 0, 1), glm::normalize(glm::vec3(-1, -1, -1)));
	/*
	sceneLights[Light::POINT].setAsPointLight(glm::vec3(0.75f, 0.75f, 0.75f), glm::vec3(2, 2, 2));
	sceneLights[Light::SPOT].setAsSpotLight(glm::vec3(0.75f, 0.75f, 0.75f),
											glm::vec3(2, 2, 2),
											glm::normalize(glm::vec3(-1, -1, -1)),
											1.0f,
											1);
	*/
	currLight = Light::DIRECTIONAL;
	
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete[] models;
	glDeleteProgram(shaderProgram);
	glDeleteProgram(directionalLightProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	models[currModel].update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	// Use the shader of programID
	glUseProgram(directionalLightProgram); // call lights' update() function here instead
	
	// Render the cube
	models[currModel].draw(directionalLightProgram);
	*/

	sceneLights[Light::DIRECTIONAL].update();
	models[currModel].draw(sceneLights[Light::DIRECTIONAL].shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		// Check which key was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (key == GLFW_KEY_1) {
			currModel = Window::BUNNY;
			std::cout << "bunny model, currModel = " << currModel << std::endl;
		}
		else if (key == GLFW_KEY_2) {
			currModel = Window::BEAR;
			std::cout << "bear model, currModel = " << currModel << std::endl;
		}
		else if (key == GLFW_KEY_3) {
			currModel = Window::DRAGON;
			std::cout << "dragon model, currModel = " << currModel << std::endl;
		}

		//Scale
		if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			if (mods & GLFW_MOD_SHIFT) {
				models[currModel].incrementScale(0.5f);
			}
			else {
				models[currModel].incrementScale(-0.5f);
			}
		}

		//Reset Position
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {

			models[currModel].setPosition(glm::vec3(0, 0, 0));
			models[currModel].setZoomVal(0);
		}
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			isLeftMouseButtonDown = true;
			lastPoint = trackBallMap(glm::vec2(mousePosition.x, mousePosition.y));
		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			isRightMouseButtonDown = true;
			lastMousePosition = mousePosition;
		}
	}
	if (action == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			isLeftMouseButtonDown = false;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			isRightMouseButtonDown = false;
		}
	}
}
void Window:: cursor_position_callback(GLFWwindow * window, double xpos, double ypos) {
	
	//define mousePosition for other functions to use
	mousePosition.x = (float)xpos;
	mousePosition.y = (float)ypos;

	glm::vec3 direction;
	

	//if left mouse button is down, do trackball rotation
	if (isLeftMouseButtonDown) {
		currPoint = trackBallMap(glm::vec2(mousePosition.x, mousePosition.y));
		direction = currPoint - lastPoint;
		float velocity = glm::length(direction);
		if (velocity > 0.0001f) {			

			glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
			GLfloat rotAngle = acos(glm::dot(lastPoint, currPoint));	
			if(!isnan(rotAngle))
				models[currModel].updateTrackBallRotate(glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis));
		}
	}

	//if right mouse button is down, translate model
	if (isRightMouseButtonDown) {
		glm::vec2 deltaMousePosition;
		deltaMousePosition.x = (mousePosition.x - lastMousePosition.x) * .033f;
		deltaMousePosition.y = (lastMousePosition.y - mousePosition.y) * .033f;
		models[currModel].move(glm::vec3(deltaMousePosition.x, deltaMousePosition.y, 0));
	}


	lastPoint = currPoint;
	lastMousePosition = mousePosition;
}

void Window::mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset) {
	//models[currModel].move(glm::vec3(0,0, yoffset));
	models[currModel].zoomModel((float)yoffset);

}

glm::vec3 Window::trackBallMap(glm::vec2 point) {


	glm::vec3 v;
	GLfloat d;
	v.x = (2.0f * point.x - Window::width) / Window::width;
	v.y = (Window::height - 2.0f * point.y) / Window::height;
	v.z = 0;
	d = glm::length(v);
	if (d > 1.0f)
		d = 1.0f;
	v.z = sqrt(1.001f - d*d);
	v = glm::normalize(v);
	return v;
}
