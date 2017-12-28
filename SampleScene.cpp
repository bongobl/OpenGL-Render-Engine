#include "SampleScene.h"

void SampleScene::initObjects() {

	isLeftMouseButtonDown = false;
	isRightMouseButtonDown = false;
	camRotationMatrix = glm::mat4(1.0f);
	camDist = 30.0f;

	mainCam = new Camera(glm::vec3(0, 0, camDist), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1.0f, 0.0f), 0,0);

	asteroid = new Asteroid(1);
	asteroid->setScale(glm::vec3(7, 7, 7));

	//vector of skybox face names
	std::vector<std::string> faceNames;
	faceNames.push_back("skybox/right.ppm");
	faceNames.push_back("skybox/left.ppm");
	faceNames.push_back("skybox/top.ppm");
	faceNames.push_back("skybox/bottom.ppm");
	faceNames.push_back("skybox/back.ppm");
	faceNames.push_back("skybox/front.ppm");

	//init cubemap
	spaceView.loadCubeMapTexture(faceNames);
	
}
void SampleScene::dispose() {
	delete asteroid;
}

void SampleScene::update(float deltaTime) {
	
	spaceView.setPosition(mainCam->position);
	asteroid->update(deltaTime);
	
}
void SampleScene::draw() {
	spaceView.draw(this);
	asteroid->draw(this);
}

//events from callbacks
void SampleScene::key_event(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_E)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(SceneManager::window, GL_TRUE);
		}

	}
}
void SampleScene::mouse_button_event(int button, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			isLeftMouseButtonDown = true;
			lastPoint = trackBallMap(glm::vec2(mousePosition.x, mousePosition.y));

		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			isRightMouseButtonDown = true;
			lastPoint = trackBallMap(glm::vec2(mousePosition.x, mousePosition.y));
		}
	}
	if (action == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			isLeftMouseButtonDown = false;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			isRightMouseButtonDown = false;
		}
	}
}
void SampleScene::cursor_position_event(double xpos, double ypos) {
	
	//define mousePosition for other functions to use
	mousePosition.x = (float)xpos;
	mousePosition.y = (float)ypos;

	glm::vec3 direction;

	//If any mouse button is down, do trackball rotation
	if (isLeftMouseButtonDown || isRightMouseButtonDown) {
		currPoint = trackBallMap(glm::vec2(mousePosition.x, mousePosition.y));
		direction = currPoint - lastPoint;
		float velocity = glm::length(direction);
		if (velocity > 0.0001f) {

			glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
			GLfloat rotAngle = acos(glm::dot(lastPoint, currPoint)) * 1.0f;
			if (!isnan(rotAngle)) {

				glm::mat4 deltaTrackBall = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis);

				if (isRightMouseButtonDown) {
					camRotationMatrix = deltaTrackBall * camRotationMatrix;
					mainCam->position = camRotationMatrix * glm::vec4(0, 0, camDist, 0);
					mainCam->look_at = camRotationMatrix * glm::vec4(0, 0, camDist - 1, 0);
					mainCam->ViewMatrix = glm::lookAt(mainCam->position, mainCam->look_at, mainCam->up);
				}
				else if (isLeftMouseButtonDown) {
					asteroid->updateTrackBall(deltaTrackBall);
				}
			}
		}
	}

	//for trackball
	lastPoint = currPoint;
}
void SampleScene::mouse_wheel_event(double xoffset, double yoffset) {
	camDist += -10 * (float)yoffset;

	mainCam->position = camRotationMatrix * glm::vec4(0, 0, camDist, 0);
	mainCam->look_at = camRotationMatrix * glm::vec4(0, 0, camDist - 1, 0);
	mainCam->ViewMatrix = glm::lookAt(mainCam->position, mainCam->look_at, mainCam->up);
}


Camera* SampleScene::getActiveCamera() {
	return mainCam;
}

//Private Subroutines
glm::vec3 SampleScene::trackBallMap(glm::vec2 point) {
	
	glm::vec3 v;
	GLfloat d;
	v.x = (2.0f * point.x - window_width) / window_width;
	v.y = (window_height - 2.0f * point.y) / window_height;
	v.z = 0;
	d = glm::length(v);
	if (d > 1.0f)
		d = 1.0f;
	v.z = sqrt(1.001f - d*d);
	v = glm::normalize(v);
	return v;

}