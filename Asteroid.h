#pragma once
#include "OBJObject.h"
#include "Material.h"
#include "shader.h"
class Asteroid {

	//static fields
	static OBJObject* asteroidTemplates;
	static Material asteroidMaterial;

	//member fields
	OBJObject* asteroidOBJ;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 spinAxis;
	float rotation;

public:

	static void initStatics();
	static void cleanUpStatics();
	Asteroid(unsigned int id);
	~Asteroid();

	void update();
	void draw();
	
	void setPosition(glm::vec3 pos);
	glm::vec3 getPosition();
	void setScale(glm::vec3 sc);
	glm::vec3 getScale();

	void setSpinAxis(glm::vec3 spa);
	void rotate(float deltaRad);


private:
	glm::mat4 getUpdatedToWorldMatrix();
};