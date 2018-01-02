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
	pointLightDist = 20;
	sceneLights = new Light[2]{
		Light(Light::DIRECTIONAL,glm::vec3(1,1,1), 1, glm::vec3(0,0,0),glm::vec3(-1,0,0)),
		Light(Light::POINT,glm::vec3(1,1,1), 30, glm::vec3(pointLightDist,0,0), glm::vec3(0,0,0))
	};
	pointLightRotationMatrix = glm::mat4(1.0f);
	

	Material graphicMaterial;
	graphicMaterial.setDiffuseColor(glm::vec3(0,0,0));
	graphicMaterial.setUseDiffuse(true);
	graphicMaterial.setAmbientColor(glm::vec3(1, 1, 0));
	graphicMaterial.setUseAmbient(true);
	pointLightGraphic = new Model("Models/Sphere.obj", graphicMaterial);
	sceneLights[1].addChild(pointLightGraphic);

	//init modes
	currEditMode = SampleScene::EDIT_MODEL;
	currActiveLight = 0;

	//INIT SCENE OBJECTS

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

	//TEST OBJECT
	//Load Asteroid Textures
	Texture asteroidTexture;
	asteroidTexture.loadStandardTexture("Textures/AsteroidTexture.ppm");
	Texture normalMapTexture;
	normalMapTexture.loadStandardTexture("Textures/AsteroidNormalMap.ppm");

	//Create Asteroid Material
	Material asteroidMaterial;
	///diffuse
	asteroidMaterial.setDiffuseColor(glm::vec3(0, 1, 1));
	asteroidMaterial.setUseDiffuse(true);

	///specular
	asteroidMaterial.setSpecularColor(glm::vec3(0, 1, 1));
	asteroidMaterial.setUseSpecular(true);

	///ambient
	asteroidMaterial.setAmbientColor(glm::vec3(0, 0.06f, 0.06f));
	asteroidMaterial.setUseAmbient(true);

	///surface texture
	asteroidMaterial.loadSurfaceTexture(asteroidTexture);
	asteroidMaterial.setSurfaceTextureStrength(0.3f);
	asteroidMaterial.setUseSurfaceTexture(true);

	///normal map
	asteroidMaterial.loadNormalMap(normalMapTexture);
	asteroidMaterial.setNormalMapStrength(0.9f);
	//asteroidMaterial.setUseNormalMap(true);

	///reflection texture
	asteroidMaterial.loadReflectionTexture(oceanViewCubeMap);
	asteroidMaterial.setReflectiveness(1.0f);
	asteroidMaterial.setUseReflectionTexture(true);


	testModel = new Model("Models/filletCube.obj", asteroidMaterial);
	testModel->setScale(glm::vec3(7, 7, 7));

	childObject = new Model("Models/Asteroid0.obj", asteroidMaterial);
	testModel->addChild(childObject);
	childObject->setPosition(glm::vec3(7, 7, 0));


	child2 = new Model("Models/Sphere.obj", Material::basic());
	child2->setPosition(glm::vec3(-7, 2, 0));
	child2->setScale(glm::vec3(0.5f,0.5f,0.5f));
	childObject->addChild(child2);

	boundingBox = new BoundingBox(childObject->getVertices());
}
void SampleScene::dispose() {
	delete testModel;
	delete childObject;
	delete mainCam;
	delete[] sceneLights;
}

void SampleScene::update(float deltaTime) {
	
	testModel->setRotation(glm::rotate(glm::mat4(1.0f), deltaTime, glm::vec3(0, 0, 1)) * testModel->getRotation());
	childObject->setRotation(glm::rotate(glm::mat4(1.0f), deltaTime * 3, glm::vec3(0, 0, 1)) * childObject->getRotation());
	oceanView.setPosition(mainCam->position);
	boundingBox->updateToWorld(childObject->getToWorldWithCenteredMesh());
}
void SampleScene::draw() {
	
	testModel->draw(this);	
	sceneLights[currActiveLight].draw(this);
	boundingBox->draw(this);
	oceanView.draw(this);

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

					if (currEditMode == SampleScene::EDIT_MODEL) {

					}
					else if (currEditMode == SampleScene::EDIT_LIGHT) {
						if(currActiveLight == 0)
							sceneLights[currActiveLight].direction = deltaTrackBall * glm::vec4(sceneLights[currActiveLight].direction, 1);
						else if (currActiveLight == 1) {
							pointLightRotationMatrix = deltaTrackBall * pointLightRotationMatrix;
							sceneLights[currActiveLight].setPosition(deltaTrackBall * glm::vec4(sceneLights[currActiveLight].getPosition(), 1));
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
			sceneLights[currActiveLight].setPosition(pointLightRotationMatrix * glm::vec4(pointLightDist, 0, 0,1));
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