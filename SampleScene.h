#pragma once
#include "Scene.h"
#include "Texture.h"
#include "SkyBox.h"
#include "Model.h"
#include "BoundingBox.h"
class SampleScene : public Scene {

	//Textures
	Texture oceanViewCubeMap;
	Texture asteroidTexture;
	Texture normalMapTexture;

	//Scene Objects
	SkyBox oceanView;
	Model* wall;
	Model* cylinder;
	Model* prism;

	//Cameras
	Camera* currActiveCamera;
	

	//camera rig
	glm::mat4 camRotationMatrix;
	float camDist;
	glm::vec3 camPos;


	//Trackball controls
	bool isRightMouseButtonDown;
	bool isLeftMouseButtonDown;
	glm::vec2 mousePosition;
	glm::vec3 currPoint;
	glm::vec3 lastPoint;




public:


	//OVERLOADED FROM SCENE CLASS

	Camera * getActiveCamera();

	void key_event(int key, int scancode, int action, int mods);
	void mouse_button_event(int button, int action, int mods);
	void cursor_position_event(double xpos, double ypos);
	void mouse_wheel_event(double xoffset, double yoffset);

	void initThisScene();
	void disposeThisScene();
	void updateThisScene();
	void drawThisSceneToShadowMap();
	void drawThisScene();


private:
	glm::vec3 trackBallMap(glm::vec2 point);
};