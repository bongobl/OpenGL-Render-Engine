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

//Camera
glm::mat4 Window::P;
glm::mat4 Window::V;

//skybox
OBJObject* Window::skybox;
vector<string> faceNames;

//ControlPoints
int selectedControlPoint = -1;
ControlPoint* points;

//Bezier Curves
BezierCurve* testCurve;
BezierCurve* testCurve2;
BezierCurve* testCurve3;
//pixel data
unsigned char currPixelColor[4];


void Window::initialize_objects()
{
	isLeftMouseButtonDown = false;
	isRightMouseButtonDown = false;
	camRotationMatrix = glm::mat4(1.0f);


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

	

	//controlPointTest
	ControlPoint::InitStatics();
	points = new ControlPoint[10]{  ControlPoint(glm::vec3(1.0f / 255, 0, 1)),
									ControlPoint(glm::vec3(2.0f / 255, 1, 0)),
									ControlPoint(glm::vec3(3.0f / 255, 1, 0)),
									ControlPoint(glm::vec3(4.0f / 255, 0, 1)),
									ControlPoint(glm::vec3(5.0f / 255, 1, 0)),
									ControlPoint(glm::vec3(6.0f / 255, 1, 0)),
									ControlPoint(glm::vec3(7.0f / 255, 0, 1)), 
									ControlPoint(glm::vec3(8.0f / 255, 1, 0)),
									ControlPoint(glm::vec3(9.0f / 255, 1, 0)), 
									ControlPoint(glm::vec3(10.0f / 255, 0, 1)) };


	points[0].move(glm::vec3(-35, 0, 0));
	points[1].move(glm::vec3(-25, 0, 0));
	points[2].move(glm::vec3(-15, 0, 0));
	points[3].move(glm::vec3(-5, 0, 0));
	points[4].move(glm::vec3(5, 0, 0));
	points[5].move(glm::vec3(15, 0, 0));
	points[6].move(glm::vec3(25, 0, 0));
	points[7].move(glm::vec3(25, 0, 10));
	points[8].move(glm::vec3(25, 0, 20));
	points[9].move(glm::vec3(25, 0, 30));

	BezierCurve::InitStatics();
	testCurve = new BezierCurve(&points[0], &points[1], &points[2], &points[3]);
	testCurve2 = new BezierCurve(&points[3], &points[4], &points[5], &points[6]);
	testCurve3 = new BezierCurve(&points[6], &points[7], &points[8], &points[9]);

	

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete skybox;

	ControlPoint::cleanUpStatics();
	BezierCurve::cleanUpStatics();
	glDeleteProgram(SkyboxShaderProgram);

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
	
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	//draw skybox
	glDepthMask(GL_FALSE);
	skybox->draw(glm::vec3(0,0,0));
	glDepthMask(GL_TRUE);

	//draw control points
	for (int i = 0; i < 10; ++i)
		points[i].draw();
	
	//draw selected point
	if (selectedControlPoint != -1) {
		points[selectedControlPoint].drawAsSelected();
	}

	//draw Bezier Curve
	testCurve->draw();
	testCurve2->draw();
	testCurve3->draw();
	

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
			
			//read current pixel val
			glReadPixels((int)mousePosition.x, Window::height - (int)mousePosition.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, currPixelColor);
			cout << "(" << currPixelColor[0] - 1 << ", " << currPixelColor[1] << ", " << currPixelColor[2] << ")" << endl;
			
			//set selectedControlPoint
			if (currPixelColor[1] == 255 && currPixelColor[2] == 0 || currPixelColor[1] == 0 && currPixelColor[2] == 255) {
				selectedControlPoint = currPixelColor[0] - 1;
			}
			else {
				selectedControlPoint = -1;
			}
			
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
				cam_pos = camRotationMatrix * glm::vec4(intial_cam_pos.x, intial_cam_pos.y, cam_dist, 0);
				V = glm::lookAt(cam_pos, cam_look_at, cam_up);
				
			}
		}
	}
	
	//if right mouse button is down, translate model
	if (isRightMouseButtonDown) {
		glm::vec2 deltaMousePosition;
		deltaMousePosition.x = (mousePosition.x - lastMousePosition.x) * 150.0f / width;
		deltaMousePosition.y = (lastMousePosition.y - mousePosition.y) * 150.0f / height;
		
		//move point
		if (selectedControlPoint != -1) {

			glm::vec3 moveVal = glm::inverse(V) * glm::vec4(deltaMousePosition.x, deltaMousePosition.y, 0,0);
			points[selectedControlPoint].move(moveVal);
			testCurve->updateSegPoints();
			testCurve2->updateSegPoints();
			testCurve3->updateSegPoints();
		}
	}

	//for trackball
	lastPoint = currPoint;

	//for screen translate
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

