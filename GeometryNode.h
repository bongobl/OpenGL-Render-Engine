#pragma once
#include "OBJObject.h"
#include "Node.h"

class GeometryNode : public Node {
	
	OBJObject* model;
	
public:

	GeometryNode(OBJObject* mod);
	GeometryNode(const char* filepath, GLuint sp);

	OBJObject* getModel();
	void draw(glm::mat4 C);
	void update();
};