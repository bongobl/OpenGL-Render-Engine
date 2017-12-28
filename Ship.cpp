#include "Ship.h"

Ship::Ship() {
	
	Material shipMaterial;
	shipMaterial.setDiffuseColor(glm::vec3(1, 0, 0));
	modelMesh = new OBJObject("Models/Ship.obj", shipMaterial);
	generalMotionMatrix = glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 4, glm::vec3(0, 1, 0))  *  glm::scale(glm::mat4(1.0f), glm::vec3(4, 4, 4));
	centerOffset = glm::vec3(0, 0, 2);
	banking = 0;
	pitch = 0;

	boundingBox = new BoundingBox(modelMesh->getVertices());
}
Ship::~Ship() {
	delete boundingBox;
	delete modelMesh;
}

void Ship::update(float deltaTime, bool turningLeft, bool turningRight, bool turningUp, bool turningDown){
	
	//calc banking
	if (turningLeft) {
		if (banking > -glm::pi<float>() / 4) {
			banking -= 2.6f * deltaTime;
		}
	}
	else {
		if (banking < 0) {
			banking += 2.6f * deltaTime;
			if (banking > 0)
				banking = 0;
		}
	}

	if (turningRight) {
		if (banking < glm::pi<float>() / 4) {
			banking += 2.6f * deltaTime;
		}
	}else {
		if (banking > 0) {
			banking -= 2.6f * deltaTime;
			if (banking < 0)
				banking = 0;
		}
	}
	
	//calc pitch
	if (turningDown) {
		if (pitch > -glm::pi<float>() / 4) {
			pitch -= 2.6f * deltaTime;
		}
	}
	else {
		if (pitch < 0) {
			pitch += 2.6f * deltaTime;
			if (pitch > 0) {
				pitch = 0;
			}
		}
	}

	if (turningUp) {
		if (pitch < glm::pi<float>() / 4) {
			pitch += 2.6f * deltaTime;
		}
	}
	else {
		if (pitch > 0) {
			pitch -= 2.6f * deltaTime;
			if (pitch < 0) {
				pitch = 0;
			}
		}
	}

	//use pitch and banking to calc toWorld
	generalMotionMatrix = generalMotionMatrix * glm::rotate(glm::mat4(1.0f), banking * -deltaTime, glm::vec3(0, 1, 0));
	generalMotionMatrix = generalMotionMatrix * glm::rotate(glm::mat4(1.0f), banking * -deltaTime, glm::vec3(0, 1, 0));
	generalMotionMatrix = generalMotionMatrix * glm::rotate(glm::mat4(1.0f), pitch * -deltaTime, glm::vec3(1, 0, 0));	
	generalMotionMatrix = generalMotionMatrix * glm::rotate(glm::mat4(1.0f), pitch * -deltaTime, glm::vec3(1, 0, 0));

	//move forward
	generalMotionMatrix = generalMotionMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 50 * deltaTime));
}
void Ship::draw() {
	modelMesh->setToWorld(generalMotionMatrix	* glm::rotate(glm::mat4(1.0f), banking, glm::vec3(0, 0, 1)) 
												* glm::rotate(glm::mat4(1.0f), 0.5f * pitch, glm::vec3(-1, 0, 0))
												* glm::translate(glm::mat4(1.00f), centerOffset));
	//modelMesh->draw();

	boundingBox->updateToWorld(generalMotionMatrix	* glm::rotate(glm::mat4(1.0f), banking, glm::vec3(0, 0, 1))
													* glm::rotate(glm::mat4(1.0f), 0.5f * pitch, glm::vec3(-1, 0, 0))
													* glm::translate(glm::mat4(1.00f), centerOffset));
	//boundingBox->draw();
}

glm::mat4 Ship::getGeneralMotionMatrix(){
	return generalMotionMatrix;
}

glm::vec3 Ship::getPosition() {
	return glm::vec3(generalMotionMatrix[3][0], generalMotionMatrix[3][1], generalMotionMatrix[3][2]);
}

	

