#pragma once
#include <iostream>
using namespace std;
#include "Asteroid.h"

OBJObject* Asteroid::asteroidTemplates = NULL;
Material Asteroid::asteroidMaterial;

void Asteroid::initStatics() {

	std::vector<std::string> faceNames;
	faceNames.push_back("skybox/right.ppm");
	faceNames.push_back("skybox/left.ppm");
	faceNames.push_back("skybox/top.ppm");
	faceNames.push_back("skybox/bottom.ppm");
	faceNames.push_back("skybox/back.ppm");
	faceNames.push_back("skybox/front.ppm");
	

	//Load Asteroid Textures
	Texture asteroidTexture;
	asteroidTexture.loadStandardTexture("Textures/AsteroidTexture.ppm");
	Texture normalMapTexture;
	normalMapTexture.loadStandardTexture("Textures/AsteroidNormalMap.ppm");
	Texture reflection;
	reflection.loadCubeMap(faceNames);

	//Create Asteroid Material


	///diffuse
	asteroidMaterial.setDiffuseColor(glm::vec3(0, 1, 1));
	asteroidMaterial.setUseDiffuse(true);

	///specular
	asteroidMaterial.setSpecularColor(glm::vec3(0, 1, 1));
	asteroidMaterial.setUseSpecular(true);

	///ambient
	asteroidMaterial.setAmbientColor(glm::vec3(0, 0.06f, 0.06f));
	asteroidMaterial.setUseAmbient(true);

	///surface texture
	asteroidMaterial.loadSurfaceTexture(asteroidTexture);
	asteroidMaterial.setSurfaceTextureStrength(0.3f);
	//asteroidMaterial.setUseSurfaceTexture(true);

	///normal map
	asteroidMaterial.loadNormalMap(normalMapTexture);	
	asteroidMaterial.setNormalMapStrength(0.9f);
	//asteroidMaterial.setUseNormalMap(true);

	///reflection texture
	asteroidMaterial.loadReflectionTexture(reflection);
	asteroidMaterial.setReflectiveness(1.0f);
	asteroidMaterial.setUseReflectionTexture(true);

	//Create Template of Asteroids
	asteroidTemplates = new OBJObject[4]{	OBJObject("Models/Asteroid0.obj", asteroidMaterial),
											OBJObject("Models/Asteroid1.obj", asteroidMaterial),
											OBJObject("Models/Sphere.obj", asteroidMaterial),
											OBJObject("Models/filletCube.obj", asteroidMaterial) };
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

	trackBallRotate = glm::mat4(1.0f);
	
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


void Asteroid::updateTrackBall(glm::mat4 delta) {
	trackBallRotate = delta * trackBallRotate;
}
glm::mat4 Asteroid::getUpdatedToWorldMatrix() {
	return glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, spinAxis) * trackBallRotate * glm::scale(glm::mat4(1.0f), scale);
}