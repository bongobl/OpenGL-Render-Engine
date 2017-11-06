#include "window.h"
const char* window_title = "GLFW Starter Project";

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"

//Shader Programs
GLint SkyboxShaderProgram;
GLint RobotShaderProgram;

// Camera parameters
float cam_dist(20.0f);
const glm::vec3 intial_cam_pos(0.0f, 0, cam_dist);
glm::vec3 cam_pos(0.0f, 0, 20.0f);			// e  | Position of camera
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
glm::mat4 Window::rotationMatrix;


OBJObject* Window::skybox;

glm::mat4 Window::P;
glm::mat4 Window::V;

vector<string> faceNames;

//Robot variables
TransformNode* worldNode;
TransformNode* bodyToWorld;
TransformNode* headToBody;
TransformNode* rightArmToBody;
TransformNode* leftArmToBody;
TransformNode* rightLegToBody;
TransformNode* leftLegToBody;
TransformNode* rightEyeToHead;
TransformNode* leftEyeToHead;
TransformNode* rightAntennaToHead;
TransformNode* leftAntennaToHead;

float legRotation = 0;

void Window::initialize_objects()
{
	isLeftMouseButtonDown = false;
	isRightMouseButtonDown = false;
	rotationMatrix = glm::mat4(1.0f);

	// Load the shader program. Make sure you have the correct filepath up top
	SkyboxShaderProgram = LoadShaders(VERTEX_SHADER_PATH, "../shader_skybox.frag");
	RobotShaderProgram = LoadShaders(VERTEX_SHADER_PATH, "../shader_robot.frag");

	//initialize skybox
	skybox = new OBJObject("myCube.obj", SkyboxShaderProgram);

	faceNames.push_back("skybox/right.ppm");
	faceNames.push_back("skybox/left.ppm");
	faceNames.push_back("skybox/top.ppm");
	faceNames.push_back("skybox/bottom.ppm");
	faceNames.push_back("skybox/back.ppm");
	faceNames.push_back("skybox/front.ppm");

	CubeMapTexture cubeMap;
	cubeMap.loadCubeMapTexture(faceNames);
	

	//initialize world Node
	worldNode = new TransformNode();

	//attach body to world
	bodyToWorld = new TransformNode();
	bodyToWorld->setRotation(glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 2, glm::vec3(1, 0, 0)));
	bodyToWorld->addChild(new GeometryNode("robot-parts/body.obj", RobotShaderProgram));
	worldNode->addChild(bodyToWorld);

	//attach head to body
	headToBody = new TransformNode();
	headToBody->setRotation(glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(1, 0, 0)));
	headToBody->setPosition(glm::vec3(0, 0, -32));
	headToBody->addChild(new GeometryNode("robot-parts/head.obj", RobotShaderProgram));
	bodyToWorld->addChild(headToBody);

	//attach right arm to body
	rightArmToBody = new TransformNode();
	rightArmToBody->setPosition(glm::vec3(-26, 0, -10));
	GeometryNode* rightArm = new GeometryNode("robot-parts/limb.obj", RobotShaderProgram);
	rightArm->setCenter(0, 0, 10);
	rightArmToBody->addChild(rightArm);
	bodyToWorld->addChild(rightArmToBody);

	//attach left arm to body
	leftArmToBody = new TransformNode();
	leftArmToBody->setPosition(glm::vec3(26, 0, -10));
	GeometryNode* leftArm = new GeometryNode("robot-parts/limb.obj", RobotShaderProgram);
	leftArm->setCenter(0, 0, 10);
	leftArmToBody->addChild(leftArm);
	bodyToWorld->addChild(leftArmToBody);

	//attach right leg to body
	rightLegToBody = new TransformNode();
	rightLegToBody->setPosition(glm::vec3(-10,0, 20));
	rightLegToBody->addChild(new GeometryNode("robot-parts/limb.obj", RobotShaderProgram));
	bodyToWorld->addChild(rightLegToBody);

	//attach left leg to body
	leftLegToBody = new TransformNode();
	leftLegToBody->setPosition(glm::vec3(10, 0, 20));
	leftLegToBody->addChild(new GeometryNode("robot-parts/limb.obj", RobotShaderProgram));
	bodyToWorld->addChild(leftLegToBody);


	//attach right eye to head
	rightEyeToHead = new TransformNode();
	rightEyeToHead->setPosition(glm::vec3(-6.5f, -17, -2));
	rightEyeToHead->setScale(0.75f);
	rightEyeToHead->addChild(new GeometryNode("robot-parts/eyeball.obj", RobotShaderProgram));
	headToBody->addChild(rightEyeToHead);

	//attach left eye to head
	leftEyeToHead = new TransformNode();
	leftEyeToHead->setPosition(glm::vec3(6.5f, -17, -2));
	leftEyeToHead->setScale(0.75f);
	leftEyeToHead->addChild(new GeometryNode("robot-parts/eyeball.obj", RobotShaderProgram));
	headToBody->addChild(leftEyeToHead);

	//attach right antenna to head
	rightAntennaToHead = new TransformNode();
	rightAntennaToHead->setPosition(glm::vec3(-6.5f, 0, 3));
	rightAntennaToHead->addChild(new GeometryNode("robot-parts/antenna.obj", RobotShaderProgram));
	headToBody->addChild(rightAntennaToHead);

	//attach left antenna to head
	leftAntennaToHead = new TransformNode();
	leftAntennaToHead->setPosition(glm::vec3(6.5f, 0, 3));
	leftAntennaToHead->setRotation(glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(1, 0, 0)));
	leftAntennaToHead->addChild(new GeometryNode("robot-parts/antenna.obj", RobotShaderProgram));
	headToBody->addChild(leftAntennaToHead);
	

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete skybox;
	glDeleteProgram(SkyboxShaderProgram);
	glDeleteProgram(RobotShaderProgram);
	
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
	legRotation += 0.03f;
	leftLegToBody->setRotation(glm::rotate(glm::mat4(1.0f), sin(legRotation), glm::vec3(1, 0, 0)));
	rightLegToBody->setRotation(glm::rotate(glm::mat4(1.0f), -sin(legRotation), glm::vec3(1, 0, 0)));

	rightArmToBody->setRotation(glm::rotate(glm::mat4(1.0f), sin(legRotation), glm::vec3(1, 0, 0)));
	leftArmToBody->setRotation(glm::rotate(glm::mat4(1.0f), -sin(legRotation), glm::vec3(1, 0, 0)));



}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	skybox->draw();

	worldNode->draw(glm::mat4(1.0f));
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
	

	//If left mouse button is down, do trackball rotation
	if (isLeftMouseButtonDown) {
		currPoint = trackBallMap(glm::vec2(mousePosition.x, mousePosition.y));
		direction = currPoint - lastPoint;
		float velocity = glm::length(direction);
		if (velocity > 0.0001f) {			

			glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
			GLfloat rotAngle = acos(glm::dot(lastPoint, currPoint)) * 1.0f;
			if (!isnan(rotAngle)) {

				rotationMatrix = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis) * rotationMatrix;
				cam_pos = rotationMatrix * glm::vec4(intial_cam_pos.x, intial_cam_pos.y, cam_dist,0);
				V = glm::lookAt(cam_pos, cam_look_at, cam_up);
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
	cam_pos = rotationMatrix * glm::vec4(intial_cam_pos.x, intial_cam_pos.y, cam_dist, 0);
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
