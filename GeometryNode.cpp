#include "GeometryNode.h"

GeometryNode::GeometryNode(OBJObject* mod) {
	model = mod;
}


void GeometryNode::draw(glm::mat4 C) {

	model->setToWorld(C);
	model->draw(glm::vec3(0,0,0));
}
void GeometryNode::update() {

}

OBJObject* GeometryNode::getModel() {
	return model;
}