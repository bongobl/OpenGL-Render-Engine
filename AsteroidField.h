#pragma once
#include "Asteroid.h"
#include <vector>
class AsteroidField {

	std::vector<Asteroid*> asteroids;
	glm::vec3 center;
public:
	AsteroidField();
	~AsteroidField();

	void update(float deltaTime);
	void draw();

	void setCenter(glm::vec3 c);
private:
	void genRandomAsteroid();
	float getRandomFloat(float min, float max);
	int getRandomInt(int min, int max);
	
};