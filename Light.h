#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"
#include "SceneObject.h"
#include "Camera.h"
class Scene;

struct LightStruct {

	glm::vec4 color;
	glm::vec4 position;
	glm::vec4 direction;

	int type;
	float brightness;
	float padding;
	float padding2;

	glm::mat4 VP;

	//LightStruct() {;}

};

class Light : public SceneObject{

	//Light gizmos
	std::vector<glm::vec3> gizmosPoints;
	GLuint VAO, VBO;

	glm::mat4 ViewProjectonMatrix;

public:
	int type;
	glm::vec3 color;
	float brightness;


	enum type {DIRECTIONAL, POINT};

	Light(int light_type, glm::vec3 light_color, float light_brightness, glm::vec3 light_position);
	~Light();
	
	void setViewProjectionMatrix(glm::mat4 light_vp_matrix);

	LightStruct getLightStruct();

	//override
	void sendThisGeometryToShadowMap();
	void drawThisSceneObject(Scene* currScene);


private:

	void drawGizmos(Scene* currScene);

};