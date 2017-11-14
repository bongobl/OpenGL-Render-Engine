#pragma once
#include "OBJObject.h"
class ControlPoint {
	
	
	enum type { INTERPOLATE, APPROX };

	//static fields
	static OBJObject *visual;
	static GLuint PointShaderProgram;

	//member fields
	glm::vec3 position;
	glm::vec3 color;

public:
	
	static void InitStatics();
	static void cleanUpStatics();

	ControlPoint(glm::vec3 col);

	void update();
	void draw();

	void move(glm::vec3 deltaPos);
	void setColor(glm::vec3 newCol);

};