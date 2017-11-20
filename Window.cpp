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

int selectedControlPoint = -1;

//ControlPoints
int numPoints(30);
ControlPoint* points;

//Bezier Curves
int numCurves(10);
BezierCurve* curves;
float trackMaximumHeight(0);

//Roller coaster car, keep car data in Window file
GLuint CarShaderProgram;
OBJObject* car;
float prevTime, currTime;
float param_T(0);
float carSpeed = .06f;
bool isCarMoving = true;

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

	
	//Curves and Points
	ControlPoint::InitStatics();
	BezierCurve::InitStatics();


	//initialize points
	points = new ControlPoint[numPoints];
	for (int i = 0; i < numPoints; ++i) {
		points[i] = ControlPoint(  glm::vec3(  (i + 1.0f) / 255,   i % 3 != 0,  i % 3 == 0 )     );
	}
	//initialize curves
	curves = new BezierCurve[numCurves];
	for (int i = 0; i < numCurves - 1; ++i) {
		int startIndex = 3 * i;
		curves[i] = BezierCurve(&points[startIndex], &points[startIndex + 1], &points[startIndex + 2], &points[startIndex + 3]);
	}
	curves[numCurves-1] = BezierCurve(&points[numPoints - 3], &points[numPoints - 2], &points[numPoints - 1], &points[0]);
	

	//position points
	for (int i = 0; i < numPoints; ++i) {		
		float angle = i * (2 * glm::pi<float>() / numPoints);
		points[i].move(glm::vec3(90 * cos(angle), 0, 90 * sin(angle)));
	}

	//update curves
	for (int i = 0; i < numCurves; ++i) {
		curves[i].updateCurveLines();
	}

	//create car
	CarShaderProgram = LoadShaders("../shader_car.vert", "../shader_car.frag");
	car = new OBJObject("sphere.obj", CarShaderProgram);
	prevTime = (float)glfwGetTime();
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete skybox;
	delete car;
	
	delete[] curves;
	BezierCurve::cleanUpStatics();
	delete[] points;
	ControlPoint::cleanUpStatics();	

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

	//physics for finding car speed
	float delta_H = trackMaximumHeight - curves[(int)param_T].positionAtTime(param_T - (int)param_T).y;
	carSpeed = isCarMoving? 0.015f + 0.03f * sqrt(abs(delta_H)) : 0;
	
	//set car position
	currTime = (float)glfwGetTime();
	float deltaTime = currTime - prevTime;
	float deltaDist = carSpeed * deltaTime;
	prevTime = currTime;

	//find change in param_T given deltaDist
	param_T += deltaDist / curves[(int)param_T].paramTDistance(param_T - (int)param_T);

	//bounds check on param_T
	if (param_T >= numCurves)
		param_T = 0;
	if (param_T < 0)
		param_T = numCurves - (0 - param_T);

	
	glm::vec3 carPosition = curves[(int)param_T].positionAtTime(param_T - (int)param_T);
	car->setToWorld(glm::translate(glm::mat4(1.0f), carPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(2,2,2)));
	
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
	for (int i = 0; i < numPoints; ++i)
		points[i].draw(&points[selectedControlPoint]);
	
	//draw selected point
	if (selectedControlPoint != -1) {
		points[selectedControlPoint].drawAsSelected();
	}

	//draw Bezier Curve
	for (int i = 0; i < numCurves; ++i) {
		curves[i].draw(&points[selectedControlPoint]);
	}
	
	//draw car
	car->draw(glm::vec3(1,0,0));

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
		else if (key == GLFW_KEY_SPACE) {
			setParamT_ToMax();
		}
		else if (key == GLFW_KEY_S) {
			isCarMoving = isCarMoving ? false : true;
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
			//cout << "(" << currPixelColor[0] - 1 << ", " << currPixelColor[1] << ", " << currPixelColor[2] << ")" << endl;
			
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
			calcMaxHeight();
			
			//Update Bezier Curve
			for (int i = 0; i < numCurves; ++i) {
				curves[i].updateCurveLines();
			}
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

void Window::setParamT_ToMax() {

	int curveIndex = 0;

	//find curveIndex
	for (int i = 0; i < numCurves; ++i) {
		
		float t_val_i = curves[i].getMaxPoint_T();
		glm::vec3 position_i = curves[i].positionAtTime(t_val_i);

		float t_val_currIndex = curves[curveIndex].getMaxPoint_T();
		glm::vec3 position_currIndex = curves[curveIndex].positionAtTime(t_val_currIndex);

		if (position_i.y > position_currIndex.y ) {
			curveIndex = i;
		}
	}

	//find param t at that index
	param_T = curveIndex + curves[curveIndex].getMaxPoint_T();
	
}

void Window::calcMaxHeight() {

	trackMaximumHeight = 0;
	for (int i = 0; i < numCurves; ++i) {

		float t_val_i = curves[i].getMaxPoint_T();
		glm::vec3 position_i = curves[i].positionAtTime(t_val_i);


		if (position_i.y > trackMaximumHeight) {
			trackMaximumHeight = position_i.y;
		}
	}
}