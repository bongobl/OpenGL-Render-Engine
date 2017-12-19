#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneManager.h"
#include "Asteroid.h"
#include "CubeMap.h"
class Scene {

public:
	
	//Public Camera Settings
	glm::vec3 cam_pos;
	glm::vec3 cam_look_at;
	glm::vec3 cam_up;
	glm::mat4 P;
	glm::mat4 V;

	virtual void initObjects() = 0;
	virtual void dispose() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw() = 0;

	//events from callbacks
	virtual void key_event(int key, int scancode, int action, int mods) = 0;	
	virtual void mouse_button_event(int button, int action, int mods) = 0;
	virtual void cursor_position_event(double xpos, double ypos) = 0;
	virtual void mouse_wheel_event(double xoffset, double yoffset) = 0;

	void resize_event(int width, int height);
};