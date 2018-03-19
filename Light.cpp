#include "Light.h"
#include "Scene.h"
#include <iostream>
Light::Light(int light_type, glm::vec3 light_color, float light_brightness, glm::vec3 light_position, glm::vec3 light_direction) {

	type = light_type;
	color = light_color;
	brightness = light_brightness;
	setLocalPosition(light_position);
	direction = light_direction;

	float gizmosSize = 3;
	float dist = sqrt(gizmosSize * gizmosSize / 3);

	gizmosPoints.push_back(glm::vec3(gizmosSize, 0, 0));
	gizmosPoints.push_back(glm::vec3(-gizmosSize, 0, 0));
	gizmosPoints.push_back(glm::vec3(0, gizmosSize, 0));
	gizmosPoints.push_back(glm::vec3(0, -gizmosSize, 0));
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

	//prepare buffers
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
void Light::applySettings() {

	//find light world position and direction
	glm::vec3 worldPosition = parentToWorld * glm::vec4(local_position,1);
	glm::vec3 worldDirection = parentToWorld * local_rotation * glm::vec4(direction, 1);

	glUniform1i(glGetUniformLocation(Material::getShaderProgram(), "light.type"), type);
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "light.color"), color.r, color.g, color.b);
	glUniform1f(glGetUniformLocation(Material::getShaderProgram(), "light.brightness"), brightness);
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "light.position"), worldPosition.x, worldPosition.y, worldPosition.z);
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "light.direction"), worldDirection.x, worldDirection.y, worldDirection.z);
}

void Light::draw(Scene* currScene) {
	drawAllChildren(currScene);
	drawGizmos(currScene);
}

void Light::drawGizmos(Scene* currScene) {

	glUseProgram(Material::getShaderProgram());

	Camera* activeCamera = currScene->getActiveCamera();

	glm::mat4 toWorldNoScale = glm::translate(glm::mat4(1.0f), local_position) * local_rotation;

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