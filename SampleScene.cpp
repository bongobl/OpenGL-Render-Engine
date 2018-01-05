#include "SampleScene.h"

void SampleScene::initObjects() {


	//init trackball controls
	isLeftMouseButtonDown = false;
	isRightMouseButtonDown = false;

	//init cameras & set currActiveCamera
	camDist = 160.0f;
	camRotationMatrix = glm::mat4(1.0f);
	mainCam = new Camera(glm::vec3(0, 0, camDist), 45);
	camera2 = new Camera(glm::vec3(0, 0, -100),45);
	camera2->setLocalRotation( glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(0, 1, 0)));
	dullCam = new Camera(glm::vec3(0, 0, 0), 45);
	currActiveCamera = mainCam;

	
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
	asteroidMaterial.setDiffuseColor(glm::vec3(1, 1, 1));
	asteroidMaterial.setUseDiffuse(true);

	///specular
	asteroidMaterial.setSpecularColor(glm::vec3(1, 1, 1));
	asteroidMaterial.setUseSpecular(true);

	///ambient
	asteroidMaterial.setAmbientColor(glm::vec3(0.06f, 0.06f, 0.06f));
	asteroidMaterial.setUseAmbient(true);

	///surface texture
	asteroidMaterial.loadSurfaceTexture(asteroidTexture);
	asteroidMaterial.setSurfaceTextureStrength(1.0f);
	//asteroidMaterial.setUseSurfaceTexture(true);

	///normal map
	asteroidMaterial.loadNormalMap(normalMapTexture);
	asteroidMaterial.setNormalMapStrength(0.4f);
	//asteroidMaterial.setUseNormalMap(true);

	///reflection texture
	asteroidMaterial.loadReflectionTexture(oceanViewCubeMap);
	asteroidMaterial.setReflectiveness(1.0f);
	asteroidMaterial.setUseReflectionTexture(true);


	testModel = new Model("Models/filletCube.obj", asteroidMaterial);
	testModel->setLocalScale(glm::vec3(7, 7, 7));

	childObject = new Model("Models/Asteroid0.obj", asteroidMaterial);
	testModel->addChild(childObject);
	childObject->setLocalPosition(glm::vec3(7, 7, 0));


	child2 = new Model("Models/Sphere.obj", Material::basic());
	child2->setLocalPosition(glm::vec3(-7, 2, 0));
	child2->setLocalScale(glm::vec3(0.7f,0.7f,0.7f));
	child2->getMaterial().setDiffuseColor(glm::vec3(1, 1, 0));
	child2->getMaterial().setUseDiffuse(true);
	child2->getMaterial().setSpecularColor(glm::vec3(1, 1, 1));
	child2->getMaterial().setUseSpecular(true);
	child2->getMaterial().setAmbientColor(glm::vec3(0.06f, 0.06f, 0));
	child2->getMaterial().setUseAmbient(true);
	child2->getMaterial().loadReflectionTexture(oceanViewCubeMap);
	child2->getMaterial().setUseReflectionTexture(true);
	childObject->addChild(child2);
	

	boundingBox = new BoundingBox(testModel->getVertices());

	//test cam
	mainCam->setTargetObject(child2);
	mainCam->setTargetMode(true);
	childObject->addChild(mainCam);
	camera2->setTargetObject(childObject);
	camera2->setTargetMode(true);
}
void SampleScene::dispose() {

	//dispose textures theoretically


	delete testModel;
	delete childObject;
	delete child2;
	delete camera2;
	delete dullCam;
	delete mainCam;
	delete[] sceneLights;
}

void SampleScene::update(float deltaTime) {
	
	
	testModel->setLocalRotation(glm::rotate(glm::mat4(1.0f), deltaTime, glm::vec3(0, 0, 1)) * testModel->getRotation(SceneObject::OBJECT));
	childObject->setLocalRotation(glm::rotate(glm::mat4(1.0f), deltaTime * 3, glm::vec3(0, 0, 1)) * childObject->getRotation(SceneObject::OBJECT));
	oceanView.setLocalPosition(getActiveCamera()->getPosition(SceneObject::WORLD));
	boundingBox->updateToWorld(testModel->getToWorldWithCenteredMesh());
	
	//mainCam->setLocalRotation(glm::rotate(glm::mat4(1.0f), deltaTime, glm::vec3(0, 1, 0)) * mainCam->getRotation(SceneObject::OBJECT));
	
	//Note::update camera last so all objects are up to date
	mainCam->update();
	camera2->update();
}
void SampleScene::draw() {
	
	testModel->draw(this);	
	sceneLights[currActiveLight].draw(this);
	boundingBox->draw(this);
	oceanView.draw(this);

	//Note::You need to draw cameras if they have SceneObject children
	mainCam->draw(this);
	camera2->draw(this);
	dullCam->draw(this);
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
		if (key == GLFW_KEY_SPACE) {
			if (currActiveCamera != NULL) {
				if (currActiveCamera == mainCam) {
					currActiveCamera = camera2;
				}
				else if (currActiveCamera == camera2) {
					currActiveCamera = mainCam;
				}
			}
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
					
					mainCam->setLocalPosition(camRotationMatrix * glm::vec4(0, 0, camDist, 0));	
				}
				else if (isLeftMouseButtonDown) {

					if (currEditMode == SampleScene::EDIT_MODEL) {

					}
					else if (currEditMode == SampleScene::EDIT_LIGHT) {
						if(currActiveLight == 0)
							sceneLights[currActiveLight].setLocalRotation(deltaTrackBall * sceneLights[currActiveLight].getRotation(SceneObject::OBJECT));
						else if (currActiveLight == 1) {
							pointLightRotationMatrix = deltaTrackBall * pointLightRotationMatrix;
							sceneLights[currActiveLight].setLocalPosition(deltaTrackBall * glm::vec4(sceneLights[currActiveLight].getPosition(SceneObject::OBJECT), 1));
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
		mainCam->setLocalPosition(camRotationMatrix * glm::vec4(0, 0, camDist, 0));
	}
	else if (currEditMode == SampleScene::EDIT_LIGHT) {
		if (currActiveLight == SampleScene::POINT_LIGHT) {
			pointLightDist += 3 * (float)yoffset;
			sceneLights[currActiveLight].setLocalPosition(pointLightRotationMatrix * glm::vec4(pointLightDist, 0, 0,1));
		}
	}
	
}

Camera* SampleScene::getActiveCamera() {
	return currActiveCamera;
}

std::vector<Camera*> SampleScene::getAllCameras() {

	std::vector<Camera*> allCams;
	allCams.push_back(mainCam);
	allCams.push_back(camera2);
	allCams.push_back(dullCam);
	return allCams;
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