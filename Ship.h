#pragma once
#include "OBJObject.h"
#include "BoundingBox.h"
class Ship {

	OBJObject* modelMesh;
	glm::mat4 toWorld;
	float banking;
	float pitch;
	
	BoundingBox* boundingBox;

public:
	
	Ship();
	~Ship();

	void update(float deltaTime, bool turningLeft, bool turningRight, bool turningUp, bool turningDown);
	void draw();
	glm::mat4 getToWorld();
	glm::vec3 getPosition();




};