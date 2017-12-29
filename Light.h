#pragma once
#include <glm/gtc/matrix_transform.hpp>
struct Light {

	int type;
	glm::vec3 color;
	float brightness;

	glm::vec3 position;
	glm::vec3 direction;


	enum type {DIRECTIONAL, POINT};

	Light(int light_type, glm::vec3 light_color, float light_brightness, glm::vec3 pos_or_dir);

};