#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"
#include "SceneObject.h"
#include "Camera.h"
class Scene;
class Light : public SceneObject{

	//Light gizmos
	std::vector<glm::vec3> gizmosPoints;
	GLuint VAO, VBO;

public:
	int type;
	glm::vec3 color;
	float brightness;
	glm::vec3 direction;


	enum type {DIRECTIONAL, POINT};

	Light(int light_type, glm::vec3 light_color, float light_brightness, glm::vec3 light_position, glm::vec3 light_direction);
	~Light();
	void applySettings();

	//override from SceneObject
	void draw(Scene* currScene);


private:

	void drawGizmos(Scene* currScene);

};