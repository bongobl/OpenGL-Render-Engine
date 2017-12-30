#include "SampleScene.h"

void SampleScene::initObjects() {

	//init trackball controls
	isLeftMouseButtonDown = false;
	isRightMouseButtonDown = false;
	
	

	//init main camera
	camDist = 30.0f;
	camRotationMatrix = glm::mat4(1.0f);
	mainCam = new Camera(glm::vec3(0, 0, camDist), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1.0f, 0.0f), 0,0);
	
	

	//init lights
	sceneLights = new Light[2]{
		Light(Light::DIRECTIONAL,glm::vec3(1,1,1), 1, glm::vec3(-1,0,0)),
		Light(Light::POINT,glm::vec3(1,1,1), 10, glm::vec3(20,0,0))
	};
	pointLightRotationMatrix = glm::mat4(1.0f);
	pointLightDist = 20;
	pointLightGraphic = new OBJObject("Models/Sphere.obj", Material::basic());

	//init modes
	currEditMode = SampleScene::EDIT_MODEL;
	currActiveLight = 0;

	//INIT SCENE OBJECTS
	asteroid = new Asteroid(3);
	asteroid->setScale(glm::vec3(7, 7, 7));

	//vector of skybox face names
	std::vector<std::string> faceNames;
	faceNames.push_back("skybox/right.ppm");
	faceNames.push_back("skybox/left.ppm");
	faceNames.push_back("skybox/top.ppm");
	faceNames.push_back("skybox/bottom.ppm");
	faceNames.push_back("skybox/back.ppm");
	faceNames.push_back("skybox/front.ppm");
	
	Texture oceanViewCubeMap;
	oceanViewCubeMap.loadCubeMap(faceNames);
	//init cubemap
	oceanView.loadCubeMapTexture(oceanViewCubeMap);
	
}
void SampleScene::dispose() {
	delete asteroid;
	delete mainCam;
	delete[] sceneLights;

}

void SampleScene::update(float deltaTime) {
	
	pointLightGraphic->setToWorld(glm::translate(glm::mat4(1.0f), sceneLights[1].position) * 
									glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)));
	oceanView.setPosition(mainCam->position);
	asteroid->update(deltaTime);
	
}
void SampleScene::draw() {
	if(currActiveLight == 1)
		pointLightGraphic->draw(this);
	oceanView.draw(this);
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
		if (key == GLFW_KEY_1)
		{
			currActiveLight = 0;
		}
		if (key == GLFW_KEY_2)
		{
			currActiveLight = 1;
		}
		if (key == GLFW_KEY_M)
		{
			currEditMode = SampleScene::EDIT_MODEL;
		}
		if (key == GLFW_KEY_L)
		{
			currEditMode = SampleScene::EDIT_LIGHT;
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

					if(currEditMode == SampleScene::EDIT_MODEL)
						asteroid->updateTrackBall(deltaTrackBall);
					else if (currEditMode == SampleScene::EDIT_LIGHT) {
						if(currActiveLight == 0)
							sceneLights[currActiveLight].direction = deltaTrackBall * glm::vec4(sceneLights[currActiveLight].direction, 1);
						else if (currActiveLight == 1) {
							pointLightRotationMatrix = deltaTrackBall * pointLightRotationMatrix;
							sceneLights[currActiveLight].position = deltaTrackBall * glm::vec4(sceneLights[currActiveLight].position, 1);
						}
					}
				}
			}
		}
	}

	//for trackball
	lastPoint = currPoint;
}
void SampleScene::mouse_wheel_event(double xoffset, double yoffset) {
	
	if (currEditMode == SampleScene::EDIT_MODEL) {
		camDist += -5 * (float)yoffset;
		mainCam->position = camRotationMatrix * glm::vec4(0, 0, camDist, 0);
		mainCam->look_at = camRotationMatrix * glm::vec4(0, 0, camDist - 1, 0);
		mainCam->ViewMatrix = glm::lookAt(mainCam->position, mainCam->look_at, mainCam->up);
	}
	else if (currEditMode == SampleScene::EDIT_LIGHT) {
		if (currActiveLight == SampleScene::POINT_LIGHT) {
			pointLightDist += 3 * (float)yoffset;
			sceneLights[currActiveLight].position = pointLightRotationMatrix * glm::vec4(pointLightDist, 0, 0,1);
		}
	}
}


Camera* SampleScene::getActiveCamera() {
	return mainCam;
}

Light* SampleScene::getActiveLight() {
	return &sceneLights[currActiveLight];
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