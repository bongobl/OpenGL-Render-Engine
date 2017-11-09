#pragma once
#include "OBJObject.h"
#include "Node.h"

class GeometryNode : public Node {
	
	OBJObject* model;
	
public:

	GeometryNode(OBJObject* mod);
	
	OBJObject* getModel();
	void draw(glm::mat4 C);
	void update();
};