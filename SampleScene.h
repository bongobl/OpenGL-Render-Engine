#pragma once
#include "Scene.h"
#include "Asteroid.h"
#include "CubeMap.h"

class SampleScene : public Scene {

	//Scene Objects
	Asteroid* asteroid;
	CubeMap spaceView;

public: 

	//basic, overloaded
	void initObjects();
	void dispose();
	void update(float deltaTime);
	void draw();


	//events from callbacks, overloaded
	void key_event(int key, int scancode, int action, int mods);
	void mouse_button_event(int button, int action, int mods);
	void cursor_position_event(double xpos, double ypos);
	void mouse_wheel_event(double xoffset, double yoffset);
};