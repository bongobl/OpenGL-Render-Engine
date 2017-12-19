#include "SampleScene.h"

void SampleScene::initObjects() {
	cam_pos = glm::vec3(0.0f, 0, 30);
	cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f);
	cam_up = glm::vec3(0.0f, 1.0f, 0.0f);

	asteroid = new Asteroid(0);
	asteroid->setRotationSpeed(3);
	asteroid->setScale(glm::vec3(7, 7, 7));

	//vector of skybox face names
	std::vector<std::string> faceNames;
	faceNames.push_back("skybox/left.ppm");
	faceNames.push_back("skybox/right.ppm");
	faceNames.push_back("skybox/top.ppm");
	faceNames.push_back("skybox/bottom.ppm");
	faceNames.push_back("skybox/front.ppm");
	faceNames.push_back("skybox/back.ppm");

	//init cubemap
	spaceView.loadCubeMapTexture(faceNames);
}
void SampleScene::dispose() {
	delete asteroid;
}

void SampleScene::update(float deltaTime) {
	if(glfwGetKey(SceneManager::window, GLFW_KEY_LEFT)) {
		asteroid->setPosition(asteroid->getPosition() + glm::vec3(-30 * deltaTime, 0, 0));
	}
	if(glfwGetKey(SceneManager::window, GLFW_KEY_RIGHT)) {
		asteroid->setPosition(asteroid->getPosition() + glm::vec3(30 * deltaTime, 0, 0));
	}
	if (glfwGetKey(SceneManager::window, GLFW_KEY_UP)) {
		asteroid->setPosition(asteroid->getPosition() + glm::vec3(0,30 * deltaTime, 0));
	}
	if (glfwGetKey(SceneManager::window, GLFW_KEY_DOWN)) {
		asteroid->setPosition(asteroid->getPosition() + glm::vec3(0, -30 * deltaTime, 0));
	}
	if (glfwGetKey(SceneManager::window, GLFW_KEY_Q)) {
		asteroid->setPosition(asteroid->getPosition() + glm::vec3(0, 0, -30 * deltaTime));
	}
	if (glfwGetKey(SceneManager::window, GLFW_KEY_A)) {
		asteroid->setPosition(asteroid->getPosition() + glm::vec3(0, 0, 30 * deltaTime));
	}
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

}
void SampleScene::cursor_position_event(double xpos, double ypos) {

}
void SampleScene::mouse_wheel_event(double xoffset, double yoffset) {
	std::cout << yoffset << std::endl;
}