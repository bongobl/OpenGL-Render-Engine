#pragma once
#include "Node.h"
#include "OBJObject.h"
class GeometryNode : public Node {
	
	OBJObject* model;

public:

	GeometryNode(OBJObject* mod);
	void draw(glm::mat4 C) = 0;

};