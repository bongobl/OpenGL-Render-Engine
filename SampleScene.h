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
	Model* testModel;
	Model* childObject;
	Model* child2;
	BoundingBox* boundingBox;

	//Cameras
	Camera* currActiveCamera;
	Camera* mainCam;
	float camDist;
	glm::mat4 camRotationMatrix;
	Camera* camera2;
	Camera* dullCam;

	//Lights
	float pointLightDist;
	glm::mat4 pointLightRotationMatrix;

	//Trackball controls
	bool isRightMouseButtonDown;
	bool isLeftMouseButtonDown;
	glm::vec2 mousePosition;
	glm::vec3 currPoint;
	glm::vec3 lastPoint;

	//modes
	enum EditMode { EDIT_MODEL, EDIT_LIGHT };
	enum ActiveLight { DIRECTIONAL_LIGHT, POINT_LIGHT };
	unsigned int currEditMode;
	unsigned int currActiveLight;


	//frame buffer objs
	GLuint blurShaderID;
	GLuint frameTextureID;
	GLuint RBO;
	GLuint FBO;

	
public: 

	
	//OVERLOADED FROM SCENE CLASS

	Camera * getActiveCamera();

	void key_event(int key, int scancode, int action, int mods);
	void mouse_button_event(int button, int action, int mods);
	void cursor_position_event(double xpos, double ypos);
	void mouse_wheel_event(double xoffset, double yoffset);

	void initThisScenesObjects();
	void disposeThisScenesObjects();
	void updateThisScenesObjects(float deltaTime);
	void drawThisScenesObjects();


private:
	glm::vec3 trackBallMap(glm::vec2 point);
};