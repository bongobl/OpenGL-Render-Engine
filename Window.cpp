#include "window.h"
const char* window_title = "GLFW Starter Project";

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"

//Shader Programs
GLint SkyboxShaderProgram;

// Camera parameters
float cam_dist(100.0f);
const glm::vec3 intial_cam_pos(0.0f, 0, cam_dist);
glm::vec3 cam_pos(0.0f, 0, cam_dist);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::currRotateMode;

int Window::width;
int Window::height;

//trackball variables
bool Window::isRightMouseButtonDown;
bool Window::isLeftMouseButtonDown;
glm::vec2 Window::mousePosition;
glm::vec2 Window::lastMousePosition;
glm::vec3 Window::currPoint;
glm::vec3 Window::lastPoint;
glm::mat4 Window::camRotationMatrix;


OBJObject* Window::skybox;

glm::mat4 Window::P;
glm::mat4 Window::V;

vector<string> faceNames;


//Robot Army array
int armyDimension = 30;
Robot** robotArmy;
glm::mat4 gridRotation(1.0f);
bool Window::drawBoundingSpheres;

void Window::initialize_objects()
{
	currRotateMode = Window::CAMERA;
	isLeftMouseButtonDown = false;
	isRightMouseButtonDown = false;
	camRotationMatrix = glm::mat4(1.0f);
	drawBoundingSpheres = false;

	// Load the shader program. Make sure you have the correct filepath up top
	SkyboxShaderProgram = LoadShaders(VERTEX_SHADER_PATH, "../shader_skybox.frag");

	//initialize skybox and put it at camera position
	skybox = new OBJObject("myCube.obj", SkyboxShaderProgram);
	skybox->setToWorld(glm::translate(glm::mat4(1.0f), cam_pos));

	faceNames.push_back("skybox/right.ppm");
	faceNames.push_back("skybox/left.ppm");
	faceNames.push_back("skybox/top.ppm");
	faceNames.push_back("skybox/bottom.ppm");
	faceNames.push_back("skybox/back.ppm");
	faceNames.push_back("skybox/front.ppm");

	CubeMapTexture cubeMap;
	cubeMap.loadCubeMapTexture(faceNames);
	

	Robot::initializeStatics();
	robotArmy = new Robot*[armyDimension * armyDimension];
	int startOffset = (armyDimension - 1) * 45;
	for (int i = 0; i < armyDimension; ++i) {
		for (int j = 0; j < armyDimension; ++j) {
			robotArmy[i * armyDimension + j] = new Robot(glm::vec3(i * 90 - startOffset, 0, j * - 90 + startOffset));
		}
	}
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete skybox;
	glDeleteProgram(SkyboxShaderProgram);
	for (int i = 0; i < armyDimension * armyDimension; ++i) {
		delete robotArmy[i];
	}
	delete robotArmy;
	Robot::cleanUpStatics();
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
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	//keep skybox position at camera position
	skybox->setToWorld(glm::translate(glm::mat4(1.0f), cam_pos));

	//update robot army
	for (int i = 0; i < armyDimension * armyDimension; ++i) {		
		robotArmy[i]->update();
	}
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	glDepthMask(GL_FALSE);
	skybox->draw();
	glDepthMask(GL_TRUE);

	
	for (int i = 0; i < armyDimension * armyDimension; ++i) {
		
		//implement culling algorithm here
		if (isRobotInFrustum(robotArmy[i]))
			robotArmy[i]->draw(gridRotation);
	}

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
		if (key == GLFW_KEY_A) {
			drawBoundingSpheres = false;
		}
		if (key == GLFW_KEY_S) {
			drawBoundingSpheres = true;
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
	if (isLeftMouseButtonDown || isRightMouseButtonDown) {
		currPoint = trackBallMap(glm::vec2(mousePosition.x, mousePosition.y));
		direction = currPoint - lastPoint;
		float velocity = glm::length(direction);
		if (velocity > 0.0001f) {

			glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
			GLfloat rotAngle = acos(glm::dot(lastPoint, currPoint)) * 1.0f;
			if (!isnan(rotAngle)) {
				if (isLeftMouseButtonDown) {
					camRotationMatrix = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis) * camRotationMatrix;
					cam_pos = camRotationMatrix * glm::vec4(intial_cam_pos.x, intial_cam_pos.y, cam_dist, 0);
					V = glm::lookAt(cam_pos, cam_look_at, cam_up);
				}
				if (isRightMouseButtonDown) {
					gridRotation = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis) * gridRotation;
				}
			}
		}
	}
	
	

	//if right mouse button is down, translate model
	if (isRightMouseButtonDown) {
		glm::vec2 deltaMousePosition;
		deltaMousePosition.x = (mousePosition.x - lastMousePosition.x) * .033f;
		deltaMousePosition.y = (lastMousePosition.y - mousePosition.y) * .033f;
	}

	lastPoint = currPoint;
	lastMousePosition = mousePosition;
}

void Window::mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset) {
	
	cam_dist += -10 * (float)yoffset;
	cam_pos = camRotationMatrix * glm::vec4(intial_cam_pos.x, intial_cam_pos.y, cam_dist, 0);
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

bool Window::isRobotInFrustum(Robot* robot) {


	//robot transform and sphere
	glm::vec3 robotPosition = gridRotation * robot->getBoundingSphereToRoot() * glm::vec4(0, 0, 0, 1);
	float robotSphereRadius = robot->getBoundingSphereRadius();
	
	//camera fields
	float angle_fov = glm::pi<float>() / 3;
	float aspect_ratio = (float)width / (float)height;

	//xyz components that differ slightly for each point
	float z = cos(angle_fov / 2) * -1;
	float y = sin(angle_fov / 2);
	float x = y * aspect_ratio;

	//define all 4 point locations on corners of camera screen
	glm::vec3 TR = glm::inverse(Window::V) * glm::vec4(x, y, z, 1);
	glm::vec3 BR = glm::inverse(Window::V) * glm::vec4(x, -y, z, 1);
	glm::vec3 TL = glm::inverse(Window::V) * glm::vec4(-x, y, z, 1);
	glm::vec3 BL = glm::inverse(Window::V) * glm::vec4(-x, -y, z, 1);


	//to re calculate for each plane
	glm::vec3 normal;
	float dist;

	//cull from top plane
	normal = glm::normalize(  glm::cross((TR - cam_pos), (TL - cam_pos))   );
	dist = glm::dot(robotPosition - cam_pos, normal);
	if(dist > robotSphereRadius)
		return false;

	//cull from bottom plane
	normal = glm::normalize( glm::cross((BL - cam_pos), (BR - cam_pos)));
	dist = glm::dot(robotPosition - cam_pos, normal);
	if (dist > robotSphereRadius)
		return false;

	//cull from right plane
	normal = glm::normalize(glm::cross((BR - cam_pos), (TR - cam_pos)));
	dist = glm::dot(robotPosition - cam_pos, normal);
	if (dist > robotSphereRadius)
		return false;

	//cull from left plane
	normal = glm::normalize(glm::cross((TL - cam_pos), (BL - cam_pos)));
	dist = glm::dot(robotPosition - cam_pos, normal);
	if (dist > robotSphereRadius)
		return false;

	//object is in camera frustum
	return true;
}
