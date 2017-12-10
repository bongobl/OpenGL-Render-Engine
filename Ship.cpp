#include "Ship.h"

Ship::Ship() {
	
	Material shipMaterial;
	shipMaterial.setColor(glm::vec3(1, 0, 0));
	modelMesh = new OBJObject("Models/Ship.obj", shipMaterial);
	modelMesh->setModelCenter(glm::vec3(0, 0, 2));
	toWorld = glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 4, glm::vec3(0, 1, 0))  *  glm::scale(glm::mat4(1.0f), glm::vec3(4, 4, 4));
	banking = 0;
	pitch = 0;

	boundingBox = new BoundingBox(modelMesh->getVertices());
}
Ship::~Ship() {

}

void Ship::update(float deltaTime, bool turningLeft, bool turningRight, bool turningUp, bool turningDown){
	
	//calc banking
	if (turningLeft) {
		if (banking > -glm::pi<float>() / 4) {
			banking -= 2.6f * deltaTime;
		}
	}
	else {
		if (banking < 0)
			banking += 2.6f * deltaTime;
	}

	if (turningRight) {
		if (banking < glm::pi<float>() / 4) {
			banking += 2.6f * deltaTime;
		}
	}else {
		if (banking > 0)
			banking -= 2.6f * deltaTime;		
	}

	//calc pitch
	if (turningDown) {
		if (pitch > -glm::pi<float>() / 4) {
			pitch -= 2.6f * deltaTime;
		}
	}
	else {
		if (pitch < 0)
			pitch += 2.6f * deltaTime;
	}

	if (turningUp) {
		if (pitch < glm::pi<float>() / 4) {
			pitch += 2.6f * deltaTime;
		}
	}
	else {
		if (pitch > 0)
			pitch -= 2.6f * deltaTime;
	}

	//use pitch and banking to calc toWorld
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), banking * -deltaTime, glm::vec3(0, 1, 0));
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), banking * -deltaTime, glm::vec3(0, 1, 0));
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), pitch * -deltaTime, glm::vec3(1, 0, 0));	
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), pitch * -deltaTime, glm::vec3(1, 0, 0));

	//move forward
	toWorld = toWorld * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 50 * deltaTime));
}
void Ship::draw() {
	modelMesh->setToWorld(toWorld * glm::rotate(glm::mat4(1.0f), banking, glm::vec3(0, 0, 1)) 
								  * glm::rotate(glm::mat4(1.0f), 0.5f * pitch, glm::vec3(-1, 0, 0)));
	modelMesh->draw();

	boundingBox->updateToWorld(toWorld	* glm::rotate(glm::mat4(1.0f), banking, glm::vec3(0, 0, 1))
										* glm::rotate(glm::mat4(1.0f), 0.5f * pitch, glm::vec3(-1, 0, 0)) * glm::translate(glm::mat4(1.0f), glm::vec3(0,0,2)));
	boundingBox->draw();
}

glm::mat4 Ship::getToWorld(){
	return toWorld;
}

glm::vec3 Ship::getPosition() {
	return glm::vec3(toWorld[3][0], toWorld[3][1], toWorld[3][2]);
}

	

