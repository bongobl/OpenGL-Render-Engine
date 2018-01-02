#include "Light.h"
#include <iostream>
Light::Light(int light_type, glm::vec3 light_color, float light_brightness, glm::vec3 light_position, glm::vec3 light_direction) {

	type = light_type;
	color = light_color;
	brightness = light_brightness;
	setPosition(light_position);
	direction = light_direction;

}

void Light::applySettings() {

	//find light world position and direction
	glm::vec3 worldPosition = parentToWorld * glm::vec4(position,1);
	glm::vec3 worldDirection = parentToWorld * rotation * glm::vec4(direction, 1);

	glUniform1i(glGetUniformLocation(Material::getShaderProgram(), "light.type"), type);
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "light.color"), color.r, color.g, color.b);
	glUniform1f(glGetUniformLocation(Material::getShaderProgram(), "light.brightness"), brightness);
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "light.position"), worldPosition.x, worldPosition.y, worldPosition.z);
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "light.direction"), worldDirection.x, worldDirection.y, worldDirection.z);
}

void Light::draw(Scene* currScene) {
	drawAllChildren(currScene);
}