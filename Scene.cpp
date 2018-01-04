#include "Scene.h"
void Scene::resize_event(int width, int height) {
	window_width = width;
	window_height = height;

	for (unsigned int i = 0; i < getAllCameras().size(); ++i) {
		getAllCameras().at(i)->resize((float)window_width, (float)window_height);
	}
}