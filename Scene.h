#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneManager.h"
#include "Camera.h"
#include "Light.h"
#include "ShadowMap.h"
class Scene {

protected:
	
	int window_width;
	int window_height;

	std::vector<Camera*> allSceneCameras;

	//Scene Lights
	const static GLuint MAX_LIGHTS = 30;
	std::vector<Light*> allSceneLights;
	std::vector<LightStruct> allSceneLightStructs;
	GLuint UBO_Lights;

	//all shadow maps
	std::vector<ShadowMap*> shadowMaps;

public:
	
	void init();
	void dispose();
	void update();
	void calcShadowMaps();
	void draw();
	
	void resize_event(int width, int height);

	
	
	virtual Camera* getActiveCamera() = 0;

	//events from callbacks
	virtual void key_event(int key, int scancode, int action, int mods) = 0;
	virtual void mouse_button_event(int button, int action, int mods) = 0;
	virtual void cursor_position_event(double xpos, double ypos) = 0;
	virtual void mouse_wheel_event(double xoffset, double yoffset) = 0;

protected:
	virtual void initThisScene() = 0;
	virtual void disposeThisScene() = 0;
	virtual void updateThisScene() = 0;
	virtual void drawThisSceneToShadowMap() = 0;
	virtual void drawThisScene() = 0;


private:
	void applyAllLights();

	void recalcUBO_Lights();

};