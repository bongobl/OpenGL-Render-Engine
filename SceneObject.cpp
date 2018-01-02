#include "SceneObject.h"
#include "Scene.h"

SceneObject::SceneObject() {
	position = glm::vec3(0, 0, 0);
	rotation = glm::mat4(1.0f);
	scale = glm::vec3(1, 1, 1);
	parentToWorld = glm::mat4(1.0f);
	updateAllMatrices();
}
void SceneObject::setPosition(glm::vec3 pos) {
	position = pos;
	updateAllMatrices();
}
glm::vec3 SceneObject::getPosition() {
	return position;
}
void SceneObject::setRotation(glm::mat4 rot) {
	rotation = rot;
	updateAllMatrices();
}
glm::mat4 SceneObject::getRotation() {
	return rotation;
}
void SceneObject::setScale(glm::vec3 sca) {
	scale = sca;
	updateAllMatrices();
}
glm::vec3 SceneObject::getScale() {
	return scale;
}

void SceneObject::addChild(SceneObject* sceneObject) {
	children.push_back(sceneObject);
}
void SceneObject::updateParentToWorldMatrix(glm::mat4 parent_to_world) {
	parentToWorld = parent_to_world;
	updateAllMatrices();
}

//Private Helper
void SceneObject::updateAllMatrices() {

	toParent = glm::translate(glm::mat4(1.0f), position) * rotation * glm::scale(glm::mat4(1.0f), scale);
	toWorld = parentToWorld * toParent;

	for (unsigned int i = 0; i < children.size(); ++i) {
		children[i]->updateParentToWorldMatrix(toWorld);
	}

}

void SceneObject::drawAllChildren(Scene* currScene) {
	for (unsigned int i = 0; i < children.size(); ++i) {
		children[i]->draw(currScene);
	}
}