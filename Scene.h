#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneManager.h"
#include "Asteroid.h"
#include "CubeMap.h"
#include "Camera.h"
class Scene {

public:
	

	virtual void initObjects() = 0;
	virtual void dispose() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw() = 0;

	virtual Camera* getActiveCamera() = 0;

	//events from callbacks
	virtual void key_event(int key, int scancode, int action, int mods) = 0;	
	virtual void mouse_button_event(int button, int action, int mods) = 0;
	virtual void cursor_position_event(double xpos, double ypos) = 0;
	virtual void mouse_wheel_event(double xoffset, double yoffset) = 0;

	virtual void resize_event(int width, int height) = 0;
};