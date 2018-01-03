#include "Camera.h"

Camera::Camera(glm::vec3 camera_position, glm::vec3 camera_look_at, glm::vec3 camera_up, float camera_field_of_view_Y, float camera_width, float camera_height) {
	
	//Camera Mode
	targetMode = false;

	//Non-target Mode
	x_basis = glm::vec4(1, 0, 0, 1);
	y_basis = glm::vec4(0, 1, 0, 1);
	z_basis = glm::vec4(0, 0, 1, 1);
	
	//Target Mode
	targetObject = NULL;
	setLocalPosition(camera_position);
	look_at = camera_look_at;
	up = camera_up;

	//To define Projection Matrix
	fieldOfViewY = camera_field_of_view_Y;
	width = camera_width;
	height = camera_height;
	near = 0.1f;
	far = 100000.0f;

	//update view and projection matrices to reflect camera properties
	updateMatrices();
}
void Camera::update() {

	updateMatrices();
}
void Camera::resize(float camera_width, float camera_height) {

		width = camera_width;
		height = camera_height;
		updateMatrices();	
}

void Camera::applySettings(GLuint currShaderProgram) {

	glm::vec3 worldPosition = getPosition(SceneObject::WORLD);

	glUniformMatrix4fv(glGetUniformLocation(currShaderProgram, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(currShaderProgram, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniform3f(glGetUniformLocation(currShaderProgram, "camPosition"), worldPosition.x, worldPosition.y, worldPosition.z); //TO GLOBAL
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

//Private Helpers
void Camera::updateMatrices() {

	
	if (targetMode && targetObject != NULL) {

		glm::vec3 targetWorldPosition = targetObject->getPosition(SceneObject::WORLD);
		look_at = targetWorldPosition;

		ViewMatrix = glm::lookAt(getPosition(SceneObject::WORLD), look_at, up);	//NOTE TO CHANGE POSITION TO GLOBAL WHEN EXTENDING SceneObject
	}
	if (!targetMode) {

		glm::vec3 x_new = toWorld * x_basis;
		glm::vec3 y_new = toWorld * y_basis;
		glm::vec3 z_new = toWorld * z_basis;

		glm::vec3 position_new = toWorld * glm::vec4(0, 0, 0, 1);
		glm::vec3 look_at_new = z_new;
		glm::vec3 up_new = y_new - position_new;

		ViewMatrix = glm::lookAt(position_new, look_at_new, up_new);
		
		
	}
	
	//ViewMatrix = glm::lookAt(position, look_at, up);

	if (height > 0)
	{
		ProjectionMatrix = glm::perspective(fieldOfViewY, (float)width / (float)height, near, far);
	}
	
}

void Camera::draw(Scene* currScene) {
	//do nothing on purpose
}