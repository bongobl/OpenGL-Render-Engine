#include "Scene.h"
#include "shader.h"
#include <iostream>

void Scene::initObjects() {

	initThisSceneObjects();

}

void Scene::dispose() {
	disposeThisSceneObjects();
}

void Scene::resize_event(int width, int height) {
	window_width = width;
	window_height = height;


	for (unsigned int i = 0; i < allSceneCameras.size(); ++i) {
		
		if (allSceneCameras.at(i) != NULL) 
			allSceneCameras.at(i)->resize((float)window_width, (float)window_height);
	}
}

void Scene::draw() {


	drawSceneToBuffer();


}

