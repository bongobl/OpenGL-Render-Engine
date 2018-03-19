#include "Scene.h"
#include "shader.h"
#include <iostream>

void Scene::initObjects() {

	initThisScenesObjects();

}

void Scene::dispose() {
	disposeThisScenesObjects();
}

void Scene::resize_event(int width, int height) {
	window_width = width;
	window_height = height;


	for (unsigned int i = 0; i < allSceneCameras.size(); ++i) {
		
		if (allSceneCameras.at(i) != NULL) 
			allSceneCameras.at(i)->resize((float)window_width, (float)window_height);
	}
}

void Scene::update(float deltaTime) {
	updateThisScenesObjects(deltaTime);

	for (unsigned int i = 0; i < allSceneCameras.size(); ++i) {

		allSceneCameras.at(i)->updateViewMatrix();
	}
}
void Scene::draw() {

	drawThisScenesObjects();

}

