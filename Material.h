#pragma once
#include <glm/gtc/matrix_transform.hpp>
struct Material {

	glm::vec3 color;
	float diffuse;
	float specular;
	

	Material();
	Material(glm::vec3 c, float d, float s);

};