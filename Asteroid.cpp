#pragma once
#include <iostream>
using namespace std;
#include "Asteroid.h"

OBJObject* Asteroid::asteroidTemplates = NULL;
Material Asteroid::asteroidMaterial;

void Asteroid::initStatics() {

	
	//Create Asteroid Material
	asteroidMaterial.setColor(glm::vec3(2, 2, 2));
	//asteroidMaterial.loadTexture("Textures/AsteroidTexture.ppm");
	//asteroidMaterial.loadNormalMap("Textures/AsteroidNormalMap.ppm");	
	
	
	std::vector<std::string> faceNames;
	faceNames.push_back("skybox/right.ppm");
	faceNames.push_back("skybox/left.ppm");
	faceNames.push_back("skybox/top.ppm");
	faceNames.push_back("skybox/bottom.ppm");
	faceNames.push_back("skybox/back.ppm");
	faceNames.push_back("skybox/front.ppm");
	asteroidMaterial.loadReflectionTexture(faceNames);

	//Create Template of Asteroids
	asteroidTemplates = new OBJObject[2]{	OBJObject("Models/Asteroid0.obj", asteroidMaterial),
											OBJObject("Models/Asteroid1.obj", asteroidMaterial) };
}

void Asteroid::cleanUpStatics() {

	delete[] asteroidTemplates;
	
}
Asteroid::Asteroid(unsigned int id) {

	if (asteroidTemplates == NULL) {
		cerr << "No Asteroid templates, make sure you call Asteroid::InitStatics() before initializing an asteroid" << endl;
		exit(-1);
	}
	asteroidOBJ = &asteroidTemplates[id];
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(3, 3, 3);
	spinAxis = glm::vec3(0, 1, 0);
	rotation = 0;
	velocity = glm::vec3(0, 0, 0);

	boundingBox = new BoundingBox(asteroidOBJ->getVertices());
	
}

Asteroid::~Asteroid() {
	delete boundingBox;
}

void Asteroid::update(float deltaTime) {
	rotation += rotationSpeed * deltaTime;
	position += velocity * deltaTime;
}
void Asteroid::draw(Scene* currScene) {
	asteroidOBJ->setToWorld(getUpdatedToWorldMatrix());
	asteroidOBJ->draw(currScene);

	boundingBox->updateToWorld(getUpdatedToWorldMatrix());
	boundingBox->draw(currScene);
}

void Asteroid::setPosition(glm::vec3 pos) {
	position = pos;
	boundingBox->updateToWorld(getUpdatedToWorldMatrix());
}

glm::vec3 Asteroid::getPosition() {
	return position;
}

void Asteroid::setVelocity(glm::vec3 vel) {
	velocity = vel;
}

glm::vec3 Asteroid::getVelocity() {
	return velocity;
}
void Asteroid::setScale(glm::vec3 sc) {
	scale = sc;
	boundingBox->updateToWorld(getUpdatedToWorldMatrix());
}

glm::vec3 Asteroid::getScale() {
	return scale;
}


BoundingBox* Asteroid::getBoundingBox() {
	return boundingBox;
}
void Asteroid::setSpinAxis(glm::vec3 spa) {
	spinAxis = spa;
	boundingBox->updateToWorld(getUpdatedToWorldMatrix());
}
void Asteroid::setRotationSpeed(float rs) {
	rotationSpeed = rs;
	boundingBox->updateToWorld(getUpdatedToWorldMatrix());
}

glm::mat4 Asteroid::getUpdatedToWorldMatrix() {
	return glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, spinAxis) * glm::scale(glm::mat4(1.0f), scale);
}