#include "window.h"
const char* window_title = "GLFW Starter Project";

//main window
GLFWwindow* Window::mainWindow;

// Camera parameters
float cam_dist(30.0f);
glm::vec3 cam_pos(0.0f, 0, cam_dist);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is
glm::mat4 Window::camRotationMatrix;

int Window::width;
int Window::height;

//Time keeping
float currTime;
float prevTime;

//trackball variables
bool Window::isRightMouseButtonDown;
bool Window::isLeftMouseButtonDown;
glm::vec2 Window::mousePosition;
glm::vec2 Window::lastMousePosition;
glm::vec3 Window::currPoint;
glm::vec3 Window::lastPoint;

//Camera
glm::mat4 Window::P;
glm::mat4 Window::V;

//cubeMap
CubeMap* spaceView;

//Asteroid Field
AsteroidField* asteroidField;

//Ship
Ship* player;

void Window::initialize_objects()
{
	//Object that need statics initialized
	Material::initStatics();
	Asteroid::initStatics();

	//basic window
	isLeftMouseButtonDown = false;
	isRightMouseButtonDown = false;
	camRotationMatrix = glm::mat4(1.0f);
	prevTime = (float)glfwGetTime();

	//vector of skybox face names
	vector<string> faceNames;
	faceNames.push_back("skybox/left.ppm");
	faceNames.push_back("skybox/right.ppm");
	faceNames.push_back("skybox/top.ppm");
	faceNames.push_back("skybox/bottom.ppm");
	faceNames.push_back("skybox/front.ppm");
	faceNames.push_back("skybox/back.ppm");

	//init cubemap
	spaceView = new CubeMap();
	spaceView->loadCubeMapTexture(faceNames);


	//Asteroid Field
	asteroidField = new AsteroidField();

	//player
	player = new Ship();
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete player;
	delete spaceView;
	delete asteroidField;
	Asteroid::cleanUpStatics();
	Material::cleanUpStatics();
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

	mainWindow = window;
	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	//calc delta time
	currTime = (float)glfwGetTime();
	float deltaTime = currTime - prevTime;
	prevTime = currTime;


	spaceView->setPosition(cam_pos);
	
	
	asteroidField->setCenter(player->getPosition());
	
	spaceView->setPosition(player->getPosition());
	
	
	player->update(deltaTime,	glfwGetKey(mainWindow, GLFW_KEY_LEFT),
								glfwGetKey(mainWindow, GLFW_KEY_RIGHT),
								glfwGetKey(mainWindow, GLFW_KEY_UP),
								glfwGetKey(mainWindow, GLFW_KEY_DOWN));
								
	Window::V = glm::rotate(glm::mat4(1.0f), glm::pi<float>() , glm::vec3(0,1,0)) * glm::translate(glm::mat4(1.0f), glm::vec3(0,-1.5,7)) * glm::inverse(player->getGeneralMotionMatrix());
	
	asteroidField->update(deltaTime);
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	//draw skybox cubemap	
	//spaceView->draw();	
	//asteroidField->draw();
	player->draw();
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
			lastPoint = trackBallMap(glm::vec2(mousePosition.x, mousePosition.y));
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

	//If any mouse button is down, do trackball rotation
	if (isLeftMouseButtonDown) {
		currPoint = trackBallMap(glm::vec2(mousePosition.x, mousePosition.y));
		direction = currPoint - lastPoint;
		float velocity = glm::length(direction);
		if (velocity > 0.0001f) {

			glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
			GLfloat rotAngle = acos(glm::dot(lastPoint, currPoint)) * 1.0f;
			if (!isnan(rotAngle)) {		

				camRotationMatrix = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis) * camRotationMatrix;
				cam_pos = camRotationMatrix * glm::vec4(0, 0, cam_dist, 0);
				cam_look_at = camRotationMatrix * glm::vec4(0, 0, cam_dist - 1, 0);
				V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			}
		}
	}
	
	//if right mouse button is down, translate model
	if (isRightMouseButtonDown) {
		glm::vec2 deltaMousePosition;
		deltaMousePosition.x = (mousePosition.x - lastMousePosition.x) * 150.0f / width;
		deltaMousePosition.y = (lastMousePosition.y - mousePosition.y) * 150.0f / height;
		
	}

	//for trackball
	lastPoint = currPoint;

	//for screen translate
	lastMousePosition = mousePosition;
}

void Window::mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset) {
	
	
	cam_dist += -10 * (float)yoffset;

	cam_pos = camRotationMatrix * glm::vec4(0, 0, cam_dist, 0);
	cam_look_at = camRotationMatrix * glm::vec4(0, 0, cam_dist - 1, 0);
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	
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