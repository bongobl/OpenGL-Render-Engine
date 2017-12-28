#pragma once
#include "Scene.h"
#include "Asteroid.h"
#include "CubeMap.h"

class SampleScene : public Scene {

	//Scene Objects
	Asteroid* asteroid;
	CubeMap spaceView;

	//Camera
	Camera* mainCam;
	float camDist;
	glm::mat4 camRotationMatrix;

	//Trackball controls
	bool isRightMouseButtonDown;
	bool isLeftMouseButtonDown;
	glm::vec2 mousePosition;
	glm::vec3 currPoint;
	glm::vec3 lastPoint;

public: 

	//basic, overloaded
	void initObjects();
	void dispose();
	void update(float deltaTime);
	void draw();

	Camera* getActiveCamera();


	//events from callbacks, overloaded from Scene
	void key_event(int key, int scancode, int action, int mods);
	void mouse_button_event(int button, int action, int mods);
	void cursor_position_event(double xpos, double ypos);
	void mouse_wheel_event(double xoffset, double yoffset);

private:
	glm::vec3 trackBallMap(glm::vec2 point);
};