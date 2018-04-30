#include "Light.h"
#include "Scene.h"
#include <iostream>
Light::Light(int light_type, glm::vec3 light_color, float light_brightness, glm::vec3 light_position) {

	type = light_type;
	color = light_color;
	brightness = light_brightness;
	setLocalPosition(light_position);

	//for gizmos points math
	float gizmosSize = 3;
	float dist = sqrt(gizmosSize * gizmosSize / 3);

	//light gizmos points
	gizmosPoints.push_back(glm::vec3(gizmosSize, 0, 0));
	gizmosPoints.push_back(glm::vec3(-gizmosSize, 0, 0));
	gizmosPoints.push_back(glm::vec3(0, gizmosSize, 0));
	gizmosPoints.push_back(glm::vec3(0, -gizmosSize, 0));
	if(type == Light::DIRECTIONAL)
		gizmosPoints.push_back(glm::vec3(0, 0, gizmosSize * 10));
	else
		gizmosPoints.push_back(glm::vec3(0, 0, gizmosSize));
	gizmosPoints.push_back(glm::vec3(0, 0, -gizmosSize));
	gizmosPoints.push_back(glm::vec3(dist, dist, dist));
	gizmosPoints.push_back(glm::vec3(-dist, -dist, -dist));
	gizmosPoints.push_back(glm::vec3(-dist, dist, dist));
	gizmosPoints.push_back(glm::vec3(dist, -dist, -dist));
	gizmosPoints.push_back(glm::vec3(dist, -dist, dist));
	gizmosPoints.push_back(glm::vec3(-dist, dist, -dist));
	gizmosPoints.push_back(glm::vec3(dist, dist, -dist));
	gizmosPoints.push_back(glm::vec3(-dist, -dist, dist));

	//prepare gizmos buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, gizmosPoints.size() * sizeof(glm::vec3), gizmosPoints.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

Light::~Light() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Light::setViewProjectionMatrix(glm::mat4 light_vp_matrix) {

	ViewProjectonMatrix = light_vp_matrix;
}
LightStruct Light::getLightStruct() {

	LightStruct working;
	working.type = this->type;
	working.color = glm::vec4(this->color, 1);
	working.brightness = this->brightness;
	working.position = parentToWorld * glm::vec4(local_position, 1);
	working.direction = toWorld * glm::vec4(0,0,1,0);  //4th component is 0 so no translations applied
	working.VP = ViewProjectonMatrix;

	return working;
}

void Light::sendThisGeometryToShadowMap() {
	//leave empty
}
void Light::drawThisSceneObject(Scene* currScene) {

	drawGizmos(currScene);
}

void Light::drawGizmos(Scene* currScene) {

	glUseProgram(Material::getShaderProgram());

	Camera* activeCamera = currScene->getActiveCamera();

	//check for negative z scale
	glm::mat4 rotationCorrector = glm::mat4(1.0f);
	if(local_scale.z < 0){
		rotationCorrector = glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(1,0,0));
	}
	//calc toWorldNoScale
	glm::mat4 toWorldNoScale = glm::translate(glm::mat4(1.0f), getPosition(SceneObject::WORLD)) * getRotation(SceneObject::WORLD) * rotationCorrector;

	//apply object properties	
	glUniformMatrix4fv(glGetUniformLocation(Material::getShaderProgram(), "toWorld"), 1, GL_FALSE, &toWorldNoScale[0][0]);

	//apply camera properties
	activeCamera->applySettings(Material::getShaderProgram());

	//apply material properties
	Material m;
	m.setSurfaceColor(color);
	m.setUseSurfaceColor(true);
	m.applySettings();

	//Bind VAO for gizmos and draw 
	glBindVertexArray(VAO);
	glLineWidth(3);
	glDrawArrays(GL_LINES, 0, gizmosPoints.size());
	glBindVertexArray(0);
}