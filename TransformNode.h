#pragma once
#include <list>
#include "Node.h"

class TransformNode : public Node {
	
	//this node's total transform matrix
	glm::mat4 M;
	
	glm::vec3 position;
	float scale;
	glm::mat4 rotation;

	std::list<Node*> children;

public:


	TransformNode(); //set M to identity
	~TransformNode();

	void draw(glm::mat4 C);
	void update();

	void addChild(Node* newChild);
	void removeChild(Node* childToRemove);

	void setPosition(glm::vec3 pos);
	void setScale(float s);
	void setRotation(glm::mat4 rot);

	void rotate(glm::mat4 deltaRot);

	glm::mat4 getMatrixM();

private:

	void updateMatrix_M();
	
};