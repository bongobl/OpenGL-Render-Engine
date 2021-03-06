#pragma once
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"
#include "SceneObject.h"
class Camera : public SceneObject {

	//Define Camera Mode
	bool targetMode;

	//Target Mode
	SceneObject* targetObject;
	glm::vec3 up;

	//To define Projection Matrix
	float fieldOfViewY;
	float width;
	float height;
	float near;
	float far;

	//blur
	float blurValue;

	//Camera Matrices
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	//Camera gizmos
	std::vector<glm::vec3> gizmosPoints;
	GLuint VAO, VBO;

public:

	Camera(glm::vec3 camera_position, float camera_field_of_view_Y);
	~Camera();

	void updateViewMatrix();
	void resize(float camera_width, float camera_height);
	void applySettings(GLuint currShaderProgram);


	void setTargetMode(bool targetMode);

	//Target Mode
	void setTargetObject(SceneObject* target_object);
	void setCameraUp(glm::vec3 camera_up);

	//To define Projection Matrix
	void setFieldOfViewY(float camera_field_of_view_Y);
	float getFieldOfViewY();
	void setNear(float camera_near);
	float getCameraNear();
	void setFar(float camera_far);
	float getCameraFar();

	//blur settings
	void setBlurValue(float camera_blur_value);
	float getBlurValue();

	//override
	void sendThisGeometryToShadowMap();
	void drawThisSceneObject(Scene* currScene);

private:

	
	void updateProjectionMatrix();
	void updateGizmos();
	void drawGizmos(Scene* currScene);
};