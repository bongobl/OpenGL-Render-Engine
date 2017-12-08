#include <ctime>
#include <cstdlib>
#include "AsteroidField.h"

AsteroidField::AsteroidField() {
	
	//prepare random number generator
	srand((unsigned int)time(0));
	rand();

	//define field size
	fieldSize = 600;
	center = glm::vec3(0, 0, 0);
	//generate 20 random asteroids
	for (int i = 0; i < 300; ++i) {
		genRandomAsteroid(glm::vec2(center.x - fieldSize, center.x + fieldSize), glm::vec2(center.y - fieldSize, center.y + fieldSize), glm::vec2(center.z - fieldSize, center.z + fieldSize));
	}

	
}
AsteroidField::~AsteroidField() {
	for (unsigned int i = 0; i < asteroids.size(); ++i) {
		delete asteroids.at(i);
	}
}
void AsteroidField::update(float deltaTime) {

	for (std::vector<Asteroid*>::iterator iter = asteroids.begin(); iter != asteroids.end(); ++iter) {
		(*iter)->update(deltaTime);
		
		//X
		if ((*iter)->getPosition().x < center.x - fieldSize) {
			delete *iter;
			asteroids.erase(iter);
			genRandomAsteroid(glm::vec2(center.x + fieldSize, center.x + fieldSize), glm::vec2(center.y - fieldSize, center.y + fieldSize), glm::vec2(center.z - fieldSize, center.z + fieldSize));
		}
		else if ((*iter)->getPosition().x > center.x + fieldSize) {
			delete *iter;
			asteroids.erase(iter);
			genRandomAsteroid(glm::vec2(center.x - fieldSize, center.x - fieldSize), glm::vec2(center.y - fieldSize, center.y + fieldSize), glm::vec2(center.z - fieldSize, center.z + fieldSize));
		}

		//Y
		else if ((*iter)->getPosition().y < center.y - fieldSize) {
			delete *iter;
			asteroids.erase(iter);
			genRandomAsteroid(glm::vec2(center.x -fieldSize, center.x+ fieldSize), glm::vec2(center.y + fieldSize, center.y + fieldSize), glm::vec2(center.z -fieldSize, center.z + fieldSize));
		}
		else if ((*iter)->getPosition().y > center.y + fieldSize) {
			delete *iter;
			asteroids.erase(iter);
			genRandomAsteroid(glm::vec2(center.x - fieldSize, center.x + fieldSize), glm::vec2(center.y - fieldSize, center.y - fieldSize), glm::vec2(center.z - fieldSize, center.z + fieldSize));
		}

		//Z
		else if ((*iter)->getPosition().z < center.z - fieldSize) {
			delete *iter;
			asteroids.erase(iter);
			genRandomAsteroid(glm::vec2(center.x -fieldSize,center.x + fieldSize), glm::vec2(center.y - fieldSize, center.z +fieldSize), glm::vec2(center.z + fieldSize, center.z + fieldSize));
		}
		else if ((*iter)->getPosition().z > center.z + fieldSize) {
			delete *iter;
			asteroids.erase(iter);
			genRandomAsteroid(glm::vec2(center.x - fieldSize, center.x + fieldSize), glm::vec2(center.y - fieldSize, center.z + fieldSize), glm::vec2(center.z - fieldSize, center.z - fieldSize));
		}
	}//END FOR

	/*
	for (unsigned int i = 1; i < asteroids.size(); ++i) {
		for (unsigned int j = 0; j < i; ++j) {
			if (asteroids[j]->getBoundingBox()->isCollidingWith(asteroids[i]->getBoundingBox())) {
				
				asteroids[i]->setVelocity(asteroids[i]->getVelocity() * glm::vec3(-1, -1, -1));
				asteroids[j]->setVelocity(asteroids[j]->getVelocity() * glm::vec3(-1, -1, -1));
			}
		}
	}*/
}
void AsteroidField::draw() {
	for (unsigned int i = 0; i < asteroids.size(); ++i) {
		asteroids.at(i)->draw();
	}
}
void AsteroidField::genRandomAsteroid(glm::vec2 xRange, glm::vec2 yRange, glm::vec2 zRange) {

	int randID = getRandomInt(0, 1);
	glm::vec3 randPosition(getRandomFloat(xRange.x, xRange.y), getRandomFloat(yRange.x, yRange.y), getRandomFloat(zRange.x, zRange.y));
	float randomScale = getRandomFloat(1, 30);
	glm::vec3 randomSpinAxis(getRandomFloat(-1, 1), getRandomFloat(-1, 1), getRandomFloat(-1, 1));
	float randomRotationSpeed = getRandomFloat(0, glm::pi<float>() / 6);
	glm::vec3 randomVelocity(getRandomFloat(-10, 10), getRandomFloat(-10, 10), getRandomFloat(-10, 10));
	Asteroid* newAsteroid = new Asteroid(randID);


	newAsteroid->setPosition(randPosition);
	newAsteroid->setScale(glm::vec3(randomScale, randomScale, randomScale));
	newAsteroid->setSpinAxis(randomSpinAxis);
	newAsteroid->setRotationSpeed(randomRotationSpeed);
	newAsteroid->setVelocity(randomVelocity);

	
	bool isColliding = false;
	do {
		isColliding = false;
		newAsteroid->setPosition(glm::vec3(getRandomFloat(xRange.x, xRange.y), getRandomFloat(yRange.x, yRange.y), getRandomFloat(zRange.x, zRange.y)));
		for (unsigned int i = 0; i < asteroids.size(); ++i) {
			if (asteroids[i]->getBoundingBox()->isCollidingWith(newAsteroid->getBoundingBox())) {
				isColliding = true;
				break;
			}
		}
	} while (isColliding);
	
	asteroids.push_back(newAsteroid);
	
}
float AsteroidField::getRandomFloat(float min, float max) {

	float param = (float)rand() / (float)RAND_MAX;
	return min * (1 - param) + max * param;
}
int AsteroidField::getRandomInt(int min, int max) {

	return rand() % (max - min + 1) + min;
}

void AsteroidField::setCenter(glm::vec3 c) {
	center = c;
}

