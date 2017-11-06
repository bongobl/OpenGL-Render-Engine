#pragma once
#include "OBJObject.h"
#include "Node.h"

class GeometryNode : public Node {
	
	OBJObject* model;
	
public:

	GeometryNode(const char* filepath, GLuint sp);
	void draw(glm::mat4 C);
	void setCenter(float x, float y, float z);
	void update();
};