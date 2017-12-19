#include "Scene.h"

void Scene::resize_event(int width, int height) {

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}