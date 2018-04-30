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

		glm::vec3 translation;
		glm::decompose(toWorld, glm::vec3(0,0,0), glm::quat(), translation, glm::vec3(0, 0, 0), glm::vec4(0, 0, 0,0));
		return translation;
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

		glm::quat rotation;
		glm::decompose(toWorld, glm::vec3(0, 0, 0), rotation, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec4(0, 0, 0, 0));
		return glm::toMat4(glm::conjugate(rotation));
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

		glm::vec3 scale;
		glm::decompose(toWorld, scale, glm::quat(), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec4(0, 0, 0, 0));
		return scale;
	}
	else {
		std::cerr << "invalid coordinate space specifier" << std::endl;
		return glm::vec3(0, 0, 0);
	}
}

glm::mat4 SceneObject::getToWorld() const {
	return toWorld;
}
void SceneObject::addChild(SceneObject* newChild) {
	children.push_back(newChild);
	newChild->updateParentToWorldMatrix(toWorld);
}

//Protected, accessible by subclasses
void SceneObject::setToWorld(glm::mat4 newToWorld) {
	toWorld = newToWorld;
	for (unsigned int i = 0; i < children.size(); ++i) {
		children[i]->updateParentToWorldMatrix(toWorld);
	}
}
//Private Helper
void SceneObject::updateParentToWorldMatrix(glm::mat4 parent_to_world) {
	parentToWorld = parent_to_world;
	updateAllMatrices();
}


void SceneObject::updateAllMatrices() {

	toParent = glm::translate(glm::mat4(1.0f), local_position) * local_rotation * glm::scale(glm::mat4(1.0f), local_scale);
	toWorld = parentToWorld * toParent;

	for (unsigned int i = 0; i < children.size(); ++i) {
		children[i]->updateParentToWorldMatrix(toWorld);
	}
}

void SceneObject::drawToShadowMap() {
	sendThisGeometryToShadowMap();
	for (unsigned int i = 0; i < children.size(); ++i) {
		children[i]->drawToShadowMap();
	}

}

void SceneObject::draw(Scene* currScene) {
	drawThisSceneObject(currScene);
	for (unsigned int i = 0; i < children.size(); ++i) {
		children[i]->draw(currScene);
	}
}