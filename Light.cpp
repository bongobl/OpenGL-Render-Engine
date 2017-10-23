#include <iostream>
#include "Light.h"

using namespace std;


Light::Light() {
	
	lightType = Light::NONE;
}
Light::~Light() {
	glDeleteProgram(shaderProgram);
}
void Light::setAsDirectionalLight(glm::vec3 c, glm::vec3 d) {
	
	lightType = Light::DIRECTIONAL;
	shaderProgram = LoadShaders( "../shader.vert", "../shader_directionalLight.frag");
	Color = c;
	Direction = d;
}
void Light::setAsPointLight(glm::vec3 c, glm::vec3 p) {
	
	lightType = Light::POINT;
	shaderProgram = LoadShaders("../shader.vert", "../shader_pointLight.frag");
	Color = c;
	Position = p;
}
void Light::setAsSpotLight(glm::vec3 c, glm::vec3 p, glm::vec3 d, float sc, float se) {
	
	lightType = Light::SPOT;
	shaderProgram = LoadShaders("../shader.vert", "../shader_spotLight.frag");
	Color = c;
	Position = p;
	Direction = d;
	spot_cutoff = sc;
	spot_exponent = se;
}

void Light::update() {

	glUseProgram(shaderProgram);

	if (lightType == Light::DIRECTIONAL) {
		glUniform3f(glGetUniformLocation(shaderProgram, "directionalLight.Color"), Color.x, Color.y, Color.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "directionalLight.Direction"), Direction.x, Direction.y, Direction.z);
	}
	else if (lightType == Light::POINT) {
		glUniform3f(glGetUniformLocation(shaderProgram, "light.Color"), Color.x, Color.y, Color.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "light.Position"), Position.x, Position.y, Position.z);
	}
	else if (lightType == Light::SPOT) {
		glUniform3f(glGetUniformLocation(shaderProgram, "light.Color"), Color.x, Color.y, Color.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "light.Position"), Position.x, Position.y, Position.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "light.Direction"), Direction.x, Direction.y, Direction.z);
		glUniform1f(glGetUniformLocation(shaderProgram, "light.spot_cutoff"), spot_cutoff);
		glUniform1f(glGetUniformLocation(shaderProgram, "light.spot_exponent"), spot_exponent);
	}
}