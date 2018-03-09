#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneManager.h"
#include "Camera.h"
#include "Light.h"

class Scene {

protected:
	int window_width;
	int window_height;

	std::vector<Camera*> allSceneCameras;


public:
	
	void initObjects();
	
	void dispose();
	virtual void update(float deltaTime) = 0;
	void draw();
	

	virtual Camera* getActiveCamera() = 0;
	virtual Light* getActiveLight() = 0;

	//events from callbacks
	virtual void key_event(int key, int scancode, int action, int mods) = 0;	
	virtual void mouse_button_event(int button, int action, int mods) = 0;
	virtual void cursor_position_event(double xpos, double ypos) = 0;
	virtual void mouse_wheel_event(double xoffset, double yoffset) = 0;

	void resize_event(int width, int height);

	

private: 
	virtual void drawSceneToBuffer() = 0;
	virtual void initThisSceneObjects() = 0;
	virtual void disposeThisSceneObjects() = 0;
};