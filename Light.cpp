#include <iostream>
#include "Light.h"

using namespace std;


Light::Light() {
	
	lightType = Light::NONE;
	trackBallRotate = glm::mat4(1.0f);
	distanceToCenter = 0;
	distanceMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, distanceToCenter));
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
	startPosition = Position = p;
}
void Light::setAsSpotLight(glm::vec3 c, glm::vec3 p, glm::vec3 d, float sc, float se) {
	
	lightType = Light::SPOT;
	shaderProgram = LoadShaders("../shader.vert", "../shader_spotLight.frag");
	Color = c;
	startPosition = Position = p;
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
		glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.Color"), Color.x, Color.y, Color.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.Position"), Position.x, Position.y, Position.z);
	}
	else if (lightType == Light::SPOT) {
		glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.Color"), Color.x, Color.y, Color.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.Position"), Position.x, Position.y, Position.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.Direction"), Direction.x, Direction.y, Direction.z);
		glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.spot_cutoff"), spot_cutoff);
		glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.spot_exponent"), spot_exponent);
	}
}

void Light::updateTrackBallRotate(glm::mat4 offset) {

	if (lightType == Light::DIRECTIONAL) {
		Direction = glm::vec3(offset * glm::vec4(Direction.x, Direction.y, Direction.z, 1.0f));
	}
	if (lightType == Light::POINT || lightType == Light::SPOT) {
		trackBallRotate = offset * trackBallRotate;
		Position = glm::vec3(trackBallRotate * distanceMatrix * glm::vec4(startPosition.x, startPosition.y, startPosition.z, 1.0f));
		Direction = glm::normalize(glm::vec3(-1, -1, -1) * Position);
	}
}

void Light::updateDistance(float offset) {
	distanceToCenter += offset;
	distanceMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, distanceToCenter));
	Position = glm::vec3(trackBallRotate * distanceMatrix * glm::vec4(startPosition.x, startPosition.y, startPosition.z, 1.0f));
	Direction = glm::normalize(glm::vec3(-1, -1, -1) * Position);
}