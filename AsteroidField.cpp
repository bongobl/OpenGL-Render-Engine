#include <ctime>
#include <cstdlib>
#include "AsteroidField.h"

AsteroidField::AsteroidField() {
	
	//prepare random number generator
	srand(time(0));
	rand();

	//generate 20 random asteroids
	for (int i = 0; i < 100; ++i) {
		genRandomAsteroid();
	}
}
AsteroidField::~AsteroidField() {

}
void AsteroidField::update(float deltaTime) {
	for (unsigned int i = 0; i < asteroids.size(); ++i) {
		asteroids.at(i)->update(deltaTime);
	}
}
void AsteroidField::draw() {
	for (unsigned int i = 0; i < asteroids.size(); ++i) {
		asteroids.at(i)->draw();
	}
}
void AsteroidField::genRandomAsteroid() {

	int randID = getRandomInt(0, 1);
	glm::vec3 randPosition(getRandomFloat(-150, 150), getRandomFloat(-150, 150), getRandomFloat(-150, 150));
	float randomScale = getRandomFloat(1, 10);
	glm::vec3 randomSpinAxis(getRandomFloat(-1, 1), getRandomFloat(-1, 1), getRandomFloat(-1, 1));
	float randomRotationSpeed = getRandomFloat(0, glm::pi<float>());
	
	Asteroid* newAsteroid = new Asteroid(randID);
	newAsteroid->setPosition(randPosition);
	newAsteroid->setScale(glm::vec3(randomScale, randomScale, randomScale));
	newAsteroid->setSpinAxis(randomSpinAxis);
	newAsteroid->setRotationSpeed(randomRotationSpeed);

	asteroids.push_back(newAsteroid);
	
}
float AsteroidField::getRandomFloat(float min, float max) {

	float param = (float)rand() / (float)RAND_MAX;
	return min * (1 - param) + max * param;
}
int AsteroidField::getRandomInt(int min, int max) {

		return rand() % (max - min + 1) + min;
}

