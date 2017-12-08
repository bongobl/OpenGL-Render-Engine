#pragma once
#include "Asteroid.h"
#include <vector>
class AsteroidField {

	std::vector<Asteroid*> asteroids;
	
	float fieldSize;
public:
	glm::vec3 center;
	AsteroidField();
	~AsteroidField();

	void update(float deltaTime);
	void draw();

	void setCenter(glm::vec3 c);
private:
	void genRandomAsteroid(glm::vec2 xRange, glm::vec2 yRange, glm::vec2 zRange);
	float getRandomFloat(float min, float max);
	int getRandomInt(int min, int max);
	
};