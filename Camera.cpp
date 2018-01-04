#include "Camera.h"

Camera::Camera(glm::vec3 camera_position, float camera_field_of_view_Y) {
	
	//SceneObject Position
	setLocalPosition(camera_position);

	//Camera Mode
	targetMode = false;
	
	//Target Mode
	targetObject = NULL;	
	up = glm::vec3(0,1,0);

	//To define Projection Matrix
	fieldOfViewY = camera_field_of_view_Y;
	width = 1;
	height = 1;
	near = 0.1f;
	far = 100000.0f;

	//update view and projection matrices to reflect camera properties
	updateViewMatrix();
	updateProjectionMatrix();
}
void Camera::update() {

	updateViewMatrix();
}
void Camera::resize(float camera_width, float camera_height) {

	width = camera_width;
	height = camera_height;

	updateProjectionMatrix();
}

void Camera::applySettings(GLuint currShaderProgram) {

	glm::vec3 worldPosition = getPosition(SceneObject::WORLD);

	glUniformMatrix4fv(glGetUniformLocation(currShaderProgram, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(currShaderProgram, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniform3f(glGetUniformLocation(currShaderProgram, "camPosition"), worldPosition.x, worldPosition.y, worldPosition.z);
}

void Camera::setTargetMode(bool target_mode) {
	if (target_mode == true && targetObject == NULL) {
		std::cerr << "No target Scene Object set" << std::endl;
		return;
	}
	targetMode = target_mode;
}
void Camera::setTargetObject(SceneObject* target_object) {
	targetObject = target_object;
}

void Camera::setCameraUp(glm::vec3 camera_up) {
	up = camera_up;
}

void Camera::setFieldOfViewY(float camera_field_of_view_Y) {
	fieldOfViewY = camera_field_of_view_Y;
	updateProjectionMatrix();
}
float Camera::getFieldOfViewY() {
	return fieldOfViewY;
}
void Camera::setNear(float camera_near) {
	near = camera_near;
	updateProjectionMatrix();
}
float Camera::getCameraNear() {
	return near;
}
void Camera::setFar(float camera_far) {
	far = camera_far;
	updateProjectionMatrix();
}
float Camera::getCameraFar() {
	return far;
}

void Camera::draw(Scene* currScene) {
	drawAllChildren(currScene);
}

//Private Helpers
void Camera::updateViewMatrix() {

	//target mode
	if (targetMode && targetObject != NULL) {

		glm::vec3 targetWorldPosition = targetObject->getPosition(SceneObject::WORLD);

		ViewMatrix = glm::lookAt(getPosition(SceneObject::WORLD), targetWorldPosition, up);
		toWorld = glm::inverse(ViewMatrix);
	}

	//non target mode
	if (!targetMode) {
		ViewMatrix = glm::inverse(toWorld);
	}
}

void Camera::updateProjectionMatrix() {

	if (height > 0)
	{
		ProjectionMatrix = glm::perspective(fieldOfViewY, (float)width / (float)height, near, far);
	}
}