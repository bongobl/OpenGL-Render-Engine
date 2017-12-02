#pragma once
#include <iostream>
using namespace std;
#include "Asteroid.h"

OBJObject* Asteroid::asteroidTemplates = NULL;
Material Asteroid::asteroidMaterial;

void Asteroid::initStatics() {

	//Create Asteroid Material
	asteroidMaterial.setColor(glm::vec3(1.6, 1.6, 1.6));
	asteroidMaterial.loadTexture("Textures/AsteroidTexture.ppm");
	asteroidMaterial.loadNormalMap("Textures/AsteroidNormalMap.ppm");	

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
}

Asteroid::~Asteroid() {

}

void Asteroid::update() {

}
void Asteroid::draw() {
	asteroidOBJ->setToWorld(getUpdatedToWorldMatrix());
	asteroidOBJ->draw();
}

void Asteroid::setPosition(glm::vec3 pos) {
	position = pos;
}

glm::vec3 Asteroid::getPosition() {
	return position;
}

void Asteroid::setScale(glm::vec3 sc) {
	scale = sc;
}

glm::vec3 Asteroid::getScale() {
	return scale;
}

void Asteroid::setSpinAxis(glm::vec3 spa) {
	spinAxis = spa;
}

void Asteroid::rotate(float deltaRad) {
	rotation += deltaRad;
}

glm::mat4 Asteroid::getUpdatedToWorldMatrix() {
	return glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, spinAxis) * glm::scale(glm::mat4(1.0f), scale);
}