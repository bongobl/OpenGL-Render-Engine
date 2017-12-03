#pragma once
#include "Asteroid.h"
#include <vector>
class AsteroidField {

	std::vector<Asteroid*> asteroids;

public:
	AsteroidField();
	~AsteroidField();

	void update(float deltaTime);
	void draw();
private:
	void genRandomAsteroid();
	float getRandomFloat(float min, float max);
	int getRandomInt(int min, int max);
	
};