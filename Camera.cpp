#include "Camera.h"
#include "Scene.h"
Camera::Camera(glm::vec3 camera_position, float camera_field_of_view_Y) {

	//SceneObject Position
	setLocalPosition(camera_position);

	//Camera Mode
	targetMode = false;

	//Target Mode
	targetObject = NULL;
	up = glm::vec3(0, 1, 0);

	//To define Projection Matrix
	fieldOfViewY = camera_field_of_view_Y;
	width = 1;
	height = 1;
	near = 0.1f;
	far = 100000.0f;

	//Define default blur value
	blurValue = 0;

	//update view and projection matrices to reflect camera properties
	updateViewMatrix();
	updateProjectionMatrix();

	//init gizmos points
	for (unsigned int i = 0; i < 16; ++i) {
		gizmosPoints.push_back(glm::vec3(0, 0, 0));
	}

	//prepare buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, gizmosPoints.size() * sizeof(glm::vec3), gizmosPoints.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Camera::~Camera() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
	

void Camera::resize(float camera_width, float camera_height) {

	width = camera_width;
	height = camera_height;

	updateProjectionMatrix();
	updateGizmos();
}

void Camera::applySettings(GLuint currShaderProgram) {


	glm::vec3 worldPosition = getPosition(SceneObject::WORLD);

	//send camera properties to current shader program
	glUseProgram(currShaderProgram);
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

void Camera::setBlurValue(float camera_blur_value) {
	blurValue = camera_blur_value;
}
float Camera::getBlurValue() {
	return blurValue;
}

void Camera::draw(Scene* currScene) {
	drawAllChildren(currScene);
	drawGizmos(currScene);
}

//Private Helpers
void Camera::updateViewMatrix() {

	//target mode
	if (targetMode && targetObject != NULL) {

		glm::vec3 targetWorldPosition = targetObject->getPosition(SceneObject::WORLD);

		ViewMatrix = glm::lookAt(getPosition(SceneObject::WORLD), targetWorldPosition, up);
		toWorld = glm::inverse(ViewMatrix) * glm::scale(glm::mat4(1.0f), local_scale);
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

void Camera::updateGizmos() {
	//UPDATE GIZMOS POINTS
	glm::vec3 pointTemplate;

	pointTemplate.z = 50; //const
	pointTemplate.x = pointTemplate.z *  tan(fieldOfViewY);
	pointTemplate.y = pointTemplate.x *  ((float)height / (float)width);



	glm::vec3 centerPoint(0, 0, 0);
	glm::vec3 upperRight(pointTemplate.x, pointTemplate.y, -pointTemplate.z);
	glm::vec3 lowerRight(pointTemplate.x, -pointTemplate.y, -pointTemplate.z);
	glm::vec3 upperLeft(-pointTemplate.x, pointTemplate.y, -pointTemplate.z);
	glm::vec3 lowerLeft(-pointTemplate.x, -pointTemplate.y, -pointTemplate.z);


	gizmosPoints[0] = centerPoint;
	gizmosPoints[1] = upperRight;
	gizmosPoints[2] = centerPoint;
	gizmosPoints[3] = lowerRight;
	gizmosPoints[4] = centerPoint;
	gizmosPoints[5] = upperLeft;
	gizmosPoints[6] = centerPoint;
	gizmosPoints[7] = lowerLeft;

	gizmosPoints[8] = upperRight;
	gizmosPoints[9] = upperLeft;
	gizmosPoints[10] = upperLeft;
	gizmosPoints[11] = lowerLeft;
	gizmosPoints[12] = lowerLeft;
	gizmosPoints[13] = lowerRight;
	gizmosPoints[14] = lowerRight;
	gizmosPoints[15] = upperRight;

	//update VBO for new gizmos vertices positions
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, gizmosPoints.size() * sizeof(glm::vec3), gizmosPoints.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Camera::drawGizmos(Scene* currScene) {

	Camera* activeCamera = currScene->getActiveCamera();

	//don't draw gizmos of current camera
	if (activeCamera == this) {
		return;
	}

	glUseProgram(Material::getShaderProgram());

	//apply object properties	
	glUniformMatrix4fv(glGetUniformLocation(Material::getShaderProgram(), "toWorld"), 1, GL_FALSE, &toWorld[0][0]);

	//apply camera properties
	activeCamera->applySettings(Material::getShaderProgram());

	//apply material properties
	Material m;
	m.setSurfaceColor(glm::vec3(0.5, 1, 0.5));
	m.setUseSurfaceColor(true);
	m.applySettings();

	//Bind VAO for gizmos and draw 
	glBindVertexArray(VAO);
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, gizmosPoints.size());
	glBindVertexArray(0);
}