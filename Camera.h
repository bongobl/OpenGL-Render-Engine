#pragma once
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"
#include "SceneObject.h"
struct Camera : public SceneObject{
	
	//Camera Mode
	bool targetMode;

	//Target Mode
	SceneObject* targetObject;
	glm::vec3 look_at;
	glm::vec3 up;

	//To define Projection Matrix
	float fieldOfViewY;
	float width;
	float height;
	float near;
	float far;

	//Camera Matrices
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	

	Camera(glm::vec3 camera_position, glm::vec3 camera_look_at, glm::vec3 camera_up, float camera_field_of_view_Y, float camera_width, float camera_height);
	void update();
	void resize(float camera_width, float camera_height);
	void applySettings(GLuint currShaderProgram);
	
	void setTargetMode(bool targetMode);
	void setTargetObject(SceneObject* target_object);

	//override
	void draw(Scene* currScene);

private:
	void updateMatrices();
};