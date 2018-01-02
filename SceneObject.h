#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
class Scene;
class SceneObject {

	//Affine transform fields
	glm::vec3 position;
	glm::mat4 rotation;
	glm::vec3 scale;

protected:

	glm::mat4 toWorld;			//global coordinates
	glm::mat4 toParent;			//local coordinates
	glm::mat4 parentToWorld;	//parent's toWorld

	std::vector<SceneObject*> children;

	void updateParentToWorldMatrix(glm::mat4 parent_to_world);
	void drawAllChildren(Scene* currScene);

public:

	SceneObject();
	void setPosition(glm::vec3 pos);
	glm::vec3 getPosition();
	void setRotation(glm::mat4 rot);
	glm::mat4 getRotation();
	void setScale(glm::vec3 sca);
	glm::vec3 getScale();


	void addChild(SceneObject* sceneObject);
	
	virtual void draw(Scene* currScene) = 0;

private:
	void updateAllMatrices();
	
};