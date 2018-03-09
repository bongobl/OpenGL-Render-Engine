#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Scene;
class SceneManager {

	//Window Variables
	static int windowWidth;
	static int windowHeight;

	//time keeping
	static float currTime;
	static float prevTime;

	//Scene
	static Scene* currScene;

	
	//FBOs
	static GLuint blurShader;
	static GLuint FramebufferName;
	static GLuint renderedTexture;
	static GLuint depthrenderbuffer;
	static GLenum DrawBuffers[1];
	static GLuint quad_VertexArrayID;
	static GLuint quad_vertexbuffer;
	static GLfloat g_quad_vertex_buffer_data[];
	static GLuint quadEBO;
	static GLuint indices[];
	
	static float testFloat[1];

public:
	
	static GLFWwindow* window;

	static int createWindow(const char* title, int window_width, int window_height);
	static void initObjects();
	static void dispose();
	static void update();
	static void draw();
	

	//callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);
	static void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);
	
	static bool isWindowOpen();

	static GLuint getBlurShader();

};