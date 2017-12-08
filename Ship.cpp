#include "Ship.h"

Ship::Ship() {
	
	Material shipMaterial;
	shipMaterial.setColor(glm::vec3(1, 0, 0));
	modelMesh = new OBJObject("Models/Ship.obj", shipMaterial);
	modelMesh->setModelCenter(glm::vec3(0, 0, 2));
	toWorld = glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 4, glm::vec3(0, 1, 0))  *  glm::scale(glm::mat4(1.0f), glm::vec3(4, 4, 4));
}
Ship::~Ship() {

}

void Ship::update(float deltaTime, bool turningLeft, bool turningRight, bool turningUp, bool turningDown){
	
	if (turningLeft) {
		toWorld = toWorld * glm::rotate(glm::mat4(1.0f), deltaTime, glm::vec3(0, 1, 0));
	}
	if (turningRight) {
		toWorld = toWorld * glm::rotate(glm::mat4(1.0f), -deltaTime, glm::vec3(0, 1, 0));
	}
	if (turningUp) {
		toWorld = toWorld * glm::rotate(glm::mat4(1.0f), deltaTime, glm::vec3(1, 0, 0));
	}
	if (turningDown) {
		toWorld = toWorld * glm::rotate(glm::mat4(1.0f), -deltaTime, glm::vec3(1, 0, 0));
	}

	//move forward
	toWorld = toWorld * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 25 * deltaTime));
}
void Ship::draw() {
	modelMesh->setToWorld(toWorld);
	modelMesh->draw();
}

glm::mat4 Ship::getToWorld(){
	return toWorld;
}

glm::vec3 Ship::getPosition() {
	return glm::vec3(toWorld[3][0], toWorld[3][1], toWorld[3][2]);
}

	

