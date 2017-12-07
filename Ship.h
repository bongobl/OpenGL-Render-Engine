#pragma once
#include "OBJObject.h"
class Ship {

	OBJObject* modelMesh;
	glm::mat4 toWorld;


public:
	
	Ship();
	~Ship();

	void update(float deltaTime, bool turningLeft, bool turningRight, bool turningUp, bool turningDown);
	void draw();
	glm::mat4 getToWorld();




};