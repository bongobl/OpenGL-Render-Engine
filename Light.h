#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
class Light {

	
	int lightType;
	
	glm::vec3 Color;
	glm::vec3 Direction;
	glm::vec3 Position;
	float spot_cutoff;	 //angle of range
	float spot_exponent;

public:
	enum lightEnum { DIRECTIONAL, POINT, SPOT, NONE };
	int shaderProgram;

	Light();
	~Light();
	void setAsDirectionalLight(glm::vec3 c, glm::vec3 d);
	void setAsPointLight(glm::vec3 c, glm::vec3 p);
	void setAsSpotLight(glm::vec3 c, glm::vec3 p, glm::vec3 d, float sc, float se);
	void update();
	
};
