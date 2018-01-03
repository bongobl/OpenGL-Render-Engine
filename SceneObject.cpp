#include "SceneObject.h"
#include "Scene.h"

SceneObject::SceneObject() {
	local_position = glm::vec3(0, 0, 0);
	local_rotation = glm::mat4(1.0f);
	local_scale = glm::vec3(1, 1, 1);
	parentToWorld = glm::mat4(1.0f);
	updateAllMatrices();
}
void SceneObject::setLocalPosition(glm::vec3 pos) {
	local_position = pos;
	updateAllMatrices();
}
glm::vec3 SceneObject::getPosition(unsigned int coordinate_space) {

	if (coordinate_space == SceneObject::OBJECT) {
		return local_position;
	}
	else if (coordinate_space == SceneObject::WORLD) {
		return parentToWorld * glm::vec4(local_position,1);
	}
	else {
		std::cerr << "invalid coordinate space specifier" << std::endl;
		return glm::vec3(0, 0, 0);
	}
}
void SceneObject::setLocalRotation(glm::mat4 rot) {
	local_rotation = rot;
	updateAllMatrices();
}
glm::mat4 SceneObject::getRotation(unsigned int coordinate_space) {
	if (coordinate_space == SceneObject::OBJECT) {
		return local_rotation;
	}
	else if (coordinate_space == SceneObject::WORLD) {
		return parentToWorld * local_rotation;
	}
	else {
		std::cerr << "invalid coordinate space specifier" << std::endl;
		return glm::mat4(1.0f);
	}
}
void SceneObject::setLocalScale(glm::vec3 sca) {
	local_scale = sca;
	updateAllMatrices();
}
glm::vec3 SceneObject::getScale(unsigned int coordinate_space) {
	if (coordinate_space == SceneObject::OBJECT) {
		return local_scale;
	}
	else if (coordinate_space == SceneObject::WORLD) {
		return parentToWorld * glm::vec4(local_scale, 1);
	}
	else {
		std::cerr << "invalid coordinate space specifier" << std::endl;
		return glm::vec3(0, 0, 0);
	}
}

glm::mat4 SceneObject::getToWorld() {
	return toWorld;
}
void SceneObject::addChild(SceneObject* newChild) {
	children.push_back(newChild);
	newChild->updateParentToWorldMatrix(toWorld);
}
void SceneObject::updateParentToWorldMatrix(glm::mat4 parent_to_world) {
	parentToWorld = parent_to_world;
	updateAllMatrices();
}

//Private Helper
void SceneObject::updateAllMatrices() {

	toParent = glm::translate(glm::mat4(1.0f), local_position) * local_rotation * glm::scale(glm::mat4(1.0f), local_scale);
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