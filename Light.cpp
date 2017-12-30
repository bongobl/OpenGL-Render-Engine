#include "Light.h"
#include <iostream>
Light::Light(int light_type, glm::vec3 light_color, float light_brightness, glm::vec3 pos_or_dir) {

	type = light_type;
	color = light_color;
	brightness = light_brightness;
	direction = pos_or_dir;
	position = pos_or_dir;

}

void Light::applySettings() {
	glUniform1i(glGetUniformLocation(Material::getShaderProgram(), "light.type"), type);
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "light.color"), color.r, color.g, color.b);
	glUniform1f(glGetUniformLocation(Material::getShaderProgram(), "light.brightness"), brightness);
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "light.position"), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "light.direction"), direction.x, direction.y, direction.z);
}