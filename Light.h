#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"
#include "SceneObject.h"
class Scene;
struct Light : public SceneObject{

	int type;
	glm::vec3 color;
	float brightness;

	glm::vec3 direction;


	enum type {DIRECTIONAL, POINT};

	Light(int light_type, glm::vec3 light_color, float light_brightness, glm::vec3 light_position, glm::vec3 light_direction);
	void applySettings();

	//override
	void draw(Scene* currScene);

};