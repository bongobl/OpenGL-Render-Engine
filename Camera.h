#pragma once
#include <glm/gtc/matrix_transform.hpp>
struct Camera {
	
	glm::vec3 position;
	glm::vec3 look_at;
	glm::vec3 up;

	float width;
	float height;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;


	Camera(glm::vec3 camera_position, glm::vec3 camera_look_at, glm::vec3 camera_up, float camera_width, float camera_height);
	void resize(float camera_width, float camera_height);
};