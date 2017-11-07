#include "GeometryNode.h"

GeometryNode::GeometryNode(OBJObject* mod) {
	model = mod;
}

GeometryNode::GeometryNode(const char* filepath, GLuint sp) {

	model = new OBJObject(filepath, sp);
}

void GeometryNode::draw(glm::mat4 C) {

	model->setToWorld(C);
	model->draw();
}
void GeometryNode::update() {

}

OBJObject* GeometryNode::getModel() {
	return model;
}