#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "shader.h"
#include "OBJObject.h"
#include "CubeMap.h"
#include "Material.h"
#include "Asteroid.h"
#include "AsteroidField.h"
using namespace std;

class Window
{
	
public:


	//Window Variables
	static int width;
	static int height;

	//trackball variables
	static bool isLeftMouseButtonDown;
	static bool isRightMouseButtonDown;
	static glm::vec2 mousePosition;
	static glm::vec2 lastMousePosition;
	static glm::vec3 currPoint;
	static glm::vec3 lastPoint;
	static glm::mat4 camRotationMatrix;


	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view

	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	//trackball functions
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);
	static void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);

private:
	static glm::vec3 trackBallMap(glm::vec2 point);
};

#endif
