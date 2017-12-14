#pragma once
#include "OBJObject.h"
#include "BoundingBox.h"
class Ship {

	OBJObject* modelMesh;
	glm::mat4 generalMotionMatrix;
	glm::vec3 centerOffset;
	float banking;
	float pitch;
	
	BoundingBox* boundingBox;

public:
	
	Ship();
	~Ship();

	void update(float deltaTime, bool turningLeft, bool turningRight, bool turningUp, bool turningDown);
	void draw();
	glm::mat4 getGeneralMotionMatrix();
	glm::vec3 getPosition();




};