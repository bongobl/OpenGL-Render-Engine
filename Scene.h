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

	//Scene Lights
	std::vector<Light*> allSceneLights;
	std::vector<LightStruct> allSceneLightStructs;
	GLuint UBO_Lights;

	//Shadows (Not used yet)
	GLuint shadowFrameBuffer;
	Texture shadowDepthTexture;



public:
	
	void initObjects();
	void dispose();
	void update(float deltaTime);
	void draw();
	
	void resize_event(int width, int height);

	
	
	virtual Camera* getActiveCamera() = 0;

											//events from callbacks
	virtual void key_event(int key, int scancode, int action, int mods) = 0;
	virtual void mouse_button_event(int button, int action, int mods) = 0;
	virtual void cursor_position_event(double xpos, double ypos) = 0;
	virtual void mouse_wheel_event(double xoffset, double yoffset) = 0;

protected:
	virtual void initThisScenesObjects() = 0;
	virtual void disposeThisScenesObjects() = 0;
	virtual void updateThisScenesObjects(float deltaTime) = 0;
	virtual void drawThisScenesObjects() = 0;

	void applyAllLights();
};