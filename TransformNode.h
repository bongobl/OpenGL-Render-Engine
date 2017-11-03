#pragma once
#include <list>
#include "Node.h"

class TransformNode : public Node {
	glm::mat4 M;
	std::list<Node*> children;

public:

	TransformNode(glm::mat4 m_param); //set M to identity
	void draw(glm::mat4 C) = 0;

	void addChild(Node* newChild);
	void removeChild(Node* childToRemove);

	
};