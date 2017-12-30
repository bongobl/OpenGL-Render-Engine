#include "Camera.h"

Camera::Camera(glm::vec3 camera_position, glm::vec3 camera_look_at, glm::vec3 camera_up, float camera_width, float camera_height) {
	position = camera_position;
	look_at = camera_look_at;
	up = camera_up;
	width = camera_width;
	height = camera_height;

}

void Camera::resize(float camera_width, float camera_height) {

	
	if (camera_height > 0)
	{
		ProjectionMatrix = glm::perspective(45.0f, (float)camera_width / (float)camera_height, 0.1f, 100000.0f);
		ViewMatrix = glm::lookAt(position, look_at, up);
	}
	
}

void Camera::applySettings(GLuint currShaderProgram) {
	glUniformMatrix4fv(glGetUniformLocation(currShaderProgram, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(currShaderProgram, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniform3f(glGetUniformLocation(currShaderProgram, "camPosition"), position.x, position.y, position.z);
}