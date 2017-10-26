#include "window.h"
#define MODEL -1
const char* window_title = "GLFW Starter Project";
GLint defaultShaderProgram;
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
int Window::currRenderMode;
int Window::currEditMode;

glm::vec2 Window::mousePosition;
glm::vec2 Window::lastMousePosition;
glm::vec3 Window::currPoint;
glm::vec3 Window::lastPoint;

OBJObject* Window::models;
Light* Window::sceneLights;

glm::mat4 Window::P;
glm::mat4 Window::V;

OBJObject* Window::pointLightGraphic;
OBJObject* Window::spotLightGraphic;

void Window::initialize_objects()
{
	isLeftMouseButtonDown = false;
	isRightMouseButtonDown = false;
	currModel = Window::BUNNY;
	models = new OBJObject[3]{ OBJObject("bunny.obj",	Material(glm::vec3(1,0,0), 0, 1, 1),0), 
		                       OBJObject("bear.obj",	Material(glm::vec3(0,1,0), 1, 0, 1),0), 
							   OBJObject("dragon.obj",	Material(glm::vec3(1,0,1), 0.5f, 1, 32),0)};

	pointLightGraphic = new OBJObject("sphere.obj", Material(glm::vec3(1, 1, 1), 1, 1, 1),0);
	pointLightGraphic->setScale(0.2f);
	spotLightGraphic = new OBJObject("cone.obj", Material(glm::vec3(1, 1, 1), 1, 1, 1), 2.0f);
	spotLightGraphic->setScale(0.2f);
	
	// Load the shader program. Make sure you have the correct filepath up top
	defaultShaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	//set up lights
	sceneLights = new Light[3];
	sceneLights[Light::DIRECTIONAL].setAsDirectionalLight( glm::vec3(0.65f, 0.65f, 0.65f), glm::normalize(glm::vec3(1, 1, -1)));
	
	sceneLights[Light::POINT].setAsPointLight(glm::vec3(0.75f, 0.75f, 0.75f), glm::vec3(0, 1, 0));
	
	sceneLights[Light::SPOT].setAsSpotLight(glm::vec3(1, 1, 1),		//color
											glm::vec3(0, 2, 0),			//position
											glm::normalize(glm::vec3(0, -1, 0)),	//direction
											glm::pi<float>()/12,
											30);
	
	currLight = Light::DIRECTIONAL;
	currRenderMode = Window::PHONG;
	currEditMode = MODEL;
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete[] models;
	delete pointLightGraphic;
	glDeleteProgram(defaultShaderProgram);
	
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
	pointLightGraphic->setPosition(sceneLights[Light::POINT].getPosition());
	//spotLightGraphic->setPosition(sceneLights[Light::SPOT].getPosition());
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (currRenderMode == Window::PHONG) {
		sceneLights[currLight].update();
		models[currModel].draw(sceneLights[currLight].shaderProgram);
	}
	else if (currRenderMode == Window::NORMAL) {
		
		// Use the shader of programID
		glUseProgram(defaultShaderProgram); // call lights' update() function here instead

		// Render the cube
		models[currModel].draw(defaultShaderProgram);
		
	}

	//draw light graphics
	if (currEditMode == Light::POINT) {
		glUseProgram(sceneLights[currLight].shaderProgram);
		pointLightGraphic->draw(sceneLights[currLight].shaderProgram);
	}
	if (currEditMode == Light::SPOT) {
		glUseProgram(sceneLights[currLight].shaderProgram);
		spotLightGraphic->draw(sceneLights[currLight].shaderProgram);
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
		// Check which key was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (key == GLFW_KEY_1) {
			currModel = Window::BUNNY;
			models[currModel].setDefaultProperties();
		}
		else if (key == GLFW_KEY_2) {
			currModel = Window::BEAR;
			models[currModel].setDefaultProperties();
		}
		else if (key == GLFW_KEY_3) {
			currModel = Window::DRAGON;
			models[currModel].setDefaultProperties();
		}

		//Scale
		if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			if (mods & GLFW_MOD_SHIFT) {
				if (currEditMode == MODEL)
					models[currModel].incrementScale(0.5f);
			}
			else {
				if (currEditMode == MODEL) 
					models[currModel].incrementScale(-0.5f);
			}
		}

		//RenderMode
		if (key == GLFW_KEY_N && action == GLFW_PRESS) {
			if (mods & GLFW_MOD_SHIFT) {
				currRenderMode = Window::PHONG;
			}
			else {
				currRenderMode = Window::NORMAL;
			}
		}

		//Spot Light Widening
		if (key == GLFW_KEY_RIGHT) {
			if (currEditMode == Light::SPOT) {
				sceneLights[Light::SPOT].spot_cutoff += glm::pi<float>() / 48;			
			}
		}

		//Spot Light Narrowing
		if (key == GLFW_KEY_LEFT) {
			if (currEditMode == Light::SPOT) {
				sceneLights[Light::SPOT].spot_cutoff -= glm::pi<float>() / 48;
			}
		}

		//Spot Light blur
		if (key == GLFW_KEY_UP) {
			if (currEditMode == Light::SPOT) {
				sceneLights[Light::SPOT].spot_exponent += 10;
			}
		}

		//Spot Light sharpen
		if (key == GLFW_KEY_DOWN) {
			if (currEditMode == Light::SPOT) {
				if(sceneLights[Light::SPOT].spot_exponent > 0)			
					sceneLights[Light::SPOT].spot_exponent -= 10;
			}
		}

		//Edit Mode/Render Mode
		else if (key == GLFW_KEY_Q) {
			currLight = Light::DIRECTIONAL;
			currEditMode = Light::DIRECTIONAL;
		}
		else if (key == GLFW_KEY_W) {
			currLight = Light::POINT;
			currEditMode = Light::POINT;
		}
		else if (key == GLFW_KEY_E) {
			currLight = Light::SPOT;
			currEditMode = Light::SPOT;
		}
		else if (key == GLFW_KEY_O) {
			currEditMode = MODEL;
		}

		//Reset Position
		else if (key == GLFW_KEY_R && action == GLFW_PRESS) {

			if (currEditMode == MODEL) {
				models[currModel].setPosition(glm::vec3(0, 0, 0));
				models[currModel].setZoomVal(0);
			}
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
			GLfloat rotAngle = acos(glm::dot(lastPoint, currPoint)) * 4.0f;
			if (!isnan(rotAngle)) {
				if (currEditMode == MODEL)
					models[currModel].updateTrackBallRotate(glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis));
				else {
					sceneLights[currLight].updateTrackBallRotate(glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis));
					if (currEditMode == Light::SPOT) {
						spotLightGraphic->updateTrackBallRotate(glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis));
					}
				}
			}
		}
	}

	//if right mouse button is down, translate model
	if (isRightMouseButtonDown) {
		glm::vec2 deltaMousePosition;
		deltaMousePosition.x = (mousePosition.x - lastMousePosition.x) * .033f;
		deltaMousePosition.y = (lastMousePosition.y - mousePosition.y) * .033f;
		if(currEditMode == MODEL)
			models[currModel].move(glm::vec3(deltaMousePosition.x, deltaMousePosition.y, 0));
	}


	lastPoint = currPoint;
	lastMousePosition = mousePosition;
}

void Window::mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset) {
	//models[currModel].move(glm::vec3(0,0, yoffset));
	if (currEditMode == MODEL)
		models[currModel].zoomModel((float)yoffset);
	else {
		sceneLights[currLight].updateDistance((float)yoffset);
		if (currEditMode == Light::SPOT) {
			spotLightGraphic->updateYOffset((float)yoffset);
		}
	}
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
