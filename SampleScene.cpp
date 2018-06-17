#include "SampleScene.h"

void SampleScene::initThisScene() {

	//init trackball controls
	isLeftMouseButtonDown = false;
	isRightMouseButtonDown = false;

	//camera rig
	camDist = 120;
	camPos = glm::vec3(0, 0, camDist);
	camRotationMatrix = glm::mat4(1.0f);

	allSceneLights.push_back(new Light(Light::DIRECTIONAL, glm::vec3(1, 1, 1), 0.84f, glm::vec3(0, 30, 50)));
	allSceneLights[0]->setLocalRotation(glm::rotate(glm::mat4(1.0f), 3 * glm::pi<float>() / 4, glm::vec3(1, 0, 0)));
	allSceneLights.push_back(new Light(Light::POINT, glm::vec3(0, 1, 0), 5, 3.3333f * glm::vec3(-20, 20, 30)));
	allSceneLights.push_back(new Light(Light::POINT, glm::vec3(0, 0, 1), 5, 3.3333f * glm::vec3(20, 30, -20)));
	allSceneLights.push_back(new Light(Light::DIRECTIONAL, glm::vec3(1, 1, 1), 0.40f, glm::vec3(0, 50, 30)));
	allSceneLights[3]->setLocalRotation(glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 8.0f, glm::vec3(0, 1, 0)) *  glm::rotate(glm::mat4(1.0f), 3 * glm::pi<float>() / 4.0f, glm::vec3(1, 0, 0)));

	//init camera
	currActiveCamera = new Camera(glm::vec3(0, 0, 120), glm::pi<float>() / 4);
	allSceneCameras.push_back(currActiveCamera);

	//vector of skybox face names
	std::vector<std::string> faceNames;
	faceNames.push_back("skybox/right.ppm");
	faceNames.push_back("skybox/left.ppm");
	faceNames.push_back("skybox/top.ppm");
	faceNames.push_back("skybox/bottom.ppm");
	faceNames.push_back("skybox/back.ppm");
	faceNames.push_back("skybox/front.ppm");

	oceanViewCubeMap.loadCubeMap(faceNames);

	//init cubemap
	oceanView.loadCubeMapTexture(oceanViewCubeMap);

	//Create Basic Material
	Material basicMaterial;
	basicMaterial.setUseDiffuse(true);
	basicMaterial.setDiffuseColor(glm::vec3(1,1,1));
	basicMaterial.setUseAmbient(true);
	basicMaterial.setAmbientColor(glm::vec3(0.1, 0.1, 0.1));

	wall = new Model("Models/Wall.obj", basicMaterial);
	wall->setLocalScale(glm::vec3(0.3, 0.3, 0.3));

	cylinder = new Model("Models/Cylinder.obj", basicMaterial);
	cylinder->setLocalPosition(glm::vec3(50, 60, 35));
	cylinder->getMaterial().setDiffuseColor(glm::vec3(1, 0, 0));
	cylinder->getMaterial().setUseSpecular(true);
	cylinder->getMaterial().setSpecularColor(glm::vec3(1, 1, 1));

	prism = new Model("Models/Prism.obj", basicMaterial);
	prism->getMaterial().setUseSpecular(true);
	prism->getMaterial().setSpecularColor(glm::vec3(1, 1, 1));
	prism->setLocalPosition(glm::vec3(-90, 60, -10));
	prism->getMaterial().loadReflectionTexture(oceanViewCubeMap);
	prism->getMaterial().setUseReflectionTexture(true);

	
	

	//scene hierarchy
	wall->addChild(cylinder);
	wall->addChild(prism);
	wall->addChild(allSceneLights[1]);
	wall->addChild(allSceneLights[2]);
	
	currActiveCamera->setTargetObject(wall);
	currActiveCamera->setTargetMode(true);

}
void SampleScene::disposeThisScene() {


	//delete models & cam gizmos
	delete wall;
	for (unsigned int i = 0; i < allSceneLights.size(); ++i) {
		delete allSceneLights[i];
	}

	//dispose textures
	oceanViewCubeMap.disposeCurrentTexture();
	asteroidTexture.disposeCurrentTexture();
	normalMapTexture.disposeCurrentTexture();

}

void SampleScene::updateThisScene() {

	//wall->setLocalRotation(glm::rotate(glm::mat4(1.0f), SceneManager::getDeltaTime() / 3, glm::vec3(0, 1, 0)) * wall->getRotation(SceneObject::OBJECT));
	allSceneLights[3]->setLocalRotation(glm::rotate(glm::mat4(1.0f), SceneManager::getDeltaTime() / 3, glm::vec3(0, 1, 0)) * allSceneLights[3]->getRotation(SceneObject::OBJECT));
	//prism->setLocalRotation(glm::rotate(glm::mat4(1.0f), deltaTime / 3, glm::vec3(0, 1, 0)) * prism->getRotation(SceneObject::OBJECT));
	//cylinder->setLocalRotation(glm::rotate(glm::mat4(1.0f), deltaTime * 2, glm::vec3(0, 0, 1)) * cylinder->getRotation(SceneObject::OBJECT));

	oceanView.setLocalPosition(getActiveCamera()->getPosition(SceneObject::WORLD));

}

void SampleScene::drawThisSceneToShadowMap() {

	oceanView.drawToShadowMap();
	currActiveCamera->drawToShadowMap();
	wall->drawToShadowMap();
	

	for (GLuint i = 0; i < allSceneLights.size(); ++i) {
		allSceneLights[i]->drawToShadowMap();
	}

}
void SampleScene::drawThisScene() {


	oceanView.draw(this);
	currActiveCamera->draw(this);
	wall->draw(this);
	for (GLuint i = 0; i < allSceneLights.size(); ++i) {
		allSceneLights[i]->draw(this);
	}
	
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
				if (isLeftMouseButtonDown) {
					camRotationMatrix = deltaTrackBall * camRotationMatrix;
					camPos = camRotationMatrix * glm::vec4(0,0,camDist,1);
					currActiveCamera->setLocalPosition(camPos);
				}
				else if (isRightMouseButtonDown) {
					allSceneLights[0]->setLocalRotation(deltaTrackBall * allSceneLights[0]->getRotation(SceneObject::OBJECT));
				}
			}
		}
	}

	//for trackball
	lastPoint = currPoint;
}
void SampleScene::mouse_wheel_event(double xoffset, double yoffset) {

	camDist +=  5 * (float)yoffset;
	camPos = camRotationMatrix * glm::vec4(0, 0, camDist, 1);
	currActiveCamera->setLocalPosition(camPos);

}

Camera* SampleScene::getActiveCamera() {
	return currActiveCamera;
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
	v.z = sqrt(1.001f - d * d);
	v = glm::normalize(v);
	return v;

}