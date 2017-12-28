#include "Scene.h"
void Scene::resize_event(int width, int height) {
	window_width = width;
	window_height = height;
	getActiveCamera()->resize((float)window_width, (float)window_height);
}