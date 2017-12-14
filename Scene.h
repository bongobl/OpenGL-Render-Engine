#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "GameManager.h"
#include "Asteroid.h"
#include "CubeMap.h"
class Scene {

	//Camera Settings
	glm::vec3 cam_pos;
	glm::vec3 cam_look_at;
	glm::vec3 cam_up;
	glm::mat4 P;
	glm::mat4 V;
	
	//Scene Objects
	Asteroid* asteroid;
	CubeMap spaceView;

public:
	
	virtual void initObjects();
	virtual void dispose();

	virtual void update(float deltaTime);
	virtual void draw();


	//events from callbacks
	virtual void key_event(int key, int scancode, int action, int mods);
	virtual void resize_event(int width, int height);
	virtual void mouse_button_event(int button, int action, int mods);
	virtual void cursor_position_event(double xpos, double ypos);
	virtual void mouse_wheel_event(double xoffset, double yoffset);

	void drawOBJObject(OBJObject &obj);
	void drawCubeMap(CubeMap &cubeMap);
};