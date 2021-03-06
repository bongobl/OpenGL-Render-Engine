#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
class Scene;
class SceneObject {

protected:
	//Local transform fields
	glm::vec3 local_position;
	glm::mat4 local_rotation;
	glm::vec3 local_scale;


	glm::mat4 toParent;			//local coordinates
	glm::mat4 parentToWorld;	//parent's toWorld
	std::vector<SceneObject*> children;

	glm::mat4 toWorld;			//global coordinates


public:

	enum CoordMode {OBJECT, WORLD};

	SceneObject();
	void setLocalPosition(glm::vec3 pos);
	glm::vec3 getPosition(unsigned int coordinate_space);
	void setLocalRotation(glm::mat4 rot);
	glm::mat4 getRotation(unsigned int coordinate_space);
	void setLocalScale(glm::vec3 sca);
	glm::vec3 getScale(unsigned int coordinate_space);
	glm::mat4 getToWorld() const;

	void addChild(SceneObject* newChild);
	
	void drawToShadowMap();
	void draw(Scene* currScene);

protected:
	void setToWorld(glm::mat4 newToWorld);

private:
	void updateParentToWorldMatrix(glm::mat4 parent_to_world);
	void updateAllMatrices();
	virtual void sendThisGeometryToShadowMap() = 0;
	virtual void drawThisSceneObject(Scene* currScene) = 0;
		
};