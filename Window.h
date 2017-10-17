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
#include "Cube.h"
#include "shader.h"
#include "OBJObject.h"

using namespace std;
class Window
{
	enum modelID { BUNNY, BEAR, DRAGON };

	static OBJObject* models; 
	
public:

	static bool isLeftMouseButtonDown;
	static bool isRightMouseButtonDown;
	static int currModel;
	static int width;
	static int height;

	//mouse variables
	static glm::vec2 mousePosition;
	static glm::vec2 lastMousePosition;
	static glm::vec3 currPoint;
	static glm::vec3 lastPoint;

	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);

private:
	static glm::vec3 trackBallMap(glm::vec2 point);
};

#endif
