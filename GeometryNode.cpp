#include "GeometryNode.h"

GeometryNode::GeometryNode(OBJObject* mod) {
	model = mod;
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